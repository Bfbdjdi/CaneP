//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <climits> //постоянные со значениями границ типов данных
#include <Windows.h> //настройки для командного окна
#include <vector>
#include <random>
#include <string>
#include <conio.h>
#include <algorithm> //занятные и очень удобные функции
#pragma execution_character_set( "utf-8" ) //Любой язык в консоли отобразится верно

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

/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Переменные, в которые будут записаны фразы на определённом языке
string err_number_outside_suitable_range; //validNum
string whether_windowed_or_fullscreen, err_invalid_window_mode_chosen; //windowsFormatter
string err_invalid_ingame_action, enter_coordinates, err_not_a_number, err_nonexistent_field_chosen, action_chooser, endgame_win, endgame_lose, endgame_score; //gameplay
string intro_text; //intro
string enter_field_sizes, err_15x15_is_max, enter_amount_of_mines, err_too_many_mines; //main
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int chosenLang; //сюда в виде номера будет сохранён выбранный язык, который будет использован далее
void languageSelector()
{
	SetConsoleOutputCP(65001); //Unicode в консоль
	
	bool okenter = false;

	while (okenter != true)
	{
		cout << "English (1), Русский язык (2)? \nChoise: ";
		okenter = true;

		string STRch;
		cin >> STRch;

		if (isNumber(STRch) != -400)//проверка на правильность ввода
		{
			int ch = isNumber(STRch);

			switch (ch) //если введённое оказалось числом
			{
			case 1:
			{
				chosenLang = 1;
				break;
			}
			case 2:
			{
				chosenLang = 2;
				break;
			}
			default:
			{
				clear();
				cout << "Choose between mentioned languages.\nType again: ";
				okenter = false;
				break;
			}
			}
		}
		else //если введено совсем не то
		{
			okenter = false;
			clear();
			cout << "Only numbers can be used. Type again.\n";
		}
	}

	switch (chosenLang)
	{
	case 1: //английский
	{
		
		err_number_outside_suitable_range = "Only numbers between 0 and int32max can be entered. Type again.";
		whether_windowed_or_fullscreen = "The game should be in window (1) or fullscreen (2)?\n";
		err_invalid_window_mode_chosen = "Only '1'/'2' can be entered. Type again.\n";
		err_invalid_ingame_action = "Only '1' and '2' can be used.\n\n";
		enter_coordinates = "\nEnter X and Y:\n";
		err_not_a_number = "Only numbers can be entered here. Type again.\n";
		err_nonexistent_field_chosen = "Yain't able to choose non-existing spaces. Type again.\n";
		action_chooser = "\nDig (1) or mark (2)?\n";
		endgame_lose = "Fin.";
		endgame_win = "You won!";
		endgame_score = "\n\nYour score: ";
		intro_text = "Actually, this is just a normal minesweeper with it's basic rules.\n\nAt first you enter dimensions of a field ('2 2' OR '2 enter 2' for \
width \nand hight respectively) and how many mines you want to have.\n\nDuring your playthrough, you enter X and Y coordinates of a square, which \
you \ncan dig out or mark/unmark.\n\n'#' - untouched squares, '^' - marked squares, '*' - mines.\n\nOn the top you will see your field, lower will \
be a counter of dug out squares.\n\n===============================================================================\n\n";
		enter_field_sizes = "Enter the dimensions of the field (width, hight): \n";
		err_15x15_is_max = "\n15x15 is the maximum. Type in a smaller field.\n";
		enter_amount_of_mines = "\n\nEnter the amount of mines: \n";
		err_too_many_mines = "Too many mines for this field. Type again.\n";
		break;
	}
	case 2: //русский
	{
		
		err_number_outside_suitable_range = "Подходят только числа от нуля до int32max. Введи подходящие значения.";
		whether_windowed_or_fullscreen = "Запустить игру в окне (1) или на весь экран (2)?\n";
		err_invalid_window_mode_chosen = "Тут подходят только '1'/'2'. Введи подходящие значения.\n";
		err_invalid_ingame_action = "Тут могут быть использованы только '1' и '2'.\n\n";
		enter_coordinates = "\nВведи координаты нужного квадрата (X, Y):\n";
		err_not_a_number = "Тут подходят только числа. Введи подходящие значения.\n";
		err_nonexistent_field_chosen = "Нельзя обращаться к несуществующим квадратам поля. \nВведи подходящие значения.\n";
		action_chooser = "\nВыкопать (1) или пометить (2)?\n";
		endgame_lose = "Игра окончена.";
		endgame_win = "Победа!";
		endgame_score = "\n\nНабранные очки: ";
		intro_text = "Вообще, это приложение - самый обыкновенный \"Сапёр\" с его правилами. \n\nВ начале нужно ввести размеры будущего игрового поля ('2 2' ИЛИ '2 enter 2' \n\
для ширины и высоты соответственно) и количество мин.\n\nВо время игры вводишь координаты X и Y квадрата, который можно или вскопать, или \
пометить.\n\n'#' - нетронутые квадраты, '^' - помеченные квадраты, '*' - мины. \n\n===============================================================================\n\n";
		enter_field_sizes = "Введи размеры игрового поля (ширина, высота): \n";
		err_15x15_is_max = "\nРазмеры поля должны не превышать 15x15. Введи размеры поменьше.\n";
		enter_amount_of_mines = "\n\nКоличество мин: \n";
		err_too_many_mines = "Для этого поля слишком много мин. Введи подходящее значение.\n";
		break;
	}
	}
}

