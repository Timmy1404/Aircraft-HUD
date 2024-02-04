#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library

int x1O = 120;
int y1O = 120;
int x1N = 0;
int y1N = 0;
int a = 90;


void setup() {
  Serial.begin(9600);
  x1N = (sqrt(x1O^2 + y1O^2) * cos(a)) + (x1O * sin(180-a));
  y1N = (sqrt(x1O^2 + y1O^2) * sin(a)) + (y1O * sin(180-a));
  Serial.print(x1N);
  Serial.print(y1N);
}

void loop() {
  // put your main code here, to run repeatedly:
}
