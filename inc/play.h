#pragma once

#include <vector>
#include "board.h"
#include "move.h"
#include "piece.h"

enum Status
{
    gameGoesOn,
    blackWins,
    whiteWins,
    remis
};

std::vector<Move> checkMoves(const Board& board, const Color& movingColor);
Status checkStatus(const Board& board, const Color& movingColor);
Move chooseMove(std::vector<Move>& moves);
bool isCheck(const Board& board, const Color& color);
int isCheckmateOrStalemate(const Board& board, const Color& movingColor);
void moveBlack(Board& board, const Color& playerColor);
void moveComputer(Board& board, const Color& playerColor);
void movePiece(Board& board, bool isTemp, const Move& move);
void movePlayer(Board& board, const Color& playerColor);
void moveWhite(Board& board, const Color& playerColor);
void play(const Color& playerColor);
int scoreBoard(const Board& board, const Color& playerColor);
void scoreMove(const Board& board, const int iterations, Move& move, const Color& movingColor, const Color& playerColor);
void scoreMoves(const Board& board, const int iterations, std::vector<Move>& moves, const Color& movingColor, const Color& playerColor, bool useMultithreading);