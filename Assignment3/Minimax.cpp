//STL HEADERS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <deque>

//PROJECT HEADERS
#include "GameTree.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::max;
using std::min;
using std::deque;

//GLOBAL VARIABLES
const bool bDebug = false;
bool heuristicPrinted = false;

int searchDepth;

GameNode *startBoard = new GameNode();

//Nature of the position on the checkerboard
enum POSITIONSTATE
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	TOP_ROW,
	BOTTOM_ROW,
	LEFT_COLUMN,
	RIGHT_COLUMN,
	MIDDLE //Not really a special position after all
};

//FUNCTION DECLARATION
bool InputBoard(char* fileName);
bool getForwardMoves(GameNode *gNode, int x, int y);
bool getForwardJumps(GameNode *gNode, int x, int y);
bool getBackwardMoves(GameNode *gNode, int x, int y);
bool getBackwardJumps(GameNode *gNode, int x, int y);
bool getAllJumps(GameNode *gNode, int x, int y);
bool getAllMoves(GameNode *gNode, int x, int y);
POSITIONSTATE checkPosition(int x, int y);
bool checkKingship(int x, int y, const BOARDPOSITION bPosition);
bool anyPiecesLeft(GameNode *gNode);

int gameMaxValue(GameNode *node, int alpha, int beta);
int gameMinValue(GameNode *node, int alpha, int beta);

bool InputBoard(char* fileName)
{
	string currentLine;
	unsigned int currentRow = 0;

	ifstream boardFile(fileName);
	if(boardFile.is_open())
	{
		while(boardFile.good())
		{
			getline(boardFile, currentLine);

			if(currentRow < BOARD_ROW_NUM)
			for(unsigned int i = 0; i < currentLine.size(); ++i)
			{
				switch(currentLine[i])
				{
				case 'X':
					startBoard->gameBoard[i][currentRow] = LIGHT_SQUARE;
					break;
				case '.':
					startBoard->gameBoard[i][currentRow] = EMPTY_DARK_SQUARE;
					break;
				case 'o':
					startBoard->gameBoard[i][currentRow] = A_PIECE;
					break;
				case '*':
					startBoard->gameBoard[i][currentRow] = B_PIECE;
					break;
				case 'k':
					startBoard->gameBoard[i][currentRow] = A_KING;
					break;
				case 'K':
					startBoard->gameBoard[i][currentRow] = B_KING;
					break;
				default: //Unrecognized character, must be an error
					cout << "Unrecognized character \'" << currentLine[i] << "\'. Exiting program..." << endl;
					return false;
					break;
				}
			}
			else
			{
				searchDepth = atoi(currentLine.c_str());
			}
			++currentRow; //Advance to the next row on the board
		}
		boardFile.close();
		startBoard->name = "Start";
		startBoard->depth = 0;
		return true;
	}
	else
	{
		cout << "Cannot open file at \'" << fileName << "\'. Exiting program..." << endl;
		return false;
	}
}

