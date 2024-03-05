# Custom firmware for the Tuya TS0202 ZigBee PIR motion sensor

![ts0202](https://github.com/xaviergr/ts0202_pir_custom_firmware/assets/1671631/e6ab9248-b88a-4981-8ef2-86a3111c9bd8)

A minimal ZigBee firmware for TLSR825x based PIR motion sensor devices.
The implementation is based on the ZigBee SDK examples provided by Telink, but
vastly simplified and cleaned up for maintainability and ease of development.

The firmware targets to support the ubiquitous AliExpress Tuya PIR motion sensors
that are sold under various names like ONENUO and AUBESS. Unfortunately, while
very competitively priced, all these devices are almost useless since you
can't really customize the occupied-to-unoccupied delay. Instead the occupied
state is cleared after a fixed 60 second delay from the moment the last movement
is detected by the PIR sensor.

This alternative firmware is pretty basic and supports the bare minimum:
 - Pairing (when first powering up or by holding the button for 3 seconds)
 - Reporting of remaining battery percentage
 - Customizable occupied to unoccupied delay

Features that are currently missing:
 - Touchlink
 - Identification
 - Binding and groups
 - Over the air updates (OTA)

The idle power consumption is minimized by utilizing the deep sleep mode of the
microcontroller and was measured to be around 13uA (vs 15.5uA for the original
firmware) for all tested devices.

## Supported devices

These little Tuya PIR devices come under various names and have many different variants.

As of now the following models have been tested and known to work:
 - TS0202 by _TZ3000_6ygjfyll
 - TS0202 by _TZ3040_bb6xaihh

Both devices utilize the BS-612 PIR sensor but they have a slightly different
PCB layout and different GPIO mappings.

In theory any device based on the Telink TLSR825x microcontroller should be
supported but if the layout of the PCB is different, slight modifications to the
firmware might be needed.

### TZ3000_6ygjfyll variant

[Back PCB layout](https://github.com/xaviergr/ts0202_pir_custom_firmware/assets/1671631/0a67bdb0-653e-4e34-9c7f-91f5806a6b56)

[Front PCB layout and wiring](https://github.com/xaviergr/ts0202_pir_custom_firmware/assets/1671631/c07b3cbc-5f7e-499f-b4ff-5234634219a5)

This variant is the better one since the ONTIME pin of the BS-612 sensor can be
configured via GPIOs for a minimum clear delay of around 4-5 seconds without any
hardware modifications.

### TZ3040_bb6xaihh variant

[Back PCB layout](https://github.com/xaviergr/ts0202_pir_custom_firmware/assets/1671631/c219fedf-c545-436c-b95c-70e964b369b3)

[Front PCB layout and wiring](https://github.com/xaviergr/ts0202_pir_custom_firmware/assets/1671631/d8d658d1-c19c-4a42-91ac-2a3dcc0bcd59)

Unfortunately this variant has a fixed voltage divider applied to the ONTIME pin
and without hardware modifications the clear delay is fixed to around 60 seconds.
To lower the minimum delay to around 3-4 seconds the highlighted resistor has
to be removed and then short the remaining pins.

## Occupied to unoccupied delay

![clear_delay](https://github.com/xaviergr/ts0202_pir_custom_firmware/assets/1671631/22819849-180d-4634-84fa-1194e8fb8759)

To adjust the clear delay, overwrite the pir_o_to_u_delay attribute of the
OccupancySensing cluster with your desired value in seconds, while the sensor
is not idle (press a button or move the device while writing the attribute).

## Compiling

To build the default TZ3000_6ygjfyll board variant:

```
git clone git@github.com:xaviergr/ts0202_pir_custom_firmware.git
cd ts0202_pir_custom_firmware
cmake -B build
cmake --build build/
```

For the TZ3040_bb6xaihh variant, run the following:
```
cmake -DBOARD_VARIANT=2 build/
cmake --build build/
```

If omitted, BOARD_VARIANT defaults to 1.
The output binary that can be flashed to the device is: 'build/ts0202.bin'

## Flashing

As of now the OTA update feature is not supported so flashing the firmware manually
is the only option. Luckily the flashing process only requires a USB to UART
adapter and to solder 4 wires. For more instructions on flashing refer to the
[TlsrComSwireWriter](https://github.com/pvvx/TlsrComSwireWriter) repository.

For the exact pin layout check the above linked photos for your board variant.

To flash the binary run the following commands:
```
.\TLSR825xComFlasher.py -p COM4 -t 100 ea
.\TLSR825xComFlasher.py -p COM4 -t 100 wf 0x0 path-to-ts0202.bin
```

Remember to always erase the entire flash (first command) before flashing the
binary. Make sure to adjust the 'COM4' port in the above example to whatever
serial port under Windows (COMx) or Linux (/dev/ttyUSBx) you are using.
