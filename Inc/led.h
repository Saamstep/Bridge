/* ****************************************************** */
/* LED Strip Parameters                                   */
/* ****************************************************** */
#define LED_COUNT 60
#define LED_COLOR_RESET 0xE0000000
#define LED_BRIGHTNESS_DEFAULT 0xE5000000

extern unsigned int buffer[LED_COUNT];

void spi32(unsigned int c);
void sendData(unsigned int *ledArray);
void clearStrip(void);
