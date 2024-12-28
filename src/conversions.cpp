#include <cctype>
#include <string>
#include "board.h"
#include "conversions.h"
#include "move.h"
#include "piece.h"

std::string charToClassName(const char c)
{
	const char cl = std::tolower(c);
	switch (cl)
	{
	case 'b': return "Bishop";
	case 'k': return "King";
	case 'n': return "Night";
	case 'p': return "Pawn";
	case 'q': return "Queen";
	case 'r': return "Rook";
	default: return "Error/Unknown";
	}
}

std::string colorToString(const Color& color)
{
	if (color == black)
	{
		return "Black";
	}
	else
	{
		return "White";
	}
}

char digitToLetter(const int i)
{
	switch (i)
	{
	case 1: return 'A';
	case 2: return 'B';
	case 3: return 'C';
	case 4: return 'D';
	case 5: return 'E';
	case 6: return 'F';
	case 7: return 'G';
	case 8: return 'H';
	default: return '?';
	}
}

int letterToDigit(const char c)
{
	switch (c)
	{
	case 'a': return 1;
	case 'b': return 2;
	case 'c': return 3;
	case 'd': return 4;
	case 'e': return 5;
	case 'f': return 6;
	case 'g': return 7;
	case 'h': return 8;
	default: return -1;
	}
}

Color oppositeColor(const Color& color)
{
	if (color == black)
	{
		return white;
	}
	return black;
}

std::string stringToLowerCase(std::string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		s[i] = std::tolower(s[i]);
	}
	return s;
}

Move stringToMove(const Board& board, const std::string& move)
{
	const int ax = letterToDigit(move[0]);
	const int ay = move[1] - '0';
	const int bx = letterToDigit(move[3]);
	const int by = move[4] - '0';
	return Move{ax, ay, bx, by};
}