#include <avr/io.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "lufa.h"
#include "print.h"
#include "sendchar.h"
#include "bluefruitle.h"
#include "bluefruitle_task.h"
#include "serial.h"
#include "keyboard.h"
#include "keycode.h"
#include "action.h"
#include "action_util.h"
#include "wait.h"
#include "suart.h"
#include "suspend.h"

static int8_t sendchar_func(uint8_t c)
{
    xmit(c);        // SUART
    sendchar(c);    // LUFA
    return 0;
}

static void SetupHardware(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    // JTAG disable for PORT F. write JTD bit twice within four cycles.
    MCUCR |= (1<<JTD);
    MCUCR |= (1<<JTD);

    // Leonardo needs. Without this USB device is not recognized.
    USB_Disable();

    USB_Init();

    // for Console_Task
    USB_Device_EnableSOFEvents();
    print_set_sendchar(sendchar_func);

#if 0
    // SUART PD0:output, PD1:input
    DDRD |= (1<<0);
    PORTD |= (1<<0);
    DDRD &= ~(1<<1);
    PORTD |= (1<<1);
#endif
}

int main(void)  __attribute__ ((weak));
int main(void)
{
    SetupHardware();
    sei();

    /* wait for USB startup to get ready for debug output */
    uint8_t timeout = 255;  // timeout when USB is not available(Bluetooth)
    while (timeout-- && USB_DeviceState != DEVICE_STATE_Configured) {
        wait_ms(4);
#if defined(INTERRUPT_CONTROL_ENDPOINT)
        ;
#else
        USB_USBTask();
#endif
    }
    print("\nUSB init\n");

    bluefruitle_init();
    bluefruitle_task_init();
    print("Bluefruit LE init\n");

    /* init modules */
    keyboard_init();

    if (!bluefruitle_rts()) {
        host_set_driver(&bluefruitle_driver);
    } else {
        host_set_driver(&lufa_driver);
    }

#ifdef SLEEP_LED_ENABLE
    sleep_led_init();
#endif

    print("Keyboard start\n");
    while (1) {
        while (bluefruitle_rts() && // Bluefruit LE is off
                USB_DeviceState == DEVICE_STATE_Suspended) {
            print("[s]");
            matrix_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            suspend_power_down();
            if (USB_Device_RemoteWakeupEnabled && suspend_wakeup_condition()) {
                    USB_Device_SendRemoteWakeup();
            }
        }

        keyboard_task();

#if !defined(INTERRUPT_CONTROL_ENDPOINT)
        USB_USBTask();
#endif

        bluefruitle_task();
    }
}