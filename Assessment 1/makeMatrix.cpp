#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <filesystem>
#include <omp.h>
#include "header.h"
namespace fs = std::filesystem;

// Function to generate a random matrix of given size
std::vector<std::vector<int>> generateMatrix(int size, int max_value = 100) {
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % max_value;
        }
    }
    return matrix;
}

// Function to multiply two matrices
std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int size = A.size();
    std::vector<std::vector<int>> C(size, std::vector<int>(size, 0));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// Function to write a matrix to a CSV file
void writeMatrixToCSV(const std::vector<std::vector<int>>& matrix, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& row : matrix) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
}

int main() {
    // Seed the random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // List of sizes
    sizeVect;
    // std::vector<int> sizes = {2, 4, 6, 8, 10};

    // std::vector<int> sizes = {150, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};

    // Parallelize the creation, multiplication, and saving of matrices
    #pragma omp parallel for num_threads(12) schedule(dynamic)
    for (int i = 0; i < sizes.size(); ++i) {
        int size = sizes[i];

        // Create subfolder for the current size
        fs::path folder = ".test/size_" + std::to_string(size);
        fs::create_directory(folder);

        // Generate matrices
        auto matrixA = generateMatrix(size);
        auto matrixB = generateMatrix(size);

        // Multiply matrices A and B to get matrix C
        auto matrixC = multiplyMatrices(matrixA, matrixB);

        // Write matrices to CSV files
        writeMatrixToCSV(matrixA, folder / "matrixA.csv");
        writeMatrixToCSV(matrixB, folder / "matrixB.csv");
        writeMatrixToCSV(matrixC, folder / "matrixC.csv");

        // Print output
        #pragma omp critical
        {
            std::cout << "Matrices of size " << size << " saved in folder " << folder << std::endl;
        }
    }

    return 0;
}
