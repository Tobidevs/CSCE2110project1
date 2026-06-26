# Component 6: Parking Lot Status and Output

## Purpose

This component is responsible for everything the user sees. It renders the parking lot as a readable grid and reports the summary figures that describe the lot's condition, such as how many cars are parked, how many spaces remain, and whether the lot is full. It is called once after initialization to show the starting state of the lot, and then again after every interaction so that the effect of each drop-off and pickup is recorded. All of its output is sent to two destinations at once: the terminal, so the user can watch the simulation run, and the file `output.txt`, so a permanent record of the run is produced.

## Data Storage and Maintenance

This component is a reader and a formatter; it never modifies the simulation. It takes the shared `SystemState` by constant reference and derives everything it prints from the garage grid and the counts already maintained there. It introduces no persistent data structures of its own. The only transient state it uses is a string buffer assembled while formatting a single report, which is then written to both output destinations so that the terminal and the file stay perfectly in sync.

## Functionality

The component exposes four functions. The two small helpers, `countAvailableSpaces` and `checkLotFull`, compute the figures that appear in the summary. The first walks the grid and counts the slots whose value is the available marker, deliberately not counting restricted slots since those are never usable. The second reports whether the lot has reached capacity by comparing the running car count against the maximum.

```cpp
void outputGarageState(const SystemState& state, std::ofstream& outFile,
                       int interactionNum, const std::string& desc);
int countAvailableSpaces(const SystemState& state);
bool checkLotFull(const SystemState& state);
void printGarageGrid(const SystemState& state, std::ofstream& outFile);
```

The function `printGarageGrid` lays out the lot as a table. It prints a header row naming each garage, then prints one row per parking level. The levels are printed from the top of the garage downward, so the highest row appears first and the bottom row last, which matches the way a person would look at a physical stack. Each cell shows the value in that slot, translating the internal encoding back into something readable: an available slot prints as a dash-one, a restricted slot prints as the letter R, and an occupied slot prints the identifier of the customer parked there. Internally the formatting is shared so that the same grid can be sent to either an in-memory buffer or a file without duplicating the layout logic.

The main function, `outputGarageState`, produces a complete report. It writes a heading that identifies either the initial state or the numbered interaction being shown, calls the grid renderer, and then appends the summary lines for the number of cars parked and the number of available spaces. For the initial state it also prints the maximum capacity, and for an interaction it prints whether the lot is now full or not full. The entire report is built once into a string buffer and then written to both the terminal and the output file, guaranteeing that the on-screen view and the saved record are identical.
