#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

#define SSD1306_ADDR (0x3C<<1)
#define SSD1306_LCD_WIDTH 128
#define SSD1306_LCD_HEIGHT 32
#define SSD1306_BUFFER_SIZE (SSD1306_LCD_WIDTH * SSD1306_LCD_HEIGHT / 8)

enum {
    SSD1306_COMM_DISPLAY_OFF                          = 0xAE,
    SSD1306_COMM_DISPLAY_ON                           = 0xAF,
    SSD1306_COMM_COLUMN_ADDR                          = 0x21,
    SSD1306_COMM_PAGE_ADDR                            = 0x22,

    SSD1306_COMM_ACTIVATE_SCROLL                      = 0x2F,
    SSD1306_COMM_DEACTIVATE_SCROLL                    = 0x2E,
    SSD1306_COMM_SET_VERTICAL_SCROLL_AREA             = 0xA3,
    SSD1306_COMM_RIGHT_HORIZONTAL_SCROLL              = 0x26,
    SSD1306_COMM_LEFT_HORIZONTAL_SCROLL               = 0x27,
    SSD1306_COMM_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29,
    SSD1306_COMM_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  = 0x2A
};

uint8_t ssd1306_init(void);
uint8_t ssd1306_demo(void);
void ssd1306_start_scroll_left(uint8_t start, uint8_t stop);
void ssd1306_start_scroll_right(uint8_t start, uint8_t stop);
void ssd1306_stop_scroll(void);

#endif
