#include "transportTask.h"

// ����� �������
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

// �������� ������-��������� ����
std::vector<std::vector<int>> northWestCorner(Instance Instance)
{
    std::vector<std::vector<int>> res( // ������������� ������� � ������
        Instance.prod.size(),
        (std::vector<int>(Instance.cons.size(), 0))
    );

    int i(0);
    int j(0);
    while ((Instance.prod.size() + Instance.cons.size() - 1) > (i + j + 1))
    {

        while(Instance.cons[j] > 0) // ���� ����, ���� ����������� �� ������ ����� 0
        {
            res[i][j] = min(Instance.cons[j], Instance.prod[i]);
            Instance.cons[j] -= res[i][j]; // ���������� ������� ������ ����������� ��������
            Instance.prod[i] -= res[i][j]; // ���������� ������� ������ ������������ ��������
            if (Instance.cons[j] > 0)
                i++;
        } 
        j++; // ������� ������
    }

    return res;
}

// ����� ������������ ��������
std::vector<std::vector<int>> minElemMethod(Instance Instance)
{
    std::vector<std::vector<int>> res( // ������������� ������� � ������
        Instance.prod.size(),
        (std::vector<int>(Instance.cons.size(), 0))
    );

    // �� ��� ���, ���� �� ����� ������������� ��� ����������� � ������������
    while (
        std::accumulate(Instance.prod.begin(), Instance.prod.end(), 0)
        + std::accumulate(Instance.cons.begin(), Instance.cons.end(), 0)
        > 0)
    {
        int minElemVal = std::numeric_limits<int>::max();
        coordinates minElemCoord;

        // ������ ������ �� ������� ��� ������ ������������ ��������
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

        // �������� �� ��, ����� �� �� ������������ ������ �������, ������� �� ����������� ��� ��������� �� �����������
        if (Instance.prod[minElemCoord.y] > 0 && Instance.cons[minElemCoord.x] > 0)
        {
            res[minElemCoord.y][minElemCoord.x] = min(Instance.cons[minElemCoord.x], Instance.prod[minElemCoord.y]);
            Instance.cons[minElemCoord.x] -= res[minElemCoord.y][minElemCoord.x]; // ���������� ������� ������ ����������� ��������
            Instance.prod[minElemCoord.y] -= res[minElemCoord.y][minElemCoord.x]; // ���������� ������� ������ ������������ ��������
        }

        // �������� ���������� ������� �� ������� ����� ��� ������������ ������ ���������� ��������
        Instance.cost[minElemCoord.y][minElemCoord.x] = std::numeric_limits<int>::max();
    }

    return res;
}

// ������ �����������
void calculatePotentials(Instance Instance, std::vector<std::vector<int>> basis)
{
    // ������� ������ ��� �������� ��������� �������:
    // V1 + V2 + .. + Vn + W1 + W2 + .. + Wm = const
    std::vector<std::vector<double>> sysOfLinEqu(
        Instance.prod.size() + Instance.cons.size(),
        (std::vector<double>(Instance.prod.size() + Instance.cons.size() + 1, 0))
    );

    int k = 0; // ����� ���������
    // V1 = 0
    sysOfLinEqu[k][0] = 1;
    sysOfLinEqu[k][sysOfLinEqu[0].size() - 1] = 0;
    k++;
    // ���� ���������� �������
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

    // ������� ������� �������� ���������
    gaussJordanMatrixSolver(sysOfLinEqu);

    // ����� ����������� �������� � �������
    std::vector<int> V, W;
    for (int i = 0; i < Instance.prod.size(); i++)
        V.push_back(int(sysOfLinEqu[i][sysOfLinEqu[0].size() - 1]));
    for (int j = 0; j < Instance.cons.size(); j++)
        W.push_back(int(sysOfLinEqu[Instance.prod.size() + j][sysOfLinEqu[0].size() - 1]));
    
    // ���������� �������� ���������
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

// ������� �������� ��������� ������� ������� ������
void gaussJordanMatrixSolver(std::vector<std::vector<double>>& matrix)
{
    int n = matrix.size();
    int m = matrix[0].size();

    for (int i = 0; i < n; ++i) {
        // ����� ��������� � ������� ��������
        double maxEl = abs(matrix[i][i]);
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (abs(matrix[k][i]) > maxEl) {
                maxEl = abs(matrix[k][i]);
                maxRow = k;
            }
        }

        // ����� ������������ ������ � ������� ������� (������� �� ��������)
        for (int k = i; k < m; ++k) {
            std::swap(matrix[maxRow][k], matrix[i][k]);
        }

        // �������������� ���� ����� ���� � 0 � ������� �������
        for (int k = i + 1; k < n; ++k) {
            double c = -matrix[k][i] / matrix[i][i]; // ��������� �����
            for (int j = i; j < m; ++j) {
                if (i == j) {
                    matrix[k][j] = 0;
                } else {
                    matrix[k][j] += c * matrix[i][j];
                }
            }
        }
    }

    // ������� ����������� �������� ������������, �������������� � ��������� �������
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        // ������� ���� ��������� � ������
        double div = matrix[i][i];
        for (int w = 0; w < m; w++)
            matrix[i][w] /= div;

        // ��������� ��������� � ������� ������
        for (int k = i - 1; k >= 0; --k) {
            double mul = matrix[k][i];
            for(int l = 0; l < m; l++)
                matrix[k][l] -= matrix[i][l] * mul;
        }
    }    
}

