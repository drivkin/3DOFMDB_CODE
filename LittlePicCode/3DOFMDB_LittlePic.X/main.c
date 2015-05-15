/* 
 * File:   main.c
 * Author: Dmitriy
 *
 * Created on May 12, 2015, 10:58 AM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "LP_UART_lib.h"

//#define HELLOWORLD
#define TEST_UART

/*
 * 
 */

_FOSCSEL(FNOSC_FRC & IESO_OFF);
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
_FWDT(FWDTEN_OFF);
_FICD(ICS_PGD1 & JTAGEN_OFF);

void ClockInit(void) {
    // 140.03 MHz VCO  -- 70 MIPS
    PLLFBD = 74;
    CLKDIVbits.PLLPRE = 0;
    CLKDIVbits.PLLPOST = 0;

    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);

    __builtin_write_OSCCONL(OSCCON | 0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001);

    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1);
}
#define LEDA PORTBbits.RB2
#define LEDB PORTAbits.RA2
#define LEDC PORTAbits.RA3


#ifdef TEST_UART

void main(void) {
    //LEDA
    ClockInit();

    TRISA = 0xffff;
    TRISB = 0xffff;

    ANSELB = 0x0000;

    ANSELBbits.ANSB2 = 0;
    TRISBbits.TRISB2 = 0;

    //LEDB
    TRISAbits.TRISA2 = 0;

    //LEDC
    TRISAbits.TRISA3 = 0;

    LEDA = 0;
    LEDB = 0;
    LEDC = 0;
    initUART();
    while (1) {
        if(U1STAbits.URXDA){
            LEDB ^=1;
            sendCharUART(getCharUART());
            //getCharUART();
            //sendFloatUART(1344466.53453);
        }
    }

}
#endif


#ifdef HELLOWORLD

void main(void) {
    //LEDA
    ClockInit();
    ANSELBbits.ANSB2 = 0;
    TRISBbits.TRISB2 = 0;

    //LEDB
    TRISAbits.TRISA2 = 0;

    //LEDC
    TRISAbits.TRISA3 = 0;

    LEDA = 0;
    LEDB = 0;
    LEDC = 0;
    while (1) {
        uint32_t i = 0;
        for (i; i < 3000000; i++);
        LEDA ^= 1;
        LEDB ^= 1;
        LEDC ^= 1;
    }

}

#endif