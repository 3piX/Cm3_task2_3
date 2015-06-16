/*
 * PC_Com.h
 *
 *  Created on: 08/06/2015
 *      Author: Marc
 */


#include "stm32f10x_lib.h"

#define USART_DXL			    0
#define USART_PC			    2


void TxDString(u8*);
void TxArray(u8 *bData, u8 len);
void TxWordArray(u16 *bData, u8 len);
void TxDByte_PC(u8);

void USART_Configuration(u8, u32);
void PC_com_RX_ISR();

void mDelay(u32 nTime);
void uDelay(u32 nTime);

extern volatile vu32                   gwTimingDelay,gw1msCounter;
extern vu8 PC_RX_com_buf[15]; // FOR ISR
extern vu8 PC_data_rdy;
