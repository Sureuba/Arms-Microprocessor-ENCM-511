/*
 * File:   main.c
 * Author: Uruba, Mufaro, Fardin
 *
 * Created on October 19, 2023, 3:13 PM
 */
// PIC24F16KA101 Configuration Bit Settings

// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

#include "xc.h"
#include "header.h"
#include "uart.h"

int main(void) {
    IOinit();       //Initialize IO's
    InitUART2();    //set up UART
    newClk(500);    //Fclk is 500hz
    Disp2String("Hello, Our Program is starting: ........");
    while(1){
        IOcheck();
    }
    
  return 0;
}