/**
 * @file       ssd1306.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Huy Le
 * @brief
 * @note
 */

/* Includes ----------------------------------------------------------- */
#include "driver_ssd1306.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];
static ssd1306_t ssd1306;

/* Private function prototypes ---------------------------------------- */
static uint8_t ssd1306_write_cmd(uint8_t command);
static uint8_t ssd1306_write_data(uint8_t *data, uint8_t size);

/* Function definitions ----------------------------------------------- */
static uint8_t ssd1306_write_data(uint8_t *data, uint8_t size)
{
	bsp_i2c3_write_mem(SSD1306_ADDRESS, 0x40, data, size, 10);
	return 0;
}
static uint8_t ssd1306_write_cmd(uint8_t command)
{
	bsp_i2c3_write_mem(SSD1306_ADDRESS, 0x00, &command, 1, 100);
	return 0;
}

//
//  Initialize the oled screen
//
ssd1306_status_t ssd1306_init()
{
	bsp_i2c3_init();
	/* Check if LCD connected to I2C */
	if (bsp_i2c3_is_device_ready(SSD1306_ADDRESS, 5, 1000) == BSP_I2C_STATUS_ERROR)
	{
		ssd1306.initialized = 0;
		/* Return false */
		return SSD1306_ERROR;
	}
	ssd1306_clear();
    // Init LCD
    ssd1306_write_cmd(SSD1306_DISPLAYOFF);   			// Display off
    ssd1306_write_cmd(SSD1306_MEMORYMODE);   			// Set Memory Addressing Mode
    ssd1306_write_cmd(0x10);   							// 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    ssd1306_write_cmd(0xB0);   							// Set Page Start Address for Page Addressing Mode,0-7
    ssd1306_write_cmd(SSD1306_COMSCANDEC);   			// Set COM Output Scan Direction
    ssd1306_write_cmd(SSD1306_SETLOWCOLUMN);   			// Set low column address
    ssd1306_write_cmd(SSD1306_SETHIGHCOLUMN);   		// Set high column address
    ssd1306_write_cmd(SSD1306_SETSTARTLINE);   			// Set start line address
    ssd1306_write_cmd(SSD1306_SETCONTRAST);   			// set contrast control register
    ssd1306_write_cmd(0xFF);
    ssd1306_write_cmd(SSD1306_SETSEGMENTREMAP); 		// Set segment re-map 0 to 127
    ssd1306_write_cmd(SSD1306_NORMALDISPLAY);   		// Set normal display

    ssd1306_write_cmd(SSD1306_SETMULTIPLEX);   			// Set multiplex ratio(1 to 64)
    ssd1306_write_cmd(SSD1306_HEIGHT - 1);

    ssd1306_write_cmd(SSD1306_DISPLAYALLON_RESUME);   	// 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    ssd1306_write_cmd(SSD1306_SETDISPLAYOFFSET);   		// Set display offset
    ssd1306_write_cmd(0x00);   							// No offset
    ssd1306_write_cmd(SSD1306_SETDISPLAYCLOCKDIV);   	// Set display clock divide ratio/oscillator frequency
    ssd1306_write_cmd(0xF0);   							// Set divide ratio
    ssd1306_write_cmd(SSD1306_SETPRECHARGE);   			// Set pre-charge period
    ssd1306_write_cmd(SSD1306_PAGEADDR);

    ssd1306_write_cmd(SSD1306_SETCOMPINS);   			// Set com pins hardware configuration
    ssd1306_write_cmd(SSD1306_COM_LR_REMAP << 5 | SSD1306_COM_ALTERNATIVE_PIN_CONFIG << 4 | 0x02);

    ssd1306_write_cmd(SSD1306_SETVCOMDETECT);   		// Set vcomh
    ssd1306_write_cmd(0x20);   							// 0x20,0.77xVcc
    ssd1306_write_cmd(SSD1306_CHARGEPUMP);   			// Set DC-DC enable
    ssd1306_write_cmd(0x14);
    ssd1306_write_cmd(SSD1306_DISPLAYON);   			// Turn on SSD1306 panel

    // Clear screen
    ssd1306_fill(SSD1306_COLOR_BLACK);

    // Flush buffer to screen
    ssd1306_update_screen();

    // Set default values for screen object
    ssd1306.current_x = 0;
    ssd1306.current_x = 0;

    ssd1306.initialized = 1;

    return SSD1306_OK;
}

