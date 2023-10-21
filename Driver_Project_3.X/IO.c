/*
 * File:   main.c
 * Author:  Mufaro Mungoni, Fardin Mahid, Uruba Alam
 *
 * Created on October 19, 2023, 2:17 PM
 */
#include <stdbool.h>
#include "IOtimers.h"


void IOinit(){
    TRISAbits.TRISA4 = 1;// RA4 == input for Push1 (on end of breadboard)
    CNPU1bits.CN0PUE = 1;// set PULL up for 1
    
    TRISBbits.TRISB4 = 1; // RB4 = inputs for Push2 (middle)
    CNPU1bits.CN1PUE = 1;// set PULL up for 2
    
    TRISAbits.TRISA2 = 1; // RA2 = inputs for Push3 (closest to MC)
    CNPU2bits.CN30PUE = 1;// set PULL up for 3
    
    TRISBbits.TRISB8 = 0;//RB8 == output
    AD1PCFG = 0xFFFF; //sets all pins as digital pin not analog
    
    int push1, push2, push3;//values for push buttons
}

void IOcheck(){
   
    
    //while 1 starts here but its put in main
       //Saving values for pushbuttons
        push1 = PORTAbits.RA2;
        push2 = PORTAbits.RA4;
        push3 = PORTBbits.RB4;
        
        //Pushbutton 1 which is connected to pin RA4 (0.5s)
        if (push1 == 0 && push2 == 1 && push3 == 1 ) {// enable flashing only if push button1 is pushed           
            LATBbits.LATB8 = 1; // toggle output to allow blinking
            delay_ms(500);
   
            LATBbits.LATB8 = 0; //LED off
            delay_ms(500);
        }
        //************************************************
        
        //Pushbutton 2 which is connected to pin RB4
        if(push1 == 1 && push2 == 0 && push3 == 1) {// enable flashing only if pushbutton2 is pushed
            LATBbits.LATB8 = 1; // toggle output
            delay_ms(1000);
            
            LATBbits.LATB8 = 0; //LED off
            delay_ms(1000);
        }
        
        //Pushbutton 3 which is connected to pin RB4
        if(push1 == 1 && push2 == 1 && push3 == 0) {// enable flashing only if pushbutton 3 is pushed
            LATBbits.LATB8 = 1; // toggle output            
            delay_ms(5000);
            
            LATBbits.LATB8 = 0; //LED off
            delay_ms(5000);
        }
        //2 or more Pushbuttons pressed at same time - logic 
        if( push1 == 0 && push2 == 0 && push3 == 1) {
            //enable LED ON (no blinking) if 1 and 2 pushbuttons are pressed                 
            
            LATBbits.LATB8 = 1;
            delay_ms(5000);
            
        }
        
        //LED off if out of bounds
        else {
            LATBbits.LATB8 = 0; 
        }
    

}
