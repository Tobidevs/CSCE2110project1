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

// Component 5: Lot reconfiguration (pickup side of the simulation).

// Removes customerID's car from its LIFO stack, temporarily relocating any cars
// stacked above it and then restoring them. Returns false if the car is not
// found or no garage can stage the displaced cars. Updates currentCars.
bool retrieveCar(SystemState& state, int customerID);

// Lifts every car stacked above targetRow off garageIdx (top down) and stacks
// them onto tempGarageIdx.
void moveCarsAbove(SystemState& state, int garageIdx, int targetRow,
                   int tempGarageIdx);

// Pops count cars back off tempGarageIdx and restacks them onto garageIdx,
// preserving their relative order.
void restoreCars(SystemState& state, int tempGarageIdx, int garageIdx,
                 int count);

#endif
