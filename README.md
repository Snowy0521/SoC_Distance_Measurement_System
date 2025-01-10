# SoC Distance Measurement System 

This project aims to create a distance measurement system using the [SpartanMC](https://www.spartanmc.de/?node=b938&page=processor&ln=de) processor, optimized for FPGAs, on the [SP601](https://docs.rs-online.com/04ff/0900766b80dc6119.pdf) FPGA board. The SpartanMC features a unique 18-bit instruction and data width tailored for modern FPGA architectures. The [SpartanMC SoC kit](https://spartanmc.de/?node=5008&page=sockit&ln=de)
built around the SpartanMC provides various perpherals and software tools for development.

---

## Table of Contents

1. [Designflow](#designflow)
2. [Display (SPI)](#display-spi)
3. [Distance Measurement (I2C)](#distance-measurement-i2c)
4. [Median filter](#median-filter)
5. [Integration and Interrupt](#integration-and-interrupt)

---

## Designflow
This distance measurement system utilizes an [SFR02 ultrasonic sensor](https://www.mikrocontroller.net/attachment/564267/srf02doku_dr.pdf) for distance detection, with results displayed on an [OLED Display NHD-2.8-25664UCB2](https://newhavendisplay.com/de/content/specs/NHD-2.8-25664UCB2.pdf) from Newhaven Display. The design follow is a three-layer SoC model:

1. Physical Layer: includes visible hardware components such as the SP601 FPGA board, the ultrasonic sensor, and the OLED display.

2. Soft-Core Layer: comprises Verilog/VHDL modules, SpartanMC processor and peripheral interfaces such as UART, I2C, and Serial Peripheral Interface (SPI).

3. Firmware Layer: manages communication and processing tasks within the system using C code.

This system is developed as three distinct subsystems-display (SPI), distance measurement (I2C), and filtering (Medianfilter). After developing them, the three subsystems are then integrated into the final system.

---

## Display (SPI)
+ To put the OLED display into operation via SPI. First, a SoC must be set up using the [JConfig System builder](https://spartanmc.de/?node=69b9&page=builder&ln=de), which consists of the following modules:
    + SpartanMC Core
    + [Xilinx DCM Clock](https://www.xilinx.com/products/intellectual-property/dcm_module.html#documentation)
    + SpartanMC Local Memory
    + SPI Master
    + Output port (must be connected to the reset port of the display)


+ An [SPI master](https://www.spartanmc.de/docs/manual/manual.pdf) (chapter 8 for more details) can be connected with up to 15 SPI-slaves which share 3 wires:
    - SCLK ( serial clock )
	- MOSI ( master out slave in )
	- MISO ( master in slave out )

+ The SPI peripheral provides four 18-bit registers
    - *spi_control*: Contains the current SPI settings e.g. CPOL, CPHA, clock divider etc.
    - *spi_data_out*: Register for outgoing data.
    - *spi_data_in*: Register for incoming data.
    - *spi_status*: Status register

+ To make the communication between SPI master and SPI slave (OLED Desplay), the **fpga_pins.pdf** provides information on which pins the display should be connected to, and the core code is provided in **spi_driver.c** and more infos in our report **SoC_Versuch_Gruppe02.pdf**

---

## Distance Measurement (I2C)
+ To measure a distance and output the result to PC console, the SRF02 ultrasonic sensor is to be put into operation. This must be operated in I2C mode. The necessary pin assignment for the sensor can also be found in **fpga_pins.pdf**. So a second system with the following components must be set up:
    * SpartanMC Core
    * Xilinx DCM Clock
    * SpartanMC Local Memory
    * [I2C Master]((https://www.spartanmc.de/docs/manual/manual.pdf)) (chapter 9)
    * [UART Light](https://www.spartanmc.de/docs/manual/manual.pdf) (chapter 7)

+ The firmware code must be programmed so that it starts a measurement process, reads the current distance value from the sensor and sends it to the PC via **uart_light_driver**.

+ I2C (two-wire interface) is a serial bus which allows for connection of multiple master devices to multiple slave devices, only using two single bidirectional lines:
	+ SCL ( serial clock line )
	+ SDA ( serial data line )

+ The I2C master controller can be configured and controlled by setting the writable registers such as the Control, Command and Data registers on the SpartanMC side with flags, commands, slave addresses or data to be sent.

+ To make the communication between I2C master and I2C slave (SF02 sensor), the core code is provided in **i2c_driver.c** and more infos in **SoC_Versuch_Gruppe02.pdf**, **ultraschallsensor_i2c.pdf** and [SpartanMC
Users Manual Chapter 9](https://www.spartanmc.de/docs/manual/manual.pdf).

---

## Median filter
+ The median filter calculates the median from the last N measurements and it is effective to remove outliers.
+ To find the median value from N measurements, the measurments are first sorted by [bubble sort](https://en.wikipedia.org/wiki/Bubble_sort) or [quicksort](https://www.youtube.com/watch?v=Vtckgz38QHs&ab_channel=BroCode), the code is in **sort_algs.c**. *Note: In our case, setting N to 3 is enough, so bubble sort is used.*

---

## Integration and Interrupt
+ To integrate the distance measurment and display functions and effectively ultilize computational power of the CPU, it is necessary to implement it through an interrupt controller.

+ So when an interrupt occurs, the controller completes the execution of the current instruction and starts the execution of an interrupt service routine (ISR).

+ Hardware Interrupt verus Software Interrupt
    + Hardware: 
        + An electronic signal from external device sent to processor. for example, pressing key on the keybroad.
    + Software:
        + A special instruction in the instruction set causes an interrupt.
        + For example, a command to divide a number by zero that cause a divide-by-zero exception

+ Interrupts of I2C master controller
	+ When the I2C controller receives data, it can trigger an interrupt, to notify the main processor that new data is available.
	+ When the I2C controller has completed a data transfer, it can generate an interrupt to inform the main processor that the transfer is completed
	+ If an error occurs during an I2C communication, the I2C controller can trigger an interrupt to inform the main processor of the error.

+ We have used a state machine for the implmentation of ISR, details in **i2c_driver.c**

---

