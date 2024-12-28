#pragma once

#include <memory>
#include "move.h"
#include "piece.h"

struct CastlingInformation
{
	bool kingE1NeverMoved;
	bool kingE8NeverMoved;
	bool rookA1NeverMoved;
	bool rookA8NeverMoved;
	bool rookH1NeverMoved;
	bool rookH8NeverMoved;
};

struct LatestMove
{
	int ax;
	int ay;
	int bx;
	int by;
};

class Board
{
public:
	// variables
	std::shared_ptr<Piece> board[9][9];
	CastlingInformation castlingInformation;
	LatestMove latestMove;
	// functions
	Board(const Color& playerColor);
	void setLatestMove(const Move& latestMove);
};