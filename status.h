#ifndef STATUS_H
#define STATUS_H

#include "config.h"
#include <fstream>
#include <string>

void outputGarageState(const SystemState& state, std::ofstream& outFile,
                       int interactionNum, const std::string& desc);
int countAvailableSpaces(const SystemState& state);
bool checkLotFull(const SystemState& state);
void printGarageGrid(const SystemState& state, std::ofstream& outFile);

#endif
