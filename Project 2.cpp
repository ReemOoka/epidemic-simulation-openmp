#include <iostream>
#include <fstream>
#include <omp.h>

const int WIDTH = 500; 
const int HEIGHT = 500; 

size_t used_memory = 0; // Track the amount of dynamically allocated memory

// Overload the new operator to track memory usage
void* operator new(size_t size) {
    used_memory += size;
    return malloc(size);
}

// Overload the delete operator to free memory
void operator delete(void* memory) noexcept {
    free(memory);
}

// Function to create a 2D grid dynamically
int** createGrid(int height, int width) {
    int** grid = new int* [height];
    for (int i = 0; i < height; i++) {
        grid[i] = new int[width](); 
    }
    return grid;
}

// Function to delete a 2D grid and free memory
void deleteGrid(int** grid, int height) {
    for (int i = 0; i < height; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

// Initialize the grid with a certain proportion of infected cells
void initializeGrid(int** grid, int** recovery_grid, double alpha, int omega) {
    int total_cells = WIDTH * HEIGHT;
    int initial_infected_count = static_cast<int>(alpha * total_cells);
    int infected_count = 0;

    // Infect cells based on alpha value uniformly across the grid
    for (int i = 0; i < HEIGHT && infected_count < initial_infected_count; i++) {
        for (int j = 0; j < WIDTH && infected_count < initial_infected_count; j++) {
            if ((i + j) % (total_cells / initial_infected_count) == 0) {
                grid[i][j] = 1; // Mark the cell as infected
                recovery_grid[i][j] = omega; // Set the recovery period
                infected_count++;
            }
        }
    }
}

// Write the current state of the grid to a file
void writeGridToFile(int day, int** grid, std::ofstream& file) {
    file << "Day " << day << ":\n";
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            file << grid[i][j] << " ";
        }
        file << "\n";
    }
    file << "\n";
}

// Simulate one day of infection spread and recovery
void simulateDay(int** grid, int** next_grid, int** recovery_grid, int omega) {
#pragma omp parallel for collapse(2)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int sick_neighbors = 0;
            int neighbors[4][2] = { {i - 1, j}, {i + 1, j}, {i, j - 1}, {i, j + 1} };
  
            for (int k = 0; k < 4; k++) {
                int ni = neighbors[k][0], nj = neighbors[k][1];
                if (ni >= 0 && ni < HEIGHT && nj >= 0 && nj < WIDTH && grid[ni][nj] == 1) {
                    sick_neighbors++;
                }
            }
            if (grid[i][j] == 1) {
                if (recovery_grid[i][j] > 1) {
                    recovery_grid[i][j]--;  
                    next_grid[i][j] = 1;    
                }
                else {
                    next_grid[i][j] = 0;  
                    recovery_grid[i][j] = 0; 
                }
            }
            else {
                if (sick_neighbors > 1) { 
                    next_grid[i][j] = 1;    
                    recovery_grid[i][j] = omega; 
                }
                else {
                    next_grid[i][j] = 0;    
                }
            }
        }
    }
#pragma omp parallel for collapse(2)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = next_grid[i][j];
            recovery_grid[i][j] = (next_grid[i][j] == 1) ? recovery_grid[i][j] : 0;
        }
    }
}


int main() {
    int num_threads, omega, days;
    double alpha, beta;
    std::cout << "Enter the number of threads: ";
    std::cin >> num_threads;
    std::cout << "Enter the initial infection ratio (alpha): ";
    std::cin >> alpha;
    std::cout << "Enter the infection probability (beta): "; 
    std::cin >> beta;
    std::cout << "Enter the duration a person remains sick (omega): ";
    std::cin >> omega;
    std::cout << "Enter the number of simulation days: ";
    std::cin >> days;

    omp_set_num_threads(num_threads);
    std::ofstream file("simulation.txt");
    double start_time = omp_get_wtime();
    int** grid = createGrid(HEIGHT, WIDTH);
    int** next_grid = createGrid(HEIGHT, WIDTH);
    int** recovery_grid = createGrid(HEIGHT, WIDTH);
    initializeGrid(grid, recovery_grid, alpha, omega);
    writeGridToFile(0, grid, file);
    for (int day = 1; day <= days; day++) {
        simulateDay(grid, next_grid, recovery_grid, omega);
        writeGridToFile(day, grid, file);
    }
    int final_infected_count = 0;
#pragma omp parallel for reduction(+:final_infected_count)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            final_infected_count += grid[i][j];
        }
    }
    double end_time = omp_get_wtime();
    deleteGrid(grid, HEIGHT);
    deleteGrid(next_grid, HEIGHT);
    deleteGrid(recovery_grid, HEIGHT);
    file.close();
    std::cout << "Execution time: " << end_time - start_time << " seconds.\n";
    std::cout << "Total used memory: " << used_memory << " bytes\n";
    std::cout << "Final infected count: " << final_infected_count << "\n";

    return 0;
}