bool getForwardMoves(GameNode *gNode, int x, int y)
{
	bool retBool = false;
	POSITIONSTATE pState = checkPosition(x, y);
	BOARDPOSITION currBoardPosition = gNode->gameBoard[x][y];
	if(gNode->isMaxNode && (currBoardPosition == A_PIECE || currBoardPosition == A_KING))
	{
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == LEFT_COLUMN)
		{
			int newX = x + 1;
			int newY = y - 1;
			if(gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				GameNode *newGameNode = new GameNode(*gNode);
				newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
				if(checkKingship(newX, newY, currBoardPosition))
				{
					newGameNode->gameBoard[newX][newY] = A_KING;
					++(newGameNode->heuristics);
				}
				else
				{
					if(currBoardPosition == A_PIECE) newGameNode->gameBoard[newX][newY] = A_PIECE;
					else newGameNode->gameBoard[newX][newY] = A_KING;
				}
				newGameNode->fromX = x;
				newGameNode->fromY = y;
				newGameNode->toX = newX;
				newGameNode->toY = newY;
				newGameNode->setName();
				gNode->successors.push_back(newGameNode);

				retBool = true;
			}
		}
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = x - 1;
			int newY = y - 1;
			if(gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				GameNode *newGameNode = new GameNode(*gNode);
				newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
				if(checkKingship(newX, newY, currBoardPosition))
				{
					newGameNode->gameBoard[newX][newY] = A_KING;
					++(newGameNode->heuristics);
				}
				else
				{
					if(currBoardPosition == A_PIECE) newGameNode->gameBoard[newX][newY] = A_PIECE;
					else newGameNode->gameBoard[newX][newY] = A_KING;
				}
				newGameNode->fromX = x;
				newGameNode->fromY = y;
				newGameNode->toX = newX;
				newGameNode->toY = newY;
				newGameNode->setName();
				gNode->successors.push_back(newGameNode);

				retBool = true;
			}
		}
	}
	else if(!(gNode->isMaxNode) && (currBoardPosition == B_PIECE || currBoardPosition == B_KING))
	{
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_LEFT || pState == LEFT_COLUMN)
		{
			int newX = x + 1;
			int newY = y + 1;
			if(gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				GameNode *newGameNode = new GameNode(*gNode);
				newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
				if(checkKingship(newX, newY, currBoardPosition))
				{
					newGameNode->gameBoard[newX][newY] = B_KING;
					--(newGameNode->heuristics);
				}
				else
				{
					if(currBoardPosition == B_PIECE) newGameNode->gameBoard[newX][newY] = B_PIECE;
					else newGameNode->gameBoard[newX][newY] = B_KING;
				}
				newGameNode->fromX = x;
				newGameNode->fromY = y;
				newGameNode->toX = newX;
				newGameNode->toY = newY;
				newGameNode->setName();
				gNode->successors.push_back(newGameNode);

				retBool = true;
			}
		}
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = x - 1;
			int newY = y + 1;
			if(gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				GameNode *newGameNode = new GameNode(*gNode);
				newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
				if(checkKingship(newX, newY, currBoardPosition))
				{
					newGameNode->gameBoard[newX][newY] = B_KING;
					--(newGameNode->heuristics);
				}
				else
				{
					if(currBoardPosition == B_PIECE) newGameNode->gameBoard[newX][newY] = B_PIECE;
					else newGameNode->gameBoard[newX][newY] = B_KING;
				}
				newGameNode->fromX = x;
				newGameNode->fromY = y;
				newGameNode->toX = newX;
				newGameNode->toY = newY;
				newGameNode->setName();
				gNode->successors.push_back(newGameNode);

				retBool = true;
			}
		}
	}
	
	return retBool;
}

