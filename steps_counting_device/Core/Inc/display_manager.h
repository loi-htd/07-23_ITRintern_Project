/**
 * @file       display_manager.h
 * @version    1.0.0
 * @date
 * @author     Huy Le
 * @brief
 * @note
 * @example
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DISPLAY_MANAGER_H_
#define __DISPLAY_MANAGER_H_

/* Includes ----------------------------------------------------------- */
#include <stdint.h>
#include <stdio.h>
#include "bsp_delay_timer.h"
#include "bsp_rtc.h"
#include "driver_mfrc522.h"
#include "driver_ssd1306.h"
#include "driver_ssd1306_fonts.h"
#include "ds1307.h"
#include "step_counting_manager.h"
#include "user_id_manager.h"

/* Public enumerate/structure ----------------------------------------- */
typedef enum 
{
  DISPLAY_MANAGER_ERROR,
  DISPLAY_MANAGER_OK,
} display_manager_status_t;

/* Public macros ------------------------------------------------------ */
/* Public function prototypes ----------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief Initializes the display manager.
 *
 * This function initializes the display manager and any related components.
 *
 * @return The status of the initialization operation (DISPLAY_MANAGER_ERROR or DISPLAY_MANAGER_OK).
 */
display_manager_status_t display_manager_init();

/**
 * @brief Manages the display to show relevant information.
 *
 * This function manages the OLED display to show information based on card ID,
 * step count, and RTC time.
 *
 * @param[in] card_id The card ID to display.
 * @param[in] step The step count to display.
 * @param[in] rtc_time The RTC time to display.
 */
void display_manager(uint8_t scan_case, uint8_t *card_id, uint16_t *step, ds1307_time_t *rtc_time);

/**
 * @brief Display the backup screen.
 * 
 * This function clears the screen and displays a message indicating a backup or restart.
 */
void display_backup_screen(void);
#endif /* _DISPLAY_MANAGER_H_ */
