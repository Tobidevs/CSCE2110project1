# Component 3: Parking Space Assignment

## Purpose

This component handles the drop-off side of the simulation. When a customer arrives and needs to park, it decides whether the lot has room and, if it does, chooses a garage and places the car. Because each garage behaves as a last-in, first-out stack, a newly arriving car is always added to the top of whichever garage receives it. The component is also the guardian of the lot's capacity rules: it refuses to park a car when doing so would exceed the maximum number of cars the lot is allowed to hold, ensuring that enough free space always remains for later retrieval maneuvers.

## Data Storage and Maintenance

Like the other behavioral components, this one does not own any separate data structure. It reads and modifies the shared `SystemState`, specifically the garage grid and the running `currentCars` count. When it parks a car it writes the customer's identifier into the chosen slot and increments `currentCars`, so the count and the grid never disagree. The capacity cap it enforces, `maxCars`, was computed once during initialization as `n * (m - 1)` and is simply consulted here rather than recomputed.

## Functionality

The component exposes three functions. The predicate `isLotFull` reports whether the lot has reached its capacity by comparing `currentCars` against `maxCars`. The function `findAvailableGarage` scans the garages and returns the index of one that still has at least one available slot, ignoring restricted markers since those can never hold a car. The main function, `assignParkingSpace`, ties these together.

```cpp
bool assignParkingSpace(SystemState& state, int customerID);
int findAvailableGarage(const SystemState& state);
bool isLotFull(const SystemState& state);
```

When asked to park a customer, `assignParkingSpace` first checks whether the lot is full. If it is, it reports that the lot cannot accept the car and returns without modifying the state. Otherwise it selects an eligible garage and finds the lowest available slot in that garage, which is the correct position for a stack that fills from the bottom up. It writes the customer's identifier into that slot, increments the running count of parked cars, and returns success. Because the lot reserves the equivalent of one full garage through the `n * (m - 1)` cap, there is always room to set cars aside later when a buried car must be reached.
