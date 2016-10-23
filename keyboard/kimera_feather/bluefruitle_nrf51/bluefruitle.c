#include <avr/io.h>
#include <stdio.h>
#include "host.h"
#include "host_driver.h"
#include "serial.h"
#include "bluefruitle.h"
/* #include "bluefruitle_spi.h" */
#include "print.h"
#include "timer.h"
#include "wait.h"
#include "suart.h"
#include "serial.h"
#include "debug.h"

#define TIMEOUT 1000

/* Host driver */
static uint8_t keyboard_leds(void);
static void send_keyboard(report_keyboard_t *report);
static void send_mouse(report_mouse_t *report);
static void send_system(uint16_t data);
static void send_consumer(uint16_t data);

host_driver_t bluefruitle_driver = {
    keyboard_leds,
    send_keyboard,
    send_mouse,
    send_system,
    send_consumer
};

static bool report_error = false;

void bluefruitle_init(void)
{
#if 0
    // PF7: BT connection control(high: connect, low: disconnect)
    bluefruitle_autoconnect();

    // PF6: linked(input without pull-up)
    DDRF  &= ~(1<<6);
    PORTF |=  (1<<6);

    // PF1: RTS(low: allowed to send, high: not allowed)
    DDRF &= ~(1<<1);
    PORTF &= ~(1<<1);

    // PD5: CTS(low: allow to send, high:not allow)
    DDRD |= (1<<5);
    PORTD &= ~(1<<5);
#endif

#if 0
    serial_init();
    while (1) {
        serial_send('!');
    }
#endif
    //DDRD |= (1<<PD0);
    DDRC |= (1<<PC6);
    DDRF |= (1<<PF5);
#if 0
    while (1) {
        xmit('!');
    }
#endif

    serial_init();
    xprintf("Serial Init\n");

    bluefruitle_puts("AT+FACTORYRESET\n");
    xprintf("Sent\n");
    xprintf("%s\n", bluefruitle_gets(TIMEOUT));

    bluefruitle_puts("AT+GAPDEVNAME=Hging's Bluefruit Keyboard\n");
    xprintf("%s\n", bluefruitle_gets(TIMEOUT));

    bluefruitle_puts("AT+BLEHIDEN=ON\n");
    xprintf("%s\n", bluefruitle_gets(TIMEOUT));
    xprintf("BLEHIDEN OK\n");

    bluefruitle_puts("AT+GAPSTARTADV\n");
    xprintf("%s\n", bluefruitle_gets(TIMEOUT));

    bluefruitle_puts("AT+GAPDELBONDS\n");
    xprintf("%s\n", bluefruitle_gets(TIMEOUT));

    bluefruitle_puts("ATZ\n");
    xprintf("%s\n", bluefruitle_gets(TIMEOUT));
    xprintf("RESET OK\n");
}

void bluefruitle_pairing(void)
{
}

int16_t bluefruitle_getc(void)
{
    return serial_recv2();
}

const char *bluefruitle_gets(uint16_t timeout)
{
    static char s[24];
    uint16_t t = timer_read();
    uint8_t i = 0;
    int16_t c;
    while (i < 23 && timer_elapsed(t) < timeout) {
        if ((c = bluefruitle_getc()) != -1) {
            if ((char)c == '\r') continue;
            if ((char)c == '\n') break;
            s[i++] = c;
        }
    }
    s[i] = '\0';
    return s;
}

void bluefruitle_putc(uint8_t c)
{
    serial_send(c);
}

void bluefruitle_puts(char *s)
{
    while (*s)
        serial_send(*s++);
}

bool bluefruitle_autoconnecting(void)
{
#if 0
    // GPIO6 for control connection(high: auto connect, low: disconnect)
    // Note that this needs config: SM,4(Auto-Connect DTR Mode)
    return (PORTF & (1<<7) ? true : false);
#endif
    return false;
}

void bluefruitle_autoconnect(void)
{
#if 0
    // hi to auto connect
    DDRF |= (1<<7);
    PORTF |= (1<<7);
#endif
}

void bluefruitle_disconnect(void)
{
#if 0
    // low to disconnect
    DDRF |= (1<<7);
    PORTF &= ~(1<<7);
#endif
}

