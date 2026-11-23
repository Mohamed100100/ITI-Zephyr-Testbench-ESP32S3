# Hardware / Pin Map (ESP32-S3 Mini)

- Console: USB Serial/JTAG (GPIO19/20) -> /dev/ttyACM0, 115200
- Test GPIOs: 7 (output), 8 (input) (loopback: connect GPIO7 -> GPIO8 for readback tests)
- ADC: GPIO1 (ADC1_CH0)
- PWM: GPIO2
- SPI: use default SPI2 pins (MOSI 11, MISO 13, SCK 12, CS 10)
- UART (extra): UART1 on GPIO17/18
