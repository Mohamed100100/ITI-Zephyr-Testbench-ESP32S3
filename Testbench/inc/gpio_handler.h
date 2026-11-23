#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

int gpio_handler_config(int pin, int is_out);
int gpio_handler_set(int pin, int value);
int gpio_handler_get(int pin, int *value);

#endif
