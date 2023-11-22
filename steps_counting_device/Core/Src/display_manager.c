/**
 * @file       display_manager.c
 * @version    1.0.0
 * @date
 * @author     Huy Le
 * @brief
 * @note
 * @example
 */

/* Includes ----------------------------------------------------------- */
#include "display_manager.h"
#include "frame.h"
#include "logo_itr.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
static char id1[4] = { 151, 237, 41, 191 };
static char id2[4] = { 78, 164, 185, 170 };
static char id3[4] = { 233, 164, 185, 25 };
static uint32_t timestamp = 0;
static uint8_t id_present[16];

/* Private function prototypes ---------------------------------------- */
static void number_to_string(uint8_t num, char *buf);
static char *time_to_string(ds1307_time_t *time);
static char *date_to_string(ds1307_time_t *time);
static char *get_dow(uint8_t dow);
static void display_manager_bmp(const unsigned char *bitmap);
static void display_manager_message(char *str, uint8_t x, uint8_t y);
static void display_manager_rtc_oled(ds1307_time_t *rtc_time);
static void display_manager_steps_count_oled(uint16_t step, uint8_t x, uint8_t y);
static char *get_card_holder_name(uint8_t *card_id);
static void display_manager_name_from_card_id(uint8_t *card_id, uint8_t x, uint8_t y);

/* Function definitions ----------------------------------------------- */
display_manager_status_t display_manager_init()
{
  if (bsp_delay_tim_init() != BSP_DELAY_TIMER_OK || ssd1306_init() != SSD1306_OK
      || ds1307_init() != DS1307_STATUS_OK || drv_mfrc522_init() != DRV_MFRC522_OK)
  {
    return DISPLAY_MANAGER_ERROR;
  }
  display_manager_bmp(itr);
  ssd1306_update_screen();
  bsp_delay_tim_ms(1000);
  ssd1306_clear();
  return DISPLAY_MANAGER_OK;
}

void display_manager(uint8_t scan_case, uint8_t *card_id, uint16_t *step, ds1307_time_t *rtc_time)
{
  switch (scan_case)
  {
  case ZERO_SCAN:
  {
    ssd1306_fill(SSD1306_COLOR_BLACK);
    display_manager_message("SCAN THE CARD !!", 10, 22);
    ssd1306_update_screen();
    break;
  }
  case FIRST_SCAN:
  {
    ssd1306_fill(SSD1306_COLOR_BLACK);
    display_manager_bmp(frame);
    display_manager_rtc_oled(rtc_time);
    display_manager_steps_count_oled(*step, 102, 34);
    display_manager_name_from_card_id(card_id, 4, 40);
    ssd1306_update_screen();
    break;
  }
  case SECOND_SCAN:
  {
    timestamp = HAL_GetTick();
    uint16_t step_display = *step;
    while (HAL_GetTick() - timestamp < 5000)  // display finish screen for 5 seconds
    {
      ssd1306_fill(SSD1306_COLOR_BLACK);
      display_manager_message("STOP COUNTING!!", 10, 22);
      display_manager_name_from_card_id(card_id, 10, 44);
      display_manager_steps_count_oled(step_display, 100, 44);
      ssd1306_update_screen();
    }
    user_clear_flag();
    break;
  }

  default: break;
  }
}

void display_backup_screen(void)
{
  ssd1306_fill(SSD1306_COLOR_BLACK);
  display_manager_message("START AGAIN :((", 10, 22);
  ssd1306_update_screen();
}

/* Private functions ---------------------------------------------------*/
/**
 * @brief Convert a number to a string representation.
 *
 * This function converts a given number (0-99) to its string representation
 * and stores it in the provided character buffer.
 *
 * @param num The number to convert (0-99).
 * @param buf The character buffer to store the string representation.
 */
static void number_to_string(uint8_t num, char *buf)
{
  if (num < 10)
  {
    buf[0] = '0';
    buf[1] = num + 48;  // Convert the number to ASCII character
  }
  else if (num > 10 && num < 99)
  {
    buf[0] = (num / 10) + 48;  // Convert the tens digit to ASCII character
    buf[1] = (num % 10) + 48;  // Convert the ones digit to ASCII character
  }
}

/**
 * @brief Convert a DS1307 time structure to a string representation.
 *
 * This function converts the provided DS1307 time structure to a string in the format "HH:MM:SS".
 * The result is stored in a static character buffer.
 *
 * @param time A pointer to a DS1307_time_t structure containing the time to be converted.
 * @return A pointer to the static character buffer containing the string representation.
 */
static char *time_to_string(ds1307_time_t *time)
{
  static char buf[9];
  buf[2] = ':';
  buf[5] = ':';
  number_to_string(time->hour, buf);
  number_to_string(time->minute, &buf[3]);
  number_to_string(time->second, &buf[6]);
  buf[8] = '\0';
  return buf;
}

/**
 * @brief Convert a DS1307 time structure to a date string representation.
 *
 * This function converts the provided DS1307 time structure to a string in the format "DD/MM/YY".
 * The result is stored in a static character buffer.
 *
 * @param time A pointer to a DS1307_time_t structure containing the date to be converted.
 * @return A pointer to the static character buffer containing the string representation.
 */
static char *date_to_string(ds1307_time_t *time)
{
  static char buf[9];
  buf[2] = '/';
  buf[5] = '/';
  number_to_string(time->date, buf);
  number_to_string(time->month, &buf[3]);
  number_to_string(time->year, &buf[6]);
  buf[8] = '\0';
  return buf;
}