//Авто-выводилка игрового поля на экран
void display(vector <vector<char>> grid, int width, int hight)
{
	
	if (width > 9)
		cout << " ";

	cout << " _|";

	for (int column = 0; column < hight; column++)
	{
		cout << " " << column + 1 << ".";
	}

	cout << endl;

	for (int i = 0; i < width; i++)
	{
		if ((width > 9) && (i < 9))
			cout << " ";
		cout << " " << i + 1 << ". ";
		for (int j = 0; j < hight; j++)
		{
			if ((hight > 9) && (j > 9))
				cout << " ";
			cout << grid[i][j] << "  ";
		}
		cout << endl;
	}
}

//Радар для подсчёта количества мир поблизости
int radar(vector <vector<char>> grid, vector <vector<char>> gridMines, int chX, int chY)
{
	int xScan, yScan;
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

	/*if (minesCounter == 0)
	{
		xScan = chX;//
		yScan = chY;// Точка начала сканирования

		try
		{
			while ((gridMines.at(xScan).at(yScan) != '*') || (yScan >= 0))
			{

				gridMines.at(xScan).at(yScan) = radar(grid, gridMines, xScan, yScan) + '0';
	yScan -= 1;			
				clear();
				display(gridMines, 7, 7);
			}
		}
		catch (out_of_range)
		{

		}

		//пока нет препятствия сверху идём сканером вверх 



	}*/

	/*if (minesCounter == 0)
	{

		for (int X = chX - 1; X <= chX + 1; ++X)
		{
			for (int Y = chY - 1; Y <= chY + 1; ++Y)
			{
				try
				{
					grid.at(chX).at(chY) = '0';

					clear();
					char mid = (radar(grid, gridMines, X, Y))+'0';
					grid.at(X).at(Y) = mid ;


				}
				catch (out_of_range)
				{
					continue;
				}
				display(grid, 7, 7);
				cout << "\n\n==============\n\n";
				display(gridMines, 7, 7);
			}
		}

	}*/
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
		cout << "\n" << err_number_outside_suitable_range << "\n";
		return 1; //требуется переввод числа
	}
}

