#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include "adc.h"

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
    !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
    ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)
};

void printDebugInfo(void)
{
    printk("Printing first adc_dt_spec structure contents for all channels\n");
    for (int i = 0; i < 3; i++) {
        printk("ADC channel %d:\n", i);
        printk("  Device pointer: %p\n", adc_channels[i].dev);
        printk("  Channel ID: %d\n", adc_channels[i].channel_id);
        printk("  Voltage reference: %d\n", adc_channels[i].vref_mv);
        printk("  Resolution: %d\n", adc_channels[i].resolution);
        printk("  Oversampling: %d\n", adc_channels[i].oversampling);
        printk("\n");
    }
}

int initializeADC(void)
{
    int err;

    for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
        if (!device_is_ready(adc_channels[i].dev)) {
            printk("ADC controller device not ready\n");
            return -1;
        }

        err = adc_channel_setup_dt(&adc_channels[i]);
        if (err < 0) {
            printk("Could not setup channel #%d (%d)\n", i, err);
            return -1;
        }
    }
    return 0;
}

struct Measurement readADCValue(void)
{
    struct Measurement m = {0, 0, 0};
    int16_t raw;
    int32_t val_mv;
    struct adc_sequence sequence = {
        .buffer = &raw,
        .buffer_size = sizeof(raw),
    };

    for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
        (void)adc_sequence_init_dt(&adc_channels[i], &sequence);

        int err = adc_read(adc_channels[i].dev, &sequence);
        if (err < 0) {
            printk("ADC read failed for channel %d (%d)\n", i, err);
            continue;
        }

        val_mv = raw;
        err = adc_raw_to_millivolts_dt(&adc_channels[i], &val_mv);
        if (err < 0) {
            printk("Could not convert raw value to mV for channel %d\n", i);
            val_mv = raw; // fallback
        }

        switch (i) {
            case 0: m.x = val_mv; break;
            case 1: m.y = val_mv; break;
            case 2: m.z = val_mv; break;
        }
    }

    return m;
}
