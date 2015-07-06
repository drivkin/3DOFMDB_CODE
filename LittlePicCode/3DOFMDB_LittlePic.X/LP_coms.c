#include "stdint.h"
#include <xc.h>

void DMA_transfer(uint16_t size, uint8_t *SendBuffer){
	DMA0CONbits.SIZE = 1; //Byte
	DMA0CONbits.DIR = 1; //RAM-to-Peripheral
	DMA0CONbits.HALF = 0; //interrupt when all data moved
	DMA0CONbits.NULLW = 0; //Null write disabled
	DMA0CONbits.AMODE = 0; //Register Indirect with Post-Increment
	DMA0CONbits.MODE = 1; //one shot, no ping-pong


        DMA0REQbits.IRQSEL = 0b00001100; //select UART1 transmitter IRQ

	DMA0CNT = size;
	DMA0PAD = (volatile uint16_t) & U1TXREG; //peripheral address register
	DMA0STAL = (volatile uint16_t) SendBuffer; //This may need some tweaking~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	DMA0STAH = 0x0000;

	IFS0bits.DMA0IF = 0; // Clear DMA Interrupt Flag
	IEC0bits.DMA0IE = 1; // Enable DMA interrupt
	DMA0CONbits.CHEN = 1; // Enable DMA channel

        DMA0REQbits.FORCE = 1; //manual transfer initiation
}

void initTimestamp(void){
       //t5 does not generate interrupts, is used for timestamps
    T5CONbits.TON = 0;
    T5CONbits.TCS = 0;
    T5CONbits.TGATE = 0;
    T5CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    TMR5 = 0x0000;
    PR5 = 0xffff;
    IFS1bits.T5IF = 0;
    IEC1bits.T5IE = 0;
    T5CONbits.TON = 1;
}

void PrintWithTimestamp(float *toSend, uint16_t size) {
    uint16_t buffSize = 500;
    static float buff1[501];
    static float buff2[501];
    static uint16_t sb1 = 0;
    static uint16_t sb2 = 0;
    static uint16_t currbuff = 1;
    int i;

    if (currbuff == 1) {
        if (sb1 + size + 1 <= buffSize-1) {

            for (i = 0; i < size; i++) {
                buff1[sb1 + i] = toSend[i];
            }

            buff1[sb1 + size] = (float) TMR5; //TIMESTAMP
            sb1 = sb1 + size + 1;
        } else {
            // LED3 = 1;
            DMA_transfer((sb1)*4, (uint8_t*) buff1);
            sb1 = 0;
            currbuff = 2;


            for (i = 0; i < size; i++) {
                buff2[sb2 + i] = toSend[i];
            }
            buff2[sb2 + size] = (float) TMR5; //TIMESTAMP
            sb2 = sb2 + size + 1;
        }
    } else {

        if (sb2 + size + 1 <= buffSize-1) {

            for (i = 0; i < size; i++) {
                buff2[sb2 + i] = toSend[i];
            }
            buff2[sb2 + size] = (float) TMR5; //TIMESTAMP
            sb2 = sb2 + size + 1;
        } else {
          //  LED2 = 1;
            DMA_transfer((sb2)*4, (uint8_t*) buff2);
            sb2 = 0;
            currbuff = 1;


            for (i = 0; i < size; i++) {
                buff1[sb1 + i] = toSend[i];
            }
            buff1[sb1 + size] = (float) TMR5; //TIMESTAMP
            sb1 = sb1 + size + 1;
        }
    }
}