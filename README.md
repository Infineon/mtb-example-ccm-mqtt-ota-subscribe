# AIROC™ CCM MQTT-OTA-SUBSCRIBE

This code example demonstrates MQTT Subscribe and OTA from AWS IoT Core with the help of the Cloud Connectivity Manager (CCM) evaluation kit. In this code example, the host receives the message whenever any new message gets published in the 'data' topic from AWS IoT core. Then the host displays it in the serial terminal and at the same time allows OTA upgrade of CCM module attached to it whenever a new OTA upgrade is available for CCM.

The CCM module is a configurable Wi-Fi connectivity module that provides a simple, secure, plug-and-play solution for connecting products to AWS IoT cloud services. The CCM module is preprogrammed with a tested secured firmware and supports an easy-to-use AT command interface for configuration.

The preprogrammed firmware supports AWS IoT ExpressLink specification. For more details, see the [AWS IoT ExpressLink programmer's guide](https://docs.aws.amazon.com/iot-expresslink/latest/programmersguide/elpg.html).

[View this README on GitHub.](https://github.com/Infineon/mtb-example-ccm-mqtt-ota-subscribe)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMzYxOTAiLCJTcGVjIE51bWJlciI6IjAwMi0zNjE5MCIsIkRvYyBUaXRsZSI6IkFJUk9D4oSiIENDTSBNUVRULU9UQS1TVUJTQ1JJQkUiLCJyaWQiOiJ2ZWxtdXJ1Z2FuIiwiRG9jIHZlcnNpb24iOiIyLjAuMSIsIkRvYyBMYW5ndWFnZSI6IkVuZ2xpc2giLCJEb2MgRGl2aXNpb24iOiJNQ0QiLCJEb2MgQlUiOiJJQ1ciLCJEb2MgRmFtaWx5IjoiV0lGSSJ9)

## Requirements

- [ModusToolbox&trade; software](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software) v3.0 or later (tested with v3.0)
- Board support package (BSP) minimum required version: 4.0.0
- Programming language: C
- Associated parts: All [PSoC&trade; 6 MCU](https://www.infineon.com/cms/en/product/microcontroller/32-bit-psoc-arm-cortex-microcontroller/psoc-6-32-bit-arm-cortex-m4-mcu) parts


## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm® embedded compiler v10.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`
- Arm&reg; compiler v6.13 (`ARM`)
- IAR C/C++ compiler v8.42.2 (`IAR`)


## Supported kits (make variable 'TARGET')

- [PSoC&trade; 6 Wi-Fi Bluetooth&reg; prototyping kit](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-064b0s2-4343w) (`CY8CPROTO-062-4343W`) – Default value of `TARGET`
- [PSoC&trade; 6 Wi-Fi Bluetooth&reg; pioneer kit](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-062-wifi-bt) (`CY8CKIT-062-WIFI-BT`)
- [PSoC&trade; 62S2 Wi-Fi Bluetooth&reg; pioneer kit](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-062s2-43012) (`CY8CKIT-062S2-43012`)
- [PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; pioneer kit](https://www.infineon.com/cms/en/product/evaluation-boards/cyw9p62s1-43438evb-01) (`CYW9P62S1-43438EVB-01`)
- [PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; pioneer kit](https://www.infineon.com/cms/en/product/evaluation-boards/cyw9p62s1-43012evb-01) (`CYW9P62S1-43012EVB-01`)
- [PSoC&trade; 64 "Secure Boot" Wi-Fi Bluetooth&reg; pioneer kit](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-064b0s2-4343w) (`CY8CKIT-064B0S2-4343W`)
- [PSoC&trade; 62S2 evaluation kit](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ceval-062s2) (`CY8CEVAL-062S2`)
- [PSoC&trade; 64 "Secure Boot" prototyping kit](https://www.infineon.com/cms/en/product/evaluation-boards/cy8cproto-064s1-sb) (`CY8CPROTO-064S1-SB`)

## Hardware setup

Attach Arduino UNO adapter with PSoC&trade; 62 kit and stack CCM evaluation kit to the Arduino UNO adapter as shown in Figure 1.

   **Figure 1. PSoC&trade; 62 with Arduino UNO adapter and CCM evaluation kit**


   ![](images/image.png)


**Note:** The PSoC&trade; 6 Bluetooth&reg; LE pioneer kit (CY8CKIT-062-BLE) and the PSoC&trade; 6 Wi-Fi Bluetooth&reg; pioneer kit (CY8CKIT-062-WIFI-BT) ship with KitProg2 installed. The ModusToolbox&trade; software requires KitProg3. Before using this code example, make sure that the board is upgraded to KitProg3. The tool and instructions are available in the [Firmware Loader](https://github.com/Infineon/Firmware-loader) GitHub repository. If you do not upgrade, you will see an error like "unable to find CMSIS-DAP device" or "KitProg firmware is out of date".


## Software setup

Install a terminal emulator if you do not have one. Instructions in this document use [PuTTY](https://www.putty.org/).

This example requires no additional software or tools.

**Note:**
Choose "implicit CR in every LF" and "implicit LF in every CR" in PuTTY Terminal Configuration.


## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade; software</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**). This launches the [Project Creator](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Project_Creator_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcabbd31e5&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Library_Manager_User_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99ab34b831ce&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. (Optional) Change the suggested **New Application Name**.

5. The **Application(s) Root Path** defaults to the Eclipse workspace which is usually the desired location for the application. If you want to store the application in a different location, you can change the *Application(s) Root Path* value. Applications that share libraries should be in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-Eclipse_IDE_for_ModusToolbox_User_Guide_1-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcb86331e8&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files) (locally available at *{ModusToolbox&trade; software install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details><summary><b>In command-line interface (CLI)</b></summary>

ModusToolbox&trade; software provides the Project Creator as both a GUI tool and the command line tool, "project-creator-cli". The CLI tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the "project-creator-cli" tool. On Windows, use the command line "modus-shell" program provided in the ModusToolbox&trade; software installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; software tools. You can access it by typing `modus-shell` in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

This tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the `<id>` field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the `<id>` field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

<br>

The following example will clone the "[mtb-example-ccm-mqtt-ota-subscribe](https://github.com/Infineon/mtb-example-ccm-mqtt-ota-subscribe)" application with the desired name "CcmMqttSubscribeLED" configured for the *CY8CPROTO-062-4343W* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CY8CPROTO-062-4343W --app-id mtb-example-ccm-mqtt-subscribe-ledbrightness --user-app-name CcmMqttSubscribeLED --target-dir "C:/mtb_projects"
   ```

**Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_2.4_User_Guide-Software-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017ed97e72563632&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details><summary><b>In third-party IDEs</b></summary>

Use one of the following options:

- **Use the standalone [Project Creator](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Project_Creator_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcabbd31e5&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files) tool:**

   1. Launch Project Creator from the Windows Start menu or from *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/project-creator.exe*.

   2. In the initial **Choose Board Support Package** screen, select the BSP, and click **Next**.

   3. In the **Select Application** screen, select the appropriate IDE from the **Target IDE** drop-down menu.

   4. Click **Create** and follow the instructions printed in the bottom pane to import or open the exported project in the respective IDE.

<br>

- **Use command-line interface (CLI):**

   1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and then import the libraries using the `make getlibs` command.

   2. Export the application to a supported IDE using the `make <ide>` command.

   3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_2.4_User_Guide-Software-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017ed97e72563632&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation


If using a PSoC&trade; 64 "Secure" MCU kit (such as CY8CKIT-064B0S2-4343W), the PSoC&trade; 64 device must be provisioned with keys and policies before being programmed. Follow the instructions in the ["Secure Boot" SDK user guide](https://www.infineon.com/dgdlac/Infineon-PSoC_64_Secure_MCU_Secure_Boot_SDK_User_Guide-Software-v07_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0f8c361a7666&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-software) to provision the device. If the kit is already provisioned, copy-paste the keys and policy folder to the application folder.

Two types of flow can be followed in this code example.

1. AWS flow 
2. Infineon CIRRENT&trade; cloud flow

To know more details about the two flows, see the [AN234322 - Getting started with AIROC&trade; IFW56810 Single-band Wi-Fi 4 Cloud Connectivity Manager](https://www.infineon.com/dgdl/Infineon-AN234322_-_Getting_Started_with_AIROC_IFW56810_Single-band_Wi-Fi_4_Cloud_Connectivity_Manager-ApplicationNotes-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017e90db764f0c6b&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-application_note).

<b>AWS flow</b>

**Note:** See section 7.1.2 AWS flow in the [AN234322 - Getting started with AIROC&trade; IFW56810 Single-band Wi-Fi 4 Cloud Connectivity Manager](https://www.infineon.com/dgdl/Infineon-AN234322_-_Getting_Started_with_AIROC_IFW56810_Single-band_Wi-Fi_4_Cloud_Connectivity_Manager-ApplicationNotes-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017e90db764f0c6b&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-application_note) for creating a "Thing" in AWS console using the output you receive from the terminal.

1. Define #define AWS_FLOW as 1.

2. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

3. Connect the CCM evaluation kit to Wi-Fi using either of the following steps:


    a.)  Modify the `SET_SSID` and `SET_PASSPHRASE` macros in *main.c* according to your Wi-Fi credentials.


                                 or

     b.)  The CIRRENT&trade; cloud solution from Infineon provides an effective Wi-Fi onboarding service through the **Cirrent Wi-Fi Onboarding** app. The CCM evaluation kit supports Wi-Fi onboarding through either Bluetooth® LE or SoftAP. 

      Download and install the **Cirrent Wi-Fi Onboarding** app from Google Play Store for Android or iOS App Store for iOS on your mobile phone.

      Define the `CIRRENT_APP_ONBOARDING` macro as **1** in *main.c*.

         Example: #define CIRRENT_APP_ONBOARDING 1


4. **MQTT_Endpoint configuration:** Modify the `SET_ENDPOINT` macro in *main.c* to match with that of the MQTT broker endpoint of your AWS console.

5. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud.

6. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE for ModusToolbox&trade; software</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
   </details>

   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain and target are specified in the application's Makefile but you can override those values manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TARGET=CY8CPROTO-062-4343W TOOLCHAIN=GCC_ARM
      ```
   </details>

7. After programming, the application starts automatically. Confirm that "AIROC™ CCM MQTT-OTA-SUBSCRIBE" is displayed on the UART terminal.


   **Figure 2. Terminal output on program startup**

      ![](images/aws-flow.png)


8. Confirm the data published from the "data" topic in AWS IoT Core is displayed in serial terminal.


<b>Infineon CIRRENT&trade; cloud flow</b>

**Note:** Use the quick start guide received along with your kit box for the initial setup.

See section 7.1.1 CIRRENT&trade; cloud flow in the [AN234322 - Getting started with AIROC&trade; IFW56810 Single-band Wi-Fi 4 Cloud Connectivity Manager](https://www.infineon.com/dgdl/Infineon-AN234322_-_Getting_Started_with_AIROC_IFW56810_Single-band_Wi-Fi_4_Cloud_Connectivity_Manager-ApplicationNotes-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017e90db764f0c6b&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-application_note) for binding the kit to your CIRRENT console.

1. Define #define AWS_FLOW as 0.

2. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

3. Connect the CCM evaluation kit to Wi-Fi using either of the following steps:


    a.)  Modify the `SET_SSID` and `SET_PASSPHRASE` macros in *main.c* according to your Wi-Fi credentials.


                                 or

     b.)  The CIRRENT&trade; cloud solution from Infineon provides an effective Wi-Fi onboarding service through the **Cirrent Wi-Fi Onboarding** app. The CCM evaluation kit supports Wi-Fi onboarding through either Bluetooth® LE or SoftAP. 

      Download and install the **Cirrent Wi-Fi Onboarding app** from Google Play Store for Android or iOS App Store for iOS on your mobile phone.

      Define the `CIRRENT_APP_ONBOARDING` macro as **1** in *main.c*.

         Example: #define CIRRENT_APP_ONBOARDING 1

4. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud.

5. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE for ModusToolbox&trade; software</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
   </details>

   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain and target are specified in the application's Makefile but you can override those values manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TARGET=CY8CPROTO-062-4343W TOOLCHAIN=GCC_ARM
      ```
   </details>

6. After programming, the application starts automatically. Confirm that "AIROC™ CCM MQTT-OTA-SUBSCRIBE" is displayed on the UART terminal.



   **Figure 3. Terminal output on program startup**

      ![](images/cirrent-flow.png)


7. Confirm the data published from the "data" topic in AWS IoT Core is displayed in Serial terminal.

**Note:**
- See section 9 "Performing firmware over-the-air update" in the [AN234322 - Getting started with AIROC&trade; IFW56810 Single-band Wi-Fi 4 Cloud Connectivity Manager](https://www.infineon.com/dgdl/Infineon-AN234322_-_Getting_Started_with_AIROC_IFW56810_Single-band_Wi-Fi_4_Cloud_Connectivity_Manager-ApplicationNotes-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017e90db764f0c6b&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-application_note) for doing OTA upgrade via AWS IoT Core.
- Send the **AT+FACTORY_RESET** command to the CCM device before changing from one flow to another flow.
- Similarly, send the **AT+RESET** command to the CCM device while changing from one endpoint to another.
- While porting to non PSoC&trade; microcontrollers, change UART TX/RX API's and bsp specific API's present in the *CCM.c* file to your microcontroller specific APIs.

## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-Eclipse_IDE_for_ModusToolbox_User_Guide_1-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcb86331e8&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files).

**Note:** **(Only while debugging)** On the CM4 CPU, some code in `main()` may execute before the debugger halts at the beginning of `main()`. This means that some code executes twice – once before the debugger stops execution, and again after the debugger resets the program counter to the beginning of `main()`. See [KBA231071](https://community.infineon.com/t5/Knowledge-Base-Articles/PSoC-6-MCU-Code-in-main-executes-before-the-debugger-halts-at-the-first-line-of/ta-p/253856) to learn about this and for the workaround.


## Design and implementation

In this application, the host controller configures the CCM module to receive any new message getting published in the topic 'data' from AWS IoT core and host receieves it from CCM evaluation kit for displaying it in the serial terminal. Then the host controller also allows the CCM module to do OTA upgrade whenever new version of CCM firmware available in AWS IoT core.

### Resources and settings

**Table 1. Application resources**

 Resource  |  Alias/object     |    Purpose
 :-------- | :-------------    | :------------
 UART (HAL)|cy_retarget_io_uart_obj| UART HAL object used by retarget-io for the debug UART port
 UART (HAL)    | uart_obj     | UART HAL object used for sending AT commands

<br>

## Related resources

Resources  | Links
-----------|----------------------------------
Application notes  | [AN228571](https://www.infineon.com/dgdl/Infineon-AN228571_Getting_started_with_PSoC_6_MCU_on_ModusToolbox_software-ApplicationNotes-v06_00-EN.pdf?fileId=8ac78c8c7cdc391c017d0d36de1f66d1&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-application_note) – Getting started with PSoC&trade; 6 MCU on ModusToolbox&trade; software <br>  [AN215656](https://www.infineon.com/dgdl/Infineon-AN215656_PSoC_6_MCU_Dual-CPU_System_Design-ApplicationNotes-v09_00-EN.pdf?fileId=8ac78c8c7cdc391c017d0d3180c4655f&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-application_note) – PSoC&trade; 6 MCU: Dual-CPU system design <br> 
Code examples | [Using ModusToolbox&trade; software](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Device documentation | [PSoC&trade; 6 MCU datasheets](https://www.infineon.com/cms/en/search.html#!view=downloads&term=PSoC%206&doc_group=Data%20Sheet) <br> [PSoC&trade; 6 technical reference manuals](https://www.infineon.com/cms/en/search.html#!view=downloads&term=PSoC%206&doc_group=Additional%20Technical%20Information)
Development kits |Select your kits from the [Evaluation board finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board) page
Libraries on GitHub  | [mtb-pdl-cat1](https://github.com/Infineon/mtb-pdl-cat1) – Peripheral driver library (PDL)  <br> [mtb-hal-cat1](https://github.com/Infineon/mtb-hal-cat1) – Hardware abstraction layer (HAL) library <br> [retarget-io](https://github.com/Infineon/retarget-io) – Utility library to retarget STDIO messages to a UART port
Tools  | [Eclipse IDE for ModusToolbox&trade; software](https://www.infineon.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use software and tools enabling rapid development with Infineon MCUs, covering applications from embedded sense and control to wireless and cloud-connected systems using AIROC&trade; Wi-Fi and Bluetooth® connectivity devices.
<br>

## Other resources


Infineon provides a wealth of data at www.infineon.com to help you select the right device, and quickly and effectively integrate it into your design.

For PSoC&trade; 6 MCU devices, see [How to design with PSoC&trade; 6 MCU - KBA223067](https://community.infineon.com/t5/Knowledge-Base-Articles/How-to-Design-with-PSoC-6-MCU-KBA223067/ta-p/248857) in the Infineon community.


## Document history
	
   
Document title: *CE236190* - *AIROC™ CCM MQTT-OTA-SUBSCRIBE*
 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example
 1.1.0   | BSP update
 1.1.1   | Removed CY8CKIT-062-BLE from supported kits
 2.0.0   | Major update to support ModusToolbox™ software v3.0
 2.0.1   | CCM library fix 
<br>

---------------------------------------------------------

© Cypress Semiconductor Corporation, 2023. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolbox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit www.infineon.com. Other names and brands may be claimed as property of their respective owners.