bool getForwardJumps(GameNode *gNode, int x, int y)
{
	bool retBool = false;
	POSITIONSTATE pState = checkPosition(x, y);
	BOARDPOSITION currBoardPosition = gNode->gameBoard[x][y];
	bool killedKing = false;
	if(gNode->isMaxNode && (currBoardPosition == A_PIECE || currBoardPosition == A_KING))
	{
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == LEFT_COLUMN)
		{
			int newX = x + 1;
			int newY = y - 1;
			if(((killedKing = (gNode->gameBoard[newX][newY] == B_KING)) || gNode->gameBoard[newX][newY] == B_PIECE))
			{
				newX += 1;
				newY -= 1;
				if(newX < BOARD_COL_NUM && newY >= 0 && gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					GameNode *newGameNode = new GameNode(*gNode);
					newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[x + 1][y - 1] = EMPTY_DARK_SQUARE;
					if(checkKingship(newX, newY, currBoardPosition))
					{
						newGameNode->gameBoard[newX][newY] = A_KING;
						++(newGameNode->heuristics);
					}
					else
					{
						if(currBoardPosition == A_PIECE) newGameNode->gameBoard[newX][newY] = A_PIECE;
						else newGameNode->gameBoard[newX][newY] = A_KING;
					}
					if(killedKing) newGameNode->heuristics += 2;
					else ++(newGameNode->heuristics);
					newGameNode->fromX = x;
					newGameNode->fromY = y;
					newGameNode->toX = newX;
					newGameNode->toY = newY;
					newGameNode->setName();
					gNode->successors.push_back(newGameNode);

					retBool = true;
				}
			}
		}
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = x - 1;
			int newY = y - 1;
			if(((killedKing = (gNode->gameBoard[newX][newY] == B_KING)) || gNode->gameBoard[newX][newY] == B_PIECE))
			{
				newX -= 1;
				newY -= 1;
				if(newX >= 0 && newY >= 0 && gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					GameNode *newGameNode = new GameNode(*gNode);
					newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[x - 1][y - 1] = EMPTY_DARK_SQUARE;
					if(checkKingship(newX, newY, currBoardPosition))
					{
						newGameNode->gameBoard[newX][newY] = A_KING;
						++(newGameNode->heuristics);
					}
					else
					{
						if(currBoardPosition == A_PIECE) newGameNode->gameBoard[newX][newY] = A_PIECE;
						else newGameNode->gameBoard[newX][newY] = A_KING;
					}
					if(killedKing) newGameNode->heuristics += 2;
					else ++(newGameNode->heuristics);
					newGameNode->fromX = x;
					newGameNode->fromY = y;
					newGameNode->toX = newX;
					newGameNode->toY = newY;
					newGameNode->setName();
					gNode->successors.push_back(newGameNode);

					retBool = true;
				}
			}
		}
	}
	else if(!(gNode->isMaxNode) && (currBoardPosition == B_PIECE || currBoardPosition == B_KING))
	{
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_LEFT || pState == LEFT_COLUMN)
		{
			int newX = x + 1;
			int newY = y + 1;
			if(((killedKing = (gNode->gameBoard[newX][newY] == A_KING)) || gNode->gameBoard[newX][newY] == A_PIECE))
			{
				newX += 1;
				newY += 1;
				if(newX < BOARD_COL_NUM && newY < BOARD_ROW_NUM && gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					GameNode *newGameNode = new GameNode(*gNode);
					newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[x + 1][y + 1] = EMPTY_DARK_SQUARE;
					if(checkKingship(newX, newY, currBoardPosition))
					{
						newGameNode->gameBoard[newX][newY] = B_KING;
						--(newGameNode->heuristics);
					}
					else
					{
						if(currBoardPosition == B_PIECE) newGameNode->gameBoard[newX][newY] = B_PIECE;
						else newGameNode->gameBoard[newX][newY] = B_KING;
					}
					if(killedKing) newGameNode->heuristics -= 2;
					else --(newGameNode->heuristics);
					newGameNode->fromX = x;
					newGameNode->fromY = y;
					newGameNode->toX = newX;
					newGameNode->toY = newY;
					newGameNode->setName();
					gNode->successors.push_back(newGameNode);

					retBool = true;
				}
			}
		}
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = x - 1;
			int newY = y + 1;
			if(((killedKing = (gNode->gameBoard[newX][newY] == A_KING)) || gNode->gameBoard[newX][newY] == A_PIECE))
			{
				newX -= 1;
				newY += 1;
				if(newX >= 0 && newY < BOARD_ROW_NUM && gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					GameNode *newGameNode = new GameNode(*gNode);
					newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[x - 1][y + 1] = EMPTY_DARK_SQUARE;
					if(checkKingship(newX, newY, currBoardPosition))
					{
						newGameNode->gameBoard[newX][newY] = B_KING;
						--(newGameNode->heuristics);
					}
					else
					{
						if(currBoardPosition == B_PIECE) newGameNode->gameBoard[newX][newY] = B_PIECE;
						else newGameNode->gameBoard[newX][newY] = B_KING;
					}
					if(killedKing) newGameNode->heuristics -= 2;
					else --(newGameNode->heuristics);
					newGameNode->fromX = x;
					newGameNode->fromY = y;
					newGameNode->toX = newX;
					newGameNode->toY = newY;
					newGameNode->setName();
					gNode->successors.push_back(newGameNode);

					retBool = true;
				}
			}
		}
	}

	return retBool;
}

