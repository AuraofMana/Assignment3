#ifndef GAMETREE_H__
#define GAMETREE_H__

#include <vector>

class GameNode
{
public:
	//MEMBERS
	int data;
	int alpha;
	int beta;
	bool isMaxNode;
	std::string name;
	std::vector<GameNode*> successors;

	//METHODS
	GameNode();
	GameNode(int pData);
	GameNode(bool pIsMaxNode);
	GameNode(int pData, bool pIsMaxNode);
	~GameNode();
	void deleteNode();
	void printAlphaBeta();
	void printData();
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