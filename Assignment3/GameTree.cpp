#include "GameTree.h"
#include <climits>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

GameNode::GameNode():
heuristics(0), alpha(INT_MIN), beta(INT_MAX), isMaxNode(true)
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
heuristics(pGameNode.heuristics), alpha(INT_MIN), beta(INT_MAX), isMaxNode(!(pGameNode.isMaxNode))
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
heuristics(0), alpha(INT_MIN), beta(INT_MAX), isMaxNode(true)
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
heuristics(0), alpha(INT_MIN), beta(INT_MAX), isMaxNode(pIsMaxNode)
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
heuristics(0), alpha(INT_MIN), beta(INT_MAX), isMaxNode(pIsMaxNode)
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
	deleteNode();
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

void GameNode::printAlphaBeta()
{
	cout << name << "(";

	if(alpha == INT_MIN) cout << "-inf";
	else cout << alpha;

	cout << ", ";

	if(beta == INT_MAX) cout << "inf";
	else cout << beta;

	cout << ")" << endl;
}

void GameNode::printGameBoard()
{
	for(int i = 0; i < BOARD_ROW_NUM; ++i)
	{
		for(int j = 0; j < BOARD_COL_NUM; ++j)
		{
			cout << gameBoard[i][j];
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