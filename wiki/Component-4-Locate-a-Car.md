# Component 4: Locate a Car

## Purpose

Before a car can be retrieved it must first be found. This component answers a single question: given a customer identifier, where in the lot is that customer's car parked? It reports the garage and the row within that garage, or signals that the car is not present anywhere. The retrieval logic depends on this answer to know how many cars are stacked above the target and which garage to work in, so locating the car correctly is a prerequisite for every pickup.

## Data Storage and Maintenance

This component is purely a reader. It does not modify the lot in any way and introduces no data structures of its own. It examines the garage grid inside the shared `SystemState` and returns its answer as a pair of integers representing the garage index and the row index. A result of negative one in both positions indicates that the car could not be found, which lets the caller distinguish a genuine location from a failed search without any ambiguity.

## Functionality

The component provides one function, `locateCar`, which performs a straightforward linear search.

```cpp
std::pair<int,int> locateCar(const SystemState& state, int customerID);
```

The search walks every garage in turn, and within each garage examines every row, comparing the stored value against the requested customer identifier. The first slot whose value matches is the car's location, and the function returns that garage and row immediately. If the search completes without finding a match, the function returns the pair of negative ones to indicate absence, and the caller is expected to report that the customer identifier was not found in any garage. Because identifiers are unique and a car occupies exactly one slot, a single match is always definitive. The search is linear in the total number of slots, which is more than fast enough for the small lots this simulation models, and it keeps the component simple and easy to reason about.
