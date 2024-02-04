#include <Adafruit_GFX.h>     
#include <Adafruit_ST7789.h>  
#include <SPI.h>              
#include <Wire.h>            

#define TFT_CS 10  
#define TFT_DC 9   
#define TFT_RST 8  

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const int MPU_addr = 0x68;

int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
double x, y;
int z;
int minVal = 265;
int maxVal = 402;

double xO, yO, xN, yN;
const int ROWS = 8;
const int COLUMNS = 4;
int Coordinates[ROWS][COLUMNS] = { 
  {60, 125, 0, 0},
  {180, 125, 0, 0},
  {60, 120+(1*(4*15)), 0, 0},
  {180, 120+(1*(4*15)), 0, 0},
  {60, 120+(2*(4*15)), 0, 0},
  {180, 120+(2*(4*15)), 0, 0},
  {60, 120+(3*(4*15)), 0, 0},
  {180, 120+(3*(4*15)), 0, 0}
  };

void setup(void) {
  Wire.begin(); 
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x6B); 
  Wire.write(0);
  Wire.endTransmission(false); 
  Serial.begin(9600);
  
  Serial.print(F("ST7789 TFT Screen Test")); 
  tft.init(240, 240, SPI_MODE2);  
  tft.setRotation(2); 
  Serial.println(F("Initialized"));
  Serial.println("done"); 

  tft.fillScreen(ST77XX_CYAN);
  tft.fillRect(60, 80, 120, 80, ST77XX_MAGENTA);
  tft.fillTriangle(60, 80, 120, 40, 180, 80, ST77XX_MAGENTA);
  tft.fillTriangle(60, 160, 120, 200, 180, 160, ST77XX_MAGENTA);

  delay(1000);
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
  
  if (y > 180){
    y = y - 360;
  }
  if (y > 90){
    y = 180 - y;
  }
  if (y < -90){
    y = -180 - y;
  }

  for (int i = 0; i < ROWS; i++) {
    xO = Coordinates[i][0];
    yO = Coordinates[i][1];
    xN = Coordinates[i][2];
    yN = Coordinates[i][3];
    
    RotateOnX(xO, yO, xN, yN, x);
    TranslateOnY(xN, yN, y); 
    
    Coordinates[i][0] = xO;
    Coordinates[i][1] = yO;
    Coordinates[i][2] = xN;
    Coordinates[i][3] = yN;
  }
  DrawElements(tft, Coordinates, ROWS);
}

double RotateOnX(double xO, double yO, double &xN, double &yN, double x){  
  xO = xO - 120; 
  yO = yO - 120;
  xN = sqrt((xO * xO) + (yO * yO)) * sin(((x + ((atan((xO) / (yO))) / PI) * 180) / 180) * PI);
  yN = sqrt((xO * xO) + (yO * yO)) * cos(((x + ((atan((xO) / (yO))) / PI) * 180) / 180) * PI);
  xN = xN + 120;
  yN = yN + 120;
}

double TranslateOnY(double &xN, double &yN,double y) { 
  xN = xN + (2 * y * sin((x * PI) / 180));
  yN = yN + (2 * y * cos((x * PI) / 180));
}

int DrawElements(Adafruit_ST7789 tft, int Coordinates[][4], int ROWS) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);

  for (int i = 0; i < ROWS; i++) {
    if (i == 0 || (i % 2 == 0)) { 
      tft.drawLine(Coordinates[i][2], Coordinates[i][3], Coordinates[i+1][2], Coordinates[i+1][3], ST77XX_ORANGE);
      tft.setCursor(Coordinates[i][2], Coordinates[i][3] + 5);
      tft.println(-(i * 15)); 

      double X1 = Coordinates[i][2] - (2*(Coordinates[i][1]-120)*sin(M_PI*(x/180)));
      double Y1 = Coordinates[i][3] - (2*(Coordinates[i][1]-120)*cos(M_PI*(x/180)));
      double X2 = Coordinates[i+1][2] - (2*(Coordinates[i][1]-120)*sin(M_PI*(x/180)));
      double Y2 = Coordinates[i+1][3] - (2*(Coordinates[i][1]-120)*cos(M_PI*(x/180)));

      tft.drawLine(X1, Y1, X2, Y2, ST77XX_BLUE);
      tft.setCursor(X1, Y1);
      tft.println((i * 15)); 
    }
  }
  tft.fillRect(0, 0, 240, 60, ST77XX_BLACK);
  tft.fillRect(0, 60, 45, 180, ST77XX_BLACK);
  tft.fillRect(195, 60, 45, 180, ST77XX_BLACK);
  tft.fillRect(0, 180, 240, 60, ST77XX_BLACK);

  tft.drawCircle(120, 120, 5, ST77XX_GREEN);

  tft.drawCircle(120, -30, 65, ST77XX_GREEN);
  tft.fillTriangle(100, 0, 140, 0, 120, 10, ST77XX_GREEN);
  tft.setCursor(110, 25);
  tft.println(x); 

  tft.drawCircle(120, 240, 50, ST77XX_GREEN);
  tft.drawFastVLine(120,215,30,ST77XX_GREEN);
  tft.fillTriangle(115, 240, 125, 240, 120, 235, ST77XX_GREEN);
  tft.fillTriangle(100, 220, 140, 220, 120, 225, ST77XX_GREEN);
  tft.setCursor(110, 200);
  tft.println(z); 

  tft.drawFastVLine(45,60,120,ST77XX_GREEN);
  tft.drawFastHLine(0,60,45,ST77XX_GREEN);
  tft.drawFastHLine(0,180,45,ST77XX_GREEN);
  tft.drawTriangle(45, 125, 45, 115, 40, 120, ST77XX_GREEN);
  tft.setCursor(0, 115);
  tft.println("HEIGHT"); 

  tft.drawFastVLine(195,60,120,ST77XX_GREEN);
  tft.drawFastHLine(195,60,45,ST77XX_GREEN);
  tft.drawFastHLine(195,180,45,ST77XX_GREEN);
  tft.drawTriangle(195, 125, 195, 115, 200, 120, ST77XX_GREEN);
  tft.setCursor(210, 115);
  tft.println("SPEED"); 
}