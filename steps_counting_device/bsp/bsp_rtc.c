/**
 * @file       bsp_i2c.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief      Board Support Package for Real-time clock IC
 * @note	   ds1307 must be inited before using (ds1307_init)
 */

/* Public includes ---------------------------------------------------------- */
#include "bsp_rtc.h"

/* Private includes --------------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------------- */
/* Private macros ----------------------------------------------------------- */
/* Public variables --------------------------------------------------------- */
/* Private variables -------------------------------------------------------- */
ds1307_t ds1307;

/* Private prototypes ------------------------------------------------------- */
/* Public implementations --------------------------------------------------- */
ds1307_status_t ds1307_init(void)
{
  /* Assign i2c funtion for ds1307
   * bsp_i2c function can be changed belong to which i2c is used
   */
  ds1307.i2c_master_transmit = bsp_i2c2_master_transmit;
  ds1307.i2c_master_receive = bsp_i2c2_master_receive;
  ds1307.i2c_init = bsp_i2c2_init;

  // Init i2c
  if (ds1307.i2c_init() != BSP_I2C_STATUS_OK)
  {
    return DS1307_STATUS_ERROR;
  }
  return DS1307_STATUS_OK;
}

ds1307_status_t ds1307_set_register(uint8_t reg_address, uint8_t reg_value)
{
  uint8_t pdata[2] = { reg_address, reg_value };  // send register address and register value
  if (ds1307.i2c_master_transmit(DS1307_I2C_ADDR, pdata, 2, DS1307_TIMEOUT) != BSP_I2C_STATUS_OK)
  {
    return DS1307_STATUS_ERROR;
  }
  return DS1307_STATUS_OK;
}

ds1307_status_t ds1307_get_register(uint8_t reg_address, uint8_t *reg_val)
{
  // send register address
  if (ds1307.i2c_master_transmit(DS1307_I2C_ADDR, &reg_address, 1, DS1307_TIMEOUT) != BSP_I2C_STATUS_OK)
  {
    return DS1307_STATUS_ERROR;
  }
  // get register value
  if (ds1307.i2c_master_receive(DS1307_I2C_ADDR, reg_val, 1, DS1307_TIMEOUT) != BSP_I2C_STATUS_OK)
  {
    return DS1307_STATUS_ERROR;
  }
  return DS1307_STATUS_OK;
}

ds1307_status_t ds1307_set_clock_halt(uint8_t halt_value)
{
  uint8_t reg_sec;
  if (ds1307_get_register(DS1307_REG_SECOND, &reg_sec) != DS1307_STATUS_OK)  // Get second register
  {
    return DS1307_STATUS_ERROR;
  }

  // add halt bit into MSB(bit 7th) of second register
  if (ds1307_set_register(DS1307_REG_SECOND, (halt_value << 7) | reg_sec) != DS1307_STATUS_OK)
  {
    return DS1307_STATUS_ERROR;
  }
  return DS1307_STATUS_OK;
}

uint8_t ds1307_get_clock_halt(void)
{
  uint8_t reg_sec;
  ds1307_get_register(DS1307_REG_SECOND, &reg_sec);
  // get_value & 0x80 to take MSB
  // >> 7 to get the real value
  return (reg_sec & 0x80) >> 7;
}

uint8_t ds1307_hex_to_bcd(uint8_t hex)
{
  return (hex % 10 + ((hex / 10) << 4));
}

void ds1307_bcd_to_hex(uint8_t *bcd)
{
  *bcd = ((*bcd & 0xf0) >> 4) * 10 + (*bcd & 0x0f);
}
/* End of file -------------------------------------------------------------- */