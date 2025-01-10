#include "transportTask.h"

// Вывод матрицы
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

// Алгоритм северо-западного угла
void northWestCorner(Instance& Inst)
{
    // Инициализация матрицы базисов
    Inst.basis.resize(Inst.prod.size());
    for (auto &i : Inst.basis)
    {
        i.resize(Inst.cons.size());
        std::fill(i.begin(), i.end(), false);
    }
    
    Instance Buff = Inst;

    std::vector<std::vector<int>> res( // инициализация матрицы с нулями
        Buff.prod.size(),
        (std::vector<int>(Buff.cons.size(), 0))
    );

    int i(0);
    int j(0);
    while ((Buff.prod.size() + Buff.cons.size() - 1) > (i + j + 1))
    {

        while(Buff.cons[j] > 0) // идем вниз, пока потребление не станет равно 0
        {
            res[i][j] = min(Buff.cons[j], Buff.prod[i]);
            Buff.basis[i][j] = true;
            Buff.cons[j] -= res[i][j]; // записываем сколько единиц потребления осталось
            Buff.prod[i] -= res[i][j]; // записываем сколько единиц производства осталось
            if (Buff.cons[j] > 0)
                i++;
        } 
        j++; // переход вправо
    }

    Inst.solution = res;
    Inst.basis = Buff.basis;
}

// Метод минимального элемента
void minElemMethod(Instance& Inst)
{
    // Инициализация матрицы базисов
    Inst.basis.resize(Inst.prod.size());
    for (auto &i : Inst.basis)
    {
        i.resize(Inst.cons.size());
        std::fill(i.begin(), i.end(), false);
    }

    Instance Buff = Inst;

    std::vector<std::vector<int>> res( // инициализация матрицы решения с нулями
        Inst.prod.size(),
        (std::vector<int>(Inst.cons.size(), 0))
    );

    // До тех пор, пока не будут использованны все потребления и производства
    int count = 0;
    int minElemVal = std::numeric_limits<int>::max();
    coordinates minElemCoord;
    while (
        std::accumulate(Buff.prod.begin(), Buff.prod.end(), 0)
        + std::accumulate(Buff.cons.begin(), Buff.cons.end(), 0)
        > 0)
    {
        // Полный проход по матрице для поиска минимального элемента
        for (int i(0); i < Buff.prod.size(); i++)
        {
            for (int j(0); j < Buff.cons.size(); j++)
            {
                if (minElemVal >= Buff.cost[i][j])
                {
                    minElemVal = Buff.cost[i][j];
                    minElemCoord = {j, i};
                }
            }
        }

        // Проверка на то, можем ли мы использовать данный элемент, хватает ли производств или требуется ли потреблению
        if (Buff.prod[minElemCoord.y] > 0 && Buff.cons[minElemCoord.x] > 0)
        {
            res[minElemCoord.y][minElemCoord.x] = min(Buff.cons[minElemCoord.x], Buff.prod[minElemCoord.y]);
            Inst.basis[minElemCoord.y][minElemCoord.x] = true;
            Buff.cons[minElemCoord.x] -= res[minElemCoord.y][minElemCoord.x]; // записываем сколько единиц потребления осталось
            Buff.prod[minElemCoord.y] -= res[minElemCoord.y][minElemCoord.x]; // записываем сколько единиц производства осталось
            count++;
        }

        // Заменяем наименьший элемент на большое число для последующего поиска следующего элемента
        Buff.cost[minElemCoord.y][minElemCoord.x] = std::numeric_limits<int>::max();
        minElemVal = std::numeric_limits<int>::max();
    }

    // Дополнение базисами решения, пока их количество не станет m - n - 1
    while (count < Buff.cons.size() + Buff.prod.size() - 1)
    {
        // Полный проход по матрице для поиска минимального элемента
        for (int i(0); i < Buff.prod.size(); i++)
        {
            for (int j(0); j < Buff.cons.size(); j++)
            {
                if (minElemVal >= Buff.cost[i][j])
                {
                    minElemVal = Buff.cost[i][j];
                    minElemCoord = {j, i};
                }
            }
        }

        // Проверка на то, можем ли мы взять этот элемент, не создает ли он цепочку
        Inst.basis[minElemCoord.y][minElemCoord.x] = true;
        auto loop = findClosedLoop(Inst.basis, minElemCoord);
        if (loop.back().x == -1 && loop.back().x == -1)
        {
            count++;
        }
        else
        {
            Inst.basis[minElemCoord.y][minElemCoord.x] = false;
        }
    }

    Inst.solution = res;
}

