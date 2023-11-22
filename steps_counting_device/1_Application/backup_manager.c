/**
 * @file       backup-manager.c
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief
 * @note
 */

/* Includes ----------------------------------------------------------- */
#include "backup_manager.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
uint8_t backup_check_status(uint32_t flag_address, uint32_t data_address, uint8_t *card_id)
{
  uint8_t status;
  if ((uint8_t) Flash_Read_Int(flag_address) == 0xFF)
  {
    status = TIME_CONFIG_MODE;
  }
  else if ((uint8_t) Flash_Read_Int(flag_address) == BACKUP_MODE)
  {
    status = BACKUP_MODE;
    Flash_Read_Array(data_address, card_id + 1, 4);  // get user ID from FLASH
  }
  else
    return NORMAL_MODE;
  return status;
}

/* FLASH address 0x080C0010 - mode address
 *    bit 0 = 1 && bit 1 = 1 -> NORMAL MODE (3)
 *    bit 0 = 1 && bit 1 = 0 -> BACKUP MODE (1)
 * FLASH address 0x080C0012 - 0x080C0016 - user ID address
 */

// use this function in the first time scanning user ID
void backup_user_data(uint32_t flag_address, uint32_t data_address, uint8_t *card_id)
{
  Flash_Write_Array(data_address, card_id + 1, 4);  // store 4 bytes user ID into FLASH
  Flash_Write_Int(flag_address, BACKUP_MODE);       // turn on backup mode
}

// use this function in the second time scanning user ID
void backup_reset(uint32_t flag_address)
{
  Flash_Erase(flag_address, 1);                // clear backup data
  Flash_Write_Int(flag_address, NORMAL_MODE);  // turn on normal mode
}
/* End of file -------------------------------------------------------- */
