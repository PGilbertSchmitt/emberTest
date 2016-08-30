#include <Adafruit_VS1053.h>

#define RESET_PIN		-1		// Unused pin
#define CS_PIN			7		// Chip select
#define DCS_PIN			6		// Data/command select
#define CARDCS_PIN		4		// SD Card chip select
#define DATA_REQ_PIN	3		// Data request interrupt
#define SECONDS			1000	// Seconds in a millisecond

// The full volume
#define HIGHVOL 		20

// The "quiet" volume
#define HALFVOL			55

// The "nothing" volume. 
/* You shouldn't be able to hear this too well, 
 * and I didn't place it too low (ie 255) because you should be able
 * to hear the full ramp.*/
#define LOWVOL			180

// The length of time (milliseconds) that a ramp lasts
#define PERIOD			2000

// The length of time (milliseconds) before ramping down to half volume
#define SWITCH			8000

const char *trackName = "track001.mp3";
int lastSec = 0;

// Test data
/* Set to false to test half volume, true for the full volume */
bool dayTime = true;
int timeOffset = 0;
/* Change the number for different playtimes */
unsigned long totalTime = 30 * SECONDS;


Adafruit_VS1053_FilePlayer player = 
	Adafruit_VS1053_FilePlayer
	(
		RESET_PIN,
		CS_PIN,
		DCS_PIN,
		DATA_REQ_PIN,
		CARDCS_PIN
	);


void setup() {
	Serial.begin(9600);
	// Music Player setup
	if (!player.begin()) {
		Serial.println(F("No VS1053"));
		while (1);
	}
	Serial.println(F("VS1053 found"));
	player.setVolume(LOWVOL, LOWVOL);
	player.useInterrupt(VS1053_FILEPLAYER_PIN_INT); // Data request interrupt

	// Initialize SD card
	SD.begin(CARDCS_PIN);

	delay(50);
	lastSec = 0;
	Serial.println(F("Starting player in setup"));
	player.startPlayingFile(trackName);
	timeOffset = millis();
	Serial.println(F("Setup done."));
}

void loop() {
	unsigned long milliSec = millis();
	
	
	// Play
	if (player.stopped()) {
		player.startPlayingFile(trackName);
		Serial.println("Player restarted");
	}

	int volume = getVolume(dayTime, milliSec - timeOffset, totalTime);

	player.setVolume(volume, volume);

	long sec = milliSec / 1000;
	Serial.print(sec);
	Serial.print("| delta: ");
	Serial.println(volume);

	delay(200);
}

// Get used to these nested ifs. I can't do squat about it right now, 
// and I hate myself for doing it like this. It could be cleaner, but it 
// just needs to work.
// b_DayTime should be set to true if it's between 8AM and 8PM
// deltaTime should be the time passed since the music began playing
uint8_t getVolume(bool b_DayTime, unsigned long deltaTime, unsigned long wholeTime){
	
	if (deltaTime > wholeTime) { return LOWVOL; }
	// This means it is nightTime, play softly
	if (!b_DayTime) 
	{ 
		// Too short
		if (wholeTime < (4 * SECONDS)) 
		{
			int halfTime = wholeTime / 2;
			
			if (deltaTime < halfTime) {
				return (uint8_t)map(deltaTime, 0, halfTime, LOWVOL, HALFVOL);
			} else {
				return (uint8_t)map(deltaTime, halfTime, wholeTime, HALFVOL, LOWVOL);
			}
		} else 
		{
			if (deltaTime < PERIOD) {
				return (uint8_t)map(deltaTime, 0, PERIOD, LOWVOL, HALFVOL);
			} else if (deltaTime > PERIOD && deltaTime < wholeTime - PERIOD) {
				return (uint8_t)HALFVOL;
			} else {
				return (uint8_t)map(deltaTime, (wholeTime - PERIOD), wholeTime, HALFVOL, LOWVOL);
			}
		}
	}

	// It's not Day time, play loudly depending on the timescale
	// Ellapsed time is too short for full envelope
	if (wholeTime < (4 * SECONDS)) 
	{
		int halfTime = wholeTime / 2;
		
		if (deltaTime < halfTime) {
			return (uint8_t)map(deltaTime, 0, halfTime, LOWVOL, HIGHVOL);
		} else {
			return (uint8_t)map(deltaTime, halfTime, wholeTime, HIGHVOL, LOWVOL);
		}
	} else if (wholeTime < (12 * SECONDS))
	{
		if (deltaTime < PERIOD) {
			return (uint8_t)map(deltaTime, 0, PERIOD, LOWVOL, HIGHVOL);
		} else if (deltaTime > PERIOD && deltaTime < wholeTime - PERIOD) {
			return (uint8_t)HIGHVOL;
		} else {
			return (uint8_t)map(deltaTime, (wholeTime - PERIOD), wholeTime, HIGHVOL, LOWVOL);
		}
	} else // This is the special profile
	{
		if (deltaTime < PERIOD) {
			return (uint8_t)map(deltaTime, 0, PERIOD, LOWVOL, HIGHVOL);
		} else if (deltaTime < SWITCH) {
			return (uint8_t)HIGHVOL;
		} else if (deltaTime < (SWITCH + PERIOD)) {
			return (uint8_t)map(deltaTime, SWITCH, (SWITCH + PERIOD), HIGHVOL, HALFVOL);
		} else if (deltaTime < (wholeTime - PERIOD)) {
			return (uint8_t)HALFVOL;
		} else {
			return (uint8_t)map(deltaTime, (wholeTime - PERIOD), wholeTime, HALFVOL, LOWVOL);
		}
	}
}















