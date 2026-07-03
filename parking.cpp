#include "parking.h"
#include "locate.h"

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

// --- Component 5 helpers ---

// Pushes a car onto the top of garage g's stack: the lowest open slot that sits
// above every occupied or restricted slot. This keeps freshly staged cars above
// any cars already in the garage, so they pop off first. Returns the row, or -1.
static int pushOnTop(SystemState& state, int g, int car) {
    int top = -1;
    for (int row = state.n - 1; row >= 0; --row) {
        if (state.garages[g][row] != AVAILABLE) {
            top = row;
            break;
        }
    }
    for (int row = top + 1; row < state.n; ++row) {
        if (state.garages[g][row] == AVAILABLE) {
            state.garages[g][row] = car;
            return row;
        }
    }
    return -1;
}

// Pops the topmost car off garage g's stack (highest occupied slot). Restricted
// slots are skipped. Returns the car's id, or -1 if the garage holds no car.
static int popFromGarage(SystemState& state, int g) {
    for (int row = state.n - 1; row >= 0; --row) {
        if (state.garages[g][row] > 0) {
            int car = state.garages[g][row];
            state.garages[g][row] = AVAILABLE;
            return car;
        }
    }
    return -1;
}

// Finds a garage other than source that can stage at least `needed` cars.
// Capacity is measured as the slots reachable from the top of the stack (those
// above the topmost occupied or restricted cell), since a stack push cannot
// reach an empty slot buried below one. Returns its index, or -1 if none fits.
static int findTempGarage(const SystemState& state, int source, int needed) {
    for (int g = 0; g < state.m; ++g) {
        if (g == source) {
            continue;
        }
        int top = -1;
        for (int row = state.n - 1; row >= 0; --row) {
            if (state.garages[g][row] != AVAILABLE) {
                top = row;
                break;
            }
        }
        int stackable = state.n - 1 - top;
        if (stackable >= needed) {
            return g;
        }
    }
    return -1;
}

void moveCarsAbove(SystemState& state, int garageIdx, int targetRow,
                   int tempGarageIdx) {
    // Work top down so the highest car is staged first and ends up on top of the
    // temporary stack.
    for (int row = state.n - 1; row > targetRow; --row) {
        if (state.garages[garageIdx][row] > 0) {
            int car = state.garages[garageIdx][row];
            state.garages[garageIdx][row] = AVAILABLE;
            pushOnTop(state, tempGarageIdx, car);
        }
    }
}

void restoreCars(SystemState& state, int tempGarageIdx, int garageIdx,
                 int count) {
    // Popping the temp stack top down and re-pushing preserves the cars'
    // original bottom-to-top order in the source garage.
    for (int i = 0; i < count; ++i) {
        int car = popFromGarage(state, tempGarageIdx);
        if (car < 0) {
            break;
        }
        pushOnTop(state, garageIdx, car);
    }
}

bool retrieveCar(SystemState& state, int customerID) {
    std::pair<int, int> loc = locateCar(state, customerID);
    int g = loc.first;
    int targetRow = loc.second;
    if (g < 0) {
        return false;  // not parked anywhere
    }

    // Count the cars stacked above the target that must be moved aside.
    int count = 0;
    for (int row = targetRow + 1; row < state.n; ++row) {
        if (state.garages[g][row] > 0) {
            ++count;
        }
    }

    // Reserve a temporary garage up front so we never move cars we cannot undo.
    int temp = -1;
    if (count > 0) {
        temp = findTempGarage(state, g, count);
        if (temp < 0) {
            return false;
        }
        moveCarsAbove(state, g, targetRow, temp);
    }

    // Remove the target car; it leaves the lot.
    state.garages[g][targetRow] = AVAILABLE;
    --state.currentCars;

    // Put the displaced cars back onto their original garage.
    if (count > 0) {
        restoreCars(state, temp, g, count);
    }

    return true;
}
