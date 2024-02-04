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
  //testdrawtext("Timmy", ST77XX_WHITE);
  //delay(1);

  //(boundary)
  //tft.drawFastHLine(60,60,120,ST77XX_RED); 
  //tft.drawFastHLine(60,180,120,ST77XX_RED);
  //tft.drawFastVLine(60,60,120,ST77XX_RED);
  //tft.drawFastVLine(180,60,120,ST77XX_RED);
 
  AngleOfAttack1(x1, y1);


  // compass bearing and bank angle
  //tft.drawCircle(120,230,30,'G'); //tft.drawCircle(x,y,r,t);//function draws a circle in x and y location and r radius and t color.
  //tft.drawTriangle(120,200,110,190,130,190,ST77XX_GREEN); //tft.drawTriangle(x1,y1,x2,y2,x3,y3,t);//function draws a filled triangle with three corner location x, y and z, and t color.
  //tft.drawCircle(120,-30,60,ST77XX_GREEN);
  //tft.drawTriangle(120,30,110,40,130,40,ST77XX_GREEN);
  //tft.setTextWrap(true); 
  //tft.setCursor(103, 225); //sets designated pixel 
  ///tft.setTextColor(ST77XX_GREEN); //sets colour of text
  //tft.setTextSize(2); //sets tect size
  //tft.print("000"); //print "text"
  //tft.setCursor(115, 207);
  //tft.print("N");

  // altitude and airspeed
  //tft.drawLine(50,60,50,180,ST77XX_GREEN); //tft.drawFastHLine(x,y,w,t); This function draws a horizontal line that starts in x, y location, and its length is w pixel and its color is t.
  //tft.drawTriangle(40,120,50,110,50,130,ST77XX_GREEN);
  //tft.drawLine(190,60,190,180,ST77XX_GREEN);
  //tft.drawTriangle(200,120,190,110,190,130,ST77XX_GREEN);
 
  // finsihed program
  Serial.println("done");
}




void loop() {
  //AngleOfAttack1(int x, int y); //X1_1, X1_2, X2_1, X2_2 , Y1_1, Y1_2, Y2_1, Y2_2, ST77XX_Green
  //AltitudeAndAirspeed1(, ST77XX_Green);
  //CompassBearing1(, ST77XX_Green);
  //OtherValues1(, ST77XX_Green);
}




void AngleOfAttack1(int x, int y) {
  //Y1 = Y1 + Tan(AccY);
  // angle of attack
  //tft.drawLine(80,80,110,80,ST77XX_GREEN); //(5)//tft.drawLine(xi,yi,xj,yj,t);//This function draws a line that starts in xi and yi location ends is in xj and yj and the color is t.
  //tft.drawLine(110,80,110,90,ST77XX_GREEN);
  //tft.drawLine(130,80,160,80,ST77XX_GREEN); //(5)
  //tft.drawLine(130,80,130,90,ST77XX_GREEN);

  tft.drawLine(60,116,110,116,ST77XX_GREEN); //(0)
  //tft.drawLine(130,116,180,116,ST77XX_GREEN); //(0)
  tft.drawCircle(120,120,5,ST77XX_GREEN); //gun sight
  //tft.drawLine(110,120,115,120,ST77XX_GREEN); //left
  //tft.drawLine(125,120,130,120,ST77XX_GREEN); //right
  //tft.drawLine(120,113,120,115,ST77XX_GREEN); //middle
  //tft.drawLine(60,124,110,124,ST77XX_GREEN); //(0)
  //tft.drawLine(130,124,180,124,ST77XX_GREEN); //(0)

  //tft.drawLine(80,160,110,160,ST77XX_GREEN); //(-5)
  //tft.drawLine(110,160,110,150,ST77XX_GREEN);
  //tft.drawLine(130,160,160,160,ST77XX_GREEN); //(-5)
  //tft.drawLine(130,160,130,150,ST77XX_GREEN);
}
//void AltitudeAndAirspeed1(char *text, uint16_t color) {
//}
//void CompassBearingAndBankAngle1(char *text, uint16_t color) {
//}
//void OtherValues1(char *text, uint16_t color) {
  // other values
  //tft.setCursor(120, 120);
  //tft.setTextColor(color);
  //tft.setTextWrap(true);
  //tft.print(text);
//}