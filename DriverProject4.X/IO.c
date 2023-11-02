/*
 * File:   main.c
 * Author:  Mufaro Mungoni, Fardin Mahid, Uruba Alam
 *
 * Created on October 19, 2023, 2:17 PM
 */
#include "xc.h"
#include "header.h"
#include <stdbool.h>
#include <stdio.h>
/*
 TODO
 Slow mode timer implementation 
 FastMode timer implementation
 */
int push1, push2, push3;//values for push buttons
uint16_t INT1E = 1;  // inturrupt enable
//uint16_t INT1EP = 1; // trigger on falling edge
char * Pushbutton1 = "PB1 was pressed";

char * Idle = "IDLE";

char * PushButton2 = "PB2 was pressed";

char * PushButton3 = "PB3 was pressed";

char * fastMode = "Fast Mode: ";

char * slowMode = "Slow Mode: ";


//char * 


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
}

void IOcheck(){
        int state = 1;
       //Saving values for pushbuttons
        push1 = PORTAbits.RA2;
        push2 = PORTAbits.RA4;
        push3 = PORTBbits.RB4;
        
        
        
        //check mode before everything else to switch each time if 3 buttons are pressed 
        if(push1 == 0 && push2 == 0 && push3 == 0 ){
            
            if(state == 1){
                state = 0;
            }
            else if(state == 0){
                state = 1;
            }
           //switch mode
           //
        }
        if(state == 1){
            fastMode();
            //run fastmode
        }
        if(state == 0){
            slowMode();
            //run slow mode
        }

}

void slowMode(){
    //Pushbutton 1 which is connected to pin RA4 (0.5s)
        if (push1 == 0 && push2 == 1 && push3 == 1 ) {// enable flashing only if push button1 is pushed 
            
           /*if pushed
            *   case 1: fast mode - default
            *   case 2: slow mode
            *       
            */
            LATBbits.LATB8 = 1; // toggle output to allow blinking
            delay_ms(5000);
   
            LATBbits.LATB8 = 0; //LED off
            delay_ms(5000);
        }
        //************************************************
        
        //Pushbutton 2 which is connected to pin RB4
        if(push1 == 1 && push2 == 0 && push3 == 1) {// enable flashing only if pushbutton2 is pushed
            LATBbits.LATB8 = 1; // toggle output
            delay_ms(3000);
            
            LATBbits.LATB8 = 0; //LED off
            delay_ms(3000);
        }
        
        //Pushbutton 3 which is connected to pin RB4
        if(push1 == 1 && push2 == 1 && push3 == 0) {// continue
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

void fastMode(){
    //Pushbutton 1 which is connected to pin RA4 (0.5s)
        if (push1 == 0 && push2 == 1 && push3 == 1 ) {// enable flashing only if push button1 is pushed 
            Disp2String(fastMode);
            Disp2String(Pushbutton1);
           /*if pushed
            *   case 1: fast mode - default
            *   case 2: slow mode
            *       
            */
            LATBbits.LATB8 = 1; // toggle output to allow blinking
            delay_ms(250);
   
            LATBbits.LATB8 = 0; //LED off
            delay_ms(250);
            
        }
        //************************************************
        
        //Pushbutton 2 which is connected to pin RB4
        if(push1 == 1 && push2 == 0 && push3 == 1) {// enable flashing only if pushbutton2 is pushed
            Disp2String(fastMode);
            Disp2String(PushButton2);
            LATBbits.LATB8 = 1; // toggle output
            delay_ms(500);
            
            LATBbits.LATB8 = 0; //LED off
            delay_ms(500);
            
            // if same button pressed
            //return 
        }
        
        //Pushbutton 3 which is connected to pin RB4
        if(push1 == 1 && push2 == 1 && push3 == 0) {// enable flashing only if pushbutton 3 is pushed
            Disp2String(fastMode);
            Disp2String(PushButton3);
            LATBbits.LATB8 = 1; // toggle output            
            delay_ms(1000);
            
            LATBbits.LATB8 = 0; //LED off
            delay_ms(1000);
        }
        //2 Pushbuttons pressed at same time - logic 
        if( push1 == 0 && push2 == 0 && push3 == 1) {
            //enable LED ON (no blinking) if 1 and 2 pushbuttons are pressed                 
            
            LATBbits.LATB8 = 1;
            delay_ms(1);
            
        }        
        //LED off if out of bounds
        else {
            LATBbits.LATB8 = 0; 
        }
}
