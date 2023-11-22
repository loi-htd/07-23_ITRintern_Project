/**
 * @file       system_manager.c
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief
 * @note
 */

/* Includes ----------------------------------------------------------- */
#include "system_manager.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
static mpu6050_t mpu6050;
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
void system_init(void)
{
  if (display_main_function_init() != display_main_function_OK)
  {
    Error_Handler();
  }
  mpu6050_init(&mpu6050, MPU6050_PWR_MNG_INTERNAL, MPU6050_SAMPLERATE_1KHZ, MPU6050_ACCELEROMETER_8G,
               MPU6050_GYROSCOPE_1000S);
}

void system_check_backup(ds1307_time_t time, uint8_t *card_id, uint8_t *pre_card_id, uint8_t *delay_flag)
{
  if (backup_check_status((uint32_t) BACKUP_FLAG, (uint32_t) BACKUP_DATA, card_id) == TIME_CONFIG_MODE)
  {
    time.hour = 18;
    time.minute = 30;
    time.second = 0;
    time.am_pm = DS1307_PM;
    time.format = DS1307_FORMAT_24H;
    time.day = THURSDAY;
    time.date = 7;
    time.month = 9;
    time.year = 23;
    ds1307_set_time(time);
  }
  else if (backup_check_status((uint32_t) BACKUP_FLAG, (uint32_t) BACKUP_DATA, card_id) == BACKUP_MODE)
  {
    user_set_flag();
    *delay_flag = TURN_OFF;
    for (uint8_t i = 1; i < 5; i++)  // store user ID
      pre_card_id[i] = card_id[i];
    display_backup_screen();
    bsp_delay_tim_ms(2000);
  }
}

void system_main_function(uint16_t *step, ds1307_time_t* time, uint8_t *card_id, uint8_t *pre_card_id, uint8_t *delay_flag)
{
  switch (user_check_scanning_time(card_id, pre_card_id))
  {
  case ZERO_SCAN:                                          // Have not been scanned
  {
    display_main_function(ZERO_SCAN, card_id, step, time);  // Default screen
    break;
  }
  case FIRST_SCAN:  // First time Card detected -> start walking
  {
    display_main_function(FIRST_SCAN, pre_card_id, step, time);

    /* Avoid detecting card continuous and backup user ID */
    if (delay_flag == TURN_ON)
    {
      backup_user_data((uint32_t) BACKUP_FLAG, (uint32_t) BACKUP_DATA, pre_card_id);
      bsp_delay_tim_ms(1500);
    }
    *delay_flag = TURN_OFF;  // Turn of delay flag for next cycles

    /* Get sensor data */
    mpu6050_read_accel(&mpu6050);
    mpu6050_read_gyro(&mpu6050);
    /* Execute step counting algorithm*/
    step_start_walking(step, mpu6050);
    break;
  }
  case SECOND_SCAN:                                         // Second time Card detected -> finish walking
  {
    backup_reset((uint32_t) BACKUP_FLAG);                   // Clear backup data
    display_main_function(SECOND_SCAN, card_id, step, time);  // Display finish screen
    user_clear_flag();                                      // Clear FSM flag to default (0)
    *step = 0;                                              // Reset numbers of step
    *delay_flag = TURN_ON;                                  // Turn on delay flag for First time Card detected
    break;
  }
  }
}
/* End of file -------------------------------------------------------- */
