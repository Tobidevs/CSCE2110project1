#include "locate.h"

// Linear search over every slot. Ids are unique and a car occupies exactly one
// slot, so the first match is definitive.
std::pair<int, int> locateCar(const SystemState& state, int customerID) {
    for (int g = 0; g < state.m; ++g) {
        for (int row = 0; row < state.n; ++row) {
            if (state.garages[g][row] == customerID) {
                return {g, row};
            }
        }
    }
    return {-1, -1};
}
