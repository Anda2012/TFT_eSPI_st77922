#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#if defined(ST77922_DRIVER) && defined(TFT_ESPI_ST77922_IMPLEMENTATION)
#include "driver/spi_master.h"
#include "ST77922.h"

static const lcd_init_cmd st77922_lcd_init[] = {
    {0xF1, (uint8_t []){0x00}, 1, 0},
    {0x60, (uint8_t []){0x00, 0x00, 0x00}, 3, 0},
    {0x65, (uint8_t []){0x80}, 1, 0},
    {0x79, (uint8_t []){0x06}, 1, 0},
    {0x7B, (uint8_t []){0x00, 0x08, 0x08}, 3, 0},
    {0x80, (uint8_t []){0x55, 0x62, 0x2F, 0x17, 0xF0, 0x52, 0x70, 0xD2, 0x52, 0x62, 0xEA}, 11, 0},
    {0x81, (uint8_t []){0x26, 0x52, 0x72, 0x27}, 4, 0},
    {0x84, (uint8_t []){0x92, 0x25}, 2, 0},
    {0x87, (uint8_t []){0x10, 0x10, 0x58, 0x00, 0x02, 0x3A}, 6, 0},
    {0x88, (uint8_t []){0x00, 0x00, 0x2C, 0x10, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x06}, 15, 0},
    {0x89, (uint8_t []){0x00, 0x00, 0x00}, 3, 0},
    {0x8A, (uint8_t []){0x13, 0x00, 0x2C, 0x00, 0x00, 0x2C, 0x10, 0x10, 0x00, 0x3E, 0x19}, 11, 0},
    {0x8B, (uint8_t []){0x15, 0xB1, 0xB1, 0x44, 0x96, 0x2C, 0x10, 0x97, 0x8E}, 9, 0},
    {0x8C, (uint8_t []){0x1D, 0xB1, 0xB1, 0x44, 0x96, 0x2C, 0x10, 0x50, 0x0F, 0x01, 0xC5, 0x12, 0x09}, 13, 0},
    {0x8D, (uint8_t []){0x0C}, 1, 0},
    {0x8E, (uint8_t []){0x33, 0x01, 0x0C, 0x13, 0x01, 0x01}, 6, 0},
    {0xB3, (uint8_t []){0x00, 0x30}, 2, 0},
    {0xF1, (uint8_t []){0x00}, 1, 0},
    {0x71, (uint8_t []){0xD0}, 1, 0},
    {0x66, (uint8_t []){0x02, 0x3F}, 2,  0},
    {0xBE, (uint8_t []){0x26, 0x00, 0x9D}, 3, 0},
    {0x70, (uint8_t []){0x01, 0xA0, 0x11, 0x40, 0xE0, 0x00, 0x11, 0x69, 0x11, 0x00, 0x00, 0x1A}, 12, 0},
    {0x90, (uint8_t []){0x04, 0x04, 0x55, 0x74, 0x00, 0x40, 0x43, 0x27, 0x27}, 9, 0},
    {0x91, (uint8_t []){0x04, 0x04, 0x55, 0x75, 0x00, 0x40, 0x42, 0x27, 0x27}, 9, 0},
    {0x92, (uint8_t []){0x04, 0x44, 0x55, 0xC0, 0x06, 0x00, 0x07, 0x05, 0x90, 0x27}, 10, 0},
    {0x93, (uint8_t []){0x04, 0x43, 0x11, 0x00, 0x00, 0x00, 0x00, 0x05, 0x90, 0x27}, 10, 0},
    {0x94, (uint8_t []){0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 6, 0},
    {0x95, (uint8_t []){0x96, 0x16, 0x00, 0x00, 0xFF}, 5, 0},
    {0x96, (uint8_t []){0x44, 0x53, 0x03, 0x12, 0x23, 0x24, 0x06, 0x05, 0x94, 0x27, 0x00, 0x44}, 12, 0},
    {0x97, (uint8_t []){0x44, 0x53, 0x47, 0x56, 0x20, 0x20, 0x02, 0x01, 0x94, 0x27, 0x00, 0x44}, 12, 0},
    {0xBA, (uint8_t []){0x55, 0x94, 0x2D, 0x94, 0x27}, 5, 0},
    {0x9A, (uint8_t []){0x40, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00}, 7, 0},
    {0x9B, (uint8_t []){0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00}, 7, 0},
    {0x9C, (uint8_t []){0x5C, 0x12, 0x00, 0x00, 0x10, 0x12, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00}, 13, 0},
    {0x9D, (uint8_t []){0x8A, 0x51, 0x00, 0x00, 0x00, 0x80, 0x1E, 0x01}, 8, 0},
    {0x9E, (uint8_t []){0x51, 0x00, 0x00, 0x00, 0x80, 0x1E, 0x01}, 7, 0},
    {0xB4, (uint8_t []){0x1D, 0x1C, 0x1E, 0x0B, 0x14, 0x02, 0x13, 0x09, 0x1E, 0x00, 0x1E, 0x10}, 12, 0},
    {0xB5, (uint8_t []){0x1D, 0x1C, 0x1E, 0x0A, 0x15, 0x03, 0x11, 0x08, 0x1E, 0x01, 0x1E, 0x12}, 12, 0},
    {0xB6, (uint8_t []){0x77, 0x77, 0x00, 0x0A, 0xFF, 0x0A, 0xFF}, 7, 0},
    {0x86, (uint8_t []){0xCD, 0x04, 0xB1, 0x02, 0x58, 0x12, 0x58, 0x0C, 0x13, 0x01, 0xA5, 0x00, 0xA5, 0xA5}, 14, 0},
    {0xB7, (uint8_t []){0x07, 0x0A, 0x0E, 0x06, 0x05, 0x03, 0x2B, 0x03, 0x03, 0x42, 0x07, 0x10, 0x10, 0x2E, 0x3F, 0x0D}, 16, 0},
    {0xB8, (uint8_t []){0x07, 0x0A, 0x0D, 0x05, 0x05, 0x02, 0x2B, 0x02, 0x03, 0x42, 0x06, 0x10, 0x0F, 0x2E, 0x3F, 0x0D}, 16, 0},
    {0xB9, (uint8_t []){0x23, 0x23}, 2, 0},
    {0xBF, (uint8_t []){0x10, 0x14, 0x14, 0x0B, 0x0B, 0x0B}, 6, 0},
    {0xF2, (uint8_t []){0x00}, 1, 0},
    {0x73, (uint8_t []){0x04, 0xDA, 0x12, 0x54, 0x47}, 5, 0},
    {0x77, (uint8_t []){0x6B, 0x5B, 0xFD, 0xC3, 0xC5}, 5, 0},
    {0x7A, (uint8_t []){0x15, 0x27}, 2, 0},
    {0x7B, (uint8_t []){0x04, 0x57}, 2, 0},
    {0x7E, (uint8_t []){0x01, 0x0E}, 2, 0},
    {0xBF, (uint8_t []){0x36}, 1, 0},
    {0xE3, (uint8_t []){0x40, 0x40}, 2, 0},
    {0xF0, (uint8_t []){0x00}, 1, 0},
    {0xD0, (uint8_t []){0x00}, 1, 0},
    {0x2A, (uint8_t []){0x00, 0x00, 0x01, 0x3F}, 4, 0},
    {0x2B, (uint8_t []){0x00, 0x00, 0x01, 0xDF}, 4, 0},
    {0x21, (uint8_t []){0x00}, 0, 0},
    {0x11, (uint8_t []){0x00}, 0, 120},
    {0x29, (uint8_t []){0x00}, 0, 0},
    {0x2C, (uint8_t []){0x00}, 0, 0},
    {0x3A, (uint8_t []){0x55}, 1, 0},
    {0x36, (uint8_t []){0x08}, 1, 0},
    {0x35, (uint8_t []){0x01}, 1, 20},
};


static spi_device_handle_t qspi;

ST77922::ST77922(void)
{
    width = LCD_WIDTH;
    height = LCD_HEIGHT;
    rotation = 0;
}

void ST77922::Begin(void)
{
    pinMode(TFT_QSPI_CS, OUTPUT);
    digitalWrite(TFT_QSPI_CS, HIGH);
#ifdef TFT_BL
     pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, LOW);
#endif
	const spi_bus_config_t buscfg = 
    {
        .data0_io_num = TFT_QSPI_D0,
        .data1_io_num = TFT_QSPI_D1,
        .sclk_io_num = TFT_QSPI_SCLK,
        .data2_io_num = TFT_QSPI_D2,
        .data3_io_num = TFT_QSPI_D3,
        .max_transfer_sz = (TX_LEN*16)+8,
        .flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_IOMUX_PINS |SPICOMMON_BUSFLAG_QUAD,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(TFT_QSPI_PORT, &buscfg, SPI_DMA_CH_AUTO));
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .mode = TFT_QSPI_MODE,
        .clock_speed_hz = TFT_QSPI_FREQUENCY,
        .spics_io_num = TFT_QSPI_CS,
        .flags = SPI_DEVICE_HALFDUPLEX ,
        .queue_size = 17,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(TFT_QSPI_PORT, &devcfg, &qspi));
    Init();
    Set_Rotation(0);
}

