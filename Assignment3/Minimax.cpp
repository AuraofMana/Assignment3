//STL HEADERS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>

//PROJECT HEADERS
#include "CheckerPiece.h"
#include "GameTree.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::max;
using std::min;

//GLOBAL VARIABLES
int searchDepth;

BOARDPOSITION gameBoard[BOARD_ROW_NUM][BOARD_COL_NUM]; //Defines the checkerboard

vector<CheckerPiece> APieces; //Pieces owned by A
vector<CheckerPiece> BPieces; //Pieces owned by B

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

class GameMove
{
public:
	Vector2 from;
	Vector2 to;
	bool AMoved;
	bool isJump;
	bool killedKing; //If isJump is true denotes whether or not a king was killed
	bool upgrade; //If the move caused an upgrade to king
	int hValue; //Heuristic value

	GameMove():isJump(false)
	{
		from.x = to.x = BOARD_COL_NUM;
		from.y = to.y = BOARD_ROW_NUM;
		AMoved = true;
		killedKing = false;
		upgrade = false;
		hValue = 0;
	}

	GameMove(int p_X, int p_Y, bool p_AMoved, bool p_isJump):
	AMoved(p_AMoved), isJump(p_isJump)
	{
		from.x = p_X;
		from.y = p_Y;
		to.x = BOARD_COL_NUM;
		to.y = BOARD_ROW_NUM;
		killedKing = false;
		upgrade = false;
		hValue = 0;
	}

	GameMove(int p_X, int p_Y, int t_X, int t_Y, bool p_AMoved, bool p_isJump):
	AMoved(p_AMoved), isJump(p_isJump)
	{
		from.x = p_X;
		from.y = p_Y;
		to.x = t_X;
		to.y = t_Y;
		killedKing = false;
		upgrade = false;
		hValue = 0;
	};

	~GameMove(){};
};

//FUNCTION DECLARATION
bool InputBoard(char* fileName);
void getForwardMoves(CheckerPiece p, vector<GameMove>& moves);
void getForwardJumps(CheckerPiece p, vector<GameMove>& moves);
void getBackwardMoves(CheckerPiece p, vector<GameMove>& moves);
void getBackwardJumps(CheckerPiece p, vector<GameMove>& moves);
POSITIONSTATE checkPosition(Vector2 pos);
bool checkKingship(int x, int y, BOARDPOSITION bPosition);
void kingship();
int currentHeuristics();
void updateHeuristics(vector<GameMove>& moves);

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
					gameBoard[currentRow][i] = LIGHT_SQUARE;
					break;
				case '.':
					gameBoard[currentRow][i] = EMPTY_DARK_SQUARE;
					break;
				case 'o':
					gameBoard[currentRow][i] = A_PIECE;
					APieces.push_back(CheckerPiece(A_PIECE, currentRow, i));
					break;
				case '*':
					gameBoard[currentRow][i] = B_PIECE;
					APieces.push_back(CheckerPiece(B_PIECE, currentRow, i));
					break;
				case 'k':
					gameBoard[currentRow][i] = A_KING;
					APieces.push_back(CheckerPiece(A_KING, currentRow, i));
					break;
				case 'K':
					gameBoard[currentRow][i] = B_KING;
					APieces.push_back(CheckerPiece(B_KING, currentRow, i));
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
		return true;
	}
	else
	{
		cout << "Cannot open file at \'" << fileName << "\'. Exiting program..." << endl;
		return false;
	}
}

