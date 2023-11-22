/**
 * @file       ds1307.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief
 * @note
 */
/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DS1307_H
#define __DS1307_H

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"
#include "main.h"

#include <stdint.h>
#include <stdlib.h>

/* Public defines ----------------------------------------------------- */
#define DS1307_I2C_ADDR 0x68
#define DS1307_REG_SECOND 0x00
#define DS1307_REG_MINUTE 0x01
#define DS1307_REG_HOUR 0x02
#define DS1307_REG_DAY 0x03
#define DS1307_REG_DATE 0x04
#define DS1307_REG_MONTH 0x05
#define DS1307_REG_YEAR 0x06
#define DS1307_REG_CONTROL 0x07
#define DS1307_REG_UTC_HR 0x08
#define DS1307_REG_UTC_MIN 0x09
#define DS1307_REG_CENT 0x10
#define DS1307_REG_RAM 0x11
#define DS1307_TIMEOUT 1000
/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief ds1307 day of week
 */
enum ds1307_day_of_week
{
  MONDAY = 1,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY,
  SUNDAY
};

/**
 * @brief ds1307 bool enumeration definition
 */
typedef enum
{
  DS1307_BOOL_FALSE = 0x00, /**< disable function */
  DS1307_BOOL_TRUE = 0x01,  /**< enable function */
} ds1307_bool_t;

/**
 * @brief ds1307 am pm enumeration definition
 */
typedef enum
{
  DS1307_AM = 0x20, /**< am */
  DS1307_PM = 0x00, /**< pm */
} ds1307_am_pm_t;

/**
 * @brief ds1307 format enumeration definition
 */
typedef enum
{
  DS1307_FORMAT_12H = 0x40, /**< 12h format */
  DS1307_FORMAT_24H = 0x00, /**< 24h format */
} ds1307_format_t;

/**
 * @brief ds1307 output level enumeration definition
 */
typedef enum
{
  DS1307_OUTPUT_LEVEL_LOW = 0x00,  /**< low level */
  DS1307_OUTPUT_LEVEL_HIGH = 0x01, /**< high level */
} ds1307_output_level_t;

/**
 * @brief ds1307 output mode enumeration definition
 */
typedef enum
{
  DS1307_OUTPUT_MODE_LEVEL = 0x00,       /**< level mode */
  DS1307_OUTPUT_MODE_SQUARE_WAVE = 0x01, /**< square wave mode */
} ds1307_output_mode_t;

/**
 * @brief ds1307 square wave frequency enumeration definition
 */
typedef enum
{
  DS1307_SQUARE_WAVE_FREQUENCY_1_HZ = 0x00,     /**< 1Hz */
  DS1307_SQUARE_WAVE_FREQUENCY_4096_HZ = 0x01,  /**< 4.096kHz */
  DS1307_SQUARE_WAVE_FREQUENCY_8192_HZ = 0x02,  /**< 8.192kHz */
  DS1307_SQUARE_WAVE_FREQUENCY_32768_HZ = 0x03, /**< 32.768kHz */
} ds1307_square_wave_frequency_t;

/**
 * @brief ds1307 time structure definition
 */
typedef struct ds1307_time_s
{
  uint8_t year;           /** year    0 - 100         */
  uint8_t month;          /** month   1 - 12          */
  uint8_t day;            /** day     MONDAY - SUNDAY */
  uint8_t date;           /** date    1 - 31          */
  uint8_t hour;           /** hour    0 - 24 / 0 -12  */
  uint8_t minute;         /** minute  0 - 60          */
  uint8_t second;         /** second  0 - 60          */
  ds1307_format_t format; /** data format             */
  ds1307_am_pm_t am_pm;   /** am pm                   */
} ds1307_time_t;

/**
 * @brief Enumeration for DS1307 module status.
 */
typedef enum ds1307_status_s
{
  DS1307_STATUS_OK,
  DS1307_STATUS_ERROR,
} ds1307_status_t;

/**
 * @brief Configuration structure for the DS1307 module.
 *
 * This structure defines the configuration or interface for the DS1307 module.
 * It contains function pointers for I2C master transmit, I2C master receive, and I2C initialization
 * functions.
 */
typedef struct ds1307_s
{
  bsp_i2c_status_t (*i2c_master_transmit)(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout);
  bsp_i2c_status_t (*i2c_master_receive)(uint8_t dev_addr, uint8_t *pdata, uint16_t len, uint32_t timeout);
  bsp_i2c_status_t (*i2c_init)(void);
} ds1307_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief Set the time in the DS1307 RTC module.
 *
 * This function sets the time in the DS1307 RTC module based on the provided
 * ds1307_time_t structure. It takes the time structure as a parameter and
 * configures the corresponding RTC registers.
 *
 * @param[in] time The time structure containing the values to set.
 */
void ds1307_set_time(ds1307_time_t time);

/**
 * @brief Get the time from the DS1307 RTC module.
 *
 * This function retrieves the time information from the DS1307 RTC module
 * and fills the provided ds1307_time_t structure with the corresponding values.
 *
 * @param[out] time Pointer to the time structure to fill with the retrieved values.
 */
ds1307_status_t ds1307_get_time(ds1307_time_t *time);
/* Private function prototypes ----------------------------------------- */
#endif
