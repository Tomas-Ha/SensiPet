#include "mbed.h"
#include "sensipet.h"
#include "sensipet_state.h"
#include "screen/globals.h"
#include "microphone/globals.h"
#include "states/globals.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>

void init()
{
    // TODO: Should be checking for errors here.
    init_microphone();

    setup_states();
}

int main()
{
    init();

    // Set the default state for the SensiPet, this should dispatch forever by default.
    gSensiPet.set_current_state(&gMainState);

    return 0;
}