#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cmd_parser.h"
#include "gpio_handler.h"
#include "adc_handler.h"
#include "pwm_handler.h"
#include "spi_handler.h"

static const struct device *const uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

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

    if (!strcmp(line, "ping")) { uart_send("PONG"); return; }
    if (!strcmp(line, "help")) {
        uart_send("Commands: ping, help, gpio config/set/get, adc read <ch>, pwm set <ch> <duty>, spi xfer <hex...>");
        return;
    }

    if (!strcmp(cmd, "gpio")) {
        if (!strcmp(a, "config") ) {
            int pin; char mode[8];
            if (sscanf(line, "gpio config %d %7s", &pin, mode) == 2) {
                int r = gpio_handler_config(pin, !strcmp(mode, "out"));
                uart_send(r == 0 ? "OK" : "ERR 2");
                return;
            }
        }
        if (!strcmp(a, "set")) {
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

    if (!strcmp(cmd, "adc")) {
        if (!strcmp(a, "read")) {
            int ch;
            if (sscanf(line, "adc read %d", &ch) == 1) {
                int v = 0;
                int r = adc_handler_read(ch, &v);
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

    if (!strcmp(cmd, "pwm")) {
        if (!strcmp(a, "set")) {
            int ch, duty;
            if (sscanf(line, "pwm set %d %d", &ch, &duty) == 2) {
                int r = pwm_handler_set(ch, duty);
                uart_send(r == 0 ? "OK" : "ERR 2");
                return;
            }
        }
    }

    if (!strcmp(cmd, "spi")) {
        if (!strcmp(a, "xfer")) {
            /* parse hex bytes after "spi xfer" */
            unsigned char tx[16], rx[16];
            size_t len = 0;
            char *p = line + strlen("spi xfer");
            while (*p && len < sizeof(tx)) {
                while (*p == ' ') p++;
                if (!*p) break;
                unsigned int byte;
                if (sscanf(p, "%2x", &byte) == 1) {
                    tx[len] = (unsigned char)byte;
                    rx[len] = 0;
                    len++;
                    p += 2;
                } else {
                    break;
                }
            }
            if (len > 0) {
                int r = spi_handler_transfer(tx, rx, len);
                if (r == 0) {
                    char resp[64];
                    size_t off = 0;
                    off += snprintf(resp + off, sizeof(resp) - off, "RX");
                    for (size_t i = 0; i < len; i++) {
                        off += snprintf(resp + off, sizeof(resp) - off, " %02X", rx[i]);
                    }
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
