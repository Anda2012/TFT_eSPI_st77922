/*
  ST77922 QSPI touch drawing example.

  Select User_Setups/Setup_ST77922_QSPI.h in User_Setup_Select.h and adjust
  the wiring macros there for the target board.
*/

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <TFT_Drivers/ST77922/ST77922_Touch.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite canvas = TFT_eSprite(&tft);
ST77922_TOUCH touch;

const uint8_t paletteSize = 30;
const uint8_t lineSize = 3;
const uint8_t colorCount = 12;
const uint8_t screenRotation = 1;

const uint16_t colors[colorCount] = {
  TFT_RED, TFT_PINK, TFT_GREEN, TFT_BLUE, TFT_OLIVE, TFT_CYAN,
  TFT_YELLOW, TFT_WHITE, TFT_MAGENTA, TFT_ORANGE, TFT_BLACK, TFT_SILVER
};

uint16_t currentColor = TFT_WHITE;
uint16_t lastX = 0;
uint16_t lastY = 0;

void presentCanvas()
{
  tft.pushImage(0, 0, canvas.width(), canvas.height(), (uint16_t *)canvas.getPointer());
}

void drawUi()
{
  for (uint8_t i = 0; i < colorCount; ++i) {
    canvas.fillRect(i * paletteSize, 0, paletteSize, paletteSize, colors[i]);
  }

  canvas.setTextColor(TFT_WHITE, TFT_BLACK);
  canvas.drawString("CLEAR", colorCount * paletteSize + 10, 8, 2);
  canvas.drawRect(canvas.width() - 40, 5, 20, 20, TFT_WHITE);
  canvas.fillRect(canvas.width() - 39, 6, 18, 18, currentColor);
  canvas.drawFastHLine(0, paletteSize, canvas.width(), TFT_DARKGREY);
}

void setup()
{
  Serial.begin(115200);

  tft.init();
  tft.setRotation(screenRotation);

  canvas.createSprite(tft.width(), tft.height());
  canvas.setSwapBytes(true);
  canvas.fillSprite(TFT_BLACK);

  touch.init();
  touch.Set_Rotation(screenRotation);

  drawUi();
  presentCanvas();
}

void loop()
{
  if (!touch.Get_Touch()) {
    lastX = 0;
    lastY = 0;
    return;
  }

  uint16_t x = touch.touch.x[0];
  uint16_t y = touch.touch.y[0];

  if (y < paletteSize + 3) {
    lastX = 0;
    lastY = 0;

    if (x >= colorCount * paletteSize) {
      canvas.fillRect(0, paletteSize + 1, canvas.width(), canvas.height() - paletteSize - 1, currentColor);
    } else {
      currentColor = colors[x / paletteSize];
      canvas.fillRect(canvas.width() - 39, 6, 18, 18, currentColor);
    }

    presentCanvas();
    return;
  }

  if (lastX == 0 && lastY == 0) {
    lastX = x;
    lastY = y;
  }

  canvas.drawWideLine(lastX, lastY, x, y, lineSize, currentColor, currentColor);
  presentCanvas();
  lastX = x;
  lastY = y;
}
