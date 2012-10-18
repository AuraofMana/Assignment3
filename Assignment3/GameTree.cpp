#include "GameTree.h"
#include <climits>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

GameNode::GameNode():
data(-1), alpha(INT_MIN), beta(INT_MAX), isMaxNode(true)
{
	
}

GameNode::GameNode(int pData):
data(pData), alpha(INT_MIN), beta(INT_MAX), isMaxNode(true)
{

}

GameNode::GameNode(bool pIsMaxNode):
data(-1), alpha(INT_MIN), beta(INT_MAX), isMaxNode(pIsMaxNode)
{

}

GameNode::GameNode(int pData, bool pIsMaxNode):
data(pData), alpha(INT_MIN), beta(INT_MAX), isMaxNode(pIsMaxNode)
{

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

void GameNode::printData()
{
	cout << name << "(" << data << ")" << endl;
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