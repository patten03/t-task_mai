#include "MenuWork.h"

//@brief вывод списка файлов в консоль для их выбора пользователем 
void showDir(const std::vector<std::string>& dir) {
	for (const auto& item : dir) {
		std::cout << "  " << item << std::endl;
	}
}

//@brief выбор файла через консоль пользователем
//@return возврат выбранного файла в виде строки
std::string findFile(std::string title, fileExtension condition) {
	std::filesystem::path p = "."; // получение пути, где находится программа
	std::string curFilepath = std::filesystem::absolute(p).string();

	// выход из цикла происходит когда выбран файл или пользователь решил выйти в меню
	while (!condition(curFilepath) && curFilepath != "") {
		try {
			std::vector<std::string> fileList;
			makeFilesList(curFilepath, fileList, condition);
			if (fileList.size() == 0) // выброс исключения из-за отсутсвия файлов
				throw std::invalid_argument("Директория пуста, ни одного файла не найдено");
			std::cout << title << std::endl;

			std::vector<std::string> menu{
				"Для выбора файла используйте ENTER и стрелочки ВВЕРХ ВНИЗ",
				"Для выхода из меню нажмите ESC"
			};

			int begCoord = menu.size() + 1;
			int cur(begCoord); // текущая координата стрелки выбора папки

			for (const auto& line : menu) {
				std::cout << line << std::endl;
			}
			showDir(fileList);

			fileChoice(begCoord, fileList, cur, curFilepath);

			system("cls");
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			return ""; // выход в меню по ошибке
		}
	}
	return curFilepath;
}

//@brief реализация меню выбора для findFile()
void fileChoice(int begCoord, std::vector<std::string> files, int& cur, std::string& filepath) {
	movingArrow(begCoord, begCoord + files.size(), cur, 0); // показ стрелки выбора папки  

	bool chosenMenu(false);
	// цикл выбора файла
	while (!chosenMenu) {
		switch (int c = _getch()) {
		case 224: // код нажатия на стрелки на клавиатуре 
		{
			cur = movingArrow(begCoord, begCoord + files.size() - 1, cur, _getch());
			break;
		}
		case 27: // выход без выбора файла 
		{
			chosenMenu = true;
			cur = begCoord;
			filepath = "";
			break;
		}
		case 13: // выбор папки 
		{
			filepath = filepath + "\\" + files[cur - begCoord];
			chosenMenu = true;
			cur = begCoord;
			break;
		}
		default: break;
		}
	}
}

//@brief создание списка файлов из директории, лежащей в программе
void makeFilesList(std::string filepath, std::vector<std::string>& folderList, fileExtension condition) {
	for (auto const& dirFolder : std::filesystem::directory_iterator(filepath + "\\")) {
		{ // цикл сохраняет файлы, выполняющие условие condition
			if (dirFolder.is_regular_file()
				&& condition(dirFolder.path().string())) {

				std::string path = dirFolder.path().string();
				path = path.substr(path.rfind("\\") + 1, path.size());

				folderList.push_back(path);
			}
		}
	}
}

//@brief передвижение стрелки для выбора файлом пользователем
int movingArrow(int ymin, int ymax, int cur, int key) {
	DWORD dw;
	COORD here{ 0, cur }; // координата стрелки в консоли (y - идет сверху вниз)
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) {
		printf("Invalid handle");
	}

	if (here.Y > ymin && key == 72) { // 72 - код клавиши стрелка вниз 
		// стирание бывшей стрелки и изменение ее координаты
		WriteConsoleOutputCharacterA(hStdOut, "  ", 2, here, &dw);
		here.Y -= 1;
	}
	if (here.Y < ymax && key == 80) { // 80 - код клавиши стрелка вниз 
		// стирание бывшей стрелки и изменение ее координаты
		WriteConsoleOutputCharacterA(hStdOut, "  ", 2, here, &dw);
		here.Y += 1;
	}

	WriteConsoleOutputCharacterA(hStdOut, ">>", 2, here, &dw);

	return here.Y;
}

//@return замена всех пробелов на нижние подчеркивания
std::string space2underscore(std::string text) {
	std::replace(text.begin(), text.end(), ' ', '_');
	return text;
}

//@return получение времени на момент создания файла для предотвращения перезаписи
std::string currentTime() {
	std::string res;

	// получение времени на данный момент
	std::time_t t = std::time(NULL);
	std::tm now{};
	localtime_s(&now, &t);

	// преобразование времени в читаемый вид
	std::string date = formatXX(now.tm_mday) + "-"
		+ formatXX(now.tm_mon + 1) + "-"
		+ std::to_string(now.tm_year + 1900);


	std::string clockTime;

	clockTime = formatXX(now.tm_hour) + "-"
		+ formatXX(now.tm_min) + "-"
		+ formatXX(now.tm_sec);

	res = date + "--" + clockTime;
	return res;
}

//@return форматирование даты и времени
std::string formatXX(int num) {
	if (num < 10)
		return "0" + std::to_string(num);
	else
		return std::to_string(num);
}

//@brief вывод списка действий в консоль
void ask(const std::vector<std::string> choice) {
	for (int i(0); i < choice.size(); i++) {
		std::cout << i + 1 << ". " << choice[i] << std::endl;
	}
}

//@brief реагирование на выбор пользователя действий
int inputChoice(const int& end) {
	int choiceInt = _getch();

	// цикл прерывается только при нажатии клавиши от 1 до <end>
	while (choiceInt <= '0' || choiceInt > char(end + '0'))
		choiceInt = _getch();

	choiceInt = choiceInt - '0'; // преобразование значения цифры из char в int 
	system("cls");
	return choiceInt;
}

//@return возврат выбранного пользователем названия файла, предотвращая ввод запрещенных символов
std::string askName(const std::string& question) {
	std::string res;
	std::cout << question << std::endl;

	bool approved(false); // переменная цикла, становится true когда строка введена без ошибок
	while (!approved) {
		try {
			std::cout << ">>";
			std::getline(std::cin, res);
			checkSpecialSymbols(res); // запрет на ввод символов \/:*?<>"|
			if (res != "")
				approved = true;
			else {
				system("cls");
				std::cout << question << std::endl;
			}
		}
		catch (std::exception& ex) {
			std::cout << ex.what() << std::endl; // вывод ошибки в консоль
		}
	}
	system("cls");
	return res;
}

//@brief проверка вводимого названия файла на специальные символы 
void checkSpecialSymbols(const std::string& word) {
	std::string forbiddenSymbols("\\/:*?<>\"|");
	for (int i(0); i < forbiddenSymbols.size(); i++) {
		if (word.find(forbiddenSymbols[i]) != -1)
			throw std::invalid_argument("Название файла/папки не может содержать \\/:*?<>\"|");
	}
}

//@brief настройка консоли
void standartSettings() {
	// установка русской кодировки для консоли
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::setlocale(LC_ALL, "ru");
}