#include "LavaFlow.h"

byte _heat[NUM_STRIPS][NUM_LEDS];

int _currentStripIndex;

void igniteNewSparks()
{
	// Randomly ignite new 'sparks' near the top
    if (random(SPARK_CHANCE_MAX_ROLL) < SPARK_CHANCE_THRESHOLD) 
	{
		int y = random(7);

		_heat[_currentStripIndex][y] = _heat[_currentStripIndex][y] + random(SPARK_HEAT_MIN, SPARK_HEAT_MAX);
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
    if (GetCurrentEruptionState() == Erupting) return;

    for (int i = 0; i < NUM_LEDS; i++) 
    {
        byte currentHeat = _heat[_currentStripIndex][i];

        int cooldown = random(0, ((GetCooling()) / NUM_LEDS) + 2);
        
        _heat[_currentStripIndex][i] = (currentHeat < cooldown) ? 0 : currentHeat - cooldown;
    }
}

void setHeatToLeds()
{
	// Convert heat to LED colors
    for (int i = 0; i < NUM_LEDS; i++) 
	{
        CRGB heatColor = HeatColor(_heat[_currentStripIndex][i]);
        
        SetLedColor(_currentStripIndex, i, heatColor);
	}
}

void UpdateLavaFlow()
{
    for (int i = 0; i < NUM_STRIPS; i++)
    {
        _currentStripIndex = i;

        igniteNewSparks();
        driftHeatDown();
        cooling();
        setHeatToLeds();
    }
}