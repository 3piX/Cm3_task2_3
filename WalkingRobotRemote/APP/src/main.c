/************************* (C) COPYRIGHT 2010 ROBOTIS **************************
* File Name          : main.c
* Author             : danceww
* Version            : V0.0.1
* Date               : 08/23/2010
* Description        : Main program body
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_lib.h"


#include "sys_init.h"
#include "PC_Com.h"
#include "DXL.h"
#include "DXLdef.h"
#include "MotorControl.h"
#include "Zigbee.h"
#include <math.h>

#include "ADC.h"

#define AUTO

#define FORWARD						0x77 //"w"
#define BACKWARD					0x73 //"s"
#define LEFT						0x61 //"a"
#define RIGHT						0x64 //"d"

#define LEFT_SPOT					0x6A //"j"
#define RIGHT_SPOT					0x6B //"k"

#define IR_SENSOR_FRONT				1
#define IR_LONG_DIST				2

#define	WALL_TRACK_RIGHT			0
#define	WALL_TRACK_LEFT				1


#define PORT_LED_AUX			GPIOB
#define PORT_LED_MANAGE			GPIOB
#define PORT_LED_PROGRAM		GPIOB
#define PORT_LED_PLAY			GPIOB
#define PORT_LED_POWER			GPIOC
#define PORT_LED_TX				GPIOC
#define PORT_LED_RX				GPIOC

#define PIN_LED_AUX				GPIO_Pin_12
#define PIN_LED_MANAGE			GPIO_Pin_13
#define PIN_LED_PROGRAM			GPIO_Pin_14
#define PIN_LED_PLAY			GPIO_Pin_15
#define PIN_LED_POWER			GPIO_Pin_13
#define PIN_LED_TX				GPIO_Pin_14
#define PIN_LED_RX				GPIO_Pin_15

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u32                             Baudrate_DXL = 	1000000;
u32                             Baudrate_PC = 57600;
u16								ADCres_buf[50];
u16								ADCres_buf_index = 0;
u16 							diff = 0;
u8								wallTrackSide = 0;
u8								IRsweepDone, sweepDirection = 0;
vu16								count = 214;
u8								rightTrackOffset;
float SCALE = 0.7f;

void __ISR_DELAY(void);
void startIRsweep();
void setWallTrackSide();




int main(void)
{

	u16 j = 0;
    /* System Clocks Configuration */
	RCC_Configuration();

	/* NVIC configuration */
	NVIC_Configuration();

	/* GPIO configuration */
	GPIO_Configuration(); // Setup of IOs
	//GPIO_Start();

	SysTick_Configuration(); // used for delay function (PC_Com.c)

	DXL_init(1000000);

	USART_Configuration(USART_PC, Baudrate_PC);



	DXL_RX_com_buf[14] = 0;

	init_ADC();

	GPIO_SetBits(ADC_5_PORT_SIG_MOT, ADC_5_PIN_SIG_MOT1P);
	GPIO_ResetBits(ADC_5_PORT_SIG_MOT, ADC_5_PIN_SIG_MOT1M);

	init_motors();
	init_motors();
	init_motors();

#ifdef AUTO
	setWallTrackSide();
#endif

	initZigbee();




	while(1)
	{

#ifndef AUTO
		switch(ZGB_RX_com_buf[0])
		{
		case FORWARD:


			move_forward(2);


			break;
		case BACKWARD:

			move_backward(2);


			break;
		case LEFT:

			move_left(2);

			break;
		case LEFT_SPOT:

			turnLeftOnSpot(2);

			break;
		case RIGHT:

			move_right(2);

			break;
		case RIGHT_SPOT:

			turnRightOnSpot(2);

			break;
		default:
			break;
		}



		uDelay(10);
#else


// SIMPLE ORIENTATION BEHAVIOUR


		for (j = 0; j<3; j++)
		{
			ADCres_buf[j] = (sampleADC(NUM_ADC3+j)+sampleADC(NUM_ADC3+j) + sampleADC(NUM_ADC3+j)+sampleADC(NUM_ADC3+j))>>2;

		}


			if(ADCres_buf[IR_LONG_DIST] > rightTrackOffset)
			{
				ADCres_buf[IR_LONG_DIST] -= rightTrackOffset;
			}
			else
			{
				ADCres_buf[IR_LONG_DIST] = 0;
			}


		if(ADCres_buf[IR_SENSOR_FRONT] > 0)
		{
			move_backward(0);

			if(wallTrackSide == WALL_TRACK_RIGHT)
				{
					turnLeftOnSpot(0);
					turnLeftOnSpot(0);
				}
				else
				{
					turnRightOnSpot(0);
					turnRightOnSpot(0);
				}

		}
		else if(ADCres_buf[IR_LONG_DIST] >= 420)//Wall track
		{
			if(wallTrackSide == WALL_TRACK_RIGHT)
			{
				//move_left(3);
				move_left((ADCres_buf[IR_LONG_DIST]/300));
			}
			else
			{
				//move_right(3);
				move_right((ADCres_buf[IR_LONG_DIST]/400));
			}
			
		}
		else if(ADCres_buf[IR_LONG_DIST] <= 350) // wall track
		{
			if(wallTrackSide == WALL_TRACK_RIGHT)
			{
				move_right((3-(ADCres_buf[IR_LONG_DIST]/175)));
				//move_right(3);
			}
			else
			{
				move_left(3-(ADCres_buf[IR_LONG_DIST]/175));
				//move_left(3);
			}
			
		}
		else
		{
			move_forward(0);
		}

		uDelay(10);

#endif
	}


	return 0;
}


void __TIM2_ISR()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{


		ADCres_buf_index++;
		TIM2->SR &= ~TIM_IT_CC1;
		TIM2->CNT = 0;

		TIM2->CR1 &= ~TIM_CR1_CEN;
	}
}


void __ISR_DELAY(void)
{
	if (gwTimingDelay != 0x00)
		gwTimingDelay--;
}
void startIRsweep()
{

	IRsweepDone = 0;
	ADCres_buf_index = 0;


	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_CEN; // ENABLE TIMER!

}

void setWallTrackSide()
{
	set_IR_position(350);
	mDelay(500);
	ADCres_buf[0] = sampleADC(NUM_ADC5);



	if(ADCres_buf[0] > 400)
	{
		wallTrackSide = WALL_TRACK_RIGHT;
		rightTrackOffset = 150;

	}
	else
	{

		wallTrackSide = WALL_TRACK_LEFT;
		set_IR_position(760);
		rightTrackOffset = 0;

	}
}

