#ifndef TRANSTASK
#define TRANSTASK

#include <Windows.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits> 
#include <numeric>

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

#endif 