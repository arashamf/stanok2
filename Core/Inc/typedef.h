
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes -------------------------------------------------------------------------//
#include "main.h"

// Defines ----------------------------------------------------------------------//
#define 	I2C_REQUEST_WRITE                       0x00
#define 	I2C_REQUEST_READ                        0x01
#define 	SLAVE_OWN_ADDRESS                       0xA0 //������ EEPROM ����������

#define 	PULSE_IN_TURN				1600 		//���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� ��������
#define 	PULSE_IN_TIM_RCR		100
#define 	STEP_IN_TURN				200		 //���������� ����� (1,8��) � ����� ������ ������� (360 ��)
#define 	STEP_DIV 						(PULSE_IN_TURN/STEP_IN_TURN)		//���������� ���������� (8) � ����� ���� ��������� (1,8��)

#define 	BASE_TURN_IN_MINUTE			30
#define   STEP_TURN_IN_MINUTE			3
#define 	MAX_VALUE_TURN 					150
#define 	MIN_VALUE_TURN 					12
#define 	BASE_PULSE_DR2					(PULSE_IN_TURN*BASE_TURN_IN_MINUTE)/60 	//800 ��

#define 	MAX_VALUE_RATIO				400 
#define 	MIN_VALUE_RATIO				25 

#define 	STEP18_IN_SEC					6480 							//���������� ������ � ����� ���� ��������� (1,8��)
#define 	CIRCLE_IN_SEC					(STEP18_IN_SEC*CIRCLE_IN_STEP)	//���������� ������ � ����� ������ ������� ��������� (360 ��)
#define 	SECOND_PER_MINUTE 		60
#define 	SECOND_PER_DEGREE 		3600

#define 	ON												1
#define 	OFF 											0
#define 	START											1
#define 	STOP 											0
#define 	FORWARD 									1
#define 	BACKWARD 									0
#define 	LEFT 											1
#define 	RIGHT 										0
#define 	DISP_CLEAR 								1
#define 	DISP_NOT_CLEAR 						0
#define 	DRIVE_FREE								1
#define 	DRIVE_BUSY								0
//#define 	EEPROM_NUMBER_BYTES 		4
#define 	CPU_CLOCK_VALUE						(72000000UL)		//������� ����������� 
#define 	MS_PER_SECOND							1000 
#define 	START_DELAY								0x64
#define 	MAX_NUMBER_COIL 					3
#define 	MAX_PRESET 								4

// Exported types ------------------------------------------------------------------//
//----------------------------------------------------------------------------------//
typedef struct 
{
	int32_t 	prevCounter_SetClick; 			//���������� ��������� ��������
	int32_t 	currCounter_SetClick; 			//������� ��������� ��������
	int32_t delta;
} encoder_data_t;

//----------------------------------------------------------------------------------//
#pragma pack(1) 
typedef struct 
{
	union
	{
		struct 
		{
			uint8_t 	set_numb_winding; //������������� ���������� �������
			uint8_t 	complet_winding; //���������� ���������� �������
			uint16_t	pulse_frequency; //���������� ��������� � �������
			uint16_t  drive2_turn_in_minute; //���������� �������� � ������
			uint16_t 	gear_ratio; //������������ �����������
			uint16_t 	set_coil[MAX_NUMBER_COIL]; //������������� ���������� ������ �������
			uint16_t 	remains_coil[MAX_NUMBER_COIL]; //���������� ���������� ������ �������		
		};
		uint8_t coil_buffer[MAX_NUMBER_COIL*2*2+5];
	};
	struct coil_data_t *next;
} coil_data_t;

//----------------------------------------------------------------------------------//
typedef union
{
	struct
	{
		uint8_t direction							: 1;
		uint8_t end_turn_drive1				: 1;
		uint8_t end_turn_drive2				: 1;
		uint8_t stop_drives						:	1;
		uint8_t reserve								:	4;
	};
	uint8_t flag;
}STATUS_FLAG_DRIVE_t;

//----------------------------------------------------------------------------------//
typedef struct 
{
	uint16_t Compare_Drive1; //�������� ��������� ��������� 1 (������������ �����������)
	uint16_t Compare_Drive2; //�������� ��������� ��������� 2
	uint16_t Period_Drive1; //������ �������� ��� ��������� 1
	uint16_t Period_Drive2; //������ �������� ��� ��������� 2
	uint16_t number_cnt_PWM_DR1; //������� ��������� ��������� 1
	uint16_t turn_number; //���������� ����������� ��������
} PWM_data_t;

#pragma pack()

//��� ������� ������----------------------------------------------------------------//
typedef enum 
{
	NO_KEY 						= 	0x00,			//������ �� ������	
	KEY_PEDAL_SHORT 	= 	0x01,			//�������� ������� ����������� ������
	KEY_PEDAL_LONG 		= 	0x02,			//������� ������� ����������� ������
	KEY_NULL_SHORT		= 	0x03,			//�������� ������� ������ ��������
	KEY_NULL_LONG			=		0x04,			//������� ������� ������ ��������
	KEY_MODE_SHORT		=		0x05,			//�������� ������� ������ ������
	KEY_MODE_LONG			=		0x06,			//�������� ������� ������ ������
} KEY_CODE_t; 					

//-----------------------------------------------------------------------------------//
typedef enum 
{
	KEY_STATE_OFF 				= 0	,				//����� - ������ �� ������
	KEY_STATE_ON							,				//����� - ������ ������
	KEY_STATE_BOUNCE					, 			//����� -  ������� ������
	KEY_STATE_AUTOREPEAT			,	 			//����� - ����� �������� (�����������) ������� ������
	KEY_STATE_WAIT_TURNOFF
} KEY_STATE_t; 									//������ ������������ ����������

//-----------------------------------------------------------------------------------//
typedef enum 
{
	NO_PRESET          	  = 0 ,
	PRESET1 							= 1	,				
	PRESET2										,				
	PRESET3										, 			
	PRESET4										,	 			
} PRESET_t; 									

//-----------------------------------------------------------------------------------//
struct KEY_MACHINE_t
{
	KEY_CODE_t 		key_code;
	KEY_STATE_t 	key_state;
};

// Private variables -----------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

