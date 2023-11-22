/**
 * @file       ssd1306.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Huy Le
 * @brief
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRIVER_SSD1306_H_
#define __DRIVER_SSD1306_H_

/* Includes ----------------------------------------------------------- */
#include "driver_ssd1306_defines.h"
#include "driver_ssd1306_fonts.h"
#include "main.h"
#include "bsp_delay_timer.h"
#include "bsp_i2c.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Public defines ----------------------------------------------------- */
#define SSD1306_I2C_PORT		STM32_I2C_PORT

// I2c address
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR        SSD1306_ADDRESS<<1 // 0x3C<<1 = 0x78
#endif

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH           128
#endif

// SSD1306 LCD height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT          64
#endif

#ifndef SSD1306_COM_LR_REMAP
#define SSD1306_COM_LR_REMAP    0
#endif // SSD1306_COM_LR_REMAP

#ifndef SSD1306_COM_ALTERNATIVE_PIN_CONFIG
#define SSD1306_COM_ALTERNATIVE_PIN_CONFIG    1
#endif // SSD1306_COM_ALTERNATIVE_PIN_CONFIG

// SSD1306 LCD Buffer Size
#define SSD1306_BUFFER_SIZE		(SSD1306_WIDTH * SSD1306_HEIGHT / 8)

// Display commands
#define SSD1306_CHARGEPUMP 			0x8D
#define SSD1306_COLUMNADDR 			0x21
#define SSD1306_COMSCANDEC 			0xC8
#define SSD1306_COMSCANINC 			0xC0
#define SSD1306_DISPLAYALLON 		0xA5
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYOFF 			0xAE
#define SSD1306_DISPLAYON 			0xAF
#define SSD1306_EXTERNALVCC 		0x1
#define SSD1306_INVERTDISPLAY 		0xA7
#define SSD1306_MEMORYMODE 			0x20
#define SSD1306_NORMALDISPLAY 		0xA6
#define SSD1306_PAGEADDR 			0x22
#define SSD1306_SEGREMAP 			0xA0
#define SSD1306_SETCOMPINS 			0xDA
#define SSD1306_SETCONTRAST 		0x81
#define SSD1306_SETDISPLAYCLOCKDIV 	0xD5
#define SSD1306_SETDISPLAYOFFSET 	0xD3
#define SSD1306_SETHIGHCOLUMN 		0x10
#define SSD1306_SETLOWCOLUMN 		0x00
#define SSD1306_SETMULTIPLEX 		0xA8
#define SSD1306_SETPRECHARGE 		0xD9
#define SSD1306_SETSEGMENTREMAP 	0xA1
#define SSD1306_SETSTARTLINE		0x40
#define SSD1306_SETVCOMDETECT 		0xDB
#define SSD1306_SWITCHCAPVCC 		0x02

/* Public enumerate/structure ----------------------------------------- */
typedef enum {
	SSD1306_OK,
	SSD1306_ERROR,
}ssd1306_status_t;

/**
 * @brief Enumeration for defining colors in SSD1306 display.
 */
typedef enum {
    SSD1306_COLOR_BLACK = 0x00,   // Black color, no pixel
    SSD1306_COLOR_WHITE = 0x01,   // Pixel is set. Color depends on LCD
} ssd1306_color_t;

/**
 * @brief Struct representing the configuration and state of the SSD1306 display.
 */
typedef struct {
    uint16_t current_x;   // The current X-coordinate for rendering
    uint16_t current_y;   // The current Y-coordinate for rendering
    uint8_t  inverted;    // Flag indicating if display rendering is inverted
    uint8_t  initialized; // Flag indicating if the display has been initialized
} ssd1306_t;

/* Public variables --------------------------------------------------- */
extern I2C_HandleTypeDef SSD1306_I2C_PORT;

/* Private function prototypes -----------------------------------------------*/
/**
 * @brief Initialize the SSD1306 display.
 *
 * @return true if initialization is successful, false otherwise.
 */
ssd1306_status_t ssd1306_init();

/**
 * @brief Fill the entire screen with a specified color.
 *
 * @param color The color to fill the screen with.
 */
void ssd1306_fill(ssd1306_color_t color);

/**
 * @brief Update the display screen with the drawn content.
 */
void ssd1306_update_screen(void);

/**
 * @brief Draw a pixel at the specified coordinates with the given color.
 *
 * @param x The X-coordinate of the pixel.
 * @param y The Y-coordinate of the pixel.
 * @param color The color of the pixel.
 */
void ssd1306_draw_pixel(uint8_t x, uint8_t y, ssd1306_color_t color);

/**
 * @brief Draw a bitmap image on the display.
 *
 * @param x The starting X-coordinate for drawing the bitmap.
 * @param y The starting Y-coordinate for drawing the bitmap.
 * @param bitmap Pointer to the bitmap image data.
 * @param width Width of the bitmap image.
 * @param height Height of the bitmap image.
 */
void ssd1306_draw_bitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t width, int16_t height);

/**
 * @brief Write a character to the display using the specified font and color.
 *
 * @param ch The character to write.
 * @param font The font to use for rendering.
 * @param color The color of the character.
 * @return The character that was written.
 */
char ssd1306_write_char(char ch, font_def font, ssd1306_color_t color);

/**
 * @brief Write a string to the display using the specified font and color.
 *
 * @param str The string to write.
 * @param font The font to use for rendering.
 * @param color The color of the string.
 * @return The last character that was written.
 */
char ssd1306_write_string(char* str, font_def font, ssd1306_color_t color);

/**
 * @brief Set the cursor position for text rendering.
 *
 * @param x The X-coordinate of the cursor.
 * @param y The Y-coordinate of the cursor.
 */
void ssd1306_set_cursor(uint8_t x, uint8_t y);

/**
 * @brief Turn on the display.
 */
void ssd1306_display_on(void);

/**
 * @brief Turn off the display.
 */
void ssd1306_display_off(void);

/**
 * @brief Invert the display content.
 */
void ssd1306_invert_display(void);

/**
 * @brief Set the display back to normal mode (non-inverted).
 */
void ssd1306_normal_display(void);

/**
 * @brief Reset the display orientation to its default.
 */
void ssd1306_reset_orientation(void);

/**
 * @brief Flip the screen vertically.
 */
void ssd1306_flip_screen_vertically(void);

/**
 * @brief Mirror the screen content.
 */
void ssd1306_mirror_screen(void);

/**
 * @brief Clear the display content.
 */
void ssd1306_clear(void);

#endif /* __DRIVER_SSD1306_H_ */

/* End of file -------------------------------------------------------- */