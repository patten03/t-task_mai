#include "transportTask.h"
#include <iostream>

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

    return 0;
}