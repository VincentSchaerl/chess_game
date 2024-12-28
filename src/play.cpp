#include <cctype>
#include <functional>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>
#include "board.h"
#include "conversions.h"
#include "input_output.h"
#include "move.h"
#include "piece.h"
#include "play.h"

#define RANGE_OF_MOVE_VALUES 0.08
// iterations = how many moves in advance get calculated
#define ITERATIONS 3
// the highest possible value of a move is if the resulting position includes:
// 2 bishops + 2 knights + 2 rooks + 9 queens + no enemy piece on board except the king =
// = 2*3 + 2*3 + 2*5 + 9*9 = 103
#define HIGHEST_POSSIBLE_MOVE_VALUE 103
// vice versa for the lowest possible value
#define LOWEST_POSSIBLE_MOVE_VALUE -103
// according to Wikipedia, this is the most common assignment of point values:
#define PAWN_VALUE 1
#define BISHOP_VALUE 3
#define KNIGHT_VALUE 3
#define ROOK_VALUE 5
#define QUEEN_VALUE 9

void play(const Color& playerColor)
{
	printInstructions(playerColor);
	Board board(playerColor);
	printBoard(board);
	Status status;
	while (true)
	{
		status = checkStatus(board, white);
		if (status != gameGoesOn)
		{
			break;
		}
		moveWhite(board, playerColor);
		printBoard(board);
		status = checkStatus(board, black);
		if (status != gameGoesOn)
		{
			break;
		}
		moveBlack(board, playerColor);
		printBoard(board);
	}
	printResult(playerColor, status);
}

// returns all possible and legal moves of the moving color
std::vector<Move> checkMoves(const Board& board, const Color& movingColor)
{
	std::vector<Move> tempMoves;
	// adds all "physically" possible moves to tempMoves
	for (int i = 1; i <= 8; i++)
	{
		for (int j = 1; j <= 8; j++)
		{
			if (board.board[i][j] != nullptr && board.board[i][j]->color == movingColor)
			{
				const int position[2] = { i, j };
				board.board[i][j]->checkMoves(board, true, tempMoves, position);
			}
		}
	}
	std::vector<Move> moves;
	// checks which of these moves are legal in terms of check and adds them to moves
	for (Move& tempMove: tempMoves)
	{
		Board tempBoard = board;
		movePiece(tempBoard, true, tempMove);
		if (!isCheck(tempBoard, movingColor))
		{
			moves.push_back(tempMove);
		}
	}
	return moves;
}

// movingColor is the color that makes the next move
Status checkStatus(const Board& board, const Color& movingColor)
{
	if (movingColor == white && isCheckmateOrStalemate(board, white) == 1)
	{
		return blackWins;
	}
	if (movingColor == black && isCheckmateOrStalemate(board, black) == 1)
	{
		return whiteWins;
	}
	if (isCheckmateOrStalemate(board, movingColor) == 2)
	{
		return remis;
	}
	return gameGoesOn;
}

// if the highest value of all moves == HIGHEST_POSSIBLE_VALUE, i.e. it checkmates the player, this move gets chosen
// otherwise all moves with a value greater than or equal the highest value - RANGE_OF_MOVE_VALUES get added to bestMoves and one of them gets randomly chosen
Move chooseMove(std::vector<Move>& moves)
{
	double highestValue = LOWEST_POSSIBLE_MOVE_VALUE;
	for (Move& move: moves)
	{
		if (move.getValue() >= highestValue)
		{
			highestValue = move.getValue();
		}
	}
	double rangeOfMoveValues = 0;
	if (highestValue != HIGHEST_POSSIBLE_MOVE_VALUE)
	{
		rangeOfMoveValues = RANGE_OF_MOVE_VALUES;
	}
	std::vector<Move> bestMoves;
	for (Move& move: moves)
	{
		if (move.getValue() >= highestValue - rangeOfMoveValues)
		{
			bestMoves.push_back(move);
		}
	}
	return bestMoves[rand() % bestMoves.size()];
}

// checks if the king with the color "color" is in check
bool isCheck(const Board& board, const Color& color)
{
	std::vector<Move> enemyMoves;
	// adds all "physically" possible moves of the enemy color to enemyMoves
	for (int i = 1; i <= 8; i++)
	{
		for (int j = 1; j <= 8; j++)
		{
			if (board.board[i][j] != nullptr && board.board[i][j]->color != color)
			{
				const int position[2] = { i, j };
				// checkCastling = false, because otherwise checkMoves and isCheck could call each other endlessly
				board.board[i][j]->checkMoves(board, false, enemyMoves, position);
			}
		}
	}
	// checks if one of these moves would capture the king
	for (Move& enemyMove: enemyMoves)
	{
		if (board.board[enemyMove.bx][enemyMove.by] != nullptr && typeid(*(board.board[enemyMove.bx][enemyMove.by])) == typeid(King))
		{
			return true;
		}
	}
	return false;
}

