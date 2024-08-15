
// Includes --------------------------------------------------------------------//
#include "eeprom.h"
#include "i2c.h"
#include "angle_calc.h"
#include "typedef.h"
//#include "ssd1306.h"
#include "usart.h"
// Defines ---------------------------------------------------------------------//
#define 	EEPROM_MEMORY_PAGE 	0x0001                                    

// Private typedef ------------------------------------------------------------//

// Private variables ---------------------------------------------------------//
uint8_t EEPROM_TX_Buf[EEPROM_NUMBER_BYTES+1] = {0} ;
uint8_t EEPROM_RX_Buf[EEPROM_NUMBER_BYTES+1] = {0} ;

// Functions -------------------------------------------------------------------//
static void EEPROM_WriteBytes	(uint16_t, uint8_t*, uint16_t);
static void EEPROM_ReadBytes	(uint16_t, uint8_t*, uint16_t); 

//--------------------------------------------------------------------------------------//
static void EEPROM_WriteBuffer (uint16_t registr, uint8_t *buf, uint16_t bytes_count)
{
	I2C_WriteBuffer (registr, buf, bytes_count);
}

//--------------------------------------------------------------------------------------//
static void EEPROM_ReadBuffer (uint16_t registr, uint8_t *buf, uint16_t bytes_count)
{  
	I2C_ReadBuffer (registr, buf, bytes_count);
}

//--------------------------------------------------------------------------------------//
void SaveData_In_EEPROM (encoder_data_t * handle_Data)
{
	uint8_t count = sizeof(handle_Data->currCounter_SetAngle);
	for (uint8_t i = 0; i < count; i++)
	{
		EEPROM_TX_Buf[i] = (uint8_t)(handle_Data->prevCounter_SetAngle >> (8*i));
	}	
	
	I2C_WriteBuffer (EEPROM_MEMORY_PAGE, EEPROM_TX_Buf, count);

	#ifdef __USE_DBG
	sprintf ((char *)DBG_buffer,  "WRITE:%d %d\r\n", handle_Data->currCounter_SetAngle, handle_Data->prevCounter_SetAngle);
	DBG_PutString(DBG_buffer);
	#endif	
}

//--------------------------------------------------------------------------------------//
encoder_data_t ReadData_From_EEPROM (void)
{
	encoder_data_t  Enc_Data = {0};
	encoder_data_t * handle_Enc = &Enc_Data;
	uint8_t count = sizeof(Enc_Data.currCounter_SetAngle);
	
	I2C_ReadBuffer (EEPROM_MEMORY_PAGE, EEPROM_RX_Buf, count);
	for (uint8_t i = 0; i < count; i++)
	{
		Enc_Data.currCounter_SetAngle |= (((uint32_t)*(EEPROM_RX_Buf+i)) << (8*i));
	}	
	Enc_Data.prevCounter_SetAngle = Enc_Data.currCounter_SetAngle;
	
	#ifdef __USE_DBG
	sprintf ((char *)DBG_buffer,  "READ:%d %d\r\n",Enc_Data.currCounter_SetAngle, Enc_Data.prevCounter_SetAngle);
	DBG_PutString(DBG_buffer);
	#endif	
	
	return Enc_Data;
}

