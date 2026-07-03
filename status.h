#ifndef STATUS_H
#define STATUS_H

#include "config.h"
#include <fstream>
#include <string>

// Component 6: report lot status and print the garage grid.

// Writes the header, an optional result line, the grid, and the lot summary to
// both stdout and outFile. interactionNum 0 prints the initial-state block.
void outputGarageState(const SystemState& state, std::ofstream& outFile,
                       int interactionNum, const std::string& desc,
                       const std::string& result = "");
// Number of empty (-1) slots across all garages.
int countAvailableSpaces(const SystemState& state);
// True when the parked-car count has reached the capacity cap.
bool checkLotFull(const SystemState& state);
void printGarageGrid(const SystemState& state, std::ofstream& outFile);

#endif
