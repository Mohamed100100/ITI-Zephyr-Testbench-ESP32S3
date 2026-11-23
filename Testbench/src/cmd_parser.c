#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cmd_parser.h"
#include "gpio_handler.h"

static const struct device *const uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
static const struct device *const gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

static char line_buf[128];
static size_t line_len = 0;

static void uart_send(const char *s)
{
    while (*s) {
        uart_poll_out(uart_dev, *s++);
    }
    uart_poll_out(uart_dev, '\r');
    uart_poll_out(uart_dev, '\n');
}

static void handle_line(char *line)
{
    char cmd[16] = {0}, a[16] = {0}, b[16] = {0};
    int n = sscanf(line, "%15s %15s %15s", cmd, a, b);

    if (!strcmp(line, "ping")) {
        uart_send("PONG");
        return;
    }
    if (!strcmp(line, "help")) {
        uart_send("Commands: ping, help, gpio config <pin> <in|out>, gpio set <pin> <0|1>, gpio get <pin>");
        return;
    }

    if (!strcmp(cmd, "gpio")) {
        if (!strcmp(a, "config") && n == 3) {
            int pin = atoi(b);
            /* mode is in b? re-parse: gpio config <pin> <mode> */
            char mode[8];
            if (sscanf(line, "gpio config %d %7s", &pin, mode) == 2) {
                int is_out = !strcmp(mode, "out");
                int r = gpio_handler_config(pin, is_out);
                uart_send(r == 0 ? "OK" : "ERR 2");
                return;
            }
        }
        if (!strcmp(a, "set") ) {
            int pin, val;
            if (sscanf(line, "gpio set %d %d", &pin, &val) == 2) {
                int r = gpio_handler_set(pin, val);
                uart_send(r == 0 ? "OK" : "ERR 2");
                return;
            }
        }
        if (!strcmp(a, "get")) {
            int pin;
            if (sscanf(line, "gpio get %d", &pin) == 1) {
                int v = 0;
                int r = gpio_handler_get(pin, &v);
                if (r == 0) {
                    char resp[32];
                    snprintf(resp, sizeof(resp), "VAL %d", v);
                    uart_send(resp);
                } else {
                    uart_send("ERR 2");
                }
                return;
            }
        }
    }

    uart_send("ERR 1");
}

void cmd_parser_init(void)
{
    if (!device_is_ready(uart_dev)) {
        return;
    }
    if (!device_is_ready(gpio_dev)) {
        return;
    }
}

void cmd_parser_process(void)
{
    unsigned char c;
    int ret = uart_poll_in(uart_dev, &c);
    if (ret == 0) {
        if (c == '\r' || c == '\n') {
            if (line_len > 0) {
                line_buf[line_len] = '\0';
                handle_line(line_buf);
                line_len = 0;
            }
        } else if (line_len < sizeof(line_buf) - 1) {
            line_buf[line_len++] = (char)c;
        }
    }
}
