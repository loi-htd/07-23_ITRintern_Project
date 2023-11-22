/**
 * @file       backup-manager.h
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief
 * @note
 * @example
 * main()
 * {
 *  init();
 *  backup_check_status();
 *
 *  while(1)
 * {
 *    read_user_id();
 *    backup_user_data();
 * }
 * }
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BACKUP_MANAGER_H
#define __BACKUP_MANAGER_H
/* Includes ----------------------------------------------------------- */
#include "bsp_flash.h"
#include <stdint.h>
#include <stdio.h>

/* Public defines ----------------------------------------------------- */
#define TIME_CONFIG_MODE 0
#define BACKUP_MODE 1
#define NORMAL_MODE 3
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief Check the backup status and retrieve data.
 *
 * This function checks the backup status and retrieves data if available.
 *
 * @param[in] flag_address   Address of the flag in memory.
 * @param[in] data_address   Address of the data in memory.
 * @param[out] card_id       Pointer to store the retrieved card ID.
 *
 * @return The status of the backup operation:
 * - TIME_CONFIG_MODE: Backup is not set; the system is in time configuration mode.
 * - BACKUP_MODE: Backup mode is set, and data is retrieved.
 * - NORMAL_MODE: No backup or normal operation mode.
 */
uint8_t backup_check_status(uint32_t flag_address, uint32_t data_address, uint8_t *card_id);

/**
 * @brief Backup user data.
 *
 * This function backs up user data by writing it to a specified memory address.
 *
 * @param[in] flag_address   Address of the flag in memory.
 * @param[in] data_address   Address to store the backup data in memory.
 * @param[in] card_id        Pointer to the user's card ID data.
 */
void backup_reset(uint32_t flag_address);

/**
 * @brief Reset the backup mode.
 *
 * This function resets the backup mode and clears any stored backup data.
 *
 * @param[in] flag_address   Address of the flag in memory.
 */
void backup_user_data(uint32_t flag_address, uint32_t data_address, uint8_t *card_id);
#endif

/* End of file -------------------------------------------------------- */