// 0 = neither checkmate nor stalemate, 1 = checkmate, 2 = stalemate
// movingColor is the color that makes the next move, i.e. the color of the king to be checked if it is checkmated or stalemated
int isCheckmateOrStalemate(const Board& board, const Color& movingColor)
{
	// moves = all possible and legal moves
	std::vector<Move> moves = checkMoves(board, movingColor);
	if (moves.size() == 0)
	{
		if (isCheck(board, movingColor))
		{
			return 1;
		}
		return 2;
	}
	return 0;
}

void moveBlack(Board& board, const Color& playerColor)
{
	if (playerColor == black)
	{
		movePlayer(board, playerColor);
	}
	else
	{
		moveComputer(board, playerColor);
	}
}

void moveComputer(Board& board, const Color& playerColor)
{
	const Color computerColor = oppositeColor(playerColor);
	std::cout << "Computer calculates...\n\n";
	std::vector<Move> moves = checkMoves(board, computerColor);
	scoreMoves(board, ITERATIONS, moves, computerColor, playerColor, true);
	Move move = chooseMove(moves);
	const int ax = move.ax;
	const int ay = move.ay;
	const int bx = move.bx;
	const int by = move.by;
	std::cout << "Computer: " << colorToString(computerColor) << " " << charToClassName(board.board[ax][ay]->getChar()) << " from " << digitToLetter(ax)
		<< ay << " to " << digitToLetter(bx) << by;
	// castling
	if (move.type == castling)
	{
		std::cout << " (castling)";
	}
	// en passant
	else if (move.type == enPassant)
	{
		std::cout << " (en passant)";
	}
	// capture
	if (board.board[bx][by] != nullptr && board.board[bx][by]->color == playerColor)
	{
		std::cout << ", capturing " << stringToLowerCase(colorToString(playerColor)) << " " << charToClassName(board.board[bx][by]->getChar());
	}
	// promotion
	if (board.board[ax][ay] != nullptr && typeid(*(board.board[ax][ay])) == typeid(Pawn) && by == 1)
	{
		std::cout << "\n" << colorToString(computerColor) << " Pawn was promoted to Queen.";
	}
	std::cout << "\n\n";
	movePiece(board, false, move);
}

void movePiece(Board& board, bool isTemp, const Move& move)
{
	const int ax = move.ax;
	const int ay = move.ay;
	const int bx = move.bx;
	const int by = move.by;
	// normal move
	if (move.type == normal)
	{
		board.board[bx][by] = std::move(board.board[ax][ay]);
	}
	// castling
	else if (move.type == castling)
	{
		board.board[bx][by] = std::move(board.board[ax][ay]);
		// rook A
		if (bx == 3)
		{
			movePiece(board, isTemp, Move{1, by, 4, by});
		}
		// rook H
		else
		{
			movePiece(board, isTemp, Move{8, by, 6, by});
		}
	}
	// en passant
	else if (move.type == enPassant)
	{
		board.board[bx][by] = std::move(board.board[ax][ay]);
		board.board[bx][ay].reset();
	}
	// promotion
	else
	{
		Color color = board.board[ax][ay]->color;
		board.board[ax][ay].reset();
		// temporary
		if (isTemp)
		{
			board.board[bx][by] = std::make_shared<Queen>(color);
		}
		// not temporary
		else
		{
			// computer
			if (by == 1)
			{
				board.board[bx][by] = std::make_shared<Queen>(color);
			}
			// player
			else
			{
				board.board[bx][by] = inputPromotion(color);
			}
		}
	}
	// castling information
	if (board.castlingInformation.kingE8NeverMoved == true && ax == 5 && ay == 8)
	{
		board.castlingInformation.kingE8NeverMoved = false;
	}
	else if (board.castlingInformation.rookA8NeverMoved == true && ax == 1 && ay == 8)
	{
		board.castlingInformation.rookA8NeverMoved = false;
	}
	else if (board.castlingInformation.rookH8NeverMoved == true && ax == 8 && ay == 8)
	{
		board.castlingInformation.rookH8NeverMoved = false;
	}
	else if (board.castlingInformation.kingE1NeverMoved == true && ax == 5 && ay == 1)
	{
		board.castlingInformation.kingE1NeverMoved = false;
	}
	else if (board.castlingInformation.rookA1NeverMoved == true && ax == 1 && ay == 1)
	{
		board.castlingInformation.rookA1NeverMoved = false;
	}
	else if (board.castlingInformation.rookH1NeverMoved == true && ax == 8 && ay == 1)
	{
		board.castlingInformation.rookH1NeverMoved = false;
	}
	// latestMove
	board.setLatestMove(move);
}

