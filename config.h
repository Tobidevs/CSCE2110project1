#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

// Sentinel values stored in a garage slot; any other (positive) value is a
// customer id occupying that slot.
const int AVAILABLE  = -1;  // empty slot
const int RESTRICTED = -2;  // permanent "R" slot; no car may park here

struct Customer {
    int id;
    std::string name;
    std::string phone;
    std::string arrivalTime;
    std::string departureTime;  // empty until the customer picks up
};

// The single authoritative state of the simulation, shared by all components.
struct SystemState {
    int m;                                  // number of garages
    int n;                                  // capacity (rows) per garage
    int maxCars;                            // capacity cap = n*(m-1)
    int currentCars;                        // cars currently parked
    std::vector<std::vector<int>> garages;  // garages[garage][row], row 0 = bottom
    std::vector<Customer> customers;        // 1-indexed; index 0 is unused
};

// Component 1: parse the config file and build the initial SystemState.
SystemState initializeSystem(const std::string& configFile);
void readGarageOccupancy(SystemState& state, const std::string& filename);
void readCustomerInfo(SystemState& state, const std::string& filename);

#endif
