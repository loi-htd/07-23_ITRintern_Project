/**
 * @file       step_counting_manager.h
 * @date
 * @author     loi ho
 * @author     dung nguyen
 * @brief
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __STEP_COUNTING_MANAGER_H
#define __STEP_COUNTING_MANAGER_H

/* Includes ----------------------------------------------------------- */
#include "driver_mpu6050.h"
#include <math.h>
#include <stdint.h>

/* Public defines ----------------------------------------------------- */
#define NOISE_TIME 300
/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Enumeration for step counting states.
 */
typedef enum step_counting_state_e
{
  STEP_WAITING = 0,
  STEP_DETECTED,
  STEP_NOISE_REDUCTION
} step_counting_state_t;

/**
 * @brief Structure for storing step-related data.
 */
typedef struct data_s
{
  float magnitude;  // the magnitude of the 3-D acceleration data
  float accel_peek; // the peak signal
} step_data_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief Start tracking walking steps based on accelerometer data.
 *
 * This function starts tracking walking steps based on accelerometer data from an MPU6050 sensor.
 *
 * @param[in, out] step_value  Pointer to the number of steps value.
 * @param[in]      mpu6050     Structure containing accelerometer data from the MPU6050 sensor.
 */
void step_start_walking(uint16_t *step_value, mpu6050_t mpu6050);

#endif  // __CODE_TEMPLATE_H

/* End of file -------------------------------------------------------- */
