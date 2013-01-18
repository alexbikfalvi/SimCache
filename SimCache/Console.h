#ifndef __Console_h__
#define __Console_h__

class CConsole
{
public:
	enum EColor
	{
		BLACK = 0,
		DARK_BLUE = 1,
		DARK_GREEN = 2,
		DARK_CYAN = 3,
		DARK_RED = 4,
		DARK_MAGENTA = 5,
		DARK_YELLOW = 6,
		DARK_GRAY = 7,
		LIGHT_GRAY = 8,
		LIGHT_BLUE = 9,
		LIGHT_GREEN = 10,
		LIGHT_CYAN = 11,
		LIGHT_RED = 12,
		LIGHT_MAGENTA = 13,
		LIGHT_YELLOW = 14,
		WHITE = 15
	};
	static void SetColor(EColor color);
	static void MoveCursorBeginLine();
};

#endif