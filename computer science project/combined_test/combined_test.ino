#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>              // Arduino SPI library

// ST7789 TFT module connections
#define TFT_CS 10  // define chip select pin
#define TFT_DC 9   // define data/command pin
#define TFT_RST 8  // define reset pin, or set to -1 and connect to Arduino RESET pin
#include <Wire.h>

// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const int MPU_addr = 0x68;

int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

//start coordinates
//original coordinates
double x1O = 60;
double y1O = 120;
//new coordinates
double x1N;
double y1N;

//end coordinates
//original coordinates
double x2O = 180;
double y2O = 120;
//new coordinates
double x2N;
double y2N;

double A = 342.9015691;

double x;
double y;
double z;

//String G = "ST77XX_GREEN";

void setup(void) {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  Serial.begin(9600);
  Serial.print(F("ST77xx TFT Test"));
  // if the display has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE2);  // Init ST7789 display 240x240 pixel
  // adjusting screen orientation
  tft.setRotation(2);
  // notify of program started
  Serial.println(F("Initialized"));
  // notify of finsihed program
  Serial.println("done");
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);

  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();

  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  tft.fillScreen(ST77XX_BLACK);
  tft.drawCircle(120, 120, 5, ST77XX_GREEN);

  y = 15;
  //y = y + 1;
  x = 60;
  //x = x + 1;

  yaxis(y1O, y1N, A, y);  //move line up or down
  yaxis(y2O, y2N, A, y);
  AngleReset(y1N);  //keep line within bounds
  AngleReset(y2N);
  xaxis(x1O, y1N, x1F, y1F, x);  //rotate line around the origin
  xaxis(x2O, y2N, x2F, y2F, x);
  screen(x1F, y1F, x2F, y2F);  //draw line on screen

  tft.setCursor(0, 0);
  Serial.println("O");
  Serial.println(x1O);
  Serial.println(y1O);
  Serial.println("");
  Serial.println("N");
  Serial.println(y1N);
  Serial.println("");
  Serial.println("F");
  Serial.println(x1F);
  Serial.println(y1F);-
  Serial.println("");
  Serial.println(x2F);
  Serial.println(y2F);

  //tft.println("A");
  //tft.println(x);
  //tft.println(y);
  delay(1000);
}

double yaxis(double y1O, double &y1N, double y) {
  y1N = y1O + (y * 6 );
}

double AngleReset(double &y1N) {
  if (180 < y1N) {
    y1N = y1N - (int((y1N - 60) / 120) * 120);
  }
  if (y1N < 60) {
    y1N = y1N + ((int(abs(y1N - 60) / 120) + 1) * 120);
  }
}

double xaxis(double xO, double &yN, double &xN, double x) {  //& = refrences value
  xO = xO - 120;
  yN = yN - 120;
  xN = 120 + (sqrt((xO * xO) + (yN * yN)) * sin(((x + ((atan((xO) / (y1N))) / M_PI) * 180) / 180) * M_PI));
  yN = 120 + (sqrt((xO * xO) + (yN * yN)) * cos(((x + ((atan((xO) / (y1N))) / M_PI) * 180) / 180) * M_PI));
}

double screen(double x1N, double y1N, double x2N, double y2N) {
  tft.drawLine(x1N, y1N, x2N, y2N, ST77XX_GREEN);
}