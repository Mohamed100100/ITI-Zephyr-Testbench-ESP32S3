/*
 * Copyright (c) 2026 ITI Zephyr Playground
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    printk("ESP32-S3 Testbench ready (shell on USB Serial/JTAG)\r\n");
    while (1) {
        k_msleep(1000);
    }
    return 0;
}
