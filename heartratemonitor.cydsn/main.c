/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include "math.h"



int a1 = 0b1110;
int a2 = 0b1101;
int a3 = 0b1011;
int a4 = 0b0111;
int b1, b2, b3 ,b4;
int int2bin(int i);
uint8 group_no = 71;
char tmpStr[30];
int Program_mode;



//=====================================Display group number================================================
void display(int a, int n){
    

    for(int x=0;x<375;x++){
        Control_Reg_2_Write(a1);
        b1 = int2bin((a%10000 - a%1000)/1000);
        Control_Reg_1_Write(b1);
        CyDelay(1);
        Control_Reg_2_Write(a2);
        b2 = int2bin((a%1000 - a%100)/100);
        Control_Reg_1_Write(b2);
        CyDelay(1);
        Control_Reg_2_Write(a3);
        b3 = int2bin((a%100 - a%10)/10);
        if(n == 1) b3 += 0b10000000;
        Control_Reg_1_Write(b3);
        CyDelay(1);
        Control_Reg_2_Write(a4);
        b4 = int2bin(a%10);
        Control_Reg_1_Write(b4);
        CyDelay(1); 
    }
    Control_Reg_2_Write(0b1111);
    
}

//====================================Display rEAd==========================================
void displayrEAd(){
    

    for(int x=0;x<500;x++){
        Control_Reg_2_Write(a1);
        Control_Reg_1_Write(0b10101111);
        CyDelay(1);
        Control_Reg_2_Write(a2);
        Control_Reg_1_Write(0b10000110);
        CyDelay(1);
        Control_Reg_2_Write(a3);
        Control_Reg_1_Write(0b10001000);
        CyDelay(1);
        Control_Reg_2_Write(a4);
        Control_Reg_1_Write(0b10100001);
        CyDelay(1); 
    }
    Control_Reg_2_Write(0b1111);
    
}

//====================================Display CALC==========================================
void displayCALC(){
    

    for(int x=0;x<500;x++){
        Control_Reg_2_Write(a1);
        Control_Reg_1_Write(0b11000110);
        CyDelay(1);
        Control_Reg_2_Write(a2);
        Control_Reg_1_Write(0b10001000);
        CyDelay(1);
        Control_Reg_2_Write(a3);
        Control_Reg_1_Write(0b11000111);
        CyDelay(1);
        Control_Reg_2_Write(a4);
        Control_Reg_1_Write(0b11000110);
        CyDelay(1); 
    }
    Control_Reg_2_Write(0b1111);
    
}

//====================================Start up display==========================================
void startup(){
    Control_Reg_2_Write(a1);
    Control_Reg_1_Write(0b00000000);
    CyDelay(1000);
    Control_Reg_2_Write(a2);
    Control_Reg_1_Write(0b00000000);
    CyDelay(1000);
    Control_Reg_2_Write(a3);
    Control_Reg_1_Write(0b00000000);
    CyDelay(1000);
    Control_Reg_2_Write(a4);
    Control_Reg_1_Write(0b00000000);
    CyDelay(1000);
    
    //Display group number
    display(group_no,0);
    
    
}


//====================================Sleep mode==========================================
void sleepmode(){
    // sleep mode
    Control_Reg_2_Write(a4);
    Control_Reg_1_Write(0b11111111);
    CyDelay(500);
    Control_Reg_1_Write(0b01111111);
    CyDelay(500);
        
        
}

//====================================Beeeeeeeepppppppppp==========================================
void beep() {
    beeperpin_Write(0);
    CyDelay(200);
    beeperpin_Write(1);
    return;
}

//===================================Measure heart rate===============================================
float Timer;
float Time;
float rec_hr = 0;
float hr;
int i=0;


CY_ISR(measurehandlerisr)                  //This is the ISR
{
    
    
    Timer = Timer_1_ReadCounter();
    Time = (65536 - Timer) /1000;
    if (Time > 0.5 & Time < 1){
        rec_hr += 60 / Time;
        i += 1;
        
    }
    
    if (i==3){
        hr = rec_hr/3;
        
        sprintf(tmpStr, " %f %f %f\n" ,Timer, Time, hr); 	 //Print string to console	            		//Create text string
        UART_1_PutString(tmpStr);
        hr = hr*10;
        display((int)hr, 1);
        i = 0;
        rec_hr = 0;
    }
    counter_reset_Write(1);
    CyDelay(1);
    counter_reset_Write(0);
    
    
    
}
CY_ISR(startupisr)                  //This is the ISR to display information
{
    beep();
    startup();
}
CY_ISR(inc_group_no)
{
    beep();
    group_no++;
}
CY_ISR(dec_group_no)
{
    beep();
    group_no--;
}

CY_ISR(save_group_no){
    EEPROM_1_WriteByte(group_no, 0);
    Program_mode = 0;
    isr_1_ClearPending();
    isr_1_StartEx(measurehandlerisr);
    isr_3_ClearPending();
    isr_3_StartEx(startupisr);
}

int main(void) {

    CyGlobalIntEnable; /* Enable global interrupts. */
     /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    isr_1_ClearPending();
    isr_1_StartEx(measurehandlerisr);
    isr_3_ClearPending();
    isr_3_StartEx(startupisr);
    
    Clock_1_Start();
    Clock_2_Start();
    Clock_3_Start();
    Timer_1_Start();
    Comp_1_Start(); 
    PGA_1_Start();
    Opamp_1_Start();
    UART_1_Start();			 //Start the UART
    EEPROM_1_Start();
    VDAC8_1_Start();
    
    

    
    group_no = EEPROM_1_ReadByte(0);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    startup();
    
    for(;;)
    {
        
        sleepmode();
        
        for (int i=0; i<5; i++){
            CyDelay(200);
            if (i==4 && (Pin_24_Read()==0)&& (Pin_25_Read()==0)){
                Program_mode = 1;
            }
        }
        while(Program_mode){
            isr_4_ClearPending();
            isr_4_StartEx(inc_group_no);
            isr_1_Disable();
            isr_3_Disable();
            isr_5_ClearPending();
            isr_5_StartEx(dec_group_no);
            display(group_no,0);
            CyDelay(500);
            isr_2_ClearPending();
            isr_2_StartEx(save_group_no);
        }
    }
}

int int2bin(int i){
    int b;
    switch(i){
        case 0: {
            b= 0b11000000;
            break;
        }
        case 1: {
            b= 0b11111001;
            break;
        }
        case 2: {
            b= 0b10100100;
            break;
        }
        case 3: {
            b= 0b10110000;
            break;
        }
        case 4: {
            b = 0b10011001;
            break;
        }
        case 5: {
            b= 0b10010010;
            break;
        }
        case 6: {
            b= 0b10000010;
            break;
        }
        case 7: {
            b = 0b11111000;
            break;
        }
        case 8: {
            b = 0b10000000;
            break;
        }
        case 9: {
            b = 0b10010000;
            break;
        }
    }
    return b;
}

/* [] END OF FILE */
