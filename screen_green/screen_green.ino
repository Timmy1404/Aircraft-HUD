#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library

// ST7789 TFT module connections
#define TFT_CS    10  // define chip select pin
#define TFT_DC     9  // define data/command pin
#define TFT_RST    8  // define reset pin, or set to -1 and connect to Arduino RESET pin

// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
String G = "ST77XX_GREEN";

int x1 = 60;
int y1 = 116;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("ST77xx TFT Test"));
  // if the display has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE2);    // Init ST7789 display 240x240 pixel
  // screen orientation
  tft.setRotation(2);
  // program started
  Serial.println(F("Initialized"));

  tft.fillScreen(ST77XX_BLACK);

  Serial.println("done");
}




void loop() {
 tft.fillScreen(ST77XX_GREEN);
}