#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

#include "uart_handler.h"

static const struct device *const uart1_dev = DEVICE_DT_GET(DT_NODELABEL(uart1));

int uart_handler_send(const char *text)
{
    if (!device_is_ready(uart1_dev)) {
        return -1;
    }
    for (size_t i = 0; text[i] != '\0'; i++) {
        uart_poll_out(uart1_dev, text[i]);
    }
    uart_poll_out(uart1_dev, '\r');
    uart_poll_out(uart1_dev, '\n');
    return 0;
}

int uart_handler_recv(char *buf, size_t bufsize)
{
    if (!device_is_ready(uart1_dev)) {
        return -1;
    }
    size_t n = 0;
    unsigned char c;
    /* read for up to ~100ms */
    int64_t deadline = k_uptime_get() + 100;
    while (n < bufsize - 1 && k_uptime_get() < deadline) {
        if (uart_poll_in(uart1_dev, &c) == 0) {
            buf[n++] = (char)c;
            deadline = k_uptime_get() + 50; /* extend a bit while data flows */
        } else {
            k_msleep(1);
        }
    }
    buf[n] = '\0';
    return (int)n;
}
