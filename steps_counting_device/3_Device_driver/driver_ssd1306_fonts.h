/**
 * @file       fonts.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Huy Le
 * @brief
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRIVER_SSD1306_FONTS_H_
#define __DRIVER_SSD1306_FONTS_H_ 

/* Includes ----------------------------------------------------------- */
#include <stdint.h>

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Structure representing a font definition.
 */
typedef struct {
    uint8_t font_width;         /* Font width in pixels */
    uint8_t font_height;        /* Font height in pixels */
    const uint16_t *data;       /* Pointer to data font data array */
} font_def;

/* Public variables --------------------------------------------------- */
extern font_def font_7x10;
extern font_def font_11x18;
extern font_def font_16x26;

#endif  /* __DRIVER_SSD1306_FONTS_H_ */

/* End of file -------------------------------------------------------- */