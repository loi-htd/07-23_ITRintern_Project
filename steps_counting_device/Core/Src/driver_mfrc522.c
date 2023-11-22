/**
 * @file       driver_mfrc522.c
 * @version    0.1.0
 * @date       2023-08-23
 * @author     Dung Nguyen
 *
 * @brief      Driver MFRC522 Function
 */

/* Includes ----------------------------------------------------------- */
#include "driver_mfrc522.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
drv_mfrc522_status_t drv_mfrc522_init(void)
{
  return bsp_mfrc522_init();
}

drv_mfrc522_status_t drv_mfrc522_read_id_card(uint8_t *card_id)
{
  bsp_mfrc522_status_t status;
  /* Find cards, return card type */
  status = bsp_mfrc522_request(BSP_PICC_REQIDL, card_id);
  if (status == BSP_MFRC522_OK)
  {
    /* Card detected */
    /* Anti-collision, return card serial number 4 bytes */
    status = bsp_mfrc522_anticoll(card_id);
  }
  bsp_mfrc522_haft(); /* Command card into hibernation */
  return status;
}
/* End of file -------------------------------------------------------- */