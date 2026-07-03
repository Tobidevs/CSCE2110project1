#include "config.h"
#include "interactions.h"
#include "status.h"
#include <iostream>
#include <string>
#include <fstream>

int main() {
    // The only user prompt: the config filename. All other filenames come from
    // the config file or are fixed by the spec.
    std::string configFile;
    std::cout << "Enter the configuration filename: ";
    std::cin >> configFile;

    // Component 1: build the initial state from the input files.
    SystemState state = initializeSystem(configFile);

    // All output is mirrored to output.txt.
    std::ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        std::cerr << "ERROR: Cannot open output.txt for writing." << std::endl;
        return 1;
    }

    // Component 6: initial state, then Component 2: run the interaction script.
    outputGarageState(state, outFile, 0, "Initial State");
    processInteractions(state, outFile);

    outFile.close();
    return 0;
}
