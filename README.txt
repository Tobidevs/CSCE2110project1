Name: Tobi Akere

Project: QParking - CSCE 2110 Section 400 (Summer 2026)
A valet parking lot simulation. Each garage is a LIFO stack. The program reads
an initial configuration, then executes a script of drop-off and pickup
interactions, printing the garage state after each one.

How to Compile:
  g++ -std=c++17 main.cpp config.cpp interactions.cpp parking.cpp locate.cpp status.cpp -o qparking
  OR: make

How to Run:
  ./qparking
  (Enter the configuration filename when prompted, e.g., config.txt)

Input files required (must be in the same directory):
  config.txt        provided by the user at runtime
  garage.csv        filename listed in config.txt line 2
  customers.csv     filename listed in config.txt line 3
  interactions.txt  fixed filename; the drop-off/pickup script

Configuration file format (config.txt), exactly 3 lines:
  Line 1: <m> <n>   number of garages and capacity per garage
  Line 2: garage occupancy CSV filename
  Line 3: customer info CSV filename

Interactions file format (interactions.txt), one interaction per line:
  D: name, phone, time   drop-off (a new customer is parked)
  P: <customerID>        pickup (that customer's car is retrieved)

Output:
  The initial garage state and the state after every interaction are written to
  standard output and to output.txt.

Components:
  1. config.{h,cpp}        configuration and initialization
  2. interactions.{h,cpp}  read and execute the interaction script
  3. parking.{h,cpp}       parking space assignment (drop-off)
  5. parking.{h,cpp}       lot reconfiguration on pickup (same files)
  4. locate.{h,cpp}        locate a car by customer id
  6. status.{h,cpp}        lot status and grid output

Makefile notes: No special flags required. Standard C++17.
