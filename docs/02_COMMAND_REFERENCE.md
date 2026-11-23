# Command Reference

The firmware uses the **Zephyr shell** over USB Serial/JTAG. You get a
prompt `tb:~$`, command history, and `help`/`history` for free.

All commands are ASCII lines terminated by `\n` (or `\r\n`). Replies are
single lines.

## General

| Command | Reply | Notes |
|---|---|---|
| `ping` | `PONG` | liveness check |
| `hello` | `Hello from ESP32-S3!` | greeting |
| `help` | `Commands: ...` | list all commands |
| (unknown) | `ERR 1` | bad command |

## GPIO

| Command | Reply | Notes |
|---|---|---|
| `gpio config <pin> <in\|out>` | `OK` / `ERR 2` | configure pin direction |
| `gpio set <pin> <0\|1>` | `OK` / `ERR 2` | drive an output |
| `gpio get <pin>` | `VAL <0\|1>` / `ERR 2` | read a pin |

## ADC

| Command | Reply | Notes |
|---|---|---|
| `adc read <ch>` | `VAL <raw>` / `ERR 2` | read ADC channel (0-9) |

## PWM

| Command | Reply | Notes |
|---|---|---|
| `pwm set <ch> <0-100>` | `OK` / `ERR 2` | set PWM duty % |

## SPI

| Command | Reply | Notes |
|---|---|---|
| `spi xfer <hex...>` | `RX <hex...>` / `ERR 2` | SPI transfer (MOSI 11, MISO 13, SCK 12, CS 10) |

## UART (extra, UART1 on GPIO17/18)

| Command | Reply | Notes |
|---|---|---|
| `uart send <text...>` | `OK` / `ERR 2` | write text to UART1 |
| `uart recv` | `RX <text>` / `ERR 2` | read available bytes from UART1 |

## Error codes

- `ERR 1` = unknown command
- `ERR 2` = bad arguments or hardware error
