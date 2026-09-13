#ifndef TFT_WIDTH
  #define TFT_WIDTH  320
#endif
#ifndef TFT_HEIGHT
  #define TFT_HEIGHT 480
#endif

#define TFT_DRIVER 0x7792

#define TFT_INIT_DELAY 0x80

#define TFT_NOP     0x00
#define TFT_SWRST   0x01
#define TFT_INVOFF  0x20
#define TFT_INVON   0x21
#define TFT_DISPOFF 0x28
#define TFT_DISPON  0x29
#define TFT_CASET   0x2A
#define TFT_PASET   0x2B
#define TFT_RAMWR   0x2C
#define TFT_RAMRD   0x2E
#define TFT_MADCTL  0x36
#define TFT_MAD_MY  0x80
#define TFT_MAD_MX  0x40
#define TFT_MAD_MV  0x20
#define TFT_MAD_BGR 0x08
#define TFT_MAD_RGB 0x00
#define TFT_MAD_COLOR_ORDER TFT_MAD_BGR