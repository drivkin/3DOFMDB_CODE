#include <xc.h>
#include "BP_coms.h"
#include "stdint.h"
#include "config.h"
#include "sys/kmem.h"



void initDMA(void){
   DMACONbits.ON =1;
}


void enable_DMA_UART_Throughput_D2_Ext(void){
    DCH0CONbits.CHEN = 1; //enable DMA channel 0
    DCH0CONbits.CHPRI = 0b11; // highest priority (00 is lowest)

    DCH0SSA = KVA_TO_PA(&U3RXREG); //source start address physical
    DCH0DSA = KVA_TO_PA(&U6TXREG); //Destination start address,  physical address of U6TXREG

    DCH0INT = 0; //Disable DMA interrupts
    DCH0CONbits.CHAED =1; //allow events even if channel is disabled

    DCH0SSIZ = 1; //length of source is 1 byte
    DCH0DSIZ = 1; // destination length is 1 byte
    DCH0CSIZ = 1; // one byte per transfer
    DCH0INTCLR=0x00ff00ff; //clear all interrupts

    DCH0ECONbits.CHSIRQ = _UART3_RX_VECTOR; //
    DCH0ECONbits.SIRQEN = 1; //allow channel to respond to interrupt event
    DCH0ECONbits.CFORCE = 1; //force transfer
}


