/**
 * @file       delay_timer.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Huy Le
 * @brief	   Header file for delay_timer module
 * @note	   Delay use timer 1
 */

/* Public includes ---------------------------------------------------------- */
#include "bsp_delay_timer.h"

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
extern TIM_HandleTypeDef htim1;
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
static void delay_tim_1ms(uint16_t time);

/* Function definitions ----------------------------------------------- */
bsp_delay_timer_t bsp_delay_tim_init(void)
{
	if(HAL_TIM_Base_Start(&htim1)!= HAL_OK)
	{
		return BSP_DELAY_TIMER_ERROR;
	}
	return BSP_DELAY_TIMER_OK;
}

void bsp_delay_tim_ms(uint16_t time)
{
	for (int i = 0; i < time; i++)
	{
		delay_tim_1ms(time);
	}
}

/* Private functions ---------------------------------------------------*/
static void delay_tim_1ms(uint16_t time)
{
	__HAL_TIM_SetCounter(&htim1,0); // Reset the timer counter
	while(time--)
	{
		while(__HAL_TIM_GetCounter(&htim1)<20){} // Wait until the timer reaches 20 -> 1ms (20*(1/20kHz))
	}
}