bool getBackwardMoves(GameNode *gNode, int x, int y)
{
	bool retBool = false;
	POSITIONSTATE pState = checkPosition(x, y);
	BOARDPOSITION currBoardPosition = gNode->gameBoard[x][y];
	if(gNode->isMaxNode && currBoardPosition == A_KING)
	{
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_LEFT || pState == LEFT_COLUMN)
		{
			int newX = x + 1;
			int newY = y + 1;
			if(gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				GameNode *newGameNode = new GameNode(*gNode);
				newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
				newGameNode->gameBoard[newX][newY] = A_KING;
				newGameNode->fromX = x;
				newGameNode->fromY = y;
				newGameNode->toX = newX;
				newGameNode->toY = newY;
				newGameNode->setName();
				gNode->successors.push_back(newGameNode);

				retBool = true;
			}
		}
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = x - 1;
			int newY = y + 1;
			if(gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				GameNode *newGameNode = new GameNode(*gNode);
				newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
				newGameNode->gameBoard[newX][newY] = A_KING;
				newGameNode->fromX = x;
				newGameNode->fromY = y;
				newGameNode->toX = newX;
				newGameNode->toY = newY;
				newGameNode->setName();
				gNode->successors.push_back(newGameNode);

				retBool = true;
			}
		}
	}
	else if(!(gNode->isMaxNode) && currBoardPosition == B_KING)
	{
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == LEFT_COLUMN)
		{
			int newX = x + 1;
			int newY = y - 1;
			if(gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				GameNode *newGameNode = new GameNode(*gNode);
				newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
				newGameNode->gameBoard[newX][newY] = B_KING;
				newGameNode->fromX = x;
				newGameNode->fromY = y;
				newGameNode->toX = newX;
				newGameNode->toY = newY;
				newGameNode->setName();
				gNode->successors.push_back(newGameNode);

				retBool = true;
			}
		}
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = x - 1;
			int newY = y - 1;
			if(gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				GameNode *newGameNode = new GameNode(*gNode);
				newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
				newGameNode->gameBoard[newX][newY] = B_KING;
				newGameNode->fromX = x;
				newGameNode->fromY = y;
				newGameNode->toX = newX;
				newGameNode->toY = newY;
				newGameNode->setName();
				gNode->successors.push_back(newGameNode);

				retBool = true;
			}
		}
	}

	return retBool;
}

bool getBackwardJumps(GameNode *gNode, int x, int y)
{
	bool retBool = false;
	POSITIONSTATE pState = checkPosition(x, y);
	BOARDPOSITION currBoardPosition = gNode->gameBoard[x][y];
	bool killedKing = false;
	if(gNode->isMaxNode && currBoardPosition == A_KING)
	{
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_LEFT || pState == LEFT_COLUMN)
		{
			int newX = x + 1;
			int newY = y + 1;
			if(((killedKing = (gNode->gameBoard[newX][newY] == B_KING)) || gNode->gameBoard[newX][newY] == B_PIECE))
			{
				newX += 1;
				newY += 1;
				if(newX < BOARD_COL_NUM && newY < BOARD_ROW_NUM && gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					GameNode *newGameNode = new GameNode(*gNode);
					newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[x + 1][y + 1] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[newX][newY] = A_KING;
					if(killedKing) newGameNode->heuristics += 2;
					else ++(newGameNode->heuristics);
					newGameNode->fromX = x;
					newGameNode->fromY = y;
					newGameNode->toX = newX;
					newGameNode->toY = newY;
					newGameNode->setName();
					gNode->successors.push_back(newGameNode);

					retBool = true;
				}
			}
		}
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = x - 1;
			int newY = y + 1;
			if(((killedKing = (gNode->gameBoard[newX][newY] == B_KING)) || gNode->gameBoard[newX][newY] == B_PIECE))
			{
				newX -= 1;
				newY += 1;
				if(newX >= 0 && newY < BOARD_ROW_NUM && gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					GameNode *newGameNode = new GameNode(*gNode);
					newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[x - 1][y + 1] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[newX][newY] = A_KING;
					if(killedKing) newGameNode->heuristics += 2;
					else ++(newGameNode->heuristics);
					newGameNode->fromX = x;
					newGameNode->fromY = y;
					newGameNode->toX = newX;
					newGameNode->toY = newY;
					newGameNode->setName();
					gNode->successors.push_back(newGameNode);

					retBool = true;
				}
			}
		}
	}
	else if(!(gNode->isMaxNode) && currBoardPosition == B_KING)
	{
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == LEFT_COLUMN)
		{
			int newX = x + 1;
			int newY = y - 1;
			if(((killedKing = (gNode->gameBoard[newX][newY] == A_KING)) || gNode->gameBoard[newX][newY] == A_PIECE))
			{
				newX += 1;
				newY -= 1;
				if(newX < BOARD_COL_NUM && newY >= 0 && gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					GameNode *newGameNode = new GameNode(*gNode);
					newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[x + 1][y - 1] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[newX][newY] = B_KING;
					if(killedKing) newGameNode->heuristics -= 2;
					else --(newGameNode->heuristics);
					newGameNode->fromX = x;
					newGameNode->fromY = y;
					newGameNode->toX = newX;
					newGameNode->toY = newY;
					newGameNode->setName();
					gNode->successors.push_back(newGameNode);

					retBool = true;
				}
			}
		}
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = x - 1;
			int newY = y - 1;
			if(((killedKing = (gNode->gameBoard[newX][newY] == A_KING)) || gNode->gameBoard[newX][newY] == A_PIECE))
			{
				newX -= 1;
				newY -= 1;
				if(newX >= 0 && newY >= 0 && gNode->gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					GameNode *newGameNode = new GameNode(*gNode);
					newGameNode->gameBoard[x][y] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[x - 1][y - 1] = EMPTY_DARK_SQUARE;
					newGameNode->gameBoard[newX][newY] = B_KING;
					if(killedKing) newGameNode->heuristics -= 2;
					else --(newGameNode->heuristics);
					newGameNode->fromX = x;
					newGameNode->fromY = y;
					newGameNode->toX = newX;
					newGameNode->toY = newY;
					newGameNode->setName();
					gNode->successors.push_back(newGameNode);

					retBool = true;
				}
			}
		}
	}

	return retBool;
}

