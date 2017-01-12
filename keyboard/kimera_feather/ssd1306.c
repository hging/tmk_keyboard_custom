#include <util/delay.h>
#include <avr/pgmspace.h>
#include "ssd1306.h"
#include "i2cmaster.h"
#include "debug.h"

#ifdef OLED_ENABLE

#define SSD1306_128_32

#define SSD1306_INIT_TABLE_LENGTH (sizeof(ssd1306_init_table)/sizeof(ssd1306_init_table[0]))
static const uint8_t ssd1306_init_table[] PROGMEM = {
    0xAE,
    0xD5, 0x80,
    0xA8, SSD1306_LCD_HEIGHT - 1,
    0xD3, 0x00,
    0x40,
    0x8D, 0x14,
    0x20, 0x00,
    0xA1,
    0xC8,
#if defined SSD1306_128_32
    0xDA, 0x02,
    0x81, 0x8F,
#elif defined SSD1306_128_64
    0xDA, 0x12,
    0x81, 0xCF,
#endif
    0xD9, 0xF1,
    0xDB, 0x40,
    0xA4,
    0xA6,
    0x2E,
    0xAF
};

#define SSD1306_DEMO_TABLE_LENGTH (sizeof(ssd1306_demo_table)/sizeof(ssd1306_demo_table[0]))
static const uint8_t ssd1306_demo_table[] PROGMEM = {
#if 1
    '\x00','\x00','\x62','\x80','\x80','\x03','\xc0','\xc0', // 0x0008
    '\x02','\xe0','\xe0','\x03','\xf0','\xf0','\x02','\xf8', // 0x0010
    '\xf8','\x02','\xfc','\xfc','\x02','\x7e','\x1e','\x07', // 0x0018
    '\x00','\x00','\x19','\xf0','\xf0','\x02','\x00','\x00', // 0x0020
    '\x05','\x80','\xc0','\x60','\x30','\x10','\x00','\x00', // 0x0028
    '\x02','\x30','\x30','\x02','\x00','\x00','\x38','\xf8', // 0x0030
    '\xf8','\x02','\xfc','\xfc','\x03','\x3c','\xbc','\xbe', // 0x0038
    '\x9e','\x9e','\x02','\x9f','\x9f','\x05','\xdf','\xcf', // 0x0040
    '\xcf','\x05','\xe7','\xe7','\x02','\x63','\x33','\x11', // 0x0048
    '\x09','\x00','\x00','\x1d','\xff','\xff','\x02','\x30', // 0x0050
    '\x18','\x0c','\x1e','\x7b','\xe1','\xc0','\x80','\x00', // 0x0058
    '\x00','\x04','\xfe','\xfe','\x02','\x00','\x00','\x03', // 0x0060
    '\xfe','\xfe','\x02','\x0c','\x06','\x03','\x03','\x02', // 0x0068
    '\x07','\xfe','\xfc','\x06','\x02','\x03','\x03','\x02', // 0x0070
    '\x06','\xfe','\xf8','\x00','\x00','\x02','\xf0','\xfc', // 0x0078
    '\x4e','\x62','\x63','\x63','\x02','\x62','\x66','\x7c', // 0x0080
    '\x78','\x00','\x00','\x03','\xfe','\xfc','\x06','\x03', // 0x0088
    '\x03','\x02','\x02','\x00','\x8c','\xce','\x46','\x43', // 0x0090
    '\x63','\x63','\x02','\x22','\xfe','\xfc','\x00','\x00', // 0x0098
    '\x04','\xc0','\xfe','\xff','\xff','\x03','\x1f','\x80', // 0x00a0
    '\xe0','\xe7','\xe7','\x08','\xf7','\xf7','\x02','\xf3', // 0x00a8
    '\x73','\x73','\x02','\x31','\x19','\x08','\x00','\x00', // 0x00b0
    '\x22','\x0f','\x0f','\x02','\x00','\x00','\x06','\x03', // 0x00b8
    '\x07','\x0e','\x0c','\x08','\x00','\x0f','\x0f','\x02', // 0x00c0
    '\x00','\x00','\x03','\x0f','\x0f','\x02','\x00','\x00', // 0x00c8
    '\x05','\x0f','\x0f','\x02','\x00','\x00','\x05','\x0f', // 0x00d0
    '\x0f','\x02','\x00','\x00','\x02','\x01','\x07','\x0e', // 0x00d8
    '\x0c','\x08','\x08','\x02','\x0c','\x0c','\x02','\x07', // 0x00e0
    '\x03','\x00','\x00','\x03','\x0f','\x0f','\x02','\x00', // 0x00e8
    '\x00','\x04','\x03','\x07','\x0f','\x08','\x08','\x03', // 0x00f0
    '\x0c','\x04','\x07','\x0f','\x88','\xe0','\xf0','\x7e', // 0x00f8
    '\x3f','\x1f','\x0f','\x03','\x10','\x1c','\x3d','\x3d', // 0x0100
    '\x05','\x1d','\x1d','\x04','\x1c','\x0c','\x0c','\x02', // 0x0108
    '\x0e','\x06','\x02','\x00','\x00','\x19',
#else
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0008
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0010
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0018
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0020
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0028
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0030
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0038
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0040
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0048
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0050
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0058
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0060
    '\x00','\x00','\x80','\x80','\x80','\xc0','\xc0','\xe0', // 0x0068
    '\xe0','\xe0','\xf0','\xf0','\xf8','\xf8','\xfc','\xfc', // 0x0070
    '\x7e','\x1e','\x07','\x00','\x00','\x00','\x00','\x00', // 0x0078
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0080
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0088
    '\x00','\x00','\x00','\x00','\xf0','\xf0','\x00','\x00', // 0x0090
    '\x00','\x00','\x00','\x80','\xc0','\x60','\x30','\x10', // 0x0098
    '\x00','\x00','\x30','\x30','\x00','\x00','\x00','\x00', // 0x00a0
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x00a8
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x00b0
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x00b8
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x00c0
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x00c8
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x00d0
    '\x00','\x00','\x00','\x00','\xf8','\xf8','\xfc','\xfc', // 0x00d8
    '\xfc','\x3c','\xbc','\xbe','\x9e','\x9e','\x9f','\x9f', // 0x00e0
    '\x9f','\x9f','\x9f','\xdf','\xcf','\xcf','\xcf','\xcf', // 0x00e8
    '\xcf','\xe7','\xe7','\x63','\x33','\x11','\x09','\x00', // 0x00f0
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x00f8
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0100
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0108
    '\x00','\x00','\x00','\x00','\xff','\xff','\x30','\x18', // 0x0110
    '\x0c','\x1e','\x7b','\xe1','\xc0','\x80','\x00','\x00', // 0x0118
    '\x00','\x00','\xfe','\xfe','\x00','\x00','\x00','\xfe', // 0x0120
    '\xfe','\x0c','\x06','\x03','\x03','\x07','\xfe','\xfc', // 0x0128
    '\x06','\x02','\x03','\x03','\x06','\xfe','\xf8','\x00', // 0x0130
    '\x00','\xf0','\xfc','\x4e','\x62','\x63','\x63','\x62', // 0x0138
    '\x66','\x7c','\x78','\x00','\x00','\x00','\xfe','\xfc', // 0x0140
    '\x06','\x03','\x03','\x02','\x00','\x8c','\xce','\x46', // 0x0148
    '\x43','\x63','\x63','\x22','\xfe','\xfc','\x00','\x00', // 0x0150
    '\x00','\x00','\xc0','\xfe','\xff','\xff','\xff','\x1f', // 0x0158
    '\x80','\xe0','\xe7','\xe7','\xe7','\xe7','\xe7','\xe7', // 0x0160
    '\xe7','\xe7','\xf7','\xf7','\xf3','\x73','\x73','\x31', // 0x0168
    '\x19','\x08','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0170
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0178
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0180
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0188
    '\x00','\x00','\x00','\x00','\x0f','\x0f','\x00','\x00', // 0x0190
    '\x00','\x00','\x00','\x00','\x03','\x07','\x0e','\x0c', // 0x0198
    '\x08','\x00','\x0f','\x0f','\x00','\x00','\x00','\x0f', // 0x01a0
    '\x0f','\x00','\x00','\x00','\x00','\x00','\x0f','\x0f', // 0x01a8
    '\x00','\x00','\x00','\x00','\x00','\x0f','\x0f','\x00', // 0x01b0
    '\x00','\x01','\x07','\x0e','\x0c','\x08','\x08','\x0c', // 0x01b8
    '\x0c','\x07','\x03','\x00','\x00','\x00','\x0f','\x0f', // 0x01c0
    '\x00','\x00','\x00','\x00','\x03','\x07','\x0f','\x08', // 0x01c8
    '\x08','\x08','\x0c','\x04','\x07','\x0f','\x88','\xe0', // 0x01d0
    '\xf0','\x7e','\x3f','\x1f','\x0f','\x03','\x10','\x1c', // 0x01d8
    '\x3d','\x3d','\x3d','\x3d','\x3d','\x1d','\x1d','\x1d', // 0x01e0
    '\x1d','\x1c','\x0c','\x0c','\x0e','\x06','\x02','\x00', // 0x01e8
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x01f0
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x01f8
    '\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00', // 0x0200
#endif
};

