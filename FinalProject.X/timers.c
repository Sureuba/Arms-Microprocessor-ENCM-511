/*
 * File:   timers.c
 * Author: Uruba
 *
 * Created on October 19, 2023, 2:19 PM
 */


#include "xc.h"
#include "header.h"

uint16_t TMR2flag = 0;

void delay_ms(uint32_t time_ms){
   //T2CON config
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0;
    T2CONbits.TCKPS = 2; //prescaler set to 10 which is 1:64
    //T2CONbits.TGATE = 0;
    T2CONbits.TSIDL = 0;
    
    //Timer 2 interrupt config
    IPC1bits.T2IP = 7;//7 is highest priority 1 is lowest
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;//enable timer interrupt
    //fclk = 8Mhz/2 = 4Mhz
    //prescaler = 2
    
    PR2 = (time_ms *4); //using 500kHz
    TMR2 = 0;
    T2CONbits.TON = 1;
    Idle();
    
}
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    //interrupt triggers when TMR2 = PR2
    IFS0bits.T2IF = 0;//Clear timer 2 interrupt flag
    T2CONbits.TON = 0;//stop timer
    TMR2flag = 1; //global variable to indicate timer2 has been triggered
    
    //count 50 beeps, if 50 reached goto error state
    
}