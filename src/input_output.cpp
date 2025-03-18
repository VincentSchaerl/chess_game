#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "board.h"
#include "conversions.h"
#include "input_output.h"
#include "move.h"
#include "piece.h"
#include "play.h"

Color inputColor()
{
	std::cout << "Which color do you want to play (b for black, w for white)?\n\n\n";
	std::string input = inputString();
	std::cout << "\n\n";
	if (input == "b")
	{
		return black;
	}
	if (input == "w")
	{
		return white;
	}
	std::cout << "Invalid input.\n";
	return inputColor();
}

Move inputMove(const Board& board, const Color& playerColor)
{
	const std::string input = inputMoveString();
	const Move move = stringToMove(board, input);
	std::vector<Move> moves = checkMoves(board, playerColor);
	for (Move& moveElement: moves)
	{
		if (moveElement == move)
		{
			return moveElement;
		}
	}
	std::cout << "This move is not possible.\n";
	return inputMove(board, playerColor);
}

std::string inputMoveString()
{
	std::cout << "Please declare your move:\n\n\n";
	std::string input = inputString();
	std::cout << "\n\n";
	if (input.length() != 5 || !isCharValidX(input[0]) || !isCharValidY(input[1]) || input[2] != ' ' || !isCharValidX(input[3]) || !isCharValidY(input[4]))
	{
		std::cout << "Invalid input.\n";
		return inputMoveString();
	}
	return input;
}

bool inputPlayAgain()
{
	std::cout << "Do you want to play again? (n for no, y for yes)\n\n\n";
	std::string input = inputString();
	std::cout << "\n\n";
	if (input == "n")
	{
		return 0;
	}
	if (input == "y")
	{
		return 1;
	}
	std::cout << "Invalid input.\n";
	return inputPlayAgain();
}

std::shared_ptr<Piece> inputPromotion(const Color& playerColor)
{
	std::cout << "Which piece shall the Pawn be promoted to (b for Bishop, n for Knight, q for Queen, r for Rook)?\n\n\n";
	std::string input = inputString();
	std::cout << "\n\n";
	if (input == "b")
	{
		std::cout << colorToString(playerColor) << " Pawn was promoted to Bishop.\n\n";
		return std::make_shared<Bishop>(playerColor);
	}
	if (input == "n")
	{
		std::cout << colorToString(playerColor) << " Pawn was promoted to Knight.\n\n";
		return std::make_shared<Knight>(playerColor);
	}
	if (input == "q")
	{
		std::cout << colorToString(playerColor) << " Pawn Pawn was promoted to Queen.\n\n";
		return std::make_shared<Queen>(playerColor);
	}
	if (input == "r")
	{
		std::cout << colorToString(playerColor) << " Pawn was promoted to Rook.\n\n";
		return std::make_shared<Rook>(playerColor);
	}
	std::cout << "Invalid input.\n";
	return inputPromotion(playerColor);
}

std::string inputString()
{
    std::string input;
    std::getline(std::cin, input);
	input = stringToLowerCase(input);
	if (input == "end" || input == "exit" || input == "quit" || input == "terminate")
	{
		throw ExitException{};
	}
	if (input == "new game")
	{
		throw NewGameException{};
	}
    return input;
}

bool isCharValidX(const char c)
{
	if (c != 'a' && c != 'b' && c != 'c' && c != 'd' && c != 'e' && c != 'f' && c != 'g' && c != 'h')
	{
		return false;
	}
	return true;
}

bool isCharValidY(const char c)
{
	if (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8')
	{
		return false;
	}
	return true;
}

void printBoard(const Board& board)
{
	std::cout << "\n\n";
	std::cout << "     A   B   C   D   E   F   G   H     \n";
	std::cout << "  -|---|---|---|---|---|---|---|---|-  \n";
	for (int i = 16; i >= 1; --i)
	{
		if (i % 2 == 0)
		{
			int y = i / 2;
			std::cout << " " << y << " |";
			for (int j = 1; j <= 8; j++)
			{
				int x = j;
				std::cout << " ";
				if (board.board[x][y] == nullptr)
				{
					std::cout << " ";
				}
				else
				{
					std::cout << board.board[x][y]->getChar();
				}
				std::cout << " |";
			}
			std::cout << " " << y;
		}
		else
		{
			std::cout << "  -|---|---|---|---|---|---|---|---|-  ";
		}
		std::cout << "\n";
	}
	std::cout << "     A   B   C   D   E   F   G   H     \n";
	std::cout << "\n\n\n";
}

void printInstructions(const Color& playerColor)
{
	if (playerColor == black)
	{
		std::cout << "White pieces (computer):    \"B\" = Bishop, \"K\" = King, \"N\" = Night, \"P\" = Pawn, \"Q\" = Queen, \"R\" = Rook    <- upper case\n";
		std::cout << "Black pieces (player):      \"b\" = Bishop, \"k\" = King, \"n\" = Night, \"p\" = Pawn, \"q\" = Queen, \"r\" = Rook    <- lower case\n\n";
	}
	else
	{
		std::cout << "Black pieces (computer):    \"b\" = Bishop, \"k\" = King, \"n\" = Night, \"p\" = Pawn, \"q\" = Queen, \"r\" = Rook    <- lower case\n";
		std::cout << "White pieces (player):      \"B\" = Bishop, \"K\" = King, \"N\" = Night, \"P\" = Pawn, \"Q\" = Queen, \"R\" = Rook    <- upper case\n\n";
	}
	std::cout << "Declare a move as follows: \"[position of piece to be moved] [position of destination]\"\n";
	std::cout << "Examples: \"e2 e4\", \"b1 c3\"\n\n";
}

void printResult(const Color& playerColor, const Status status)
{
	switch (status)
	{
	case blackWins: std::cout << "Black wins!"; break;
	case whiteWins: std::cout << "White wins!"; break;
	case remis: std::cout << "Remis!"; break;
	}
	if ((playerColor == black && status == blackWins) || (playerColor == white && status == whiteWins))
	{
		std::cout << " Congratulations.";
	}
	std::cout << "\n\n";
}