
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
uint8_t read_encoder1_rotation (encoder_data_t * HandleEncData) ;

// Variables ------------------------------------------------------------------//
extern __IO uint32_t need_step;
#ifdef __cplusplus
}
#endif

#endif 

