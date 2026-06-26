#include "config.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

SystemState initializeSystem(const std::string& configFile) {
    SystemState state;
    state.m = 0;
    state.n = 0;
    state.maxCars = 0;
    state.currentCars = 0;

    std::ifstream cfg(configFile);
    if (!cfg.is_open()) {
        std::cerr << "ERROR: Cannot open config file '" << configFile << "'."
                  << std::endl;
        std::exit(1);
    }

    std::string line;
    if (!std::getline(cfg, line)) {
        std::cerr << "ERROR: Config file '" << configFile
                  << "' is empty (expected line 1: <m> <n>)." << std::endl;
        std::exit(1);
    }
    {
        std::istringstream iss(trim(line));
        if (!(iss >> state.m >> state.n)) {
            std::cerr << "ERROR: Config line 1 must be '<m> <n>' (two integers)."
                      << std::endl;
            std::exit(1);
        }
    }
    if (state.m <= 0 || state.n <= 0) {
        std::cerr << "ERROR: m and n must be positive (got m=" << state.m
                  << ", n=" << state.n << ")." << std::endl;
        std::exit(1);
    }

    std::string garageFile;
    if (!std::getline(cfg, garageFile)) {
        std::cerr << "ERROR: Config file missing line 2 (garage CSV filename)."
                  << std::endl;
        std::exit(1);
    }
    garageFile = trim(garageFile);

    std::string customerFile;
    if (!std::getline(cfg, customerFile)) {
        std::cerr << "ERROR: Config file missing line 3 (customer CSV filename)."
                  << std::endl;
        std::exit(1);
    }
    customerFile = trim(customerFile);

    cfg.close();

    state.maxCars = state.n * (state.m - 1);
    state.garages.assign(state.m, std::vector<int>(state.n, AVAILABLE));

    readGarageOccupancy(state, garageFile);
    readCustomerInfo(state, customerFile);

    return state;
}

void readGarageOccupancy(SystemState& state, const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "ERROR: Cannot open garage CSV '" << filename << "'."
                  << std::endl;
        std::exit(1);
    }

    state.currentCars = 0;

    std::string line;
    int fileRow = 0;

    while (std::getline(in, line)) {
        if (trim(line).empty()) {
            continue;
        }

        if (fileRow >= state.n) {
            std::cerr << "WARNING: Garage CSV has more than n=" << state.n
                      << " rows; ignoring extra row " << (fileRow + 1) << "."
                      << std::endl;
            ++fileRow;
            continue;
        }

        int rowIdx = state.n - 1 - fileRow;

        std::istringstream iss(line);
        std::string cell;
        int col = 0;
        while (std::getline(iss, cell, ',')) {
            if (col >= state.m) {
                std::cerr << "WARNING: Garage CSV row " << (fileRow + 1)
                          << " has more than m=" << state.m
                          << " columns; ignoring extra cells." << std::endl;
                break;
            }

            std::string token = trim(cell);
            int value;
            if (token == "R" || token == "r") {
                value = RESTRICTED;
            } else if (token.empty() || token == "-1") {
                value = AVAILABLE;
            } else {
                value = std::atoi(token.c_str());
                if (value > 0) {
                    ++state.currentCars;
                }
            }
            state.garages[col][rowIdx] = value;
            ++col;
        }

        if (col < state.m) {
            std::cerr << "WARNING: Garage CSV row " << (fileRow + 1)
                      << " has only " << col << " of m=" << state.m
                      << " columns; remaining slots left available." << std::endl;
        }

        ++fileRow;
    }

    in.close();
}

void readCustomerInfo(SystemState& state, const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "ERROR: Cannot open customer CSV '" << filename << "'."
                  << std::endl;
        std::exit(1);
    }

    state.customers.assign(1, Customer{0, "", "", "", ""});

    std::string line;
    while (std::getline(in, line)) {
        if (trim(line).empty()) {
            continue;
        }

        std::vector<std::string> fields;
        std::istringstream iss(line);
        std::string field;
        while (std::getline(iss, field, ',')) {
            fields.push_back(trim(field));
        }

        if (fields.empty() || fields[0].empty()) {
            continue;
        }

        Customer c;
        c.id            = std::atoi(fields[0].c_str());
        c.name          = (fields.size() > 1) ? fields[1] : "";
        c.phone         = (fields.size() > 2) ? fields[2] : "";
        c.arrivalTime   = (fields.size() > 3) ? fields[3] : "";
        c.departureTime = (fields.size() > 4) ? fields[4] : "";

        if (c.id <= 0) {
            std::cerr << "WARNING: Skipping customer row with non-positive id '"
                      << fields[0] << "'." << std::endl;
            continue;
        }

        if (static_cast<size_t>(c.id) >= state.customers.size()) {
            state.customers.resize(c.id + 1, Customer{0, "", "", "", ""});
        }
        state.customers[c.id] = c;
    }

    in.close();
}
