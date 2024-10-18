
#ifndef __DRIVE_H__
#define __DRIVE_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes ---------------------------------------------------------------------//
#include "main.h"
#include "typedef.h"

// Defines ---------------------------------------------------------------------//

// Prototypes ------------------------------------------------------------------//
void init_status_flags_drives (void);
void start_drives_turn (uint8_t , coil_data_t * );
void Counter_PWM_Drive1_Callback (void);
void Counter_PWM_Drive2_Callback (void);
void Left_Sensor_Callback (void);
void Right_Sensor_Callback (void);

// Variables ------------------------------------------------------------------//
extern __IO uint32_t need_step;

#ifdef __cplusplus
}
#endif

#endif 

