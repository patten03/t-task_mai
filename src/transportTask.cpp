#include "transportTask.h"

// ¬ывод матрицы
void printMatrix(std::vector<std::vector<int>> matrix)
{
    const int width(5);
    for (const auto & row : matrix)
    {
        for (const auto & elem : row)
        {
            std::cout << std::setw(width) << elem << " ";            
        }
        std::cout << std::endl;        
    }
}

// јлгоритм северо-западного угла
std::vector<std::vector<int>> northWestCorner(Instance Instance)
{
    std::vector<std::vector<int>> res( // инициализаци€ матрицы с нул€ми
        Instance.prod.size(),
        (std::vector<int>(Instance.cons.size(), 0))
    );

    int i(0);
    int j(0);
    while ((Instance.prod.size() + Instance.cons.size() - 1) > (i + j + 1))
    {

        while(Instance.cons[j] > 0) // идем вниз, пока потребление не станет равно 0
        {
            res[i][j] = min(Instance.cons[j], Instance.prod[i]);
            Instance.cons[j] -= res[i][j]; // записываем сколько единиц потреблени€ осталось
            Instance.prod[i] -= res[i][j]; // записываем сколько единиц производства осталось
            if (Instance.cons[j] > 0)
                i++;
        } 
        j++; // переход вправо
    }

    return res;
}

// ћетод минимального элемента
std::vector<std::vector<int>> minElemMethod(Instance Instance)
{
    std::vector<std::vector<int>> res( // инициализаци€ матрицы с нул€ми
        Instance.prod.size(),
        (std::vector<int>(Instance.cons.size(), 0))
    );

    // ƒо тех пор, пока не будут использованны все потреблени€ и производства
    while (
        std::accumulate(Instance.prod.begin(), Instance.prod.end(), 0)
        + std::accumulate(Instance.cons.begin(), Instance.cons.end(), 0)
        > 0)
    {
        int minElemVal = std::numeric_limits<int>::max();
        coordinates minElemCoord;

        // ѕолный проход по матрице дл€ поиска минимального элемента
        for (int i(0); i < Instance.prod.size(); i++)
        {
            for (int j(0); j < Instance.cons.size(); j++)
            {
                if (minElemVal >= Instance.cost[i][j])
                {
                    minElemVal = Instance.cost[i][j];
                    minElemCoord = {j, i};
                }
            }
        }

        // ѕроверка на то, можем ли мы использовать данный элемент, хватает ли производств или требуетс€ ли потреблению
        if (Instance.prod[minElemCoord.y] > 0 && Instance.cons[minElemCoord.x] > 0)
        {
            res[minElemCoord.y][minElemCoord.x] = min(Instance.cons[minElemCoord.x], Instance.prod[minElemCoord.y]);
            Instance.cons[minElemCoord.x] -= res[minElemCoord.y][minElemCoord.x]; // записываем сколько единиц потреблени€ осталось
            Instance.prod[minElemCoord.y] -= res[minElemCoord.y][minElemCoord.x]; // записываем сколько единиц производства осталось
        }

        // «амен€ем наименьший элемент на большое число дл€ последующего поиска следующего элемента
        Instance.cost[minElemCoord.y][minElemCoord.x] = std::numeric_limits<int>::max();
    }

    return res;
}

// –асчет потенциалов
void calculatePotentials(Instance Instance, std::vector<std::vector<int>> basis)
{
    // ћатрица √аусса дл€ линейных уравнений формата:
    // V1 + V2 + .. + Vn + W1 + W2 + .. + Wm = const
    std::vector<std::vector<double>> sysOfLinEqu(
        Instance.prod.size() + Instance.cons.size(),
        (std::vector<double>(Instance.prod.size() + Instance.cons.size() + 1, 0))
    );

    int k = 0; // номер уравнени€
    // V1 = 0
    sysOfLinEqu[k][0] = 1;
    sysOfLinEqu[k][sysOfLinEqu[0].size() - 1] = 0;
    k++;
    // ÷икл заполнени€ матрицы
    for (int i = 0; i < Instance.prod.size(); i++)
    {
        for (int j = 0; j < Instance.cons.size(); j++)
        {
            if (basis[i][j] != 0)
            {
                sysOfLinEqu[k][i] = 1;
                sysOfLinEqu[k][4 + j] = 1;
                sysOfLinEqu[k][sysOfLinEqu[0].size() - 1] = double(Instance.cost[i][j]);
                k++;
            }
        }        
    }

    // –ешение системы линейных уравнений
    gaussJordanMatrixSolver(sysOfLinEqu);

    // ¬ывод потенциалов отдельно в вектора
    std::vector<int> V, W;
    for (int i = 0; i < Instance.prod.size(); i++)
        V.push_back(int(sysOfLinEqu[i][sysOfLinEqu[0].size() - 1]));
    for (int j = 0; j < Instance.cons.size(); j++)
        W.push_back(int(sysOfLinEqu[Instance.prod.size() + j][sysOfLinEqu[0].size() - 1]));
    
    // «аполнение симлпекс разностей
    std::vector<std::pair<coordinates, int>> simplexDiff;

    for (int i = 0; i < Instance.prod.size(); i++)
    {
        for (int j = 0; j < Instance.cons.size(); j++)
        {
            if (basis[i][j] == 0)
            {
                simplexDiff.push_back(std::make_pair(coordinates{i,j}, (Instance.cost[i][j] - (V[i] + W[j]))));
            }
        }
    }
}

// –ешение линейных уравнений методом ∆ордана √аусса
void gaussJordanMatrixSolver(std::vector<std::vector<double>>& matrix)
{
    int n = matrix.size();
    int m = matrix[0].size();

    for (int i = 0; i < n; ++i) {
        // ѕоиск максимума в текущей колоннке
        double maxEl = abs(matrix[i][i]);
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (abs(matrix[k][i]) > maxEl) {
                maxEl = abs(matrix[k][i]);
                maxRow = k;
            }
        }

        // ќбмен максимальной строки с текущей строкой (колонна за колонной)
        for (int k = i; k < m; ++k) {
            std::swap(matrix[maxRow][k], matrix[i][k]);
        }

        // ѕреобразование всех строк ниже в 0 в текущей колонне
        for (int k = i + 1; k < n; ++k) {
            double c = -matrix[k][i] / matrix[i][i]; // получение знака
            for (int j = i; j < m; ++j) {
                if (i == j) {
                    matrix[k][j] = 0;
                } else {
                    matrix[k][j] += c * matrix[i][j];
                }
            }
        }
    }

    // –ешение оставшегос€ верхнего треугольника, преобразование в единичную матрицу
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        // ƒеление всех элементов в строке
        double div = matrix[i][i];
        for (int w = 0; w < m; w++)
            matrix[i][w] /= div;

        // ¬ычитание элементов в строках сверху
        for (int k = i - 1; k >= 0; --k) {
            double mul = matrix[k][i];
            for(int l = 0; l < m; l++)
                matrix[k][l] -= matrix[i][l] * mul;
        }
    }    
}