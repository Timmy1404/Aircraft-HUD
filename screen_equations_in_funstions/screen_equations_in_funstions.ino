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

const int MPU_addr=0x68;  

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;  

int minVal=265;  
int maxVal=402;  

//start coordinates  
//original coordinates  
double x1O = 60-120; //60  
double y1O = 116-120; //116  
//new coordinates  
double x1N = 0;  
double y1N = 0;  
//final coordinates  
double x1F = 0;  
double y1F = 0;  

//end coordinates  
//original coordinates  
double x2O = 180-120;   
double y2O = 116-120;  
//new coordinates  
double x2N = 0;  
double y2N = 0;  
//final coordinates  
double x2F = 0;  
double y2F = 0;  

double x;  
double y;  
double z;  

double ax = 0;  
double ay = 0; 
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
  ax = x;  
  ay = y;  

  tft.fillScreen(ST77XX_BLACK);  
  tft.drawCircle(120,120,5,ST77XX_GREEN);  
  AngleOfAttack(x1O, y1O, x1N, y1N, ay); 
  AngleOfAttack1(x1N, y1N, x1F, y1F, ax);  
  AngleOfAttack1(x2N, y2N, x2F, y2F, ax); 
  AngleOfAttack2(x1F, y1F, x2F, y2F);  

  tft.setCursor(0, 0);  
  //tft.println(x1F);  
  //tft.println(y1F);  
  //tft.println(x2F);  
  //tft.println(y2F);  
  //delay(1000);  
}  

double AngleOfAttack(double x1N, double y1N, double &x1F, double &y1F, double ay) {  
  y1F = y1N + (tan(ay) * (5)); //5cm ish noidea how many pixels )  
  y2F = y2N + (tan(ay) * (5)); //5cm ish noidea how many pixels )  
}  

double AngleOfAttack1(double x1N, double y1N, double &x1F, double &y1F, double ax) {  //& = refrences value  
x1N = 120-(sqrt((x1N*x1N) + (y1N*y1N)) * sin(((ax + ((atan((x1N) / (y1N)))/M_PI)*180)/180)*M_PI));  
y1N = 120-(sqrt((x1N*x1N) + (y1N*y1N)) * cos(((ax + ((atan((x1N) / (y1N)))/M_PI)*180)/180)*M_PI));   
}  

double AngleOfAttack2(double x1F, double y1F, double x2F, double y2F) {  
  tft.drawLine(x1F,y1F,x2F,y2F,ST77XX_GREEN); //(0)  
  //tft.drawLine(130,116,180,116,ST77XX_GREEN); //(0)  
  //tft.drawCircle(120,120,5,ST77XX_GREEN); //gun sight  
}  

 