/**
 * @file       bsp_spi.h
 * @version    0.1.0
 * @date       2023-08-23
 * @author     Dung Nguyen
 *             
 * @brief      BSP SPI Function            
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_BSP_SPI_H_
#define INC_BSP_SPI_H_

/* Public macros ------------------------------------------------------ */
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "bsp_gpio.h"

/* Public variables --------------------------------------------------- */
extern SPI_HandleTypeDef hspi1;
/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief SPI Status
 */
typedef enum
{
  SPI_STATUS_OK,
  SPI_STATUS_ERROR,
  SPI_STATUS_BUSY,
  SPI_STATUS_TIMEOUT
} spi_status_t;

/* Public function prototypes ----------------------------------------- */
/**
 * The function spi_transmit transmits data over SPI and returns the status of the transmission.
 *
 * @param hspi A pointer to the SPI handle structure. This structure contains the configuration and
 * state information for the SPI peripheral.
 * @param pdata The pdata parameter is a pointer to the data buffer that contains the data to be
 * transmitted over SPI.
 * @param size The size parameter specifies the number of bytes to be transmitted over SPI.
 * @param timeout The timeout parameter is the maximum amount of time in milliseconds that the function
 * will wait for the transmission to complete before returning a timeout error.
 *
 * @return a value of type spi_status_t.
 */
spi_status_t bsp_spi_transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
 * The function spi_receive receives data over SPI and returns the status of the operation.
 *
 * @param hspi A pointer to the SPI handle structure. This structure contains the configuration and
 * state information for the SPI peripheral.
 * @param pdata The pdata parameter is a pointer to the buffer where the received data will be stored.
 * @param size The size parameter specifies the number of bytes to receive from the SPI device.
 * @param timeout The timeout parameter is the maximum amount of time in milliseconds that the function
 * will wait for the SPI data to be received before returning with a timeout status.
 *
 * @return a value of type spi_status_t.
 */
spi_status_t bsp_spi_receive(SPI_HandleTypeDef *hspi, uint8_t *pdata, uint16_t size, uint32_t timeout);

/* Public macros ------------------------------------------------------ */
#ifdef __cplusplus
}
#endif
#endif /* INC_BSP_SPI_H_ */
/* End of file -------------------------------------------------------- */