// ���������� �������
std::vector<std::pair<coordinates, int>> findClosedLoop(std::vector<std::vector<bool>> basisElem, coordinates start)
{
    // ������ ��� - ���������� ������ ����� � �������� ��� �� ����� "�������������_������_�������������������_����������_�������_����������"
    // ��� ������ ������ � ������� ����� ��������� false
    bool wasCrossed;
    std::vector<bool> crossedRow(basisElem.size(), false);
    std::vector<bool> crossedCol(basisElem[0].size(), false);
    do
    {
        wasCrossed = false;
        // ���������� ������ �����
        for (int i = 0; i < basisElem.size(); i++)
        {
            if(std::count(basisElem[i].begin(), basisElem[i].end(), true) <= 1 && !crossedRow[i]) // �������� �� ����� ������ ��������
            {
                std::fill(basisElem[i].begin(), basisElem[i].end(), false);
                crossedRow[i] = true;
                wasCrossed = true;
            }
        }
        // ���������� ������ ��������
        for (int j = 0; j < basisElem[0].size(); j++)
        {
            int count = 0;
            for (int i = 0; i < basisElem.size(); i++)
            {  
                if (basisElem[i][j] == true)
                    count++;
            }
            if (count <= 1 && !crossedCol[j]) // �������� �� ����� ������ ��������
            {
                for (int i = 0; i < basisElem.size(); i++)
                    basisElem[i][j] = false;
                crossedCol[j] = true;
                wasCrossed = true;
            }
        }
    } while(wasCrossed); // �� ��� ���, ���� ����� ���������� ������ ��� �������


    // ������ ��� - ����� �������
    std::vector<std::pair<coordinates, int>> res;
    // �������� �� ��, �� ���������� �� ���, ���� �� ������ �������
    if (std::all_of(crossedRow.begin(), crossedRow.end(), [](bool v) { return v; }) && 
        std::all_of(crossedCol.begin(), crossedCol.end(), [](bool v) { return v; }))
    {
        // ��� �������
        res.push_back(std::make_pair(coordinates{-1,-1}, 0)); // ���� ����, ��� ������� ���
    }
    else
    {
        // ���� �������
        coordinates cur = start;
        coordinates prec = start;
        res.push_back(std::make_pair(prec, 10));
        // �������� ������� ���� ������� ����� �� �������� ���������
        do
        {
            // ���� �������� ����������
            for (short dir = right; dir <= down; dir++)
            {
                coordinates buff = cur;
                switch (dir)
                {
                case right:
                {
                    // ���� ������� ������ ���� �� ������ �� ���� ������� ��� �� ������ ��������� �������
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
                    // ���� ������� �����
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
                    // ���� ������� �����
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
                    while (buff.y < (basisElem.size() - 1)) // ���� ������� ����
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

                // ���� ��������� ������� �� �������� ���������� � �������� ������������ �������
                if (basisElem[buff.y][buff.x] &&
                    !(buff.x == prec.x && buff.y == prec.y) &&
                    !(buff.x == cur.x && buff.y == cur.y))
                {
                    cur = buff;
                    prec = res.back().first;
                    res.push_back(std::make_pair(buff, 10));
                    
                    break;
                }
            }

        } while (!(cur.x == start.x && cur.y == start.y));
    }
    res.pop_back();
    return res;
}