//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <climits> //постоянные со значениями границ типов данных
#include <Windows.h> //настройки для командного окна
#include <vector>
#include <random>
#include <string>
#include <conio.h>

using namespace std;

//Число или нет?
int isNumber(string snum)
{
	try
	{
		int num = stoi(snum);
		return num;
	}
	catch (invalid_argument)
	{
		return -400; //требование переввести число
	}
}

//Генератор случайных цифр
int random(int a, int b) //случайные числа в рамках
{
	if (a > 0) return a + rand() % (b - a);
	else return a + rand() % (abs(a) + b);
}

//Авто-выводилка игрового поля на экран
void display(vector <vector<char>> grid, int width, int hight) //вывод игрового поля
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < hight; j++)
		{
			cout << grid[i][j];
		}
		cout << endl;
	}
}

//Радар для подсчёта количества мир поблизости
int radar(vector <vector<char>> gridMines, int chX, int chY)
{
	int minesCounter = 0; //мин вокруг конкретной координаты
	for (int X = chX - 1; X <= chX + 1; ++X)
	{
		for (int Y = chY - 1; Y <= chY + 1; ++Y)
		{
			try
			{
				if (gridMines.at(X).at(Y) == '*')
					minesCounter += 1;
			}
			catch (out_of_range)
			{
				continue;
			}
		}
	}
	return minesCounter;
}

//Число больше нуля и не больше 32бит?
int validNum(int input, int low = 0, int high = INT32_MAX)
{
	if ((input > low) && (input < high))
	{
		return 0; //число подходит, дополнительные действия не требуются
	}
	else
	{
		cout << "\nOnly numbers between 0 and int32max can be entered. Type again.\n";
		return 1; //требуется переввод числа
	}
}

//Чистим консоль 
void clear() 
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

//Первоначальная настройка окна (его размер, шрифт текста)
bool windowed = false; //если игра запущена в маленьком окне
void windowFormatter(/*int width, int hight*/)
{

	cout << "The game should be in (w)indow or (f)ullscreen?\n";
	char ch;

	int fontWidth, fontHight;

	bool okenter = false;
	while (okenter != true)
	{
		cin >> ch;
		ch = toupper(ch);

		okenter = true;
		switch (ch)
		{
			case 'W':
			{
				//string windowConfig = "MODE " + to_string(windowWidth) + "," + to_string(windowHight); //произвольные, вводимые пользователем размеры
				//system(windowConfig.c_str()); //размеры окна консоли
				system("MODE 80,25");

				fontWidth = 8;
				fontHight = 16;

				windowed = true;
				break;
			}
			case 'F':
			{
				ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

				fontWidth = 14;
				fontHight = 28;
				break;
			}
			default:
			{
				cout << "Only 'w'/'f' can be entered. Type again.\n";
				okenter = false;
				break;
			}
		}
	}

	clear(); //очищает консоль

	SetConsoleTitleA("BRUTAL MINESWEEPER");

	// Obtain the Console handle
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Obtain the Console handle

	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();

	// set the size of the CONSOLE_FONT_INFOEX
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);

	// get the current value
	GetCurrentConsoleFontEx(hConsole, 0, lpConsoleCurrentFontEx);

	// set size to be 8x18, the default size is 8x16
	lpConsoleCurrentFontEx->dwFontSize.X = fontWidth;
	lpConsoleCurrentFontEx->dwFontSize.Y = fontHight;

	// submit the settings
	SetCurrentConsoleFontEx(hConsole, 0, lpConsoleCurrentFontEx);

}

