#include "move.h"

//	ax = x-coordinate of original position		ay == y-coordinate of original position,
//	bx = x-coordinate of destination			by = y-coordinate of destination

Move::Move(const int ax, const int ay, const int bx, const int by) : ax{ax}, ay{ay}, bx{bx}, by{by}, type{normal}, value{0} {}

Move::Move(const int ax, const int ay, const int bx, const int by, const MoveType type) : ax{ax}, ay{ay}, bx{bx}, by{by}, type{type}, value{0} {}

bool operator==(const Move& move1, const Move& move2)
{
	return (move1.ax == move2.ax && move1.ay == move2.ay && move1.bx == move2.bx && move1.by == move2.by);
}

bool operator!=(const Move& move1, const Move& move2)
{
	return (move1.ax != move2.ax || move1.ay != move2.ay || move1.bx != move2.bx || move1.by != move2.by);
}

double Move::getValue()
{
	return value;
}

void Move::setValue(const double value)
{
	this->value = value;
}