#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

#include "pwm_handler.h"

static const struct device *const pwm_dev = DEVICE_DT_GET(DT_NODELABEL(ledc0));

int pwm_handler_set(int channel, int duty_percent)
{
    if (!device_is_ready(pwm_dev)) {
        return -1;
    }
    if (duty_percent < 0) duty_percent = 0;
    if (duty_percent > 100) duty_percent = 100;

    /* period = 1 MHz clock / 1000 = 1 kHz PWM; pulse = duty% of period */
    uint32_t period = 1000000; /* cycles */
    uint32_t pulse = (period * duty_percent) / 100;

    return pwm_set_cycles(pwm_dev, channel, period, pulse, 0);
}
