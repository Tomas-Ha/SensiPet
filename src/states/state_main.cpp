#include "states/state_main.h"
#include <cstdio>

void MainState::init()
{
    printf("Main state initialized.\n");
}

void MainState::update(unsigned int delta_ms)
{
    printf("Main state update called.\n");
}

void MainState::cleanup()
{
    printf("Main state cleaned up.\n");
}