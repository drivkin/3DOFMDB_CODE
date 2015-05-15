/* 
 * File:   LP_UART_lib.h
 * Author: Dmitriy
 *
 * Created on May 12, 2015, 12:17 PM
 */

#ifndef LP_UART_LIB_H
#define	LP_UART_LIB_H

/*void initUART (void);
 *
 * Initialize UART TX and RX, but no interrupts. The controller will poll the UART
 * registers to see if there's a new command.
 */
void initUART (void);

void sendCharUART(char c);
char getCharUART(void);
void sendFloatUART(float x);
#endif	/* LP_UART_LIB_H */

