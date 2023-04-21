#ifndef LAVA_FLOW_H
#define LAVA_FLOW_H

#include <Arduino.h>
#include <FastLED.h>

#include "Eruption.h"
#include "LedStripControls.h"
#include "VintageVolcanicVaginah.h"

// LavaFlow Settings
#define SPARK_CHANCE_MAX_ROLL  255
#define SPARK_CHANCE_THRESHOLD 120
#define SPARK_HEAT_MIN         160
#define SPARK_HEAT_MAX         255

void UpdateLavaFlow();

#endif