//Первоначальная настройка окна (его размер, шрифт текста)
bool windowed = false; //если игра запущена в маленьком окне
void windowFormatter(/*int width, int hight*/)
{

	cout << whether_windowed_or_fullscreen;
	string STRch;
	int ch;
	int fontWidth, fontHight;

	bool okenter = false;
	while (okenter != true)
	{
		cin >> STRch;
		ch = isNumber(STRch);

		okenter = true;
		if (ch != -400)
		{
			if (ch == 1)
			{

				//string windowConfig = "MODE " + to_string(windowWidth) + "," + to_string(windowHight); //произвольные, вводимые пользователем размеры
				//system(windowConfig.c_str()); //размеры окна консоли
				system("MODE 80,25");

				fontWidth = 8;
				fontHight = 16;

				windowed = true;
				break;
			}
			else if (ch == 2)
			{
				ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

				fontWidth = 13;
				fontHight = 24;
				break;
			}
			else
			{
				cout << err_invalid_window_mode_chosen;
				okenter = false;
				continue;
			}
		}
		else
		{
			okenter = false;
			cout << err_not_a_number;
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
		system("MODE 60,26");

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
			cout << err_invalid_ingame_action;
		}

		display(grid, width, hight);
		//cout << "\n===========================\n";
		//display(gridMines, width, hight);

		cout << endl << numOfDigged << " | " << numOfSafes << endl; //выкопано | осталось выкопать
		cout << enter_coordinates;
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
				cout << err_not_a_number;
				okEnter = false;
				continue;
			}

			chX -= 1; //
			chY -= 1; //смещение координат для правильной работы с массивами

			if (((chX < 0) || (chX > width - 1)) || ((chY < 0) || (chY > hight - 1))) //если попытка обратиться к ячейке вне массива
			{
				okEnter = false;
				cout << err_nonexistent_field_chosen;
				continue;
			}
		}

		cout << action_chooser;
		string STRact;
		cin >> STRact;

		int act = isNumber(STRact); //независимость ввода от регистра

		if (act != -400)
		{
			switch (act)
			{
			case (1): //копание
			{
				if (gridMines[chX][chY] == '*') //если попались на мину ИЛИ победа
				{
					active = false;
					clear();
				}
				else
				{
					char tmpMinesNum = radar(grid, gridMines, chX, chY) + '0'; //если мины не оказалось
			//if (tmpMinesNum != '0')
					grid[chX][chY] = tmpMinesNum;
					//else
					//{
						//for (int addX = -1; addX < 2; addX++)
						//{
						//	char tmpMinesNum;
						//	for (int addY = -1; addY < 2; addY++)
						//	{

						//		try
						//		{
						//			if (gridMines.at(chX + addX).at(chY + addY) == '*')
						//				continue;
						//			tmpMinesNum = radar(grid, gridMines, chX + addX, chY + addY) + '0'; //если мины не оказалось

						//			grid[chX + addX][chY + addY] = tmpMinesNum;
						//		}
						//		catch (out_of_range)
						//		{
						//			continue;
						//		}


						//	}
						//}
					//}
					numOfDigged += 1;
					score += radar(grid, gridMines, chX, chY) * 150;
				}
				break;
			}

			case (2): //установка или снятие метки
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
		}
		else
			cout << err_not_a_number;
	}

	if ((active == false) && (numOfDigged != numOfSafes))
	{
		display(grid, width, hight);
		cout << "\n------------------------------\n\n" << endgame_lose << "\n";
		display(gridMines, width, hight);
	}
	else
		cout << "\n------------------------------\n\n" << endgame_win<< "\n";

	cout << endgame_score << score;
	cout << endl << "==============================\n\n";
}

//Краткое объяснение правил в самом начале
void intro()
{
	cout << intro_text;
}

//Лобби и подготовка к игре
int main()
{
	system("MODE 52,5"); //уменьшение консоли при запуске (держим размеры окна под контролем)

	languageSelector();
	
	windowFormatter();
	intro();

	while (true) //игра перезапускается по кругу при завершении попытки
	{

		cout << enter_field_sizes;
		
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
			{
				okenter = false;
				cout << err_not_a_number;
			}
			if ((width > 15) || (hight > 15))
			{
				okenter = false;

				cout << err_15x15_is_max;
			}

		}

		cout << enter_amount_of_mines;
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
				cout << err_too_many_mines;
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