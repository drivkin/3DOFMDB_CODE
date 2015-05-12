/* 
 * File:   BP_UART_lib.h
 * Author: Dmitriy
 *
 * Created on April 28, 2015, 5:35 PM
 */

#ifndef BP_UART_LIB_H
#define	BP_UART_LIB_H

/* void initUART_D0(void)
 *
 * Initializes UART communications with the motor driver diagonally across from
 * the big pic
 *
 */

void initUART_D0(void);


/*void sendCharD0(char c)
 *
 * Add a char to the TX queue to D0
 */
void sendCharD0(char c);


/*char getCharD0(void)
 *
 * get the first char from the RX queue for D0
 *
 *  */
char getCharD0(void);

/*functions for External UART, going through J2 to the computer*/
void initUART_Ext(void);
void sendCharExt(char c);
char getCharExt(void);


#endif	/* BP_UART_LIB_H */

