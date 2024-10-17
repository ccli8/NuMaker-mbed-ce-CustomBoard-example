# Example for supporting custom board on Nuvoton's Mbed CE platform

This is an example to instruct how to add custom board support on Nuvoton's Mbed CE platform.
This support has the following prerequisites:

-   The MCU series the custom board bases on must have been supported on Mbed OS, usually through Mbed-Enabled boards.
-   The Mbed OS implementations for the MCU series must enable capability of being configurable to some degree.

## Support custom boards

-   NuMaker-M483KG  ( Target = NUMAKER_M483KG )
    
-   NuMaker-IOT-M2354  ( Target = NU_IOT_M2354 )

## Hardware requirements

-   Custom board, e.g.: NuMaker-M483KG

## Software requirements

Use cmake-based build system.
Check out [hello world example](https://github.com/mbed-ce/mbed-ce-hello-world) for getting started.

> **⚠️ Warning**
>
> Legacy development tools below are not supported anymore.
> - [Arm's Mbed Studio](https://os.mbed.com/docs/mbed-os/v6.15/build-tools/mbed-studio.html)
> - [Arm's Mbed CLI 2](https://os.mbed.com/docs/mbed-os/v6.15/build-tools/mbed-cli-2.html)
> - [Arm's Mbed CLI 1](https://os.mbed.com/docs/mbed-os/v6.15/tools/developing-mbed-cli.html)

For [VS Code development](https://github.com/mbed-ce/mbed-os/wiki/Project-Setup:-VS-Code)
or [OpenOCD as upload method](https://github.com/mbed-ce/mbed-os/wiki/Upload-Methods#openocd),
install below additionally:

-   [NuEclipse](https://github.com/OpenNuvoton/Nuvoton_Tools#numicro-software-development-tools): Nuvoton's fork of Eclipse
-   Nuvoton forked OpenOCD: Shipped with NuEclipse installation package above.
    Checking openocd version `openocd --version`, it should fix to `0.10.022`.

## Instructing step-by-step

In the following, we take **NuMaker-M483KG** board, not Mbed-Enabled, as an example for Mbed OS support.
It is suggested application developers first reference
[Using Mbed OS on a custom board](https://os.mbed.com/docs/mbed-os/v6.16/porting/porting-custom-boards.html)
and
[Mbed CE Custom Targets Example](https://github.com/mbed-ce/mbed-ce-custom-targets)
for base knowledge.
The `targets.json5` file (located in `mbed-os/targets/targets.json5`) is necessary for reference.

1.  Clone the example and navigate into it
    ```
    $ git clone https://github.com/mbed-nuvoton/NuMaker-mbed-ce-CustomBoard-example
    $ cd NuMaker-mbed-ce-CustomBoard-example
    $ git checkout -f master
    ```

1.  Deploy necessary libraries
    ```
    $ git submodule update --init
    ```
    Or for fast install:
    ```
    $ git submodule update --init --filter=blob:none
    ```

1.  Create Mbed target for **NuMaker-M483KG** board into `custom_targets.json5` file:

    ```json55
    "NUMAKER_M483KG": {
        ......
    }
    ```

1.  Inherit `MCU_M480` target, which **NuMaker-M483KG** board bases on:
    ```json55
    "inherits": [
        "MCU_M480"
    ],
    ```

1.  Specify `device_name` (MCU part name) for **NuMaker-M483KG** board:
    ```json55
    "device_name": "M483KGCAE2A",
    ```

    > **ℹ️ Information**
    >
    > Comment out this line if your meet build error with:
    > ERROR: Target specifies device_name <device_name> but this device is not
    > listed cmsis_mcu_descriptions.json5

1.  Specify `detect_code` for **NuMaker-M483KG** board. This can acquire by running `mbedls` when the board is connected to host through USB cable:
    ```json55
    "detect_code": [
        "1301"
    ],
    ```

1.  Change pin names by providing custom `PinNames.h`:
    1.  Create `TARGET_NUMAKER_M483KG` directory.
    1.  Copy `mbed-os/targets/TARGET_NUVOTON/TARGET_M480/TARGET_NUMAKER_PFM_M487/PinNames.h` there.
    1.  Adapt `TARGET_NUMAKER_M483KG`/`PinNames.h` to **NuMaker-M483KG** board.

    > **ℹ️ Information**
    >
    > For consistency and correctness, change UART configurations for USB VCOM and STDIO via target configuration options as below.
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
    ```json55
    "overrides": {
        ......
        "usb-uart": "UART_0",
        "usb-uart-tx": "PB_13",
        "usb-uart-rx": "PB_12",
        ......
    },
    ```

1.  **NuMaker-M483KG** board doesn't support SPIM CCM mode. Disable it:
    ```json55
    "overrides": {
        ......
        "spim-ccm-enable": 0,
        ......
    },
    ```

1.  **NuMaker-M483KG** board doesn't have network interface. Disable it:
    ```json55
    "overrides": {
        ......
        "network-default-interface-type": null
        ......
    },
    ```

1.  Change memory specification for **NuMaker-M483KG** board:
    ```json55
    "memory_banks": {
        "IROM1": {
            "access": {
                "execute": true,
                "non_secure": false,
                "non_secure_callable": false,
                "peripheral": false,
                "read": true,
                "secure": false,
                "write": false
            },
            "default": true,
            "size": 0x40000,
            "start": 0x0,
            "startup": true
        },
        "IRAM1": {
            "access": {
                "execute": false,
                "non_secure": false,
                "non_secure_callable": false,
                "peripheral": false,
                "read": true,
                "secure": false,
                "write": true
            },
            "default": true,
            "size": 0x20000,
            "start": 0x20000000,
            "startup": false
        }
    }
    ```

    **Note**: Check M480 Selection Code in M480 datasheet for actual memory size.

1.  Compile with cmake/ninja
    ```
    $ mkdir build; cd build
    $ cmake .. -GNinja -DCMAKE_BUILD_TYPE=Develop -DMBED_TARGET=NUMAKER_M483KG
    $ ninja
    $ cd ..
    ```

1.  Flash by drag-n-drop built image `NuMaker-mbed-ce-CustomBoard-example.bin` or `NuMaker-mbed-ce-CustomBoard-example.hex` onto **NuMaker-M483KG** board

1.  We should see the red LED on **NuMaker-M483KG** board is blinking. On host terminal (115200/8-N-1), we should see messages:
    ```
    blinking: 0
    blinking: 1
    blinking: 2
    blinking: 3
    ```
