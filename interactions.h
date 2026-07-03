#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include "config.h"
#include <fstream>
#include <string>

// Component 2: Read and execute the interaction script (interactions.txt).

// Reads every line of the interaction file and dispatches each one.
void processInteractions(SystemState& state, std::ofstream& outFile);

// Handles a "D: name, phone, time" line: registers a new customer and parks it.
void parseDropOff(const std::string& line, SystemState& state,
                  std::ofstream& outFile, int interactionNum);

// Handles a "P: <customerID>" line: retrieves the named customer's car.
void parsePickUp(const std::string& line, SystemState& state,
                 std::ofstream& outFile, int interactionNum);

#endif
