/*
 * Copyright (c) 2026 ITI Zephyr Playground
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "cmd_parser.h"

int main(void)
{
    cmd_parser_init();

    printk("ESP32-S3 Testbench ready\r\n");

    while (1) {
        cmd_parser_process();
        k_msleep(10);
    }

    return 0;
}
