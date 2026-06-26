# Component 1: Configuration and Initialization

## Purpose

This component is responsible for bringing the simulation to life from a set of input files. Before any cars can be parked or retrieved, the program needs to know how many garages exist, how tall each one is, which slots are already occupied, and who the existing customers are. Component 1 reads all of this from disk and assembles it into a single in-memory structure that the rest of the program operates on. It is the first thing `main` calls, and it is the only place where the garage and customer files are opened and interpreted. If any required file is missing or malformed at the top level, this component reports the problem and stops the program, since there is no meaningful way to continue a simulation whose initial state could not be loaded.

## Data Storage and Maintenance

Component 1 defines the two structures that the entire system shares, both declared in `config.h`. The `Customer` structure holds a customer's identifier, name, phone number, arrival time, and departure time, with the departure time left empty until the customer picks up their car. The `SystemState` structure holds the full state of the simulation: the garage count `m`, the per-garage capacity `n`, the derived capacity cap `maxCars`, the running count `currentCars`, the garage grid, and the customer list.

The garage grid is a vector of vectors of integers, addressed as `garages[garageIdx][rowIdx]`. Each garage is a column, and within that column index `0` is the bottom of the stack while index `n-1` is the top. The cell values use a small encoding shared across the program: the constant `AVAILABLE` (`-1`) marks an empty slot, `RESTRICTED` (`-2`) marks a permanent slot that can never be parked in but exists to support retrieval maneuvers, and any positive integer is the identifier of the customer parked in that slot. The customer list is a vector indexed directly by customer identifier, with position zero reserved as an unused placeholder so that a customer's record is always found at the position matching their identifier.

```cpp
struct SystemState {
    int m;
    int n;
    int maxCars;
    int currentCars;
    std::vector<std::vector<int>> garages;
    std::vector<Customer> customers;
};
```

## Functionality

The work is carried out by three functions. The top-level function, `initializeSystem`, opens the configuration file and reads its three lines. The first line provides the two integers `m` and `n`, the second line names the garage occupancy file, and the third line names the customer information file. After reading these, the function computes the capacity cap as `n * (m - 1)`, pre-sizes the garage grid to `m` columns of `n` available slots, and then delegates to the two loader functions. It returns the completed `SystemState` by value.

The function `readGarageOccupancy` parses the garage file, which is laid out so that columns correspond to garages and rows correspond to parking slots. An important detail is that the first row in the file represents the top of each garage, while the internal grid stores the bottom at index zero. The loader therefore reverses the rows as it reads them, mapping file row `i` to internal row `n - 1 - i`. As it reads each cell it trims surrounding whitespace, converts the tokens `R` or `r` into the restricted marker, leaves `-1` and empty cells as available, and otherwise interprets the token as a customer identifier. Every positive value encountered increments `currentCars`, so the running count reflects exactly the cars present in the loaded grid. The loader also guards against files that have more rows or columns than the configuration declared, emitting a warning rather than corrupting memory.

The function `readCustomerInfo` parses the customer file, where each line lists a customer's identifier, name, phone number, arrival time, and an optional departure time separated by commas. It splits each line into fields, trims them, and stores the resulting `Customer` at the vector position matching its identifier, growing the vector as needed so that any identifier can be addressed directly. Rows with a missing or non-positive identifier are skipped with a warning so that a single bad line does not abort the load. A small shared helper, `trim`, removes leading and trailing whitespace and is used throughout both loaders to keep the token comparisons and numeric conversions reliable.
