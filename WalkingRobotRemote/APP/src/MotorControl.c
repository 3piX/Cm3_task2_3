/*
 * MotorControl.c
 *
 *  Created on: 09/06/2015
 *      Author: Marc
 */
#include "stm32f10x_lib.h"

#include "DXL.h"
#include "DXLdef.h"
#include "MotorControl.h"
#include "PC_Com.h"

#define MOTOR_LEFT_ADD 					10
#define MOTOR_RIGHT_ADD					9
#define MOTOR_STEP						2

#define RIGHT_MOTOR_CORR 				45

#define LEFT_FRONT_TOP_MOTOR			3
#define LEFT_FRONT_BOT_MOTOR			5
#define	RIGHT_FRONT_TOP_MOTOR			4
#define RIGHT_FRONT_BOT_MOTOR			1
#define	LEFT_BACK_TOP_MOTOR				8
#define LEFT_BACK_BOT_MOTOR				6
#define	RIGHT_BACK_TOP_MOTOR			2
#define RIGHT_BACK_BOT_MOTOR 			7

#define LEFT_FRONT_TOP_MOTOR_INI		299
#define LEFT_FRONT_BOT_MOTOR_INI		511 - 135
#define	RIGHT_FRONT_TOP_MOTOR_INI		703
#define RIGHT_FRONT_BOT_MOTOR_INI		511 + 135
#define	LEFT_BACK_TOP_MOTOR_INI			299
#define LEFT_BACK_BOT_MOTOR_INI			511 + 135
#define	RIGHT_BACK_TOP_MOTOR_INI		703
#define RIGHT_BACK_BOT_MOTOR_INI		511 - 135


#define BOT_MOTOR_INI 					511
#define LEFT_TOP_MOTOR_INI				299
#define RIGHT_TOP_MOTOR_INI				703

#define HIGH_DELAY						200
#define LOW_DELAY						75

u16 old_speed = MAX_SPEED;

void init_motors()
{
	// Set initial position of motors:

	DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);

	DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);
	DXL_send_word(LEFT_FRONT_TOP_MOTOR, GOAL_POSITION_L, LEFT_FRONT_TOP_MOTOR_INI);

	DXL_send_word(RIGHT_FRONT_TOP_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_TOP_MOTOR_INI);
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);

	DXL_send_word(LEFT_BACK_TOP_MOTOR, GOAL_POSITION_L, LEFT_BACK_TOP_MOTOR_INI);
	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI);

	DXL_send_word(RIGHT_BACK_TOP_MOTOR, GOAL_POSITION_L, RIGHT_BACK_TOP_MOTOR_INI);
	DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI);
}


void set_IR_position(u16 pos)
{
	if(pos > 1023)
	{
		pos = 1023;
	}

	DXL_send_word(MOTOR_STEP, GOAL_POSITION_L, pos);

}

void move_forward(u16 speed)
{
	// Lift two legs!

	liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, 110);

	mDelay(LOW_DELAY);

	// Take a right step (1)
	takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, 135, 1);

	//DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI-135);
	//DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);

	mDelay(HIGH_DELAY);

	// set down legs
	setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);

	mDelay(LOW_DELAY);

	// Lift two legs!
	liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, 110);

	mDelay(LOW_DELAY);

	// Take a left step (0)
	takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, 135, 0);

	mDelay(HIGH_DELAY);


	// sit down legs
	setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);

	mDelay(LOW_DELAY);
}

void move_backward(u16 speed)
{



}

void move_left(u16 speed)
{






}

void move_right(u16 speed)
{






}

void move_break()
{

}



void turnLeftOnSpot(u16 speed)
{
	DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);
	DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI);

	setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);
	mDelay(LOW_DELAY);

	// lift legs
	liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, 110);
	mDelay(LOW_DELAY);

	// Move to position for next step
	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI);
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);
	mDelay(HIGH_DELAY);

	// Set down legs
	setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);
	mDelay(LOW_DELAY);

	// Lift legs
	liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, 110);
	mDelay(LOW_DELAY);

	// take step
	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI-135);
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI-135);

//	// Set down legs
//	setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);
}

void turnRightOnSpot(u16 speed)
{
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);
	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI);

	setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);
	mDelay(LOW_DELAY);

	// lift legs
	liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, 110);
	mDelay(LOW_DELAY);

	// Move to position for next step
//	turnStep(LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, 135);

	DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI);
	DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);

	mDelay(HIGH_DELAY);

	// Set down legs
	setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);
	mDelay(LOW_DELAY);

	// Lift legs
	liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, 110);
	mDelay(LOW_DELAY);

	// take step
	DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI+135);
	DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI+135);


	// Set down legs
//	setDownLegs(LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR);
}

void setDownLegs(u8 motor1, u8 motor2)
{
	DXL_send_word(motor1, GOAL_POSITION_L, getLeftTopIni());
	DXL_send_word(motor2, GOAL_POSITION_L, getRightTopIni());
}

void liftUpLegs(u8 motor1, u8 motor2, u8 value)
{
	DXL_send_word(motor1, GOAL_POSITION_L, getLeftTopIni() + value);
	DXL_send_word(motor2, GOAL_POSITION_L, getRightTopIni() - value);
}

void takeStep(u8 motor1, u8 motor2, u8 motor3, u8 motor4, u8 value, u8 side)
{
	int botMotorIni = getBotIni();
	// Take a right or else take a left step

	if(side == 1)
	{
		DXL_send_word(motor1, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI - value);
		DXL_send_word(motor2, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);

		//DXL_send_word(motor1, GOAL_POSITION_L, botMotorIni);
		//DXL_send_word(motor2, GOAL_POSITION_L, botMotorIni + value);

		DXL_send_word(motor3, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI + value);
		DXL_send_word(motor4, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI);

		//DXL_send_word(motor3, GOAL_POSITION_L, botMotorIni);
		//DXL_send_word(motor4, GOAL_POSITION_L, botMotorIni - value);
	}
	else
	{
		DXL_send_word(motor1, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI );
		DXL_send_word(motor2, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI - value);


		DXL_send_word(motor3, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);
		DXL_send_word(motor4, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI + value);


//		DXL_send_word(motor1, GOAL_POSITION_L, botMotorIni + value);
//		DXL_send_word(motor2, GOAL_POSITION_L, botMotorIni);
//
//		DXL_send_word(motor3, GOAL_POSITION_L, botMotorIni - value);
//		DXL_send_word(motor4, GOAL_POSITION_L, botMotorIni);
	}

}

void turnStep(u8 motor1, u8 motor2, u8 value)
{
	int botMotorIni = getBotIni();
	DXL_send_word(motor1, GOAL_POSITION_L, botMotorIni + value);
	DXL_send_word(motor2, GOAL_POSITION_L, botMotorIni + value);
}

int getLeftTopIni()
{
	return LEFT_TOP_MOTOR_INI;
}

int getRightTopIni()
{
	return RIGHT_TOP_MOTOR_INI;
}

int getBotIni()
{
	return BOT_MOTOR_INI;
}