void getForwardMoves(CheckerPiece p, vector<GameMove>& moves)
{
	POSITIONSTATE pState = checkPosition(p.pos);
	if(p.identity == A_PIECE || p.identity == A_KING)
	{
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == LEFT_COLUMN)
		{
			int newX = p.pos.x + 1;
			int newY = p.pos.y - 1;
			if(gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, true, false));
				moves.back().upgrade = checkKingship(newX, newY, p.identity);
			}
		}
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = p.pos.x - 1;
			int newY = p.pos.y - 1;
			if(gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, true, false));
				moves.back().upgrade = checkKingship(newX, newY, p.identity);
			}
		}
	}
	else if(p.identity == B_PIECE || p.identity == B_KING)
	{
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_LEFT || pState == LEFT_COLUMN)
		{
			int newX = p.pos.x + 1;
			int newY = p.pos.y + 1;
			if(gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, false, false));
				moves.back().upgrade = checkKingship(newX, newY, p.identity);
			}
		}
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = p.pos.x - 1;
			int newY = p.pos.y + 1;
			if(gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, false, false));
				moves.back().upgrade = checkKingship(newX, newY, p.identity);
			}
		}
	}
}

void getForwardJumps(CheckerPiece p, vector<GameMove>& moves)
{
	POSITIONSTATE pState = checkPosition(p.pos);
	bool killedKing = false;
	if(p.identity == A_PIECE || p.identity == A_KING)
	{
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == LEFT_COLUMN)
		{
			int newX = p.pos.x + 1;
			int newY = p.pos.y - 1;
			if(((killedKing = (gameBoard[newX][newY] == B_KING)) || gameBoard[newX][newY] == B_PIECE))
			{
				newX += 1;
				newY -= 1;
				if(newX < BOARD_COL_NUM && newY >= 0 && gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, true, true));
					moves.back().upgrade = checkKingship(newX, newY, p.identity);
					moves.back().killedKing = killedKing;
				}
			}
		}
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = p.pos.x - 1;
			int newY = p.pos.y - 1;
			if(((killedKing = (gameBoard[newX][newY] == B_KING)) || gameBoard[newX][newY] == B_PIECE))
			{
				newX -= 1;
				newY -= 1;
				if(newX >= 0 && newY >= 0 && gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, true, true));
					moves.back().upgrade = checkKingship(newX, newY, p.identity);
					moves.back().killedKing = killedKing;
				}
			}
		}
	}
	else if(p.identity == B_PIECE || p.identity == B_KING)
	{
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_LEFT || pState == LEFT_COLUMN)
		{
			int newX = p.pos.x + 1;
			int newY = p.pos.y + 1;
			if(((killedKing = (gameBoard[newX][newY] == A_KING)) || gameBoard[newX][newY] == A_PIECE))
			{
				newX += 1;
				newY += 1;
				if(newX < BOARD_COL_NUM && newY < BOARD_ROW_NUM && gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, false, true));
					moves.back().upgrade = checkKingship(newX, newY, p.identity);
					moves.back().killedKing = killedKing;
				}
			}
		}
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = p.pos.x - 1;
			int newY = p.pos.y + 1;
			if(((killedKing = (gameBoard[newX][newY] == A_KING)) || gameBoard[newX][newY] == A_PIECE))
			{
				newX -= 1;
				newY += 1;
				if(newX >= 0 && newY < BOARD_ROW_NUM && gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, false, true));
					moves.back().upgrade = checkKingship(newX, newY, p.identity);
					moves.back().killedKing = killedKing;
				}
			}
		}
	}
}

void getBackwardMoves(CheckerPiece p, vector<GameMove>& moves)
{
	POSITIONSTATE pState = checkPosition(p.pos);
	if(p.identity == A_KING)
	{
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_LEFT || pState == LEFT_COLUMN)
		{
			int newX = p.pos.x + 1;
			int newY = p.pos.y + 1;
			if(gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, true, false));
			}
		}
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = p.pos.x - 1;
			int newY = p.pos.y + 1;
			if(gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, true, false));
			}
		}
	}
	else if(p.identity == B_KING)
	{
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == LEFT_COLUMN)
		{
			int newX = p.pos.x + 1;
			int newY = p.pos.y - 1;
			if(gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, false, false));
			}
		}
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = p.pos.x - 1;
			int newY = p.pos.y - 1;
			if(gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
			{
				moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, false, false));
			}
		}
	}	
}

