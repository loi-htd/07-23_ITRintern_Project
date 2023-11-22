/**
 * @file       step_counting_manager.c
 * @date
 * @author     loi ho
 * @author     dung nguyen
 * @brief
 * @note
 */

/* Includes ----------------------------------------------------------- */
#include "step_counting_manager.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
static step_data_t data, pre_data;
static step_counting_state_t step_status;
static uint16_t timestamp;
static float accel_threshold;

/* Private function prototypes ---------------------------------------- */
/**
 * @brief Calculate the threshold for step detection.
 *
 * This function calculates the threshold for step detection based on accelerometer data.
 *
 * @param[in] data      Current accelerometer data.
 * @param[in] pre_data  Previous accelerometer data.
 *
 * @return The calculated threshold value for step detection.
 */
static float step_calc_threshold(step_data_t *data, step_data_t *pre_data);

/**
 * @brief Count the number of steps based on accelerometer data.
 *
 * This function counts the number of steps based on accelerometer data and a threshold.
 *
 * @param[in, out] step_value  Pointer to the number of steps value.
 * @param[in] magnitude        Current magnitude of accelerometer data.
 * @param[in] threshold        Threshold for step detection.
 */
static void step_counting(uint16_t *step_value, float magnitude, float threshold);

/* Function definitions ----------------------------------------------- */
void step_start_walking(uint16_t *step_value, mpu6050_t mpu6050)
{
  // calculate the magnitude of the 3-D acceleration data
  data.magnitude = sqrt(mpu6050.accelerometer_x * mpu6050.accelerometer_x
                     + mpu6050.accelerometer_y * mpu6050.accelerometer_y
                     + mpu6050.accelerometer_z * mpu6050.accelerometer_z);
  // calculate threshold
  accel_threshold = step_calc_threshold(&data, &pre_data);
  // counting step
  step_counting(step_value, data.magnitude, accel_threshold);
}

/* Private function definitions ---------------------------------------- */
static float step_calc_threshold(step_data_t *data, step_data_t *pre_data)
{
  float gt = 0.5;  // threshold coefficient
  float gp;        // gain coefficient
  float threshold;

  // determines the loop gain and a smaller coefficient corresponds to a slower response
  if (data->magnitude > pre_data->accel_peek)
    gp = 0.5;
  else
    gp = (float) (1.0 / 16.0);

  /* calculate accel threshold base on transfer function
   *          accel peak        gp * z^(-1)
   * Hp(z) = ------------ = -------------------
   *           magnitude     1 - (1-gp) * z^(-1)
   * accel threshold = gt * accel peak
   */
  data->accel_peek = (pre_data->magnitude - pre_data->accel_peek) * gp + pre_data->accel_peek;
  threshold = data->accel_peek * gt;

  // assign data again for next loop
  pre_data->accel_peek = data->accel_peek;
  pre_data->magnitude = data->magnitude;

  return threshold;
}

static void step_counting(uint16_t *step_value, float magnitude, float threshold)
{
  uint16_t number_of_steps = *step_value;

  switch (step_status)
  {
  case STEP_WAITING:
    if (magnitude > threshold)
    {
      step_status = STEP_DETECTED;
    }
    break;

  case STEP_DETECTED:
    if (magnitude <= threshold)
    {
      number_of_steps++;
      timestamp = HAL_GetTick();  // get timestamp
      step_status = STEP_NOISE_REDUCTION;
    }
    break;

  default:
    if ((HAL_GetTick() - timestamp) > NOISE_TIME)
      step_status = STEP_WAITING;
    break;
  }
  *step_value = number_of_steps;
}

/* End of file -------------------------------------------------------- */