// Расчет потенциалов
bool calculatePotentials(Instance& Inst)
{
    // Матрица Гаусса для линейных уравнений формата:
    // V1 + V2 + .. + Vn + W1 + W2 + .. + Wm = const
    std::vector<std::vector<double>> sysOfLinEqu(
        Inst.prod.size() + Inst.cons.size(),
        (std::vector<double>(Inst.prod.size() + Inst.cons.size() + 1, 0))
    );

    int k = 0; // номер уравнения
    // V1 = 0
    sysOfLinEqu[k][0] = 1;
    sysOfLinEqu[k][sysOfLinEqu[0].size() - 1] = 0;
    k++;
    // Цикл заполнения матрицы
    for (int i = 0; i < Inst.prod.size(); i++)
    {
        for (int j = 0; j < Inst.cons.size(); j++)
        {
            // if (basis[i][j] != 0)
            if (Inst.basis[i][j] != false)
            {
                sysOfLinEqu[k][i] = 1;
                sysOfLinEqu[k][Inst.cons.size() + j] = 1;
                sysOfLinEqu[k][sysOfLinEqu[0].size() - 1] = double(Inst.cost[i][j]);
                k++;
            }
        }        
    }

    // Решение системы линейных уравнений
    gaussJordanMatrixSolver(sysOfLinEqu);

    // Вывод потенциалов отдельно в вектора
    std::vector<int> V, W;
    for (int i = 0; i < Inst.prod.size(); i++)
        V.push_back(int(sysOfLinEqu[i][sysOfLinEqu[0].size() - 1]));
    for (int j = 0; j < Inst.cons.size(); j++)
        W.push_back(int(sysOfLinEqu[Inst.prod.size() + j][sysOfLinEqu[0].size() - 1]));
    
    // Заполнение симлпекс разностей
    std::vector<std::pair<coordinates, int>> simplexDiff;
    std::pair<coordinates, int> minimum({-1, -1}, std::numeric_limits<int>::max());

    for (int i = 0; i < Inst.prod.size(); i++)
    {
        for (int j = 0; j < Inst.cons.size(); j++)
        {
            if (Inst.solution[i][j] == false)
            {
                simplexDiff.push_back(std::make_pair(coordinates{j,i}, (Inst.cost[i][j] - (V[i] + W[j]))));
                // Поиск минимума симлекс разности
                if (simplexDiff.back().second <= minimum.second)
                    minimum = simplexDiff.back();
            }
        }
    }

    // Нахождение минимальной разницы
    auto additional = std::min_element(simplexDiff.cbegin(), simplexDiff.cend(), [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second; } );

    // Проверка выполнения условия оптимальности
    if ((*additional).second >= 0)
    {
        return true;
    }
    else
    {
        // Добавление нового элемента в базис
        coordinates temp = coordinates{(*additional).first.x, (*additional).first.y};
        Inst.basis[temp.y][temp.x] = true;
        
        std::vector<coordinates> closedLoop = findClosedLoop(Inst.basis, (*additional).first);
        
        // Вывод "отрицательных" четных элементов
        for (auto elem : closedLoop)
        {
            std::cout << "(" << (elem.y + 1) <<  "," << (elem.x + 1) << ")" << " ";
        }
        std::cout << std::endl;

        // Корректировка решения
        // Вывод наименьшего отрицательного элемент
        int delIter = 1;
        int delVal = Inst.solution[closedLoop[delIter].y][closedLoop[delIter].x];
        for (int i = 1; i < closedLoop.size(); i += 2)
        {
            if (delVal > Inst.solution[closedLoop[i].y][closedLoop[i].x])
            {
                delIter = i;
                delVal = Inst.solution[closedLoop[delIter].y][closedLoop[delIter].x];
            }
        }

        Inst.basis[closedLoop[delIter].y][closedLoop[delIter].x] = false;
        
        // Вычитание значение из нечетных мест и прибавления значения в четных местах
        for (int i = 0; i < closedLoop.size(); i++)
        {
            // Нечетное по порядку, индекс будет четным
            if (i % 2 == 0)
                Inst.solution[closedLoop[i].y][closedLoop[i].x] += delVal;
            else
                Inst.solution[closedLoop[i].y][closedLoop[i].x] -= delVal;
        }
    }

    return false;
}

