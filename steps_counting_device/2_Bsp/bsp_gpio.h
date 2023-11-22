/**
 * @file       bsp_gpio.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief      Board Support Package for GPIO
 * @note
 */

#ifndef __BSP_GPIO_H_
#define __BSP_GPIO_H_

/* Includes ----------------------------------------------------------- */
#include "main.h"

#include <stdint.h>
#include <stdlib.h>

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Enumeration for pin states.
 */
typedef enum
{
  BSP_PIN_RESET = 0,
  BSP_PIN_SET
} bsp_pin_state_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief         Write a specified state to a GPIO pin.
 *
 * @param[in]     GPIO    Pointer to the GPIO port.
 * @param[in]     PIN     The GPIO pin number.
 * @param[in]     STATE   The state to be written (bsp_pin_state_t: BSP_PIN_RESET or BSP_PIN_SET).
 */
void bsp_gpio_write_pin(void *GPIO, uint16_t PIN, bsp_pin_state_t STATE);

/**
 * @brief         Read the current state of a GPIO pin.
 *
 * @param[in]     GPIO    Pointer to the GPIO port.
 * @param[in]     PIN     The GPIO pin number.
 *
 * @return        The current state of the GPIO pin (bsp_pin_state_t: BSP_PIN_RESET or BSP_PIN_SET).
 */
bsp_pin_state_t bsp_gpio_read_pin(void *GPIO, uint16_t PIN);

/**
 * @brief         Toggle the state of a GPIO pin.
 *
 * @param[in]     GPIO    Pointer to the GPIO port.
 * @param[in]     PIN     The GPIO pin number to toggle.
 */
void bsp_gpio_toggle_pin(void *GPIO, uint16_t PIN);

#endif
/* End of file -------------------------------------------------------------- */
