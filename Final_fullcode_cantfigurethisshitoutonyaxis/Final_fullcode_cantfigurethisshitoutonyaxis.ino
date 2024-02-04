#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>              // Arduino SPI library
#include <Wire.h>             // transmission protocol

// ST7789 TFT module connections
#define TFT_CS 10  // define chip srelect pin
#define TFT_DC 9   // define data/command pin
#define TFT_RST 8  // define reset pin, or set to -1 and connect to Arduino RESET pin

// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//sensor variable
const int MPU_addr = 0x68;

//gyroscope variables
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
double x, y;
int z;
int minVal = 265;
int maxVal = 402;

// multi dimentional array to hold all coordinates accross entire code, first bracket is number of elements in array, second is elements in the sub-arrays
double xO, yO, xN, yN;
const int ROWS = 8;
const int COLUMNS = 4;
int Coordinates[ROWS][COLUMNS] = { 
  {60, 120+40, 0, 0},     //line1 start - old and new x and y values
  {180, 120+40, 0, 0},    //line1 end - old and new x and y values
  {60, 120, 0, 0},     //line2 start
  {180, 120, 0, 0},    //line2 end
  {60, 120-40, 0, 0},     
  {180, 120-40, 0, 0},  
  };

void setup(void) {
  Wire.begin(); // initializes the I2C communication as a master device.
  
  //configure sensor
  Wire.begin();
  Wire.beginTransmission(MPU_addr); // begins the transmission to the device with the address MPU_addr. In this case, it is used to configure the screen.
  Wire.write(0x6B); // sends data to the device. Here, it writes the value 0x6B to configure a specific register of the screen.
  Wire.write(0);
  Wire.endTransmission(false); // ends the I2C transmission without releasing the I2C bus. The false parameter indicates that the Arduino will keep the connection active, allowing for subsequent read operations without retransmitting the address.
  Serial.begin(9600);// initializes the serial communication at a baud rate of 9600. It is used to communicate with the Arduino IDE or a serial monitor for debugging and displaying messages.
  
  //configure screen
  Serial.print(F("ST7789 TFT Screen Test")); // This line prints the message "ST7789 TFT Screen Test" to the serial monitor.
  tft.init(240, 240, SPI_MODE2);  // initializes the ST7789 TFT screen using the tft object. It specifies the screen dimensions as 240x240 pixels and the SPI mode as SPI_MODE2.
  tft.setRotation(2); // sets the rotation of the screen to 2. The rotation value can be changed to adjust the screen orientation.
  Serial.println(F("Initialized")); // prints the message "Initialized" to the serial monitor.
  Serial.println("done"); // prints the message "done" to the serial monitor.
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

  x = 0;
  y = 0;
  
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
    if (Coordinates[i][1] < 120){
      //Coordinates[i][2] = (Coordinates[i-1][2] + (40 * cos(x)));
      //Coordinates[i][3] = (Coordinates[i-1][3] + (40 * sin(x)));
      //Coordinates[i][2] = (Coordinates[i-1][2] - 40);
      Coordinates[i][3] = (Coordinates[i-2][3] - 40);
      Coordinates[i+1][3] = (Coordinates[i-2][3] - 40);
    }else{
    //redefining the variable for efficientcy
    xO = Coordinates[i][0];
    yO = Coordinates[i][1];
    xN = Coordinates[i][2];
    yN = Coordinates[i][3];
    
    RotateOnX(xO, yO, xN, yN, x);  //rotate line around the origin
    TranslateOnY(xN, yN, y); //translates line away from the origin
    
    Coordinates[i][0] = xO;
    Coordinates[i][1] = yO;
    Coordinates[i][2] = xN;
    Coordinates[i][3] = yN;
    };
  }
  //draw all static display elements
  DrawElements(tft, Coordinates, ROWS);

  tft.setCursor(0, 0);
  tft.println(y); 

delay(1000);
}

double RotateOnX(double xO, double yO, double &xN, double &yN, double x) {  //& = refrences value
  xO = xO - 120; //Convert out of context
  yO = yO - 120;
  xN = sqrt((xO * xO) + (yO * yO)) * sin(((x + ((atan((xO) / (yO))) / PI) * 180) / 180) * PI);
  yN = sqrt((xO * xO) + (yO * yO)) * cos(((x + ((atan((xO) / (yO))) / PI) * 180) / 180) * PI);
  xN = xN + 120; //Convert back to context
  yN = yN + 120;

}

double TranslateOnY(double &xN, double &yN,double y) { 
  xN = xN + (2 * y * sin((x * PI) / 180));
  yN = yN + (2 * y * cos((x * PI) / 180));
}

int DrawElements(Adafruit_ST7789 tft, int Coordinates[][4], int ROWS) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);

  tft.drawCircle(120, 120, 5, ST77XX_GREEN);

  tft.drawCircle(120, -30, 65, ST77XX_GREEN);
  tft.fillTriangle(100, 0, 140, 0, 120, 10, ST77XX_GREEN);

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

  //draw all updates lines simultaneously
  for (int i = 0; i < ROWS; i++) {
    if ((i == 0 || (i % 2 == 0)) && (60 < Coordinates[i][3] && Coordinates[i][3] < 180)) { 
      tft.drawLine(Coordinates[i][2], Coordinates[i][3], Coordinates[i+1][2], Coordinates[i+1][3], ST77XX_GREEN);
      tft.setCursor(Coordinates[i][2], Coordinates[i][3] + 5);
      //tft.println(i * 10); 
      tft.println("?"); 
    }
  }
}