#include "states/state_sleep.h"
#include <cstdio>

void SleepState::init()
{
    printf("Initializing sleep state.\n");
}

void SleepState::update(unsigned int delta_ms)
{
    printf("Updating sleep state.\n");
}

void SleepState::cleanup()
{
    printf("Cleaning up sleep state.\n");
}