void movePlayer(Board& board, const Color& playerColor)
{
	Move move = inputMove(board, playerColor);
	const int ax = move.ax;
	const int ay = move.ay;
	const int bx = move.bx;
	const int by = move.by;
	std::cout << "Player: " << colorToString(playerColor) << " " << charToClassName(board.board[ax][ay]->getChar()) << " from " << digitToLetter(ax)
		<< ay << " to " << digitToLetter(bx) << by;
	// castling
	if (move.type == castling)
	{
		std::cout << " (castling)";
	}
	// en passant
	else if (move.type == enPassant)
	{
		std::cout << " (en passant)";
	}
	// capture
	if (board.board[bx][by] != nullptr && board.board[bx][by]->color == oppositeColor(playerColor))
	{
		std::cout << ", capturing " << stringToLowerCase(colorToString(oppositeColor(playerColor))) << " " << charToClassName(board.board[bx][by]->getChar());
	}
	std::cout << "\n\n";
	// promotion: movePiece -> inputPromotion
	movePiece(board, false, move);
}

void moveWhite(Board& board, const Color& playerColor)
{
	if (playerColor == black)
	{
		moveComputer(board, playerColor);
	}
	else
	{
		movePlayer(board, playerColor);
	}
}

// scores the board in favor of the computer: pieces of the player deduct points, pieces of the computer add points
int scoreBoard(const Board& board, const Color& playerColor)
{
	int points = 0;
	for (int i = 1; i <= 8; i++)
	{
		for (int j = 1; j <= 8; j++)
		{
			if (board.board[i][j] != nullptr)
			{
				char c = std::tolower(board.board[i][j]->getChar());
				if (board.board[i][j]->color == playerColor)
				{
					switch (c)
					{
					case 'p': points -= PAWN_VALUE; break;
					case 'b': points -= BISHOP_VALUE; break;
					case 'n': points -= KNIGHT_VALUE; break;
					case 'r': points -= ROOK_VALUE; break;
					case 'q': points -= QUEEN_VALUE; break;
					}
				}
				else
				{
					switch (c)
					{
					case 'p': points += PAWN_VALUE; break;
					case 'b': points += BISHOP_VALUE; break;
					case 'n': points += KNIGHT_VALUE; break;
					case 'r': points += ROOK_VALUE; break;
					case 'q': points += QUEEN_VALUE; break;
					}
				}
			}
		}
	}
	return points;
}

// assigns a value to move
// movingColor is the color that makes the next move (one of vector moves)
void scoreMove(const Board& board, const int iterations, Move& move, const Color& movingColor, const Color& playerColor)
{
		Board tempBoard = board;
		movePiece(tempBoard, true, move);
		int tempStatus = checkStatus(tempBoard, oppositeColor(movingColor));
		// game goes on:
		if (tempStatus == 0)
		{
			if (iterations == 1)
			{
				move.setValue(scoreBoard(tempBoard, playerColor));
			}
			else
			{
				std::vector<Move> tempMoves = checkMoves(tempBoard, oppositeColor(movingColor));
				scoreMoves(tempBoard, iterations - 1, tempMoves, oppositeColor(movingColor), playerColor, false);
				double points = 0;
				for (Move& tempMove: tempMoves)
				{
					points += tempMove.getValue();
				}
				double value = points / tempMoves.size();
				move.setValue(value);
			}
		}
		// computer wins:
		else if ((playerColor == black && tempStatus == 2) || (playerColor == white && tempStatus == 1))
		{
			move.setValue(HIGHEST_POSSIBLE_MOVE_VALUE);
		}
		// player wins:
		else if ((playerColor == black && tempStatus == 1) || (playerColor == white && tempStatus == 2))
		{
			move.setValue(LOWEST_POSSIBLE_MOVE_VALUE);
		}
		// remis:
		else
		{
			move.setValue(0);
		}
}

// calls scoreMove() for every move in moves
// movingColor is the color that makes the next move (one of vector moves)
void scoreMoves(const Board& board, const int iterations, std::vector<Move>& moves, const Color& movingColor, const Color& playerColor, bool useMultithreading)
{
	if (useMultithreading)
    {
        std::vector<std::thread> threads;
        for (Move& move : moves)
        {
            threads.emplace_back
			(
				[&, move = std::ref(move)]()
				{
                	scoreMove(board, iterations, move, movingColor, playerColor);
				}
			);
        }
        for (std::thread& t : threads)
        {
            if (t.joinable())
            {
				t.join();
            }
        }
    }
	else
	{
		for (Move& move : moves)
		{
			scoreMove(board, iterations, move, movingColor, playerColor);
		}
	}
}