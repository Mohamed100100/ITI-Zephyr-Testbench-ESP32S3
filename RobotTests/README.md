# RobotTests — Robot Framework tests

Robot Framework test suites that drive the ESP32-S3 testbench firmware over
the serial port.

## What it does

Each `.robot` file is a test suite for one peripheral. Tests use readable
keywords provided by the per-peripheral Python libraries:

| File | Library | Keywords |
|---|---|---|
| `tests/smoke.robot` | `CoreLibrary` | `Ping`, `Help` |
| `tests/gpio/gpio.robot` | `GpioLibrary` | `Configure Pin`, `Set Pin`, `Get Pin` |
| `tests/adc/adc.robot` | `AdcLibrary` | `Read Channel` |
| `tests/pwm/pwm.robot` | `PwmLibrary` | `Set Duty` |
| `tests/spi/spi.robot` | `SpiLibrary` | `Transfer` |

Shared serial logic lives in `libraries/serial_common.py`.

## Configure

Edit `config/testbench.yaml`:

```yaml
testbench:
  port: /dev/ttyACM0
  baudrate: 115200
  timeout: 2.0
  prompt: "tb:~$"
```

## Install

```bash
cd RobotTests/
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

## Run

```bash
# all tests
robot tests/

# one group
robot tests/gpio/

# with report folder
robot --outputdir results/ tests/

# dry run (no hardware)
robot --dryrun tests/
```

## Notes

- Close any serial monitor before running tests.
- Wire GPIO7 → GPIO8 for readback tests.
- SPI needs MISO↔MOSI loopback for non-zero readback.
EOF
echo done