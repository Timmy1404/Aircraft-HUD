#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>              // Arduino SPI library
#include <Wire.h>             // Transmission protocol for I2C

// ST7789 TFT module connections
#define TFT_CS 10  // Define chip select pin
#define TFT_DC 9   // Define data/command pin
#define TFT_RST 8  // Define reset pin, or set to -1 and connect to Arduino RESET pin

// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Sensor variable
const int MPU_addr = 0x68;

// Gyroscope variables
int16_t AcX, AcY, AcZ;
double x, y;
int z;
const int minVal = 265;
const int maxVal = 402;

// Multi-dimensional array to hold all coordinates across the entire code
double xO, yO, xN, yN;
const int ROWS = 8;
const int COLUMNS = 4;
int Coordinates[ROWS][COLUMNS] = { 
  {60, 125, 0, 0},
  {180, 125, 0, 0},
  {60, 120 + (1 * (4 * 15)), 0, 0},
  {180, 120 + (1 * (4 * 15)), 0, 0},
  {60, 120 + (2 * (4 * 15)), 0, 0},
  {180, 120 + (2 * (4 * 15)), 0, 0},
  {60, 120 + (3 * (4 * 15)), 0, 0},
  {180, 120 + (3 * (4 * 15)), 0, 0}
};

void setup(void) {
  Wire.begin(); // Initializes the I2C communication as a master device.
  
  // Configure sensor
  Wire.begin();
  Wire.beginTransmission(MPU_addr); // Begins the transmission to the device with the address MPU_addr.
  Wire.write(0x6B); // Configures a specific register of the screen.
  Wire.write(0);
  Wire.endTransmission(false); // Ends the I2C transmission without releasing the I2C bus.
  Serial.begin(9600); // Initializes the serial communication at a baud rate of 9600.

  // Configure screen
  Serial.print(F("ST7789 TFT Screen Test"));
  tft.init(240, 240, SPI_MODE2);  // Initializes the ST7789 TFT screen using the tft object.
  tft.setRotation(2); // Sets the rotation of the screen.
  Serial.println(F("Initialized"));
  Serial.println("done");

  // Print Logo
  tft.fillScreen(ST77XX_CYAN);
  tft.fillRect(60, 80, 120, 80, ST77XX_MAGENTA);
  tft.fillTriangle(60, 80, 120, 40, 180, 80, ST77XX_MAGENTA);
  tft.fillTriangle(60, 160, 120, 200, 180, 160, ST77XX_MAGENTA);
}

void loop() {
  // Get sensor data
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

  // Angle corrections
  if (y > 180) {
    y = y - 360;
  }
  if (y > 90) {
    y = 180 - y;
  }
  if (y < -90) {
    y = -180 - y;
  }

  //loop through each line 
  for (int i = 0; i < ROWS; i++) {
    // Redefining the variable for efficiency
    xO = Coordinates[i][0];
    yO = Coordinates[i][1];
    xN = Coordinates[i][2];
    yN = Coordinates[i][3];
    
    // Rotate line around the origin
    RotateOnX(xO, yO, xN, yN, x); 
    // Translates line away from the origin
    TranslateOnY(xN, yN, y); 
    
    Coordinates[i][0] = xO;
    Coordinates[i][1] = yO;
    Coordinates[i][2] = xN;
    Coordinates[i][3] = yN;
  }
  
  // Draw all static display elements
  DrawElements(tft, Coordinates, ROWS);
}

double RotateOnX(double xO, double yO, double &xN, double &yN, double x) {
  // Translate values out of context
  xO = xO - 120; 
  yO = yO - 120;
  // X angle equation
  xN = sqrt((xO * xO) + (yO * yO)) * sin(((x + ((atan((xO) / (yO))) / PI) * 180) / 180) * PI); 
  yN = sqrt((xO * xO) + (yO * yO)) * cos(((x + ((atan((xO) / (yO))) / PI) * 180) / 180) * PI);
  // Translate values in to context
  xN = xN + 120; 
  yN = yN + 120;
}

double TranslateOnY(double &xN, double &yN, double y) {
  // Y angle equation
  xN = xN + (2 * y * sin((x * PI) / 180)); 
  yN = yN + (2 * y * cos((x * PI) / 180));
}

int DrawElements(Adafruit_ST7789 tft, int Coordinates[][4], int ROWS) {
  // Reset the screen colours
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);

  // Draw all updated lines simultaneously
  for (int i = 0; i < ROWS; i++) {
    if (i == 0 || (i % 2 == 0)) {
      // Draw positive lines
      tft.drawLine(Coordinates[i][2], Coordinates[i][3], Coordinates[i + 1][2], Coordinates[i + 1][3], ST77XX_ORANGE);
      tft.setCursor(Coordinates[i][2], Coordinates[i][3] + 5);
      tft.println(-(i * 15));

      // Calculate negative values
      double X1 = Coordinates[i][2] - (2 * (Coordinates[i][1] - 120) * sin(M_PI * (x / 180)));
      double Y1 = Coordinates[i][3] - (2 * (Coordinates[i][1] - 120) * cos(M_PI * (x / 180)));
      double X2 = Coordinates[i + 1][2] - (2 * (Coordinates[i][1] - 120) * sin(M_PI * (x / 180)));
      double Y2 = Coordinates[i + 1][3] - (2 * (Coordinates[i][1] - 120) * cos(M_PI * (x / 180)));

      // Draw negative lines
      tft.drawLine(X1, Y1, X2, Y2, ST77XX_BLUE);
      tft.setCursor(X1, Y1);
      tft.println((i * 15));
    }
  }

  // Hide unnecessary elements
  tft.fillRect(0, 0, 240, 60, ST77XX_BLACK);
  tft.fillRect(0, 60, 45, 180, ST77XX_BLACK);
  tft.fillRect(195, 60, 45, 180, ST77XX_BLACK);
  tft.fillRect(0, 180, 240, 60, ST77XX_BLACK);

  // Draw middle pointer
  tft.drawCircle(120, 120, 5, ST77XX_GREEN);

  // Draw roll instrument
  tft.drawCircle(120, -30, 65, ST77XX_GREEN);
  tft.fillTriangle(100, 0, 140, 0, 120, 10, ST77XX_GREEN);
  tft.setCursor(110, 25);
  tft.println(x);
  
  // Draw yaw instrument
  tft.drawCircle(120, 240, 50, ST77XX_GREEN);
  tft.drawFastVLine(120, 215, 30, ST77XX_GREEN);
  tft.fillTriangle(115, 240, 125, 240, 120, 235, ST77XX_GREEN);
  tft.fillTriangle(100, 220, 140, 220, 120, 225, ST77XX_GREEN);
  tft.setCursor(110, 200);
  tft.println(z);

  // Draw altitude instrument
  tft.drawFastVLine(45, 60, 120, ST77XX_GREEN);
  tft.drawFastHLine(0, 60, 45, ST77XX_GREEN);
  tft.drawFastHLine(0, 180, 45, ST77XX_GREEN);
  tft.drawTriangle(45, 125, 45, 115, 40, 120, ST77XX_GREEN);
  tft.setCursor(0, 115);
  tft.println("HEIGHT");

  // Draw velocity instrument
  tft.drawFastVLine(195, 60, 120, ST77XX_GREEN);
  tft.drawFastHLine(195, 60, 45, ST77XX_GREEN);
  tft.drawFastHLine(195, 180, 45, ST77XX_GREEN);
  tft.drawTriangle(195, 125, 195, 115, 200, 120, ST77XX_GREEN);
  tft.setCursor(210, 115);
  tft.println("SPEED");
}