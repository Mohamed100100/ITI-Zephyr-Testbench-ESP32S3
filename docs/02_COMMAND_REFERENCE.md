# Command Reference

All commands are ASCII lines terminated by \\r\\n. Replies are single lines.

| Command | Reply | Notes |
|---|---|---|
| `ping` | `PONG` | liveness check |
| `help` | `Commands: ...` | list |
| `gpio config <pin> <in\|out>` | `OK` / `ERR 2` | configure pin |
| `gpio set <pin> <0\|1>` | `OK` / `ERR 2` | drive output |
| `gpio get <pin>` | `VAL <0\|1>` / `ERR 2` | read pin |
| (unknown) | `ERR 1` | bad command |

ERR 1 = unknown command, ERR 2 = bad arguments / hardware error.

| `adc read <ch>` | `VAL <raw>` / `ERR 2` | read ADC channel |
| `pwm set <ch> <0-100>` | `OK` / `ERR 2` | set PWM duty % |
| `spi xfer <hex...>` | `RX <hex...>` / `ERR 2` | SPI transfer |
