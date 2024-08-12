
/* Includes ------------------------------------------------------------------*/
#include "ssd1306_interface.h"
#include "spi.h"
#include "gpio.h"

/* Private define ------------------------------------------------------------*/
#define SSD1306_SPI_TIMEOUT                                	0xFF

/* Private typedef -----------------------------------------------------------*/
extern SSD1306_State SSD1306_state;

/* Private variables ---------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void ssd1306_SendCommand(uint8_t type_command)
{
//	CS(ON);
	LCD_DC(OFF); //передача команды
	spi_write_byte (type_command);
//	CS(OFF);
}

/*----------------------------------------------------------------------------*/
void ssd1306_SendByteData(uint8_t data)
{
//	CS(ON);
	LCD_DC(ON); //передача данных
	spi_write_byte (data);
//	CS(OFF);
}

/*----------------------------------------------------------------------------*/
void ssd1306_SendDataBuffer(uint8_t *data, uint16_t data_size)
{
	CS(ON);
	LCD_DC(ON); //передача данных
	spi_write_buffer (data, data_size);
	CS(OFF);
}

/*----------------------------------------------------------------------------*/
