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

