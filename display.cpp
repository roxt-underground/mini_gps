#include <Adafruit_SSD1306.h>
#include "display.h"

const float speed_ranges[] = {15.0, 35.0, 55.0, 75.0, 95.0, 105.0, 115.0, 120.0, 125.0};

void draw_speed(Adafruit_SSD1306 *display, float speed, short hour, byte minute) {
  display->clearDisplay();

  display->setTextSize(2);             // Normal 1:1 pixel scale
  display->setTextColor(SSD1306_WHITE);        // Draw white text
  display->setCursor(2,2);             // Start at top-left corner
  display->cp437(true);                // Use full 256 char 'Code Page 437' font
  display->print(speed);
  display->setTextSize(1);
  display->println(F("km/h"));

  short a = 0;
  for (short i=0; i<9; i++) {
    if (speed > speed_ranges[i]) a++;
    else break; 
  }

  for (short i=0; i<a; i++){
    display->drawBitmap(
    BAR_WIDTH * i,
    (display->height() - BAR_HEIGH),
    bar_bmp, BAR_WIDTH, BAR_HEIGH, 1);
  }
  if (hour >= 0) draw_time(display, hour, minute);
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

void draw_time(Adafruit_SSD1306 *display, short hour, byte minute){
  char tm[8];
  display->setTextSize(1);
  display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display->setCursor(93,12);
  sprintf(tm, "%02d:%02d", hour, minute);
  display->print(tm);
}

#ifdef LOGOTEST

void test_speed(Adafruit_SSD1306 *display) {
  float speed_range[] = {
     0.00, 12.45, 23.01, 33.97, 48.16, 69.12, 83.27, 97.31, 112.12, 119.36, 122.51, 127.84, 139.62,
  };
  draw_logo(display);
  delay(500);
  for (short i; i<13; i++) {
    if (i < 5)
      draw_speed(display, speed_range[i]);
    else
      draw_speed(display, speed_range[i], i+2, (i*13) % 60);
    delay(700);
  }
  
}

#endif