POSITIONSTATE checkPosition(int x, int y)
{
	if(x == 0)
	{
		if(y == 0) return TOP_LEFT;
		else if(y == BOARD_ROW_NUM - 1) return BOTTOM_LEFT;
		else return LEFT_COLUMN;
	}
	else if(x == BOARD_COL_NUM - 1)
	{
		if(y == 0) return TOP_RIGHT;
		else if(y == BOARD_ROW_NUM - 1) return BOTTOM_RIGHT;
		else return RIGHT_COLUMN;
	}
	else if(y == 0)
	{
		return TOP_ROW;
	}
	else if(y == BOARD_ROW_NUM - 1)
	{
		return BOTTOM_ROW;
	}
	else return MIDDLE;
}

bool checkKingship(int x, int y, const BOARDPOSITION bPosition)
{
	POSITIONSTATE pState = checkPosition(x, y);

	if(bPosition == A_PIECE && (pState == TOP_ROW || pState == TOP_LEFT || pState == TOP_RIGHT))
	{
		return true;
	}

	if(bPosition == B_PIECE && (pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == BOTTOM_RIGHT))
	{
		return true;
	}

	return false;
}

bool anyPiecesLeft(GameNode *gNode)
{
	BOARDPOSITION playerPiece, kingPiece;
	if(gNode->isMaxNode)
	{
		playerPiece = A_PIECE;
		kingPiece = A_KING;
	}
	else
	{
		playerPiece = B_PIECE;
		kingPiece = B_KING;
	}

	for(int i = 0; i < BOARD_ROW_NUM; ++i)
	{
		for(int j = 0; j < BOARD_COL_NUM; ++j)
		{
			if(gNode->gameBoard[i][j] == playerPiece || gNode->gameBoard[i][j] == kingPiece)
			{
				return true;
			}
		}
	}
	return false;
}

int gameMaxValue(GameNode *node, int alpha, int beta)
{
	if(node->successors.empty())
	{
		cout << "Heuristic value of the current board = " << node->heuristics << "." << endl;
		heuristicPrinted = true;
		return node->heuristics;
	}

	node->alpha = alpha;
	node->beta = beta;
	if(node->depth != 0)
	{
		cout << node->name << endl;
	}
	node->printDepth();
	for(int i = 0; i < node->successors.size(); ++i)
	{
		node->alpha = max(node->alpha, gameMinValue(node->successors[i], node->alpha, node->beta));
		if(node->printDepth(-1)) cout << node->name << endl;
		if(i + 1 < node->successors.size()) node->printDepth();
		if(node->alpha >= node->beta)
		{
			int nextIndex = i + 1;
			if(node->successors.size() - nextIndex <= 0) node->printDepth();
			cout << "Pruning Player A's moves: ";
			if(node->successors.size() - nextIndex > 0)
			{
				node->successors[nextIndex]->printPruningInfo();
				for(int j = nextIndex + 1; j < node->successors.size(); ++j)
				{
					cout << ", ";
					node->successors[j]->printPruningInfo();
				}
				cout << "; ";

				//Delete the nodes
				for(int j = nextIndex; j < node->successors.size(); ++j)
				{
					node->successors[j]->deleteNode();
				}
			}
			else
			{
				cout << "<no children> ";
			}
			cout << "Alpha = " << node->alpha << "; Beta = " << node->beta << ".\n" << endl;
			heuristicPrinted = false;
			return node->beta;
		}
	}
	heuristicPrinted = false;
	return node->alpha;
}

