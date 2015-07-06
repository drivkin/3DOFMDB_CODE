/* 
 * File:   BP_coms.h
 * Author: Dmitriy
 *
 * This library implements a communications protocol between the BP and LP, and
 * the BP and the PC
 *
 * Created on June 25, 2015, 11:31 AM
 */

#ifndef BP_COMS_H
#define	BP_COMS_H


#include "stdint.h"

/*void initDMA(void)
 
 Initialize DMA module*/
void initDMA(void);




/*void enable_DMA_UART_Throughput_D0_Ext(void);

 Use DMA to make the BP take incoming data on the D0 UART and output it on the
 External UART*/
void enable_DMA_UART_Throughput_D2_Ext(void);

#endif	/* BP_COMS_H */

