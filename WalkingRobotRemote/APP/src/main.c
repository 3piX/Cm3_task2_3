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

#define FORWARD						0x77 //"w"
#define BACKWARD					0x73 //"s"
#define LEFT						0x61 //"a"
#define RIGHT						0x64 //"d"

#define LEFT_SPOT					0x6A //"j"
#define RIGHT_SPOT					0x6B //"k"

#define IR_SENSOR_RIGHT				2
#define IR_SENSOR_RIGHT_front		3
#define IR_SENSOR_FRONT				1
#define IR_SENSOR_LEFT_front		0
#define IR_SENSOR_LEFT				4
#define IR_LONG_DIST				5

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
float SCALE = 0.7f;

void __ISR_DELAY(void);
void startIRsweep();
void setWallTrackSide();




int main(void)
{
	s16 i = 0;
	u16 tempADCres, j = 0, k = 0;
	u16 sum, avg;
	float hans;
	double bo;
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



//	Init_Timer2();
//
//	TxDString(" HELLO :)\n\r");
//	DXL_RX_com_buf[14] = 0;
//
//	init_ADC();
//
//	GPIO_SetBits(ADC_6_PORT_SIG_MOT, ADC_6_PIN_SIG_MOT1P);
//	GPIO_ResetBits(ADC_6_PORT_SIG_MOT, ADC_6_PIN_SIG_MOT1M);
//	mDelay(1000);
//
//	setWallTrackSide();
//	mDelay(1000);
//
//	move_forward(MAX_SPEED);

	initZigbee();

	init_motors();
	init_motors();
	init_motors();



	while(1)
	{
		switch(ZGB_RX_com_buf[0])
		{
		case FORWARD:


			move_forward(0);


			break;
		case BACKWARD:

			move_backward(0);


			break;
		case LEFT:
		case LEFT_SPOT:

			turnLeftOnSpot(0);

			break;
		case RIGHT:
		case RIGHT_SPOT:

			turnRightOnSpot(0);

			break;
		default:
			break;
		}

//		if(ZGB_RX_com_buf[0] == '1')
//		{
//			move_forward(0);
//
//			GPIO_ResetBits(PORT_LED_PROGRAM, PIN_LED_PROGRAM);
//			zigbeeTxWord(((0x31)<<8)|(0x0D));
//			ZGB_RX_com_buf[0] = 0;
//		}
//		else if (ZGB_RX_com_buf[0] == '2')
//		{
//			turnLeftOnSpot(0);
//			GPIO_SetBits(PORT_LED_PROGRAM, PIN_LED_PROGRAM);
//			zigbeeTxWord(((0x32)<<8)|(0x0D));
//			ZGB_RX_com_buf[0] = 0;
//		}

		uDelay(100);

// SIMPLE ORIENTATION BEHAVIOUR
/*

		for (j = 0; j<6; j++)
		{
			ADCres_buf[j] = (sampleADC(NUM_ADC1+j)+sampleADC(NUM_ADC1+j) + sampleADC(NUM_ADC1+j)+sampleADC(NUM_ADC1+j))>>2;

		}

		if(ADCres_buf[IR_SENSOR_FRONT] > 0)
		{
			move_break();

			mDelay(300);

		while((sampleADC(IR_SENSOR_FRONT) + sampleADC(IR_SENSOR_FRONT) + sampleADC(IR_SENSOR_FRONT)+sampleADC(IR_SENSOR_FRONT))>>2){	


			if(wallTrackSide == WALL_TRACK_RIGHT)
				{
					turnLeftOnSpot(700);
				}
				else
				{
					turnRightOnSpot(700);
				}

				mDelay(50);
				
				move_break();

				mDelay(150);
			}

			move_forward(800);

		}
		else if((ADCres_buf[IR_SENSOR_RIGHT] >ADCres_buf[IR_SENSOR_LEFT]) || (ADCres_buf[IR_SENSOR_RIGHT_front] > ADCres_buf[IR_SENSOR_LEFT_front]))
		{
			move_left(((ADCres_buf[IR_SENSOR_RIGHT] - ADCres_buf[IR_SENSOR_LEFT])) + (((ADCres_buf[IR_SENSOR_RIGHT_front]-ADCres_buf[IR_SENSOR_LEFT_front]))*6) + ADCres_buf[IR_SENSOR_RIGHT_front]*8);
		}
		else if((ADCres_buf[IR_SENSOR_LEFT] > ADCres_buf[IR_SENSOR_RIGHT]) || (ADCres_buf[IR_SENSOR_LEFT_front] > ADCres_buf[IR_SENSOR_RIGHT_front]) )
		{
			move_right(((ADCres_buf[IR_SENSOR_LEFT] - ADCres_buf[IR_SENSOR_RIGHT])) + (((ADCres_buf[IR_SENSOR_LEFT_front]-ADCres_buf[IR_SENSOR_RIGHT_front]))*6) + ADCres_buf[IR_SENSOR_RIGHT_front]*8);
		}
		else if(ADCres_buf[IR_LONG_DIST] >= 850)//Wall track
		{
			if(wallTrackSide == WALL_TRACK_RIGHT)
			{
				move_left((ADCres_buf[IR_LONG_DIST]-805));
			}
			else
			{
				move_right((ADCres_buf[IR_LONG_DIST]-805));
			}
			
		}
		else if(ADCres_buf[IR_LONG_DIST] <= 700) // wall track
		{
			if(wallTrackSide == WALL_TRACK_RIGHT)
			{
				move_right((745-(ADCres_buf[IR_LONG_DIST])));
			}
			else
			{
				move_left((745-(ADCres_buf[IR_LONG_DIST])));
			}
			
		}
		else
		{
			move_forward(MAX_SPEED);
		}

		uDelay(10);
*/

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

		//DXL_read_byte(2,MOVING); // If sweep is done (Motor is not moving)

		if(1)//!(DXL_RX_com_buf[5]&0x01)) // disable intterupt
		{
			ADCres_buf[ADCres_buf_index] =sampleADC(NUM_ADC6);// ((sampleADC(NUM_ADC6) + sampleADC(NUM_ADC6))>>1) + ((sampleADC(NUM_ADC6) + sampleADC(NUM_ADC6))>>1);

			if(sweepDirection == 0)
			{
				set_IR_position(count);
				count += 60;
			}
			else
			{
				set_IR_position(count);
				count -= 60;
			}

			if(count >=814 || count <= 214)
			{
				TIM2->CR1 &= ~TIM_CR1_CEN;
				IRsweepDone = 1;
				sweepDirection ^= 0x01;
			}
		}


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
	set_IR_position(330);
	set_IR_position(330);
	mDelay(500);
	ADCres_buf[0] = sampleADC(NUM_ADC6);


	set_IR_position(698);
	set_IR_position(698);
	mDelay(500);
	ADCres_buf[1] = sampleADC(NUM_ADC6);

	if(ADCres_buf[0] > ADCres_buf[1])
	{
		wallTrackSide = WALL_TRACK_RIGHT;
		set_IR_position(330);

	}
	else
	{

		wallTrackSide = WALL_TRACK_LEFT;
		set_IR_position(698);

	}
}

