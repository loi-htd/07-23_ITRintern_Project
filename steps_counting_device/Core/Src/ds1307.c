/**
 * @file       ds1307.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Loi Ho
 * @brief
 * @note
 */

/* Public includes ---------------------------------------------------------- */
#include "ds1307.h"

/* Private includes --------------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------------- */
/* Private macros ----------------------------------------------------------- */
/* Public variables --------------------------------------------------------- */
/* Private variables -------------------------------------------------------- */
/* Private prototypes ------------------------------------------------------- */
/* Public implementations --------------------------------------------------- */

void ds1307_set_time(ds1307_time_t time)
{
  time.second = time.second | (ds1307_get_clock_halt() << 7);  // Save halt bit value
  ds1307_set_register(DS1307_REG_SECOND, ds1307_hex_to_bcd(time.second));

  ds1307_set_register(DS1307_REG_MINUTE, ds1307_hex_to_bcd(time.minute));

  // Check hour format
  if (time.format == DS1307_FORMAT_24H)
  {
    // Format 24h: do not care about am or pm
    ds1307_set_register(DS1307_REG_HOUR, ds1307_hex_to_bcd(time.hour | time.format));
  }
  else
  {
    // Format 12h: include am_pm bit
    ds1307_set_register(DS1307_REG_HOUR, ds1307_hex_to_bcd(time.hour | time.format | time.am_pm));
  }

  ds1307_set_register(DS1307_REG_DAY, ds1307_hex_to_bcd(time.day));
  ds1307_set_register(DS1307_REG_DATE, ds1307_hex_to_bcd(time.date));
  ds1307_set_register(DS1307_REG_MONTH, ds1307_hex_to_bcd(time.month));
  ds1307_set_register(DS1307_REG_YEAR, ds1307_hex_to_bcd(time.year));
}

ds1307_status_t ds1307_get_time(ds1307_time_t *time)
{
  ds1307_status_t state = DS1307_STATUS_OK;

  /* STATUS_OK is 0, if any other states is returned, "state |= " will make state != 0 */
  state |= ds1307_get_register(DS1307_REG_SECOND, &time->second);
  time->second &= 0x7f;  // remove halt bit
  ds1307_bcd_to_hex(&time->second);

  state |= ds1307_get_register(DS1307_REG_MINUTE, &time->minute);
  ds1307_bcd_to_hex(&time->minute);

  state |= ds1307_get_register(DS1307_REG_HOUR, &time->hour);
  time->format = time->hour & 0x40;  // get format bit
  if (time->format == DS1307_FORMAT_24H)
  {
    time->hour &= 0x3f;  // remove format bit and assign hour data again
    ds1307_bcd_to_hex(&time->hour);
  }
  else if (time->format == DS1307_FORMAT_12H)  // DS1307_FORMAT_12H
  {
    time->am_pm = time->hour & 0x20;           // check am_pm bit
    time->hour &= 0x1f;                        // remove format bit and am_pm bit
    ds1307_bcd_to_hex(&time->hour);
  }
  else
    return DS1307_STATUS_ERROR;

  state |= ds1307_get_register(DS1307_REG_DAY, &time->day);
  ds1307_bcd_to_hex(&time->day);

  state |= ds1307_get_register(DS1307_REG_DATE, &time->date);
  ds1307_bcd_to_hex(&time->date);

  state |= ds1307_get_register(DS1307_REG_MONTH, &time->month);
  ds1307_bcd_to_hex(&time->month);

  state |= ds1307_get_register(DS1307_REG_YEAR, &time->year);
  ds1307_bcd_to_hex(&time->year);

  if (state != DS1307_STATUS_OK)
    return DS1307_STATUS_ERROR;
  else
    return DS1307_STATUS_OK;
}
