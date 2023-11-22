/**
 * @file       user_id_manager.h
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief
 * @note
 * @example
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __USER_ID_MANAGER_H
#define __USER_ID_MANAGER_H
/* Includes ----------------------------------------------------------- */
#include "driver_mfrc522.h"

#include <stdint.h>
#include <stdio.h>


/* Public defines ----------------------------------------------------- */
#define ZERO_SCAN 0
#define FIRST_SCAN 1
#define SECOND_SCAN 2
#define SAME_ID 1
#define DIFFERENCE_ID 2
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief Check scanning time for an RFID card.
 *
 * This function checks the scanning time for an RFID card and returns the corresponding status.
 *
 * @param[in, out] card_id   Pointer to the card's ID.
 *
 * @return The scanning time status:
 * - ZERO_SCAN: Initial scan.
 * - FIRST_SCAN: First scan.
 * - SECOND_SCAN: Second scan with the same ID as the first.
 */
uint8_t user_check_scanning_time(uint8_t *card_id, uint8_t *pre_card_id);

/**
 * @brief Clear user-defined flags or variables.
 *
 * This function clears user-defined flags or variables, such as the 'times' variable, to reset or initialize
 * them.
 */
void user_clear_flag(void);

/**
 * @brief Set a user-defined flag or variable.
 * 
 * This function sets a user-defined flag or variable, such as the 'times' variable, to a specific value.
 */
void user_set_flag(void)
#endif

  /* End of file -------------------------------------------------------- */
