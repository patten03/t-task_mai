#ifndef TRANSTASK
#define TRANSTASK

#include <Windows.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits> 
#include <numeric>
#include <algorithm>
#include <cmath>

#undef max

struct coordinates
{
    int x, y;
};

struct Instance
{
    std::vector<std::vector<int>> cost;
    std::vector<int> prod;
    std::vector<int> cons;
};

void printMatrix(std::vector<std::vector<int>> matrix);
std::vector<std::vector<int>> northWestCorner(Instance Instance);
std::vector<std::vector<int>> minElemMethod(Instance Instance);

void calculatePotentials(Instance Instance, std::vector<std::vector<int>> basis);
void gaussJordanMatrixSolver(std::vector<std::vector<double>>& matrix);

#endif 