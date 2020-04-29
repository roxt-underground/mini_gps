#include <Adafruit_SSD1306.h>
#include "display.h"

void draw_speed(Adafruit_SSD1306 *display, float speed) {
  display->clearDisplay();

  display->setTextSize(3);             // Normal 1:1 pixel scale
  display->setTextColor(SSD1306_WHITE);        // Draw white text
  display->setCursor(2,2);             // Start at top-left corner
  display->cp437(true);                // Use full 256 char 'Code Page 437' font
  display->print(speed);
  display->setTextSize(3);
  display->println(F("km/h"));

  display->display();
}

void draw_logo(Adafruit_SSD1306 *display) {
  display->clearDisplay();

  display->drawBitmap(
    (display->width()  - LOGO_WIDTH ) / 2,
    (display->height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display->display();
}
