/* 
 * File:   LP_coms.h
 * Author: Dmitriy
 *
 * This library provides communication functions for the little pic. The LP is
 * always the slave. During operation, most communication between the BP and LP
 * will be control commands. The BP might want to read some value each time as
 * well, such as motor velocity.
 *
 * During testing however, the LP might need to send large amounts of data to the
 * PC through the BP. For this we provide large parallel buffers and a function
 * to print whatever data is desired with a timestamp to the buffer. The buffers
 * are dumped (using DMA) to the BP when the BP sends a request. This
 * functionality may be disabled during real operation, or buffer sizes may be
 * reduced. Transfer should still be done using DMA though.
 *
 * All data is sent as floats.
 *
 *
 * Created on June 25, 2015, 10:47 AM
 */

#ifndef LP_COMS_H
#define	LP_COMS_H

/*void DMA_transfer(uint16_t size, uint8_t *SendBuffer);
 * 
 * Transfers data to BP over UART
 */
void DMA_transfer(uint16_t size, uint8_t *SendBuffer);

/*void initTimestamp(void);

 Starts the timestamp timer*/
void initTimestamp(void);

/*void PrintWithTimestamp(float *toSend, uint16_t size)
 *
 * Adds the given array to the print queue and appends a timestamp. When the
 * queue is full, it is automatically dumped over UART using DMA.
 */
void PrintWithTimestamp(float *toSend, uint16_t size);




#endif	/* LP_COMS_H */

