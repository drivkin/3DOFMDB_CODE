/* 
 * File:   main.c
 * Author: Dmitriy
 *
 * Created on April 27, 2015, 1:29 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "BP_UART_lib.h"

#pragma config FMIIEN = OFF
#pragma config FETHIO = OFF
#pragma config PGL1WAY = OFF
#pragma config IOL1WAY = OFF
#pragma config FUSBIDIO = OFF
#pragma config FPLLIDIV = DIV_1
#pragma config FPLLRNG = RANGE_5_10_MHZ
#pragma config FPLLMULT = MUL_20
#pragma config FPLLODIV = DIV_2
#pragma config UPLLFSEL = FREQ_12MHZ
#pragma config UPLLEN = OFF
#pragma config FNOSC = SPLL 
#pragma config DMTINTV = WIN_0
#pragma config FSOSCEN = OFF
#pragma config IESO = OFF
#pragma config POSCMOD = OFF
#pragma config OSCIOFNC = OFF
#pragma config FCKSM = CSDCMD
#pragma config WDTPS = PS1
#pragma config WDTSPGM = STOP
#pragma config WINDIS = NORMAL
#pragma config FWDTEN = OFF
#pragma config FWDTWINSZ = WINSZ_25
#pragma config DMTCNT = DMT8
#pragma config FDMTEN = OFF // this one is important




#define J2_2PORT PORTBbits.RB6
#define J2_2TRIS TRISBbits.TRISB6

#define J2_12PORT PORTBbits.RB13
#define J2_12TRIS TRISBbits.TRISB13



//#define HELLOWORLD
//#define TEST_UART_0
#define TEST_UART_EXT
//#define TEST_PINS

void configurePeripheralBus(void){
    // I think that the main clock (SYSCLK) is 8MHz*20/2 = 80MHz
    // by default, all peripheral busses except bus 7 are SYSCLK/2, but the divider can be adjusted
    // Peripheral busses are:
    //PBCLK2 : PMP ; I2C ; UART SPI
    //PBCLK3: ADC (SFR access only) ; Comparator ; Timers, Output compare; input capture
    //PBCLK4: Ports
    //PBCLK5: Flash; Crypto; RNG; USB (SFR); CAN; Ethernet (SFR);SQI (SFR)
    //PBCLK7: CPU, deadman timer
    //PBCLK8: EBI
}

#ifdef TEST_PINS
int main(void){
    //pins f12 and f13 don't work for some unknown reason
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;

    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    TRISF = 0xffffffff;
    TRISG = 0;

     while(1){
        int i = 0;

        for(i;i<100000;i++){
            Nop();
        }
        PORTA ^=0xffffffff;
        PORTB ^=0xffffffff;
        PORTC ^=0xffffffff;
        PORTD ^=0xffffffff;
        PORTE ^=0xffffffff;
//        PORTF ^=0xffffffff;
        PORTG ^=0xffffffff;
    }

}
#endif

#ifdef TEST_UART_EXT
void main(void){
    __builtin_enable_interrupts(); // enable interrupts

    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;

    TRISA = 0xffffffff;
    TRISB = 0xffffffff;
    TRISC = 0xffffffff;
    TRISD = 0xffffffff;
    TRISE = 0xffffffff;
    TRISF = 0xffffffff;
    TRISG = 0xffffffff;



    initUART_Ext();
    while(1){
    }
}


void __ISR(_UART6_RX_VECTOR, IPL2AUTO) UART6_RX_ISR(void){
   //read and echo
    char c = getCharExt();
    sendCharExt(c);
    IFS5bits.U6RXIF = 0;
}

#endif

#ifdef TEST_UART_0
int main(void){
  
    J2_12TRIS = 0; // set j2 as output
    J2_12PORT = 1; //
    initUART_D0();
    while(1){
        int i = 0;
        for(i;i<10;i++){
            Nop();
        }
        sendCharD0('p');
        J2_12PORT ^= 1;
    }
}


#endif


#ifdef HELLOWORLD
int main(void) {
    J2_12TRIS = 0; // set j2 as output
    J2_12PORT = 1; //
    while(1){
        int i = 0;
        for(i;i<1;i++){
            Nop();
        }
        J2_12PORT ^= 1;
    }
    return (EXIT_SUCCESS);
}
#endif