static uint8_t ssd1306_command(uint8_t command);
static uint8_t ssd1306_data(uint8_t data);

uint8_t ssd1306_init(void)
{
    /* reset */
    /* PB0 -> PB7 */
    DDRB  |=  (1<<PB7);
    PORTB |=  (1<<PB7);
    _delay_ms(1);
    PORTB &= ~(1<<PB7);
    _delay_ms(10);
    PORTB |=  (1<<PB7);
    _delay_ms(10);

    uint8_t i;
    uint8_t ret = 0;
    for (i = 0; i < SSD1306_INIT_TABLE_LENGTH; i++) {
        ret = ssd1306_command(pgm_read_byte(&ssd1306_init_table[i]));
        if (ret) {
            break;
        }
    }

    return ret;
}

uint8_t ssd1306_demo(void)
{
    uint8_t ret = 0;

    ssd1306_command(SSD1306_COMM_COLUMN_ADDR);
    ssd1306_command(0);
    ssd1306_command(SSD1306_LCD_WIDTH - 1);

    ssd1306_command(SSD1306_COMM_PAGE_ADDR);
    ssd1306_command(0);
    ssd1306_command(7);

#if 1
#ifdef TWBR
    uint8_t _twbr = TWBR;
    TWBR = 12;
#endif
#endif

#if 1
    ret = i2c_start(SSD1306_ADDR | I2C_WRITE);
    if (ret) goto stop;
    ret = i2c_write(0x40);
    if (ret) goto stop;

    uint16_t i, j;
    uint16_t count = 1;
    uint8_t data;
    uint8_t length = 1;
    for (i = 0; i < SSD1306_DEMO_TABLE_LENGTH; i++) {
        data = pgm_read_byte(&ssd1306_demo_table[i]);
        if (data == pgm_read_byte(&ssd1306_demo_table[i + 1])) {
            length = pgm_read_byte(&ssd1306_demo_table[i + 2]);
            for (j = 0; j < length; j++) {
                ret = i2c_write(data);
                if (ret) goto stop;
                count++;
            }
            i += 2;
        }
        else {
            ret = i2c_write(data);
            if (ret) goto stop;
            count++;
        }
        if (count >= SSD1306_BUFFER_SIZE) {
            break;
        }
    }
#else
    uint16_t i;
    uint8_t j;
    for (i = 0; i < SSD1306_BUFFER_SIZE; i++) {
        ret = i2c_start(SSD1306_ADDR | I2C_WRITE);
        ret = i2c_write(0x40);
        for (j = 0; j < 16; j++) {
            //ret = i2c_write(0xFF);
            ret = i2c_write(pgm_read_byte(&ssd1306_demo_table[i]));
            i++;
        }
        i--;
        i2c_stop();
    }
#endif

stop:
    i2c_stop();

#if 1
#ifdef TWBR
    TWBR = _twbr;
#endif
#endif

    return ret;
}

