#include "Piece.h"
#include "PieceTypeEnum.h"
#include "PieceOrientationEnum.h"
#include "Block.h"
#include <iostream>

using namespace std;

Block ** Piece::GetBlocks() { return blocks; }
int Piece::GetX() { return x; }
int Piece::GetY() { return y; }
void Piece::SetX(int x) { this->x = x; }
void Piece::SetY(int y) { this->y = y; }
PieceOrientationEnum Piece::GetOrientation() { return this->orientation; }
int Piece::GetSize() { return size; }
bool Piece::IsInit() { return init; }
bool Piece::IsInFocus() 
{ 

	return inFocus; 
}
void Piece::SetInFocus(bool inFocus) { this->inFocus = inFocus; }

//Grid contains position information, Piece only contains image and type info
Piece::Piece(int x, int y, PieceTypeEnum pieceType, PieceOrientationEnum pieceOrientation, ALLEGRO_BITMAP * image)
{
	this->inFocus = true;
	init = true;
	size = 4;
	this->x = x;
	this->y = y;
	//Block's positions are relative to (x,y)
	this->blocks = new Block * [size];
	this->pieceType = pieceType;
	this->orientation = *(new PieceOrientationEnum(pieceOrientation));

	blocks[0] = new Block(image, x, y);

	switch (pieceType)
	{
	case I:
		blocks[1] = new Block(image, x, y);
		blocks[2] = new Block(image, x, y+1);
		blocks[0] = new Block(image, x, y+2);
		blocks[3] = new Block(image, x, y+3);
		break;
	case J:
		blocks[1] = new Block(image, x + 1, y);
		blocks[2] = new Block(image, x + 1, y+1);
		blocks[3] = new Block(image, x + 1, y+2);
		break;
	case L:
		blocks[1] = new Block(image, x + 1, y);
		blocks[2] = new Block(image, x, y+1);
		blocks[3] = new Block(image, x, y+2);
		break;
	case O:
		blocks[1] = new Block(image, x+1, y);
		blocks[2] = new Block(image, x, y+1);
		blocks[3] = new Block(image, x+1, y+1);
		break;
	case S:
		blocks[1] = new Block(image, x + 1, y);
		blocks[2] = new Block(image, x + 1, y+1);
		blocks[3] = new Block(image, x + 2, y+1);
		break;
	case T:
		blocks[0] = new Block(image, x, y);
		blocks[1] = new Block(image, x, y + 1);
		blocks[2] = new Block(image, x + 1, y + 1);
		blocks[3] = new Block(image, x - 1, y + 1);
		break;
	case Z:
		blocks[1] = new Block(image, x, y);
		blocks[0] = new Block(image, x, y + 1);
		blocks[2] = new Block(image, x + 1, y);
		blocks[3] = new Block(image, x - 1 , y + 1);
		break;
	}

	//cout << blocks[0] << endl;
	//cout << blocks[1] << endl;
	//cout << blocks[2] << endl;
	//cout << blocks[3] << endl;
}
