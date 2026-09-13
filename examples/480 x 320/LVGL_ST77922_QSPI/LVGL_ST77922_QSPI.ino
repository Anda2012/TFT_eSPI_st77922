/*
 * LVGL v8 on ST77922 QSPI with SquareLine Studio output.
 * Portrait mode uses rotation 0 and small LVGL draw buffers.
 *
 * Put the SquareLine export in the lvgl/ subfolder beside this sketch and
 * select User_Setups/Setup_ST77922_QSPI.h in User_Setup_Select.h.
 */

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "esp_heap_caps.h"
#include <TFT_Drivers/ST77922/ST77922_Touch.h>
#include "lvgl/ui.h"

#if LV_COLOR_DEPTH != 16
  #error "LV_COLOR_DEPTH must be 16 for this example"
#endif

#define SCREEN_ROTATION 0
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480
#define BUF_LINES 40

TFT_eSPI tft = TFT_eSPI();
ST77922_TOUCH touch;

static lv_disp_draw_buf_t drawBuffer;
static lv_color_t lvBuffer1[SCREEN_WIDTH * BUF_LINES];
static lv_color_t lvBuffer2[SCREEN_WIDTH * BUF_LINES];
static uint16_t *composeBuffer;

static uint16_t swapRedBlue(uint16_t value)
{
  return (uint16_t)((value & 0x07E0) |
                    ((value & 0xF800) >> 11) |
                    ((value & 0x001F) << 11));
}

static void flushDisplay(lv_disp_drv_t *display, const lv_area_t *area, lv_color_t *color)
{
  const uint32_t width = (uint32_t)(area->x2 - area->x1 + 1);
  const uint32_t height = (uint32_t)(area->y2 - area->y1 + 1);
  const uint16_t *source = (const uint16_t *)color;

  for (uint32_t row = 0; row < height; ++row) {
    uint32_t destination = (uint32_t)(area->y1 + row) * SCREEN_WIDTH + area->x1;
    for (uint32_t column = 0; column < width; ++column) {
      uint16_t value = swapRedBlue(source[row * width + column]);
      composeBuffer[destination + column] = __builtin_bswap16(value);
    }
  }

  if (lv_disp_flush_is_last(display)) {
    tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, composeBuffer);
  }

  lv_disp_flush_ready(display);
}

static void readTouch(lv_indev_drv_t *input, lv_indev_data_t *data)
{
  if (touch.Get_Touch()) {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touch.touch.x[0];
    data->point.y = touch.touch.y[0];
  } else {
    data->state = LV_INDEV_STATE_REL;
  }

  (void)input;
}

static void lvglTick(void *argument)
{
  (void)argument;
  lv_tick_inc(1);
}

void setup()
{
  Serial.begin(115200);

  lv_init();

  tft.init();
  tft.setRotation(SCREEN_ROTATION);
  tft.setSwapBytes(false);

  if (tft.width() != SCREEN_WIDTH || tft.height() != SCREEN_HEIGHT) {
    Serial.println("FATAL: ST77922 rotation is not 320x480 portrait");
    while (true) delay(1000);
  }

  composeBuffer = (uint16_t *)heap_caps_malloc(
      (size_t)SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint16_t),
      MALLOC_CAP_SPIRAM
  );
  if (composeBuffer == NULL) {
    Serial.println("WARNING: PSRAM allocation failed; using internal RAM");
    composeBuffer = (uint16_t *)heap_caps_malloc(
        (size_t)SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint16_t),
        MALLOC_CAP_8BIT
    );
  }
  if (composeBuffer == NULL) {
    Serial.println("FATAL: unable to allocate LVGL compose buffer");
    while (true) delay(1000);
  }

  memset(composeBuffer, 0, (size_t)SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint16_t));

  lv_disp_draw_buf_init(
      &drawBuffer,
      lvBuffer1,
      lvBuffer2,
      SCREEN_WIDTH * BUF_LINES
  );

  static lv_disp_drv_t displayDriver;
  lv_disp_drv_init(&displayDriver);
  displayDriver.hor_res = SCREEN_WIDTH;
  displayDriver.ver_res = SCREEN_HEIGHT;
  displayDriver.flush_cb = flushDisplay;
  displayDriver.draw_buf = &drawBuffer;
  lv_disp_drv_register(&displayDriver);

  touch.init();
  touch.Set_Rotation(SCREEN_ROTATION);

  static lv_indev_drv_t inputDriver;
  lv_indev_drv_init(&inputDriver);
  inputDriver.type = LV_INDEV_TYPE_POINTER;
  inputDriver.read_cb = readTouch;
  lv_indev_drv_register(&inputDriver);

  const esp_timer_create_args_t timerArguments = {
    .callback = &lvglTick,
    .name = "lvgl_tick"
  };
  esp_timer_handle_t timer;
  esp_timer_create(&timerArguments, &timer);
  esp_timer_start_periodic(timer, 1000);

  ui_init();
}

void loop()
{
  lv_timer_handler();
  delay(5);
}
