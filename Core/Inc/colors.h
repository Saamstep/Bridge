#ifndef COLORS_H
#define COLORS_H

/* ****************************************************** */
/* Color Definitions                                      */
/* ****************************************************** */
#define RED 		(0x000000ff | LED_BRIGHTNESS_DEFAULT)
#define GREEN 	(0x0000ff00 | LED_BRIGHTNESS_DEFAULT)
#define BLUE 		(0x00ff0000 | LED_BRIGHTNESS_DEFAULT)
#define WHITE		(0x00555555 | LED_BRIGHTNESS_DEFAULT)
#define PURPLE	(0x007f007f | LED_BRIGHTNESS_DEFAULT)
#define YELLOW 	(0x000064FF | LED_BRIGHTNESS_DEFAULT)
#define ORANGE 	(0x000014F5 | LED_BRIGHTNESS_DEFAULT)
#define INDIGO 	(0x00ef0010 | LED_BRIGHTNESS_DEFAULT)

/* ****************************************************** */
/* LED Generic Definitions                                */
/* ****************************************************** */
#define LED_COLOR_RESET 0xE0000000
#define LED_BRIGHTNESS_DEFAULT 0xE5000000

#endif