bool bluefruitle_rts(void)
{
#if 0
    // low when RN-42 is powered and ready to receive
    return PINF&(1<<1);
#endif
    return report_error;
}

void bluefruitle_cts_hi(void)
{
#if 0
    // not allow to send
    PORTD |= (1<<5);
#endif
}

void bluefruitle_cts_lo(void)
{
#if 0
    // allow to send
    PORTD &= ~(1<<5);
#endif
}

bool bluefruitle_linked(void)
{
#if 0
    // RN-42 GPIO2
    //   Hi-Z:  Not powered
    //   High:  Linked
    //   Low:   Connecting
    return PINF&(1<<6);
#endif
    return true;
}


static uint8_t leds = 0;
static uint8_t keyboard_leds(void) { return leds; }
void bluefruitle_set_leds(uint8_t l) { leds = l; }

static void send_keyboard(report_keyboard_t *report)
{
    static char buf[43];
    sprintf(buf, "AT+BLEKEYBOARDCODE=%02X-00-%02X-%02X-%02X-%02X-%02X-%02X\n",
        report->mods,
        report->keys[0],
        report->keys[1],
        report->keys[2],
        report->keys[3],
        report->keys[4],
        report->keys[5]);
    bluefruitle_puts(buf);
    const char *result = bluefruitle_gets(TIMEOUT);
    if (memcmp(result, "OK", 2) == 0) {
        report_error = 0;
    }
    else {
        report_error = 1;
    }
#if 0
    xprintf("%s\n", bluefruitle_gets(TIMEOUT));
#endif
}

static void send_mouse(report_mouse_t *report)
{
    // wake from deep sleep
/*
    PORTD |= (1<<5);    // high
    wait_ms(5);
    PORTD &= ~(1<<5);   // low
*/

#if 0
    serial_send(0xFD);  // Raw report mode
    serial_send(5);     // length
    serial_send(2);     // descriptor type
    serial_send(report->buttons);
    serial_send(report->x);
    serial_send(report->y);
    serial_send(report->v);
#endif
}

static void send_system(uint16_t data)
{
    // Table 5-6 of RN-BT-DATA-UB
    // 81,82,83 scan codes can be used?
}


static uint16_t usage2bits(uint16_t usage)
{
    switch (usage) {
        case AC_HOME:               return 0x01;
        case AL_EMAIL:              return 0x02;
        case AC_SEARCH:             return 0x04;
        //case AL_KBD_LAYOUT:         return 0x08;  // Apple virtual keybaord toggle
        case AUDIO_VOL_UP:          return 0x10;
        case AUDIO_VOL_DOWN:        return 0x20;
        case AUDIO_MUTE:            return 0x40;
        case TRANSPORT_PLAY_PAUSE:  return 0x80;
        case TRANSPORT_NEXT_TRACK:  return 0x100;
        case TRANSPORT_PREV_TRACK:  return 0x200;
        case TRANSPORT_STOP:        return 0x400;
        case TRANSPORT_STOP_EJECT:  return 0x800;
        //case return 0x1000;   // Fast forward
        //case return 0x2000;   // Rewind
        //case return 0x4000;   // Stop/eject
        //case return 0x8000;   // Internet browser
    };
    return 0;
}

static void send_consumer(uint16_t data)
{
#if 0
    uint16_t bits = usage2bits(data);
    serial_send(0xFD);  // Raw report mode
    serial_send(3);     // length
    serial_send(3);     // descriptor type
    serial_send(bits&0xFF);
    serial_send((bits>>8)&0xFF);
#endif
}


/* Null driver for config_mode */
static uint8_t config_keyboard_leds(void);
static void config_send_keyboard(report_keyboard_t *report);
static void config_send_mouse(report_mouse_t *report);
static void config_send_system(uint16_t data);
static void config_send_consumer(uint16_t data);

host_driver_t bluefruitle_config_driver = {
    config_keyboard_leds,
    config_send_keyboard,
    config_send_mouse,
    config_send_system,
    config_send_consumer
};

static uint8_t config_keyboard_leds(void) { return leds; }
static void config_send_keyboard(report_keyboard_t *report) {}
static void config_send_mouse(report_mouse_t *report) {}
static void config_send_system(uint16_t data) {}
static void config_send_consumer(uint16_t data) {}
