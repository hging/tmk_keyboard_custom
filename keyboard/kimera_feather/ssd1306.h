#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

#define SSD1306_ADDR (0x3C<<1)
#define SSD1306_LCD_WIDTH 128
#define SSD1306_LCD_HEIGHT 32
#define SSD1306_BUFFER_SIZE (SSD1306_LCD_WIDTH * SSD1306_LCD_HEIGHT / 8)

enum {
    SSD1306_COMM_DISPLAYOFF  = 0xAE,
    SSD1306_COMM_DISPLAYON   = 0xAF,
    SSD1306_COMM_COLUMNADDR  = 0x21,
    SSD1306_COMM_PAGEADDR    = 0x22
};

uint8_t ssd1306_init(void);
uint8_t ssd1306_demo(void);
uint8_t ssd1306_command(uint8_t command);
uint8_t ssd1306_data(uint8_t data);

#endif
