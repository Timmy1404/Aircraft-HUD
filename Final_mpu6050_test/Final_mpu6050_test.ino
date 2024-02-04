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
double x, y, z;
int minVal = 265;
int maxVal = 402;

// multi dimentional array to hold all coordinates accross entire code, first bracket is number of elements in array, second is elements in the sub-arrays
double xO, yO, xN, yN;
int Coordinates[4][4] = {
  {60, 120, 0, 0},     //line1 start - old and new x and y values
  {180, 120, 0, 0},    //line1 end - old and new x and y values
  {60, 140, 0, 0},     //line2 start
  {180, 140, 0, 0}     //line2 end
  };

void setup(void) {
  Wire.begin(); // initializes the I2C communication as a master device.
  
  //configure sensor
  Wire.beginTransmission(MPU_addr); // begins the transmission to the device with the address MPU_addr. In this case, it is used to configure the screen.
  Wire.write(0x6B); // sends data to the device. Here, it writes the value 0x6B to configure a specific register of the screen.
  Wire.endTransmission(false); // ends the I2C transmission without releasing the I2C bus. The false parameter indicates that the Arduino will keep the connection active, allowing for subsequent read operations without retransmitting the address.
  Wire.requestFrom(MPU_addr, 14, true); // requests data from the MPU device. It specifies the device address (MPU_addr), the number of bytes to request (14), and the option to release the I2C bus after the transmission (true).
  
  //configure screen
  Serial.begin(9600); // initializes the serial communication at a baud rate of 9600. It is used to communicate with the Arduino IDE or a serial monitor for debugging and displaying messages.
  Serial.print(F("ST7789 TFT Screen Test")); // This line prints the message "ST7789 TFT Screen Test" to the serial monitor.
  tft.init(240, 240, SPI_MODE2);  // initializes the ST7789 TFT screen using the tft object. It specifies the screen dimensions as 240x240 pixels and the SPI mode as SPI_MODE2.
  tft.setRotation(2); // sets the rotation of the screen to 2. The rotation value can be changed to adjust the screen orientation.
  Serial.println(F("Initialized")); // prints the message "Initialized" to the serial monitor.
  Serial.println("done"); // prints the message "done" to the serial monitor.
}

void loop() {
  
for (int j = 1; j <= 9; j++) {
  for (int i = 0; i <= 1; i++) {
    // get sensor data
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
    int xAng = map(AcX, minVal, maxVal, -90, 90);
    int yAng = map(AcY, minVal, maxVal, -90, 90);
    int zAng = map(AcZ, minVal, maxVal, -90, 90);
    x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
    y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
    z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI); 
    
    //redefining the variable for efficientcy
    xO = Coordinates[i][0];
    yO = Coordinates[i][1];
    xN = Coordinates[i][2];
    yN = Coordinates[i][3];
    
    NewCoordinates(xO, yO, xN, yN, x, y);  //rotate line around the origin
  }
}

//draw all static display elements
tft.fillScreen(ST77XX_BLACK);
tft.drawCircle(120, 120, 5, ST77XX_GREEN);
//draw all updates lines simultaneously
for (int i = 0; i <= 10; i++) {
  tft.drawLine(Coordinates[i][2], Coordinates[i][3], Coordinates[i+1][2], Coordinates[i+1][3], ST77XX_GREEN);
}

}

double NewCoordinates(double xO, double yO, double &xN, double &yN, double x, double y) {  //& = refrences value
  xO = xO - 120;
  yO = yO - 120;
  xN = (sqrt((xO * xO) + (yO * yO)) * sin(((x + ((atan((xO) / (yO))) / M_PI) * 180) / 180) * M_PI) + (y * sin(x)));
  yN = (sqrt((xO * xO) + (yO * yO)) * cos(((x + ((atan((xO) / (yO))) / M_PI) * 180) / 180) * M_PI) + (y * cos(x)));
  xN = xN + 120; 
  yN = yN + 120;
}