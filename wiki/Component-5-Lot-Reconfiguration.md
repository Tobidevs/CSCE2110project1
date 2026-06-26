# Component 5: Lot Reconfiguration

## Purpose

This component handles the difficult half of a pickup: actually removing a customer's car from a garage that behaves as a last-in, first-out stack. If the requested car happens to be on top of its stack, retrieval is trivial. The interesting case is when other cars are parked above it. Because a stack only allows access from the top, those cars must be moved out of the way first, the target car removed, and the displaced cars then put back. This component carries out that reconfiguration, using the restricted slots and the deliberately reserved free space in the lot to stage the temporary moves without ever exceeding capacity.

## Data Storage and Maintenance

The component works entirely on the shared `SystemState`, modifying the garage grid and keeping the `currentCars` count accurate as the target car leaves the lot. During a retrieval it needs somewhere to hold the cars it lifts off the target. It uses another garage as temporary storage, chosen so that it has enough free space to receive the displaced cars, and it tracks how many cars it moved so they can all be restored afterward. No permanent structure is introduced; the temporary storage is just ordinary garage slots that are occupied briefly and then freed again once the displaced cars return to their original garage. The restricted markers loaded during initialization remain fixed throughout and are never overwritten, since their whole purpose is to keep retrieval maneuvers possible.

## Functionality

The component is built from three cooperating functions.

```cpp
bool retrieveCar(SystemState& state, int customerID);
void moveCarsAbove(SystemState& state, int garageIdx, int targetRow, int tempGarageIdx);
void restoreCars(SystemState& state, int tempGarageIdx, int garageIdx, int count);
```

The main function, `retrieveCar`, begins by calling Component 4 to locate the requested car. With the garage and target row in hand, it counts how many occupied slots sit above the target. It then selects a different garage with enough room to act as temporary storage and calls `moveCarsAbove`, which lifts the cars sitting above the target off the source garage, from the top downward, and pushes them onto the temporary garage. With the path cleared, the target slot is set back to available and the running car count is decremented, completing the customer's pickup. Finally `restoreCars` reverses the staging step, popping the displaced cars off the temporary garage and pushing them back onto the source garage so that their relative order is preserved. The reserved capacity from the `n * (m - 1)` rule guarantees that a suitable temporary garage always exists, so the maneuver can be completed no matter how deeply a car is buried.
