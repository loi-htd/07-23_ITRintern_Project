/**
 * @file       system_manager.h
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief
 * @note
 * @example
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYSTEM_MANAGER_H
#define __SYSTEM_MANAGER_H
/* Includes ----------------------------------------------------------- */
#include <stdint.h>
#include <stdio.h>
#include "backup_manager.h"
#include "display_manager.h"

/* Public defines ----------------------------------------------------- */
#define BACKUP_FLAG 0x0800C010
#define BACKUP_DATA 0x0800C012
#define TURN_ON 1
#define TURN_OFF 0

/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief Initialize the system components.
 * 
 * This function initializes the display manager and the MPU6050 sensor for the system.
 */
void system_init(void);

/**
 * @brief Check backup status and perform actions accordingly.
 * 
 * This function checks the backup status and performs actions based on the status, such as setting the time or entering backup mode.
 * 
 * @param[in] time        DS1307 time structure.
 * @param[out] id_receive Pointer to store the received card ID.
 * @param[out] pre_card_id Pointer to store the previous card ID.
 * @param[out] delay_flag  Pointer to the delay flag.
 */
void system_check_backup(ds1307_time_t time, uint8_t *id_receive, uint8_t *pre_card_id, uint8_t *delay_flag);

/**
 * @brief Execute the main system function based on the scanning time.
 * 
 * This function executes the main system function based on the scanning time, including handling different cases for card scans.
 * 
 * @param[in, out] step       Pointer to the number of steps value.
 * @param[in, out] time       DS1307 time structure.
 * @param[in]      id_receive Pointer to the received card ID.
 * @param[out]     pre_card_id Pointer to the previous card ID.
 * @param[out]     delay_flag  Pointer to the delay flag.
 */
void system_main_function(uint16_t *step, ds1307_time_t* time, uint8_t *id_receive, uint8_t *pre_card_id, uint8_t *delay_flag);

#endif

/* End of file -------------------------------------------------------- */