void ST77922::Write_Reg(uint32_t cmd, void *data, uint8_t len)
{
	LCD_CS_LOW; 
    spi_transaction_ext_t qspit;
    memset(&qspit, 0, sizeof(qspit));
    qspit.base.flags = (SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR);
    qspit.base.cmd = QSPI_1W_CMD;
    qspit.base.addr = cmd << 8;
    qspit.command_bits = 8;
    qspit.address_bits = 24;
    if(len != 0)
    {
        qspit.base.tx_buffer = data;
        qspit.base.length = 8 * len;
    }
    else
    {
        qspit.base.tx_buffer = NULL;
        qspit.base.length = 0;        
    }
    spi_device_polling_transmit(qspi, (spi_transaction_t *)&qspit);
    LCD_CS_HIGH;
}

void ST77922::Init(void)
{
	uint16_t i = 0;
    for(i=0; i<sizeof(st77922_lcd_init)/sizeof(lcd_init_cmd); i++)
    {
        Write_Reg(st77922_lcd_init[i].cmd, st77922_lcd_init[i].data, st77922_lcd_init[i].len);
        delay(st77922_lcd_init[i].delay_ms);
    }
#ifdef TFT_BL
	LCD_BL_HIGH;
#endif
}

void ST77922::Push_Pixels(const uint16_t* color, uint32_t count, bool swap_bytes)
{
    LCD_CS_LOW;
    while (count > 0) {
        uint32_t chunk = count > TX_LEN ? TX_LEN : count;
        uint16_t *buffer = (uint16_t *)color;
        if (swap_bytes) {
            static uint16_t swap_buffer[TX_LEN];
            for (uint32_t i = 0; i < chunk; ++i) {
                swap_buffer[i] = (uint16_t)(color[i] << 8 | color[i] >> 8);
            }
            buffer = swap_buffer;
        }
        spi_transaction_ext_t espit = {0};
        espit.base.flags = SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR;
        espit.base.cmd = QSPI_4W_CMD;
        espit.base.addr = WR_RAM_C_CMD << 8;
        espit.command_bits = 8;
        espit.address_bits = 24;
        espit.base.tx_buffer = buffer;
        espit.base.length = chunk * 16;
        spi_device_polling_transmit(qspi, (spi_transaction_t *)&espit);
        color += chunk;
        count -= chunk;
    }
    LCD_CS_HIGH;
}

