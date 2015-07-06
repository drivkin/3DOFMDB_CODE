#include "LP_UART_lib.h"
#include "stdint.h"
#include <xc.h>


void initUART (void){
    //for now, the pins configured as UART might be specific to the top left
    //little pic, since what gets mapped to UART depends on pin availability on
    //the big pic

    //setup U1TX
    RPOR3bits.RP41R = 0b000001; //RP41R. corresponds to RB9

    //U1RX
    RPINR18bits.U1RXR = 0b0100000; // RPI32 which is RB0

    //interrupts on every character receive (for DMA)
    U1STAbits.URXISEL = 0b00;
    
    //interrupts on every send (for DMA)
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXISEL1 = 0;

    U1MODEbits.UARTEN = 1; // enable UART
    U1STAbits.UTXEN = 1; // ENABLE TX

    U1BRG = 37; //should make 115,200 baud rate at 70MHz
}

void sendCharUART(char c){
    U1TXREG = c;
}

char getCharUART(void){
    return U1RXREG;
}

void sendFloatUART(float x){
    char* dummy;
    dummy = (char*)&x;
    int i=0;
    for(i=0;i<4;i++){
        sendCharUART(dummy[i]);
    }
}