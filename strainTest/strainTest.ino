#include <Adafruit_VS1053.h>

void setup() {
  Serial.begin(9600);
  while(!Serial){}

  Serial.print("This program will spit out a reading every\n");
  Serial.print("half a second. Write down the readings you\n");
  Serial.print("like, and I can use those later.\n");
}

void loop() {
  Serial.print("Current reading: ");
  Serial.print(analogRead(0));
  Serial.println("\n");
  delay(500);
}
