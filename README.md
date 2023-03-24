<!-- Please do not change this logo with link -->
[![MCHP](images/microchip.png)](https://www.microchip.com)

# Low-Power Current and Voltage Sensing Using the AVR® EA MCU

<!-- This is where the introduction to the example goes, including mentioning the peripherals used -->
Many systems includes one or more sensors that needs to be checked periodically. These signals are often low-power current or voltage signals. In battery-powered systems it is important to reduce the power consumption of the microcontroller. The signals are read using the Analog-to-Digital (ADC) peripheral, and the microcontroller is in power saving mode between measurements.

These Microchip Studio examples describe use cases for low-power current and voltage measurements on the AVR® EA family of microcontrollers:

- [Low-Power AVR® EA Current Sensing Measurements](https://bitbucket.microchip.com/projects/EBE/repos/avr64ea48-lp-voltage-current-meas-studio/browse/analog-current-sensing)  
Measure a current by using the Analog-to-Digital (ADC) peripheral
- [Low-Power AVR® EA Resistance Temperature Detector (RTD) Measurements](https://bitbucket.microchip.com/projects/EBE/repos/avr64ea48-lp-voltage-current-meas-studio/browse/analog-voltage-sensing)  
Measure a voltage by using the Analog-to-Digital (ADC) peripheral

## Related Documentation

- [AVR64EA48 device page](https://www.microchip.com/wwwproducts/en/AVR64EA48)
- [MPLAB Code Configurator](https://www.microchip.com/en-us/development-tools-tools-and-software/embedded-software-center/mplab-code-configurator)  

## Software Used

<!-- All software used in this example must be listed here. Use unbreakable links!
-->
- [MPLAB® X IDE v6.05](http://www.microchip.com/mplab/mplab-x-ide) or newer
- [Microchip AVR64EA48 Device Support Pack AVR-Ex_DFP v2.2.56](https://packs.download.microchip.com/) or newer
- [MPLAB® XC8 Compiler v2.41](http://www.microchip.com/mplab/compilers) or newer
- MPLAB® Code Configurator (MCC)
  - [MCC Plugin v5.3.0](https://www.microchip.com/mplab/mplab-code-configurator) or newer
  - [MCC Core v5.5.0](https://www.microchip.com/mplab/mplab-code-configurator) or newer  
  
## Hardware Used

- [AVR64EA48 Curiosity Nano](https://www.microchip.com/en-us/development-tool/EV66E56A)

## Summary
<!-- Summarize what the example has shown -->
These examples offer a better understanding for how to periodically measure a voltage or current signal from a sensor while consuming as little power as possible.
