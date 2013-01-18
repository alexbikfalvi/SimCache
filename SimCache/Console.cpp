#include "stdafx.h"
#include "Console.h"
#include <windows.h>

void CConsole::SetColor(CConsole::EColor color)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	switch(color)
	{
	case CConsole::BLACK: SetConsoleTextAttribute(console, 0); break;
	case CConsole::DARK_BLUE: SetConsoleTextAttribute(console, FOREGROUND_BLUE); break;
	case CConsole::DARK_GREEN: SetConsoleTextAttribute(console, FOREGROUND_GREEN); break;
	case CConsole::DARK_CYAN: SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN); break;
	case CConsole::DARK_RED: SetConsoleTextAttribute(console, FOREGROUND_RED); break;
	case CConsole::DARK_MAGENTA: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_BLUE); break;
	case CConsole::DARK_YELLOW: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN); break;
	case CConsole::DARK_GRAY: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); break;
	case CConsole::LIGHT_GRAY: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
	case CConsole::LIGHT_BLUE: SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
	case CConsole::LIGHT_GREEN: SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
	case CConsole::LIGHT_CYAN: SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
	case CConsole::LIGHT_RED: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY); break;
	case CConsole::LIGHT_MAGENTA: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
	case CConsole::LIGHT_YELLOW: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
	case CConsole::WHITE: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
	}
}

void CConsole::MoveCursorBeginLine()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	
	
	GetConsoleScreenBufferInfo(console, &consoleInfo);

	consoleInfo.dwCursorPosition.X = 0;
	consoleInfo.dwCursorPosition.Y -= 1;

	SetConsoleCursorPosition(console, consoleInfo.dwCursorPosition);
}