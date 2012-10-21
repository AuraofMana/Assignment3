#ifndef GAMETREE_H__
#define GAMETREE_H__

#include <vector>

//Defines the size of the checkerboard
const unsigned int BOARD_ROW_NUM = 8;
const unsigned int BOARD_COL_NUM = 8;

//Represents the possible states a position on the checkerboard could have
enum BOARDPOSITION
{
	LIGHT_SQUARE,
	EMPTY_DARK_SQUARE,
	A_PIECE,
	B_PIECE,
	A_KING,
	B_KING,
	UNDEFINED
};

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
	int depth; //Depth of the current node
	int fromX; //X-coordinate of the from
	int fromY; //Y-coordinate of the from
	int toX; //X-coordinate of the to
	int toY; //Y-coordinate of the to
	std::vector<GameNode*> successors; //A list of successor nodes

	//METHODS
	GameNode();
	GameNode(const GameNode& pGameNode); //Copy constructor -- Only copies gameBoard, heuristics, and the opposite of isMaxNode
	GameNode(BOARDPOSITION pGameBoard[BOARD_ROW_NUM][BOARD_COL_NUM]);
	GameNode(bool pIsMaxNode);
	GameNode(BOARDPOSITION pGameBoard[BOARD_ROW_NUM][BOARD_COL_NUM], bool pIsMaxNode);
	~GameNode();
	void deleteNode();
	void setName();
	void printAlphaBeta() const;
	void printDepth() const;
	void printPruningInfo() const;
	void printGameBoard() const;
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