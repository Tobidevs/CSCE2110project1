#include "interactions.h"

#include "parking.h"
#include "status.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

// Removes surrounding whitespace from a string.
static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Splits a comma-separated string and trims each field.
static std::vector<std::string> splitFields(const std::string& s) {
    std::vector<std::string> fields;
    std::istringstream iss(s);
    std::string field;
    while (std::getline(iss, field, ',')) {
        fields.push_back(trim(field));
    }
    return fields;
}

// Returns the {garage, row} of a parked car, or {-1,-1} if not found.
static std::pair<int, int> findParkedCar(const SystemState& state, int id) {
    for (int g = 0; g < state.m; ++g) {
        for (int row = 0; row < state.n; ++row) {
            if (state.garages[g][row] == id) {
                return {g, row};
            }
        }
    }
    return {-1, -1};
}

void parseDropOff(const std::string& line, SystemState& state,
                  std::ofstream& outFile, int interactionNum) {
    // Everything after the "D:" marker is "name, phone, time".
    std::string rest = trim(line.substr(line.find(':') + 1));
    std::vector<std::string> fields = splitFields(rest);

    // A drop-off customer is new, so give it the next id. The customer list is
    // 1-indexed and contiguous, so the next id equals its current size.
    Customer c;
    c.id            = static_cast<int>(state.customers.size());
    c.name          = (fields.size() > 0) ? fields[0] : "";
    c.phone         = (fields.size() > 1) ? fields[1] : "";
    c.arrivalTime   = (fields.size() > 2) ? fields[2] : "";
    c.departureTime = "";
    state.customers.push_back(c);

    // Component 3 places the car; report where it landed, or that the lot is full.
    std::string result;
    if (assignParkingSpace(state, c.id)) {
        std::pair<int, int> loc = findParkedCar(state, c.id);
        std::ostringstream msg;
        msg << "Result: Customer " << c.name << " (ID: " << c.id
            << ") parked in Garage " << (loc.first + 1)
            << ", Row " << (loc.second + 1) << ".";
        result = msg.str();
    } else {
        result = "Result: Lot is FULL. Cannot park Customer " + c.name + ".";
    }

    outputGarageState(state, outFile, interactionNum, line, result);
}

void parsePickUp(const std::string& line, SystemState& state,
                 std::ofstream& outFile, int interactionNum) {
    // The value after the "P:" marker is the customer id to retrieve.
    std::string idText = trim(line.substr(line.find(':') + 1));
    int id = std::atoi(idText.c_str());

    std::string result = "Result: Pickup requested for Customer ID "
        + std::to_string(id) + ".";

    outputGarageState(state, outFile, interactionNum, line, result);
}

void processInteractions(SystemState& state, std::ofstream& outFile) {
    std::ifstream in("interactions.txt");
    if (!in.is_open()) {
        std::cerr << "ERROR: Cannot open interactions file 'interactions.txt'."
                  << std::endl;
        std::exit(1);
    }

    // Read each interaction in order, dispatching on the leading marker.
    std::string line;
    int interactionNum = 0;
    while (std::getline(in, line)) {
        std::string t = trim(line);
        if (t.empty()) {
            continue;
        }

        ++interactionNum;
        if (t[0] == 'D' || t[0] == 'd') {
            parseDropOff(line, state, outFile, interactionNum);
        } else if (t[0] == 'P' || t[0] == 'p') {
            parsePickUp(line, state, outFile, interactionNum);
        } else {
            std::cerr << "WARNING: Unrecognized interaction on line "
                      << interactionNum << ": '" << line << "'." << std::endl;
            --interactionNum;
        }
    }

    in.close();
}
