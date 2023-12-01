/*
 * File:   IO.c
 * Author:  Mufaro Mungoni, Fardin Mahid, Uruba Alam
 *
 * Created on October 19, 2023, 2:17 PM
 */
#include "xc.h"
#include "header.h"
#include "uart.h"
#include <stdbool.h>
#include <stdio.h>
#include <p24F16KA101.h>
/*
 TODO
 Slow mode timer implementation 
 FastMode timer implementation
 */
void fastMode();

void slowMode();


int push1, push2, push3;//values for push buttons

//uint16_t INT1EP = 1; // trigger on falling edge
//char *

//uint16_t PB1Pressed = 0; //flag for pushbutton1
uint16_t PB2Pressed = 0; //flag for pushbutton2  
//uint16_t PB3Pressed = 0; //flag for pushbutton3  

uint16_t state = 1;  // initial state is fastmode (1) (slowmode (0))

uint16_t CNflag = 1;

void IOinit(){
    TRISAbits.TRISA4 = 1;// RA4 == input for Push1 (on end of breadboard)
    CNPU1bits.CN0PUE = 1;// set PULL up for 1
    CNEN1bits.CN0IE = 1; //enable cn on CN0
    
    TRISBbits.TRISB4 = 1; // RB4 = inputs for Push2 (middle)
    CNPU1bits.CN1PUE = 1;// set PULL up for 2
    CNEN1bits.CN1IE = 1; //enable cn on CN1
    
    TRISAbits.TRISA2 = 1; // RA2 = inputs for Push3 (closest to MC)
    CNPU2bits.CN30PUE = 1;// set PULL up for 3
    CNEN2bits.CN30IE = 1; //enable cn on CN30
    
    TRISBbits.TRISB8 = 0;//RB8 == output
    AD1PCFG = 0xFFFF; //sets all pins as digital pin not analog
    
    IPC4bits.CNIP = 6; //set priority level
    IFS1bits.CNIF = 0; //clear inturrupt flags
    IEC1bits.CNIE = 1; // enable inturrupt
    INTCON1bits.NSTDIS = 0; // inturrupt nesting enabled   
}
void inputs(){
        //Saving values for pushbuttons
        push1 ;//= PORTAbits.RA2;
        push2 = PORTAbits.RA4;
        push3 ;//= PORTBbits.RB4;
}

void IOcheck(){
        inputs();
        if(PB2Pressed == 0){
            
            //fastMode();
            //run fastmode
        }
        else if(PB2Pressed == 1 ){
            //inputs();
            LATBbits.LATB8 = 1;
            delay_ms(1000);
            LATBbits.LATB8 = 0;
            delay_ms(1000);
        }
        if(state == 0){
            //slowMode();
            //run slow mode
        }

}

//Error State

void Error(){
    
    //do sometihng
}


//CN Inturrupt for pushing buttons
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
    
    //do state stuff here 
    
    //uint16_t prevPB1 = PB1Pressed;
    uint16_t prevPB2 = PB2Pressed;
    //uint16_t prevPB3 = PB3Pressed;
//    
//    if(prevPB1 == 1 && push1 == 0 || prevPB1 == 0 && push1 == 1){
//        PB1Pressed = 1;  
//        //pressedState();
//    }
    if(prevPB2 == 1 && push2 == 0){
        PB2Pressed = 0;    
    }
    else if(prevPB2 == 0 && push2 == 0){
        PB2Pressed = 1;
    }
//    if((prevPB3 == 1 && push3 == 0) || (prevPB3 == 0 && push3 == 1)){
//        PB3Pressed = 1;
//        LATBbits.LATB8 = 1;
//        delay_ms(250);
//        LATBbits.LATB8 = 0;
//        delay_ms(250);
//    }
//            prevPB1 = push1;
            prevPB2 = PB2Pressed;
//            prevPB3 = push3;
            CNflag = 1;
            IFS1bits.CNIF = 0; //clear inturrupt flags
    

}


