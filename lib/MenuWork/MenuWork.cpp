#include "MenuWork.h"

//@brief ����� ������ ������ � ������� ��� �� ������ ������������� 
void showDir(const std::vector<std::string>& dir) {
	for (const auto& item : dir) {
		std::cout << "  " << item << std::endl;
	}
}

//@brief ����� ����� ����� ������� �������������
//@return ������� ���������� ����� � ���� ������
std::string findFile(std::string title, fileExtension condition) {
	std::filesystem::path p = "."; // ��������� ����, ��� ��������� ���������
	std::string curFilepath = std::filesystem::absolute(p).string();

	// ����� �� ����� ���������� ����� ������ ���� ��� ������������ ����� ����� � ����
	while (!condition(curFilepath) && curFilepath != "") {
		try {
			std::vector<std::string> fileList;
			makeFilesList(curFilepath, fileList, condition);
			if (fileList.size() == 0) // ������ ���������� ��-�� ��������� ������
				throw std::invalid_argument("���������� �����, �� ������ ����� �� �������");
			std::cout << title << std::endl;

			std::vector<std::string> menu{
				"��� ������ ����� ����������� ENTER � ��������� ����� ����",
				"��� ������ �� ���� ������� ESC"
			};

			int begCoord = menu.size() + 1;
			int cur(begCoord); // ������� ���������� ������� ������ �����

			for (const auto& line : menu) {
				std::cout << line << std::endl;
			}
			showDir(fileList);

			fileChoice(begCoord, fileList, cur, curFilepath);

			system("cls");
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			return ""; // ����� � ���� �� ������
		}
	}
	return curFilepath;
}

//@brief ���������� ���� ������ ��� findFile()
void fileChoice(int begCoord, std::vector<std::string> files, int& cur, std::string& filepath) {
	movingArrow(begCoord, begCoord + files.size(), cur, 0); // ����� ������� ������ �����  

	bool chosenMenu(false);
	// ���� ������ �����
	while (!chosenMenu) {
		switch (int c = _getch()) {
		case 224: // ��� ������� �� ������� �� ���������� 
		{
			cur = movingArrow(begCoord, begCoord + files.size() - 1, cur, _getch());
			break;
		}
		case 27: // ����� ��� ������ ����� 
		{
			chosenMenu = true;
			cur = begCoord;
			filepath = "";
			break;
		}
		case 13: // ����� ����� 
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

//@brief �������� ������ ������ �� ����������, ������� � ���������
void makeFilesList(std::string filepath, std::vector<std::string>& folderList, fileExtension condition) {
	for (auto const& dirFolder : std::filesystem::directory_iterator(filepath + "\\")) {
		{ // ���� ��������� �����, ����������� ������� condition
			if (dirFolder.is_regular_file()
				&& condition(dirFolder.path().string())) {

				std::string path = dirFolder.path().string();
				path = path.substr(path.rfind("\\") + 1, path.size());

				folderList.push_back(path);
			}
		}
	}
}

//@brief ������������ ������� ��� ������ ������ �������������
int movingArrow(int ymin, int ymax, int cur, int key) {
	DWORD dw;
	COORD here{ 0, cur }; // ���������� ������� � ������� (y - ���� ������ ����)
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) {
		printf("Invalid handle");
	}

	if (here.Y > ymin && key == 72) { // 72 - ��� ������� ������� ���� 
		// �������� ������ ������� � ��������� �� ����������
		WriteConsoleOutputCharacterA(hStdOut, "  ", 2, here, &dw);
		here.Y -= 1;
	}
	if (here.Y < ymax && key == 80) { // 80 - ��� ������� ������� ���� 
		// �������� ������ ������� � ��������� �� ����������
		WriteConsoleOutputCharacterA(hStdOut, "  ", 2, here, &dw);
		here.Y += 1;
	}

	WriteConsoleOutputCharacterA(hStdOut, ">>", 2, here, &dw);

	return here.Y;
}

//@return ������ ���� �������� �� ������ �������������
std::string space2underscore(std::string text) {
	std::replace(text.begin(), text.end(), ' ', '_');
	return text;
}

//@return ��������� ������� �� ������ �������� ����� ��� �������������� ����������
std::string currentTime() {
	std::string res;

	// ��������� ������� �� ������ ������
	std::time_t t = std::time(NULL);
	std::tm now{};
	localtime_s(&now, &t);

	// �������������� ������� � �������� ���
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

//@return �������������� ���� � �������
std::string formatXX(int num) {
	if (num < 10)
		return "0" + std::to_string(num);
	else
		return std::to_string(num);
}

//@brief ����� ������ �������� � �������
void ask(const std::vector<std::string> choice) {
	for (int i(0); i < choice.size(); i++) {
		std::cout << i + 1 << ". " << choice[i] << std::endl;
	}
}

//@brief ������������ �� ����� ������������ ��������
int inputChoice(const int& end) {
	int choiceInt = _getch();

	// ���� ����������� ������ ��� ������� ������� �� 1 �� <end>
	while (choiceInt <= '0' || choiceInt > char(end + '0'))
		choiceInt = _getch();

	choiceInt = choiceInt - '0'; // �������������� �������� ����� �� char � int 
	system("cls");
	return choiceInt;
}

//@return ������� ���������� ������������� �������� �����, ������������ ���� ����������� ��������
std::string askName(const std::string& question) {
	std::string res;
	std::cout << question << std::endl;

	bool approved(false); // ���������� �����, ���������� true ����� ������ ������� ��� ������
	while (!approved) {
		try {
			std::cout << ">>";
			std::getline(std::cin, res);
			checkSpecialSymbols(res); // ������ �� ���� �������� \/:*?<>"|
			if (res != "")
				approved = true;
			else {
				system("cls");
				std::cout << question << std::endl;
			}
		}
		catch (std::exception& ex) {
			std::cout << ex.what() << std::endl; // ����� ������ � �������
		}
	}
	system("cls");
	return res;
}

//@brief �������� ��������� �������� ����� �� ����������� ������� 
void checkSpecialSymbols(const std::string& word) {
	std::string forbiddenSymbols("\\/:*?<>\"|");
	for (int i(0); i < forbiddenSymbols.size(); i++) {
		if (word.find(forbiddenSymbols[i]) != -1)
			throw std::invalid_argument("�������� �����/����� �� ����� ��������� \\/:*?<>\"|");
	}
}

//@brief ��������� �������
void standartSettings() {
	// ��������� ������� ��������� ��� �������
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::setlocale(LC_ALL, "ru");
}