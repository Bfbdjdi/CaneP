#include <iostream>
#include <climits> //useful math constants
#include <Windows.h> //playing with a window propeties
#include <vector>
#include <random>
#include <string>
#include <conio.h>
#include <algorithm> //just neat functions
#pragma execution_character_set( "utf-8" ) //Any language will be printed in a cmd without problems

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////
// Variables used in several functions many times

int width, hight; //width and hight of a game field
int numOfDug = 0; //how many mines have been dug; when it's equal numOfSaves, a player wins
///////////////////////////////////////////////////////////////////////////////////////////////

//Is an input a number?
int isNumber(string snum)
{
	try
	{
		int num = stoi(snum);
		return num;
	}
	catch (invalid_argument)
	{
		return -400; //the game should ask a player to reenter the number
	}
}

//Random nums generator with set borders (from 'a' to 'b')
int random(int a, int b)
{
	if (a > 0) return a + rand() % (b - a);
	else return a + rand() % (abs(a) + b);
}

//Clearing a console
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

//Copying chosen elements from one matrix to another (or changing values of chosen elements to other values in a single matrix)
vector <vector<char>> matrixElementsReplacer(vector <vector<char>>& matrixCopyFrom, vector <vector<char>>& matrixCopyTo, char searchWhat, char replaceWithWhat, int matrixWidth, int requiredMatrixHight)
{
	for (int i = 0; i < matrixWidth; i++)
	{
		for (int j = 0; j < requiredMatrixHight; j++)
		{
			if (matrixCopyFrom[i][j] == searchWhat)
			{
				numOfDug += 1;
				matrixCopyTo[i][j] = replaceWithWhat;
			}
		}
	}

	return matrixCopyTo;
}

//Grouping zero-ish squares in gridMines
vector <vector<char>> matrixSingleElementsUniter(vector <vector<char>>& matrix)
{
	char groupLast = 64; //it's a letter 'А' for the first (group of) mine(-s) / variable is used to mark different groups of mines with different letters\
		(letters are hidden from a player)

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < hight; j++)
		{
			if (matrix[i][j] == '#') //if this square has a value of 0
			{

				bool worked = false; //just a thing that makes the following 'try' block work
				try
				{
					//checking a square on the top of the selected square (matrix[i][j])

					if (((int)matrix.at(i - 1).at(j) >= 65) && ((int)matrix.at(i - 1).at(j) <= 90)) //if there is a letter A-Z(2) on the top of the chosen square(1)
					{
						matrix.at(i).at(j) = matrix.at(i - 1).at(j); //changing the square's (1) value to (2)
						worked = true;

						//if there is a letter(1) on the top, but it is not a letter(2) on the left, we seek for every appearence of the (2) and change them to (1)
						if ((((int)matrix.at(i).at(j - 1) >= 65) && ((int)matrix.at(i).at(j - 1) <= 90)/*if there is a letter on the top...*/) && \
							(matrix.at(i - 1).at(j) != matrix.at(i).at(j - 1))/*... and it isn't a (2)*/)
						{
							matrix = matrixElementsReplacer(matrix/*copy from*/, matrix/*copy to*/, matrix.at(i).at(j - 1)/*change what...*/, matrix.at(i - 1).at(j)/*...to what*/, \
								width, j /*only a selected part of matr's will be modified*/);
						}

						continue;
					}

				}
				//if we try to access invalid squares
				catch (out_of_range)
				{
					if (worked == true)
						continue;
				}

				try
				{

					//checking a square on the left
					if (((int)matrix.at(i).at(j - 1) >= 65) && ((int)matrix.at(i).at(j - 1) <= 90)) 
					{
						matrix.at(i).at(j) = matrix.at(i).at(j - 1);
						continue;
					}

				}
				catch (out_of_range) {/*nothing*/ }

				try
				{

					//checking a square on the right
					if (((int)matrix.at(i).at(j + 1) >= 65) && ((int)matrix.at(i).at(j + 1) <= 90)) 
					{
						matrix.at(i).at(j) = matrix.at(i).at(j + 1);
						continue;
					}

				}
				catch (out_of_range) {/*nothing*/ }

				//if a chosen square is not near other squares from other groups
				groupLast += 1; //adding a new group of mines
				matrix.at(i).at(j) = groupLast; //adding this new mine to the new group

				continue;
			}
		}
	}

	return matrix;
}

