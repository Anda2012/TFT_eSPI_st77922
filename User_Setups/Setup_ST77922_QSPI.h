#define USER_SETUP_INFO "ST77922 QSPI"

#define ST77922_DRIVER

// ST77922 QSPI display wiring. Adjust these values for the target board.
#define TFT_QSPI_CS    10
#define TFT_QSPI_SCLK  12
#define TFT_QSPI_D0    11
#define TFT_QSPI_D1    13
#define TFT_QSPI_D2    14
#define TFT_QSPI_D3     9
#define TFT_QSPI_PORT  SPI2_HOST
#define TFT_QSPI_FREQUENCY 80000000
#define TFT_QSPI_MODE SPI_MODE0

#define TFT_BL 41
#define TFT_BACKLIGHT_ON HIGH

#define ST77922_TOUCH_SCL 39
#define ST77922_TOUCH_SDA 38
#define ST77922_TOUCH_RST 48
#define ST77922_TOUCH_INT 47

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4