/*
    Board: NodeMCU-ESP32s
	Upload Speed: 921600
	Pinout Reference: https://esphome.io/devices/nodemcu_esp32.html

    Notes:
    - Cannot use loop in addLeds function because the pin number must be a runtime constant. Gross, I know.
    - Ensure esp32 is grounded to psu, even if powered by seperate usb
    - ws2815 are unidirectional. You must connect to the female end of the strip to a male JST SM it from the board
    - Parrallel output  - writing to each pixel takes 30µs, so FastLED.show() takes a very long time because it has to iterate over all pixels and strips in series (ie one at a time)
                        - parrallel will write to strips in parrallel and must be enabled
                        - reddit explination: https://www.reddit.com/r/FastLED/comments/aqlb94/troubleshooting_slow_performance_tied_to/
                        - https://github.com/FastLED/FastLED/wiki/Parallel-Output
                        - solution: https://www.reddit.com/r/FastLED/comments/klw88g/are_there_magical_words_to_summon_parallel_output/
    - pins used (green wire / blue wire):
		- 19 / 18 
		- 5 / 17 
		- 4 / 0
		- 2 / 15

		- 32 / 33
		- 25 / 26
		- 27 / 14
		- 12 / 13
*/

#include <FastLED.h>

// Enable parrallel output
#define FASTLED_ESP32_I2S true
#define FASTLED_RMT_MAX_CHANNELS 8

#define COLOR_ORDER RGB
#define CHIPSET     WS2811
#define NUM_LEDS    300
#define NUM_STRIPS 	8

#define COOLING 55
#define SPARKLING 120

bool _isErupting;
int _lavaSpeed;

CRGB _leds[NUM_STRIPS][NUM_LEDS];
byte _heat[NUM_STRIPS][NUM_LEDS];

void setup() 
{
    _isErupting = false;
    _lavaSpeed = 1000;

	addLeds();
}

void addLeds()
{
	//left side
	FastLED.addLeds<CHIPSET, 19, COLOR_ORDER>(_leds[0], NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<CHIPSET, 18, COLOR_ORDER>(_leds[0], NUM_LEDS).setCorrection(TypicalLEDStrip);
	
	FastLED.addLeds<CHIPSET, 5, COLOR_ORDER>(_leds[1], NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<CHIPSET, 17, COLOR_ORDER>(_leds[1], NUM_LEDS).setCorrection(TypicalLEDStrip);

	FastLED.addLeds<CHIPSET, 4, COLOR_ORDER>(_leds[2], NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<CHIPSET, 0, COLOR_ORDER>(_leds[2], NUM_LEDS).setCorrection(TypicalLEDStrip);

	FastLED.addLeds<CHIPSET, 2, COLOR_ORDER>(_leds[3], NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<CHIPSET, 15, COLOR_ORDER>(_leds[3], NUM_LEDS).setCorrection(TypicalLEDStrip);

	//right side
	FastLED.addLeds<CHIPSET, 32, COLOR_ORDER>(_leds[4], NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<CHIPSET, 33, COLOR_ORDER>(_leds[4], NUM_LEDS).setCorrection(TypicalLEDStrip);

	FastLED.addLeds<CHIPSET, 25, COLOR_ORDER>(_leds[5], NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<CHIPSET, 26, COLOR_ORDER>(_leds[5], NUM_LEDS).setCorrection(TypicalLEDStrip);

	FastLED.addLeds<CHIPSET, 27, COLOR_ORDER>(_leds[6], NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<CHIPSET, 14, COLOR_ORDER>(_leds[6], NUM_LEDS).setCorrection(TypicalLEDStrip);

	FastLED.addLeds<CHIPSET, 12, COLOR_ORDER>(_leds[7], NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<CHIPSET, 13, COLOR_ORDER>(_leds[7], NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void loop() 
{
    for (int i = 0; i < 8; i++)
    {
        lavaFlow(i);
    }

    FastLED.show();
    // FastLED.delay(1000 / _lavaSpeed);
}

void lavaFlow(int stripIndex) 
{
	// Step 1.  Cool down every cell a little
	if (!_isErupting)
	{
		for (int i = 0; i < NUM_LEDS; i++) 
		{
			int cooldown = random(0, ((COOLING * 10) / NUM_LEDS) + 2);
			
			if (cooldown > _heat[stripIndex][i]) 
			{
				_heat[stripIndex][i] = 0;
			} 
			else 
			{
				_heat[stripIndex][i] = _heat[stripIndex][i] - cooldown;
			}
		}
	}
	
	// Step 2.  Heat from each cell drifts 'down' and diffuses a little
	for (int i = NUM_LEDS - 1; i >= 2; i--) 
	{
		_heat[stripIndex][i] = (_heat[stripIndex][i - 1] + _heat[stripIndex][i - 2] + _heat[stripIndex][i - 2]) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' near the top
	if (random(255) < SPARKLING) 
	{
		int y = random(7);

		_heat[stripIndex][y] = _heat[stripIndex][y] + random(160,255);
	}

	// Step 4.  Convert heat to LED colors
	for (int i = 0; i < NUM_LEDS; i++) 
	{
        _leds[stripIndex][i] = HeatColor(_heat[stripIndex][i]);
	}
}