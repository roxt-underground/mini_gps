// display libs
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// GPS libs
#include <SoftwareSerial.h>
#include <TinyGPS.h>

// display env
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET    4 // Reset pin # (or -1 if sharing Arduino reset pin)


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   32
#define LOGO_WIDTH    48
static const unsigned char PROGMEM logo_bmp[] = 
{
B00000000, B00000000, B00001111, B11111000, B00000000, B00000000,
B00000000, B00000000, B11111111, B11111111, B00000000, B00000000,
B00000000, B00000111, B11111111, B11111111, B00100000, B00000000,
B00000000, B00011111, B10000000, B11111111, B01111000, B00000000,
B00000000, B01111100, B00000001, B11111110, B00111110, B00000000,
B00000000, B11110000, B00000001, B11111100, B00001111, B00000000,
B00000011, B11100000, B00000011, B11111000, B00000111, B10000000,
B00000011, B11000000, B00000111, B11111000, B00000011, B11000000,
B00000111, B10000000, B00001111, B11110000, B00000001, B11100000,
B00001111, B00000000, B00001111, B11100000, B00000000, B11110000,
B00001110, B00000000, B00011111, B11000000, B00000000, B01110000,
B00011110, B00000000, B00111111, B11000000, B00000000, B01111000,
B00011100, B00000000, B01111111, B10000000, B00000000, B01111000,
B00011100, B00000000, B01111111, B00000000, B00000000, B00111000,
B00111100, B00000000, B11111111, B00000000, B00000000, B00111100,
B00111100, B00000001, B11111110, B00000000, B00000000, B00111100,
B00111100, B00000001, B11111100, B00000000, B00000000, B00111100,
B00111100, B00000011, B11111100, B00000000, B00000000, B00111100,
B00011100, B00000011, B11111000, B00000000, B00000000, B00111000,
B00011110, B00000011, B11111000, B00000000, B00000000, B01111000,
B00011110, B00000011, B11111100, B00000000, B00000000, B11110000,
B00011110, B00000011, B11111111, B11111111, B11111111, B11110000,
B00001111, B00000001, B11111111, B11111111, B11111111, B11100000,
B00000111, B10000000, B11111111, B11111111, B11111111, B11100000,
B00000111, B11000000, B00000000, B00000000, B00000000, B00000000,
B00000011, B11100000, B00000000, B00000000, B00000111, B00000000,
B00000001, B11110000, B00000000, B00000000, B00011111, B00000000,
B00000000, B01111100, B00000000, B00000000, B01111110, B00000000,
B00000000, B00111111, B10000000, B00000001, B11111000, B00000000,
B00000000, B00001111, B11111100, B00111111, B11100000, B00000000,
B00000000, B00000001, B11111111, B11111111, B00000000, B00000000,
B00000000, B00000000, B00001111, B11110000, B00000000, B00000000,
};

// GPS env
TinyGPS gps;
SoftwareSerial ss(6, 5);
unsigned long last_data_ts;


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
  
  display.display();
  draw_logo();
  delay(1200);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
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
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    draw_speed(gps.f_speed_kmph());
    last_data_ts = millis();
  }
  else {
    if (millis() - last_data_ts > 5000) draw_logo();
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

void draw_logo() {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void draw_speed(float speed) {
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(5,5);             // Start at top-left corner
  display.cp437(true);                // Use full 256 char 'Code Page 437' font
  display.print(speed);
  display.println(F("km/h"));

  display.display();
}
