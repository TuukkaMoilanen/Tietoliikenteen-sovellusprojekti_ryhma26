#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include "adc.h"

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)
};

int initializeADC(void)
{
    int err;

	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
	    if (!device_is_ready(adc_channels[i].dev)) {
			return -ENODEV;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			return err;
		}
	}
    return 0;
}

struct SensorData readADCValue(uint16_t orientation)
{
    struct SensorData s = { .orientation = orientation };
	int16_t raw_buf;
    struct adc_sequence seq = {
	  .buffer = &raw_buf,
	  .buffer_size = sizeof(raw_buf),
    };

	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		int err;
        int32_t val_mv = 0;

		(void)adc_sequence_init_dt(&adc_channels[i], &seq);
		err = adc_read(adc_channels[i].dev, &seq);
		if (err < 0) continue;

		val_mv = raw_buf;
		err = adc_raw_to_millivolts_dt(&adc_channels[i],&val_mv);
		if (err < 0) continue;

        switch (i) {
            case 0: s.x = val_mv; break;
            case 1: s.y = val_mv; break;
            case 2: s.z = val_mv; break;
            default: break;
        }
	}
    return s;
}
