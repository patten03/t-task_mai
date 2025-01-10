#include "transportTask.h"

int main(){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL,"rus");

    Instance Big;
    Big.cost = {
           {15, 4, 10, 11, 3, 2, 4, 7, 7, 17},
           {10, 4, 7, 5, 6, 5, 10, 1, 9, 11},
           {4, 12, 8, 6, 14, 14, 18, 1, 14, 15},
           {6, 9, 7, 2, 11, 11, 16, 6, 12, 6},
           {8, 5, 6, 2, 8, 7, 12, 2, 9, 9},
           {2, 11, 4, 6, 12, 13, 15, 9, 10, 5},
           {6, 5, 1, 5, 6, 8, 10, 5, 5, 9},
           {5, 6, 2, 5, 7, 9, 11, 6, 6, 8},
           {17, 6, 12, 13, 4, 5, 2, 9, 7, 19},
           {8, 13, 11, 6, 15, 15, 20, 10, 16, 5}
    };
    Big.cons = {5, 2, 8, 7, 1, 3, 7, 1, 2, 6};
    Big.prod = {8, 5, 8, 1, 2, 1, 4, 3, 7, 3};

    Instance Small;
    Small.cost = {
        {15, 4, 10, 11},
        {10, 4, 7, 5},
        {4, 12, 8, 6},
        {6, 9, 7, 2},
    };
    Small.cons = {5, 2, 8, 7};
    Small.prod = {8, 5, 8, 1};

    // northWestCorner(Big);
    minElemMethod(Big);

    printMatrix(Big.solution);
    bool opt = false;
    int count = 0;
    while (!opt)
    {
        std::cout << count << std::endl;
        count++;
        opt = calculatePotentials(Big);
        std::cout << "---------------" << std::endl;
        printMatrix(Big.solution);
    }

    return 0;
}