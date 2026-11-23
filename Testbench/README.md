# Testbench — Zephyr firmware (ESP32-S3 Mini)

Zephyr RTOS firmware for the WeAct ESP32-S3 Mini. It exposes hardware
peripherals through a **Zephyr shell** over USB Serial/JTAG so Robot
Framework (or a human in a serial terminal) can drive them.

## What it does

On boot it prints `ESP32-S3 Testbench ready (shell on USB Serial/JTAG)` and
presents a prompt `tb:~$`. Available commands:

| Command | Description |
|---|---|
| `ping` | Liveness check → `PONG` |
| `help` | List commands |
| `hello` | `Hello from ESP32-S3!` |
| `gpio config <pin> <in\|out>` | Configure a GPIO |
| `gpio set <pin> <0\|1>` | Drive an output |
| `gpio get <pin>` | Read a pin → `VAL 0/1` |
| `adc read <ch>` | Read ADC channel → `VAL <raw>` |
| `pwm set <ch> <0-100>` | Set PWM duty % |
| `spi xfer <hex...>` | SPI transfer → `RX <hex...>` |

## Build

```bash
cd Testbench/
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr
west build -p always -b weact_esp32s3_mini/esp32s3/procpu -d build .
```

## Flash

```bash
west flash -d build
```

## Test manually

```bash
python -m serial.tools.miniterm /dev/ttyACM0 115200
# tb:~$ ping
# PONG
```

## Files

- `CMakeLists.txt` — build definition
- `prj.conf` — enabled subsystems (GPIO, serial, shell, ADC, PWM, SPI)
- `boards/weact_esp32s3_mini_procpu.overlay` — enables spi2/adc0/adc1/ledc0
- `src/main.c` — prints banner, sleeps (shell runs in its own thread)
- `src/shell_cmds.c` — shell command handlers
- `src/*_handler.c` — peripheral drivers (GPIO, ADC, PWM, SPI)
- `inc/*.h` — handler headers
EOF
echo done