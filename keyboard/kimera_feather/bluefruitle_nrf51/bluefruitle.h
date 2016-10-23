#ifndef BLUEFRUITLE_H
#define BLUEFRUITLE_H

#include <stdbool.h>

host_driver_t bluefruitle_driver;
host_driver_t bluefruitle_config_driver;

void bluefruitle_init(void);
void bluefruitle_pairing(void);
int16_t bluefruitle_getc(void);
const char *bluefruitle_gets(uint16_t timeout);
void bluefruitle_putc(uint8_t c);
void bluefruitle_puts(char *s);
bool bluefruitle_autoconnecting(void);
void bluefruitle_autoconnect(void);
void bluefruitle_disconnect(void);
bool bluefruitle_rts(void);
void bluefruitle_cts_hi(void);
void bluefruitle_cts_lo(void);
bool bluefruitle_linked(void);
void bluefruitle_set_leds(uint8_t l);

#endif