uint8_t ssd1306_command(uint8_t command)
{
    uint8_t ret;
    ret = i2c_start(SSD1306_ADDR | I2C_WRITE);
    if (ret) goto stop;
    ret = i2c_write(0x00);
    if (ret) goto stop;
    ret = i2c_write(command);
stop:
    i2c_stop();
    return ret;
}

uint8_t ssd1306_data(uint8_t data)
{
    uint8_t ret;
    ret = i2c_start(SSD1306_ADDR | I2C_WRITE);
    if (ret) goto stop;
    ret = i2c_write(0x40);
    if (ret) goto stop;
    ret = i2c_write(data);
stop:
    i2c_stop();
    return ret;
}

void ssd1306_start_scroll_left(uint8_t start, uint8_t stop)
{
    ssd1306_command(SSD1306_COMM_LEFT_HORIZONTAL_SCROLL);
    ssd1306_command(0x00);
    ssd1306_command(start);
    ssd1306_command(0x00);
    ssd1306_command(stop);
    ssd1306_command(0x00);
    ssd1306_command(0xFF);
    ssd1306_command(SSD1306_COMM_ACTIVATE_SCROLL);
}

void ssd1306_start_scroll_right(uint8_t start, uint8_t stop)
{
    ssd1306_command(SSD1306_COMM_RIGHT_HORIZONTAL_SCROLL);
    ssd1306_command(0x00);
    ssd1306_command(start);
    ssd1306_command(0x00);
    ssd1306_command(stop);
    ssd1306_command(0x00);
    ssd1306_command(0xFF);
    ssd1306_command(SSD1306_COMM_ACTIVATE_SCROLL);
}

void ssd1306_stop_scroll(void)
{
    ssd1306_command(SSD1306_COMM_DEACTIVATE_SCROLL);
}

#endif
