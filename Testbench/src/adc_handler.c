#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>

#include "adc_handler.h"

static const struct device *const adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc0));

int adc_handler_read(int channel, int *value)
{
    if (!device_is_ready(adc_dev)) {
        return -1;
    }

    struct adc_channel_cfg cfg = {
        .gain = ADC_GAIN_1,
        .reference = ADC_REF_INTERNAL,
        .acquisition_time = ADC_ACQ_TIME_DEFAULT,
        .channel_id = channel,
    };

    int err = adc_channel_setup(adc_dev, &cfg);
    if (err < 0) {
        return err;
    }

    int16_t buf = 0;
    struct adc_sequence seq = {
        .channels = BIT(channel),
        .buffer = &buf,
        .buffer_size = sizeof(buf),
        .resolution = 12,
    };

    err = adc_read(adc_dev, &seq);
    if (err < 0) {
        return err;
    }

    *value = buf;
    return 0;
}
