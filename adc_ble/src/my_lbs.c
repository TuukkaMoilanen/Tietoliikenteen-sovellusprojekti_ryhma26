#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include "my_lbs.h"

static bool notify_mysensor_enabled;
static bool indicate_enabled;
static bool button_state;
static struct my_lbs_cb lbs_cb;
static struct bt_gatt_indicate_params ind_params;

static void mylbsbc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	indicate_enabled = (value == BT_GATT_CCC_INDICATE);
}

static void mylbsbc_ccc_mysensor_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	notify_mysensor_enabled = (value == BT_GATT_CCC_NOTIFY);
}

static void indicate_cb(struct bt_conn *conn, struct bt_gatt_indicate_params *params, uint8_t err)
{

}

static ssize_t write_led(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                         const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
{
	if (len != 1U) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (offset != 0) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	if (lbs_cb.led_cb) {
		uint8_t val = *((uint8_t *)buf);
		if (val == 0x00 || val == 0x01) {
		    lbs_cb.led_cb(val ? true : false);
		} else {
		    return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
		}
	}
	return len;
}

static ssize_t read_button(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                           void *buf, uint16_t len, uint16_t offset)
{
	if (lbs_cb.button_cb) {
		button_state = lbs_cb.button_cb();
		return bt_gatt_attr_read(conn, attr, buf, len, offset, &button_state, sizeof(button_state));
	}
	return 0;
}

BT_GATT_SERVICE_DEFINE(my_lbs_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_LBS),
	BT_GATT_CHARACTERISTIC(BT_UUID_LBS_BUTTON,
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_INDICATE,
			               BT_GATT_PERM_READ,
                           read_button, NULL, &button_state),
	BT_GATT_CCC(mylbsbc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

	BT_GATT_CHARACTERISTIC(BT_UUID_LBS_LED,
                           BT_GATT_CHRC_WRITE,
                           BT_GATT_PERM_WRITE,
                           NULL, write_led, NULL),

	BT_GATT_CHARACTERISTIC(BT_UUID_LBS_MYSENSOR,
                           BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_NONE,
                           NULL, NULL, NULL),
	BT_GATT_CCC(mylbsbc_ccc_mysensor_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

int my_lbs_init(struct my_lbs_cb *callbacks)
{
	if (callbacks) {
	    lbs_cb = *callbacks;
	}
	return 0;
}

int my_lbs_send_button_state_indicate(bool state)
{
	if (!indicate_enabled) {
		return -EACCES;
	}

	ind_params.attr = &my_lbs_svc.attrs[2];
	ind_params.func = indicate_cb;
	ind_params.destroy = NULL;
	ind_params.data = &state;
	ind_params.len = sizeof(state);

	return bt_gatt_indicate(NULL, &ind_params);
}

int my_lbs_send_sensor_notify(struct SensorData *data)
{
	if (!notify_mysensor_enabled) {
		return -EACCES;
	}
	return bt_gatt_notify(NULL, &my_lbs_svc.attrs[7], data, sizeof(*data));
}
