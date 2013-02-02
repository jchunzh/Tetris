#ifndef BLOCK_H
#define BLOCK_H
#include "allegro5\allegro.h"

class Block
{
private:

protected:
	//Position on grid
	float x;
	float y;
	ALLEGRO_BITMAP *image;

public:
	bool occupied;
	Block();
	Block(ALLEGRO_BITMAP* image);
	Block(ALLEGRO_BITMAP * image, int x, int y);
	int GetX();
	int GetY();
	void SetX(float x);
	void SetY(float y);
	void Draw(float xPixel, float yPixel);
	ALLEGRO_BITMAP* GetImage();
	void SetImage(ALLEGRO_BITMAP *image);
};

#endif