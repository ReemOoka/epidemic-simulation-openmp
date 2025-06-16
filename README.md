# 🦠 Parallel Grid-Based Infection Simulation in C++

A multithreaded, grid-based epidemic simulation built using **C++** and **OpenMP**, designed to model the spread of infection across a population over time. The project demonstrates concepts in **parallel processing**, **memory profiling**, and **cellular automata-style simulation** on a 2D grid.

Built for the **Parallel Computing course at Marist College**, this system simulates infection dynamics using adjustable parameters for infection ratio, recovery time, and simulation duration.

---

## 🔬 Simulation Overview

- The simulation models a **500x500 grid** of individuals (cells)
- Each cell can be **healthy**, **infected**, or **recovered**
- Infected cells can infect neighboring cells based on simulation rules
- The recovery period and infection probability are customizable
- Simulation runs over a user-defined number of days
- Uses OpenMP to parallelize infection spread and statistics gathering

---

## 🧪 Key Features

### ⚙️ Dynamic Memory Allocation with Tracking
- Overloads `new` operator to track total memory usage
- Memory stats are printed at the end of execution

### 🧵 Parallel Simulation Logic
- Infection spread and recovery steps are computed in parallel using `#pragma omp parallel for`
- Uses OpenMP’s reduction and `collapse` directives for efficient multi-core execution
- Grid updates are safely handled via a double-buffered update system (`grid` and `next_grid`)

### 📉 Output
- Writes the state of the grid for each day into a file (`simulation.txt`)
- Records execution time and final infected cell count
- Sample output files for various thread counts: `simulation-1.txt`, `simulation-2.txt`, etc.

---

## 🧰 Technologies Used

| Category         | Tools/Concepts                       |
|------------------|--------------------------------------|
| Language         | C++17                                |
| Parallelism      | OpenMP                               |
| Memory Profiling | Custom `new` operator tracking       |
| I/O              | `std::ifstream`, `std::ofstream`     |
| Optimization     | Double buffering, conditional logic  |
| Simulation       | Cellular Automata–style infection modeling |

---

## 🚀 How to Run

### 🛠 Prerequisites

- C++17 compatible compiler (e.g., g++, Visual Studio)
- OpenMP support (`-fopenmp` for GCC)

### 💻 Compilation

**Linux/macOS (GCC):**
```bash
g++ -std=c++17 -fopenmp -o simulation Project\ 2.cpp
./simulation

Windows (Visual Studio):

Open Project 2.sln in Visual Studio

Set to Release x64

Press Ctrl + F5 to build and run
```

## 📂 Project Structure

```bash

Project-2/
├── Project 2.cpp            # Main simulation logic
├── simulation.txt           # Output from the latest run
├── simulation-1.txt ...     # Output files for different threads
├── Project 2.sln            # Visual Studio solution file
├── Project 2.vcxproj        # VS project files
└── README.md

```

## 🧠 Parameters Prompted at Runtime
Number of threads

Initial infection ratio (alpha)

Infection probability (beta)

Recovery duration (omega)

Number of simulation days

## 📊 Sample Output
```bash
Execution time: 3.21 seconds.
Total used memory: 5000000 bytes
Final infected count: 43892
```

## 👩‍💻 Author

Reem Ooka

Full Stack Java Developer| AI/ML Researcher
