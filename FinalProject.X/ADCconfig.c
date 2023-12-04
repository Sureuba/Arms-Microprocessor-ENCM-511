/*
 * File:   ADCconfig.c
 * Author: Fardin Mahid
 *
 * Created on November 16, 2023, 1:49 PM
 */
#include "xc.h"
#include "header.h"

uint16_t do_ADC(void) {
   
    uint16_t ADCvalue ;              // 16 bit register used to hold ADC converted digital output ADC1BUF0                                 
  
     /* ------------- ADC INITIALIZATION  ------------------*/ 

    // Configure ADC by setting bits in AD1CON1 register ?..
    AD1CON1bits.ADON = 1; // turn on ADC module
    AD1CON1bits.FORM = 0; // Output in integer format
    AD1CON1bits.SSRC = 7; // auto conversion (internal counter end sampling and starts conversion)
    AD1CON1bits.ASAM = 0; // Sampling begins when SAMP bit set
    
    // Configure ADC by setting bits in AD1CON2 as shown in slides 15-16
    AD1CON2bits.VCFG = 0b000; // Selects AVDD, AVSS (supply voltage to PIC) as Vref
    AD1CON2bits.CSCNA = 0; // Don't scan inputs
    
   // AD1CON2bits.SMPI = 0; // Interrupt at completion of conversion for each sample/convert sequence
    AD1CON2bits.BUFM = 0; // Buffer configures as one 16 word buffer
    AD1CON2bits.ALTS = 0; // Always uses MUX A input multiplexer settings
    
    //Configure ADC by setting bits in AD1CON3
    AD1CON3bits.ADRC = 0; // Use system clock
    AD1CON3bits.SAMC = 10; // Sample time = 1/10 * signal being sampled
   // AD1CON3bits.ADCS = 31 // Only needed for ADRC = 1

    //USE MUX A
    AD1CHSbits.CH0NA = 0; // channel 0 negative input is VR-
    AD1CHSbits.CH0SA = 5; // channel 0 positive input is AN5
    
    /*-------------------------------------------------------*/
    // Select and configure ADC input
    /* ------------- ADC SAMPLING AND CONVERSION  ------------------*/
    AD1PCFGbits.PCFG5 = 0; // configure pin in analog mode, I/O port read disabled
    AD1CON1bits.SAMP=1; //Start Sampling, Conversion starts automatically after SSRC and SAMC settings
    
    //********************************************************
    while(AD1CON1bits.DONE==0) // While not done sampling
    {}
    
    ADCvalue= ADC1BUF0; // ADC output is stored in ADC1BUF0 as this point
    AD1CON1bits.SAMP=0; //Stop sampling
    AD1CON1bits.ADON=0; //Turn off ADC, ADC value stored in ADC1BUF0;
    
    return (ADCvalue); //returns 10 bit ADC output stored in ADC1BIF0 to calling function
}



//ADC interrupt
// ?? Do we need this ??
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{
// ?? Ask where interrupt flag goes.  ??
IPC3bits.AD1IP = 7;
//Optionally do stuff here
IFS0bits.AD1IF = 0; // Clear the ADC1 Interrupt Flag
}