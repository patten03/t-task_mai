#include "transportTask.h"
#include "MenuWork.h"

int main()
{
    standartSettings();

    // Экземпляр большой задачи
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

    // Экземпляр малой задачи
    Instance Small;
    Small.cost = {
        {15, 4, 10, 11},
        {10, 4, 7, 5},
        {4, 12, 8, 6},
        {6, 9, 7, 2},
    };
    Small.cons = {5, 2, 8, 7};
    Small.prod = {8, 5, 8, 1};

    bool stop = false;
    while (!stop)
    {
        if (trace)
            std::cout << "Трассировка включена" << std::endl;
        else
            std::cout << "Трассировка выключена" << std::endl;
        std::cout << "Выберите задачу и метод решения:" << std::endl;
        std::vector<std::string> task = {
            "Задача малой размерности алгоритмом северо-западного угла",
            "Задача малой размерности методом минимального элемента",
            "Задача большой размерности алгоритмом северо-заподного угла",
            "Задача большой размерности методом минимального элемента",
            "Переключить трассировку",
            "Выйти из программы"
        };
        ask(task);
        int choice = inputChoice(task.size());

        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point accept;
        std::chrono::high_resolution_clock::time_point done;
        start = std::chrono::high_resolution_clock::now();
        int step = 0;
        switch (choice)
        {
            case 1:
            {
                std::cout << "Решение задачи алгоритмом северо-западного угла" << std::endl;
                printTask(Small);
                std::cout << std::endl;
                northWestCorner(Small);
                if (trace)
                {
                    std::cout << "==============Полученное допустимое решение==============" << std::endl;
                    std::cout << "Количество элементов в базисе: " << Small.prod.size() + Small.cons.size() - 1 << std::endl;
                    std::cout << "Значение целевой функции: " << total(Small) << std::endl;
                    std::cout << "Полученный допустимый базис:" << std::endl;
                    printBasis(Small);
                    std::cout << "Базисное допустимое решение Т-задачи:" << std::endl;
                    printMatrix(Small.solution);
                    std::cout << std::endl;
                }

                accept = std::chrono::high_resolution_clock::now();
                
                if (trace)
                    std::cout << "Поиск оптимального решения задачи" << std::endl;
                
                bool opt = false;
                while (!opt)
                {
                    step++;
                    if (trace)
                        std::cout << "============= Шаг№" << step << " ==============" << std::endl;
                    opt = calculatePotentials(Small);
                    if (trace)
                        std::cout << std::endl;
                }

                break;
            }
            case 2:
            {
                std::cout << "Решение задачи методом минимального элемента" << std::endl;
                printTask(Small);
                std::cout << std::endl;
                minElemMethod(Small);
                if (trace)
                {
                    std::cout << "==============Полученное допустимое решение==============" << std::endl;
                    std::cout << "Количество элементов в базисе: " << Small.prod.size() + Small.cons.size() - 1 << std::endl;
                    std::cout << "Значение целевой функции: " << total(Small) << std::endl;
                    std::cout << "Полученный допустимый базис:" << std::endl;
                    printBasis(Small);
                    std::cout << "Базисное допустимое решение Т-задачи:" << std::endl;
                    printMatrix(Small.solution);
                    std::cout << std::endl;
                }

                accept = std::chrono::high_resolution_clock::now();
                
                if (trace)
                    std::cout << "Поиск оптимального решения задачи" << std::endl;
                
                bool opt = false;
                while (!opt)
                {
                    step++;
                    if (trace)
                        std::cout << "============= Шаг№" << step << " ==============" << std::endl;
                    opt = calculatePotentials(Small);
                    if (trace)
                        std::cout << std::endl;
                }

                break;
            }
            case 3:
            {
                std::cout << "Решение задачи алгоритмом северо-западного угла" << std::endl;
                printTask(Big);
                northWestCorner(Big);
                accept = std::chrono::high_resolution_clock::now();
                bool opt = false;
                while (!opt)
                {
                    step++;
                    opt = calculatePotentials(Big);
                }

                break;
            }
            case 4:
            {
                std::cout << "Решение задачи методом минимального элемента" << std::endl;
                printTask(Big);
                minElemMethod(Big);
                accept = std::chrono::high_resolution_clock::now();
                bool opt = false;
                while (!opt)
                {
                    step++;
                    opt = calculatePotentials(Big);
                }

                break;
            }
            case 5:
            {
                trace ^= true;
                break;
            }
            case 6:
            {
                stop = true;
                break;
            }
            default: break;
        }

        if (choice != 5 && choice != 6)
        {
            done = std::chrono::high_resolution_clock::now();
            Instance Buff;
            if (choice == 1 || choice == 2)
                Buff = Small;
            else
                Buff = Big;
            std::cout << std::endl;
            std::cout << "============================" << std::endl;
            std::cout << "Количестов сделанных шагов: " << step << std::endl;
            std::cout << "Оптимальное значение целевой функции: " << total(Buff) << std::endl;
            std::chrono::duration<double> duration1, duration2, duration3;
            duration1 = accept - start;
            duration2 = done - accept;
            duration3 = done - start;
            std::cout << std::endl;
            std::cout << "Время поиска допустимого решения: " << duration1.count() << " секунды." << std::endl;
            std::cout << "Время поиска оптимального решения: " << duration2.count() << " секунды." << std::endl;
            std::cout << "Время решения задачи: " << duration3.count() << " секунды." << std::endl;
            std::cout << std::endl;
            printResult(Buff);

            if (!stop)
            {
                std::cout << "Для продолжения нажмите любую клавишу" << std::endl;
                _getch();
            }
            system("cls");
        }

    }
    return 0;
}