#pragma once

#include <exception>
#include <memory>
#include <string>
#include "board.h"
#include "move.h"
#include "piece.h"
#include "play.h"

class ExitException : public std::exception {};
class NewGameException : public std::exception {};

Color inputColor();
Move inputMove(const Board& board, const Color& playerColor);
std::string inputMoveString();
bool inputPlayAgain();
std::shared_ptr<Piece> inputPromotion(const Color& playerColor);
std::string inputString();
bool isCharValidX(const char c);
bool isCharValidY(const char c);
void printBoard(const Board& board);
void printInstructions(const Color& playerColor);
void printResult(const Color& playerColor, const Status status);