#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <dk_buttons_and_leds.h>
#include "my_lbs.h"
#include "adc.h"

static const struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
	(BT_LE_ADV_OPT_CONN | BT_LE_ADV_OPT_USE_IDENTITY),
	800, 801, NULL);

#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)

#define RUN_STATUS_LED          DK_LED1
#define CON_STATUS_LED          DK_LED2
#define USER_LED                DK_LED3
#define USER_BUTTON             DK_BTN1_MSK
#define ORIENTATION_BUTTON      DK_BTN2_MSK

#define STACKSIZE               1024
#define PRIORITY                7
#define RUN_LED_BLINK_INTERVAL  1000
#define NOTIFY_INTERVAL         1000

static bool app_button_state;
static uint16_t orientation = 0;
static struct k_work adv_work;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_LBS_VAL),
};

static void adv_work_handler(struct k_work *work)
{
	int err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) return;
}

static void advertising_start(void) { k_work_submit(&adv_work); }

static void recycled_cb(void) { advertising_start(); }

static void app_led_cb(bool led_state) { dk_set_led(USER_LED, led_state); }

static bool app_button_cb(void) { return app_button_state; }

void send_data_thread(void)
{
	while (1) {
	    struct SensorData data = readADCValue(orientation);
		my_lbs_send_sensor_notify(&data);

		printk("x=%d, y=%d, z=%d, Orientation=%d\n", data.x, data.y, data.z, data.orientation);
        k_sleep(K_MSEC(NOTIFY_INTERVAL));
	}
}

static struct my_lbs_cb app_callbacks = {
	.led_cb = app_led_cb,
	.button_cb = app_button_cb,
};

static void button_changed(uint32_t button_state, uint32_t has_changed)
{
	if (has_changed & USER_BUTTON) {
		uint32_t user_button_state = button_state & USER_BUTTON;
		my_lbs_send_button_state_indicate(user_button_state);
		app_button_state = user_button_state ? true : false;
	}

    if (has_changed & ORIENTATION_BUTTON) {
        if (button_state & ORIENTATION_BUTTON) {
            orientation = (orientation + 1) % 6;
        }
    }
}
static void on_connected(struct bt_conn *conn, uint8_t err)
{
	dk_set_led_on(CON_STATUS_LED);
    if (err) return;
}

static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	dk_set_led_off(CON_STATUS_LED);
}

struct bt_conn_cb connection_callbacks = {
	.connected = on_connected,
	.disconnected = on_disconnected,
	.recycled = recycled_cb,
};

static int init_button(void)
{
	return dk_buttons_init(button_changed);
}

int main(void)
{
	bool led_on = false;
	int err;

	err = dk_leds_init();
	if (err) return -1;

	if (init_button()) return -1;

	if (bt_enable(NULL)) return -1;
	bt_conn_cb_register(&connection_callbacks);

	if (my_lbs_init(&app_callbacks)) return -1;

    if (initializeADC() != 0) return -1;

	k_work_init(&adv_work, adv_work_handler);
	advertising_start();

	while (1) {
        led_on = !led_on;
		dk_set_led(RUN_STATUS_LED, led_on);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
	}
}

K_THREAD_DEFINE(send_data_thread_id, STACKSIZE, send_data_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
