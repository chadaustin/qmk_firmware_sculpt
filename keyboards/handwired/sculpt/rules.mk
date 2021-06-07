MCU = at90usb1286

F_CPU = 16000000
F_USB = $(F_CPU)

BOOTLOADER = atmel-dfu

# Interrupt driven control endpoint task(+60)
OPT_DEFS += -DINTERRUPT_CONTROL_ENDPOINT

# The default debouncer (sym_defer_g) adds 5 ms of latency to each keypress.
# sym_eager_pk shaves about 5 ms of latency off each keypress, but I have
# observed some bouncing. Since keydown latency is more perceptible than
# keyup, use an asymmetric debouncer.
DEBOUNCE_TYPE = asym_defer_pk

BOOTMAGIC_ENABLE = no       # Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE = yes       # Mouse keys(+4700)
EXTRAKEY_ENABLE = yes       # Audio control and System control(+450)
CONSOLE_ENABLE = no        # Console for debug(+400)
COMMAND_ENABLE = no        # Commands for debug and configuration
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
NKRO_ENABLE = no            # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality on B7 by default
MIDI_ENABLE = no            # MIDI controls
UNICODE_ENABLE = no         # Unicode
BLUETOOTH_ENABLE = no       # Enable Bluetooth with the Adafruit EZ-Key HID
AUDIO_ENABLE = no           # Audio output on port C6
DIP_SWITCH_ENABLE = yes
