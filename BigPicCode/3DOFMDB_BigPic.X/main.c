/* 
 * File:   main.c
 * Author: Dmitriy
 *
 * Created on April 27, 2015, 1:29 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#pragma config POSCMOD = OFF // External Clock disabled
#pragma config FPLLIDIV = DIV_1
#pragma config FNOSC = SPLL // System Clock is through System PLL
#pragma config FPLLICLK = PLL_FRC // Input to PLL is from Primary Oscillator
#pragma config FPLLRNG = RANGE_5_10_MHZ // 8MHZ
#pragma config FPLLMULT = MUL_20 // Multiply frequency by 20
#pragma config FPLLODIV = DIV_2 // Divide VCO output by 2

#define J2_1PORT PORTBbits.RB6
#define J2_1TRIS TRISBbits.TRISB6
/*
 * 
 */
int main(void) {
    J2_1TRIS = 0; // set j2 as output
    J2_1PORT = 1; //
    while(1){
        int i = 0;
        for(i;i<100000;i++){
            Nop();
        }
        J2_1PORT ^= 1;
    }
    return (EXIT_SUCCESS);
}

