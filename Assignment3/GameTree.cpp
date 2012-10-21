#include "GameTree.h"
#include <climits>
#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

GameNode::GameNode():
heuristics(0), alpha(INT_MIN), beta(INT_MAX), isMaxNode(true), depth(-1), fromX(-1), fromY(-1), toX(-1), toY(-1)
{
	for(int i = 0; i < BOARD_ROW_NUM; ++i)
	{
		for(int j = 0; j < BOARD_COL_NUM; ++j)
		{
			gameBoard[i][j] = UNDEFINED;
		}
	}
}

GameNode::GameNode(const GameNode& pGameNode):
heuristics(pGameNode.heuristics), alpha(INT_MIN), beta(INT_MAX), isMaxNode(!(pGameNode.isMaxNode)), depth(pGameNode.depth + 1), fromX(-1), fromY(-1), toX(-1), toY(-1)
{
	for(int i = 0; i < BOARD_ROW_NUM; ++i)
	{
		for(int j = 0; j < BOARD_COL_NUM; ++j)
		{
			gameBoard[i][j] = pGameNode.gameBoard[i][j];
		}
	}
}

GameNode::GameNode(BOARDPOSITION pGameBoard[BOARD_ROW_NUM][BOARD_COL_NUM]):
heuristics(0), alpha(INT_MIN), beta(INT_MAX), isMaxNode(true), depth(-1), fromX(-1), fromY(-1), toX(-1), toY(-1)
{
	for(int i = 0; i < BOARD_ROW_NUM; ++i)
	{
		for(int j = 0; j < BOARD_COL_NUM; ++j)
		{
			gameBoard[i][j] = pGameBoard[i][j];
			switch(gameBoard[i][j])
			{
			case A_PIECE:
				++heuristics;
				break;
			case B_PIECE:
				--heuristics;
				break;
			case A_KING:
				heuristics += 2;
				break;
			case B_KING:
				heuristics -= 2;
				break;
			}
		}
	}
}

GameNode::GameNode(bool pIsMaxNode):
heuristics(0), alpha(INT_MIN), beta(INT_MAX), isMaxNode(pIsMaxNode), depth(-1), fromX(-1), fromY(-1), toX(-1), toY(-1)
{
	for(int i = 0; i < BOARD_ROW_NUM; ++i)
	{
		for(int j = 0; j < BOARD_COL_NUM; ++j)
		{
			gameBoard[i][j] = UNDEFINED;
		}
	}
}

GameNode::GameNode(BOARDPOSITION pGameBoard[BOARD_ROW_NUM][BOARD_COL_NUM], bool pIsMaxNode):
heuristics(0), alpha(INT_MIN), beta(INT_MAX), isMaxNode(pIsMaxNode), depth(-1), fromX(-1), fromY(-1), toX(-1), toY(-1)
{
	for(int i = 0; i < BOARD_ROW_NUM; ++i)
	{
		for(int j = 0; j < BOARD_COL_NUM; ++j)
		{
			gameBoard[i][j] = pGameBoard[i][j];
			switch(gameBoard[i][j])
			{
			case A_PIECE:
				++heuristics;
				break;
			case B_PIECE:
				--heuristics;
				break;
			case A_KING:
				heuristics += 2;
				break;
			case B_KING:
				heuristics -= 2;
				break;
			}
		}
	}
}

GameNode::~GameNode()
{
	//deleteNode();
}

void GameNode::deleteNode()
{
	if(!successors.empty())
	{
		for(int i = 0; i < successors.size(); ++i)
		{
			successors[i]->deleteNode();
		}
		successors.clear();
	}
	delete this;
}

void GameNode::setName()
{
	stringstream ssOut;
	ssOut << depth << " " << fromX << " " << fromY << " " << toX << " " << toY;

	string depthStr, fromXStr, fromYStr, toXStr, toYStr;
	ssOut >> depthStr >> fromXStr >> fromYStr >> toXStr >> toYStr;

	name = "Player ";
	if(isMaxNode) name += "B ";
	else name += "A ";
	name += "moves the piece at (" + fromXStr + ", " + fromYStr + ") to (" + toXStr + ", " + toYStr + ").";
}

void GameNode::printAlphaBeta() const
{
	cout << "Alphabeta: (";

	if(alpha == INT_MIN) cout << "-inf";
	else cout << alpha;

	cout << ", ";

	if(beta == INT_MAX) cout << "inf";
	else cout << beta;

	cout << ")" << endl;
}

void GameNode::printDepth() const
{
	for(int i = 0; i < depth; ++i)
	{
		cout << "\t";
	}
	cout << "Depth " << depth << ": ";
}

void GameNode::printPruningInfo() const
{
	cout << "(" << fromX << ", " << fromY << ") to (" << toX << ", " << toY << ")";
}

void GameNode::printGameBoard() const
{
	for(int i = 0; i < BOARD_ROW_NUM; ++i)
	{
		for(int j = 0; j < BOARD_COL_NUM; ++j)
		{
			switch(gameBoard[j][i])
			{
			case LIGHT_SQUARE:
				cout << "X";
				break;
			case EMPTY_DARK_SQUARE:
				cout << ".";
				break;
			case A_PIECE:
				cout << "o";
				break;
			case B_PIECE:
				cout << "*";
				break;
			case A_KING:
				cout << "k";
				break;
			case B_KING:
				cout << "K";
				break;
			}
		}
		cout << endl;
	}
}

GameTree::GameTree():
head(NULL)
{
	
}

GameTree::GameTree(GameNode *pHead):
head(pHead)
{

}

GameTree::~GameTree()
{
	head->deleteNode();
}