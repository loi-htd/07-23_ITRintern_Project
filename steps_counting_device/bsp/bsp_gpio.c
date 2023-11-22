/**
 * @file       bsp_gpio.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief      Board Support Package for GPIO
 * @note
 */

/* Public includes ---------------------------------------------------------- */
#include "bsp_gpio.h"

/* Private includes --------------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------------- */
/* Private macros ----------------------------------------------------------- */
/* Public variables --------------------------------------------------------- */
/* Private variables -------------------------------------------------------- */
/* Private prototypes ------------------------------------------------------- */
/* Public implementations --------------------------------------------------- */
void bsp_gpio_write_pin(void *GPIO, uint16_t PIN, bsp_pin_state_t STATE)
{
  HAL_GPIO_WritePin(GPIO, PIN, STATE);
}

bsp_pin_state_t bsp_gpio_read_pin(void *GPIO, uint16_t PIN)
{
  bsp_pin_state_t state;
  state = HAL_GPIO_ReadPin(GPIO, PIN);
  return state;
}

void bsp_gpio_toggle_pin(void *GPIO, uint16_t PIN)
{
  HAL_GPIO_TogglePin(GPIO, PIN);
}

/* Private implementations -------------------------------------------------- */

/* End of file -------------------------------------------------------------- */