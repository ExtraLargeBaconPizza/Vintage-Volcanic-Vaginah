#include "LedStripControls.h"

CRGB _leds[NUM_STRIPS][NUM_LEDS];

void addLedStrips()
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
}

void setupLedSettings()
{
    FastLED.setCorrection(COLOR_CORRECTION);
    FastLED.setBrightness(BRIGHTNESS);
}

void SetupLedStrips()
{
    addLedStrips();
    setupLedSettings();
}

void SetLedColor(int stripIndex, int ledIndex, CRGB ledColor)
{
    _leds[stripIndex][ledIndex] = ledColor;
}

void UpdateLedStrips()
{
    FastLED.show();
}