void ST77922::Push_Image(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h,
                         const uint16_t* color, bool swap_bytes)
{
    if (w == 0 || h == 0) return;

    if (rotation == 1 || rotation == 3) {
        size_t count = (size_t)w * h;
        static uint16_t *rotated = nullptr;
        static size_t capacity = 0;

        if (count > capacity) {
            uint16_t *grown = (uint16_t *)realloc(rotated, count * sizeof(uint16_t));
            if (grown == nullptr) return;
            rotated = grown;
            capacity = count;
        }

        for (uint16_t y = 0; y < h; ++y) {
            for (uint16_t x = 0; x < w; ++x) {
                uint32_t destination;
                if (rotation == 1) destination = (uint32_t)x * h + (h - y - 1);
                else destination = (uint32_t)(w - x - 1) * h + y;
                rotated[destination] = color[(uint32_t)y * w + x];
            }
        }

        uint16_t physicalX = sy;
        uint16_t physicalY = sx;
        Set_Windows(physicalX, physicalY, physicalX + h, physicalY + w);
        Push_Pixels(rotated, count, swap_bytes);
        return;
    }

    Set_Windows(sx, sy, sx + w, sy + h);
    Push_Pixels(color, (uint32_t)w * h, swap_bytes);
}

void ST77922::Set_Rotation(uint8_t r)
{
	uint8_t value;
	rotation = r;
    switch(rotation)
    {
        case 0:
            value = 0x08;
			width = LCD_WIDTH;
            height = LCD_HEIGHT;
            break;
        case 1:
            value = MADCTL_MV | 0x08;
            width = LCD_HEIGHT;
            height = LCD_WIDTH;
            break;
        case 2:
            value = MADCTL_MY | 0x08;
            width = LCD_WIDTH;
            height = LCD_HEIGHT;
            break;
        case 3:
            value = MADCTL_MX | MADCTL_MY | MADCTL_MV | 0x08;
			width = LCD_HEIGHT;
            height = LCD_WIDTH;
			break;
        default:
            break;
    }
    Write_Reg(MADCTL_CMD, &value, 1);
}

