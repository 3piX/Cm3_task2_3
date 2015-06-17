/*
 * Zigbee.c

 *
 *  Created on: 16/06/2015
 *      Author: Marc
 */

#include "stm32f10x_lib.h"
#include "Zigbee.h"

#define	PORT_ZIGBEE_RESET	GPIOA
#define	PIN_ZIGBEE_RESET	GPIO_Pin_12

u32                             Baudrate_ZIGBEE = 57600;
u8 ZGB_RX_com_buf[15];
u8 ZGB_data_rdy = 0;
u8 ZGB_RX_buff_index = 0;



void initZigbee()
{ // Zigbee uses UART 5!

	USART_InitTypeDef USART_InitStructure;

	USART_StructInit(&USART_InitStructure);

	USART_InitStructure.USART_BaudRate = Baudrate_ZIGBEE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_DeInit(UART5);
	mDelay(10);
	/* Configure the UART5 */
	USART_Init(UART5, &USART_InitStructure);

	/* Enable UART5 Receive and Transmit interrupts */
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

	/* Enable the UART5 */
	USART_Cmd(UART5, ENABLE);

	GPIO_ResetBits(PORT_ZIGBEE_RESET, PIN_ZIGBEE_RESET);

}

void zigbeeTxWord(u16 data)
{

	u8 zgbData[6], i;


	zgbData[0] = 0xff;
	zgbData[1] = 0x55;
	zgbData[2] = (data&0x00FF);
	zgbData[3] = ~(data&0x00FF);
	zgbData[4] = ((data&0xFF00)>>8);
	zgbData[5] = ~((data&0xFF00)>>8);

	for(i = 0; i<6; i++)
	{
		USART_SendData(UART5,zgbData[i]);
		while( USART_GetFlagStatus(UART5, USART_FLAG_TC)==RESET );
	}

}

void zigbeeTxString(u16 *data)
{

	while (*data)
		zigbeeTxWord(*data++);


}

void zigbeeTxArray(u8 *data, u16 len)
{


}

void __zigbeeRxISR()
{
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) // IF RX interrupt RXNE bit auto-clear!
	{
		ZGB_RX_com_buf[ZGB_RX_buff_index] = USART_ReceiveData(UART5);
		ZGB_data_rdy = 1;
		//ZGB_RX_buff_index++;
	}


}


