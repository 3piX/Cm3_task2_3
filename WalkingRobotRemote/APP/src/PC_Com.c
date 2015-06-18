/*
 * PC_Com.c
 *
 *  Created on: 08/06/2015
 *      Author: Marc
 */

#include "stm32f10x_lib.h"
#include "PC_Com.h"

/*---------------- GLOBAL-------------------*/
vu32                   gwTimingDelay,gw1msCounter;
/* GLOBAL VAR */

vu8 PC_RX_com_buf[15]; // FOR ISR

/* LOCAL VAR */
vu32 PC_RX_buff_index = 0;
vu8 PC_data_rdy = 0;





void USART_Configuration(u8 PORT, u32 baudrate)
{

	USART_InitTypeDef USART_InitStructure;

	USART_StructInit(&USART_InitStructure);


	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


	if( PORT == USART_DXL )
	{
		USART_DeInit(USART1);
		mDelay(10);
		/* Configure the USART1 */
		USART_Init(USART1, &USART_InitStructure);

		/* Enable USART1 Receive and Transmit interrupts */
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		//USART_ITConfig(USART1, USART_IT_TC, ENABLE);

		/* Enable the USART1 */
		USART_Cmd(USART1, ENABLE);
	}

	else if( PORT == USART_PC )
	{
		USART_DeInit(USART3);
		mDelay(10);
		/* Configure the USART3 */
		USART_Init(USART3, &USART_InitStructure);

		/* Enable USART3 Receive and Transmit interrupts */
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		//USART_ITConfig(USART3, USART_IT_TC, ENABLE);

		/* Enable the USART3 */
		USART_Cmd(USART3, ENABLE);
	}
}


void TxDString(u8 *bData)
{
	while (*bData)
		TxDByte_PC(*bData++);
}
void TxArray(u8 *bData, u8 len)
{
	int i;
	for(i = 0; i<len; i++)
	{
		TxDByte_PC(*bData++);
	}
}

void TxDWord16(u16 wSentData) {
	TxDByte_PC((wSentData >> 8) & 0xff);
	TxDByte_PC(wSentData & 0xff);
}
void TxWordArray(u16 *bData, u8 len)
{
	int i;
	for(i = 0; i<len; i++)
	{
		TxDWord16((u16)(bData));
		TxDByte_PC("\n");
		TxDByte_PC("\r");
		*bData+=1;
	}
}

void TxDByte_PC(u8 bTxdData)
{
	USART_SendData(USART3,bTxdData);
	while( USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET );
}

void mDelay(u32 nTime)
{
	uDelay(nTime*1000);
}

void uDelay(u32 nTime)
{
	/* Enable the SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Enable);

	gwTimingDelay = nTime;

	while(gwTimingDelay != 0);

	/* Disable SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Disable);
	/* Clear SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Clear);
}

void __PC_com_RX_ISR()
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // IF RX interrupt RXNE bit auto-clear!
	{
		PC_RX_com_buf[PC_RX_buff_index] = USART_ReceiveData(USART3);
		PC_data_rdy = 1;
		//PC_RX_buff_index++;
	}

}
