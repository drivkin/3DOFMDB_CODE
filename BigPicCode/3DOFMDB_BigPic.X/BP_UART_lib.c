#include "xc.h"
#include "BP_UART_lib.h"
#include "peripheral/uart.h"

void initUART_D0(void){
//pin mapping
    // there might be something you have to do to unlock pin remapping
  TRISDbits.TRISD11 = 0; // for output
  RPD11R = 0b0001;  //RPD11, labeled SDO on the schematic, is U1TX

  TRISDbits.TRISD10 = 1; // for input
  U1RXR = 0b0011;  //RPD10, labeled SDI on the schematic,  U1RX

  U1MODEbits.ON = 0; //disable UART1
  U1STAbits.URXEN = 1; //enable RX
  U1STAbits.UTXEN = 1; //enable UX
  U1MODEbits.ON = 1; //enable UART1
}

void sendCharD0(char c){
    U1TXREG = c;
}

char getCharD0(void){
    return U1RXREG;
}