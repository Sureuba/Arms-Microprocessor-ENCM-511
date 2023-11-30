/*
 * File:   IO.c
 * Author:  Mufaro Mungoni, Fardin Mahid, Uruba Alam
 * Created on October 19, 2023, 2:17 PM
 */
#include "xc.h"
#include "header.h"
#include "uart.h"
#include <stdbool.h>
#include <stdio.h>

int push1, push2, push3;//values for push buttons

//uint16_t INT1EP = 1; // trigger on falling edge (want the opposite function pull up becomes pull down)

char * Idle = "IDLE";
char * Pushbutton1 = "PB1 was pressed";
char * PushButton2 = "PB2 was pressed";
char * PushButton3 = "PB3 was pressed";
char * fm = "Fast Mode: ";
char * sm = "Slow Mode: ";

uint16_t CN0Flag = 0;   //flag for pushbutton1
uint16_t CN1Flag = 0;   //flag for pushbutton2  
uint16_t CN30Flag = 0;  //flag for pushbutton3  

uint16_t state = 1;     //initial state is fastmode =(1) slowmode =(0)

uint16_t pbstate = 0; 
//0 = nothing
//1 = pb1 pressed
//2 = pb2 pressed
//3 = pb3 pressed
//4 = any two pbs pressed

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
    
    TRISBbits.TRISB8 = 0;//RB8 == output (LED)
    AD1PCFG = 0xFFFF; //sets all pins as digital pin not analog
    
    //Interrupt setups
    IPC4bits.CNIP = 6; //set priority level
    IFS1bits.CNIF = 0; //clear inturrupt flags
    IEC1bits.CNIE = 1; // enable inturrupt
    INTCON1bits.NSTDIS = 0; // inturrupt nesting enabled   
}

void IOcheck(){
        inputs(); //Saving values for pushbuttons
//        inputs();
        //Idle();
        if(state == 1){ //run fastmode
            fastMode();
             Idle();
        }
        if(state == 0){ //run slow mode
            slowMode();
            Idle();
        }
}

void inputs(){//saves inputs when asked
    push1 = PORTAbits.RA2;
    push2 = PORTAbits.RA4;
    push3 = PORTBbits.RB4;
}

void slowMode(){
    //Pushbutton 1 which is connected to pin RA4 (0.5s)
    inputs(); 
        if (push1 == 0 && push2 == 1 && push3 == 1 ) {// enable flashing only if push button1 is pushed 
            Disp2String(sm);
            Disp2String(Pushbutton1);
            delay_ms(250);
            while(pbstate != 0){
                LATBbits.LATB8 = 1; // toggle output to allow blinking
                delay_ms(5000);

                LATBbits.LATB8 = 0; //LED off
                delay_ms(5000);
            }
            delay_ms(250);
            //return;
        }
        //************************************************
      
        //Pushbutton 2 which is connected to pin RB4
        if(push1 == 1 && push2 == 0 && push3 == 1) {// enable flashing only if pushbutton2 is pushed
            Disp2String(sm);
            Disp2String(PushButton2);
            delay_ms(250);
            while(pbstate != 0){
                LATBbits.LATB8 = 1; // toggle output
                delay_ms(3000);

                LATBbits.LATB8 = 0; //LED off
                delay_ms(3000);
            }
            delay_ms(250);
        }
        //Pushbutton 3 which is connected to pin RB4
        if(push1 == 1 && push2 == 1 && push3 == 0) {// continue
            Disp2String(sm);
            Disp2String("PB3: IDLE ");
        }
        //2 or more Pushbuttons pressed at same time - logic 
        if( push1 == 0 && push2 == 0 && push3 == 1) {
            //enable LED ON (no blinking) if 1 and 2 pushbuttons are pressed                 
            Disp2String(sm);
            Disp2String(" PB1 & PB2 pressed. LED on (No Blinking). ");
            delay_ms(250);
            LATBbits.LATB8 = 1;
            delay_ms(5000);
            delay_ms(250);
            
        }
        
        //LED off if out of bounds
        else {
            LATBbits.LATB8 = 0; 
        }
}

