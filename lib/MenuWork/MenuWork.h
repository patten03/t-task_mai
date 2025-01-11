#ifndef MENUWORK
#define MENUWORK

#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include <ctime>
#include <conio.h>
#include <Windows.h>

// тип услови€, по которому выбираютс€ файл дл€ отображени€ в меню на основе названи€ файла
typedef bool (*fileExtension)(const std::string&);

// функции, измен€ющие название файлов

std::string space2underscore(std::string text); // замена пробелов в названии файлах на нижние подчеркивани€
std::string currentTime();                      // получение времени на данный момент дл€ предотвращени€ перезаписи файлов
std::string formatXX(int num);                  // преобразование чисел дл€ времени и дат

// функци€, позвол€юща€ выбирать пользователю файлы

std::string findFile(std::string title, fileExtension condition);

// составные части findFile()

int movingArrow(int ymin, int ymax, int cur, int key);                                                   // движение стрелки выбора
void fileChoice(int begCoord, std::vector<std::string> folders, int& cur, std::string& filepath);        // выбор файла
void makeFilesList(std::string filepath, std::vector<std::string>& folderList, fileExtension condition); // создание списка файлов

// функции реализации меню 

void ask(const std::vector<std::string> choice);    // вывод списка действий в консоль
int inputChoice(const int& end);                    // реагирование на выбор пользовател€ действий
void checkSpecialSymbols(const std::string& word);  // проверка названи€ файла на спец символы
std::string askName(const std::string& question);   // запрос у пользовател€ названи€ файла и проверка на нелегальные символы

void standartSettings(); // установка русской кодировки

#endif //MENUWORK