
#ifndef __MENU_H__
#define __MENU_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ---------------------------------------------------------------------//
#include "main.h"
#include "typedef.h"
#include "fonts.h"

// Defines ---------------------------------------------------------------------//


// Prototypes ------------------------------------------------------------------//
void setup_menu (encoder_data_t *, coil_data_t * );
void main_menu (void);
void dbg_menu (void);
void turn_drive_menu (coil_data_t * , uint8_t );
// Variables ------------------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

