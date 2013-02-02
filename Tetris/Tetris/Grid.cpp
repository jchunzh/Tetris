#include "Grid.h"
#include "Block.h"
#include "Piece.h"
#include "RotationEnum.h"
#include <iostream>
#include <vector>
//Protected:

using namespace std;

//Public:
int Grid::GetLength() { return length;}
int Grid::GetHeight() { return height;}
int Grid::GetSize() { return size; }
vector<Piece *> * Grid::GetPieces() { return pieces; }
vector<Block*> *** Grid::GetBlocks() { return blocks; }

Grid::Grid(int length, int height)
{
	pieceSize = 4;
	pieceArrayPos = 0;
	this->length = length;
	this->height = height;
	this->size = length * height;
	this->blocks = new vector<Block*> ** [length];

	for (int i = 0; i < length; i++)
	{
		this->blocks[i] = new vector<Block*> * [height];

		for (int j = 0; j < height; j++)
		{
			this->blocks[i][j] = new vector<Block*>();
		}
	}

	this->pieces = new std::vector<Piece*>();
}

//Add block to grid at (x,y) coordinates starting at (0,0)
//Sets block.x and block.y to x and y
void Grid::AddBlock(Block * block, int x, int y)
{
	delete blocks[x][y];
	blocks[x][y]->push_back(block);
}

void Grid::RemoveBlock(Block * block)
{
	int x = block->GetX();
	int y = block->GetY();

	for (int z = 0; z < blocks[x][y]->size(); z++)
	{
		if (blocks[x][y]->at(z) == block)
		{
			blocks[x][y]->erase(blocks[x][y]->begin() + z);
//			blocks[x][y]->shrink_to_fit();
			delete block;
		}
	}
	//delete blocks[x][y];
}

void Grid::RemoveBlocksAt(int x, int y)
{
	delete blocks[x][y];
	blocks[x][y] = new vector<Block*>();
}

vector<Block*> * Grid::GetBlocksAt(int x, int y) 
{ 
	if (x >= this->length || y >= this->height || x < 0 || y < 0)
		return NULL;

	return this->blocks[x][y]; 
}

bool Grid::TryMoveBlock(Block * block, int x2, int y2)
{
	if (x2 >= length || y2 >= height || x2 < 0 || y2 < 0) 
		return false;

	return true;
}

bool Grid::MoveBlock(Block * block, int x2, int y2)
{
	if (x2 >= length || y2 >= height || x2 < 0 || y2 < 0) 
		return false;

	int x1 = block->GetX();
	int y1 = block->GetY();

	if (x1 == x2 && y1 == y2)
		return true;

	blocks[x2][y2]->push_back(block);
	vector<Block*> * temp = new vector<Block*>();
	bool skipOnce = false;

	for (int z = 0; z < blocks[x1][y1]->size(); z++)
	{
		if (blocks[x1][y1]->at(z) != block)
		{
			temp->push_back(blocks[x1][y1]->at(z));
		}
	}

	blocks[x1][y1] = temp;
	block->SetX(x2);	
	block->SetY(y2);

	return true;
}

bool Grid::AddPiece(Piece * piece)
{
	for (int i = 0; i < piece->GetSize(); i++)
	{
		int x = (piece->GetBlocks()[i])->GetX();
		int y = (piece->GetBlocks()[i])->GetY();

		if (x >= length || x < 0 || y >= height || y < 0)
			return false;
	}

	pieces->push_back(piece);

	//std::cout << "Piece added at: " << endl;

	for (int i = 0; i < piece->GetSize(); i++)
	{
		int x = (piece->GetBlocks()[i])->GetX();
		int y = (piece->GetBlocks()[i])->GetY();
		blocks[x][y]->push_back(piece->GetBlocks()[i]);
		//std::cout << "(" << blocks[x][y]->GetX() << "," << blocks[x][y]->GetY() << ")" << std::endl;
	}

	return true;
}

