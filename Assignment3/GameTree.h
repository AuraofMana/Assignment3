#ifndef GAMETREE_H__
#define GAMETREE_H__

#include <vector>

#include "CheckerPiece.h"

class GameNode
{
public:
	//MEMBERS
	BOARDPOSITION gameBoard[BOARD_ROW_NUM][BOARD_COL_NUM]; //The configuration of the checker board
	int heuristics; //Heuristics value
	int alpha; //Alpha value for Alphabeta pruning
	int beta; //Beta value for Alphabeta pruning
	bool isMaxNode; //Flag for whether or not this node is a max node
	std::string name; //Name for the flag (named by moves)
	std::vector<GameNode*> successors; //A list of successor nodes

	//METHODS
	GameNode();
	GameNode(const GameNode& pGameNode); //Copy constructor -- Only copies gameBoard, heuristics, and the opposite of isMaxNode
	GameNode(BOARDPOSITION pGameBoard[BOARD_ROW_NUM][BOARD_COL_NUM]);
	GameNode(bool pIsMaxNode);
	GameNode(BOARDPOSITION pGameBoard[BOARD_ROW_NUM][BOARD_COL_NUM], bool pIsMaxNode);
	~GameNode();
	void deleteNode();
	void printAlphaBeta();
	void printGameBoard();
};

class GameTree
{
public:
	//MEMBERS
	GameNode *head;

	//METHODS
	GameTree();
	GameTree(GameNode *pHead);
	~GameTree();
};

#endif