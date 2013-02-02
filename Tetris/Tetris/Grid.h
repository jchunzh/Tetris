#ifndef GRID_H
#define GRID_H
#include "Block.h"
#include "Piece.h"
#include "PieceTypeEnum.h"
#include "PieceOrientationEnum.h"
#include "RotationEnum.h"
#include <vector>

using namespace std;

class Grid
{
protected:
	//Length of grid in units
	int length;
	//Height of grid in units
	int height;
	int size;
	vector<Block*> *** blocks;
	int pieceSize;
	int pieceArrayPos;
	std::vector<Piece *> * pieces;

public:
	Grid(int length, int height);
	int GetSize();
	int GetLength();
	int GetHeight();
	vector<Block*> *** GetBlocks();
	vector<Piece*> * GetPieces();
	//Add block to grid at (x,y) coordinates starting at (0,0)
	//Sets block.x and block.y to x and y
	void AddBlock(Block *block, int x, int y);
	vector<Block*> * GetBlocksAt(int x, int y);
	//Removes input block at the blocks x,y coord
	void RemoveBlock(Block * block);
	//Removes all blocks at (x,y)
	void RemoveBlocksAt(int x, int y);
	bool TryMoveBlock(Block * block, int x2, int y2);
	//Move block at (x1, y1) to (x2, y2). ]
	//Returns false if the block at (x1, y1) does not exist, (x2, y2) is off the grid, 
	//or if there is a block at (x2, y2)
	bool MoveBlock(Block * block, int x2, int y2);
	//Adds piece to grid. Returns false if the location is occupied or out of bounds.	
	bool AddPiece(Piece * piece);
	bool AddPieceToEmpty(Piece * piece);
	//Moves piece by (delX, delY)
	bool MovePiece(Piece * piece, int delX, int delY);
	bool MovePieceToEmpty(Piece * piece, int delX, int delY);
	bool RotatePiece(Piece * piece, RotationEnum dir);
	bool RotatePiece(Piece * piece, RotationEnum dir, int xOffset, int yOffset);
};

#endif