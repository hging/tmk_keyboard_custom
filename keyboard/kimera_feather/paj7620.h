#ifndef PAJ7620_H
#define PAJ7620_H

// Device ID
#define PAJ7620_ID                      0x73

// Register Bank Select
#define PAJ7620_ADDR_BASE               0x00
#define PAJ7620_REGISTER_BANK_SEL       (PAJ7620_ADDR_BASE + 0xEF)

// Register Bank 0
#define PAJ7620_ADDR_SUSPEND_CMD        (PAJ7620_ADDR_BASE + 0x03)
#define PAJ7620_ADDR_GES_PS_DET_MASK_0  (PAJ7620_ADDR_BASE + 0x41)
#define PAJ7620_ADDR_GES_PS_DET_MASK_1  (PAJ7620_ADDR_BASE + 0x42)
#define PAJ7620_ADDR_GES_PS_DET_FLAG_0  (PAJ7620_ADDR_BASE + 0x43)
#define PAJ7620_ADDR_GES_PS_DET_FLAG_1  (PAJ7620_ADDR_BASE + 0x44)
#define PAJ7620_ADDR_STATE_INDICATOR    (PAJ7620_ADDR_BASE + 0x45)
#define PAJ7620_ADDR_PS_HIGH_THRESHOLD  (PAJ7620_ADDR_BASE + 0x69)
#define PAJ7620_ADDR_PS_LOW_THRESHOLD   (PAJ7620_ADDR_BASE + 0x6A)
#define PAJ7620_ADDR_PS_APPROACH_STATE  (PAJ7620_ADDR_BASE + 0x6B)
#define PAJ7620_ADDR_PS_RAW_DATA        (PAJ7620_ADDR_BASE + 0x6C)

// Register Bank 1
#define PAJ7620_ADDR_PS_GAIN            (PAJ7620_ADDR_BASE + 0x44)
#define PAJ7620_ADDR_IDLE_S1_STEP_0     (PAJ7620_ADDR_BASE + 0x67)
#define PAJ7620_ADDR_IDLE_S1_STEP_1     (PAJ7620_ADDR_BASE + 0x68)
#define PAJ7620_ADDR_IDLE_S2_STEP_0     (PAJ7620_ADDR_BASE + 0x69)
#define PAJ7620_ADDR_IDLE_S2_STEP_1     (PAJ7620_ADDR_BASE + 0x6A)
#define PAJ7620_ADDR_OP_TO_S1_STEP_0    (PAJ7620_ADDR_BASE + 0x6B)
#define PAJ7620_ADDR_OP_TO_S1_STEP_1    (PAJ7620_ADDR_BASE + 0x6C)
#define PAJ7620_ADDR_OP_TO_S2_STEP_0    (PAJ7620_ADDR_BASE + 0x6D)
#define PAJ7620_ADDR_OP_TO_S2_STEP_1    (PAJ7620_ADDR_BASE + 0x6E)
#define PAJ7620_ADDR_OPERATION_ENABLE   (PAJ7620_ADDR_BASE + 0x72)

// PAJ7620_REGISTER_BANK_SEL
enum {
    PAJ7620_BANK0 = 0,
    PAJ7620_BANK1
};

// PAJ7620_ADDR_SUSPEND_CMD
enum {
    PAJ7620_I2C_SUSPEND = 0,
    PAJ7620_I2C_WAKEUP
};

// PAJ7620_ADDR_OPERATION_ENABLE
enum {
    PAJ7620_DISABLE = 0,
    PAJ7620_ENABLE
};

// Register 0
enum {
    PAJ7620_GES_RIGHT_FLAG              = (1<<0),
    PAJ7620_GES_LEFT_FLAG               = (1<<1),
    PAJ7620_GES_UP_FLAG                 = (1<<2),
    PAJ7620_GES_DOWN_FLAG               = (1<<3),
    PAJ7620_GES_FORWARD_FLAG            = (1<<4),
    PAJ7620_GES_BACKWARD_FLAG           = (1<<5),
    PAJ7620_GES_CLOCKWISE_FLAG          = (1<<6),
    PAJ7620_GES_COUNT_CLOCKWISE_FLAG    = (1<<7)
};

// Register 1
enum {
    PAJ7620_WAVE_FLAG                   = (1<<0)
};

bool paj7620_init(void);
uint8_t paj7620_read_gesture(void);
uint8_t paj7620_read_gesture1(void);

#endif
