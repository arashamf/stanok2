
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ----------------------------------------------------------------------//
#include "main.h"

// Exported types ---------------------------------------------------------------//
typedef struct 
{
	//���������� ���� ���� ���� � ������� ��/���/���/
	uint16_t 	set_degree; 			//����� �������������� ���� ���� � ��������
	int8_t 		set_minute; 			//����� �������������� ���� ���� � �������
	int8_t 		set_second; 			 //����� �������������� ���� ���� � ��������
	//������� ��������� ���� � ������� ��/���/���/
	uint16_t 	shaft_degree; 		//����� �������� ��������� ���� � ��������
	int8_t 		shaft_minute; 		//����� �������� ���������  ���� � �������
	int8_t 		shaft_second; 		//����� �������� ���������  ���� � ��������
	
	uint32_t 	StepAngleInSec;				//������ ������������� ��� ���� � ��������
	uint32_t 	ShaftAngleInSec; 			//������ ������� ��������� ���� � ��������
	uint32_t 	SetShaftAngleInSec; 	//������������� ��������� ���� � ��������
	float mode1_error;
} angular_data_t;

typedef struct 
{
	int32_t 	prevCounter_SetAngle; 			//���������� ��������� ��������
	int32_t 	currCounter_SetAngle; 			//������� ��������� ��������
	int32_t 	prevCounter_ShaftRotation;  //���������� ��������� ��������
	int32_t 	currCounter_ShaftRotation;	//������� ��������� ��������
	int32_t delta;
//	int8_t 		flag_DirShaftRotation;
} encoder_data_t;

//------------------------------------------------------------------------------------//
typedef struct 
{
	uint8_t 	teeth_gear_numbers; //������������� ���������� ������
	uint8_t 	remain_teeth_gear; //���������� ���������� ������
	uint16_t 	step_shaft_degree; //����� ���� �������� ���� � ������ ���������� � ��������
	int8_t 		step_shaft_minute; //����� ���� �������� ���� � ������ ���������� � �������
	int8_t 		step_shaft_second; //����� ���� �������� ���� � ������ ���������� � ��������
	uint32_t 	AngleTeethInSec; //���� �������� � ������ ���������� � ��������
	float milling_error;
} milling_data_t;

//----------------------------------------------------------------------------------//
typedef enum 
{
	MODE_DEFAULT 				= 0,				//����� - �� ���������
	MODE_MILLING										//����� - ����������
} MACHINE_MODE_t; 	

//----------------------------------------------------------------------------------//
typedef union
{
	struct
	{
		uint8_t tool_mode			: 1;
		uint8_t left_flag			: 1;
		uint8_t right_flag		: 1;
		uint8_t raw						:	5;
	};
	uint8_t flag;
}STATUS_FLAG_t;

//��� ������� ������----------------------------------------------------------------//
typedef enum 
{
	NO_KEY 						= 	0x00,			//������ �� ������	
	KEY_LEFT 					= 	0x01,			//����� ������
	KEY_CENTER_SHORT 	= 	0x02,			//�������� ������� ����������� ������
	KEY_CENTER_LONG 	= 	0x03,			//������� ������� ����������� ������
	KEY_RIGHT					= 	0x04, 		//������ ������
	KEY_ENC_SHORT			= 	0x05,			//�������� ������� ������ ��������
	KEY_ENC_LONG			=		0x06,			//������� ������� ������ ��������
	KEY_MODE_SHORT		=		0x07,			//�������� ������� ������ ������
	KEY_MODE_LONG			=		0x08,			//�������� ������� ������ ������
} KEY_CODE_t; 					

//-----------------------------------------------------------------------------------//
typedef enum 
{
	KEY_STATE_OFF 				= 0	,			//����� - ������ �� ������
	KEY_STATE_ON							,				//����� - ������ ������
	KEY_STATE_BOUNCE					, 			//����� -  ������� ������
	KEY_STATE_AUTOREPEAT			,	 			//����� - ����� �������� (�����������) ������� ������
	KEY_STATE_WAIT_TURNOFF
} KEY_STATE_t; 									//������ ������������ ����������

//-----------------------------------------------------------------------------------//
struct KEY_MACHINE_t
{
	KEY_CODE_t 		key_code;
	KEY_STATE_t 	key_state;
};

// Defines ----------------------------------------------------------------------//
#define   REDUCER 						40 //�������� ���������
#define 	STEPS_IN_REV				20000 	//���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� ��������
#define 	CIRCLE_IN_STEP			200		 //���������� ����� (1,8��) � ����� ������ ������� (360 ��)
#define 	STEP_DIV 						(STEPS_IN_REV/CIRCLE_IN_STEP)		//���������� ���������� (100) � ����� ���� ��������� (1,8��)
#define 	STEP_TOOL						(STEPS_IN_REV*REDUCER) //���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� �������� � ��������� 

#define 	STEP18_IN_SEC					6480 //���������� ������ � ����� ���� ��������� (1,8��)
#define 	CIRCLE_IN_SEC					(STEP18_IN_SEC*CIRCLE_IN_STEP)	//���������� ������ � ����� ������ ������� ��������� (360 ��)
#define 	SECOND_PER_MINUTE 		60
#define 	SECOND_PER_DEGREE 		3600
// Private variables -----------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

