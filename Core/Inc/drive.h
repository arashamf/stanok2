
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
uint8_t read_encoder1_rotation (encoder_data_t * HandleEncData);
//void dr2_rotate_step (uint8_t );
void dr1_one_full_turn (void);
void dr2_one_full_turn (void);
void PWM_continue (void);
void TIM_PWM1_Callback(void);
void turn_coil (coil_data_t * );

// Variables ------------------------------------------------------------------//
extern __IO uint32_t need_step;

#ifdef __cplusplus
}
#endif

#endif 

