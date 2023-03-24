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

// add this to use some fancy calculating tricks like pow
#include <math.h>

// Defined constants
#define NBR_ADC_SAMPLES 16      // Number of samples for ADC burst mode
#define ADC_GAIN 16             // Gain for PGA operation
#define ADC_REF_V 3.300         // ADC reference voltage in V
#define WAKEUP_TIME 10          // Wakeup and sample ADC each 10 seconds
#define R_SENSE 10000           // Sense resistor value in Ohm

// functions defined in main.c 
void PORT_initialize(void);
void ADC0_measure(void);
void USART1_send_string(const char *strptr);
void reverse_string(char* str, uint8_t len);
uint8_t int_to_string(uint8_t x, char str[], uint8_t d);
void float_to_string(float n, char* res, uint8_t decimals);
void pit_int_function(void);

//Global variables
int32_t adc_sample = 0;
float sample_adc_mean = 0;
float dac_val = 0;
float dac_ref = 0;
uint16_t dac_data = 0;
float measured_voltage = 0;
float measured_current = 0;
volatile uint8_t timeout = 0;


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
 *  reverse_string
 * 
 *  Reverse a string str of length len
 *  Used to make string look correct when sent to terminal using USART 
 *  since string buffer is executed from left to right
 * 
 */
void reverse_string(char* str, uint8_t len)
{
    int i = 0, j = len - 1, temp;

    while (i < j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}


/*
 *  int_to_string
 * 
 *  Convert integer x to string[]
 *  d = number of digits in output
 *  If d > no. of digits in x, 0s are added to the start of the string
 * 
 */
uint8_t int_to_string(uint8_t x, char str[], uint8_t d)
{
    uint8_t i = 0;
    
    while (x)
    {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
    
    //if d is bigger than chars in x, add '0's at the beginning
    while (i < d)
    {
        str[i++] = '0';
    }

    // to print number correctly, the order must be reversed
    reverse_string(str, i);
    str[i] = '\0';                      // last character is NULL
    return i;
}


/*
 *  float_to_string(float n, char* res, uint8_t decimals)
 *
 *  Convert integer x to string str[]
 *  d =  number of digits in the output
 *  if d > number of digits in x, 0s are added at the beginning
 * 
 */
void float_to_string(float n, char* res, uint8_t decimals)
{
    // Extract integer part
    uint8_t ipart = (uint8_t) n;

    // Extract floating part
    float fpart = n - (float) ipart;

    // convert integer part to string
    uint8_t i = int_to_string(ipart, res, 0);

    if (ipart == 0)
    {
        res[i++] = '0'; // if integer part zero then add zero to string
    }

    // Check for display option after point
    if (decimals != 0)
    {
        res[i] = '.'; // add decimal symbol ('dot')

        // Get the value of fraction part up to given no. of points after dot.
        // The third parameter is needed to handle cases like 233.007
        fpart = fpart * pow(10, decimals);
        int_to_string((uint8_t) fpart, res + i + 1, decimals);
    }
}


/*
 *  USART1_send_string
 * 
 *  Send a string using USART1 Tx
 *  C automatically adds a NULL at the end of a string, so when 
 *  the NULL value is reached the while(*strptr) loop ends
 * 
 */
void USART1_send_string(const char *strptr)
{
    // as long as pointer value is not NULL
    while(*strptr)
    {
        // send byte and increment string pointer
        USART1_Write(*strptr);
        strptr++;
        // wait for Tx buffer empty
        while(!(USART1_IsTxDone()))
            ;
        // write '1' to clear Transmit complete flag
        USART1.STATUS |= USART_TXCIF_bm;
    }
}



/*
 *  ADC0_measure
 * 
 *  Perform ADC0 measurement and send result to terminal
 * 
 */
void ADC0_measure(void)
{
    float adc_samples = 0;
    float nbr_adc_samples = NBR_ADC_SAMPLES;
    float adc_gain = ADC_GAIN;
    float adc_ref_v = ADC_REF_V;
    float r_sense = R_SENSE;
    float adc_samples_mean;

    // Enables DAC and ADC as it is turned off to save power
    DAC0_Enable();
    ADC0_Enable();

    while (ADC0.STATUS > 0) // wait for ADC ready
        ;
    // Starting the differential  ADC measurement using MCC generated function
    ADC0_GetDiffConversion(1, ADC_MUXPOS_AIN1_gc, ADC_MUXNEG_AIN0_gc);
    // Returns the result of the ADC0_GetDiffConversion
    adc_samples = ADC0_GetConversionResult();

    //removing PGA gain
    adc_samples = adc_samples / adc_gain;
    // Get mean value
    adc_samples_mean = adc_samples / nbr_adc_samples;
    // Calculate voltage
    measured_voltage = ((adc_samples_mean * adc_ref_v) / 2048);
    // calculate current in uA
    measured_current = (measured_voltage / r_sense) * 1000000;

    char res[20];   // string buffer
    
    //Convert integer x to string str[]
    float_to_string(measured_voltage, res, 4);
    // Sends the result over uart
    USART1_send_string("Measured Voltage: ");
    USART1_send_string(res);
    USART1_send_string("V\n");

    //Fixing rounding problems
    measured_current = measured_current + 0.5;
    //Convert integer x to string str[]
    float_to_string(measured_current, res, 4);

    // Sends the result over uart
    USART1_send_string("Measured Current: ");
    USART1_send_string(res);
    USART1_send_string("uA\n");

    // Disables DAC and ADC to save power
    ADC0_Disable();
    DAC0_Disable();
}


/*
 *  Interrupt function for the periodic interrupt
 *  Each time the interrupt is triggered timeout is decremented by one
 *  Actual interrupt handler is located in rtc.c
 */

void pit_int_function(void)
{
   timeout--;                       // increment timeout counter
}



/*
 *     Main application 
*/

int main(void)
{
    // initialize all peripherals
    SYSTEM_Initialize();        
    // disable interrupt and digital buffer on all GPIO pins to save power
    PORT_initialize();
    
    USART1_send_string("Starting...\n");    // Send start message to terminal
    timeout = WAKEUP_TIME;                  // Set ADC measure frequency (seconds)
    
    // set PIT interrupts to execute this function
    RTC_SetPITIsrCallback(pit_int_function);
    
    // loop forever here
    while (1)
    {
        sleep_cpu();                    // go to sleep

        if (timeout < 1)                
        {
            cli();                      // disable global interrupts
            
            LED0_SetDigitalOutput();
            LED0_SetLow();              // turn on LED0 (active low)
            
            ADC0_measure();             // do ADC0 sampling
            timeout = WAKEUP_TIME;      // Reset timeout counter
            
            LED0_SetDigitalInput();
            LED0_DisableDigitalInputBuffer();   // turn off LED0, save power

            sei();                      // enable global interrupts
        }
    }   
}