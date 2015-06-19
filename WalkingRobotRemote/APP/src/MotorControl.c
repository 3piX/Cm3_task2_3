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

#define MOTOR_STEP						9

#define RIGHT_MOTOR_CORR 				45

#define LEFT_FRONT_TOP_MOTOR			3
#define LEFT_FRONT_BOT_MOTOR			5
#define	RIGHT_FRONT_TOP_MOTOR			4
#define RIGHT_FRONT_BOT_MOTOR			1
#define	LEFT_BACK_TOP_MOTOR				8
#define LEFT_BACK_BOT_MOTOR				6
#define	RIGHT_BACK_TOP_MOTOR			2
#define RIGHT_BACK_BOT_MOTOR 			7

#define LEFT_FRONT_TOP_MOTOR_INI		300
#define LEFT_FRONT_BOT_MOTOR_INI		320//376
#define	RIGHT_FRONT_TOP_MOTOR_INI		740
#define RIGHT_FRONT_BOT_MOTOR_INI		702//646
#define	LEFT_BACK_TOP_MOTOR_INI			300
#define LEFT_BACK_BOT_MOTOR_INI			702//646//
#define	RIGHT_BACK_TOP_MOTOR_INI		740
#define RIGHT_BACK_BOT_MOTOR_INI		320//376//

#define RL_FRONT_BACK_BOT_MOTOR_INI 	702//646//
#define RL_BACK_FRONT_BOT_MOTOR_INI 	320//376//
#define LEFT_TOP_MOTOR_INI				300
#define RIGHT_TOP_MOTOR_INI				740

/* delay constants */
#define HIGH_DELAY						130
#define LOW_DELAY						70

/* movement constants */
#define LIFT							120
#define MOV								230//170//

u16 old_speed = MAX_SPEED;

void takeStep(u8 motor1, u8 motor2, u8 motor3, u8 motor4, u8 value, u8 side);
void turnStep(u8 motor1, u8 motor2, u8 value, u8 side);
void liftUpLegs(u8 motor1, u8 motor2, u8 value);
void setDownLegs(u8 motor1, u8 motor2);
int getLeftTopIni();
int getRightTopIni();
int getFrontBackBotIni();
int getBackFrontBotIni();

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

			liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, LIFT);


			mDelay(LOW_DELAY);

			// Take a right step (1)
			takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, MOV, 1);

			//DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI-MOV);
			//DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);

			mDelay(HIGH_DELAY);

			// set down legs
			setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);

			mDelay(HIGH_DELAY);

			// Lift two legs!
			liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, LIFT);

			mDelay(LOW_DELAY);

			// Take a left step (0)
			takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, MOV, 0);

			mDelay(HIGH_DELAY);


			// sit down legs
			setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);

			mDelay(LOW_DELAY);
	}
	
void move_backward(u16 speed)
	{
	// Lift two legs!

			liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, LIFT);

			mDelay(LOW_DELAY);

			// Take a right step (1)
			takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, MOV, 1);

			//DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI-MOV);
			//DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);

			mDelay(HIGH_DELAY);

			// set down legs
			setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);

			mDelay(HIGH_DELAY);

			// Lift two legs!
			liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, LIFT);

			mDelay(LOW_DELAY);

			// Take a left step (0)
			takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, MOV, 0);

			mDelay(HIGH_DELAY);


			// sit down legs
			setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);

			mDelay(LOW_DELAY);

}

void move_left(u16 speed)
{


	// Lift two legs!

			liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, LIFT);

			mDelay(LOW_DELAY);

			// Take a right step (1)
			takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, MOV, 1);

			//DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI - MOV);
			//DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI );

			//DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI + MOV);
			//DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI );

			mDelay(HIGH_DELAY);

			// set down legs
			setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);

			mDelay(HIGH_DELAY);

			// Lift two legs!
			liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, LIFT);

			mDelay(LOW_DELAY);

			// Take a left step (0)
			takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, MOV/speed, 0);

			//DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI );
			//DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI - MOV/2);

			//DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);
			//DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI + MOV/2);
			mDelay(HIGH_DELAY);


			// sit down legs
			setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);

			mDelay(LOW_DELAY);
}

void move_right(u16 speed)
{// Lift two legs!

	liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, LIFT);

	mDelay(LOW_DELAY);

	// Take a right step (1)
	takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, MOV/speed, 1);

	//DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI - MOV/2);
	//DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);


	//DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI + MOV/2);
	//DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI);

	mDelay(HIGH_DELAY);

	// set down legs
	setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);

	mDelay(HIGH_DELAY);

	// Lift two legs!
	liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, LIFT);

	mDelay(LOW_DELAY);

	// Take a left step (0)
	takeStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, MOV, 0);

	//DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI );
	//DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI - MOV);

	//DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);
	//DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI + MOV);
	mDelay(HIGH_DELAY);


	// sit down legs
	setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);

	mDelay(LOW_DELAY);


}

