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
#include <unordered_map>

#undef max

enum direction {right = 0, up, left, down};

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


std::vector<std::pair<coordinates, int>> findClosedLoop(std::vector<std::vector<bool>> basisElem, coordinates start);
void calculatePotentials(Instance Instance, std::vector<std::vector<int>> basis);
void gaussJordanMatrixSolver(std::vector<std::vector<double>>& matrix);

#endif 