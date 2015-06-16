/*
 * sys_init.h
 *
 *  Created on: 08/06/2015
 *      Author: Marc
 */


#define CCR1_Val 				100 		// 1ms

#define PIN_LED_POWER			GPIO_Pin_13

#define PORT_ENABLE_TXD			GPIOB
#define PORT_ENABLE_RXD			GPIOB
#define PORT_DXL_TXD			GPIOB
#define PORT_DXL_RXD			GPIOB

#define PIN_ENABLE_TXD			GPIO_Pin_4
#define PIN_ENABLE_RXD			GPIO_Pin_5
#define PIN_DXL_TXD				GPIO_Pin_6
#define PIN_DXL_RXD				GPIO_Pin_7
#define PIN_PC_TXD				GPIO_Pin_10
#define PIN_PC_RXD              GPIO_Pin_11


#define PORT_ADC_0				PORTC
#define PORT_ADC_1				PORTA

#define PIN_ENABLE_TXD			GPIO_Pin_4
#define PIN_ENABLE_RXD			GPIO_Pin_5
#define PIN_DXL_TXD				GPIO_Pin_6
#define PIN_DXL_RXD				GPIO_Pin_7
#define PIN_PC_TXD				GPIO_Pin_10
#define PIN_PC_RXD              GPIO_Pin_11

void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void SysTick_Configuration(void);
void Init_Timer2();