bool Grid::AddPieceToEmpty(Piece * piece)
{
	for (int i = 0; i < piece->GetSize(); i++)
	{
		int x = (piece->GetBlocks()[i])->GetX();
		int y = (piece->GetBlocks()[i])->GetY();

		if (x >= length || x < 0 || y >= height || y < 0)
			return false;
		//Space is occupied
		if (GetBlocksAt(x, y)->size() > 0)
			return false;
	}

	pieces->push_back(piece);

	//std::cout << "Piece added at: " << endl;

	for (int i = 0; i < piece->GetSize(); i++)
	{
		int x = (piece->GetBlocks()[i])->GetX();
		int y = (piece->GetBlocks()[i])->GetY();
		blocks[x][y]->push_back(piece->GetBlocks()[i]);
		//std::cout << "(" << blocks[x][y]->GetX() << "," << blocks[x][y]->GetY() << ")" << std::endl;
	}

	return true;
}

//Moves piece to (delX, delY)
bool Grid::MovePiece(Piece * piece, int delX, int delY)
{
	for (int i = 0; i < piece->GetSize(); i++)
	{
		//Don't have to update positions in the Piece object because MoveBlock will change what positions it points to
		if (!MoveBlock(piece->GetBlocks()[i], piece->GetBlocks()[i]->GetX() + delX, piece->GetBlocks()[i]->GetY() + delY))
		{
			//Move blocks back into their original position
			//TODO move into own method?
			for (int j = i - 1; j >= 0; j--)
			{
				MoveBlock(piece->GetBlocks()[j], piece->GetBlocks()[j]->GetX() - delX, piece->GetBlocks()[j]->GetY() - delY);
			}

			std::cout << "Move failed" << std::endl;

			return false;
		}
	}

	piece->SetX(piece->GetX() + delX);
	piece->SetY(piece->GetY() + delY);

	return true;
}

bool Grid::MovePieceToEmpty(Piece * piece, int delX, int delY)
{
	if (!MovePiece(piece, delX, delY))
		return false;

	for (int i = 0; i < piece->GetSize(); i++)
	{
		//Check if any other blocks are at the new position
		if (GetBlocksAt(piece->GetBlocks()[i]->GetX(), piece->GetBlocks()[i]->GetY())->size() > 1)
		{
			vector<Block*> * temp = GetBlocksAt(piece->GetBlocks()[i]->GetX(), piece->GetBlocks()[i]->GetY());
			
			for (int i = 0; i < temp->size(); i++)
			{
				Block * b = temp->at(i);
			}

			//Move piece back if they are
			MovePiece(piece, -delX, -delY);
			return false;
		}
	}

	return true;
}

bool Grid::RotatePiece(Piece * piece, RotationEnum dir)
{
	return RotatePiece(piece, dir, 0, 0);
}

bool Grid::RotatePiece(Piece * piece, RotationEnum dir, int xOffset, int yOffset)
{
	int ** matrix = new int * [2];
	matrix[0] = new int[2];
	matrix[1] = new int[2];

	matrix[0][0] = 0;
	matrix[1][0] = -1 * dir;
	matrix[0][1] = 1 * dir;
	matrix[1][1] = 0;

	for (int i = 0; i < piece->GetSize(); i++)
	{
		//Move reference block to (0,0)
		int x = matrix[0][0] * (piece->GetBlocks()[i]->GetX() - piece->GetX()) + matrix[1][0] * (piece->GetBlocks()[i]->GetY() - piece->GetY());
		int y = matrix[0][1] * (piece->GetBlocks()[i]->GetX() - piece->GetX()) + matrix[1][1] * (piece->GetBlocks()[i]->GetY() - piece->GetY());

		if (!TryMoveBlock(piece->GetBlocks()[i], piece->GetX() + xOffset + x, piece->GetY() + yOffset + y))
		{
			return false;
		}
	}

	for (int i = 0; i < piece->GetSize(); i++)
	{
		int x = matrix[0][0] * (piece->GetBlocks()[i]->GetX() - piece->GetX()) + matrix[1][0] * (piece->GetBlocks()[i]->GetY() - piece->GetY());
		int y = matrix[0][1] * (piece->GetBlocks()[i]->GetX() - piece->GetX()) + matrix[1][1] * (piece->GetBlocks()[i]->GetY() - piece->GetY());

		//Add offset back to rotated blocks
		MoveBlock(piece->GetBlocks()[i], piece->GetX() + xOffset + x, piece->GetY() + yOffset + y);
	}

	return true;
}