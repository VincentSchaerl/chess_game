#include <memory>
#include "board.h"
#include "conversions.h"
#include "move.h"
#include "piece.h"

// no explicit copy constructor, because the default copy constructor serves the purpose just fine:
// the board array gets shallow copied, so that the piece objects do not get duplicated (better perfomance),
// but the castlingInformation and latestMove structs get deep copied, so that there is no corruption of the original values

// players pieces always occupy the squares A1 - B8
Board::Board(const Color& playerColor) : castlingInformation{true, true, true, true, true, true}, latestMove{-1, -1, -1, -1}
{
	Color computerColor = oppositeColor(playerColor);
	board[1][8] = std::make_shared<Rook>(computerColor);
	board[2][8] = std::make_shared<Knight>(computerColor);
	board[3][8] = std::make_shared<Bishop>(computerColor);
	board[4][8] = std::make_shared<Queen>(computerColor);
	board[5][8] = std::make_shared<King>(computerColor);
	board[6][8] = std::make_shared<Bishop>(computerColor);
	board[7][8] = std::make_shared<Knight>(computerColor);
	board[8][8] = std::make_shared<Rook>(computerColor);
	for (int i = 1; i <= 8; i++)
	{
		board[i][7] = std::make_shared<Pawn>(computerColor, false);
		board[i][2] = std::make_shared<Pawn>(playerColor, true);
	}
	board[1][1] = std::make_shared<Rook>(playerColor);
	board[2][1] = std::make_shared<Knight>(playerColor);
	board[3][1] = std::make_shared<Bishop>(playerColor);
	board[4][1] = std::make_shared<Queen>(playerColor);
	board[5][1] = std::make_shared<King>(playerColor);
	board[6][1] = std::make_shared<Bishop>(playerColor);
	board[7][1] = std::make_shared<Knight>(playerColor);
	board[8][1] = std::make_shared<Rook>(playerColor);
}

void Board::setLatestMove(const Move& move)
{
	latestMove = {move.ax, move.ay, move.bx, move.by};
}