uint8_t ST77922::Get_Rotation(void)
{
	return rotation;
}

void ST77922::Set_Windows(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
	uint8_t x_data[] = {
        (uint8_t)(sx >> 8), 
        (uint8_t)(sx & 0xFF), 
        (uint8_t)((ex - 1) >> 8), 
        (uint8_t)((ex - 1) & 0xFF)
    };
    Write_Reg(SET_X_CMD, x_data, 4);

    // 修改 Y 坐标部分
    uint8_t y_data[] = {
        (uint8_t)(sy >> 8), 
        (uint8_t)(sy & 0xFF), 
        (uint8_t)((ey - 1) >> 8), 
        (uint8_t)((ey - 1) & 0xFF)
    };
    Write_Reg(SET_Y_CMD, y_data, 4);
}

#define FILL_COLORS_TILE 16   /* try 8/16/32 - larger can help or hurt
                                  depending on cache size; 16 is a safe
                                  general-purpose starting point */
 
void ST77922::Fill_Colors(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h, uint16_t* color)
{
	bool flag = true;
    size_t tx_len;
	uint16_t* tx_buf = nullptr;
    uint16_t i, j, tmp;
    spi_transaction_ext_t espit = {0};
	size_t total = 0;
 
    /* Persistent rotation scratch buffer - function-local static, so it
     * survives across calls without needing a class member / header
     * change. Grows if a bigger request ever comes in; never freed. */
    static uint16_t *cbuf = nullptr;
    static size_t cbuf_capacity = 0;
 
    if((sx >= width) || (sy >= height))
        return;
    if(((sx + w) > width) || ((sy + h) > height))
        return;
    if(((w < 1) || (w > width)) || ((h < 1) || (h > height)))
        return;
 
	if((rotation == 1)||(rotation == 3))
    {
        size_t needed = (size_t)w * (size_t)h;
        if (needed > cbuf_capacity)
        {
            uint16_t *grown = (uint16_t *)ps_malloc(sizeof(uint16_t) * needed);
            if (grown == nullptr)
            {
                return;   /* allocation failed - old buffer (if any) is
                             left intact and untouched */
            }
            if (cbuf != nullptr) {
                free(cbuf);
            }
            cbuf = grown;
            cbuf_capacity = needed;
        }
 
        /* Cache-blocked transpose - same math as before, just processed
         * in TILE x TILE chunks instead of one full sweep. i, j are
         * LOCAL (0-based) indices within the w x h region. */
        uint16_t bi, bj, i_end, j_end;
        for (bi = 0; bi < h; bi += FILL_COLORS_TILE)
        {
            i_end = (bi + FILL_COLORS_TILE < h) ? (bi + FILL_COLORS_TILE) : h;
            for (bj = 0; bj < w; bj += FILL_COLORS_TILE)
            {
                j_end = (bj + FILL_COLORS_TILE < w) ? (bj + FILL_COLORS_TILE) : w;
                for (i = bi; i < i_end; i++)
                {
                    for (j = bj; j < j_end; j++)
                    {
                        if (rotation == 1)
                        {
                            *(cbuf + j*h + (h-i-1)) = *(color + i*w + j);
                        }
                        else
                        {
                            *(cbuf + (w - j -1)*h + i) = *(color + i*w + j);
                        }
                    }
                }
            }
        }
 
		tx_buf = cbuf;
        tmp = sx;
        sx = sy;
        sy = tmp;
        tmp = w;
        w = h;
        h = tmp;
    }
    else
    {
        tx_buf = color;
    }
	total = w*h;
	Set_Windows(sx, sy, sx + w, sy + h);    
    LCD_CS_LOW;
     do
     {
         if(flag)
        {
            espit.base.flags = (SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR);
            espit.base.cmd = QSPI_4W_CMD;
            espit.base.addr = WR_RAM_C_CMD << 8;
            espit.command_bits = 8;
            espit.address_bits = 24;
            flag = false;
        }
        else
        {
            espit.base.flags = (SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY);
        }
        tx_len = (total>TX_LEN)?TX_LEN:total;
        espit.base.tx_buffer = tx_buf;
        espit.base.length = tx_len * 16;
        spi_device_polling_transmit(qspi, (spi_transaction_t *)&espit);
        total -= tx_len;
        tx_buf += tx_len;
  }while(total>0);
  LCD_CS_HIGH;
  /* NOTE: cbuf is intentionally NOT freed here anymore - it's reused on
   * the next call. This is expected and correct, not a leak. */
}

