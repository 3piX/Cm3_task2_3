/*
 * Zigbee.h
 *
 *  Created on: 16/06/2015
 *      Author: Marc
 */

#ifndef APP_SRC_ZIGBEE_H_
#define APP_SRC_ZIGBEE_H_

#include "stm32f10x_lib.h"

void initZigbee();
void zigbeeTxString(u16 *data);
void zigbeeTxByte(u8 data);
void zigbeeTxArray(u8 *data, u16 len);
void __zigbeeRxISR();


extern u8 ZGB_RX_com_buf[15];
extern u8 ZGB_data_rdy;
extern u8 ZGB_RX_buff_index;



#endif /* APP_SRC_ZIGBEE_H_ */
