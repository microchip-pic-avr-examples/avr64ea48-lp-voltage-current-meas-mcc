  /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include <util/delay.h>

void PORT_initialize(void);


/*
 * PORT_initialize(void)
 * 
 * Disable all pin interrupts & digital input buffer to save power
 * 
 */
void PORT_initialize(void)
{
	PORTA.PINCONFIG = PORT_ISC_INPUT_DISABLE_gc;
    PORTA.PINCTRLUPD = 0xff;
    PORTB.PINCTRLUPD = 0xff;
    PORTC.PINCTRLUPD = 0xff;
    PORTD.PINCTRLUPD = 0xff;
    PORTE.PINCTRLUPD = 0xff;
    PORTF.PINCTRLUPD = 0xff;
}

/*
 *	main(void)
 * 
 * This code periodically measures the resistance of an RTD (Resistance 
 * Temperature Detector) and uses it to compute the temperature.
 * 
*/

int main(void)
{
    SYSTEM_Initialize();
    PORT_initialize();
    
	volatile int32_t ADCtmp = 0;	// Use for saving raw accumulated ADC result
    volatile float xAverage = 0;
    volatile float rOhm = 0;		// Resistance in Ohms
    volatile float tempDegC = 0;	// Temperature in Degrees Celsius
    volatile float PGA = 16; 
    volatile float ADCsamples = 16;
    volatile float Rfixed = 1800;
    volatile float ADCres = 2048;
	
    while(1)
    {
        sleep_cpu();				// Sleep CPU until interrupt wakes it up
        
        LED0_SetDigitalOutput();    
        LED0_SetLow();              // Turn on LED0

        DAC0_Enable();
        ADC0_Enable();
        //Command the ADC to start a differential conversion immediately
        ADCtmp = ADC0_GetDiffConversion(1, ADC_MUXPOS_AIN0_gc, ADC_MUXNEG_AIN1_gc);

        DAC0_Disable();
        ADC0_Disable();
        
        // If there is a previous ADC result (x > 0),
        // do math for converting previous ADC result into resistance and temperature
        if (ADCtmp > 0)
        {
            xAverage = ((float) ADCtmp) / ADCsamples; // Determine average ADC result from accumulated result
            rOhm = (xAverage * Rfixed) / ((PGA * ADCres) - xAverage); // Compute resistor value
            tempDegC = (rOhm - 100.0) / 0.385; // Simple temperature calculation only valid for 0 to 100 C
        }
        
        LED0_SetDigitalInput();
        LED0_DisableDigitalInputBuffer();       // Turn off LED0, save power
    }
}