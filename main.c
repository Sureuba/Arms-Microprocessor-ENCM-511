/*
 * File:   newmainXC16.c
 * Author: Fardin Mahid
 *
 * Created on October 3, 2023, 9:12 AM
 */

#include <p24F16KA101.h>
#include "xc.h"



int main(void) {
    
    uint32_t count = 0; 
    TRISAbits.TRISA4 = 1;// RA4 == input for Push1 (on end of breadboard)
    CNPU1bits.CN0PUE = 1;// set PULL down for 1
    TRISBbits.TRISB4 = 1; // RB4 = inputs for Push2 (middle)
    CNPU1bits.CN1PUE = 1;// set PULL down for 2
    TRISAbits.TRISA2 = 1; // RA2 = inputs for Push3 (closest to MC)
    CNPU2bits.CN30PUE = 1;// set PULL down for 3
    
    TRISBbits.TRISB8 = 0;//RB8 == output
    AD1PCFG = 0xFFFF; //sets all pins as digital pin not analog
    
    while (1) {
        //Pushbutton 1 which is connected to pin RA4
        if (PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 1 ) {// enable flashing only if push button is pushed
            LATBbits.LATB8 ^= 1; // toggle output
            
            while (count < 150000/2){
                count++;
            }//adds a delay
            count = 0;
    
            LATBbits.LATB8 = 0; //off
            while (count < 150000/2){
                count++;
            }//adds a delay
            count = 0;
        }
        //************************************************
        //count = 0;
        //Pushbutton 2 which is connected to pin RB4
        if(PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0 && PORTAbits.RA2 == 1) {// enable flashing only if push button2 is pushed
            LATBbits.LATB8 ^= 1; // toggle output
            
            while (count < 150000){
                count++;
            }//adds a delay
            count = 0;
    
            LATBbits.LATB8 = 0; //off
            while (count < 150000){
                count++;
            }
            count = 0;
        }
        //Pushbutton 3 which is connected to pin RB4
        if(PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 0) {// enable flashing only if push button 3 is pushed
            LATBbits.LATB8 ^= 1; // toggle output
            
            while (count < 150000*2){
                count++;
            }//adds a delay
            count = 0;
    
            LATBbits.LATB8 = 0; //off
            while (count < 150000*2){
                count++;
            }
            count = 0;
        }
        //Pushbuttons pressed at same time 
        if((PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0 && PORTAbits.RA2 == 1)||
                (PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 0) ||
                (PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0 && PORTAbits.RA2 == 0) ||
                (PORTAbits.RA4 == 0&& PORTBbits.RB4 == 0 && PORTAbits.RA2 == 0) ) 
        {// enable flashing only if push button 3 is pushed
            LATBbits.LATB8 = 1; // toggle output
            while (count < 150000){
                count++;
            }
            count = 0;
        }
        
        else {
            LATBbits.LATB8 = 0;
        }
    }

    return 0;

}

