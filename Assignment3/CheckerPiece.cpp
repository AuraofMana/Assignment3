//PROJECT HEADERS
#include "CheckerPiece.h"

CheckerPiece::CheckerPiece():
identity(UNDEFINED)
{
	pos.x = BOARD_COL_NUM + 1;
	pos.y = BOARD_ROW_NUM + 1;
};

CheckerPiece::CheckerPiece(BOARDPOSITION pIdentity, Vector2 pPos):
identity(pIdentity), pos(pPos)
{

}

CheckerPiece::CheckerPiece(BOARDPOSITION pIdentity, unsigned int pX, unsigned int pY):
identity(pIdentity)
{
	pos.x = pX;
	pos.y = pY;
}

CheckerPiece::~CheckerPiece()
{

}