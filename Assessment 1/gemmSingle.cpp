#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>
#include <chrono>
#include <omp.h>
#include "header.h"
#include "rapidcsv.h"
namespace fs = std::filesystem;

std::vector<std::vector<double>> readMatrix(std::string path, int size)
{
    std::vector<std::vector<double>> output;
    std::ifstream file(path);
    std::string line, cell;
    if (!file.is_open())
    {
        std::cerr << "Error opening the file " << std::endl;
    }
    while (std::getline(file, line))
    {
        std::vector<double> row;
        std::stringstream lineStream(line);

        while (std::getline(lineStream, cell, ','))
        {
            row.push_back(std::stod(cell));
        }
        output.push_back(row);
    }
    file.close();
    return output;
}

std::vector<std::vector<double>> multMatrix(std::vector<std::vector<double>> A, std::vector<std::vector<double>> B, int size)
{
    std::vector<std::vector<double>> output;
    for (int i = 0; i < size; ++i)
    {
        std::vector<double> row;
        for (int j = 0; j < size; ++j)
        {
            double sum = 0;
            for (int k = 0; k < size; ++k)
            {
                sum += A[i][k] * B[k][j];
            }
            row.push_back(sum);
        }
        output.push_back(row);
    }
    return output;
}

bool assertMultiply(std::vector<std::vector<double>> C, int size)
{
    fs::path folder = ".test/size_" + std::to_string(size);
    auto matrixC = readMatrix(folder / "matrixC.csv", size);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (C[i][j] != matrixC[i][j])
            {
                std::cerr << "Error: C[" << i << "][" << j << "] = " << C[i][j] << " but expected " << matrixC[i][j] << std::endl;
                return false;
            }
        }
    }
    return true;
}

int main()
{
    sizeVect;
    rapidcsv::Document doc("speeds.csv");
    for (int i = 0; i < sizes.size(); i++)
    {
        int size = sizes[i];

        fs::path folder = ".test/size_" + std::to_string(size);
        auto matrixA = readMatrix(folder / "matrixA.csv", size);
        auto matrixB = readMatrix(folder / "matrixB.csv", size);
        auto start = std::chrono::high_resolution_clock::now();
        auto matrixC = multMatrix(matrixA, matrixB, size);
        auto end = std::chrono::high_resolution_clock::now();
        // measure time in milliseconds
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Time taken for size " << size << " is " << duration.count() << " ms" << std::endl;
    }
}