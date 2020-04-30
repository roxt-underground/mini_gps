#include <Adafruit_SSD1306.h>
#include "display.h"

void draw_speed(Adafruit_SSD1306 *display, float speed) {
  display->clearDisplay();

  display->setTextSize(2);             // Normal 1:1 pixel scale
  display->setTextColor(SSD1306_WHITE);        // Draw white text
  display->setCursor(2,2);             // Start at top-left corner
  display->cp437(true);                // Use full 256 char 'Code Page 437' font
  display->print(speed);
  display->setTextSize(1);
  display->println(F("km/h"));
  short a;
  if      (speed < 15.0)  a = 0;
  else if (speed < 35.0)  a = 1;
  else if (speed < 55.0)  a = 2;
  else if (speed < 75.0)  a = 3;
  else if (speed < 95.0)  a = 4;
  else if (speed < 105.0) a = 5;
  else if (speed < 115.0) a = 5;
  else if (speed < 120.0) a = 6;
  else if (speed < 125.0) a = 7;
  else                    a = 8;

  for (short i=0; i<a; i++){
    display->drawBitmap(
    BAR_WIDTH * i,
    (display->height() - BAR_HEIGH),
    bar_bmp, BAR_WIDTH, BAR_HEIGH, 1);
  }

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

#ifdef LOGOTEST

void test_speed(Adafruit_SSD1306 *display) {
  float speed_range[] = {
     0.00, 12.45, 23.01, 33.97, 48.16, 69.12, 83.27, 97.31, 112.12, 119.36, 122.51, 127.84, 139.62,
  };
  draw_logo(display);
  delay(1000);
  for (short i; i<13; i++) {
    draw_speed(display, speed_range[i]);
    delay(1000);
  }
  
}

#endif
