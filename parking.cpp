#include "parking.h"

// The capacity cap (maxCars = n*(m-1)) reserves one garage's worth of space so
// there is always room to shuffle cars aside during a retrieval.
bool isLotFull(const SystemState& state) {
    return state.currentCars >= state.maxCars;
}

// Restricted (R) cells never count as available since a car cannot park there.
int findAvailableGarage(const SystemState& state) {
    for (int g = 0; g < state.m; ++g) {
        for (int row = 0; row < state.n; ++row) {
            if (state.garages[g][row] == AVAILABLE) {
                return g;
            }
        }
    }
    return -1;
}

bool assignParkingSpace(SystemState& state, int customerID) {
    if (isLotFull(state)) {
        return false;
    }

    int g = findAvailableGarage(state);
    if (g < 0) {
        return false;
    }

    // Fill from the bottom up: the lowest open row is the top of a LIFO stack
    // that has been packed contiguously.
    for (int row = 0; row < state.n; ++row) {
        if (state.garages[g][row] == AVAILABLE) {
            state.garages[g][row] = customerID;
            ++state.currentCars;
            return true;
        }
    }

    return false;
}
