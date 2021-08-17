# Example for supporting custom board on Nuvoton's Mbed platform

This is an example to instruct how to add custom board support on Nuvoton's Mbed platform.
This support has the following prerequisites:

-   The MCU series the custom board bases on must have been supported on Mbed OS, usually through Mbed-Enabled boards.
-   The Mbed OS implementations for the MCU series must enable capability of being configurable to some degree.

## Support custom boards

-   NuMaker-M483KG  ( Target = NUMAKER_M483KG )
    
-   NuMaker-IOT-M2354  ( Target = NU_IOT_M2354 )

## Hardware requirements

-   Custom board, e.g.: NuMaker-M483KG

## Software requirements

-   [Arm Mbed CLI](https://github.com/ARMmbed/mbed-cli)
-   [Arm Compiler 6](https://os.mbed.com/docs/mbed-os/v5.15/tools/index.html)

## Instructing step-by-step

In the following, we take **NuMaker-M483KG** board, not Mbed-Enabled, as an example for Mbed OS support.
It is suggested application developers first reference [Using Mbed OS on a custom board](https://os.mbed.com/docs/mbed-os/v5.15/porting/porting-a-custom-board.html) for base knowledge.
The `targets.json` file (located in `mbed-os/targets/targets.json`) is necessary for reference.

**Note**: This support heavily relies on configurability of target MCU series and is subject to change with Mbed OS version.

1.  Create Mbed target for **NuMaker-M483KG** board into `custom_targets.json` file:

    ```json
    "NUMAKER_M483KG": {
        ......
    }
    ```

1.  Inherit `MCU_M480` target, which **NuMaker-M483KG** board bases on:
    ```json
    "inherits": [
        "MCU_M480"
    ],
    ```

1.  Specify `device_name` (MCU part name) for **NuMaker-M483KG** board:
    ```json
    "device_name": "M483KGCAE2A",
    ```

1.  Specify `detect_code` for **NuMaker-M483KG** board. This can acquire by running `mbed detect` when the board is connected to host through USB cable:
    ```json
    "detect_code": [
        "1301"
    ],
    ```

1.  Change pin names by providing custom `PinNames.h`:
    1.  Create `TARGET_NUMAKER_M483KG` directory.
    1.  Copy `mbed-os/targets/TARGET_NUVOTON/TARGET_M480/TARGET_NUMAKER_PFM_M487/PinNames.h` there.
    1.  Adapt `TARGET_NUMAKER_M483KG`/`PinNames.h` to **NuMaker-M483KG** board.

    **Note**: For consistency and correctness, change UART configurations for USB VCOM and STDIO via target configuration options as below.
    ```C++
    #if defined(MBED_CONF_TARGET_USB_UART_TX)
        USBTX           = MBED_CONF_TARGET_USB_UART_TX,
    #else
        USBTX           = NC,
    #endif
    #if defined(MBED_CONF_TARGET_USB_UART_RX)
        USBRX           = MBED_CONF_TARGET_USB_UART_RX,
    #else
        USBRX           = NC,
    #endif
    #if defined(MBED_CONF_TARGET_STDIO_UART_TX)
        STDIO_UART_TX   = MBED_CONF_TARGET_STDIO_UART_TX,
    #else
        STDIO_UART_TX   = USBTX,
    #endif
    #if defined(MBED_CONF_TARGET_STDIO_UART_RX)
        STDIO_UART_RX   = MBED_CONF_TARGET_STDIO_UART_RX,
    #else
        STDIO_UART_RX   = USBRX,
    #endif
    ```

1.  Change UART configurations for USB VCOM of **NuMaker-M483KG** board:
    ```json
    "overrides": {
        ......
        "usb-uart": "UART_0",
        "usb-uart-tx": "PB_13",
        "usb-uart-rx": "PB_12",
        ......
    },
    ```

1.  **NuMaker-M483KG** board doesn't support SPIM CCM mode. Disable it:
    ```json
    "overrides": {
        ......
        "spim-ccm-enable": 0,
        ......
    },
    ```

1.  **NuMaker-M483KG** board doesn't have network interface. Disable it:
    ```json
    "overrides": {
        ......
        "network-default-interface-type": null
        ......
    },
    ```

1.  Change Flash/SRAM specification for **NuMaker-M483KG** board:
    ```json
    "mbed_rom_start": "0x0",
    "mbed_rom_size": "0x40000",
    "mbed_ram_start": "0x20000000",
    "mbed_ram_size": "0x20000",
    ```

    **Note**: Check M480 Selection Code in M480 datasheet for actual memory size.

1.  Build the example on NUMAKER_M483KG/ARMCC or GCC:
    1.  Build code by CLI1.
    ```sh
    $ mbed compile -m NUMAKER_M483KG -t ARMC6
    $ mbed compile -m NUMAKER_M483KG -t GCC_ARM   
    ```
    1.  Build code by CLI2.
    ```sh
    $ mbed-tools compile -m NUMAKER_M483KG -t ARM
    $ mbed-tools compile -m NUMAKER_M483KG -t GCC_ARM
    ```
1.  Flash by drag-n-drop built image onto **NuMaker-M483KG** board

1.  We should see the red LED on **NuMaker-M483KG** board is blinking. On host terminal (115200/8-N-1), we should see messages:
    ```
    blinking: 0
    blinking: 1
    blinking: 2
    blinking: 3
    ```

## Troubleshooting

1.  Fail to export Keil uVision project for **NUMAKER_M483KG** target

    To export Keil uVision project for **NUMAKER_M483KG** target, run:
    ```sh
    $ mbed export -i uvision6 -m NUMAKER_M483KG
    ```
    
    But meet error with:
    ```
    NUMAKER_M483KG not supported by uvision6
    ```
    
    This is because the MCU part name **M483KGCAE2A** defined above is not supported in mbed-os's cmsis pack database.
    Try change to another close one, e.g. **M483SGAAE**:
    ```json
    "device_name": "M483SGAAE",
    ```

    **Note**: Though SRAM memory specs in **M483KGCAE2A** (128KiB) and in **M483SGAAE** (64KiB) are different, it has been addressed in:
    ```json
    "mbed_ram_start": "0x20000000",
    "mbed_ram_size": "0x20000",
    ```

1.  Meet compile warning or error with bootloader on **NUMAKER_M483KG** target, like `Configuration error: Bootloader not supported on this target: targets.json `device_name` not found in arm_pack_manager index.`

    Same as above, this is because the MCU part name **M483KGCAE2A** defined above is not supported in mbed-os's cmsis pack database.
    Resolve it as above.
