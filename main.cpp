#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <cstring>

// Define RESTRICT macro based on command-line option and compiler
#ifdef USE_RESTRICT
    #if defined(_MSC_VER) && _MSC_VER >= 1900
        #define RESTRICT __restrict
    #elif defined(__GNUC__) || defined(__clang__)
        #define RESTRICT __restrict
    #else
        #define RESTRICT
    #endif
#else
    #define RESTRICT
#endif

const int ROWS = 1000;
const int COLS = 1000;

void generate_random_nums(int** matrix, int rows, int cols) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 99);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dist(gen);
        }
    }
}

// Pointer-based function with heavy dereferencing
inline int pointer_sum(int** RESTRICT matrix, int rows, int cols) {
    int sum = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sum += matrix[i][j]; // Double dereferencing
        }
    }
    return sum;
}

// Optimized function with direct indexing and temporary pointer
inline int pointer_elim_sum(int** RESTRICT matrix, int rows, int cols) {
    int sum = 0;
    for (int i = 0; i < rows; ++i) {
        const int* RESTRICT row = matrix[i];
        for (int j = 0; j < cols; ++j) {
            sum += row[j]; // Direct indexing
        }
    }
    return sum;
}

void get_time(int** RESTRICT matrix, int rows, int cols, int(*sum)(int** RESTRICT matrix, int rows, int cols), std::string funcName) {
    auto start = std::chrono::steady_clock::now();
    int sum_result = sum(matrix, rows, cols);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << funcName << " Sum: " << sum_result << ", Time: " << duration << " us\n";

}

int main(int argc, char* argv[]) {
    // Check for --restrict command-line option
    bool useRestrict = false;
    if (argc > 1) {
        if (std::strcmp(argv[1], "--restrict") == 0) {
            useRestrict = true;
        }
    }

    // Print whether RESTRICT is enabled
    std::cout << "RESTRICT is " << (useRestrict ? "enabled" : "disabled") << "\n";
#ifdef USE_RESTRICT
    std::cout << "Using RESTRICT macro: " << (useRestrict ? " __restrict" : " none") << "\n";
#else
    std::cout << "RESTRICT macro not defined (no --restrict flag)\n";
#endif

    // Initialize matrix with random numbers
    int** matrix = new int*[ROWS];
    for (int i = 0; i < ROWS; ++i) {
        matrix[i] = new int[COLS];
    }

    generate_random_nums(matrix, ROWS, COLS);

    get_time(matrix, ROWS, COLS, pointer_sum, "Pointer-based");
    get_time(matrix, ROWS, COLS, pointer_elim_sum, "Optimized");
    return 0;
}