/**
  @page S2868A2_UDP_Client application for S2-LP Expansion Board and STM32 Nucleo Boards

  @verbatim
  ******************************************************************************
  * @file    UDP Client/readme.txt
  * @author  SRA Application Team
  * @brief   This application provides an example on how to send messages using
  *          UDP protocol to a UDP Server.
  ******************************************************************************
  *
  * Copyright (c) 2020 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under ODE Software License Agreement
  * SLA0094, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0094
  *
  ******************************************************************************
   @endverbatim

@par Example Description

Application sending messages via UPD to a UDP Server.
------------------------------------
WARNING: When starting the project from Example Selector in STM32CubeMX and regenerating it
from ioc file, you may face a build issue because the Contiki-NG middleware is copied in a 
different folder and all the files are duplicated. To solve it, please delete the project folder of the 
IDE that you are using (EWARM, MDK-ARM or STM32CubeIDE).
Other duplicate files that you should delete, if you started the project for the
Nucleo-F401RE board, from Src and Inc folders, are: stm32f4xx_nucleo.c, stm32f4xx_nucleo.h
and stm32f4xx_nucleo_errno.h.
The same operations apply if you starts the project for another STM32 Nucleo board (e.g. for
Nucleo-L053R8, the files to be removed are stm32l0xx_nucleo.c, stm32l0xx_nucleo.c, stm32l0xx_nucleo.h
and stm32l0xx_nucleo_errno.h).
------------------------------------
This sample application shows how to use S2-LP radio and Contiki-NG OS to enable
UDP communication over 6LoWPAN network.
This sample application configures a node to periodically send a message to a
receiver (UDP Server).

To test this application you need:
- one STM32 Nucleo board with its X-NUCLEO-S2868A1 or X-NUCLEO-S2868A2 or
  X-NUCLEO-S2915A1 expansion board.
- a host PC running Windows or Linux (optional to print output to a serial line
  monitor)

On the STM32 Nucleo board this sample application for Udp Client must be running.
Another STM32 Nucleo board with its X-NUCLEO-S2868A1 or X-NUCLEO-S2868A2 or
X-NUCLEO-S2915A1 STM32 expansion board (running the Udp Server sample application
included in this package) should be used to test the communication via UDP
over 6LoWPAN.

@par Keywords

Contiki-NG, 6LoWPAN, SubGHz_Phy, SPI, SUBGHZ, UART, TIM

@par Directory contents

 - app_contiki-ng.c       This file provides interface between the main.c and
                          Contiki-NG based applications.
 
 - main.c                 Main program body.
 
 - stm32**xx_hal_msp.c    This file provides code for the MSP Initialization
                          and de-Initialization.
						
 - stm32**xx_nucleo_bus.c Source file for the BSP BUS IO driver.
 
 - udp-client.c           Source file for the UDP Client Contiki-NG Thread.
 
 - contiki-platform.c     This file provides the initialization for Contiki-NG.
 
 - radio-driver.c         This file provides the binding between Contiki-NG
                          network API and S2-LP radio.
 
 - stm32**xx_it.c         Source code for interrupt Service Routines.
 
 - subg2_contiki_it.c     Source code for interrupt Service Routines managed
                          within Contiki-NG based application.
						  
 - csma-output.c          Slightly modified version of Contiki-NG CSMA MAC
                          output (transmission) code.
						  
 - s2lp_interface.c       Initialization for S2-LP radio.
 
 - stm32**xx_nucleo.c     Source file for the BSP Common driver
 
 - system_stm32**xx.c     CMSIS Cortex-M4 Device Peripheral Access Layer
                          System Source File.



@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with X-NUCLEO-S2868A1 or
    X-NUCLEO-S2868A2 or X-NUCLEO-S2915A1 STM32 expansion board
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L152RE RevC board
    - NUCLEO-F401RE RevC board
    and can be easily tailored to any other supported device and development board.

ADDITIONAL_BOARD : X-NUCLEO-S2868A2 https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-connect-hw/x-nucleo-s2868a2.html
ADDITIONAL_COMP : S2-LP https://www.st.com/content/st_com/en/products/wireless-connectivity/long-range/proprietary-sub-1-ghz-products/s2-lp.html

@par How to use it?

In order to make the program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open STM32CubeIDE toolchain (see the release note for detailed information about the version).
   Alternatively you can use the Keil uVision toolchain (see the release note for detailed information about the version).
   Alternatively you can use the IAR (see the release note for detailed information about the version).
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package. 
   
 - IMPORTANT NOTE: To avoid issues with USB connection (mandatory if you have USB 3.0), it is   
   suggested to update the ST-Link/V2 firmware for STM32 Nucleo boards to the latest version.
   Please refer to the readme.txt file in the Applications directory for details.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
