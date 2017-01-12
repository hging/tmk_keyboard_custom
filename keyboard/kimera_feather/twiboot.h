#ifndef TWIBOOT_H
#define TWIBOOT_H

#ifndef TWI_ADDRESS
#define TWI_ADDRESS             0x29
#endif

/* SLA+R */
#define CMD_WAIT                0x00
#define CMD_READ_VERSION        0x01
#define CMD_READ_MEMORY         0x02

/* SLA+W */
#define CMD_SWITCH_APPLICATION  CMD_READ_VERSION
#define CMD_WRITE_MEMORY        CMD_READ_MEMORY

/* CMD_SWITCH_APPLICATION parameter */
#define BOOTTYPE_BOOTLOADER     0x00                            /* only in APP */
#define BOOTTYPE_APPLICATION    0x80

/* CMD_{READ|WRITE}_* parameter */
#define MEMTYPE_CHIPINFO        0x00
#define MEMTYPE_FLASH           0x01
#define MEMTYPE_EEPROM          0x02
#define MEMTYPE_PARAMETERS      0x03                            /* only in APP */

#endif
