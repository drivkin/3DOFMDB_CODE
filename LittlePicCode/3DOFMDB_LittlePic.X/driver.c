
#include <xc.h>
#include "stdint.h"
#include "driver.h"


float u = 1;
//initialize the PWM. This will be different for block commutation vs SVM

void PWMInit(void) {
    PTCON = 0x0000;
    // only the pwm lines are configured as PWM. The Reset lines are just logic pins
    //pwm lock must be off for this to work

    IOCON1bits.PENH = 1; //pwm
    IOCON1bits.PENL = 0; //reset
    IOCON2bits.PENH = 1; //pwm
    IOCON2bits.PENL = 0; //reset
    IOCON3bits.PENH = 1; //pwm
    IOCON3bits.PENL = 0; //reset

    //setup the reset pins as outputs
    TRISBbits.TRISB11 = 0; //A
    TRISBbits.TRISB13 = 0; //B
    TRISBbits.TRISB15 = 0; //C

    /* Set PWM Period on Primary Time Base */
    PTPER = 1000;

    /* Set Phase Shift */
    PHASE1 = 0;
    PHASE2 = 0;
    PHASE3 = 0;

    /*set duty cycles*/
    PDC1 = 0;
    PDC2 = 0;
    PDC3 = 0;

    /* Set Primary Time Base, Edge-Aligned Mode and Independent Duty Cycles */
    PWMCON1 = PWMCON2 = PWMCON3 = 0x0000;

    //disable faults
    FCLCON1 = FCLCON2 = FCLCON3 = 0x0003;

    //prescaler 1:1
    PTCON2bits.PCLKDIV = 0b000;

    //enable PWM
    PTCONbits.PTEN = 1;
}


//Init change notification for hall effect sensors

void CNInit(void) {
    //Set up Change Notify Interrupt
    CNENBbits.CNIEB3 = 1; // HALL A
    CNENBbits.CNIEB7 = 1; // HALL B
    CNENAbits.CNIEA4 = 1; // HALL C

    IEC1bits.CNIE = 1; // enable CN interrupt
    IPC4bits.CNIP = 0b001; // interrupt priority
    IFS1bits.CNIF = 0; // Reset CN interrupt flag
}

//initialize hall effect sensors as inputs

void HallInit(void) {
    TRISBbits.TRISB3 = 1; // A

    // it is very bad that HallB is connected to the FLT32 pin, and is being moved
    // to RB7 using surgery. This will change in future iterations
    TRISBbits.TRISB7 = 1; // B
    TRISAbits.TRISA4 = 1; // C


}

// init LEDs as outputs

void LEDInit(void) {
    TRISBbits.TRISB2 = 0; //A
    TRISAbits.TRISA2 = 0; //B
    TRISAbits.TRISA3 = 0; //C
}

