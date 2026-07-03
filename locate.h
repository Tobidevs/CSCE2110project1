#ifndef LOCATE_H
#define LOCATE_H

#include "config.h"
#include <utility>

// Component 4: Locate a car.

// Returns the {garage, row} of the customer's car, or {-1, -1} if it is not
// parked anywhere in the lot.
std::pair<int, int> locateCar(const SystemState& state, int customerID);

#endif
