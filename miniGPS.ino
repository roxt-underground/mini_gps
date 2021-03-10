// display libs
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"

// GPS libs
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define OLED_RESET    4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// GPS env
TinyGPS gps;
SoftwareSerial ss(6, 5);
unsigned long last_data_ts;
const short timezone = 3;

void log_cords(TinyGPS *_gps);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  ss.begin(9600);
  last_data_ts = millis();

  display.clearDisplay();
  display.display();
  draw_logo(&display);
  delay(1200);
  
#ifdef LOGOTEST
  test_speed(&display);
  delay(2000);
#endif

}

void loop() {
  // put your main code here, to run repeatedly:
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
  if (newData)
  {
    log_cords(&gps);
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
    draw_speed(&display, gps.f_speed_kmph(), (hour+timezone) % 24, minute);
    last_data_ts = millis();
  }
  else {
    if (millis() - last_data_ts > 5000) draw_logo(&display);
  }
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}

void log_cords(TinyGPS *_gps) {
    float flat, flon;
    unsigned long age;
    _gps->f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(_gps->satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : _gps->satellites());
    Serial.print(" PREC=");
    Serial.print(_gps->hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : _gps->hdop());
}