void TrapUpdate(uint16_t dc, uint16_t direction) {
    if (dc > PTPER) {
        dc = PTPER;
    }
//
//    if (HALLA) {
//        LEDA = 1;
//    } else {
//        LEDA = 0;
//    }
//    if (HALLB) {
//        LEDB = 1;
//    } else {
//        LEDB = 0;
//    }
//    if (HALLC) {
//        LEDC = 1;
//    } else {
//        LEDC = 0;
//    }
    //ACTIVE LOW RESETS
    // For some reason you have to set LEDB before LEDA, otherwise LEDB isn't set
    // WTF? Same for resets
    if (direction == CW) {
        if ((HALLA && HALLB && !HALLC)) {
            RB = 1;
            RA = 0;
            
            RC = 1;

            DCA = 0;
            DCB = dc;
            DCC = 0;

            //            GH_A_DC = 0;
            //            GL_A_DC = 0;
            //            GH_B_DC = torque;
            //            GL_B_DC = 0;
            //            GH_C_DC = 0;
            //            GL_C_DC = torque;
            LEDB = 1;
            LEDA = 1;
            LEDC = 0;
        } else if ((!HALLA && HALLB && !HALLC)) {
            RB = 1;
            RA = 1;
            
            RC = 0;

            DCA = 0;
            DCB = dc;
            DCC = 0;

            //            GH_A_DC = 0;
            //            GL_A_DC = torque;
            //            GH_B_DC = torque;
            //            GL_B_DC = 0;
            //            GH_C_DC = 0;
            //            GL_C_DC = 0;
            LEDB = 1;
            LEDA = 0;
            LEDC = 0;
        } else if ((!HALLA && HALLB && HALLC)) {
            RB = 0;
            RA = 1;
            
            RC = 1;

            DCA = 0;
            DCB = 0;
            DCC = dc;

            //            GH_A_DC = 0;
            //            GL_A_DC = torque;
            //            GH_B_DC = 0;
            //            GL_B_DC = 0;
            //            GH_C_DC = torque;
            //            GL_C_DC = 0;
            LEDB = 1;
            LEDA = 0;
            
            LEDC = 1;
        } else if ((!HALLA && !HALLB && HALLC)) {
            RB = 1;
            RA = 0;
            
            RC = 1;

            DCA = 0;
            DCB = 0;
            DCC = dc;
            //
            //            GH_A_DC = 0;
            //            GL_A_DC = 0;
            //            GH_B_DC = 0;
            //            GL_B_DC = torque;
            //            GH_C_DC = torque;
            //            GL_C_DC = 0;
            LEDB = 0;
            LEDA = 0;
            
            LEDC = 1;
        } else if ((HALLA && !HALLB && HALLC)) {
            RB = 1;
            RA = 1;
            
            RC = 0;

            DCA = dc;
            DCB = 0;
            DCC = 0;

            //            GH_A_DC = torque;
            //            GL_A_DC = 0;
            //            GH_B_DC = 0;
            //            GL_B_DC = torque;
            //            GH_C_DC = 0;
            //            GL_C_DC = 0;
            LEDB = 0;
            LEDA = 1;
            
            LEDC = 1;
        } else if ((HALLA && !HALLB && !HALLC)) {
            RB = 0;
            RA = 1;
            
            RC = 1;

            DCA = dc;
            DCB = 0;
            DCC = 0;

            //            GH_A_DC = torque;
            //            GL_A_DC = 0;
            //            GH_B_DC = 0;
            //            GL_B_DC = 0;
            //            GH_C_DC = 0;
            //            GL_C_DC = torque;
             LEDB = 0;
            LEDA = 1;
           
            LEDC = 0;
        }
    } else if (direction == CCW) {
        if ((HALLA && HALLB && !HALLC)) {
            RB = 1;
            RA = 0;
            
            RC = 1;

            DCA = 0;
            DCB = 0;
            DCC = dc;

            //            GH_A_DC = 0;
            //            GL_A_DC = 0;
            //            GH_B_DC = 0;
            //            GL_B_DC = torque;
            //            GH_C_DC = torque;
            //            GL_C_DC = 0;
            LEDB = 1;
            LEDA = 1;
            
            LEDC = 0;
        } else if ((!HALLA && HALLB && !HALLC)) {
            RB = 1;
            RA = 1;
            
            RC = 0;

            DCA = dc;
            DCB = 0;
            DCC = 0;

            //            GH_A_DC = torque;
            //            GL_A_DC = 0;
            //            GH_B_DC = 0;
            //            GL_B_DC = torque;
            //            GH_C_DC = 0;
            //            GL_C_DC = 0;
            LEDB = 1;
            LEDA = 0;
            
            LEDC = 0;
        } else if ((!HALLA && HALLB && HALLC)) {
            RB = 0;
            RA = 1;
            
            RC = 1;

            DCA = dc;
            DCB = 0;
            DCC = 0;
            //
            //            GH_A_DC = torque;
            //            GL_A_DC = 0;
            //            GH_B_DC = 0;
            //            GL_B_DC = 0;
            //            GH_C_DC = 0;
            //            GL_C_DC = torque;
            LEDB = 1;
            LEDA = 0;
            
            LEDC = 1;
        } else if ((!HALLA && !HALLB && HALLC)) {
            RB = 1;
            RA = 0;
            
            RC = 1;

            DCA = 0;
            DCB = dc;
            DCC = 0;

            //            GH_A_DC = 0;
            //            GL_A_DC = 0;
            //            GH_B_DC = torque;
            //            GL_B_DC = 0;
            //            GH_C_DC = 0;
            //            GL_C_DC = torque;
            LEDB = 0;
            LEDA = 0;
            
            LEDC = 1;
        } else if ((HALLA && !HALLB && HALLC)) {
            RB = 1;
            RA = 1;
            
            RC = 0;

            DCA = 0;
            DCB = dc;
            DCC = 0;
            //
            //            GH_A_DC = 0;
            //            GL_A_DC = torque;
            //            GH_B_DC = torque;
            //            GL_B_DC = 0;
            //            GH_C_DC = 0;
            //            GL_C_DC = 0;
            LEDB = 0;
            LEDA = 1;
            
            LEDC = 1;
        } else if ((HALLA && !HALLB && !HALLC)) {
            RB = 0;
            RA = 1;
            
            RC = 1;

            DCA = 0;
            DCB = 0;
            DCC = dc;

            //            GH_A_DC = 0;
            //            GL_A_DC = torque;
            //            GH_B_DC = 0;
            //            GL_B_DC = 0;
            //            GH_C_DC = torque;
            //            GL_C_DC = 0;
            LEDB = 0;
            LEDA = 1;
            
            LEDC = 0;
        }
    }

}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {
    //hallUpdate();
    if (u < 0) {
        TrapUpdate((uint16_t) (-1 * u * PTPER), CCW);
    } else if (u >= 0) {
        TrapUpdate((uint16_t) (u * PTPER), CW);
    }
    //    if (HALLA) {
    //        LEDA = 1;
    //    } else {
    //        LEDA = 0;
    //    }
    //    if (HALLB) {
    //        LEDB = 1;
    //    } else {
    //        LEDB = 0;
    //    }
    //    if (HALLC) {
    //        LEDC = 1;
    //    } else {
    //        LEDC = 0;
    //    }
    // LEDA = LEDB = LEDC =1;
    IFS1bits.CNIF = 0; // Clear CN interrupt
}

void driverInit(void) {

    LEDInit();
    PWMInit();
    HallInit();
    CNInit();
}

