#ifndef _ST77922_H_
#define _ST77922_H_

#include "hal/gpio_ll.h"

#define LCD_WIDTH 320
#define LCD_HEIGHT 480

#define WR_RAM_C_CMD 0x3C
#define WR_RAM_CMD 0x2C
#define RD_RAM_CMD 0x2E
#define SET_X_CMD  0x2A
#define SET_Y_CMD  0x2B
#define MADCTL_CMD 0x36
#define MADCTL_MY 0x80
#define MADCTL_MX 0x40
#define MADCTL_MV 0x20
#define QSPI_1W_CMD 0x02
#define QSPI_4W_CMD 0x32

#define TX_LEN (0x4000)

#ifndef TFT_QSPI_PORT
	#define TFT_QSPI_PORT SPI2_HOST
#endif
#ifndef TFT_QSPI_FREQUENCY
	#define TFT_QSPI_FREQUENCY 80000000
#endif
#ifndef TFT_QSPI_MODE
	#define TFT_QSPI_MODE SPI_MODE0
#endif

#if defined(ST77922_DRIVER) && (!defined(TFT_QSPI_CS) || !defined(TFT_QSPI_SCLK) || \
		!defined(TFT_QSPI_D0) || !defined(TFT_QSPI_D1) || \
	!defined(TFT_QSPI_D2) || !defined(TFT_QSPI_D3))
	#error "ST77922 QSPI requires TFT_QSPI_CS, TFT_QSPI_SCLK and TFT_QSPI_D0..TFT_QSPI_D3 in the selected TFT_eSPI setup"
#endif

//Pin operation
#if ((TFT_QSPI_CS>=0) && (TFT_QSPI_CS<32))
	#define LCD_CS_LOW  GPIO.out_w1tc = (1 << TFT_QSPI_CS)
	#define LCD_CS_HIGH GPIO.out_w1ts = (1 << TFT_QSPI_CS)
#elif (TFT_QSPI_CS>=32)
	#define LCD_CS_LOW  GPIO.out1_w1tc.val = (1 << (TFT_QSPI_CS - 32))
	#define LCD_CS_HIGH GPIO.out1_w1ts.val = (1 << (TFT_QSPI_CS - 32))
#endif

#ifdef TFT_BL
	#if ((TFT_BL>=0) && (TFT_BL<32))
			#define LCD_BL_LOW  GPIO.out_w1tc = (1 << TFT_BL)
			#define LCD_BL_HIGH GPIO.out_w1ts = (1 << TFT_BL)
	#elif (TFT_BL>=32)
			#define LCD_BL_LOW  GPIO.out1_w1tc.val = (1 << (TFT_BL - 32))
			#define LCD_BL_HIGH GPIO.out1_w1ts.val = (1 << (TFT_BL - 32))
	#endif
#else
	#define LCD_BL_LOW
	#define LCD_BL_HIGH
#endif

class ST77922
{
public:
	ST77922(void);
	void Begin(void);
	void Write_Reg(uint32_t cmd, void *data, uint8_t len);
	void Init(void);
	void Set_Rotation(uint8_t r);
	uint8_t Get_Rotation(void);
	void Set_Windows(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey);
	void Fill_Colors(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h, uint16_t* color);
	void Push_Image(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h, const uint16_t* color, bool swap_bytes);
	void Push_Pixels(const uint16_t* color, uint32_t count, bool swap_bytes);
	//does not work
	void Draw_Pixel(uint16_t x, uint16_t y, uint16_t color);

	uint16_t Get_Width(void);
	uint16_t Get_Height(void);
private:
	uint16_t width, height, rotation;
};

typedef struct {
    uint8_t cmd;       
    void *data;      
    uint8_t len;     
    uint32_t delay_ms; 
}lcd_init_cmd;

#endif