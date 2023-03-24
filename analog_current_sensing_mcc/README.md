<!-- Please do not change this logo with link -->
[![MCHP](../images/microchip.png)](https://www.microchip.com)

# Low-Power AVR EA Current Measurement

This example shows how to use the AVR EA microcontoller to measure a current by using the Analog-to-Digital (ADC) peripheral. The only hardware needed in addition to the microcontroller are two 100 kΩ resistors and one 10 kΩ resistor.  

## Hardware Used

- [AVR64EA48 Curiosity Nano](https://www.microchip.com/en-us/development-tool/EV66E56A)
- 10 kΩ Resistor
- 2 × 100 kΩ Resistor  

## Test Setup

![HW Wiring](../images/current_measure-resistor_setup.png)

- PORTA Pin 7 is set as output (only used as debug pin)
- PORTD Pin 6 is DAC0 Out, connected to R<sub>1</sub>
- PORTD Pin 1 is ADC0 Positive Input 1, connected to R<sub>SENSE</sub>
- PORTD Pin 0 is ADC0 Negative Input 0, connected after R<sub>SENSE</sub>

## Peripherals Configuration using MCC/Melody

Add the required peripherals: ADC0, DAC0, RTC, UART1 and SLPCTRL. These are found under Drivers in the Device Resources window. Press the green "plus" sign to add to Project Resources, this will also make the peripheral show in the Builder view. Note that the peripheral VREF is added automatically when ADC0 or DAC0 is added, since these peripherals require the VREF peripheral. These System peripherals are added automatically: CLKCTRL, Configuration Bits, Interrupt Manager, Main and Pins.  

![MCC - Adding Peripherals](../images/csense_AddP.png)

### System Configuration: CLKCTRL

- Set Clock Selection to "Internal high-frequency oscillator"
- Disable the prescaler by turning off the "Prescaler Enable" button (main clock = 20 MHz)
- Set Timebase to 20 (20 / 20 MHz = 1 µs)  

![MCC - CLKCTRL](../images/vsense_CLKCTRL.png)

### System Configuration: Interrupt Manager

- Enable Global interrupt by turning on the "Global Interrupt enable" button  

 ![MCC - Interrupt Manager](../images/interrupt.png)

### System Configuration: SLPCTRL

- Enable sleep by turning on the "Enable Sleep" button
- Under Sleep Mode, select "PDOWN"  

![MCC - SLPCTRL](../images/vsense_SLPCTRL.png)

### System Configuration: Pin Config

If Pin Config window does not appear, click the "Pin Grid View" tab at the bottom as indicated by the red arrow.

- PORTC Pin 0 is used by USART1 as TXD output
- PORTD Pin 0 and 1 is used by the ADC0 as analog inputs
- PORTD Pin 6 is used by the DAC0 an output  

![MCC - Pin Config](../images/csense_PINS.png)

- In the Pins configuration, PORTB pin 3 is renamed to "LED0" to indicate this pin is connected to LED0 on the curisity nano board
- For all pints the ISC is set to "Digital Input Buffer disabled"  

![MCC - Pin Config2](../images/csense_PINS2.png)  

### Peripherals Configuration: VREF Configuration

Both the DAC0 and the ADC0 is using the VREF peripheral.  

- Set VDD to 3.3V by inputing 3.3 into the VDD field
- Enable Force DAC Voltage reference by turning on the "Enable Force Dac Voltage Reference" button
- Set DAC Voltage Reference source to VDD as reference  

![MCC - VREF](../images/vref.png)

### Peripherals Configuration: RTC Configuration

- Enable the RTC by turning on the "Enable RTC" button
- Select "Internal 32 kHz Oscillator divided by 32" clock select
- Enable Periodic Interrupt Timer by turning on the "PIT Enable" button
- Under Periodic Interrupt Timer set Period Selection to "RTC Clock Cycles 1024"  

![MCC - RTC](../images/RTC.png)

### Peripherals Configuration: DAC0 Configuration

- DAC0 must be enabled by turning on the "Enable DAC" button
- Enable Output on DAC by turning on the "Enable Output on DAC" button
- Set required voltage to 1.0V  

![MCC - DAC0](../images/DAC0.png)

### Peripherals Configuration: ADC0 Configuration

In ADC Clock Settings:  

- Select Prescaler Value "System Clock Divided by 10"
- Sample Duration 12  

In Hardware Settings:  

- Enable ADC0 by turning on the "ADC Enable" button
- Set mode to "BURST"
- Enable differential mode by turning on the "Differential mode" button
- Set Sample numbers to 16 samples accumulated
- Set Start command to start a conversion immediatley
- Set Reference select to "VDD" (in this example VDD is 3.3V)
- Set first analog channel selection bits (MUXPOS) to "ADC input pin 1"
- Set second analog channel selection bits (MUXNEG) to "ADC input pin 0"
- Set Analog inputs Via to "Inputs connected via PGA"
- Set Sign Chopping to "Enable"

In PGA Control Settings:  

- Enable PGA by turning on the "Enable PGA" button
- Set PGA BIAS Select to 100 % BIAS current
- Set Gain to 16x gain  

![MCC - ADC0](../images/ADC0.png)

### Peripherals Configuration: USART1 Configuration

We are using USART1 because the Rx and Tx pins of USART1 are accessible over the USB interface for the Curiosity Nano board. See the cnano user manual for more information.

In the "USART1_Peripheral" configuration:

- Turn on the Enable USART Receiver button
- Turn on the Enable USART Transmitter button

![MCC - USART1-1](../images/csense_USART1_1.png)

In the "UART1" configuararion:

- For Requested Baudrate write "115200"  

For the rest we use the default settings, so 8 data bits, 1 stop bit, no parity and no flow control.  

![MCC - USART1-1](../images/csense_USART1_2.png)

### Adding support for sleep functions

The sleep instruction is issued to the device by "SLEEP" in assembler. Since MPLAB X is using C language, we need to define this in a function. This is done by including the file "sleep.h" from the compiler include folder structure.  

Open the file "system.h" and enter "#include <avr/sleep.h>" after the other "#includes" like shown in the image:  

![MCC - includesleep](../images/vsense_SLPCTRL_include_sleep.png)  

### Adding support for math functions

Some of the functions used to generate a character buffer from a floating point number (float) needs the "math.h" header file. This is included by adding the following to the beginning of your "main.c" file:

![MCC - includemath](../images/csense_include_math.png)  

## Setup

- Connect the hardware together as seen in the schematic shown in the _Setup_ section  

## Operation

- Connect the AVR6448 Curiosity Nano to a computer using a USB cable
- Clone the repository or download the zip to get the source code
- Open the project folder you want to run with MPLAB® X
- Compile and run the code  

## Output to terminal

This example adds a terminal output using the USART1 interface built into the CNANO board USB connection (for more details see the cnano user guide found here: [AVR64EA48 device page](https://www.microchip.com/wwwproducts/en/AVR64EA48)).  

Open a terminal application. This example uses MPLAB® Data Visualizer standalone version, but any terminal application can be used.

![MCC - terminal](../images/csense_terminal.png)  

Follow these steps to see the USART1 output. Note that this example uses COM3, but this may differ as COM ports are assigned automatically by windows:

1. Expand the "AVR64EA48 Curiosity Nano" section
2. Left-click on the "COMx" section
3. Set Baud Rate to: 115200
4. In source, select "COMx on AVR64EA48 Curiosity Nano"
5. Press the "PLAY" symbol (arrow head pointing right) to start streaming

If you are not seeing text, you may need to select "8-bit ASCII" from the "Display As" drop down menu.

## Measurement Calculation

Some sensors, like photodiodes, phototransistors and some temperature sensors, will output a current signal. Use the 12-bit ADC peripheral to measure the signal coming from such sensors. The ADC can only convert voltages. To measure a current, send the current through a "sense" resistor and measure the voltage drop across it. Then use this to calculate the current.  

This example will use the following circuit to measure the current:

![HW Wiring](../images/current_measure-resistor_setup.png)

where:  

<!-- If the markdown viewer supports math functions you can replace image by this equation
$$
R_{1} = R_{2} = 100k\Omega
$$
$$
R_{SENSE} = 10k\Omega
$$
$$
V_{0} = 1V
$$
-->

![definitions_resistors](../images/definitions_for_resistor_setup.png)

Use the Digital-to-Analog Converter (DAC) peripheral as "current source". Although this will give a defined voltage signal V<sub>0</sub>, the resistors in the circuit will create a current I<sub>0</sub> that is given by:

<!-- If the markdown viewer supports math functions you can replace image by this equation
$$
I_{0} =  \frac{V_{0}} {R_{1} + R_{SENSE} + R_{2}}
$$
-->
![IO_eq1](../images/I_0_equation1.png)

The voltage divider rule gives the following equation for the voltage drop across R<sub>SENSE</sub>:

<!-- If the markdown viewer supports math functions you can replace image by this equation
$$
V_{SENSE} = V_{0} \left ( \frac{R_{SENSE}} {R_{1} + R_{SENSE} + R_{2}} \right)
$$
-->
![Vsense_eq1](../images/V_SENSE_equation1.png)

(Detailed information about the voltage divider rule is out of the scope for this readme but can easily be found on the internet).

Substituting with the expression for I<sub>0</sub> (the same curent flows through all the resistors), leads to:

<!-- If the markdown viewer supports math functions you can replace image by this equation
$$
V_{SENSE} = I_{0} \cdot R_{SENSE}
$$
-->
![Vsense_eq2](../images/V_SENSE_equation2.png)

which leads to:

<!-- If the markdown viewer supports math functions you can replace image by this equation
$$
I_{0} = \frac{V_{SENSE}}{R_{SENSE}}
$$
-->
![IO_eq2](../images/I_0_equation2.png)

Knowing that R<sub>SENSE</sub>,  the current can be calculated by measuring the voltage drop across it.

The output voltage is set to 1V (DAC Out), and using the resistor values as listed in the beginning, results in:

<!-- If the markdown viewer supports math functions you can replace image by this equation
$$
V_{SENSE} = V_{0} \left ( \frac{R_{SENSE}} {R_{1} + R_{SENSE} + R_{2}} \right) = 1.0 V * \left ( \frac{10k\Omega} {100k\Omega + 10k\Omega + 100k\Omega} \right) = 47.6mV
$$
-->
![Vsense_eq3](../images/V_SENSE_equation3.png)

<!-- If the markdown viewer supports math functions you can replace image by this equation
$$
I_{0} = \frac{V_{SENSE}}{R_{SENSE}} = \frac{47.6mV}{10k\Omega} = 4.8\mu A
$$
-->
![IO_eq3](../images/I_0_equation3.png)

The Periodic Interrupt Timer (PIT), a part of the Real Time Counter (RTC), is set up to generate an interrupt approximately each second to bring the device out of sleep mode. When this happens, a counter is incremented and checked against a predefined period (10s).  

If the value matches this period, the DAC is enabled to produce an output voltage of 1.8 V and the ADC is enabled. The ADC is commanded to start a differential conversion immediately.  While the AD conversion is in progress, the CPU performs the calculations necessary for converting the previous ADC value into a voltage and a current. The results are printed to the terminal. As soon as this happens, the AD conversion is complete, the DAC and ADC are disabled, and the device goes back into sleep mode.  

The AVR® EA is configured to stay in Power-Down sleep mode whenever a measurement is not in progress, to minimize the power consumption.  

When measuring low-value signals like in this example, the PGA should be enabled to amplify the input signal to get better resolution on the measurement. In this example, the PGA gain is set to 16x and PGA BIAS set to 100% (since the main clock is changed). Since PGA is used, the VIA bit fields of the MUXPOS and MUXNEG registers must be enabled.  

## Summary

The bare metal example leads to the following:

- Increasing clockspeed --> Reduced power consumption

|Main Clock | PGA Disabled (ADC), Average Current (uA)  | PGA Disabled (ADC), Average Current (uA) | Relative Difference | 
|:-------:|:-------:|:------:|:-----:|
|2 MHz    |1.63 |1.42 | -13% |
|3.33 MHz |1.41 |1.33 | -6% |
|20 MHz   |1.14 |1.12 | -2% |

- Average consumption is lower with PGA Enabled

| PGA | Average Current Consumption |
|:----|:---:|
|PGA OFF | 6.06mA |  
|PGA ON | 6.06mA |

While using MCC Melody, an example for Low-Power current sensing, based on the findings from the bare metal code, was created. This examples shows how to use Melody to measure voltage and current using the AVR's ADC, DAC and transfer display the date on a computer using UART.  
