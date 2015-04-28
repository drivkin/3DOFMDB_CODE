/* 
 * File:   main.c
 * Author: Dmitriy
 *
 * Created on April 27, 2015, 1:29 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#pragma config FMIIEN = OFF
#pragma config FETHIO = OFF
#pragma config PGL1WAY = OFF
#pragma config IOL1WAY = OFF
#pragma config FUSBIDIO = OFF
#pragma config FPLLIDIV = DIV_1
#pragma config FPLLRNG = RANGE_5_10_MHZ
#pragma config FPLLMULT = MUL_10
#pragma config FPLLODIV = DIV_2
#pragma config UPLLFSEL = FREQ_12MHZ
#pragma config UPLLEN = OFF
#pragma config FNOSC = SPLL // OSCILLATOR SELECTION, CHECK IT'S NOT PLL
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
//#pragma config JTAGEN = OFF
//#pragma config ICESEL = ICS_PGx1



#define J2_1PORT PORTBbits.RB6
#define J2_1TRIS TRISBbits.TRISB6
/*
 * 
 */
int main(void) {
    ANSELBbits.ANSB6 = 0; // set j2 as digital output
    J2_1TRIS = 0; // set j2 as output
    J2_1PORT = 1; //
    while(1){
        int i = 0;
        for(i;i<1;i++){
            Nop();
        }
        J2_1PORT ^= 1;
    }
    return (EXIT_SUCCESS);
}