//Собственно, тут расписан игровой процесс
void gameplay(vector <vector<char>> grid, vector <vector<char>> gridMines, int width, int hight, int numOfSafes)
{
	if (windowed == true)     //если игра в маленьком окне, то при начале первой игры окно уменьшается ещё сильнее
		system("MODE 55,20");

	/////////////////////////////////////////////////////
	//Это нужно для правильного отображения ползунка прокрутки 
	//окна cmd, без этого фрагмента ползунок то есть, то его нет.

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	csbi.dwSize.Y = 10000;//10000 строк
	SetConsoleScreenBufferSize(hConsole, csbi.dwSize);

	/////////////////////////////////////////////////////

	bool active = true;
	int numOfDigged = 0; //сколько полей уже выкапано; если равно numOfSaves, то победа
	int score = 0;
	bool actionMisType = false;

	while (active == true)
	{

		if (numOfDigged == numOfSafes) //если все мины уже найдены, то выход из цикла
		{
			active = false;
			break;
		}

		clear();

		if (actionMisType == true)
		{
			actionMisType = false;
			cout << "Only 'D' and 'M' can be used.\n\n";
		}

		display(grid, width, hight);

		cout << endl << numOfDigged << " | " << numOfSafes << endl; //выкопано | осталось выкопать
		cout << endl << "Enter X and Y:" << endl;
		int chX, chY;
		bool okEnter = false;

		while (okEnter != true)
		{
			okEnter = true;
			string STRUserChX, STRUserChY;
			cin >> STRUserChX >> STRUserChY;

			try
			{
				chX = stoi(STRUserChY);//
				chY = stoi(STRUserChX);//Инверсия управления, чтобы Х был горизонтален, Y вертикален
			}
			catch (invalid_argument)
			{
				cout << "Only numbers can be entered here. Type again.\n";
				okEnter = false;
				continue;
			}

			chX -= 1; //
			chY -= 1; //смещение координат для правильной работы с массивами

			if (((chX < 0) || (chX > width - 1)) || ((chY < 0) || (chY > hight - 1))) //если попытка обратиться к ячейке вне массива
			{
				okEnter = false;
				cout << "Yain't able to choose non-existing spaces. Type again.\n";
				continue;
			}
		}

		cout << endl << "(D)ig or (m)ark?" << endl;
		char act;
		cin >> act;

		act = toupper(act); //независимость ввода от регистра

		switch (act)
		{
		case ('D'): //копание
		{
			if (gridMines[chX][chY] == '*') //если попались на мину ИЛИ победа
			{
				active = false;
				clear();
			}
			else
			{
				//char tmpMinesNum = radar(gridMines, chX, chY) + '0'; //если мины не оказалось
				//grid[chX][chY] = tmpMinesNum;

				for (int addX = -1; addX < 2; addX++)
				{
					char tmpMinesNum;
					for (int addY = -1; addY < 2; addY++)
					{
						tmpMinesNum = radar(gridMines, chX + addX, chY + addY) + '0'; //если мины не оказалось
						grid[chX+addX][chY+addY] = tmpMinesNum;
					}
				}

				numOfDigged += 1;
				score += radar(gridMines, chX, chY) * 150;
			}
			break;
		}

		case ('M'): //установка или снятие метки
		{
			if (grid[chX][chY] == '^')
				grid[chX][chY] = '#'; //снятие метки
			else if (grid[chX][chY] == '#')
				grid[chX][chY] = '^'; //если ячейка ещё не выкопана, то ставим метку
			break;
		}

		default:
		{
			clear();
			actionMisType = true;
			break;
		}
		}

		display(grid, width, hight);

	}

	if ((active == false) && (numOfDigged != numOfSafes))
	{
		cout << "\n------------------------------\n\nFin" << "\n";
		display(gridMines, width, hight);
	}
	else
		cout << "\n------------------------------\n\nYou won!" << "\n";

	cout << "\n\nYour score: " << score;
	cout << endl << "==============================\n\n";
}

//Краткое объяснение правил в самом начале
void intro()
{
	printf("Actually, this is just a normal minesweeper with it's basic rules.\n\nAt first you enter dimensions of a field ('2 2' OR '2 enter 2' for \
width \nand hight respectively) and how many mines you want to have.\n\nDuring your playthrough, you enter X and Y coordinates of a square, which \
you \ncan dig out or mark/unmark.\n\n'#' - untouched squares, '^' - marked squares, '*' - mines.\n\nOn the top you will see your field, lower will \
be a counter of dug out squares.\n\n===============================================================================\n\n");

}

//Лобби и подготовка к игре
int main()
{

	//windowFormatter(80, 25);
	windowFormatter();
	intro();

	while (true) //игра перезапускается по кругу при завершении попытки
	{

		cout << "Enter the dimensions of the field (width, hight): \n";
		string shight, swidth;
		int hight, width;

		bool okenter = false;
		while (okenter != true)
		{
			okenter = true;
			cin >> shight >> swidth;

			width = isNumber(swidth);
			hight = isNumber(shight);

			if ((width == -400) || (hight == -400)/*если хотя бы одно число отрицательно или слишком большое*/\
				|| (validNum(width) == 1) || (validNum(hight) == 1)) /*если хотя бы одно число введено неверно ИЛИ не является числом*/
				okenter = false;

			if ((width > 15) || (hight > 15))
			{
				okenter = false;

				cout << "\n15x15 is the maximum. Type in a smaller field.\n";
			}
				
		}

		cout << "\n\nEnter the amount of mines: \n";
		string STRnumOfMines;

		int numOfMines;

		okenter = false;
		while (okenter != true)
		{
			okenter = true;
			cin >> STRnumOfMines;

			numOfMines = isNumber(STRnumOfMines);

			if (numOfMines == -400)
				okenter = false;

			if (numOfMines >= hight * width)
			{
				okenter = false;
				cout << "Too many mines for this field. Type again.\n";
			}

			if (validNum(numOfMines) == 1)
				okenter = false;
		}

		int numOfSafes = hight * width - numOfMines;

		clear(); //очищает консоль
		vector <vector<char>> grid(width, vector<char>(hight));
		vector <vector<char>> gridMines(width, vector<char>(hight));

		for (int i = 0; i < width; i++) //заполнение массивов фоном
		{
			for (int j = 0; j < hight; j++)
			{
				grid[i][j] = '#';
				gridMines[i][j] = '#';
			}
		}

		srand(time(NULL)); //ключ генератора чисел

		int i = 0;
		while (i < numOfMines)
		{
			int randX = random(0, width - 1);
			int randY = random(0, hight - 1);

			if (gridMines[randX][randY] != '*')
			{
				gridMines[randX][randY] = '*';
				i += 1;
			}

		}

		gameplay(grid, gridMines, width, hight, numOfSafes);
	}
}