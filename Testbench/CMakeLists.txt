cmake_minimum_required(VERSION 3.28.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(esp32s3_testbench)
target_sources(app PRIVATE
    src/main.c
    src/cmd_parser.c
    src/gpio_handler.c
    src/adc_handler.c
    src/pwm_handler.c
    src/spi_handler.c
)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
