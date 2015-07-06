/* 
 * File:   main.c
 * Author: Dmitriy
 *
 * Created on April 27, 2015, 1:29 PM
 */

#include "BP_coms.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include <sys/attribs.h>
#include "BP_UART_lib.h"
#include "config.h"
#include "sys/kmem.h"
#include "string.h"

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

void configurePeripheralBus(void) {
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


#ifdef UART_SHOOT_THROUGH_POLLING
    void main(void) {
    __builtin_enable_interrupts(); // enable interrupts
    INTCONbits.MVEC = 1;

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

    TRISBbits.TRISB7 = 0;
    PORTBbits.RB7 = 1;

    TRISAbits.TRISA10 = 0;
    PORTAbits.RA10 = 0;

    initUART_Ext();
    initUART_D2();


// disable interrupts in this mode
    IEC4bits.U3RXIE = 0;
    IEC5bits.U6RXIE = 0;
    while (1) {
        while(!U3STAbits.URXDA);
        sendCharExt(getCharD2());
    }
}
#endif

#ifdef BUFFERED_UART_SHOOT_THROUGH

/* in this mode, the big pic is just a link between the external uart and the UART
 for D2*/
void main(void) {
    __builtin_enable_interrupts(); // enable interrupts
    INTCONbits.MVEC = 1;

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

    TRISBbits.TRISB7 = 0;
    PORTBbits.RB7 = 1;

    TRISAbits.TRISA10 = 0;
    PORTAbits.RA10 = 0;

    initUART_Ext();
    //in this case, we want Tx interrupts from the external UART to tell us to
    //send more data
    U6STAbits.UTXISEL = 0b01; //interrupt generated when all characters have been transmitted
    IEC5bits.U6TXIE = 0; // need to generate an interrupt when transmit buffer empty for DMA
    IFS5bits.U6TXIF = 0; // flag is reset
    IPC47bits.U6TXIP = 0b010; // priority 2, for now
    IPC47bits.U6TXIS = 0b00;
    IEC5bits.U6TXIE = 0; // enable TX interrupt


    initUART_D2();
    while (1) {
    }
}

uint8_t cbuff[10000];
int length = 10000;
int tail = 0;
int head = 0;

void addToHead(uint8_t dat){
    cbuff[head] = dat;
    head = head+1;
    if(head == length){
        head = 0;
    }
    if(IEC5bits.U6TXIE == 0){
        IEC5bits.U6TXIE = 1; // enable TX interrupt
        sendFromTail();
    }
      
}

void sendFromTail(){
    if(tail!=head){
        sendCharExt(cbuff[tail]);
        tail++;
        if(tail == length){
            tail = 0;
        }
    }else{
        IEC5bits.U6TXIE = 0; // disable TX interrupt
    }
}

//for d2

void __ISR(_UART3_RX_VECTOR, IPL2AUTO) UART3_RX_ISR(void) {
    addToHead(getCharD2());
    //sendFromTail();
    IFS4bits.U3RXIF = 0;
}

void __ISR(_UART6_TX_VECTOR, IPL2AUTO) UART6_TX_ISR(void) {
    IFS5bits.U6TXIF = 0;
    sendFromTail();
}



//for external
void __ISR(_UART6_RX_VECTOR, IPL2AUTO) UART6_RX_ISR(void) {
    IFS5bits.U6RXIF = 0;
}


#endif


#ifdef PRINT_ADDRESSES
void main(void) {
    __builtin_enable_interrupts(); // enable interrupts
    INTCONbits.MVEC = 1;

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

    TRISBbits.TRISB7 = 0;
    PORTBbits.RB7 = 1;

    TRISAbits.TRISA10 = 0;
    PORTAbits.RA10 = 0;

    initUART_Ext();
    while (1) {
    }
}
void __ISR(_UART6_RX_VECTOR, IPL2AUTO) UART6_RX_ISR(void) {
    //read and echo
//        char c = getCharExt();
//        sendCharExt(c)
    getCharExt();
    char str[100];
    int length=3;
    length = sprintf(str,"%x",KVA_TO_PA(&U3RXREG));
    int i=0;
    for(i;i<length;i++){
        sendCharExt(str[i]);
     }
    IFS5bits.U6RXIF = 0;
}
#endif


#ifdef DMA_UART_SHOOT_THROUGH
void main(void) {
    __builtin_enable_interrupts(); // enable interrupts
    INTCONbits.MVEC = 1;

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

    TRISBbits.TRISB7 = 0;
    PORTBbits.RB7 = 1;

    TRISAbits.TRISA10 = 0;
    PORTAbits.RA10 = 0;

    initUART_Ext();
    initUART_D2();
    initDMA();
    enable_DMA_UART_Throughput_D2_Ext();
    while (1) {
    }
}
void __ISR(_UART6_RX_VECTOR, IPL2AUTO) UART6_RX_ISR(void) {
    //read and echo
        char c = getCharExt();
  //      sendCharExt(c);
}
#endif

#ifdef TEST_DMA_EXT
void main(void) {
    __builtin_enable_interrupts(); // enable interrupts
    INTCONbits.MVEC = 1;

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
    initDMA();

    uint8_t ar[100];
    uint8_t i=0;
    for(i;i<100;i++){
        ar[i] = i;
    }

    while (1) {
        int b = 0;
        for (b; b < 10000; b++);
        DMA_Transfer_Ext(ar,100);
    }
}

void __ISR(_UART6_RX_VECTOR, IPL2AUTO) UART6_RX_ISR(void) {
    //read and echo
    //    char c = getCharExt();
    //    sendCharExt(c);
    IFS5bits.U6RXIF = 0;
    uint8_t ar[100];
    uint8_t i=0;
    for(i;i<100;i++){
        ar[i] = i;
    }
    DMA_Transfer_Ext(ar,100);
}

void __ISR(_UART6_TX_VECTOR, IPL2AUTO) UART6_TX_ISR(void) {
    IFS5bits.U6TXIF = 0;
}

#endif

#ifdef UART_SHOOT_THROUGH

/* in this mode, the big pic is just a link between the external uart and the UART
 for D2*/
void main(void) {
    __builtin_enable_interrupts(); // enable interrupts
    INTCONbits.MVEC = 1;

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

    TRISBbits.TRISB7 = 0;
    PORTBbits.RB7 = 1;

    TRISAbits.TRISA10 = 0;
    PORTAbits.RA10 = 0;

    initUART_Ext();
    initUART_D2();
    while (1) {
    }
}

//for d2

void __ISR(_UART3_RX_VECTOR, IPL2AUTO) UART3_RX_ISR(void) {
//    while(U3STAbits.URXDA){
//        sendCharExt(getCharD2());
//    }
    sendCharExt(getCharD2());
    IFS4bits.U3RXIF = 0;
}

//for external

void __ISR(_UART6_RX_VECTOR, IPL2AUTO) UART6_RX_ISR(void) {
    //read and echo
    char c = getCharExt();
    sendCharD2(c);
    IFS5bits.U6RXIF = 0;
}

#endif

#ifdef TEST_PINS

int main(void) {
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

    while (1) {
        int i = 0;

        for (i; i < 100000; i++) {
            Nop();
        }
        PORTA ^= 0xffffffff;
        PORTB ^= 0xffffffff;
        PORTC ^= 0xffffffff;
        PORTD ^= 0xffffffff;
        PORTE ^= 0xffffffff;
        //        PORTF ^=0xffffffff;
        PORTG ^= 0xffffffff;
    }

}
#endif

#ifdef TEST_UART_EXT

void main(void) {
    __builtin_enable_interrupts(); // enable interrupts
    INTCONbits.MVEC = 1;

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
    while (1) {
    }
}

void __ISR(_UART6_RX_VECTOR, IPL2AUTO) UART6_RX_ISR(void) {
    //read and echo
    char c = getCharExt();
    sendCharExt(c);
    IFS5bits.U6RXIF = 0;
}

void __ISR(_UART6_TX_VECTOR, IPL2AUTO) UART6_TX_ISR(void) {
    IFS5bits.U6TXIF = 0;
}

#endif

#ifdef TEST_UART_0

int main(void) {

    J2_12TRIS = 0; // set j2 as output
    J2_12PORT = 1; //
    initUART_D0();
    while (1) {
        int i = 0;
        for (i; i < 10; i++) {
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
    while (1) {
        int i = 0;
        for (i; i < 1; i++) {
            Nop();
        }
        J2_12PORT ^= 1;
    }
    return (EXIT_SUCCESS);
}
#endif
