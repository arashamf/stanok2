
// Includes ------------------------------------------------------------------//
#include "drive.h"
#include "gpio.h"
#include "tim.h"
#include "menu.h"
#include "usart.h"
#include "typedef.h"
#include "button.h"
#include "eeprom.h"

// Functions --------------------------------------------------------------------------------------//
static void init_drives_setting (coil_data_t * , uint8_t);
static void drives_one_full_turn (void);
static void drive1_turn (void);

// Variables --------------------------------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //количество секунд в одном микрошаге(1,8гр/100=6480/100=64,8)
PWM_data_t Drives_PWM = {0};
volatile STATUS_FLAG_DRIVE_t status_drives; 

//-----------------------------------------------------------------------------------------------------//
void init_status_flags_drives (void) 
{
	status_drives.direction 			= 	RIGHT; //двигатели не активны, сдвиг катушки вправо
	status_drives.end_turn_drive1 = 	DRIVE_FREE; //двигатель 1 свободен
	status_drives.end_turn_drive2 = 	DRIVE_FREE; //двигатель 2 свободен
	status_drives.stop_drives			=		START; //разрешение на запуск двигателей
}

//-----------------------------------------------------------------------------------------------------//
static void init_drives_setting (coil_data_t * HandleCoilData, uint8_t number)
{
	uint32_t period_drive2 		=	1000000/HandleCoilData->pulse_frequency; //период импульса верхнего (мотающего) двигателя в мкс
	uint32_t period_drive1 		=	(period_drive2*100)/HandleCoilData->gear_ratio;			//период импульса нижнего (сдвигающего) двигателя
	Drives_PWM.Compare_Drive1	=	(period_drive1/2)-1; 	//длительность импульса нижнего (сдвигающего) двигателя 1
	Drives_PWM.Period_Drive1	=	(period_drive1-1); 	//сохранение периода нижнего (сдвигающего) двигателя 1
	Drives_PWM.Compare_Drive2	=	(period_drive2/2) - 1; //длительность импульса верхнего (мотающего) двигателя 2
	Drives_PWM.Period_Drive2	=	(period_drive2-1); 	//сохранение периода верхнего (мотающего) двигателя 2
	Drives_PWM.turn_number 		= (HandleCoilData->remains_coil[number]); //сохранение количества полных оборотов 
	Drives_PWM.number_cnt_PWM_DR1 = (PULSE_IN_TURN*HandleCoilData->gear_ratio)/100; //отсечка для таймера считающего количество импульсов таймера управляющего нижним двигателем
	
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "ratio=%u,Period=%u,Compare=%u\r\n", HandleCoilData->gear_ratio, Drives_PWM.Period_Drive1, Drives_PWM.Compare_Drive1);
		DBG_PutString(DBG_buffer);
	#endif
}

//---------------------инициализация и запуск вращения полного оборота двигателей---------------------//
static void drives_one_full_turn (void) 
{
	DRIVE2_ENABLE(START);		//разрешение включения двигателя 2
	DIR_DRIVE2 (BACKWARD); //направление вращения двигателя 2
	DRIVE1_ENABLE(START); //разрешение включения двигателя 1
	DIR_DRIVE1 (status_drives.direction); //направление вращения двигателя 1
	delay_us (3);	
	
	Drives_PWM_start (&Drives_PWM); //загрузка настроек в таймеры
}

//-----------------------инициализация и запуск вращения сдвигающего двигателя 1-----------------------//
static void drive1_turn (void) 
{
	DRIVE1_ENABLE(START); //сигнал разрешения запуску двигателя 1
	DIR_DRIVE1 (status_drives.direction); //направление вращения двигателя 1
	delay_us (2);		
	Drive1_PWM_start (&Drives_PWM); //загрузка настроек в таймеры
}

//-----------------------------------------------------------------------------------------------------//
void settings_preset_reset (uint8_t numb_preset, coil_data_t * HandleCoilData)
{
	for (uint8_t count = 0; count < HandleCoilData->set_numb_winding; count++)
	{
		HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count]; //сброс количества витков обмотки
	}
	HandleCoilData->complet_winding = 0; //оставшееся количество обмоток равно 0
	turn_drive_menu (numb_preset, HandleCoilData, HandleCoilData->complet_winding); //вывод меню
}

