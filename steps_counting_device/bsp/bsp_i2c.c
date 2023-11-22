/**
 * @file       bsp_i2c.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief      Board Support Package for i2c
 * @note       Device address has been shifted left
 */

/* Public includes ---------------------------------------------------------- */
#include "bsp_i2c.h"

/* Private includes --------------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------------- */
/* Private macros ----------------------------------------------------------- */
/* Public variables --------------------------------------------------------- */
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;
/* Private variables -------------------------------------------------------- */
/* Private prototypes ------------------------------------------------------- */
/* Public implementations --------------------------------------------------- */

// I2C Init
bsp_i2c_status_t bsp_i2c1_init(void)
{
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t bsp_i2c2_init(void)
{
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t bsp_i2c3_init(void)
{
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

// I2C Device is ready
bsp_i2c_status_t bsp_i2c1_is_device_ready(uint8_t dev_addr, uint32_t trials, uint32_t timeout)
{
  if (HAL_I2C_IsDeviceReady(&hi2c1, dev_addr << 1, trials, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t bsp_i2c2_is_device_ready(uint8_t dev_addr, uint32_t trials, uint32_t timeout)
{
  if (HAL_I2C_IsDeviceReady(&hi2c2, dev_addr << 1, trials, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t bsp_i2c3_is_device_ready(uint8_t dev_addr, uint32_t trials, uint32_t timeout)
{
  if (HAL_I2C_IsDeviceReady(&hi2c3, dev_addr << 1, trials, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

// I2C Master Transmit/Receive
bsp_i2c_status_t bsp_i2c1_master_transmit(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Master_Transmit(&hi2c1, dev_addr << 1, pdata, len, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t bsp_i2c2_master_transmit(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Master_Transmit(&hi2c2, dev_addr << 1, pdata, len, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t bsp_i2c3_master_transmit(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Master_Transmit(&hi2c3, dev_addr << 1, pdata, len, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t bsp_i2c1_master_receive(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Master_Receive(&hi2c1, dev_addr << 1, pdata, len, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t bsp_i2c2_master_receive(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Master_Receive(&hi2c2, dev_addr << 1, pdata, len, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t bsp_i2c3_master_receive(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Master_Receive(&hi2c3, dev_addr << 1, pdata, len, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

// I2C Mem Read/Write
bsp_i2c_status_t
bsp_i2c1_mem_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Mem_Read(&hi2c1, dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, len, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t
bsp_i2c2_mem_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Mem_Read(&hi2c2, dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, len, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t
bsp_i2c3_mem_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Mem_Read(&hi2c3, dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, len, timeout) != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t
bsp_i2c1_write_mem(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Mem_Write(&hi2c1, dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, len, timeout)
      != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t
bsp_i2c2_write_mem(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Mem_Write(&hi2c2, dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, len, timeout)
      != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}

bsp_i2c_status_t
bsp_i2c3_write_mem(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout)
{
  if (HAL_I2C_Mem_Write(&hi2c3, dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, len, timeout)
      != HAL_OK)
    return BSP_I2C_STATUS_ERROR;

  return BSP_I2C_STATUS_OK;
}
/* End of file -------------------------------------------------------- */
