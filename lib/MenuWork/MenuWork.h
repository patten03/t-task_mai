#ifndef MENUWORK
#define MENUWORK

#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include <ctime>
#include <conio.h>
#include <Windows.h>

// ��� �������, �� �������� ���������� ���� ��� ����������� � ���� �� ������ �������� �����
typedef bool (*fileExtension)(const std::string&);

// �������, ���������� �������� ������

std::string space2underscore(std::string text); // ������ �������� � �������� ������ �� ������ �������������
std::string currentTime();                      // ��������� ������� �� ������ ������ ��� �������������� ���������� ������
std::string formatXX(int num);                  // �������������� ����� ��� ������� � ���

// �������, ����������� �������� ������������ �����

std::string findFile(std::string title, fileExtension condition);

// ��������� ����� findFile()

int movingArrow(int ymin, int ymax, int cur, int key);                                                   // �������� ������� ������
void fileChoice(int begCoord, std::vector<std::string> folders, int& cur, std::string& filepath);        // ����� �����
void makeFilesList(std::string filepath, std::vector<std::string>& folderList, fileExtension condition); // �������� ������ ������

// ������� ���������� ���� 

void ask(const std::vector<std::string> choice);    // ����� ������ �������� � �������
int inputChoice(const int& end);                    // ������������ �� ����� ������������ ��������
void checkSpecialSymbols(const std::string& word);  // �������� �������� ����� �� ���� �������
std::string askName(const std::string& question);   // ������ � ������������ �������� ����� � �������� �� ����������� �������

void standartSettings(); // ��������� ������� ���������

#endif //MENUWORK