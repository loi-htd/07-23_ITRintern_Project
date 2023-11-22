/**
 * @file       bsp_rtc.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief      Board Support Package for Real-time clock IC
 * @note	   ds1307 must be inited before using (ds1307_init)
 */

#ifndef __BSP_RTC_H_
#define __BSP_RTC_H_

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"
#include "ds1307.h"
#include "main.h"

#include <stdint.h>
#include <stdlib.h>


/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief Initialize the DS1307 real-time clock module.
 *
 * This function initializes the DS1307 module by configuring the I2C interface and setting it up for
 * communication.
 *
 * @return DS1307 status: DS1307_STATUS_OK if initialization is successful, DS1307_STATUS_ERROR on failure.
 */
ds1307_status_t ds1307_init(void);

/**
 * @brief Set a specific register of the DS1307 module.
 *
 * This function sets a specific register in the DS1307 module with the given register address and value.
 *
 * @param[in] reg_address   The address of the register to be set.
 * @param[in] reg_value     The value to write to the register.
 *
 * @return DS1307 status: DS1307_STATUS_OK if the operation is successful, DS1307_STATUS_ERROR on failure.
 */
ds1307_status_t ds1307_set_register(uint8_t reg_address, uint8_t reg_value);

/**
 * @brief Get the value of a specific register from the DS1307 module.
 *
 * This function reads the value of a specific register in the DS1307 module with the given register address.
 *
 * @param[in]  reg_address   The address of the register to be read.
 * @param[out] reg_val       Pointer to store the read value.
 *
 * @return DS1307 status: DS1307_STATUS_OK if the operation is successful, DS1307_STATUS_ERROR on failure.
 */
ds1307_status_t ds1307_get_register(uint8_t reg_address, uint8_t *reg_val);

/**
 * @brief Set or clear the Clock Halt bit of the DS1307 module.
 *
 * This function sets or clears the Clock Halt bit of the DS1307 module, allowing or halting the clock.
 *
 * @param[in] halt_value   Value to set the Clock Halt bit to (0 to run, 1 to halt).
 *
 * @return DS1307 status: DS1307_STATUS_OK if the operation is successful, DS1307_STATUS_ERROR on failure.
 */
ds1307_status_t ds1307_set_clock_halt(uint8_t halt_value);

/**
 * @brief Get the Clock Halt bit status from the DS1307 module.
 *
 * This function reads the Clock Halt bit status from the DS1307 module.
 *
 * @return Clock Halt status (0 if running, 1 if halted).
 */
uint8_t ds1307_get_clock_halt(void);

/**
 * @brief Convert a hexadecimal value to Binary-Coded Decimal (BCD) format.
 *
 * This function converts a hexadecimal value to Binary-Coded Decimal (BCD) format.
 *
 * @param[in] hex   The hexadecimal value to be converted.
 *
 * @return The BCD representation of the input value.
 */
uint8_t ds1307_hex_to_bcd(uint8_t hex);

/**
 * @brief Convert Binary-Coded Decimal (BCD) to hexadecimal format.
 *
 * This function converts a Binary-Coded Decimal (BCD) value to hexadecimal format.
 *
 * @param[in, out] bcd   Pointer to the BCD value to be converted. The result will be stored in the same
 * location.
 */
void ds1307_bcd_to_hex(uint8_t *bcd);
#endif
/* End of file -------------------------------------------------------------- */