void fastMode(){
    //Pushbuttons pressed at same time 
    inputs(); 
        if((push1 == 0 && push2 == 0 && push3 == 1)||
                (push1 == 0 && push2 == 1 && push3 == 0) ||
                (push1 == 1 && push2 == 0 && push3 == 0)) {
            //enable LED ON (no blinking) if 2 or more pushbuttons are pressed            
            
            LATBbits.LATB8 = 1; // NO toggle output
            Disp2String(fm);
            if(push1 == 0 && push2 == 0 && push3 == 1){Disp2String("PB 1 and PB 2 are pressed");}
            if(push1 == 0 && push2 == 1 && push3 == 0){Disp2String("PB 1 and PB 3 are pressed");}
            if(push1 == 1 && push2 == 0 && push3 == 0){Disp2String("PB 2 and PB 3 are pressed");}
            
            while (pbstate == 4){  
                LATBbits.LATB8 = 1; // toggle output to allow blinking
                delay_ms(1);
            }
            delay_ms(250);
        }
    //Pushbutton 1 which is connected to pin RA4 (0.5s
        if (push1 == 0 && push2 == 1 && push3 == 1 ) {// enable flashing only if push button1 is pushed 
            //change mini state
            delay_ms(250);
            Disp2String(fm);
            Disp2String(Pushbutton1);

            while (pbstate != 0){ //pbstate = 0 
                LATBbits.LATB8 = 1; // toggle output to allow blinking
                delay_ms(250);

                LATBbits.LATB8 = 0; //LED off
                delay_ms(250);
            }
            delay_ms(250);
//            
        }
        //************************************************
        
        //Pushbutton 2 which is connected to pin RB4
        if(push1 == 1 && push2 == 0 && push3 == 1) {// enable flashing only if pushbutton2 is pushed
            Disp2String(fm);
            Disp2String(PushButton2);
            delay_ms(250);
            while(pbstate != 0){
                LATBbits.LATB8 = 1; // toggle output
                delay_ms(500);

                LATBbits.LATB8 = 0; //LED off
                delay_ms(500);
            }
            delay_ms(250);
            // if same button pressed
            //return 
        }
        
        //Pushbutton 3 which is connected to pin RB4
        if(push1 == 1 && push2 == 1 && push3 == 0) {// enable flashing only if pushbutton 3 is pushed
            Disp2String(fm);
            Disp2String(PushButton3);
            delay_ms(250);
            while(pbstate != 0){
                LATBbits.LATB8 = 1; // toggle output            
                delay_ms(1000);

                LATBbits.LATB8 = 0; //LED off
                delay_ms(1000);
            }
            delay_ms(250);
        }
        
        //2 Pushbuttons pressed at same time - logic 
        if( push1 == 0 && push2 == 0 && push3 == 1) {
            //enable LED ON (no blinking) if 1 and 2 pushbuttons are pressed 
            Disp2String(fm);
            Disp2String(" Two PBs pressed together. ");
            delay_ms(250);
            while(pbstate != 0){
                LATBbits.LATB8 = 1;
                delay_ms(1);
                LATBbits.LATB8 = 0;
                delay_ms(1);
            }
            delay_ms(250);
        }        
        //LED off if out of bounds
        else {
            LATBbits.LATB8 = 0; 
        }
        //Idle();
}

//CN Inturrupt for WHEN buttons ARE PUSHED 
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
    //FASTMODE *************************************************************
     delay_ms(1000); //buffer so it can read all of them at the same time 
    if(state == 1){ //in FM
            if(push1 == 0 && push2 == 0 && push3 == 0 ){ //All buttons pushed in fastmode changes to slowmode
                state = 0;
                pbstate = 0;
                Disp2String(" Mode changed to slowMode ");
            }
            //PB1 pushed 
            else if(push1 == 0 && push2 == 1 && push3 == 1){
                if(pbstate == 1 || pbstate == 4){ //pb1 pushed and pb 1 was on before
                    Disp2String(fm);
                    Disp2String(" IDLE ");
                    pbstate = 0; // back to idle
                }
                else{//pb1 pushed and pb 1 was off before
                    pbstate = 1;
                }
            }
            //PB2 pushed 
            else if(push1 == 1 && push2 == 0 && push3 == 1){
                if(pbstate == 2 || pbstate == 4){
                    Disp2String(fm);
                    Disp2String(" IDLE ");
                    pbstate = 0;
                }
                else{
                    pbstate = 2;
                }
            }
            //PB3 pushed 
            else if(push1 == 1 && push2 == 1 && push3 == 0){
                if(pbstate == 3 || pbstate == 4){
                    Disp2String(fm);
                    Disp2String(" IDLE ");
                    pbstate = 0;
                }
                else{
                    pbstate = 3;
                }
            }
            //2 PB's pushed at same time
            else if((push1 == 0 && push2 == 0 && push3 == 1)||
                (push1 == 0 && push2 == 1 && push3 == 0) ||
                (push1 == 1 && push2 == 0 && push3 == 0)) {                 
                if(pbstate != 4){
                      pbstate = 4;
                }
            
            else{ //anything else
//                 Disp2String(fm);
//                 Disp2String(" IDLE, lowkey didn't go into anything in fastmode interrupt");
                 pbstate = 0;
                }
            }
            CN0Flag = 1; 
            CN1Flag = 1; 
            CN30Flag = 1;
            IFS1bits.CNIF = 0; //clear inturrupt flags
    }
    
    //SLOWMODE *************************************************************
    else if(state == 0 ){ 
            if(push1 == 0 && push2 == 0 && push3 == 0 ){
                state = 1;
                pbstate = 0;
                Disp2String(" Mode changed to fastMode ");
            }
            else if(push1 == 0 && push2 == 1 && push3 == 1){
                if(pbstate == 1){
                    Disp2String(sm);
                    Disp2String(" IDLE ");
                    pbstate = 0;
                }
                else{
                pbstate = 1;
                }
            }
            else if(push1 == 1 && push2 == 0 && push3 == 1){
                if(pbstate == 2){
                    Disp2String(sm);
                    Disp2String(" IDLE ");
                    pbstate = 0;
                }
                else{
                pbstate = 2;
                }
            }
            //TODO when pb2 and 3 pushed together
            else{ 
                //Disp2String(fm);
                pbstate = 0;
            } 
            
            CN0Flag = 1; 
            CN1Flag = 1; 
            CN30Flag = 1;
            IFS1bits.CNIF = 0; //clear inturrupt flags
    }
            
   


}

