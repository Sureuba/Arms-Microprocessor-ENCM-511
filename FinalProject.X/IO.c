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
#include <ctype.h>
#include <string.h>



int push1, push2, push3;//values for push buttons
char* password;
//uint16_t INT1EP = 1; // trigger on falling edge (want the opposite function pull up becomes pull down)

char * Pushbutton1 = "PB1 was pressed";
char * PushButton2 = "PB2 was pressed";
char * PushButton3 = "PB3 was pressed";

uint16_t CN0Flag = 0;   //flag for pushbutton1
uint16_t CN1Flag = 0;   //flag for pushbutton2  
uint16_t CN30Flag = 0;  //flag for pushbutton3  

uint16_t state = 0;     //initial state is Locked =(0) Unlocked =(1)
bool checkPassScript(char * newPass){
        for(int i = 0; i < 3;i++){
            if(!isalpha(newPass[i])){
               Disp2String("\r\n ERROR: Invalid input (Must be English Characters)r\n");
               return false;
            } 
        }
        return true;
}

void IOinit(){
    password = "dog7";
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

void inputs(){//saves inputs when asked
    push1 = PORTAbits.RA2;
    push2 = PORTAbits.RA4;
    push3 = PORTBbits.RB4;
}

void LEDBlink(int delay){
    
    LATBbits.LATB8 = 1; // LED ON            
    delay_ms(delay);

    LATBbits.LATB8 = 0; //LED off
    delay_ms(delay);
}

void errorState(){
    //turns buttons off
    //displays string saying 'error'
    Disp2String("\r\nError: Please press PB2 to restart program\r\n");
    LATBbits.LATB8 = 0;
    Idle();
    delay_ms(500);
    inputs();
    if (CN1Flag == 1) {// enable flashing only if push button1 is pushed 
        //back to initState())
        CN1Flag = 0;
        initState();
//            
    }
    else {
        CN0Flag = 0;
        CN1Flag = 0;
        CN30Flag = 0;
        errorState();
    }
    //if pb2 pressed, go back to main state
}

void initState(){
    state = 0;
    inputs();
    Disp2String("\r\nWelcome to Unlock Program!\r\n");
    Disp2String("Please enter the first character of the password: \r\n");
    LATBbits.LATB8 = 1; //LED = ON
    char first = RecvUartChar();
    
    if(password[0] == first){
        inputs(); //Saving values for pushbuttons
        state1();
    }
    else{
        inputs(); //Saving values for pushbuttons
        errorState();
    }
}

void state1(){
    inputs();
    Disp2String("\r\nCorrect 1st Character. Enter 2nd Character\r\n");
    int counter = 0;
    char second = '\0';
    while (password[1] != second && counter != 50) {

        second = NonBlockingRecvUartChar();
        if(second == password[1]){
            state2();
        }
        LATBbits.LATB8 = 1; // toggle output            
        delay_ms(500);

        LATBbits.LATB8 = 0; //LED off
        delay_ms(500);

        counter++;
    }
    errorState();
    //displays second character
    //if wrong or timeout (inturrupt)goto errorstate 
    //timeout after 50 blinks at 0,5s intervals;LED flashes
    //if char correct, goto second state
}

void state2(){
    inputs();
    Disp2String("\r\nCorrect 2nd Character. Enter 3rd Character\r\n");
    int counter = 0;
    char third = '\0';
    while (password[2] != third && counter != 50) {

        third = NonBlockingRecvUartChar();
        if(third == password[2]){
            state3();
        }
        LATBbits.LATB8 = 1; // toggle output            
        delay_ms(300);

        LATBbits.LATB8 = 0; //LED off
        delay_ms(300);

        counter++;
    }
    errorState();
    //displays second character
    //if wrong or timeout (inturrupt)goto errorstate
    //timeout after 50 0.3 second intervals
    //if char correct, goto third state
}

void state3(){  //POTENTIOMETER
    inputs();
    Disp2String("\r\nCorrect 3nd Character. Use potentiometer to input 4th character\r\n");
    int counter = 0;
    uint16_t num = 0;
   
    while (num != (password[3]- '0') && counter != 50) { 
        num = do_ADC(); 
        num = ((num * 9) / 1023); //scaler
        Disp2Dec(num);
        LATBbits.LATB8 = 1; // toggle output            
        delay_ms(200);

        LATBbits.LATB8 = 0; //LED off
        delay_ms(200);
        if(num == (password[3]- '0')){ 
            Disp2String("\r\n Correct value found. Press enter to continue\r\n");
            RecvUartChar();
            Unlock();
        }

        counter++;
    }
    errorState();
    //displays third character
    //if wrong or timeout (inturrupt)goto errorstate
    //timeout after 50 0.1 second intervals
    //if char correct, goto unlocked state
}

void Unlock(){
    state = 1; //CHANGE STATE to UNLOCKED
    Disp2String("\r\n Unlocked: what would you like to do?\r\n");
    Disp2String("\r\n PB1: Lock System\r\n");
    Disp2String("\r\n PB2: Set New Password\r\n");
    Disp2String("\r\n PB3: Enter Special State\r\n");
    Idle(); //changes flags
    inputs();
    delay_ms(500);
    Disp2String("\r\nFLag Values\r\n");
    Disp2Dec(CN0Flag);
    Disp2Dec(CN1Flag);
    Disp2Dec(CN30Flag);
    if(CN0Flag == 1){
        CN0Flag = 0;
        initState();
    }
    else if(CN1Flag == 1){
        CN1Flag = 0;
        newPassState();
    }
    else if(CN30Flag == 1){
        CN30Flag = 0;
        specialState();
    }
    else{
        errorState();
    }
     
}

void newPassState(){
    
    while(1){
        char * newPass[4];
        Disp2String("\r\n Please enter first character of new password : \r\n");
        newPass[0] = RecvUartChar();
        Disp2String("\r\n Please enter second character of new password : \r\n");
        newPass[1] = RecvUartChar();
        Disp2String("\r\n Please enter third character of new password : \r\n");
        newPass[2] = RecvUartChar();
        Disp2String("\r\n Please enter a number using ADC : \r\n");
        char choice = '\0';
        while(choice != 'y'){
            uint16_t num = do_ADC();
            num = ((num * 9) / 1023); //scaler
            Disp2Dec(num);
            Disp2String("\r\n Is this the number you want? y/n \r\n");
            choice = RecvUartChar();
            if(choice == 'y'){
                newPass[3] = num + '0';
                break;
            }
            else if(choice == 'n'){
                continue;
            }
            else{
                Disp2String("\r\n Please enter a valid input \r\n");
                continue;
            }
                
        }
        
        //TODO
        Disp2String("\r\n newPass: \r\n");
        Disp2String(newPass);
        Disp2String("\r\n Old Password: \r\n");
        Disp2String(password);
        strcpy(password,newPass);
        Disp2String("\r\n new Password: \r\n");
        Disp2String(password);
        break;

    //enter 3 char and a number
    //timeout after 5 seconds
    }
    
}

void specialState(){
    //do someting 
}

//CN Inturrupt for WHEN buttons ARE PUSHED 
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
        if(push1 == 0 && push2 == 1 && push3 == 1){
            CN0Flag ^= 1;
        }
        //PB2 pushed 
        else if(push1 == 1 && push2 == 0 && push3 == 1){
            CN1Flag ^= 1;
        }
        //PB3 pushed 
        else if(push1 == 1 && push2 == 1 && push3 == 0){
            CN30Flag ^= 1;
        }
    IFS1bits.CNIF = 0; //clear inturrupt flags
}
    
