//M_PI = 3.14159

int x = 100;
int y = 0;

int xO = 180;
int yO = 180;
int xN = 0;
int yN = 180;

void setup() {
 xO = xO - 120; 
 yN = yN - 120; 
 xN = 120 + (sqrt((xO * xO) + (yN * yN)) * sin(((x + ((atan((xO) / (yN))) / M_PI) * 180) / 180) * M_PI)); 
 xO = xO + 120; 

 Serial.begin(9600);
 Serial.println(",");
 //Serial.println(x);
 //Serial.println(y);
 Serial.println(xO);
 //Serial.println(yO);
 Serial.println(xN); 
 //Serial.println(yN);
}

void loop() {
  // put your main code here, to run repeatedly:
}



