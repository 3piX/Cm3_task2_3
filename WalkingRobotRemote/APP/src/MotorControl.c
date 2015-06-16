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

#define MOTOR_LEFT_ADD 	10
#define MOTOR_RIGHT_ADD	9
#define MOTOR_STEP	2

#define RIGHT_MOTOR_CORR 45

#define LEFT_FRONT_TOP_MOTOR	3
#define LEFT_FRONT_BOT_MOTOR	5
#define	RIGHT_FRONT_TOP_MOTOR	4
#define RIGHT_FRONT_BOT_MOTOR	1
#define	LEFT_BACK_TOP_MOTOR		8
#define LEFT_BACK_BOT_MOTOR		6
#define	RIGHT_BACK_TOP_MOTOR	2
#define RIGHT_BACK_BOT_MOTOR	7

#define LEFT_FRONT_TOP_MOTOR_INI	309
#define LEFT_FRONT_BOT_MOTOR_INI	511
#define	RIGHT_FRONT_TOP_MOTOR_INI	683
#define RIGHT_FRONT_BOT_MOTOR_INI	511
#define	LEFT_BACK_TOP_MOTOR_INI		309
#define LEFT_BACK_BOT_MOTOR_INI		511
#define	RIGHT_BACK_TOP_MOTOR_INI	683
#define RIGHT_BACK_BOT_MOTOR_INI	511


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
	u16 temp = 0;

	if(speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}

	temp = 1024 + speed; //Set MSB!
	old_speed = temp; // save speed for "turn" function
	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, temp-RIGHT_MOTOR_CORR);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, speed);
}

void move_backward(u16 speed)
{
	u16 temp = 0;
	if(speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}

	temp = 1024 + speed; //Set MSB!
	old_speed = speed;
	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, speed-RIGHT_MOTOR_CORR);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, temp);

}

void move_left(u16 speed)
{

	if(speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}
	speed = old_speed - speed;

	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, old_speed);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, speed^1024);

}

void move_right(u16 speed)
{

	if(speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}
	speed = old_speed - speed;

	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, speed);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, old_speed^1024);

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
	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, speed^1024);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, speed^1024);
}

void turnRightOnSpot(u16 speed)
{

	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, speed);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, speed);	
	
}
