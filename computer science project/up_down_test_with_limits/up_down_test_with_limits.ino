#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library

// ST7789 TFT module connections
#define TFT_CS    10  // define chip select pin
#define TFT_DC     9  // define data/command pin
#define TFT_RST    8  // define reset pin, or set to -1 and connect to Arduino RESET pin
#include <Wire.h>

// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
//String G = "ST77XX_GREEN";


const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;
 
double x;
double y;
double z;

//start coordinates
//original coordinates
double x1O = 60; //60
double y1O = 120; //116
//new coordinates
double x1N = 0;
double y1N = 0;
//final coordinates  
double x1F = 0;  
double y1F = 0;  

//end coordinates
//original coordinates
double x2O = 180; 
double y2O = 120;
//new coordinates
double x2N = 0;
double y2N = 0;
//final coordinates  
double x2F = 0;  
double y2F = 0;  


double a = 0;
double A = 342.9015691;

void setup(void) {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
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
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);
 
  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
 
  Serial.print("AngleX= "); // bank angle
  Serial.print(x);
  Serial.print("AngleY= "); // pitch angle
  Serial.print(y);
  //Serial.print("AngleZ= ");
  //Serial.print(z);
  Serial.println("");

  tft.fillScreen(ST77XX_BLACK);
  tft.drawLine(0,60,240,60,ST77XX_RED);
  tft.drawLine(0,180,240,180,ST77XX_RED);
  //tft.drawCircle(120,120,5,ST77XX_GREEN);



  AngleOfAttack(y1O, y1N, A, y);
  AngleOfAttack(y2O, y2N, A, y);
  //AngleOfAttack(y1O, y1N, A, y);
  //AngleOfAttack(y2O, y2N, A, y);
  //AngleOfAttack(y1O, y1N, A, y);
  //AngleOfAttack(y2O, y2N, A, y);
  //AngleOfAttack(y1O, y1N, A, y);
  //AngleOfAttack(y2O, y2N, A, y);

  AngleReset(y1N);
  AngleReset(y2N);
  //AngleReset(y1N);
  //AngleReset(y2N);
  //AngleReset(y1N);
  //AngleReset(y2N);
  //AngleReset(y1N);
  //AngleReset(y2N);


  AngleOfAttack2(x1N, y1N, x2N, y2N);
  //AngleOfAttack2(x1N, y1N, x2N, y2N);
  //AngleOfAttack2(x1N, y1N, x2N, y2N);
  //AngleOfAttack2(x1N, y1N, x2N, y2N);

  //tft.setCursor(0, 0);
  //tft.println(y);
  //delay(1000);
}

double AngleReset(double &y1N) {
  if (180 < y1N) {
    y1N = y1N - (int((y1N - 60) / 120) * 120);           
  }
  if (y1N < 60) {
    y1N = y1N + ((int(abs(y1N - 60) / 120)+1) * 120);              //<<<<-------------- this shit right here so annoying, had to adapt because of negatives, gay
  }
}

double AngleOfAttack(double y1O, double &y1N, double A, double y) {
  y1N = y1O - (A * tan((y/180)*M_PI));
}

double AngleOfAttack2(double x1N, double y1N, double x2N, double y2N) {
  tft.drawLine(x1O,y1N,x2O,y2N,ST77XX_GREEN); //(0)
}

