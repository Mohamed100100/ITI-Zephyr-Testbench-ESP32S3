#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#include "gpio_handler.h"

static const struct device *const gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

int gpio_handler_config(int pin, int is_out)
{
    gpio_flags_t flags = is_out ? GPIO_OUTPUT_INACTIVE : GPIO_INPUT;
    return gpio_pin_configure(gpio_dev, pin, flags);
}

int gpio_handler_set(int pin, int value)
{
    return gpio_pin_set(gpio_dev, pin, value);
}

int gpio_handler_get(int pin, int *value)
{
    int v = gpio_pin_get(gpio_dev, pin);
    if (v < 0) {
        return v;
    }
    *value = v;
    return 0;
}
