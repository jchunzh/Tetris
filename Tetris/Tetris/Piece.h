#ifndef PIECE_H
#define PIECE_H
#include "Block.h"
#include "PieceTypeEnum.h"
#include "PieceOrientationEnum.h"

class Piece
{
protected:
	Block ** blocks;
	
	PieceOrientationEnum orientation;
	int size;
	bool init;
	int x;
	int y;
	bool inFocus;

public:
	PieceTypeEnum pieceType;
	Piece(int x, int y, PieceTypeEnum  pieceType, PieceOrientationEnum pieceOrientation, ALLEGRO_BITMAP * image);
	int GetX();
	int GetY();
	void SetX(int x);
	void SetY(int y);
	bool IsInFocus();
	void SetInFocus(bool focus);
	PieceOrientationEnum GetOrientation();
	bool IsInit();
	Block ** GetBlocks();
	void Draw();
	int GetSize();
	void Rotate(PieceOrientationEnum orientation);
};

#endif
