#include <Adafruit_VS1053.h>

#define	THRESHOLD_L	335
#define THRESHOLD_H	343

#define TIME_RAMP	250
#define VOL_HIGH	20
#define VOL_LOW		180

#define SHIELD_RESET   -1	// VS1053 reset pin (unused!)
#define SHIELD_CS     	7	// VS1053 chip select pin (output)
#define SHIELD_DCS    	6	// VS1053 Data/command select pin (output)
#define CARDCS 			4	// Card chip select pin
#define DREQ 			3	// VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =
	// create shield-example object!
	Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

const char *filename = "track001.mp3";

uint8_t volume;
long start;
long stopped;
bool wasDown;

void setup() {
	Serial.begin(9600);
	while(!Serial){}

	// Initialize VS1053
	Serial.println(F("Initializing Adafruit Soundplayer"));
	if (! musicPlayer.begin() ) {
		Serial.println(F("Could not initialize VS1053"));
		while(true){};
	}
	Serial.println(F("Initialation of VS1053 successful"));

	// Initialize SD Card
	SD.begin(CARDCS);

	volume = VOL_LOW;
	musicPlayer.setVolume(volume, volume);
	start = 0;
	stopped = 0;
	wasDown = false;

	// DREQ pin set for background audio
	musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
}

void loop() {
	long ms = millis();

	if (isButtonDown()) {
		if (!wasDown) {
			wasDown = true;
			start = ms;
		}
		volume = rampUp(ms - start);
		musicPlayer.setVolume(volume, volume);
		
		if (!musicPlayer.playingMusic) {
			musicPlayer.startPlayingFile(filename);
		}
	} else {
		if (wasDown){
			wasDown = false;
			stopped = ms;			
		}
		volume = rampDown(ms - stopped);
		musicPlayer.setVolume(volume, volume);
		
		if ((ms - stopped) > TIME_RAMP) {
			musicPlayer.stopPlaying();
		}
	}

	//Serial.print("Reading: ");
	Serial.println(analogRead(0));
	//Serial.print(" sets to ");
	//Serial.println(isButtonDown());
	//Serial.println(volume);
}

bool isButtonDown() {
	int reading = analogRead(0);
	if (reading < THRESHOLD_L || reading > THRESHOLD_H) {
		return true;
	}
	return false;
}

uint8_t rampUp(long t) {
	if (t < TIME_RAMP) { // milliseconds
		return (uint8_t)map(t, 0, TIME_RAMP, VOL_LOW, VOL_HIGH);
	} else {
		return VOL_HIGH;
	}
}

uint8_t rampDown(long t) {
	if (t < TIME_RAMP) {
		return (uint8_t)map(t, 0, TIME_RAMP, VOL_HIGH, VOL_LOW);
	} else {
		return VOL_LOW;
	}
}

