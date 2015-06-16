/*
 * ADC.h
 *
 *  Created on: 09/06/2015
 *      Author: Marc
 */

#ifndef APP_INC_ADC_H_
#define APP_INC_ADC_H_

#include "stm32f10x_lib.h"


u16 sampleADC(u8);
void init_ADC();
u16 isZero(u16 ADCres);

#define PIN_VDD_VOLT			GPIO_Pin_3

#define PORT_ADC_SELECT0        GPIOC
#define PORT_ADC_SELECT1        GPIOC
#define PIN_ADC_SELECT0         GPIO_Pin_1
#define PIN_ADC_SELECT1         GPIO_Pin_2

#define PORT_ADC0				GPIOC
#define PIN_ADC0				GPIO_Pin_0

#define PORT_ADC1				GPIOA
#define PIN_ADC1				GPIO_Pin_5
// PINS/PORTS for turning on IR LEDs
#define ADC_1_PORT_SIG_MOT            GPIOA
#define ADC_1_PIN_SIG_MOT1P           GPIO_Pin_0
#define ADC_1_PIN_SIG_MOT1M           GPIO_Pin_1

#define ADC_2_PORT_SIG_MOT            GPIOA
#define ADC_2_PIN_SIG_MOT1P           GPIO_Pin_2
#define ADC_2_PIN_SIG_MOT1M           GPIO_Pin_3

#define ADC_3_PORT_SIG_MOT            GPIOC
#define ADC_3_PIN_SIG_MOT1P           GPIO_Pin_6
#define ADC_3_PIN_SIG_MOT1M           GPIO_Pin_7

#define ADC_4_PORT_SIG_MOT            GPIOC
#define ADC_4_PIN_SIG_MOT1P           GPIO_Pin_8
#define ADC_4_PIN_SIG_MOT1M           GPIO_Pin_9

#define ADC_5_PORT_SIG_MOT            GPIOA
#define ADC_5_PIN_SIG_MOT1P           GPIO_Pin_8
#define ADC_5_PIN_SIG_MOT1M           GPIO_Pin_11

#define ADC_6_PORT_SIG_MOT            GPIOB
#define ADC_6_PIN_SIG_MOT1P           GPIO_Pin_8
#define ADC_6_PIN_SIG_MOT1M           GPIO_Pin_9

// ADC defines
#define NUM_ADC1							0x00
#define NUM_ADC2							0x01
#define NUM_ADC3							0x02
#define NUM_ADC4							0x03
#define NUM_ADC5							0x04
#define NUM_ADC6							0x05

#define SIG_ADC_0						ADC_Channel_10
#define SIG_ADC_1						ADC_Channel_5

#endif /* APP_INC_ADC_H_ */
