/**
 * @file       bsp_spi.c
 * @version    0.1.0
 * @date       2023-08-23
 * @author     Dung Nguyen
 *
 * @brief      BSP SPI Function
 */

/* Includes ------------------------------------------------------------------*/
#include "bsp_spi.h"
/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
/* SPI1 init function */
spi_status_t bsp_spi_transmit(SPI_HandleTypeDef *hspi, uint8_t *pdata, uint16_t size, uint32_t timeout)
{
  switch (HAL_SPI_Transmit(hspi, pdata, size, timeout))
  {
  case HAL_BUSY: return SPI_STATUS_BUSY; break;
  case HAL_ERROR: return SPI_STATUS_ERROR; break;
  case HAL_TIMEOUT: return SPI_STATUS_TIMEOUT; break;
  case HAL_OK: return SPI_STATUS_OK; break;
  }
  return SPI_STATUS_ERROR;
}

spi_status_t bsp_spi_receive(SPI_HandleTypeDef *hspi, uint8_t *pdata, uint16_t size, uint32_t timeout)
{
  switch (HAL_SPI_Receive(hspi, pdata, size, timeout))
  {
  case HAL_BUSY: return SPI_STATUS_BUSY; break;
  case HAL_ERROR: return SPI_STATUS_ERROR; break;
  case HAL_TIMEOUT: return SPI_STATUS_TIMEOUT; break;
  case HAL_OK: return SPI_STATUS_OK; break;
  }
  return SPI_STATUS_ERROR;
}
/* End of file -------------------------------------------------------- */