int gameMinValue(GameNode *node, int alpha, int beta)
{
	if(node->successors.empty())
	{
		cout << "Heuristic value of the current board = " << node->heuristics << "." << endl;
		heuristicPrinted = true;
		return node->heuristics;
	}

	node->alpha = alpha;
	node->beta = beta;
	if(node->depth != 0)
	{
		cout << node->name << endl;
	}
	node->printDepth();
	for(int i = 0; i < node->successors.size(); ++i)
	{
		node->beta = min(node->beta, gameMaxValue(node->successors[i], node->alpha, node->beta));
		if(node->printDepth(-1)) cout << node->name << endl;
		if(i + 1 < node->successors.size()) node->printDepth();
		if(node->beta <= node->alpha)
		{
			int nextIndex = i + 1;
			if(node->successors.size() - nextIndex <= 0) node->printDepth();
			cout << "Pruning Player B's moves: ";
			if(node->successors.size() - nextIndex > 0)
			{
				node->successors[nextIndex]->printPruningInfo();
				for(int j = nextIndex + 1; j < node->successors.size(); ++j)
				{
					cout << ", ";
					node->successors[j]->printPruningInfo();
				}
				cout << "; ";

				//Delete the nodes
				for(int j = nextIndex; j < node->successors.size(); ++j)
				{
					node->successors[j]->deleteNode();
				}
			}
			else
			{
				cout << "<no children> ";
			}
			cout << "Alpha = " << node->alpha << "; Beta = " << node->beta << ".\n" << endl;
			heuristicPrinted = false;
			return node->alpha;
		}
	}
	heuristicPrinted = false;
	return node->beta;
}

