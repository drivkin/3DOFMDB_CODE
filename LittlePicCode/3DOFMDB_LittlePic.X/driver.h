/* 
 * File:   driver.h
 * Author: Dmitriy
 *
 * Created on May 19, 2015, 9:36 AM
 */

#ifndef DRIVER_H
#define	DRIVER_H

#define HALLA PORTBbits.RB3
#define HALLB PORTBbits.RB7
#define HALLC PORTAbits.RA4

#define LEDA PORTBbits.RB2
#define LEDB PORTAbits.RA2
#define LEDC PORTAbits.RA3

#define DCA PDC3
#define DCB PDC2
#define DCC PDC1

#define RA PORTBbits.RB11
#define RB PORTBbits.RB13
#define RC PORTBbits.RB15


#define CCW 0
#define CW 1
/*
 * void driverInit(void)\
 * 
 * Initialize driver. This sets up the PWM modules, and sets up interrupt driven 
 * drive logic. After this function is called, the motor will start spinning
 */
void driverInit(void);

#endif	/* DRIVER_H */

