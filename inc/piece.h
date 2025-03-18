#pragma once

#include <vector>
#include "move.h"

class Board;

enum Color
{
	black,
	white
};

class Piece
{
public:
	// variables
	const Color color;
	// functions
	Piece(const Color& color);
	virtual char getChar() = 0;
	virtual void checkMoves(const Board& board, bool checkCastling, std::vector<Move>& moves, const int* const position) = 0;
};

class Bishop : public Piece
{
public:
	// functions
	Bishop(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, std::vector<Move>& moves, const int* const position);
};

class King : public Piece
{
public:
	// functions
	King(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, std::vector<Move>& moves, const int* const position);
};

class Knight : public Piece
{
public:
	// functions
	Knight(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, std::vector<Move>& moves, const int* const position);
};

class Pawn : public Piece
{
	// variables
	bool isPlayer;
public:
	// functions
	Pawn(const Color& color, bool isPlayer);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, std::vector<Move>& moves, const int* const position);
};

class Queen : public Piece
{
public:
	// functions
	Queen(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, std::vector<Move>& moves, const int* const position);
};

class Rook : public Piece
{
public:
	// functions
	Rook(const Color& color);
	char getChar();
	void checkMoves(const Board& board, bool checkCastling, std::vector<Move>& moves, const int* const position);
};