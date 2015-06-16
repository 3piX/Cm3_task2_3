/*
 * DXLdef.h
 *
 *  Created on: 08/06/2015
 *      Author: Marc
 */

#ifndef APP_INC_DXLDEF_H_
#define APP_INC_DXLDEF_H_


/* INSTRUCTIONS */
#define PING				0x01
#define READ_DATA 			0x02
#define WRITE_DATA			0x03
#define REG_WRITE			0x04
#define	ACTION				0x05
#define DXL_RESET			0x06
#define SYNC_WRITE			0x83
//LOLO
/*ADDRESSES*/
#define MODEL_NUMBER_L		0x00
#define MODEL_NUMBER_H		0x01
#define FIRMWARE_VERSION	0x02
#define ID					0x03
#define BAUD_RATE			0x04
#define RETURN_DELAY_TIME	0x05
#define CW_ANGLE_LIMIT_L	0x06
#define CW_ANGLE_LIMIT_H	0x07
#define CCW_ANGLE_LIMIT_L	0x08
#define CCW_ANGLE_LIMIT_H	0x09
#define TEMPERATURE_LIMIT_H 0x0B
#define VOLTAGE_LIMIT_L		0x0C
#define VOLTAGE_LIMIT_H		0x0D
#define MAX_TORQUE_L		0x0E
#define MAX_TORQUE_H		0x0F
#define STATUS_RETURN_LEVEL 0x10
#define ALARM_LED			0x11
#define ALARM_SHUTDOWN		0x12
#define TORQUE_ENABLE		0x18
#define LED					0x19
#define CW_COMPLIANCE_MARG  0x1A
#define CCW_COMPLIANCE_MARG 0x1B
#define CW_COMP_SLOPE		0x1C
#define CCW_COMP_SLOPE		0x1D
#define GOAL_POSITION_L		0x1E
#define GOAL_POSITION_H		0x1F
#define MOVING_SPEED_L		0x20
#define MOVING_SPEED_H		0x21
#define TORQUE_LIMIT_L		0x22
#define TORQUE_LIMIT_H		0x23
#define PRESENT_POSTION_L	0x24
#define PRESENT_POSTION_H	0x25
#define PRESENT_SPEED_L		0x26
#define PRESENt_SPEED_H		0x27
#define PRESENT_LOAD_L		0x28
#define PRESENT_LOAD_H		0x29
#define PRESENT_VOLTAGE		0x2A
#define PRESENT_TEMPERATURE 0x2B
#define INST_REGISTERD		0x2C
#define MOVING				0x2E
#define EEPROM_LOCK			0x2F
#define PUNCH_L				0x30
#define PUNCH_H				0x31


#endif /* APP_INC_DXLDEF_H_ */
