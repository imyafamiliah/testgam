#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>

const int rowsCounts = 10;
const int columsCounts = 15;

const unsigned char symbolHero = 2;
const unsigned char symbolWall = 177;
const unsigned char symbolBox = 254;
const unsigned char symbolExit = 176;
const unsigned char levelData0[rowsCounts][columsCounts + 1] =
{
	"#####2#########",
	"# X    #   X ##",
	"#             #",
	"#     X       #",
	"#        X    #",
	"#   XX        #",
	"#          X  #",
	"#      X      #",
	"# 1   X       #",
	"###############",
};

HANDLE consoleHandle = 0;
bool isGameActive = true;
unsigned char levelData[rowsCounts][columsCounts];
int heroRow = 0;
int heroColumn = 0;

void SetupSystem()
{
	srand(time(0));

	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Initialize()
{
	for (int r = 0; r < rowsCounts; r++)
	{
		for (int c = 0; c < columsCounts; c++)
		{
			unsigned char symbol = levelData0[r][c];
			switch (symbol)
			{
				//Стена 
			case'#':
			{
				levelData[r][c] = symbolWall;
				break;
			}
			   //Игрок
			case'1':
			{
				levelData[r][c] = symbolHero;
				heroRow = r;
				heroColumn = c;
				break;
			}
			    //Ящик
			case'X':
			{
				levelData[r][c] = symbolBox;
				break;
			}
			//Другие символы
			default:
			{
				levelData[r][c] = symbol;
				break;
			}
			}
		}
	}
}

void Render()
{
	COORD cursorCoord;
	cursorCoord.X = 0;
	cursorCoord.Y = 0;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);
	printf("\n\n\t");
	for (int r = 0; r < rowsCounts; r++)
	{
		for (int c = 0; c < columsCounts; c++)
		{
			unsigned char symbol = levelData[r][c];

			switch (symbol)
			{
			case symbolWall:
			{
				SetConsoleTextAttribute(consoleHandle,10);
				break;
			}
			case symbolHero:
			{
				SetConsoleTextAttribute(consoleHandle, 15);
				break;
			}
			case symbolBox:
			{
				SetConsoleTextAttribute(consoleHandle, 14);
				break;
			}
			case symbolExit:
			{
				SetConsoleTextAttribute(consoleHandle, 12);
				break;
			}
			}
			printf("%c", symbol);
		}
		printf("\n\t");
	}
}
void MoveHeroTo(int row, int column)
{
	unsigned char destinationCell = levelData[row][column];
	bool canMoveToCell = false;

	switch (destinationCell)
	{
	case ' ':
	{
		canMoveToCell = true;
		break;
	}
	case symbolExit:
	{
		isGameActive = false;
		break;
	}
	case symbolBox:
	{
		int herodirectionr = row - heroRow;
		int herodirectionc = column - heroColumn;

		if (levelData[row + herodirectionr][column + herodirectionc] == ' ')
		{
			canMoveToCell = true;
			levelData[row][column] = ' ';
			levelData[row + herodirectionr][column + herodirectionc] = symbolBox;
		}
		break;
	}
	}
	if (canMoveToCell)
	{
		levelData[heroRow][heroColumn] = ' ';
		heroRow = row;
		heroColumn = column;
		levelData[heroRow][heroColumn] = symbolHero;
	}
}
void Update()
{
	unsigned char inputChar = _getch();
	inputChar = tolower(inputChar);

	switch (inputChar)
	{
	case'w':
	{
		MoveHeroTo(heroRow - 1, heroColumn);
		break;
	}
	case's':
	{
		MoveHeroTo(heroRow + 1, heroColumn);
		break;
	}
	case'a':
	{
		MoveHeroTo(heroRow , heroColumn-1);
		break;
	}
	case'd':
	{
		MoveHeroTo(heroRow , heroColumn+1);
		break;
	}
	case'r':
	{
		Initialize();
		break;
	}
	}
}

void Shutdown()
{
	system("cls");
	printf("\n\t Press any key to continue...");
	_getch();
}

int main()
{
	SetupSystem();
	Initialize();

	do
	{
		Render();
		Update();
	} while (isGameActive);

	Shutdown();

	return 0;
}