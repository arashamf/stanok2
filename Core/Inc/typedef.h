
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes -------------------------------------------------------------------------//
#include "main.h"

// Exported types ------------------------------------------------------------------//

//----------------------------------------------------------------------------------//
typedef struct 
{
	int32_t 	prevCounter_SetClick; 			//сохранённое показание энкодера
	int32_t 	currCounter_SetClick; 			//текущее показание энкодера
	int32_t delta;
} encoder_data_t;

//----------------------------------------------------------------------------------//
#define 	MAX_NUMBER_COIL 					3
#pragma pack(1) 
typedef struct 
{
	union
	{
		struct 
		{
			uint8_t number_coil;
			uint16_t set_coil[MAX_NUMBER_COIL]; 		
			uint16_t remains_coil[MAX_NUMBER_COIL]; 		
		};
		uint8_t coil_buffer[MAX_NUMBER_COIL*MAX_NUMBER_COIL*2+1];
	};
	struct coil_data_t *next;
} coil_data_t;

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

//----------------------------------------------------------------------------------//
typedef struct 
{
	uint16_t Compare_Drive1;
	uint16_t Compare_Drive2; //значение сравнения двигателя 2
	uint16_t value_Period; //период импульса
	uint16_t turn_number; //количество совершённых оборотов
	uint8_t count_start_PWM_TIM; //количество итераций в одном обороте
} PWM_data_t;

#pragma pack()

//код нажатой кнопки----------------------------------------------------------------//
typedef enum 
{
	NO_KEY 						= 	0x00,			//кнопка не нажата	
	KEY_PEDAL_SHORT 	= 	0x01,			//короткое нажатие центральной кнопки
	KEY_PEDAL_LONG 	= 		0x02,			//длинное нажатие центральной кнопки
	KEY_ENC_SHORT			= 	0x03,			//короткое нажатие кнопки энкодера
	KEY_ENC_LONG			=		0x04,			//длинное нажатие кнопки энкодера
	KEY_NULL_SHORT		=		0x05,			//короткое нажатие выбора режима
	KEY_NULL_LONG			=		0x06,			//короткое нажатие выбора режима
} KEY_CODE_t; 					

//-----------------------------------------------------------------------------------//
typedef enum 
{
	KEY_STATE_OFF 				= 0	,				//режим - кнопка не нажата
	KEY_STATE_ON							,				//режим - кнопка нажата
	KEY_STATE_BOUNCE					, 			//режим -  дребезг кнопки
	KEY_STATE_AUTOREPEAT			,	 			//режим - режим ожидания (автоповтора) отжатия кнопки
	KEY_STATE_WAIT_TURNOFF
} KEY_STATE_t; 									//статус сканирования клавиатуры

//-----------------------------------------------------------------------------------//
struct KEY_MACHINE_t
{
	KEY_CODE_t 		key_code;
	KEY_STATE_t 	key_state;
};

// Defines ----------------------------------------------------------------------//
#define 	I2C_REQUEST_WRITE                       0x00
#define 	I2C_REQUEST_READ                        0x01
#define 	SLAVE_OWN_ADDRESS                       0xA0 //адресс EEPROM микросхемы

#define   REDUCER 						40 			//делитель редуктора
#define 	PULSE_IN_TURN				1600 		//количество микрошагов в одном полном обороте (360 гр) с учётом делителя драйвера
#define 	PULSE_IN_TIM_RCR		100
#define 	STEP_IN_TURN				200		 //количество шагов (1,8гр) в одном полном обороте (360 гр)
#define 	STEP_DIV 						(PULSE_IN_TURN/STEP_IN_TURN)		//количество микрошагов (8) в одном шаге двигателя (1,8гр)
#define 	STEP_TURNOVER				(PULSE_IN_TURN*REDUCER) //количество микрошагов в одном полном обороте (360 гр) с учётом делителя драйвера и редуктора 

#define 	STEP18_IN_SEC					6480 							//количество секунд в одном шаге двигателя (1,8гр)
#define 	CIRCLE_IN_SEC					(STEP18_IN_SEC*CIRCLE_IN_STEP)	//количество секунд в одном полном обороте двигателя (360 гр)
#define 	SECOND_PER_MINUTE 		60
#define 	SECOND_PER_DEGREE 		3600

#define 	ON 												1
#define 	OFF 											0
#define 	FORWARD 									1
#define 	BACKWARD 									0
#define 	LEFT 											1
#define 	RIGHT 										0
#define 	DISP_CLEAR 								1
#define 	DISP_NOT_CLEAR 						0
//#define 	EEPROM_NUMBER_BYTES 			4
#define 	CPU_CLOCK_VALUE						(72000000UL)		//частота контроллера 
#define 	TICKS_PER_SECOND					1000 
// Private variables -----------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

