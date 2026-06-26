# Component 2: Read and Execute Interactions

## Purpose

Once the simulation has been initialized, this component drives it forward by replaying a script of customer interactions. The script lives in a text file, with each interaction written on its own line. Two kinds of interaction are supported: a drop-off, in which a new customer arrives and needs a parking space, and a pickup, in which an existing customer returns to collect their car. Component 2 is the coordinator for this phase of the program. It does not decide where a car is parked or how a buried car is retrieved; instead it reads each line, determines which kind of interaction it represents, and hands the work off to the components that specialize in parking and retrieval. After each interaction has been carried out it triggers the status output so that the lot's condition is recorded at every step.

## Data Storage and Maintenance

This component does not introduce any new long-lived data structures. It operates on the shared `SystemState` produced by Component 1 and passes it by reference to every function it calls, so that drop-offs and pickups modify the single authoritative copy of the lot. It also holds the output file stream and forwards it to the status component after each interaction. The only transient data it manages are the parsed fields of the current line, such as the name, phone, and time of a drop-off, or the customer identifier of a pickup. Drop-offs introduce new customers that did not appear in the original customer file; for these, the component assigns a fresh identifier equal to one more than the current largest identifier and appends the new `Customer` record to the shared customer list, keeping the direct identifier-to-position relationship intact.

## Functionality

The entry point, `processInteractions`, opens the interactions file, which is expected to be named `interactions.txt` and to live in the same directory as the program. It reads the file line by line. For each line it inspects the leading marker: a line beginning with `D:` is a drop-off and a line beginning with `P:` is a pickup. Drop-off lines are passed to `parseDropOff` and pickup lines to `parsePickUp`, along with the shared state, the output stream, and the sequence number of the interaction so that the output can be labeled.

```cpp
void processInteractions(SystemState& state, std::ofstream& outFile);
void parseDropOff(const std::string& line, SystemState& state,
                  std::ofstream& outFile, int interactionNum);
void parsePickUp(const std::string& line, SystemState& state,
                 std::ofstream& outFile, int interactionNum);
```

The function `parseDropOff` extracts the customer's name, phone number, and arrival time from the remainder of the line, creates a new customer record with a freshly assigned identifier, and calls the parking assignment logic in Component 3 to place the car. The function `parsePickUp` reads the customer identifier that follows the marker and invokes the locate and retrieval logic in Components 4 and 5 to find and remove that customer's car. In both cases, once the parking or retrieval work is complete, control returns and the status component is invoked to print the updated grid and the lot summary. By keeping the parsing and dispatch logic here and delegating the actual lot manipulation elsewhere, this component stays focused on reading the script correctly and sequencing the work in the right order.
