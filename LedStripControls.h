#ifndef LED_STRIP_CONTROLS_h
#define LED_STRIP_CONTROLS_h

#include <Arduino.h>
#include <FastLED.h>

#include "VintageVolcanicVaginah.h"

// Enable Parrallel Output
#define FASTLED_ESP32_I2S        true
#define FASTLED_RMT_MAX_CHANNELS NUM_STRIPS

// LED Settings
#define CHIPSET          WS2811
#define COLOR_ORDER      RGB
#define COLOR_CORRECTION CRGB(255, 100, 240)
#define BRIGHTNESS       255

void SetupLedStrips();
void SetLedColor(int stripIndex, int ledIndex, CRGB ledColor);
void UpdateLedStrips();

#endif