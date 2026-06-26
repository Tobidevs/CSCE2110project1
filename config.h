#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

const int AVAILABLE  = -1;
const int RESTRICTED = -2;

struct Customer {
    int id;
    std::string name;
    std::string phone;
    std::string arrivalTime;
    std::string departureTime;
};

struct SystemState {
    int m;
    int n;
    int maxCars;
    int currentCars;
    std::vector<std::vector<int>> garages;
    std::vector<Customer> customers;
};

SystemState initializeSystem(const std::string& configFile);
void readGarageOccupancy(SystemState& state, const std::string& filename);
void readCustomerInfo(SystemState& state, const std::string& filename);

#endif
