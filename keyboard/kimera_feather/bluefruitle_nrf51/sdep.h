#ifndef SDEP_H
#define SDEP_H

#include <stdint.h>
#include <stdbool.h>

#define ATTR_PACKED                 __attribute__ ((packed))
#define SDEP_MAX_PACKETSIZE         16 // Maximum payload per packet
#define SDEP_DEFAULT_TIMEOUT        250
#define SDEP_DEFAULT_DELAY_US       50
#define SDEP_SLAVE_NOT_READY        0xFEu
#define SDEP_SLAVE_READ_OVERFLOW    0xFFu
#define SDEP_IRQ_DDR                DDRE
#define SDEP_IRQ_PIN                PINE
#define SDEP_IRQ_BIT                PE6

typedef enum
{
    SDEP_CMDTYPE_INITIALIZE     = 0xBEEF,   /**< Controls the on board LED(s) */
    SDEP_CMDTYPE_AT_WRAPPER     = 0x0A00,
    SDEP_CMDTYPE_BLE_UARTTX     = 0x0A01,
    SDEP_CMDTYPE_BLE_UARTRX     = 0x0A02,
} sdep_cmd_type_t;

typedef enum
{
    SDEP_MSGTYPE_COMMAND        = 0x10,
    SDEP_MSGTYPE_RESPONSE       = 0x20,
    SDEP_MSGTYPE_ALERT          = 0x40,
    SDEP_MSGTYPE_ERROR          = 0x80
} sdep_msg_type_t;

typedef struct ATTR_PACKED {
    uint8_t msg_type;               // 8-bit message type indicator (sdep_msg_type_t)

    union
    {
        uint16_t cmd_id;            // 16-bit command ID
        struct
        {
            uint8_t cmd_id_low;
            uint8_t cmd_id_high;
        };
    };

    struct ATTR_PACKED
    {
        uint8_t length    : 7;      // Payload length (for this packet)
        uint8_t more_data : 1;      // 'more' bit for multiple packet transfers
    };
} sdep_msg_header_t;

typedef struct ATTR_PACKED 
{
    sdep_msg_header_t header;
    uint8_t payload[SDEP_MAX_PACKETSIZE];
} sdep_msg_command_t;

typedef sdep_msg_command_t sdep_msg_response_t;
typedef sdep_msg_command_t sdep_msg_alert_t;

void sdep_init(void);
bool sdep_send_packet(uint16_t command, const uint8_t *buf, uint8_t count, uint8_t more_data);
bool sdep_receive_packet(sdep_msg_response_t *response);
bool sdep_has_irq(void);

#endif
