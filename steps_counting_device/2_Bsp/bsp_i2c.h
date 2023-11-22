/**
 * @file       bsp_i2c.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief      Board Support Package for i2c
 * @note
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_I2C_H_
#define BSP_I2C_H_

/* Includes ----------------------------------------------------------- */
#include "main.h"

#include <stdint.h>
#include <stdlib.h>

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  BSP_I2C_STATUS_OK,
  BSP_I2C_STATUS_ERROR,
} bsp_i2c_status_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */

/**
 * @brief         Initialize I2C1
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c1_init(void);

/**
 * @brief         Initialize I2C2
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c2_init(void);

/**
 * @brief         Initialize I2C3
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c3_init(void);

/**
 * @brief         Perform I2C1 check device is ready or not
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     trials         Number to try send
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c1_is_device_ready(uint8_t dev_addr, uint32_t trials, uint32_t timeout);

/**
 * @brief         Perform I2C2 check device is ready or not
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     trials         Number to try send
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c2_is_device_ready(uint8_t dev_addr, uint32_t trials, uint32_t timeout);

/**
 * @brief         Perform I2C3 check device is ready or not
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     trials         Number to try send
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c3_is_device_ready(uint8_t dev_addr, uint32_t trials, uint32_t timeout);
/**
 * @brief         Perform I2C1 master transmit
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     pdata          Pointer to data to transmit
 * @param[in]     len            Number of bytes to transmit
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c1_master_transmit(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C2 master transmit
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     pdata          Pointer to data to transmit
 * @param[in]     len            Number of bytes to transmit
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c2_master_transmit(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C3 master transmit
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     pdata          Pointer to data to transmit
 * @param[in]     len            Number of bytes to transmit
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c3_master_transmit(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C1 master receive
 *
 * @param[in]     dev_addr       Device address
 * @param[out]    pdata          Pointer to store received data
 * @param[in]     len            Number of bytes to receive
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c1_master_receive(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C2 master receive
 *
 * @param[in]     dev_addr       Device address
 * @param[out]    pdata          Pointer to store received data
 * @param[in]     len            Number of bytes to receive
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c2_master_receive(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C3 master receive
 *
 * @param[in]     dev_addr       Device address
 * @param[out]    pdata          Pointer to store received data
 * @param[in]     len            Number of bytes to receive
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t bsp_i2c3_master_receive(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C1 memory read
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     reg_addr       Register address
 * @param[out]    p_data         Pointer to store the read data
 * @param[in]     len            Number of bytes to read
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t
bsp_i2c1_mem_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C2 memory read
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     reg_addr       Register address
 * @param[out]    p_data         Pointer to store the read data
 * @param[in]     len            Number of bytes to read
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t
bsp_i2c2_mem_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C3 memory read
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     reg_addr       Register address
 * @param[out]    p_data         Pointer to store the read data
 * @param[in]     len            Number of bytes to read
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t
bsp_i2c3_mem_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C1 memory write
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     reg_addr       Register address
 * @param[in]     p_data         Pointer to data to be written
 * @param[in]     len            Number of bytes to write
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t
bsp_i2c1_write_mem(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C2 memory write
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     reg_addr       Register address
 * @param[in]     p_data         Pointer to data to be written
 * @param[in]     len            Number of bytes to write
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t
bsp_i2c2_write_mem(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout);

/**
 * @brief         Perform I2C3 memory write
 *
 * @param[in]     dev_addr       Device address
 * @param[in]     reg_addr       Register address
 * @param[in]     p_data         Pointer to data to be written
 * @param[in]     len            Number of bytes to write
 * @param[in]     timeout        Timeout in milliseconds
 *
 * @return        bsp_i2c_status_t: BSP_I2C_STATUS_OK if successful, BSP_I2C_STATUS_ERROR on failure
 */
bsp_i2c_status_t
bsp_i2c3_write_mem(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout);
#endif
/* End of file -------------------------------------------------------- */