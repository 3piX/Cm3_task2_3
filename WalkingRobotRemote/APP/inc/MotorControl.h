/*
 * MotorControl.h
 *
 *  Created on: 09/06/2015
 *      Author: Marc
 */

#ifndef APP_INC_MOTORCONTROL_H_
#define APP_INC_MOTORCONTROL_H_

#include "stm32f10x_lib.h"

#define MAX_SPEED 800


void move_forward(u16 speed);
void move_backward(u16 speed);
void move_left(u16 speed);
void move_right(u16 speed);
void move_break();
void init_motors();
void set_IR_position(u16 pos);
void turnRightOnSpot(u16 speed);
void turnLeftOnSpot(u16 speed);
#endif /* APP_INC_MOTORCONTROL_H_ */
