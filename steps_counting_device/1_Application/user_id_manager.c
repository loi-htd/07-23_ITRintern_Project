/**
 * @file       user_id_manager.c
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief
 * @note
 */

/* Includes ----------------------------------------------------------- */
#include "user_id_manager.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
static uint8_t times = 0;  // FSM variable

/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/**
 * @brief Compare two RFID card IDs.
 *
 * This function compares two RFID card IDs to check if they are the same.
 *
 * @param[in] card_id       Pointer to the current card's ID.
 * @param[in] pre_card_id   Pointer to the previous card's ID.
 *
 * @return Comparison result:
 * - SAME_ID: IDs are the same.
 * - DIFFERENCE_ID: IDs are different.
 */
static uint8_t user_compare_id(uint8_t *card_id, uint8_t *pre_card_id);

/* Function definitions ----------------------------------------------- */
uint8_t user_check_scanning_time(uint8_t *card_id, uint8_t *pre_card_id)
{
  if (drv_mfrc522_read_id_card(card_id) == DRV_MFRC522_OK)
  {
    if (times == ZERO_SCAN)
    {
      for (uint8_t i = 1; i < 5; i++)  // store user ID
        pre_card_id[i] = card_id[i];
      times = FIRST_SCAN;
    }
    else if (times == FIRST_SCAN)
    {
      if (user_compare_id(card_id, pre_card_id) == SAME_ID)
      {
        times = ZERO_SCAN;   // reset FSM to IDLE
        return SECOND_SCAN;  // return SECOND_SCAN state in this cycle.
      }
    }
  }
  return times;
}

static uint8_t user_compare_id(uint8_t *card_id, uint8_t *pre_card_id)
{
  for (uint8_t i = 1; i < 5; i++)  // only compare 4 bytes id
  {
    if (card_id[i] != pre_card_id[i])
      return DIFFERENCE_ID;
  }
  return SAME_ID;
}

void user_set_flag(void)
{
	times = FIRST_SCAN;
}
void user_clear_flag(void)
{
	times = 0;
}
/* End of file -------------------------------------------------------- */
