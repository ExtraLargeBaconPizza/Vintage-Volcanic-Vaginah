#ifndef ERUPTION_H
#define ERUPTION_H

#include <Arduino.h>

#define ERUPTION_LENGTH_MINUTES     1
#define MAX_NUMBER_OF_INCREMENTS    24
#define SPEED_DELAY_INCREMENT       1
#define COOLING_INCREMENT           10
#define INCREMENT_INTERVAL_SECONDS  10
#define RESET_INTERVAL_MINUTES      20

enum EruptionState 
{
    Unknown,
    Erupting,
    Flowing,
};

EruptionState GetCurrentEruptionState();
int GetCooling();
int GetSpeedDelay();

void UpdateEruption();

#endif