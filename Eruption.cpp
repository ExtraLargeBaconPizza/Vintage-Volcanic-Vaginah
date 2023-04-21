#include "Eruption.h"

EruptionState _currentEruptionState;

int _numberOfIncrements;
int _cooling;
int _speedDelay;
int _lastIncrementTime;

void updateEruptionState()
{   
    int seconds = millis() / 1000;
    int currentMinute = seconds / 60 % ERUPTION_RESET_INTERVAL_MINUTES;

    if (currentMinute < ERUPTION_LENGTH_MINUTES && _currentEruptionState != Erupting)
    {
        _currentEruptionState = Erupting;

        _numberOfIncrements = 0;
        _cooling = 0;
        _speedDelay = 0;
        _lastIncrementTime = seconds;
    }
    else if (currentMinute >= ERUPTION_LENGTH_MINUTES && _currentEruptionState != Flowing)
    {
        _currentEruptionState = Flowing;
    }
}

void updateCoolingAndDelay()
{
    if (_currentEruptionState != Flowing) return;
    if (_numberOfIncrements >= MAX_NUMBER_OF_INCREMENTS) return;

    int seconds = millis() / 1000;

    if (seconds - _lastIncrementTime >= INCREMENT_INTERVAL_SECONDS)
    {
        _numberOfIncrements++;
        
        _cooling += COOLING_INCREMENT;
        _speedDelay += SPEED_DELAY_INCREMENT;

        _lastIncrementTime = seconds;
    }
}

EruptionState GetCurrentEruptionState()
{
    return _currentEruptionState;
}

int GetCooling()
{
    return _cooling;
}

int GetSpeedDelay()
{
    return _speedDelay;
}

void UpdateEruption()
{
    updateEruptionState();
    updateCoolingAndDelay();
}