//-----------------------------------------------------------------------------------------------------//
int8_t start_drives_turn (uint8_t numb_preset, coil_data_t * HandleCoilData)
{
	uint8_t count = 0;
	if(HandleCoilData->complet_winding < HandleCoilData->set_numb_winding) //если количество намотанных обмоток меньше необходимого количества обмоток
	{
		count = HandleCoilData->complet_winding; //текущий номер обмотки
		init_drives_setting (HandleCoilData, count); //расчёт настроек двигателей
		while (HandleCoilData->remains_coil[count] > 0) //пока количество оставшихся витков обмотки больше нуля
		{
			if ((status_drives.end_turn_drive2 == DRIVE_FREE) && (status_drives.end_turn_drive1 == DRIVE_FREE)) 
				// && (status_drives.stop_drives == START)) //проверка флагов (двигателя свободны и не вращаются)
			{
				HandleCoilData->remains_coil[count] = (Drives_PWM.turn_number-1); //сохранение количества оставшихся обмоток				
				status_drives.end_turn_drive2 = DRIVE_BUSY; //двигатель 2 занят (активен)
				status_drives.end_turn_drive1 = DRIVE_BUSY; //двигатель 1 занят (активен)
				drives_one_full_turn(); //запуск вращения на один полный оборот
				
				#ifdef __USE_DBG
					sprintf (DBG_buffer,  "turn_start, numb=%u\r\n", Drives_PWM.turn_number);
					DBG_PutString(DBG_buffer);
				#endif
				
				turn_drive_menu (numb_preset, HandleCoilData, count); //вывод меню			
			}
			if	(scan_button_PEDAL() == OFF) //если педаль была отпущена
			{	
				//status_drives.stop_drives = STOP; //установка флага остановки двигателей	
				return (HandleCoilData->set_numb_winding - HandleCoilData->complet_winding); //возврат количества намотанных обмоток			
			}
		}
		HandleCoilData->complet_winding++; //увелечиние на 1 количества намотанных обмоток			
		while	((status_drives.end_turn_drive2 != DRIVE_FREE) && 
		(status_drives.end_turn_drive1 != DRIVE_FREE)) {} //ожидание завершения оборотов двигателей
		status_drives.stop_drives = STOP; //установка флага остановки двигателей		
		return (HandleCoilData->set_numb_winding - HandleCoilData->complet_winding);	
	}
	return -1;
}

//-----------------------------------------------------------------------------------------------------//
int8_t select_direction(void)
{
	__IO uint16_t key_code = NO_KEY;
	while ((key_code = scan_keys()) == NO_KEY) {} //ожидание нажатия кнопки
	switch (key_code) //обработка кода нажатой кнопки
	{	
			case KEY_MODE_SHORT:
			case KEY_MODE_LONG:
				status_drives.direction = LEFT;
				break;
			
			case KEY_NULL_SHORT:
			case KEY_NULL_LONG:
				status_drives.direction = RIGHT;
				break;
			
			default:
				return ERROR;
				break; 
	}
	return status_drives.direction;
}


//-----------------------------------------------------------------------------------------------------//
void setup_null_position (void)
{		
	while (scan_button_PEDAL() != ON) {} //ожидание нажатия педали
	while ((scan_keys()) == NO_KEY) //пока не нажата никакая кнопка
	{
		if (status_drives.end_turn_drive1 == DRIVE_FREE) //проверка флага двигателя 1
		{
			status_drives.end_turn_drive1 = DRIVE_BUSY; //статус флага двигателя 1 - занят 
			drive1_turn(); //запуск вращения на один полный оборот
			while (scan_button_PEDAL() == ON){} //ожидание отпускания педали
			Drive1_PWM_stop(); //остановка генерации ШИМа
			DRIVE1_ENABLE(STOP); //запрет запуска двигателя 1
			status_drives.end_turn_drive1 = DRIVE_FREE; //статус флаг двигателя 1 - свободен
		}
	}
}

//------------------------------------------------------------------------------------------------------//
void Counter_PWM_Drive1_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "drive1_full_turn_end\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE1_ENABLE(STOP); //выключение двигателя 1
	status_drives.end_turn_drive1 = DRIVE_FREE; //двигатель 1 не активен
}

//------------------------------------------------------------------------------------------------------//
void Counter_PWM_Drive2_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "drive2_full_turn_end\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE2_ENABLE(STOP);  //выключение двигателя 2
	status_drives.end_turn_drive2 = DRIVE_FREE; //двигатель 2 не активен
	if (Drives_PWM.turn_number > 0) //если количество оборотов верхнего двигателя больше нуля
	{	Drives_PWM.turn_number--; } //уменьшение количества оборотов на единицу
}

//-------------------------------------------LL_EXTI_LINE_12-------------------------------------------//
void Left_Sensor_Callback (void)
{
	DRIVE1_ENABLE(STOP);  //выключение двигателя 1
	status_drives.direction = (RIGHT); //левый датчик меняет направление намотки направо
	DIR_DRIVE1 (status_drives.direction);
	DRIVE1_ENABLE(START);  //включение двигателя 1
}

//--------------------------------------------LL_EXTI_LINE_2--------------------------------------------//
void Right_Sensor_Callback (void)
{	
	DRIVE1_ENABLE(STOP); //выключение двигателя 1
	status_drives.direction = (LEFT); //правый датчик меняет направление намотки налево
	DIR_DRIVE1 (status_drives.direction);
	DRIVE1_ENABLE(START); //включение двигателя 1
}
