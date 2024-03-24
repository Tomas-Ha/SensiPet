#pragma once

#include "sensipet_state.h"

class SleepState : public SensiPetState
{
    public:
        void init() override;
        void update(unsigned int delta_ms) override;
        void cleanup() override;
};