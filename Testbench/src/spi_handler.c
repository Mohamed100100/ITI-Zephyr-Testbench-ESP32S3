#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>

#include "spi_handler.h"

static const struct device *const spi_dev = DEVICE_DT_GET(DT_NODELABEL(spi2));

int spi_handler_transfer(const unsigned char *tx, unsigned char *rx, size_t len)
{
    if (!device_is_ready(spi_dev)) {
        return -1;
    }

    struct spi_config config = {
        .frequency = 1000000,
        .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
        .slave = 0,
        .cs = NULL,
    };

    struct spi_buf tx_buf = { .buf = (void *)tx, .len = len };
    struct spi_buf_set tx_set = { .buffers = &tx_buf, .count = 1 };

    struct spi_buf rx_buf = { .buf = rx, .len = len };
    struct spi_buf_set rx_set = { .buffers = &rx_buf, .count = 1 };

    return spi_transceive(spi_dev, &config, &tx_set, &rx_set);
}
