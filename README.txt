Name: Tobi Akere

Project: QParking - CSCE 2110 Section 400 (Summer 2026)
Milestone 1: Configuration, Initialization, and Initial State Output

How to Compile:
  g++ -std=c++17 main.cpp config.cpp status.cpp -o qparking
  OR: make

How to Run:
  ./qparking
  (Enter the configuration filename when prompted, e.g., config.txt)

Input files required (must be in the same directory):
  config.txt        provided by the user at runtime
  garage.csv        filename listed in config.txt line 2
  customers.csv     filename listed in config.txt line 3

Configuration file format (config.txt), exactly 3 lines:
  Line 1: <m> <n>   number of garages and capacity per garage
  Line 2: garage occupancy CSV filename
  Line 3: customer info CSV filename

Output:
  The initial garage state is written to standard output and to output.txt.

Makefile notes: No special flags required. Standard C++17.