//
//  Fill the whole screen with the given color
//
void ssd1306_fill(ssd1306_color_t color)
{
    // Fill screenbuffer with a constant value (color)
    uint32_t i;

    for(i = 0; i < sizeof(ssd1306_buffer); i++)
    {
        ssd1306_buffer[i] = (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF;
    }
}

//
//  Write the screenbuffer with changed to the screen
//
void ssd1306_update_screen(void)
{
    uint8_t page_index;

    for (page_index = 0; page_index < 8; page_index++) {
        ssd1306_write_cmd(0xB0 + page_index);
        ssd1306_write_cmd(SSD1306_SETLOWCOLUMN);
        ssd1306_write_cmd(SSD1306_SETHIGHCOLUMN);
        ssd1306_write_data(&ssd1306_buffer[SSD1306_WIDTH * page_index], SSD1306_WIDTH);
    }
}

//
//  Draw one pixel in the screenbuffer
//  X => X Coordinate
//  Y => Y Coordinate
//  color => Pixel color
//
void ssd1306_draw_pixel(uint8_t x, uint8_t y, ssd1306_color_t color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        // Don't write outside the buffer
        return;
    }

    // Check if pixel should be inverted
    if (ssd1306.inverted)
    {
        color = (ssd1306_color_t)!color;
    }

    // Draw in the correct color
    if (color == SSD1306_COLOR_WHITE)
    {
        ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void ssd1306_draw_bitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t width, int16_t height)
{

    int16_t byte_width = (width + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for(int16_t row=0; row<height; row++, y++)
    {
        for(int16_t col=0; col<width; col++)
        {
            if(col & 7)
            {
               byte <<= 1;
            }
            else
            {
               byte = (*(const unsigned char *)(&bitmap[row * byte_width + col / 8]));
            }
            if(byte & 0x80) ssd1306_draw_pixel(x + col, y, SSD1306_COLOR_WHITE);
        }
    }
}

//
//  Draw 1 char to the screen buffer
//  ch      => Character to write
//  Font    => Font to use
//  color   => Black or White
//
char ssd1306_write_char(char ch, font_def font, ssd1306_color_t color)
{
    uint32_t row, row_pixels_data, col;

    // Check remaining space on current line
    if (SSD1306_WIDTH <= (ssd1306.current_x + font.font_width) ||
        SSD1306_HEIGHT <= (ssd1306.current_y + font.font_height))
    {
        // Not enough space on current line
        return 0;
    }

    // Translate font to screenbuffer
    for (row = 0; row < font.font_height; row++)
    {
        row_pixels_data = font.data[(ch - 32) * font.font_height + row];
        for (col = 0; col < font.font_width; col++)
        {
            if ((row_pixels_data << col) & 0x8000)
            {
                ssd1306_draw_pixel(ssd1306.current_x + col, (ssd1306.current_y + row), (ssd1306_color_t) color);
            }
            else
            {
                ssd1306_draw_pixel(ssd1306.current_x + col, (ssd1306.current_y + row), (ssd1306_color_t)!color);
            }
        }
    }

    // The current space is now taken
    ssd1306.current_x += font.font_width;

    // Return written char for validation
    return ch;
}

//
//  Write full string to screenbuffer
//
char ssd1306_write_string(char* str, font_def font, ssd1306_color_t color)
{
    // Write until null-byte
    while (*str)
    {
        if (ssd1306_write_char(*str, font, color) != *str)
        {
            // Char could not be written
            return *str;
        }

        // Next char
        str++;
    }

    // Everything ok
    return *str;
}

//
//  Set cursor position
//
void ssd1306_set_cursor(uint8_t x, uint8_t y)
{
    ssd1306.current_x = x;
    ssd1306.current_y = y;
}

void ssd1306_display_on(void)
{
	ssd1306_write_cmd(SSD1306_DISPLAYON);
}

void ssd1306_display_off(void)
{
	ssd1306_write_cmd(SSD1306_DISPLAYOFF);
}

void ssd1306_invert_display(void)
{
	ssd1306_write_cmd(SSD1306_INVERTDISPLAY);
}

void ssd1306_normal_display(void)
{
	ssd1306_write_cmd(SSD1306_NORMALDISPLAY);
}

void ssd1306_reset_orientation(void)
{
	ssd1306_write_cmd(SSD1306_SEGREMAP);
	ssd1306_write_cmd(SSD1306_COMSCANINC);           //Reset screen rotation or mirroring
}

void ssd1306_flip_screen_vertically(void)
{
	ssd1306_write_cmd(SSD1306_SEGREMAP | 0x01);
	ssd1306_write_cmd(SSD1306_COMSCANDEC);           //Rotate screen 180 Deg
}

void ssd1306_mirror_screen(void)
{
	ssd1306_write_cmd(SSD1306_SEGREMAP);
	ssd1306_write_cmd(SSD1306_COMSCANDEC);           //Mirror screen
}

void ssd1306_clear(void)
{
	memset(ssd1306_buffer, 0, SSD1306_BUFFER_SIZE);
}

/* End of file -------------------------------------------------------- */