/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Variables to store phrases, written in a language (English, Russian, etc.) 
string err_number_outside_suitable_range; //validNum
string whether_windowed_or_fullscreen, err_invalid_window_mode_chosen; //windowsFormatter
string err_invalid_ingame_action, enter_coordinates, err_not_a_number, err_nonexistent_field_chosen, action_chooser, endgame_win, endgame_lose, endgame_score, \
	num_of_mines, dug_several_times; //gameplay
string intro_text; //intro
string enter_field_sizes, err_15x15_is_max, enter_amount_of_mines, err_too_many_mines; //main
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int chosenLang; //this variable is to remember what language was chosen by a player
void languageSelector()
{
	SetConsoleOutputCP(65001); //Unicode support

	bool okenter = false;

	while (okenter != true) //the program will leave this 'while' when a user enters a suitable number
	{
		cout << "English (1), Русский язык (2)? \nChoise: ";
		okenter = true;

		string STRch;
		cin >> STRch;

		if (isNumber(STRch) != -400) //if the input is not a number, the function will return -400, otherwise we will get a true integer
		{
			int ch = isNumber(STRch);

			switch (ch) //if the input is a number...
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
			default: //a user tried to choose a non-existing language
			{
				clear();
				cout << "Choose between mentioned languages.\nType again: ";
				okenter = false;
				break;
			}
			}
		}
		else 
		{
			okenter = false;
			clear();
			cout << "Only numbers can be used. Type again.\n";
		}
	}

	switch (chosenLang)
	{
	case 1: //English
	{

		err_number_outside_suitable_range = "Only numbers between 0 and int32max can be entered. Type again.";
		whether_windowed_or_fullscreen = "The game should be in window (1) or fullscreen (2)?\n";
		err_invalid_window_mode_chosen = "Only '1'/'2' can be entered. Type again.\n";
		err_invalid_ingame_action = "Only '1' and '2' can be used.\n\n";
		enter_coordinates = "\nEnter X and Y:\n";
		err_not_a_number = "Only numbers can be entered here. Type again.\n";
		err_nonexistent_field_chosen = "Yain't able to choose non-existing spaces. Type again.\n";
		action_chooser = "\nDig (1) or mark (2)?\n";
		num_of_mines = "\nMines to avoid: ";
		dug_several_times = "No need to dig a single square twice.\n\n";
		endgame_lose = "Fin.";
		endgame_win = "You won!";
		endgame_score = "\nYour score: ";
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
	case 2: //Russian
	{

		err_number_outside_suitable_range = "Подходят только числа от нуля до int32max. Введи подходящие значения.";
		whether_windowed_or_fullscreen = "Запустить игру в окне (1) или на весь экран (2)?\n";
		err_invalid_window_mode_chosen = "Тут подходят только '1'/'2'. Введи подходящие значения.\n";
		err_invalid_ingame_action = "Тут могут быть использованы только '1' и '2'.\n\n";
		enter_coordinates = "\nВведи координаты нужного квадрата (X, Y):\n";
		err_not_a_number = "Тут подходят только числа. Введи подходящие значения.\n";
		err_nonexistent_field_chosen = "Нельзя обращаться к несуществующим квадратам поля. \nВведи подходящие значения.\n";
		action_chooser = "\nВыкопать (1) или пометить (2)?\n";
		num_of_mines = "\nМин на поле: ";
		dug_several_times = "Не нужно выкапывать один квадрат дважды.\n\n";
		endgame_lose = "Игра окончена.";
		endgame_win = "Победа!";
		endgame_score = "\nНабранные очки: ";
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

//Displaying a matrix in a cmd
void display(vector <vector<char>>& grid)
{

	if (width > 9)   //
		cout << " "; //if the matr's hight is 10+, we add an additional space in the top left corner to make the horizontal "ruller" look ok

	cout << " _|";  //*decoration*

	for (int column = 0; column < hight; column++)
	{
		cout << " " << column + 1 << "."; //printing a horizontal "ruller"
	}

	cout << endl;

	for (int i = 0; i < width; i++)
	{
		if ((width > 9) && (i < 9))  //
			cout << " ";		     //an additional space to make numbers 1-9 look ok when hight is 10+
		cout << " " << i + 1 << ". ";    //printing a vertical "ruller"
		for (int j = 0; j < hight; j++)
		{
			if ((hight > 9) && (j > 9))  //
				cout << " ";             //an additional space to fit in numbers 10+
			cout << grid[i][j] << "  ";  
		}
		cout << endl;
	}
}

//A radar to find mines nearby
int radar(vector <vector<char>>& grid, vector <vector<char>>& gridMines, int chX, int chY)
{

	int minesCounter = 0; //how many mines nearby
	for (int X = chX - 1; X <= chX + 1; ++X)      //
	{											  //we ananalize the surroundings in a radius of 1
		for (int Y = chY - 1; Y <= chY + 1; ++Y)  //
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

// 0 <= a number <= int32max ?
int validNum(int input, int low = 0, int high = INT32_MAX)
{
	if ((input > low) && (input < high))
	{
		return 0; //if the number is ok
	}
	else
	{
		cout << "\n" << err_number_outside_suitable_range << "\n";
		return 1; //asking a user to reenter the number
	}
}

//Setting up a cmd window 
bool windowed = false; //if the game is in "small window" mode
void windowFormatter()
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
			if (ch == 1) //"small window" mode
			{

				//string windowConfig = "MODE " + to_string(windowWidth) + "," + to_string(windowHight); //
				//system(windowConfig.c_str());															 //in case we want to manually adjust the sizes from the console
				system("MODE 80,25");

				fontWidth = 8;
				fontHight = 16;

				windowed = true;
				break;
			}
			else if (ch == 2) //"fullscreen" mode
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

	clear();

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

//The game itself
void gameplay(vector <vector<char>>& grid, vector <vector<char>>& gridMines, int numOfSafes, int numOfMines)
{
	if (windowed == true)     //if the game is in a small window, the latter will be shrinked even more
		system("MODE 60,26");

	/////////////////////////////////////////////////////
	//Making scrollbars appear

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	csbi.dwSize.Y = 10000;//10000 lines of vertical space buffer in console (?)
	SetConsoleScreenBufferSize(hConsole, csbi.dwSize);

	/////////////////////////////////////////////////////

	bool active = true; //has the game ended?

	int score = 0;
	bool actionMisType = false, actionDugTwice = false;

	while (active == true) //while the game has not been finished
	{

		if (numOfDug == numOfSafes) //in case the game is successfully won
		{
			active = false; //stopping the current round
			clear();

			display(grid);

			break;
		}

		clear();

		if (actionMisType == true)  //handling mistypes
		{
			actionMisType = false;
			cout << err_invalid_ingame_action;
		}

		if (actionDugTwice == true) //not allowing to dig a square twice
		{
			actionDugTwice = false;
			cout << dug_several_times;
		}

		display(grid);

		cout << num_of_mines << numOfMines << endl; //prints the total number of mines
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
				chY = stoi(STRUserChX);//making X coords horizonal, Y coords vertical
			}
			catch (invalid_argument)
			{
				cout << err_not_a_number;
				okEnter = false;
				continue;
			}

			chX -= 1; //
			chY -= 1; //the entered coordinates vary from *1* to a certain number, so we -= 1 both coordinates for our own comfort (to make them vary from *0* to number-1)

			if (((chX < 0) || (chX > width - 1)) || ((chY < 0) || (chY > hight - 1))) //when trying to access a non-existing square
			{
				okEnter = false;
				cout << err_nonexistent_field_chosen;
				continue;
			}
		}

		cout << action_chooser;
		string STRact;
		cin >> STRact;

		int act = isNumber(STRact); 

		{
			switch (act)
			{
			case (1): //the command 'dig'
			{
				if (gridMines[chX][chY] == '*') //if there is a mine OR a user has won
				{
					active = false;
					clear();
				}
				else
				{
					if ((gridMines[chX][chY] >= 65) && (gridMines[chX][chY] <= 90) && (grid[chX][chY] == '#')/*if gridMines[chX][chY] is from a zeros group AND it hasn't been revealed yet*/)
					{
						grid = matrixElementsReplacer(gridMines, grid, gridMines[chX][chY], '0', width, hight);
					}
					else
					{
						if ((grid[chX][chY] >= 48) && (grid[chX][chY] <= 57)) //stopping from digging a square twice
						{
							actionDugTwice = true;
							break;
						}
						else
						{
							char tmpMinesNum = radar(grid, gridMines, chX, chY) + '0'; //searching for mines nearby (and counting them)

							grid[chX][chY] = tmpMinesNum; //saving the number of mines in grid

							numOfDug += 1; //how many "mine-less" squares have been dug out
							score += radar(grid, gridMines, chX, chY) * 150; //the in-game score counter
						}

					}

				}
				break;
			}

			case (2): //(un-)marking a square
			{
				if (grid[chX][chY] == '^')
					grid[chX][chY] = '#'; //unmarking
				else if (grid[chX][chY] == '#')
					grid[chX][chY] = '^'; //marking if the square hasn't been dug out
				break;
			}

			default:
			{
				actionMisType = true; //the user entered neither 'D' nor 'M'
				break;
			}
			}
		}
			
	}

	if ((active == false) && (numOfDug != numOfSafes)) //if the user has lost
	{
		display(grid);
		cout << "\n------------------------------\n\n" << endgame_lose << "\n";
		display(gridMines);
	}
	else //if the user has won
		cout << "\n------------------------------\n\n" << endgame_win << "\n";

	cout << endgame_score << score;
	cout << endl << "==============================\n\n";

	numOfDug = 0; //to prepare for the next round with a new field
}

//The rules
void intro()
{
	cout << intro_text;
}

//Preparing for a round
int main()
{
	system("MODE 52,5"); //shrinking the cmd window

	languageSelector(); //the user chooses the language 

	windowFormatter(); //resizing the window
	intro();

	while (true) //an infinite loop
	{

		cout << enter_field_sizes;

		string shight, swidth;

		bool okenter = false;
		while (okenter != true)
		{
			okenter = true;
			cin >> shight >> swidth;

			width = isNumber(swidth);
			hight = isNumber(shight);

			if ((width == -400) || (hight == -400)/*a number is negative or greater than int32max*/\
				|| (validNum(width) == 1) || (validNum(hight) == 1)) /*if the input is not a number*/
			{
				okenter = false;
				cout << err_not_a_number;
			}
			if ((width > 15) || (hight > 15)) //the field cannot be larger than 15x15 (or it won't fit in the screen normally)
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

			if (numOfMines == -400) //not a number
				okenter = false;

			if (numOfMines >= hight * width) //too many mines
			{
				okenter = false;
				cout << err_too_many_mines;
			}

			if (validNum(numOfMines) == 1) //negative or int32max
				okenter = false;
		}

		int numOfSafes = hight * width - numOfMines; //the amount of safe squares

		clear(); 
		vector <vector<char>> grid(width, vector<char>(hight));       //what the user will see
		vector <vector<char>> gridMines(width, vector<char>(hight));  //where the mines will be stored

		for (int i = 0; i < width; i++) //making backgrounds for both matrixes
		{
			for (int j = 0; j < hight; j++)
			{
				grid[i][j] = '#';
				gridMines[i][j] = '#';
			}
		}

		//Generating mines
		srand(time(NULL)); //to make the generator create almost random numbers
		int i = 0;
		while (i < numOfMines)
		{
			int randX = random(0, width - 1);
			int randY = random(0, hight - 1);

			if (gridMines[randX][randY] != '*')
			{
				gridMines[randX][randY] = '*';
				i += 1;
				//////////////////////////////////////////////////////////////////////////////////////////
				// Creating around the mines 3x3 zones, where zeros can't be found 

				for (int markX = randX - 1; markX < randX + 2; markX++)
				{
					for (int markY = randY - 1; markY < randY + 2; markY++)
					{
						try
						{
							if (gridMines.at(markX).at(markY) != '*') //if it's not a mine
							{
								gridMines.at(markX).at(markY) = '@'; //marking as a non-zero square without a mine
							}
						}
						catch (out_of_range)
						{
							continue;
						}
					}
				}
				//////////////////////////////////////////////////////////////////////////////////////////

			}

		}

		gridMines = matrixSingleElementsUniter(gridMines);

		gameplay(grid, gridMines, numOfSafes, numOfMines);
	}
}