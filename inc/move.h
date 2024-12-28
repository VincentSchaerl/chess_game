#pragma once

enum MoveType
{
	normal,
	castling,
	enPassant,
	promotion
};

class Move
{
	// variables
	double value;
public:
	const int ax, ay, bx, by;
	const MoveType type;
	// functions
	Move(const int ax, const int ay, const int bx, const int by);
	Move(const int ax, const int ay, const int bx, const int by, MoveType type);
	friend bool operator==(const Move& move1, const Move& move2);
	friend bool operator!=(const Move& move1, const Move& move2);
	double getValue();
	void setValue(const double value);
};