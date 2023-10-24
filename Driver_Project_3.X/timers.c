/*
 * File:   timers.c
 * Author: Uruba
 *
 * Created on October 19, 2023, 2:19 PM
 */


#include "xc.h"
#include "IOtimers.h"

uint16_t TMR2flag = 0;

void delay_ms(int time_ms){
   //T2CON config
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0;
    T2CONbits.TCKPS = 3; //prescaler set to 11 which is 1:256
    //T2CONbits.TGATE = 0;
    T2CONbits.TSIDL = 0;
    
    //Timer 2 interrupt config
    IPC1bits.T2IP = 2;//7 is highest priority 1 is lowest
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;//enable timer interrupt
    //fclk = 8Mhz/2 = 4Mhz
    //prescaler = 4*10^6
    
    PR2 = (time_ms *8000000)/(2*1000*256); //pr2 = time_ms * 8Mhz/2 /1000 (for delay in ms)
    //PR2 = (time_ms *32000)/2000; //pr2 = time_ms * 8Mhz/2 /1000 (for delay in ms)
    TMR2 = 0;
    T2CONbits.TON = 1;
    Idle();
    
}
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    
    IFS0bits.T2IF = 0;//Clear timer 2 interrupt flag
    T2CONbits.TON = 0;//stop timer
    TMR2flag = 1; //global variable to indicate timer2 has been triggered
    
}
