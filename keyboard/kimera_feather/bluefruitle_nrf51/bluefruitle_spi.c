#include <string.h>
#include "bluefruitle_spi.h"
#include "sdep.h"

#define TX_BUFFER_SIZE (SDEP_MAX_PACKETSIZE)
#define RX_BUFFER_SIZE (SDEP_MAX_PACKETSIZE * 2)

static uint8_t tx_buffer[TX_BUFFER_SIZE + 1];
static uint8_t tx_count = 0;
static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint8_t rx_put = 0;
static uint8_t rx_get = 0;
static uint8_t rx_empty = 1;

#if 0
#define rx_count() ((rx_put - rx_get + RX_BUFFER_SIZE) % RX_BUFFER_SIZE)
#define rx_remain() (RX_BUFFER_SIZE - rx_count())
#endif

static bool get_response(void);
static inline bool rx_push(uint8_t c);
static inline bool rx_pop(uint8_t *c);
static inline uint8_t rx_count(void);
static inline uint8_t rx_remain(void);

void serial_init(void)
{
    sdep_init();
    tx_count = 0;
    rx_put = 0;
    rx_get = 0;
    rx_empty = 1;
}

void serial_send(uint8_t c)
{
    if (c == '\r' || c == '\n') {
        if (tx_count > 0) {
            tx_buffer[tx_count] = 0;
            sdep_send_packet(SDEP_CMDTYPE_AT_WRAPPER, tx_buffer, tx_count, 0);
            tx_count = 0;
        }
    }
    else if (tx_count == SDEP_MAX_PACKETSIZE) {
        tx_buffer[tx_count] = 0;
        sdep_send_packet(SDEP_CMDTYPE_AT_WRAPPER, tx_buffer, tx_count, 1);
        tx_buffer[0] = c;
        tx_count = 1;
    }
    else {
        tx_buffer[tx_count++] = c;
    }
}

int16_t serial_recv2(void)
{
    uint8_t c;
    if (rx_pop(&c)) {
        return (uint16_t)c;
    }
    else {
        if (sdep_has_irq()) {
            if (!get_response()) {
                return -1;
            }
        }
        if (rx_pop(&c)) {
            return (uint16_t)c;
        }
        else {
            return -1;
        }
    }
}

bool get_response(void)
{
    while (rx_remain() >= SDEP_MAX_PACKETSIZE) {
        sdep_msg_response_t msg;
        memset(&msg, 0x00, sizeof(sdep_msg_response_t));
        if (!sdep_receive_packet(&msg)) {
            return false;
        }
        if (msg.header.length > 0) {
            for (uint8_t i = 0; i < msg.header.length; i++) {
                rx_push(msg.payload[i]);
            }
        }
        if (!msg.header.more_data) {
            break;
        }
    }

    return true;
}

inline bool rx_push(uint8_t c)
{
    if (!rx_empty && rx_put == rx_get) {
        return false;
    }
    else {
        rx_buffer[rx_put] = c;
        rx_put = (rx_put + 1) % RX_BUFFER_SIZE;
        if (rx_empty == 1) rx_empty = 0;
        return true;
    }
}

inline bool rx_pop(uint8_t *c)
{
    if (rx_empty) {
        return false;
    }
    else {
        *c = rx_buffer[rx_get];
        rx_get = (rx_get + 1) % RX_BUFFER_SIZE;
        if (rx_get == rx_put) rx_empty = 1;
        return true;
    }
}

inline uint8_t rx_count(void)
{
    return (rx_put - rx_get + RX_BUFFER_SIZE) % RX_BUFFER_SIZE;
}

inline uint8_t rx_remain(void)
{
    return RX_BUFFER_SIZE - rx_count();
}
