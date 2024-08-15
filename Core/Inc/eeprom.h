
#ifndef __EEPROM_H__
#define __EEPROM_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ---------------------------------------------------------------------//
#include "main.h"
#include "typedef.h"

// Defines ---------------------------------------------------------------------//
#define 	EEPROM_MEMORY_PAGE 	0x0001

// Prototypes -----------------------------------------------------------------//
void SaveData_In_EEPROM (encoder_data_t * );
encoder_data_t ReadData_From_EEPROM (void);

// Variables ------------------------------------------------------------------//
//extern uint8_t eeprom_rx_buffer[];
//extern uint8_t eeprom_tx_buffer[];
#ifdef __cplusplus
}
#endif

#endif 