/**
 * @brief Get the day of the week as a string.
 *
 * This function takes an integer value representing the day of the week (0-6 or 1-7, depending on the convention)
 * and returns a pointer to a string representing the corresponding day of the week.
 *
 * @param dow The day of the week as an integer (0-6 or 1-7).
 * @return A pointer to a string representing the day of the week.
 */
static char *get_dow(uint8_t dow)
{
  char *day[] = { "MON ", "TUE ", "WED ", "THU ", "FRI ", "SAT ", "SUN " };
  return day[dow - 1];
}

/**
 * @brief Display a bitmap on the OLED screen.
 *
 * This function displays a bitmap image on the OLED screen at coordinates (0, 0).
 *
 * @param bitmap A pointer to the bitmap image data.
 */
static void display_manager_bmp(const unsigned char *bitmap)
{
  ssd1306_draw_bitmap(0, 0, bitmap, 128, 64);  // Display a bitmap on the OLED screen
}

/**
 * @brief Display a message on the OLED screen.
 *
 * This function sets the cursor position on the OLED screen to the specified (x, y) coordinates
 * and then writes the provided string to the screen using the specified font and color.
 *
 * @param str A pointer to the string to be displayed.
 * @param x The x-coordinate of the cursor position on the screen.
 * @param y The y-coordinate of the cursor position on the screen.
 */
static void display_manager_message(char *str, uint8_t x, uint8_t y)
{
  ssd1306_set_cursor(x, y);                                   // Set the cursor position on the OLED screen
  ssd1306_write_string(str, font_7x10, SSD1306_COLOR_WHITE);  // Write a string to the OLED screen
}

/**
 * @brief Display RTC time and date on the OLED screen.
 *
 * This function retrieves the RTC time from the DS1307 module, formats and displays it on the OLED screen.
 *
 * @param rtc_time A pointer to a DS1307_time_t structure containing the RTC time and date.
 */
static void display_manager_rtc_oled(ds1307_time_t *rtc_time)
{
  ds1307_get_time(rtc_time);  // Get the RTC time

  char *am_pm;
  if (rtc_time->format != DS1307_FORMAT_24H)
  {
    am_pm = (rtc_time->am_pm != DS1307_PM) ? "AM" : "PM";                            // Determine AM/PM format
    ssd1306_set_cursor(2, 4);
    ssd1306_write_string(time_to_string(rtc_time), font_7x10, SSD1306_COLOR_WHITE);  // Display the time
    ssd1306_write_string(am_pm, font_7x10, SSD1306_COLOR_WHITE);                     // Display AM/PM
  }
  else
  {
    ssd1306_set_cursor(2, 4);
    ssd1306_write_string(time_to_string(rtc_time), font_7x10,
                         SSD1306_COLOR_WHITE);  // Display the time (24-hour format)
  }
  ssd1306_set_cursor(2, 16);
  ssd1306_write_string(get_dow(rtc_time->day), font_7x10,
                       SSD1306_COLOR_WHITE);  // Display the day of the week
  ssd1306_write_string(date_to_string(rtc_time), font_7x10, SSD1306_COLOR_WHITE);  // Display the date
}

/**
 * @brief Display the step count on the OLED screen.
 *
 * This function converts the provided step count to a string and displays it on the OLED screen
 * at the specified (x, y) coordinates.
 *
 * @param step The step count to be displayed.
 * @param x The x-coordinate of the screen position.
 * @param y The y-coordinate of the screen position.
 */
static void display_manager_steps_count_oled(uint16_t step, uint8_t x, uint8_t y)
{
  char str[16];
  sprintf(str, "%d", step);            // Convert the step count to a string
  display_manager_message(str, x, y);  // Display the step count on the OLED screen
}

/**
 * @brief Get the card holder's name based on the card ID.
 *
 * This function retrieves the card holder's name based on the provided card ID.
 * It compares the card ID with predefined card IDs (id1, id2, id3) to determine the name.
 * If no match is found, it returns "Un Known".
 *
 * @param card_id A pointer to the card ID data.
 * @return A pointer to the card holder's name.
 */
static char *get_card_holder_name(uint8_t *card_id)
{
  // Note: The following code appears to be incomplete or missing some context.
  // You may need to provide the definitions of 'id1', 'id2', and 'id3' for this function to work correctly.
  for (uint8_t i = 0; i < sizeof(card_id); i++)
  {
    if (card_id[i + 1] == id1[i])
    {
      return "Dung Nguyen";
    }
    else if (card_id[i + 1] == id2[i])
    {
      return "Huy Le     ";
    }
    else if (card_id[i + 1] == id3[i])
    {
      return "Loi Ho     ";
    }
    else
      return "Un Known   ";
  }
  return "Un Known   ";
}

/**
 * @brief Display the card holder's name on the OLED screen.
 *
 * This function retrieves the card holder's name based on the provided card ID
 * and displays it on the OLED screen at the specified (x, y) coordinates.
 *
 * @param card_id A pointer to the card ID data.
 * @param x The x-coordinate on the OLED screen.
 * @param y The y-coordinate on the OLED screen.
 */
static void display_manager_name_from_card_id(uint8_t *card_id, uint8_t x, uint8_t y)
{
  char *name = get_card_holder_name(card_id);  // Get the card holder's name
  display_manager_message(name, x, y);         // Display the name on the OLED screen
}

/* End of file -------------------------------------------------------- */