#pragma once

#include <string>
#include "board.h"
#include "move.h"
#include "piece.h"

std::string charToClassName(const char c);
std::string colorToString(const Color& color);
char digitToLetter(const int i);
int letterToDigit(const char c);
Color oppositeColor(const Color& color);
std::string stringToLowerCase(std::string s);
Move stringToMove(const Board& board, const std::string& move);