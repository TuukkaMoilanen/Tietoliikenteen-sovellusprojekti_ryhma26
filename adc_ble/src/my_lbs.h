#ifndef MY_LBS_H_
#define MY_LBS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/types.h>
#include "adc.h"

#define BT_UUID_LBS_VAL             BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_LBS_BUTTON_VAL      BT_UUID_128_ENCODE(0x00001524, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_LBS_LED_VAL         BT_UUID_128_ENCODE(0x00001525, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_LBS_MYSENSOR_VAL    BT_UUID_128_ENCODE(0x00001526, 0x1212, 0xefde, 0x1523, 0x785feabcd123)

#define BT_UUID_LBS                 BT_UUID_DECLARE_128(BT_UUID_LBS_VAL)
#define BT_UUID_LBS_BUTTON          BT_UUID_DECLARE_128(BT_UUID_LBS_BUTTON_VAL)
#define BT_UUID_LBS_LED             BT_UUID_DECLARE_128(BT_UUID_LBS_LED_VAL)
#define BT_UUID_LBS_MYSENSOR        BT_UUID_DECLARE_128(BT_UUID_LBS_MYSENSOR_VAL)

typedef void (*led_cb_t)(const bool led_state);
typedef bool (*button_cb_t)(void);

struct my_lbs_cb {
	led_cb_t led_cb;
	button_cb_t button_cb;
};

int my_lbs_init(struct my_lbs_cb *callbacks);
int my_lbs_send_button_state_indicate(bool button_state);
int my_lbs_send_sensor_notify(struct SensorData *data);

#ifdef __cplusplus
}
#endif

#endif
