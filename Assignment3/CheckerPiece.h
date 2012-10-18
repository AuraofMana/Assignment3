#ifndef CHECKERPIECE_H__
#define CHECKERPIECE_H__

//STL HEADERS
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

//Represents the 2D coordinate on the checkerboard
struct Vector2
{
	unsigned int x;
	unsigned int y;
};

//Represents a piece on the checkerboard
class CheckerPiece
{
public:
	//MEMBERS
	BOARDPOSITION identity; //Only uses A_PIECE, B_PIECE, A_KING, B_KING, and UNDEFINED for checking the identity of the piece
	Vector2 pos; //2D position of the piece

	//METHODS
	CheckerPiece();
	CheckerPiece(BOARDPOSITION p_identity, Vector2 p_pos);
	CheckerPiece(BOARDPOSITION p_identity, unsigned int p_x, unsigned int p_y);
	~CheckerPiece();
};

#endif