#include <util/delay.h>
#include <string.h>
#include "sdep.h"
#include "spi.h"
#include "timer.h"
#include "debug.h"

void sdep_init(void)
{
    SDEP_IRQ_DDR &= ~(1<<SDEP_IRQ_BIT);
    spi_init();
    timer_init();
}

bool sdep_send_packet(uint16_t command, const uint8_t *buf, uint8_t count, uint8_t more_data)
{
#if 1
    xprintf("SDEP Send: 0x%04X, %u, %u, %s\n", command, count, more_data, buf);
#endif
    sdep_msg_command_t msg;
    msg.header.msg_type     = SDEP_MSGTYPE_COMMAND;
    msg.header.cmd_id       = command;
    msg.header.length       = count;
    msg.header.more_data    = (count == SDEP_MAX_PACKETSIZE) ? more_data: 0;

    if (buf != NULL && count > 0) {
        memcpy(msg.payload, buf, count);
    }

    spi_cs_enable();
    uint16_t t = timer_read();

    bool result = false;
    do {
        if (spi_fast_shift(msg.header.msg_type) != SDEP_SLAVE_NOT_READY) {
            result = true;
            break;
        }
        spi_cs_disable();
        _delay_us(SDEP_DEFAULT_DELAY_US);
        spi_cs_enable();
    } while (timer_elapsed(t) < SDEP_DEFAULT_TIMEOUT);

    if (result) {
        spi_transmit_sync(((uint8_t *)&msg) + 1, sizeof(sdep_msg_header_t) + count - 1);
    }

    spi_cs_disable();

    return result;
}

inline bool sdep_has_irq(void)
{
    return (SDEP_IRQ_PIN & (1<<SDEP_IRQ_BIT)) != 0;
}

bool sdep_receive_packet(sdep_msg_response_t *response)
{
    xprintf("SDEP Receiving ");
    uint16_t t = timer_read();

    while (!sdep_has_irq()) {
        xprintf("1");
        if (timer_elapsed(t) > SDEP_DEFAULT_TIMEOUT * 2) {
            xprintf("no IRQ!\n");
            return false;
        }
    }


    sdep_msg_header_t *header = &(response->header);

    spi_cs_enable();
    t = timer_read();

    bool result = false;
    do {
        xprintf("2");
        header->msg_type = spi_fast_shift(0xFF);

        if ((header->msg_type != SDEP_SLAVE_NOT_READY) &&
                (header->msg_type != SDEP_SLAVE_READ_OVERFLOW)) {
            result = true;
            break;
        }
        spi_cs_disable();
        _delay_us(SDEP_DEFAULT_DELAY_US);
        spi_cs_enable();
    } while (timer_elapsed(t) < SDEP_DEFAULT_TIMEOUT);
    if (!result) {
        xprintf("Timeout!\n");
    }

    if (result) {
        result = false;
        do {
            t = timer_read();
            do {
                xprintf("3");
                if ((header->msg_type == SDEP_MSGTYPE_RESPONSE) ||
                        (header->msg_type == SDEP_MSGTYPE_ERROR)) {
                    result = true;
                    break;
                }
                header->msg_type = spi_fast_shift(0xFF);
            } while (timer_elapsed(t) < SDEP_DEFAULT_TIMEOUT);

            if (!result) {
                xprintf("Timeout!\n");
                break;
            }
            result = false;

            uint8_t *p = &(header->msg_type) + 1;
            memset(p, 0xFF, sizeof(sdep_msg_header_t) - 1);
            spi_transfer_sync(p, p, sizeof(sdep_msg_header_t) - 1);

            if (header->msg_type == SDEP_MSGTYPE_ERROR) {
                xprintf("Error!\n");
                break;
            }

            uint16_t cmd_id = header->cmd_id;
            if (!(cmd_id == SDEP_CMDTYPE_AT_WRAPPER ||
                    cmd_id == SDEP_CMDTYPE_BLE_UARTTX ||
                    cmd_id == SDEP_CMDTYPE_BLE_UARTRX))
            {
                xprintf("No Data!\n");
                break;
            }

            if (header->length > SDEP_MAX_PACKETSIZE) {
                xprintf("Invalid Length!\n");
                break;
            }

            memset(response->payload, 0xFF, header->length);
            spi_transfer_sync(response->payload, response->payload, header->length);

            xprintf("OK\n");
            result = true;
        } while (0);
    }


    spi_cs_disable();

#if 1
    xprintf("\nSDEP Receive: 0x%02X, 0x%04X, %u, %u, %16s\n", header->msg_type, header->cmd_id, header->length, header->more_data, response->payload);
#endif

    return result;
}