void getBackwardJumps(CheckerPiece p, vector<GameMove>& moves)
{
	POSITIONSTATE pState = checkPosition(p.pos);
	bool killedKing = false;
	if(p.identity == A_KING)
	{
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_LEFT || pState == LEFT_COLUMN)
		{
			int newX = p.pos.x + 1;
			int newY = p.pos.y + 1;
			if(((killedKing = (gameBoard[newX][newY] == B_KING)) || gameBoard[newX][newY] == B_PIECE))
			{
				newX += 1;
				newY += 1;
				if(newX < BOARD_COL_NUM && newY < BOARD_ROW_NUM && gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, true, true));
					moves.back().killedKing = killedKing;
				}
			}
		}
		if(pState == MIDDLE || pState == TOP_ROW || pState == TOP_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = p.pos.x - 1;
			int newY = p.pos.y + 1;
			if(((killedKing = (gameBoard[newX][newY] == B_KING)) || gameBoard[newX][newY] == B_PIECE))
			{
				newX -= 1;
				newY += 1;
				if(newX >= 0 && newY < BOARD_ROW_NUM && gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, true, true));
					moves.back().killedKing = killedKing;
				}
			}
		}
	}
	else if(p.identity == B_KING)
	{
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == LEFT_COLUMN)
		{
			int newX = p.pos.x + 1;
			int newY = p.pos.y - 1;
			if(((killedKing = (gameBoard[newX][newY] == A_KING)) || gameBoard[newX][newY] == A_PIECE))
			{
				newX += 1;
				newY -= 1;
				if(newX < BOARD_COL_NUM && newY >= 0 && gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, false, true));
					moves.back().killedKing = killedKing;
				}
			}
		}
		if(pState == MIDDLE || pState == BOTTOM_ROW || pState == BOTTOM_RIGHT || pState == RIGHT_COLUMN)
		{
			int newX = p.pos.x - 1;
			int newY = p.pos.y - 1;
			if(((killedKing = (gameBoard[newX][newY] == A_KING)) || gameBoard[newX][newY] == A_PIECE))
			{
				newX -= 1;
				newY -= 1;
				if(newX >= 0 && newY >= 0 && gameBoard[newX][newY] == EMPTY_DARK_SQUARE)
				{
					moves.push_back(GameMove(p.pos.x, p.pos.y, newX, newY, false, true));
					moves.back().killedKing = killedKing;
				}
			}
		}
	}
}

POSITIONSTATE checkPosition(Vector2 pos)
{
	if(pos.x == 0)
	{
		if(pos.y == 0) return TOP_LEFT;
		else if(pos.y == BOARD_ROW_NUM - 1) return BOTTOM_LEFT;
		else return LEFT_COLUMN;
	}
	else if(pos.x == BOARD_COL_NUM - 1)
	{
		if(pos.y == 0) return TOP_RIGHT;
		else if(pos.y == BOARD_ROW_NUM - 1) return BOTTOM_RIGHT;
		else return RIGHT_COLUMN;
	}
	else if(pos.y == 0)
	{
		return TOP_ROW;
	}
	else if(pos.y == BOARD_ROW_NUM - 1)
	{
		return BOTTOM_ROW;
	}
	else return MIDDLE;
}

