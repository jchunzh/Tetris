#include <iostream>
#include <stdio.h>
#include <allegro5\allegro.h>
#include "Block.h"

using namespace std;

int Block::GetX() { return x; }
int Block::GetY() { return y; }
void Block::SetX(float x) { this->x = x; }
void Block::SetY(float y) { this->y = y; }
ALLEGRO_BITMAP* Block::GetImage() { return image; }
void Block::SetImage(ALLEGRO_BITMAP *image) { this->image = image; }

Block::Block()
{
	this->image = NULL;
	this->occupied = false;
}
Block::Block(ALLEGRO_BITMAP* image)
{
	x = 0;
	y = 0;
	this->image = image;
	this->occupied = true;
}

Block::Block(ALLEGRO_BITMAP * image, int x, int y)
{
	this->x = x;
	this->y = y;
	this->image = image;
}

void Block::Draw(float xPixel, float yPixel)
{
	al_draw_bitmap(image, xPixel, yPixel, 0);
}