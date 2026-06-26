# QParking System Overview

QParking is a valet parking lot simulation written in C++17. It models a parking structure as a collection of `m` independent garages, where each garage behaves as a last-in, first-out (LIFO) stack with a fixed capacity of `n` slots. The program loads an initial configuration from a set of input files, prints the starting state of the lot, and then processes a scripted sequence of customer drop-offs and pickups, reporting the state of the lot after every interaction. All output is written to both the terminal and a file named `output.txt`.

The simulation is divided into six components. Each component owns a clearly defined responsibility, and together they form a pipeline that turns raw input files into a running simulation. The sections below describe how the components fit together, the shared data structures they operate on, and the overall flow of control. Each component also has its own page with a fuller description of its purpose, the data it maintains, and the functions that implement it.

## The Six Components

The first component, [[Component 1 Configuration and Initialization]], is the entry point for all data loading. It reads the three-line configuration file, opens the garage and customer files named inside it, and builds the in-memory state of the whole simulation. The second component, [[Component 2 Read and Execute Interactions]], opens the interactions script and walks through it one line at a time, deciding for each line whether a car is being dropped off or picked up and dispatching to the appropriate logic. The third component, [[Component 3 Parking Space Assignment]], handles drop-offs by choosing a garage with room and placing the new car on top of that garage's stack while respecting the capacity rules. The fourth component, [[Component 4 Locate a Car]], searches the garages to find where a given customer's car is parked. The fifth component, [[Component 5 Lot Reconfiguration]], handles the harder case of retrieving a car that is not on top of its stack by temporarily moving the cars above it, removing the target, and restoring the displaced cars. The sixth component, [[Component 6 Parking Lot Status and Output]], renders the garage grid and the summary statistics that are printed after the initial load and after every interaction.

## Shared Data Structures

Every component operates on a single shared structure, `SystemState`, which holds the complete live state of the simulation. It stores the garage count `m`, the per-garage capacity `n`, the derived hard cap `maxCars`, the running count of parked cars `currentCars`, the two-dimensional grid of garage slots, and the list of known customers. The grid is stored column by column as `garages[garageIdx][rowIdx]`, where index `0` of each garage is the bottom of the stack and index `n-1` is the top. Each slot holds one of three kinds of value: `-1` for an available slot, `-2` for a permanent restricted marker that can never be parked in but assists retrieval, and any positive integer for the identifier of the customer occupying that slot.

Customer records are stored in a `Customer` structure that carries the customer identifier, full name, phone number, arrival time, and departure time. These records live in a vector that is indexed directly by customer identifier, so the record for customer five is found at position five. Position zero is left unused as a placeholder so that identifiers map cleanly onto vector positions.

## Capacity Rules

The lot enforces a hard limit on how many cars may be parked at once. With `m` garages of `n` slots each, the maximum number of simultaneously parked cars is `n * (m - 1)`. Reserving the equivalent of one full garage guarantees there is always enough free space to perform the temporary moves required when retrieving a car buried inside a stack.

## System Flow

```
User provides config filename (one prompt only)
        |
Component 1: parse config -> parse garage CSV -> parse customer CSV -> build state
        |
Component 6: print initial garage state
        |
Component 2: read interactions file, for each of the interactions:
        |-- Drop-off: Component 3 assigns a space, then Component 6 prints state
        |-- Pickup:   Component 4 locates the car, Component 5 retrieves it,
        |             then Component 6 prints state
        |
All output written to output.txt
```

A diagram of this flow, produced in draw.io, accompanies this page in the wiki and shows the same pipeline visually, with the shared `SystemState` structure in the center and arrows from each component into the state it reads or modifies.
