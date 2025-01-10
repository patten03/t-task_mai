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

    std::vector<std::vector<int>> solution;
    std::vector<std::vector<bool>> basis;
};

void printMatrix(std::vector<std::vector<int>> matrix);
void printMatrix(std::vector<std::vector<bool>> matrix);
void northWestCorner(Instance& Inst);
void minElemMethod(Instance& Inst);

std::vector<coordinates> findClosedLoop(std::vector<std::vector<bool>> basisElem, coordinates start);
bool calculatePotentials(Instance& Instances);
void gaussJordanMatrixSolver(std::vector<std::vector<double>>& matrix);


#endif 