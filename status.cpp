#include "status.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

// Renders a slot value for display: -1, R, or the customer id.
static std::string cellToString(int value) {
    if (value == AVAILABLE) {
        return "-1";
    }
    if (value == RESTRICTED) {
        return "R";
    }
    return std::to_string(value);
}

// Prints the garage grid with the top row first and a header per garage.
static void writeGrid(const SystemState& state, std::ostream& out) {
    const int colWidth = 10;

    out << std::setw(8) << " ";
    for (int g = 0; g < state.m; ++g) {
        std::string header = "Garage " + std::to_string(g + 1);
        out << std::setw(colWidth) << header;
    }
    out << "\n";

    for (int row = state.n - 1; row >= 0; --row) {
        std::string label = "Row " + std::to_string(row + 1) + ":";
        out << std::setw(8) << std::left << label << std::right;
        for (int g = 0; g < state.m; ++g) {
            out << std::setw(colWidth) << cellToString(state.garages[g][row]);
        }
        out << "\n";
    }
}

int countAvailableSpaces(const SystemState& state) {
    int available = 0;
    for (int g = 0; g < state.m; ++g) {
        for (int row = 0; row < state.n; ++row) {
            if (state.garages[g][row] == AVAILABLE) {
                ++available;
            }
        }
    }
    return available;
}

bool checkLotFull(const SystemState& state) {
    return state.currentCars >= state.maxCars;
}

void printGarageGrid(const SystemState& state, std::ofstream& outFile) {
    writeGrid(state, outFile);
}

void outputGarageState(const SystemState& state, std::ofstream& outFile,
                       int interactionNum, const std::string& desc,
                       const std::string& result) {
    std::ostringstream out;

    if (interactionNum == 0) {
        out << "=== Initial Garage State ===\n";
    } else {
        out << "=== Interaction " << interactionNum << ": " << desc << " ===\n";
    }

    if (!result.empty()) {
        out << result << "\n";
    }

    writeGrid(state, out);

    out << "Total parked: " << state.currentCars << "\n";
    out << "Available spaces: " << countAvailableSpaces(state) << "\n";

    if (interactionNum == 0) {
        out << "Max capacity: " << state.maxCars << "\n";
    } else {
        out << "Lot status: " << (checkLotFull(state) ? "FULL" : "NOT FULL")
            << "\n";
    }
    out << "\n";

    std::cout << out.str();
    outFile << out.str();
}
