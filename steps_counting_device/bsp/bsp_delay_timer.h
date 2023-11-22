/**
 * @file       delay_timer.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Huy Le
 * @brief	   Header file for delay_timer module
 * @note       Delay use timer 1 
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BSP_DELAY_TIMER_H
#define __BSP_DELAY_TIMER_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------- */
#include "stm32f4xx_hal.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
	BSP_DELAY_TIMER_OK,
	BSP_DELAY_TIMER_ERROR,
}bsp_delay_timer_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief      Initialize the delay timer module.
 * @return     Status of the initialization (BSP_DELAY_TIMER_OK or BSP_DELAY_TIMER_ERROR).
 */
bsp_delay_timer_t bsp_delay_tim_init(void);

/**
 * @brief      Generate a delay in milliseconds using the timer.
 * @param[in]  time: Time in milliseconds.
 * @return     void
 */
void bsp_delay_tim_ms(uint16_t time);

#ifdef _cplusplus
}
#endif
#endif /* __BSP_DELAY_TIMER_H */

