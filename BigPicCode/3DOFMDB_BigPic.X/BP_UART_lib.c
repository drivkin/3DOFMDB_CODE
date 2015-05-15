#include "xc.h"
#include "BP_UART_lib.h"

void initUART_D0(void) {
    //pin mapping
    // there might be something you have to do to unlock pin remapping
    TRISDbits.TRISD11 = 0; // for output
    RPD11R = 0b0001; //RPD11, labeled SDO on the schematic, is U1TX

    TRISDbits.TRISD10 = 1; // for input
    U1RXR = 0b0011; //RPD10, labeled SDI on the schematic,  U1RX

    U1MODEbits.ON = 0; //disable UART1
    U1STAbits.URXEN = 1; //enable RX
    U1STAbits.UTXEN = 1; //enable UX

    //setup the baud rate to be 115,200, assuming the peripheral clock is 40MHz
    U1BRG = 0b10101;
    U1MODEbits.ON = 1; //enable UART1
}

void sendCharD0(char c) {
    U1TXREG = c;
}

char getCharD0(void) {
    return U1RXREG;
}

void initUART_D2(void){
        //pin mapping
    TRISDbits.TRISD2 = 0; // for output
    RPD2R = 0b0001; //RPD2, labeled SS2 on the schematic, is U3TX

    TRISDbits.TRISD3 = 1; // for input
    U3RXR = 0b0000; //RPD3, labeled SDI2 on the schematic,  U3RX

 // clearing registers
    U3MODE = 0;
    U3STA = 0;

    //setup the baud rate to be 115,200, assuming the peripheral clock is 40MHz
    U3BRG = 0b10101;

        //interrupts
    IEC4bits.U3TXIE = 0; // not going to be interested in this one

    IEC4bits.U3RXIE = 1; // will keep this one
    IFS4bits.U3RXIF = 0; // flag is reset
    IPC39bits.U3RXIP = 0b010; // priority 2, for now
    IPC39bits.U3TXIS = 0b00;
    IEC4bits.U3RXIE = 1; //enable RX interrupt

    U3STAbits.URXEN = 1; //enable RX
    U3STAbits.UTXEN = 1; //enable UX
    U3MODEbits.ON = 1; //enable UART3
}

void sendCharD2(char c){
    U3TXREG = c;
}
char getCharD2(void){
    return U3RXREG;
}


void initUART_Ext(void) {
    //pin mapping
    //RPB8 TO U6TX
    TRISBbits.TRISB8 = 0;
    RPB8R = 0b0100;

    //RPB6 TO U6RX
    TRISBbits.TRISB6 = 1;
    U6RXR = 0b0101;

    // clearing registers
    U6MODE = 0;
    U6STA = 0;

    //setup the baud rate to be 115,200, assuming the peripheral clock is 40MHz
    U6BRG = 0b10101;
    
        //interrupts
    IEC5bits.U6TXIE = 0; // not going to be interested in this one

    IEC5bits.U6RXIE = 1; // will keep this one
    IFS5bits.U6RXIF = 0; // flag is reset
    IPC47bits.U6RXIP = 0b010; // priority 2, for now
    IPC47bits.U6TXIS = 0b00;
    IEC5bits.U6RXIE = 1; //enable RX interrupt
    
    U6STAbits.URXEN = 1; //enable RX
    U6STAbits.UTXEN = 1; //enable UX
    U6MODEbits.ON = 1; //enable UART6

    
    //interrupts
//    IEC5bits.U6TXIE = 0; // not going to be interested in this one
//
//    IEC5bits.U6RXIE = 0; // will keep this one
//    IFS5bits.U6RXIF = 0; // flag is reset
//    IPC47bits.U6RXIP = 0b010; // top priority, for now
//    IEC5bits.U6RXIE = 1; //enable RX interrupt
}

void sendCharExt(char c) {
     U6TXREG = c;
}

char getCharExt(void) {
    return U6RXREG;
}