/*
 * File:   main.c
 * Author: Bahtiyar Bayram
 *
 * Created on December 2, 2018, 4:59 PM
 */

// PIC18F45K22 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block)
#pragma config PLLCFG = OFF     // 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock is always enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = INTMCLR  // MCLR Pin Enable bit (RE3 input pin enabled; MCLR disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>

#define _XTAL_FREQ 8000000

#define R_TIME  50
#define Y_TIME  25

unsigned int option = 1;

void road1(void);
void road2(void);
void road3(void);
void road4(void);

// interrupt function 
void __interrupt() cardDetected(void){ 
    INTCONbits.GIE = 0;
    
    // ------------------- Road 1 -------------------
    if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0) {
        switch(option) {
            case 2:
                PORTBbits.RB3 = 1;
                break;
            case 3:
                PORTDbits.RD6 = 1;
                break;
            case 4:
                PORTCbits.RC6 = 1;
                break;
        }
        PORTBbits.RB6 = 1;
        __delay_ms(Y_TIME*100);
        road1();
        while((PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0))
                __delay_ms(10);
        /*switch(option) {
            case 2:
                PORTBbits.RB3 = 1;
                break;
            case 3:
                PORTDbits.RD6 = 1;
                break;
            case 4:
                PORTCbits.RC6 = 1;
                break;
        }
        PORTBbits.RB6 = 1;
        __delay_ms(Y_TIME*100);*/
    }
    // ------------------- Road 2 -------------------
    else if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 1) {
        switch(option) {
            case 1: 
                PORTBbits.RB6 = 1;
                break;
            case 3:
                PORTDbits.RD6 = 1;
                break;
            case 4:
                PORTCbits.RC6 = 1;
                break;
        }
        PORTBbits.RB3 = 1;
        __delay_ms(Y_TIME*100);
        road2();
        while((PORTAbits.RA0 == 0 && PORTAbits.RA1 == 1))
                __delay_ms(10);
        /*switch(option) {
            case 1: 
                PORTBbits.RB6 = 1;
                break;
            case 2:
                break;
            case 3:
                PORTDbits.RD6 = 1;
                break;
            case 4:
                PORTCbits.RC6 = 1;
                break;
        }
        PORTBbits.RB3 = 1;
        __delay_ms(Y_TIME*100);*/
    }
    // ------------------- Road 3 -------------------
    else if(PORTAbits.RA0 == 1 && PORTAbits.RA1 == 0) {
        switch(option) {
            case 1: 
                PORTBbits.RB6 = 1;
                break;
            case 2:
                PORTBbits.RB3 = 1;
                break;
            case 4:
                PORTCbits.RC6 = 1;
                break;
        }
        PORTDbits.RD6 = 1;
        __delay_ms(Y_TIME*100);
        road3();
        while((PORTAbits.RA0 == 1 && PORTAbits.RA1 == 0))
                __delay_ms(10);
        /*switch(option) {
            case 1: 
                PORTBbits.RB6 = 1;
                break;
            case 2:
                PORTBbits.RB3 = 1;
                break;
            case 4:
                PORTCbits.RC6 = 1;
                break;
        }
        PORTDbits.RD6 = 1;
        __delay_ms(Y_TIME*100);*/
    }
    // ------------------- Road 4 -------------------
    else if(PORTAbits.RA0 == 1 && PORTAbits.RA1 == 1) {
        switch(option) {
            case 1: 
                PORTBbits.RB6 = 1;
                break;
            case 2:
                PORTBbits.RB3 = 1;
                break;
            case 3:
                PORTDbits.RD6 = 1;
                break;
        }
        PORTCbits.RC6 = 1;
        __delay_ms(Y_TIME*100);
        road4();
        while((PORTAbits.RA0 == 1 && PORTAbits.RA1 == 1))
                __delay_ms(10);
        /*switch(option) {
            case 1: 
                PORTBbits.RB6 = 1;
                break;
            case 2:
                PORTBbits.RB3 = 1;
                break;
            case 3:
                PORTDbits.RD6 = 1;
                break;
        }
        PORTCbits.RC6 = 1;
        __delay_ms(Y_TIME*100);*/
    }
    
    INTCONbits.INT0IF = 0;
    INTCONbits.GIE = 1;
}

