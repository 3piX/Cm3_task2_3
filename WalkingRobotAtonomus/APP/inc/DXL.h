/*
 * AX12W.h
 *
 *  Created on: 08/06/2015
 *      Author: Marc
 */

#include "stm32f10x_lib.h"

void DXL_init(u32 );//baud



/* FOR ISR */
void DXL_RX_interrupt(void);

void DXL_send_word(u8 , u8 , u16);
void DXL_read_byte(u8 devId, u8 add);

extern vu8 DXL_RX_com_buf[15];
extern u8 DXL_TX_com_buf[15];
