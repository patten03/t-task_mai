#include "transportTask.h"

int main(){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL,"rus");

    Instance Small;
    Small.cost = {
        {15, 4, 10, 11},
        {10, 4, 7, 5},
        {4, 12, 8, 6},
        {6, 9, 7, 2},
    };
    Small.cons = {5, 2, 8, 7};
    Small.prod = {8, 5, 8, 1};

    printMatrix(northWestCorner(Small));
    printMatrix(minElemMethod(Small));

    std::vector<std::vector<double>> A = {
        {1, 0, 0, 0, 1, 0, 0, 0, 15},
        {1, 0, 0, 0, 0, 1, 0, 0, 4},
        {1, 0, 0, 0, 0, 0, 1, 0, 10},
        {0, 1, 0, 0, 0, 0, 1, 0, 7},
        {0, 0, 1, 0, 0, 0, 1, 0, 8},
        {0, 0, 1, 0, 0, 0, 0, 1, 6},
        {0, 0, 0, 1, 0, 0, 0, 1, 2},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    gaussJordanMatrixSolver(A);
    
    auto basis = northWestCorner(Small);
    calculatePotentials(Small, basis);


    std::vector<std::vector<bool>> B = {
        // {1, 1, 1, 0},
        // {0, 0, 1, 0},
        // {1, 0, 1, 1},
        // {0, 0, 0, 1}

        {1, 1, 0, 0},
        {1, 0, 0, 1},
        {0, 0, 0, 0},
        {0, 1, 0, 1}

        // {1, 1, 1, 0},
        // {0, 0, 1, 0},
        // {0, 0, 1, 1},
        // {0, 0, 0, 1}
    };
    findClosedLoop(B, {0,0});

    return 0;
}