void main(void) {
    OSCCONbits.IRCF = 0b110;
    OSCTUNEbits.INTSRC = 1;
    OSCTUNEbits.TUN = 0b011111;

    OSCCONbits.IRCF = 0b111;
    
    INTCONbits.GIE = 1;
    INTCONbits.INT0IE = 1;
    
    PORTB = 0;
    PORTA = 0;
    PORTD = 0;
    PORTC = 0;
    TRISA = 0b00000011;
    TRISB = 0b00000001;
    TRISD = 0b00000000;
    TRISC = 0b00000000;
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    
    while(1) {
        option = 1;
        PORTBbits.RB6 = 1; // road 1 yellow
        PORTCbits.RC6 = 1; // road 4 yellow
        __delay_ms(Y_TIME*100);
        road1();
        __delay_ms(R_TIME*100);

        option = 2;
        PORTBbits.RB3 = 1; // road 2 yellow
        PORTBbits.RB6 = 1; // road 1 yellow
        __delay_ms(Y_TIME*100);
        road2();
        __delay_ms(R_TIME*100);

        option = 3;
        PORTDbits.RD6 = 1; // road 3 yellow
        PORTBbits.RB3 = 1; // road 2 yellow
        __delay_ms(Y_TIME*100);
        road3();
        __delay_ms(R_TIME*100);

        option = 4;
        PORTCbits.RC6 = 1; // road 4 yellow
        PORTDbits.RD6 = 1; // road 3 yellow
        __delay_ms(Y_TIME*100);
        road4();
        __delay_ms(R_TIME*100);
       
    }
    return;
}

void road1(void) {
    PORTBbits.RB7 = 0;
    PORTBbits.RB6 = 0;
    PORTBbits.RB5 = 1;

    PORTBbits.RB4 = 1;
    PORTBbits.RB3 = 0;
    PORTBbits.RB2 = 0;

    PORTDbits.RD7 = 1;
    PORTDbits.RD6 = 0;
    PORTDbits.RD5 = 0;

    PORTCbits.RC7 = 1;
    PORTCbits.RC6 = 0;
    PORTCbits.RC5 = 0;
}

void road2(void) {
    PORTBbits.RB7 = 1;
    PORTBbits.RB6 = 0;
    PORTBbits.RB5 = 0;

    PORTBbits.RB4 = 0;
    PORTBbits.RB3 = 0;
    PORTBbits.RB2 = 1;

    PORTDbits.RD7 = 1;
    PORTDbits.RD6 = 0;
    PORTDbits.RD5 = 0;

    PORTCbits.RC7 = 1;
    PORTCbits.RC6 = 0;
    PORTCbits.RC5 = 0;
}

void road3(void) {
    PORTBbits.RB7 = 1;
    PORTBbits.RB6 = 0;
    PORTBbits.RB5 = 0;

    PORTBbits.RB4 = 1;
    PORTBbits.RB3 = 0;
    PORTBbits.RB2 = 0;

    PORTDbits.RD7 = 0;
    PORTDbits.RD6 = 0;
    PORTDbits.RD5 = 1;

    PORTCbits.RC7 = 1;
    PORTCbits.RC6 = 0;
    PORTCbits.RC5 = 0;
}

void road4(void) {
    PORTBbits.RB7 = 1;
    PORTBbits.RB6 = 0;
    PORTBbits.RB5  = 0;

    PORTBbits.RB4 = 1;
    PORTBbits.RB3 = 0;
    PORTBbits.RB2 = 0;

    PORTDbits.RD7 = 1;
    PORTDbits.RD6 = 0;
    PORTDbits.RD5 = 0;

    PORTCbits.RC7 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC5 = 1;
}