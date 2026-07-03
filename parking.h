#ifndef PARKING_H
#define PARKING_H

#include "config.h"

// Component 3: Parking space assignment (drop-off side of the simulation).

// True once the lot has reached its capacity cap of n*(m-1) cars.
bool isLotFull(const SystemState& state);

// Index of the first garage (left to right) that has an open slot, or -1.
int findAvailableGarage(const SystemState& state);

// Parks customerID in the lowest open slot of an eligible garage.
// Returns false if the lot is full. Updates currentCars on success.
bool assignParkingSpace(SystemState& state, int customerID);

#endif
