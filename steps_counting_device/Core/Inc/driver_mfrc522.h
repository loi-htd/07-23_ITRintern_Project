/**
 * @file       driver_mfrc522.h
 * @version    0.1.0
 * @date       2023-08-23
 * @author     Dung Nguyen
 *
 * @brief      Driver MFRC522 Function
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef DRIVER_MFRC522_H
#define DRIVER_MFRC522_H

/* Includes ----------------------------------------------------------- */
#include "bsp_mfrc522.h"
#include "main.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Driver MFRC522 Status
 *
 */
typedef enum
{
  DRV_MFRC522_ERROR,
  DRV_MFRC522_OK
} drv_mfrc522_status_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * The function `drv_mfrc522_init` initializes the MFRC522 RFID reader.
 *
 * @return The function `drv_mfrc522_init` is returning a value of type `drv_mfrc522_status_t`.
 */
drv_mfrc522_status_t drv_mfrc522_init(void);

/**
 * The function `drv_mfrc522_read_id_card` reads the ID card using the MFRC522 RFID module and returns
 * the status of the operation.
 *
 * @param card_id A pointer to an array of uint8_t type, which will store the card ID or serial number.
 *
 * @return a status code of type drv_mfrc522_status_t.
 */
drv_mfrc522_status_t drv_mfrc522_read_id_card(uint8_t *card_id);

#endif /* DRIVER_MFRC522_H */
/* End of file -------------------------------------------------------- */