bool checkKingship(int x, int y, BOARDPOSITION bPosition)
{
	Vector2 currVec;
	currVec.x = x;
	currVec.y = y;

	POSITIONSTATE pState = checkPosition(currVec);

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

void kingship()
{
	POSITIONSTATE pState;

	for(int i = 0; i < APieces.size(); ++i)
	{
		pState = checkPosition(APieces[i].pos);
		if(APieces[i].identity == A_PIECE && (pState == TOP_ROW || pState == TOP_LEFT || pState == TOP_RIGHT))
		{
			APieces[i].identity = A_KING;
		}
	}

	for(int i = 0; i < BPieces.size(); ++i)
	{
		pState = checkPosition(BPieces[i].pos);
		if(BPieces[i].identity == B_PIECE && (pState == BOTTOM_ROW || pState == BOTTOM_LEFT || pState == BOTTOM_RIGHT))
		{
			BPieces[i].identity = B_KING;
		}
	}
}

int currentHeuristics()
{
	int retValue = 0;
	int i = 0, j = 0;

	while(i < APieces.size() || j < BPieces.size())
	{
		if(i < APieces.size() && j < BPieces.size())
		{
			if(APieces[i].identity == A_PIECE && BPieces[j].identity == B_KING)
			{
				--retValue;
			}
			else if(APieces[i].identity == A_KING && BPieces[j].identity == B_PIECE)
			{
				++retValue;
			}
			++i;
			++j;
		}
		else if(i < APieces.size())
		{
			if(APieces[i].identity == A_PIECE) ++retValue;
			else retValue += 2;
			++i;
		}
		else
		{
			if(BPieces[j].identity == B_PIECE) --retValue;
			else retValue -= 2;
			++j;
		}
	}

	return retValue;
}

void updateHeuristics(vector<GameMove>& moves)
{
	int cHeuristics = currentHeuristics();
	for(int i = 0; i < moves.size(); ++i)
	{
		if(moves[i].AMoved)
		{
			if(moves[i].isJump)
			{
				++cHeuristics;
				if(BPieces.size() == 1) //If the opponent has no pieces left
				{
					cHeuristics = INT_MAX;
					continue;
				}
			}
			if(moves[i].killedKing) ++cHeuristics;
			if(moves[i].upgrade) ++cHeuristics;
		}
		else
		{
			if(moves[i].isJump)
			{
				--cHeuristics;
				if(APieces.size() == 1) //If the opponent has no pieces left
				{
					cHeuristics = INT_MIN;
					continue;
				}
			}
			if(moves[i].killedKing) --cHeuristics;
			if(moves[i].upgrade) --cHeuristics;
		}
		moves[i].hValue = cHeuristics;
	}
}

int gameMaxValue(GameNode *node, int alpha, int beta)
{
	if(node->successors.empty())
	{
		node->printData();
		return node->data;
	}

	node->alpha = alpha;
	node->beta = beta;
	node->printAlphaBeta();
	for(int i = 0; i < node->successors.size(); ++i)
	{
		node->alpha = max(node->alpha, gameMinValue(node->successors[i], node->alpha, node->beta));
		node->printAlphaBeta();
		if(node->alpha >= node->beta)
		{
			cout << "Pruning: ";
			node->printAlphaBeta();
			for(int j = i + 1; j < node->successors.size(); ++j)
			{
				cout << "Pruning: " << node->successors[j]->name << endl;
			}
			node->deleteNode();
			return node->beta;
		}
	}
	return node->alpha;
}

int gameMinValue(GameNode *node, int alpha, int beta)
{
	if(node->successors.empty())
	{
		node->printData();
		return node->data;
	}

	node->alpha = alpha;
	node->beta = beta;
	node->printAlphaBeta();
	for(int i = 0; i < node->successors.size(); ++i)
	{
		node->beta = min(node->beta, gameMaxValue(node->successors[i], node->alpha, node->beta));
		node->printAlphaBeta();
		if(node->beta <= node->alpha)
		{
			cout << "Pruning: ";
			node->printAlphaBeta();
			for(int j = i + 1; j < node->successors.size(); ++j)
			{
				cout << "Pruning: " << node->successors[j]->name << endl;
			}
			node->deleteNode();
			return node->alpha;
		}
	}
	return node->beta;
}

int main(int argc, char **argv)
{
	/*
	if(!InputBoard(""))
	{
		return 1; //Exit due to error
	}
	*/

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

	getchar();
	return 0;
}