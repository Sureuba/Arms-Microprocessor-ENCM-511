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
    AD1CON1bits.FORM = 00;            // output format in integer
    AD1CON1bits.SSRC = 111;           //internal counter ends sampling

    AD1CON2bits.VCFG = 0b000;         // Selects AVDD, AVSS (supply voltage to PIC) as Vref ?..
                                      // Configure ADC by setting bits in AD1CON2
    AD1CON3bits.ADRC = 0;             // Use system clock ?..
                                      //Configure the ADC?s sample time by setting bits in AD1CON3 ?..                                  
                                      // Ensure sample time is 1/10th of signal being sampled
    AD1CON3bits.SAMC = 11111;         //Tad = 31 (WRONG TBD later!) (should be 1/10th signal being sampled)

    //ADC input selection::
    AD1CHSbits.CH0NB = 0;             // ?? negative input is Vr- (TDB) ??
    AD1CHSbits.CH0SB = 0101;          // ?? Positive input is AN5 ??
                                      //Question the purpose of MUX B and negative/positive input
    AD1PCFGbits.PCFG5 = 0;            //change from analog to digital
    
    
    /*-------------------------------------------------------*/
// Select and configure ADC input
/* ------------- ADC SAMPLING AND CONVERSION  ------------------*/
AD1CON1bits.ASAM = 0;              // Initiate ADC sampling
AD1CON1bits.SAMP= 1;               //Start Sampling, Conversion starts automatically after SSRC and SAMC settings 
AD1CON1bits.ADON = 1;               //ADC is operating (or here??)
while(AD1CON1bits.DONE==0)
   {}                              //?? ask if anything needs to be put into the while loop. ??
   ADCvalue = ADC1BUF0;            // ADC output is stored in ADC1BUF0 as this point 
   AD1CON1bits.SAMP=0;             //Stop sampling
   AD1CON1bits.ADON=0;             //Turn off ADC, ADC value stored in ADC1BUF0; 
   return (ADCvalue);              //returns 10 bit ADC output stored in ADC1BIF0 to calling function
}

#include "xc.h"

//ADC interrupt
// ?? Do we need this ??
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{
// ?? Ask where interrupt flag goes.  ??
IPC3bits.AD1IP = 7;
//Optionally do stuff here
IFS0bits.AD1IF = 0; // Clear the ADC1 Interrupt Flag
}
