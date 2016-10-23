#ifndef BLUEFRUITLE_SPI_H
#define BLUEFRUITLE_SPI_H

#include <stdint.h>
#include <stdbool.h>

void serial_init(void);
void serial_send(uint8_t c);
int16_t serial_recv2(void);

#endif
