#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include "adc.h"
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include "confusion.h"

#define USER_LED1 DK_LED1
#define USER_LED2 DK_LED2
#define USER_LED3 DK_LED3
#define USER_LED4 DK_LED4

#define USER_BUTTON_1 DK_BTN1_MSK
#define USER_BUTTON_2 DK_BTN2_MSK
#define USER_BUTTON_3 DK_BTN3_MSK
#define USER_BUTTON_4 DK_BTN4_MSK

#define DEBUG 0

LOG_MODULE_REGISTER(MAIN, LOG_LEVEL_INF);

static int direction = -1;

static void button_changed(uint32_t button_state, uint32_t has_changed)
{
    // Button 1: Print confusion matrix
    if ((has_changed & USER_BUTTON_1) && (button_state & USER_BUTTON_1)) {
        printk("Button 1 pressed: printing Confusion Matrix\n");
        printConfusionMatrix();
    }

    // Button 2: Reset confusion matrix
    if ((has_changed & USER_BUTTON_2) && (button_state & USER_BUTTON_2)) {
        printk("Button 2 pressed: resetting Confusion Matrix\n");
        resetConfusionMatrix();
        printConfusionMatrix();
    }

    // Button 3: Take new measurement (fake or real)
    if ((has_changed & USER_BUTTON_3) && (button_state & USER_BUTTON_3)) {
        #if DEBUG
        printk("Button 3 pressed: running 100 fake classifications\n");
        makeHundredFakeClassifications();
        printConfusionMatrix();
        #else
        direction = (direction + 1) % 6;

        switch (direction) {
            case 0: printk("Direction: x = high\n"); break;
            case 1: printk("Direction: x = low\n"); break;
            case 2: printk("Direction: y = high\n"); break;
            case 3: printk("Direction: y = low\n"); break;
            case 4: printk("Direction: z = high\n"); break;
            case 5: printk("Direction: z = low\n"); break;
            default: printk("Unknown direction!\n"); break;
        }

        struct Measurement m = readADCValue();
        printk("ADC reading -> x: %d, y: %d, z: %d\n", m.x, m.y, m.z);
        #endif
    }

    // Button 4: Classify current measurement
    if ((has_changed & USER_BUTTON_4) && (button_state & USER_BUTTON_4)) {
        if (direction < 0) {
            printk("Button 4 pressed but direction not set! Press Button 3 first.\n");
            return;
        }
        printk("Button 4 pressed: classifying measurement for direction %d\n", direction);
        makeOneClassificationAndUpdateConfusionMatrix(direction);
        printConfusionMatrix();
    }
}

void main(void)
{
    int err;
    err = dk_leds_init();
    if (err) {
        LOG_ERR("LEDs init failed (err %d)\n", err);
        return;
    }

    err = dk_buttons_init(button_changed);
    if (err) {
        printk("Buttons init failed (err: %d)\n", err);
        return;
    }

    if (initializeADC() != 0) {
        printk("ADC initialization failed!\n");
        return;
    }

    printk("System initialized. Waiting for button presses...\n");

    while (1) {
        dk_set_led_on(USER_LED1);
        dk_set_led_on(USER_LED2);
        dk_set_led_on(USER_LED3);
        dk_set_led_on(USER_LED4);
        k_sleep(K_MSEC(1000));

        dk_set_led_off(USER_LED1);
        dk_set_led_off(USER_LED2);
        dk_set_led_off(USER_LED3);
        dk_set_led_off(USER_LED4);
        k_sleep(K_MSEC(1000));
    }
}
