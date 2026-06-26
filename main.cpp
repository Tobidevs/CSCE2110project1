#include "config.h"
#include "status.h"
#include <iostream>
#include <string>
#include <fstream>

int main() {
    std::string configFile;
    std::cout << "Enter the configuration filename: ";
    std::cin >> configFile;

    SystemState state = initializeSystem(configFile);

    std::ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        std::cerr << "ERROR: Cannot open output.txt for writing." << std::endl;
        return 1;
    }

    outputGarageState(state, outFile, 0, "Initial State");

    outFile.close();
    return 0;
}
