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

//start coordinates
//original coordinates
float x1O = 60-120; //60
float y1O = 116-120; //116
//new coordinates
float x1N = 0;
float y1N = 0;

//end coordinates
//original coordinates
float x2O = 180-120; 
float y2O = 116-120;
//new coordinates
float x2N = 0;
float y2N = 0;

float a = 0;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("ST77xx TFT Test"));
  // if the display has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE2);    // Init ST7789 display 240x240 pixel
  // adjusting screen orientation
  tft.setRotation(2);
  // notify of program started
  Serial.println(F("Initialized"));
  // notify of finsihed program
  Serial.println("done");
  




}

void loop() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawCircle(120,120,5,ST77XX_GREEN);
  //AngleOfAttack(x1O, y1O, x1N, y1N, a);
  //AngleOfAttack1(x1N, y1N, x1N, y1N);
  //tft.drawLine(60,60,120,120,ST77XX_GREEN);
  x1N = (sqrt((x1O*x1O) + (y1O*y1O)) * sin(((a + ((atan((x1O) / (y1O)))/M_PI)*180)/180)*M_PI))+120;
  y1N = (sqrt((x1O*x1O) + (y1O*y1O)) * cos(((a + ((atan((x1O) / (y1O)))/M_PI)*180)/180)*M_PI))+120;
  tft.println(x1N);
  tft.println(y1N);
  x2N = (sqrt((x2O*x2O) + (y2O*y2O)) * sin(((a + ((atan((x2O) / (y2O)))/M_PI)*180)/180)*M_PI))+120;
  y2N = (sqrt((x2O*x2O) + (y2O*y2O)) * cos(((a + ((atan((x2O) / (y2O)))/M_PI)*180)/180)*M_PI))+120;
  tft.println(x2N);
  tft.println(y2N);
  tft.drawLine(x1N,y1N,x2N,y2N,ST77XX_GREEN); //(0)
  //a = a - 10;

  tft.setCursor(0, 0);
  delay(1000);
}

void AngleOfAttack(float x1O, float y1O, float x1N, float y1N, float a) {
//x1N = (sqrt((x1O*x1O) + (y1O*y1O)) * sin(((a + ((atan((x1O) / (y1O)))/M_PI)*180)/180)*M_PI))+120;
//y1N = (sqrt((x1O*x1O) + (y1O*y1O)) * cos(((a + ((atan((x1O) / (y1O)))/M_PI)*180)/180)*M_PI))+120;
}

void AngleOfAttack1(float x1N, float y1N, float x2N, float y2N) {
  tft.drawLine(x1N,y1N,x2N,y2N,ST77XX_GREEN); //(0)
  //tft.drawLine(130,116,180,116,ST77XX_GREEN); //(0)
  tft.drawCircle(120,120,5,ST77XX_GREEN); //gun sight
}