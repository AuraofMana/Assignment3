//PROJECT HEADERS
#include "CheckerPiece.h"

CheckerPiece::CheckerPiece():
identity(UNDEFINED)
{
	pos.x = BOARD_COL_NUM + 1;
	pos.y = BOARD_ROW_NUM + 1;
};

CheckerPiece::CheckerPiece(BOARDPOSITION p_identity, unsigned int p_x, unsigned int p_y):
identity(p_identity)
{
	pos.x = p_x;
	pos.y = p_y;
}

CheckerPiece::CheckerPiece(BOARDPOSITION p_identity, Vector2 p_pos):
identity(p_identity), pos(p_pos)
{

}

CheckerPiece::~CheckerPiece()
{

}