# Getting Started

## 1. Build and flash firmware

```bash
cd Testbench/
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr
west build -p always -b weact_esp32s3_mini/esp32s3/procpu -d build .
west flash -d build
```

## 2. Verify with serial terminal

```bash
python -m serial.tools.miniterm /dev/ttyACM0 115200
# type: ping  -> PONG
```

## 3. Install Python deps

```bash
cd RobotTests/
pip install -r requirements.txt
```

## 4. Run tests

```bash
robot tests/
robot tests/gpio/
robot --outputdir results/ tests/
```