//does not work
void ST77922::Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x >= width) || (y >= height))
        return;
 
    if ((rotation == 1) || (rotation == 3))
    {
        /* Same rect-swap Fill_Colors applies before its Set_Windows() call */
        Set_Windows(y, x, y + 1, x + 1);
    }
    else
    {
        Set_Windows(x, y, x + 1, y + 1);
    }
 
    /* RGB565 -> BGR565 (swap the 5-bit R and B fields, leave 6-bit G alone) */
    uint16_t bgr = (uint16_t)(
        (color & 0x07E0) |
        ((color & 0xF800) >> 11) |
        ((color & 0x001F) << 11)
    );
    /* then byte-order swap for transmit, same as before.
     *
     * DIAGNOSTIC CHANGE: `static` instead of a plain stack-local variable.
     * Fill_Colors always transmits from heap-allocated memory (ps_malloc
     * or caller-provided heap buffers) for its QIO transfers; this is the
     * only place that ever tried to QIO-transmit from a stack address.
     * If that turns out to matter, `static` (BSS/data section, not stack)
     * rules it out cheaply while we find out. */
    static uint16_t pixel;
    pixel = (uint16_t)((bgr >> 8) | (bgr << 8));
 
    LCD_CS_LOW;
 
    spi_transaction_ext_t espit = {0};
 
    espit.base.flags =
        SPI_TRANS_MODE_QIO |
        SPI_TRANS_VARIABLE_CMD |
        SPI_TRANS_VARIABLE_ADDR;
 
    espit.base.cmd = QSPI_4W_CMD;
    espit.base.addr = WR_RAM_C_CMD << 8;
 
    espit.command_bits = 8;
    espit.address_bits = 24;
 
    espit.base.tx_buffer = &pixel;
    espit.base.length = 16;
 
    /* DIAGNOSTIC CHANGE: actually check the result. Neither Draw_Pixel nor
     * Fill_Colors checked this before - if the transaction is failing at
     * the driver level, this is how we'll know instead of guessing. */
    esp_err_t err = spi_device_polling_transmit(
        qspi,
        (spi_transaction_t *)&espit
    );
    if (err != ESP_OK) {
        Serial.printf("Draw_Pixel: spi_device_polling_transmit FAILED: %s (x=%u y=%u)\n",
                      esp_err_to_name(err), (unsigned)x, (unsigned)y);
    }
 
    LCD_CS_HIGH;
}
uint16_t ST77922::Get_Width(void)
{
	return width;
}

uint16_t ST77922::Get_Height(void)
{
	return height;
}

#endif


