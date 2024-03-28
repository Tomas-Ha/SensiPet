#pragma once

#include "sensipet_state.h"

class EatingState : public SensiPetState
{
    public:
        EatingState(string name) {
            this->name = name;
        }
        void init() override;
        void update(unsigned int delta_ms) override;
        void cleanup() override;
};