// Решение линейных уравнений методом Жордана Гаусса
void gaussJordanMatrixSolver(std::vector<std::vector<double>>& matrix)
{
    int n = matrix.size();
    int m = matrix[0].size();

    for (int i = 0; i < n; ++i) {
        // Поиск максимума в текущей колоннке
        double maxEl = abs(matrix[i][i]);
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (abs(matrix[k][i]) > maxEl) {
                maxEl = abs(matrix[k][i]);
                maxRow = k;
            }
        }

        // Обмен максимальной строки с текущей строкой (колонна за колонной)
        for (int k = i; k < m; ++k) {
            std::swap(matrix[maxRow][k], matrix[i][k]);
        }

        // Преобразование всех строк ниже в 0 в текущей колонне
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

    // Решение оставшегося верхнего треугольника, преобразование в единичную матрицу
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        // Деление всех элементов в строке
        double div = matrix[i][i];
        for (int w = 0; w < m; w++)
            matrix[i][w] /= div;

        // Вычитание элементов в строках сверху
        for (int k = i - 1; k >= 0; --k) {
            double mul = matrix[k][i];
            for(int l = 0; l < m; l++)
                matrix[k][l] -= matrix[i][l] * mul;
        }
    }    
}

// Нахождение цепочки
std::vector<coordinates> findClosedLoop(std::vector<std::vector<bool>> basisElem, coordinates start)
{
    // Первый шаг - исключение лишних строк и столбцов как по книге "Теоретические_основы_автоматизированного_управления_Хахулин_Красовская"
    // все лишние строки и столбцы будут заполнены false
    bool wasCrossed;
    std::vector<bool> crossedRow(basisElem.size(), false);
    std::vector<bool> crossedCol(basisElem[0].size(), false);
    do
    {
        wasCrossed = false;
        // Исключение лишних строк
        for (int i = 0; i < basisElem.size(); i++)
        {
            if(std::count(basisElem[i].begin(), basisElem[i].end(), true) <= 1 && !crossedRow[i]) // осталось не менее одного элемента
            {
                std::fill(basisElem[i].begin(), basisElem[i].end(), false);
                crossedRow[i] = true;
                wasCrossed = true;
            }
        }
        // Исключение лишних столбцов
        for (int j = 0; j < basisElem[0].size(); j++)
        {
            int count = 0;
            for (int i = 0; i < basisElem.size(); i++)
            {  
                if (basisElem[i][j] == true)
                    count++;
            }
            if (count <= 1 && !crossedCol[j]) // осталось не менее одного элемента
            {
                for (int i = 0; i < basisElem.size(); i++)
                    basisElem[i][j] = false;
                crossedCol[j] = true;
                wasCrossed = true;
            }
        }
    } while(wasCrossed); // До тех пор, пока можем вычеркнуть строку или столбца


    // Второй шаг - поиск цепочки
    std::vector<coordinates> res;
    // Проверка на то, не вычеркнуто ли все, есть ли вообще цепочка
    if (std::all_of(crossedRow.begin(), crossedRow.end(), [](bool v) { return v; }) && 
        std::all_of(crossedCol.begin(), crossedCol.end(), [](bool v) { return v; }))
    {
        // Нет цепочки
        res.push_back(coordinates{-1,-1}); // знак того, что цепочки нет
        return res;
    }
    else
    {
        // Есть цепочка
        coordinates cur = start;
        coordinates prec = start;
        res.push_back(prec);
        // Собираем цепочку пока текущая точка не является начальной
        do
        {
            // Цикл поворота директории
            for (short dir = right; dir <= down; dir++)
            {
                coordinates buff = cur;
                switch (dir)
                {
                case right:
                {
                    // цикл прохода вправо пока не дойдем до края массива или не найдем следующий элемент
                    while (buff.x < (basisElem[0].size() - 1))
                    {
                        buff.x++;

                        if (basisElem[buff.y][buff.x])
                            break;
                    }

                    break;
                }
                case up:
                {
                    // цикл прохода вверх
                    while (buff.y > 0) 
                    {
                        buff.y--;
                        
                        if (basisElem[buff.y][buff.x])
                            break;
                    }

                    break;
                }
                case left:
                {
                    // цикл прохода влево
                    while (buff.x > 0)
                    {
                        buff.x--;

                        if (basisElem[buff.y][buff.x])
                            break;                        
                    }

                    break;
                }
                case down:
                {
                    while (buff.y < (basisElem.size() - 1)) // цикл прохода вниз
                    {
                        buff.y++;

                        if (basisElem[buff.y][buff.x])
                            break;
                    }

                    break;
                }                    
                
                default:
                    break;
                }

                // Если найденный элемент не является предыдущим и является продолжением цепочки
                if (basisElem[buff.y][buff.x] &&
                    !(buff.x == prec.x && buff.y == prec.y) &&
                    !(buff.x == cur.x && buff.y == cur.y))
                {
                    cur = buff;
                    prec = res.back();
                    res.push_back(buff);
                    
                    break;
                }
            }

        } while (!(cur.x == start.x && cur.y == start.y));
    }

    res.pop_back();
    return res;
}