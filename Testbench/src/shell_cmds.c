#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <stdlib.h>
#include <stdio.h>

#include "gpio_handler.h"
#include "adc_handler.h"
#include "pwm_handler.h"
#include "spi_handler.h"
#include "uart_handler.h"

static int cmd_ping(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "PONG");
    return 0;
}

static int cmd_hello(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Hello from ESP32-S3!");
    return 0;
}

static int cmd_gpio(const struct shell *sh, size_t argc, char **argv)
{
    if (argc < 2) {
        shell_error(sh, "usage: gpio <config|set|get> ...");
        return -EINVAL;
    }
    if (!strcmp(argv[1], "config") && argc == 4) {
        int pin = atoi(argv[2]);
        int is_out = !strcmp(argv[3], "out");
        int r = gpio_handler_config(pin, is_out);
        shell_print(sh, r == 0 ? "OK" : "ERR %d", r);
        return 0;
    }
    if (!strcmp(argv[1], "set") && argc == 4) {
        int pin = atoi(argv[2]);
        int val = atoi(argv[3]);
        int r = gpio_handler_set(pin, val);
        shell_print(sh, r == 0 ? "OK" : "ERR %d", r);
        return 0;
    }
    if (!strcmp(argv[1], "get") && argc == 3) {
        int pin = atoi(argv[2]);
        int v = 0;
        int r = gpio_handler_get(pin, &v);
        if (r == 0) {
            shell_print(sh, "VAL %d", v);
        } else {
            shell_print(sh, "ERR %d", r);
        }
        return 0;
    }
    shell_error(sh, "bad gpio command");
    return -EINVAL;
}

static int cmd_adc(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 3 && !strcmp(argv[1], "read")) {
        int ch = atoi(argv[2]);
        int v = 0;
        int r = adc_handler_read(ch, &v);
        if (r == 0) {
            shell_print(sh, "VAL %d", v);
        } else {
            shell_print(sh, "ERR %d", r);
        }
        return 0;
    }
    shell_error(sh, "usage: adc read <ch>");
    return -EINVAL;
}

static int cmd_pwm(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 4 && !strcmp(argv[1], "set")) {
        int ch = atoi(argv[2]);
        int duty = atoi(argv[3]);
        int r = pwm_handler_set(ch, duty);
        shell_print(sh, r == 0 ? "OK" : "ERR %d", r);
        return 0;
    }
    shell_error(sh, "usage: pwm set <ch> <0-100>");
    return -EINVAL;
}

static int cmd_spi(const struct shell *sh, size_t argc, char **argv)
{
    if (argc >= 3 && !strcmp(argv[1], "xfer")) {
        unsigned char tx[16], rx[16];
        size_t len = 0;
        for (size_t i = 2; i < argc && len < sizeof(tx); i++) {
            tx[len] = (unsigned char)strtoul(argv[i], NULL, 16);
            rx[len] = 0;
            len++;
        }
        int r = spi_handler_transfer(tx, rx, len);
        if (r == 0) {
            shell_fprintf(sh, SHELL_NORMAL, "RX");
            for (size_t i = 0; i < len; i++) {
                shell_fprintf(sh, SHELL_NORMAL, " %02X", rx[i]);
            }
            shell_fprintf(sh, SHELL_NORMAL, "\n");
        } else {
            shell_print(sh, "ERR %d", r);
        }
        return 0;
    }
    shell_error(sh, "usage: spi xfer <hex...>");
    return -EINVAL;
}


static int cmd_uart(const struct shell *sh, size_t argc, char **argv)
{
    if (argc >= 3 && !strcmp(argv[1], "send")) {
        /* join argv[2..] into one string */
        char buf[128];
        size_t off = 0;
        for (size_t i = 2; i < argc && off < sizeof(buf) - 1; i++) {
            if (i > 2 && off < sizeof(buf) - 1) buf[off++] = ' ';
            size_t l = strlen(argv[i]);
            if (off + l >= sizeof(buf)) l = sizeof(buf) - off - 1;
            memcpy(buf + off, argv[i], l);
            off += l;
        }
        buf[off] = '\0';
        int r = uart_handler_send(buf);
        shell_print(sh, r == 0 ? "OK" : "ERR %d", r);
        return 0;
    }
    if (argc == 2 && !strcmp(argv[1], "recv")) {
        char buf[128];
        int n = uart_handler_recv(buf, sizeof(buf));
        if (n >= 0) {
            shell_print(sh, "RX %s", buf);
        } else {
            shell_print(sh, "ERR %d", n);
        }
        return 0;
    }
    shell_error(sh, "usage: uart send <text...> | uart recv");
    return -EINVAL;
}

SHELL_CMD_REGISTER(ping, NULL, "Liveness check", cmd_ping);
SHELL_CMD_REGISTER(hello, NULL, "Hello message", cmd_hello);
SHELL_CMD_REGISTER(gpio, NULL, "GPIO: config/set/get", cmd_gpio);
SHELL_CMD_REGISTER(adc, NULL, "ADC: read <ch>", cmd_adc);
SHELL_CMD_REGISTER(pwm, NULL, "PWM: set <ch> <0-100>", cmd_pwm);
SHELL_CMD_REGISTER(spi, NULL, "SPI: xfer <hex...>", cmd_spi);
SHELL_CMD_REGISTER(uart, NULL, "UART1: send/recv", cmd_uart);
