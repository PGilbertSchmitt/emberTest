#include <Adafruit_VS1053.h>

int i;
int lowestReading;

void setup() {
	Serial.begin(9600);
	while(!Serial){}
	
	lowestReading = 400;
	i = 1;
}

void loop() {
	int reading = analogRead(0);
	if (reading < lowestReading) {
		lowestReading = reading;
	}
	
	Serial.print(i++);
	Serial.print(" - Current reading: ");
	Serial.print(reading);
	Serial.print("\nLowest Reading: ");
	Serial.println(lowestReading);
	
	delay(700);
}
