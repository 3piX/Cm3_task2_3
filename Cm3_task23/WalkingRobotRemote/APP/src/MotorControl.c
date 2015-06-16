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

#define LEFT_FRONT_TOP_MOTOR_INI		309
#define LEFT_FRONT_BOT_MOTOR_INI		511
#define	RIGHT_FRONT_TOP_MOTOR_INI		683
#define RIGHT_FRONT_BOT_MOTOR_INI		511
#define	LEFT_BACK_TOP_MOTOR_INI			309
#define LEFT_BACK_BOT_MOTOR_INI			511
#define	RIGHT_BACK_TOP_MOTOR_INI		683
#define RIGHT_BACK_BOT_MOTOR_INI		511

#define BOT_MOTOR_INI 					511
#define LEFT_TOP_MOTOR_INI				309
#define RIGHT_TOP_MOTOR_INI				683

u16 old_speed = MAX_SPEED;

void init_motors()
{
	// Set initial position of motors:

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

	liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, 80);

	mDelay(100);

	// ready for next step
	// set position ready for next step
	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI);
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI+135);

	//WALK!
	DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);
	DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI-135);

	mDelay(300);

	// set down legs
	setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);

	mDelay(100);

	// Lift two legs!
	liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, 80);

	mDelay(100);

	//WALK!
	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI+135);
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);

	// set position ready for next step
	DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI-135);
	DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI);

	mDelay(300);


	// sit down legs
	setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);

	mDelay(100);
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
	//DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, 0);
	//DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, 0);

	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, 250-RIGHT_MOTOR_CORR);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L,1024+250);
}



void turnLeftOnSpot(u16 speed)
{
	//Sit down legs
	DXL_send_word(LEFT_FRONT_TOP_MOTOR, GOAL_POSITION_L, LEFT_FRONT_TOP_MOTOR_INI);
	DXL_send_word(RIGHT_BACK_TOP_MOTOR, GOAL_POSITION_L, RIGHT_BACK_TOP_MOTOR_INI);

	mDelay(100);
	// lift legs
	DXL_send_word(LEFT_BACK_TOP_MOTOR, GOAL_POSITION_L, LEFT_BACK_TOP_MOTOR_INI + 80);
	DXL_send_word(RIGHT_FRONT_TOP_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_TOP_MOTOR_INI - 80);

	mDelay(100);
	// Move to position for next step
	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI+135);
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI+135);

	mDelay(300);

	// Sit down legs
	DXL_send_word(LEFT_BACK_TOP_MOTOR, GOAL_POSITION_L, LEFT_BACK_TOP_MOTOR_INI);
	DXL_send_word(RIGHT_FRONT_TOP_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_TOP_MOTOR_INI);

	mDelay(100);

	DXL_send_word(LEFT_FRONT_TOP_MOTOR, GOAL_POSITION_L, LEFT_FRONT_TOP_MOTOR_INI + 80);
	DXL_send_word(RIGHT_BACK_TOP_MOTOR, GOAL_POSITION_L, RIGHT_BACK_TOP_MOTOR_INI - 80);

	mDelay(100);

	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI);
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);

}

void turnRightOnSpot(u16 speed)
{

	
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