void move_break()
{

}



void turnLeftOnSpot(u16 speed)
{

	turnStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, MOV, 1);
	mDelay(HIGH_DELAY);

	setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);
	mDelay(LOW_DELAY);

	// lift legs
	liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, LIFT);
	mDelay(LOW_DELAY);

	// Move to position for next step
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);
	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI);

	turnStep(LEFT_FRONT_BOT_MOTOR, RIGHT_BACK_BOT_MOTOR, MOV, 0);

	mDelay(HIGH_DELAY);

	// Set down legs
	setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);
	mDelay(HIGH_DELAY);

	// Lift legs
	liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, LIFT);
	mDelay(LOW_DELAY);

	// take step
	DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI);
	DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);


	//DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI+135);
	//DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI+135);


}

void turnRightOnSpot(u16 speed)
{
	DXL_send_word(RIGHT_FRONT_BOT_MOTOR, GOAL_POSITION_L, RIGHT_FRONT_BOT_MOTOR_INI);
	DXL_send_word(LEFT_BACK_BOT_MOTOR, GOAL_POSITION_L, LEFT_BACK_BOT_MOTOR_INI);
	mDelay(HIGH_DELAY);
	setDownLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR);
	mDelay(LOW_DELAY);

	// lift legs
	liftUpLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR, LIFT);
	mDelay(LOW_DELAY);

	// Move to position for next step
	turnStep(LEFT_BACK_BOT_MOTOR, RIGHT_FRONT_BOT_MOTOR, MOV, 1);

	DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI);
	DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI);

	mDelay(HIGH_DELAY);

	// Set down legs
	setDownLegs(LEFT_FRONT_TOP_MOTOR, RIGHT_BACK_TOP_MOTOR);
	mDelay(HIGH_DELAY);

	// Lift legs
	liftUpLegs(LEFT_BACK_TOP_MOTOR, RIGHT_FRONT_TOP_MOTOR, LIFT);
	mDelay(LOW_DELAY);

	// take step
	turnStep(RIGHT_BACK_BOT_MOTOR, LEFT_FRONT_BOT_MOTOR, MOV, 0);

	//DXL_send_word(RIGHT_BACK_BOT_MOTOR, GOAL_POSITION_L, RIGHT_BACK_BOT_MOTOR_INI+135);
	//DXL_send_word(LEFT_FRONT_BOT_MOTOR, GOAL_POSITION_L, LEFT_FRONT_BOT_MOTOR_INI+135);


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
	int botFrontBackMotorIni = getFrontBackBotIni();
	int botBackFrontMotorIni = getBackFrontBotIni();
	
	// Take a right or else take a left step
	if(side == 1)
	{
		DXL_send_word(motor1, GOAL_POSITION_L, botFrontBackMotorIni - value);
		DXL_send_word(motor2, GOAL_POSITION_L, botFrontBackMotorIni);

		//DXL_send_word(motor1, GOAL_POSITION_L, botMotorIni);
		//DXL_send_word(motor2, GOAL_POSITION_L, botMotorIni + value);

		DXL_send_word(motor3, GOAL_POSITION_L, botBackFrontMotorIni + value);
		DXL_send_word(motor4, GOAL_POSITION_L, botBackFrontMotorIni);


		//DXL_send_word(motor3, GOAL_POSITION_L, botMotorIni);
		//DXL_send_word(motor4, GOAL_POSITION_L, botMotorIni - value);
	}
	else
	{
		DXL_send_word(motor1, GOAL_POSITION_L, botFrontBackMotorIni );
		DXL_send_word(motor2, GOAL_POSITION_L, botFrontBackMotorIni - value);


		DXL_send_word(motor3, GOAL_POSITION_L, botBackFrontMotorIni);
		DXL_send_word(motor4, GOAL_POSITION_L, botBackFrontMotorIni + value);


//		DXL_send_word(motor1, GOAL_POSITION_L, botMotorIni + value);
//		DXL_send_word(motor2, GOAL_POSITION_L, botMotorIni);
//
//		DXL_send_word(motor3, GOAL_POSITION_L, botMotorIni - value);
//		DXL_send_word(motor4, GOAL_POSITION_L, botMotorIni);
	}

}

void turnStep(u8 motor1, u8 motor2, u8 value, u8 side)
{
	int botMotorIni = 0;
	
	if(side == 1)
		botMotorIni = getFrontBackBotIni();
	else
		botMotorIni = getBackFrontBotIni();
	
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

int getFrontBackBotIni()
{
	return RL_FRONT_BACK_BOT_MOTOR_INI;
}

int getBackFrontBotIni()
{
	return RL_BACK_FRONT_BOT_MOTOR_INI;
}
