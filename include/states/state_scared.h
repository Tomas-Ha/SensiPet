#pragma once

#include "sensipet_state.h"

class ScaredState : public SensiPetState
{
    public:
        ScaredState(string name) {
            this->name = name;
        }
        void init() override;
        void update(unsigned int delta_ms) override;
        void cleanup() override;
};