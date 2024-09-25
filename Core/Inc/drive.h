
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
void dr1_one_full_turn (void);
void init_drive_turn (coil_data_t * );
uint8_t read_encoder1_rotation (encoder_data_t * );
void encoder_reset (encoder_data_t * );
void TIM_PWM1_Callback(void);

// Variables ------------------------------------------------------------------//
extern __IO uint32_t need_step;

#ifdef __cplusplus
}
#endif

#endif 