int main(int argc, char **argv)
{
	if(!InputBoard("Test.txt"))
	{
		return 1; //Exit due to error
	}

	if(bDebug)
	{
		startBoard->printGameBoard();
		cout << endl;
	}

	/*
	GameNode a0(true);
	a0.name = "a0";
	GameTree checkerTree(&a0);
	
	GameNode b0(false);
	b0.name = "b0";
	a0.successors.push_back(&b0);
	GameNode b1(false);
	b1.name = "b1";
	a0.successors.push_back(&b1);

	GameNode c0(true);
	c0.name = "c0";
	b0.successors.push_back(&c0);
	GameNode c1(true);
	c1.name = "c1";
	b0.successors.push_back(&c1);
	GameNode c2(true);
	c2.name = "c2";
	b1.successors.push_back(&c2);
	GameNode c3(true);
	c3.name = "c3";
	b1.successors.push_back(&c3);

	GameNode d0(false);
	d0.name = "d0";
	c0.successors.push_back(&d0);
	GameNode d1(false);
	d1.name = "d1";
	c0.successors.push_back(&d1);
	GameNode d2(false);
	d2.name = "d2";
	c1.successors.push_back(&d2);
	GameNode d3(false);
	d3.name = "d3";
	c1.successors.push_back(&d3);
	GameNode d4(false);
	d4.name = "d4";
	c2.successors.push_back(&d4);
	GameNode d5(false);
	d5.name = "d5";
	c2.successors.push_back(&d5);
	GameNode d6(false);
	d6.name = "d6";
	c3.successors.push_back(&d6);
	GameNode d7(false);
	d7.name = "d7";
	c3.successors.push_back(&d7);

	GameNode e0(0, true);
	e0.name = "e0";
	d0.successors.push_back(&e0);
	GameNode e1(5, true);
	e1.name = "e1";
	d0.successors.push_back(&e1);
	GameNode e2(-3, true);
	e2.name = "e2";
	d1.successors.push_back(&e2);
	GameNode e3(3, true);
	e3.name = "e3";
	d1.successors.push_back(&e3);
	GameNode e4(3, true);
	e4.name = "e4";
	d2.successors.push_back(&e4);
	GameNode e5(4, true);
	e5.name = "e5";
	d2.successors.push_back(&e5);
	GameNode e6(-2, true);
	e6.name = "e6";
	d3.successors.push_back(&e6);
	GameNode e7(3, true);
	e7.name = "e7";
	d3.successors.push_back(&e7);
	GameNode e8(5, true);
	e8.name = "e8";
	d4.successors.push_back(&e8);
	GameNode e9(1, true);
	e9.name = "e9";
	d4.successors.push_back(&e9);
	GameNode ea(-3, true);
	ea.name = "ea";
	d5.successors.push_back(&ea);
	GameNode eb(0, true);
	eb.name = "eb";
	d5.successors.push_back(&eb);
	GameNode ec(6, true);
	ec.name = "ec";
	d6.successors.push_back(&ec);
	GameNode ed(8, true);
	ed.name = "ed";
	d6.successors.push_back(&ed);
	GameNode ee(9, true);
	ee.name = "ee";
	d7.successors.push_back(&ee);
	GameNode ef(-3, true);
	ef.name = "ef";
	d7.successors.push_back(&ef);

	gameMaxValue(&a0, INT_MIN, INT_MAX);
	*/

	//Prepare the Minimax game tree first
	bool playerFlag = true; //Player A / Max starts first
	BOARDPOSITION playerPiece, kingPiece;

	deque<GameNode*> nodeList; //Used to perform BFS on the minimax game tree
	nodeList.push_back(startBoard); //Start off using the starting checkerboard
	GameNode *currentNode;

	int currentDepth = 1;

	//While we have not reached the depth we want and we still have something left in the node list
	while(currentDepth <= searchDepth && nodeList.size() != 0)
	{
		if(bDebug) cout << "============================" << endl;

		//Grab the first node in the queue
		currentNode = nodeList.front();
		nodeList.pop_front();

		//If the current node is ever different than the player flag then we've moved down a depth
		if(currentNode->isMaxNode ^ playerFlag)
		{
			++currentDepth;
			playerFlag = !playerFlag;
		}

		//If there are no pieces left for the current player then this is a victory for the other player
		if(!anyPiecesLeft(currentNode))
		{
			if(currentNode->isMaxNode) currentNode->heuristics = INT_MIN;
			else currentNode->heuristics = INT_MAX;
			continue; //This node won't have any successors so just move on to the next
		}

		//Use the flag to determine which player to consider for this turn
		if(currentNode->isMaxNode)
		{
			playerPiece = A_PIECE;
			kingPiece = A_KING;
		}
		else
		{
			playerPiece = B_PIECE;
			kingPiece = B_KING;
		}

		//First check if there are jumps first
		for(int i = 0; i < BOARD_ROW_NUM; ++i)
		{
			for(int j = 0; j < BOARD_COL_NUM; ++j)
			{
				getForwardJumps(currentNode, j, i);
				getBackwardJumps(currentNode, j, i);
			}
		}

		//If there are no jumps then check for moves
		if(currentNode->successors.size() == 0)
		{
			//Loop through the board
			for(int i = 0; i < BOARD_ROW_NUM; ++i)
			{
				for(int j = 0; j < BOARD_COL_NUM; ++j)
				{
					getForwardMoves(currentNode, j, i);
					getBackwardMoves(currentNode, j, i);
				}
			}

			//If there are still no moves after doing everything then this is a victory for the other player
			if(currentNode->successors.size() == 0)
			{
				if(currentNode->isMaxNode) currentNode->heuristics = INT_MIN;
				else currentNode->heuristics = INT_MAX;
				continue; //This node won't have any successors so just move on to the next
			}
		}

		//Push back all the successors to the back of the queue
		for(int k = 0; k < currentNode->successors.size(); ++k)
		{
			nodeList.push_back(currentNode->successors[k]);
			if(bDebug)
			{
				currentNode->successors[k]->printGameBoard();
				cout << endl;
			}
		}

		if(bDebug)
		{
			cout << "depth: " << currentDepth << " ============================" << endl;
			if(currentNode->successors.size() != 0)
			{
				if(currentNode->successors[0]->isMaxNode) cout << "Max Node" << endl;
				else cout << "Min Node" << endl;
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	//Run Minimax with alphabeta pruning
	gameMaxValue(startBoard, INT_MIN, INT_MAX);
	/////////////////////////////////////////////////////////////////////////////////////////

	cout << "\nExpansions Complete.\n" << endl;
	//Loop through startBoard's successors until startBoard's alpha == successor's beta
	GameNode *chosenSuccessor;
	for(int i = 0; i < startBoard->successors.size(); ++i)
	{
		if(startBoard->alpha == startBoard->successors[i]->beta)
		{
			chosenSuccessor = startBoard->successors[i];
			break;
		}
	}
	
	cout << "\nAnswer: " << chosenSuccessor->name << endl;

	getchar();
	return 0;
}