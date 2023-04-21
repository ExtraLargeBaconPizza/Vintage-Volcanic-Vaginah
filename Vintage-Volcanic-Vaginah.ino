/*
    Board: NodeMCU-ESP32s
	Upload Speed: 921600
	Pinout Reference: https://esphome.io/devices/nodemcu_esp32.html
    Ws2815 Data Sheet: https://www.led-stuebchen.de/download/WS2815.pdf

    Notes:
    - Cannot use loop in addLeds function because the pin number must be a runtime constant. Gross, I know.
    - Ensure esp32 is grounded to psu, even if powered by seperate usb
    - Ws2815 are unidirectional. You must connect to the female end of the strip to a 4-pin  male JST-SM from the board
    - Parrallel output: - writing to each pixel takes 30µs, so FastLED.show() can take while because it has to iterate over all pixels and strips in series (ie one at a time)
                        - parrallel will write to strips in parrallel and must be enabled
                        - reddit explination: https://www.reddit.com/r/FastLED/comments/aqlb94/troubleshooting_slow_performance_tied_to/
                        - https://github.com/FastLED/FastLED/wiki/Parallel-Output
                        - solution: https://www.reddit.com/r/FastLED/comments/klw88g/are_there_magical_words_to_summon_parallel_output/
    - Backup data line: - Do not wire backup data line to seperate pins. This can cause noise over longer wire lengths, which can result in random flickering
                        - Either wire both data lines to the same pin or wire the backup data line directly to the ground (as close to strip as possible)
                        - https://www.reddit.com/r/WLED/comments/yw10qa/ws2815_backup_data_line_question/
*/

#include <FastLED.h>
#include "Test2.h"

// Enable Parrallel Output
#define FASTLED_ESP32_I2S        true
#define FASTLED_RMT_MAX_CHANNELS 8

// LED Settings
#define CHIPSET          WS2811
#define COLOR_ORDER      RGB
#define NUM_LEDS         300
#define NUM_STRIPS 	     8
#define COLOR_CORRECTION CRGB(255, 100, 240)
#define BRIGHTNESS       255

// Lava Settings
#define SPARK_CHANCE                120
#define ERUPTION_LENGTH_MINUTES     20
#define MAX_COOLING_AND_SPEED_DELAY 20
#define INCREMENT_INTERVAL_SECONDS  10
#define RESET_INTERVAL_MINUTES      20

enum EruptionState 
{
    Unknown,
    Erupting,
    Flowing,
};

EruptionState _eruptionState;

CRGB _leds[NUM_STRIPS][NUM_LEDS];
byte _heat[NUM_STRIPS][NUM_LEDS];

int _currentStripIndex;
int _coolingAndSpeedDelay;
int _lastCoolingIncrementTime;

void setup() 
{
    Serial.begin(115200);

	setupLedStrips();
}

void setupLedStrips()
{
	// left side
	FastLED.addLeds<CHIPSET, 32, COLOR_ORDER>(_leds[0], NUM_LEDS);
	FastLED.addLeds<CHIPSET, 25, COLOR_ORDER>(_leds[1], NUM_LEDS);
	FastLED.addLeds<CHIPSET, 27, COLOR_ORDER>(_leds[2], NUM_LEDS);
	FastLED.addLeds<CHIPSET, 12, COLOR_ORDER>(_leds[3], NUM_LEDS);
    
	// right side
	FastLED.addLeds<CHIPSET, 19, COLOR_ORDER>(_leds[4], NUM_LEDS);
	FastLED.addLeds<CHIPSET, 5,  COLOR_ORDER>(_leds[5], NUM_LEDS);
	FastLED.addLeds<CHIPSET, 16, COLOR_ORDER>(_leds[6], NUM_LEDS);
	FastLED.addLeds<CHIPSET, 0,  COLOR_ORDER>(_leds[7], NUM_LEDS);

    // color correction needed otherwise it will be too green
    FastLED.setCorrection(CRGB(255, 100, 240));
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() 
{
    updateEruptionState();
    updateCoolingAndDelay();
    updateLedStrips();

    FastLED.show();
    FastLED.delay(_coolingAndSpeedDelay);
}

void updateEruptionState()
{   
    int seconds = millis() / 1000;
    int currentMinute = seconds / 60 % RESET_INTERVAL_MINUTES;

    if (currentMinute < ERUPTION_LENGTH_MINUTES && _eruptionState != Erupting)
    {
        Serial.print("Erupting");
        Serial.println();

        _eruptionState = Erupting;
        _coolingAndSpeedDelay = 0;
        _lastCoolingIncrementTime = seconds;
    }
    else if (currentMinute >= ERUPTION_LENGTH_MINUTES && _eruptionState != Flowing)
    {
        Serial.print("Flowing");
        Serial.println();

        _eruptionState = Flowing;
    }
}

void updateCoolingAndDelay()
{
    if (_eruptionState != Flowing) return;
    if (_coolingAndSpeedDelay >= MAX_COOLING_AND_SPEED_DELAY) return;

    int seconds = millis() / 1000;

    if (seconds - _lastCoolingIncrementTime >= INCREMENT_INTERVAL_SECONDS)
    {
        _coolingAndSpeedDelay++;

        Serial.print("_coolingAndDelay: ");
        Serial.print(_coolingAndSpeedDelay);

        Serial.print(" seconds: ");
        Serial.print(seconds);
        Serial.println();

        _lastCoolingIncrementTime = seconds;
    }
}

void updateLedStrips()
{
    for (int i = 0; i < 8; i++)
    {
        _currentStripIndex = i;

        lavaFlow();
    }
}

void lavaFlow() 
{
    cooling();
	driftHeatDown();
	igniteNewSparks();
	setHeatToLeds();
}

void igniteNewSparks()
{
	// Randomly ignite new 'sparks' near the top
    if (random(255) < SPARK_CHANCE) 
	{
		int y = random(7);

		_heat[_currentStripIndex][y] = _heat[_currentStripIndex][y] + random(160, 255);
	}
}

void driftHeatDown()
{
	// Heat from each cell drifts 'down' and diffuses a little
    for (int i = NUM_LEDS - 1; i >= 2; i--) 
	{
		_heat[_currentStripIndex][i] = (_heat[_currentStripIndex][i - 1] + _heat[_currentStripIndex][i - 2] + _heat[_currentStripIndex][i - 3]) / 3;
	}
}

void cooling()
{
	// Cool down every cell a little
    if (_eruptionState == Erupting) return;

    for (int i = 0; i < NUM_LEDS; i++) 
    {
        byte currentHeat = _heat[_currentStripIndex][i];

        int cooldown = random(0, ((_coolingAndSpeedDelay * 10) / NUM_LEDS) + 2);
        
        _heat[_currentStripIndex][i] = (currentHeat < cooldown) ? 0 : currentHeat - cooldown;
    }
}

void setHeatToLeds()
{
	// Convert heat to LED colors
    for (int i = 0; i < NUM_LEDS; i++) 
	{
        _leds[_currentStripIndex][i] = HeatColor(_heat[_currentStripIndex][i]);
	}
}