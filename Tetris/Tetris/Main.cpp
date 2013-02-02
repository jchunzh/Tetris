#include <iostream>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "allegro5/allegro_native_dialog.h"
#include "Block.h"
#include "Grid.h"
#include "PieceOrientationEnum.h"
#include "PieceTypeEnum.h"
#include <random>
#include <time.h>
#include "Settings.h"

using namespace std;
using namespace Settings;

Piece * GeneratePiece(Grid * grid, PieceTypeEnum pieceType, ALLEGRO_BITMAP * image)
{
	Piece * tempPiece;

	switch (pieceType)
	{
	case I:
		tempPiece = new Piece((GRID_WIDTH / 2) - 3, 2, pieceType, NORTH, image);
		grid->AddPiece(tempPiece);
		grid->RotatePiece(tempPiece, RIGHT);
		break;
	case J:
		tempPiece = new Piece((GRID_WIDTH / 2) - 2, 1, pieceType, NORTH, image);
		grid->AddPiece(tempPiece);
		grid->RotatePiece(tempPiece, LEFT);
		break;
	case L:
		tempPiece = new Piece((GRID_WIDTH / 2) - 2, 1, pieceType, WEST, image);
		grid->AddPiece(tempPiece);
		grid->RotatePiece(tempPiece, RIGHT);
		break;
	case O:
		tempPiece = new Piece((GRID_WIDTH / 2) - 1, 0, pieceType, NORTH, image);
		grid->AddPiece(tempPiece);
		break;
	case S:
		tempPiece =  new Piece((GRID_WIDTH / 2) - 2, 0, pieceType, NORTH, image);
		grid->AddPiece(tempPiece);
		break;
	case Z:
		tempPiece = new Piece((GRID_WIDTH / 2) - 2, 0, pieceType, NORTH, image);
		grid->AddPiece(tempPiece);
		break;
	case T:
		tempPiece = new Piece((GRID_WIDTH / 2) - 2, 1, pieceType, NORTH, image);
		grid->AddPiece(tempPiece);
		grid->RotatePiece(tempPiece, LEFT);
		grid->RotatePiece(tempPiece, LEFT);
		grid->MovePiece(tempPiece, (GRID_WIDTH / 2) - 2, 0);
		break;
	};
	//cout << "Piece spawns" << endl;

	return tempPiece;
}

void Draw(Grid * grid, int width, int height, float xOffset, float yOffset)
{
	vector<Block*> *** blocks = grid->GetBlocks();

	for (int i = 0; i < grid->GetLength(); i++)
	{
		for (int j = 0; j < grid->GetHeight(); j++)
		{
			if (blocks[i][j]->size() == 0)
				continue;

			int xPixel = (int)(blocks[i][j]->front()->GetX() * width / GRID_WIDTH) + xOffset;
			int yPixel = (int)(blocks[i][j]->front()->GetY() * height / GRID_HEIGHT) + yOffset;
			blocks[i][j]->front()->Draw(xPixel, yPixel);
		}
	}
}

//Move all blocks one space down starting at startRow
void MoveAllDown(Grid * grid, int startRow)
{
	for (int y = startRow; y >= 0; y--)
	{
		for (int x = 0; x < grid->GetLength(); x++)
		{
			vector<Block*> * space = grid->GetBlocks()[x][y];

			for (int z = 0; z < space->size(); z++)
			{
				grid->MoveBlock(space->at(z), space->at(z)->GetX(), space->at(z)->GetY() + 1);
			}
		}
	}
}

void DeleteRow(Grid * grid, int row)
{
	for (int i = 0; i < grid->GetLength(); i++)
	{
		grid->RemoveBlocksAt(i, row);
	}
}

bool DeleteFullRows(Grid * grid, int & deletedRow)
{
	deletedRow = -1;
	bool hasDeleted = false;

	for (int y = grid->GetHeight() - 1; y >= 0; y--)
	{
		int fullSpaces = 0;
		for (int x = 0; x < grid->GetLength(); x++)
		{
			if (grid->GetBlocks()[x][y]->size() == 0)
				continue;

			fullSpaces++;
		}

		if (fullSpaces == grid->GetLength())
		{
			DeleteRow(grid, y);
			hasDeleted = true;

			if (deletedRow == -1)
				deletedRow = y;

			y++;
		}
	}

	return hasDeleted;
}

void Rotate(Grid * grid, Piece * piece, RotationEnum dir)
{
	switch(piece->pieceType)
	{

	case Z:
		grid->RotatePiece(piece, dir, 1, 0);
		break;
	};

	//grid->RotatePiece(piece, dir);
}

ALLEGRO_BITMAP * ResizeBitmap(ALLEGRO_BITMAP * bitmap, float x, float y)
{
	ALLEGRO_BITMAP * resizedBitmap = al_create_bitmap(x, y);
	al_set_target_bitmap(resizedBitmap);
	al_draw_bitmap(bitmap, 0, 0, 0);

	return resizedBitmap;
}

int Setup(ALLEGRO_DISPLAY * &display, ALLEGRO_DISPLAY_MODE * &set_disp_data, ALLEGRO_EVENT_QUEUE * &event_queue)
{
	ALLEGRO_DISPLAY_MODE disp_data;
		//Must init before creating timer
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	

 
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
 
	event_queue = al_create_event_queue();

	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	al_install_mouse();
	al_install_keyboard();

	al_init_image_addon();
	al_init_primitives_addon();

	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
	//display = al_create_display(disp_data.width / 2, disp_data.height / 2);

	int r = 100;
	bool switcher = true;

	al_clear_to_color(al_map_rgb(r, 0, 0));
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	set_disp_data = &disp_data;

	return 1;
}


ALLEGRO_BITMAP ** LoadBitmaps(ALLEGRO_DISPLAY * display)
{
	string resourceNames [] = {"Red.png", "Yellow.png", "Magenta.png", "Blue.png", "Cyan.png", "Green.png", "Orange.png"};
	ALLEGRO_BITMAP ** bitmaps = new ALLEGRO_BITMAP * [7];

	for (int i = 0; i < 7; i++)
	{
		bitmaps[i] = ResizeBitmap(al_load_bitmap(resourceNames[i].c_str()), DISPLAY_GRID_WIDTH/GRID_WIDTH + 1, DISPLAY_GRID_HEIGHT/GRID_HEIGHT + 1);

		if(!bitmaps[i]) {
			al_show_native_message_box(display, "Error", "Error", "Failed to load image!", 
				NULL, ALLEGRO_MESSAGEBOX_ERROR);
			al_destroy_display(display);

			//delete resourceNames;
			//delete bitmaps;
			return NULL;
		}
	}

//	delete resourceNames;

	return bitmaps;
}

int PlayGame(ALLEGRO_DISPLAY * display, ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_BITMAP ** bitmaps, ALLEGRO_BITMAP * sideOverlay, ALLEGRO_BITMAP * bottomOverlay)
{
	ALLEGRO_TIMER *timer = NULL;

	timer = al_create_timer(1/FPS);
	if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	ALLEGRO_COLOR backgroundColor = al_map_rgb(0,0,0);
	al_clear_to_color(backgroundColor);

	default_random_engine generator(time(NULL));
	uniform_int_distribution<int> pieceDistribution(0, 6);
	uniform_int_distribution<int> orientationDistribution(0, 3);

	Grid* grid = new Grid(GRID_WIDTH, GRID_HEIGHT);

	PieceTypeEnum tempType = (PieceTypeEnum)pieceDistribution(generator);
	Piece * inFocusPiece = GeneratePiece(grid, tempType, bitmaps[tempType]);

	grid->AddPiece(inFocusPiece);
	//grid->MovePiece(inFocusPiece, 0, 1);

	al_start_timer(timer);
	float time = al_get_timer_count(timer) * 1/FPS;
	float delTime = 0;
	float speeder = 1.5;
	int ctr = 1;
	int corrector = 1;

	while(1)
	{
		ALLEGRO_EVENT ev;
		ALLEGRO_TIMEOUT timeout;

		al_init_timeout(&timeout, 0.001);
		bool get_event = al_wait_for_event_until(event_queue, &ev, &timeout);

		if(get_event && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			al_destroy_timer(timer);
			return 0;
		}

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (inFocusPiece != NULL)
			{
				if (ev.keyboard.keycode == ALLEGRO_KEY_UP)
				{
					if (inFocusPiece->pieceType == T || inFocusPiece->pieceType == J || inFocusPiece->pieceType == L)
					{
						grid->RotatePiece(inFocusPiece, (RotationEnum)(LEFT));
					}
					else if (inFocusPiece->pieceType == O)
					{

					}
					else
					{
						grid->RotatePiece(inFocusPiece, (RotationEnum)(corrector * LEFT));
						corrector *= -1;
					}
				}

				if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
				{
					grid->RotatePiece(inFocusPiece, (RotationEnum)(corrector * LEFT));
					corrector *= -1;
				}

				if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
				{
					grid->MovePieceToEmpty(inFocusPiece, -1, 0);
				}

				if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				{
					grid->MovePieceToEmpty(inFocusPiece, 1, 0);
				}

				if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
				{
					while (grid->MovePieceToEmpty(inFocusPiece, 0, 1)){}

					inFocusPiece->SetInFocus(false);
					int startRow;

					for (int i = 0; i < grid->GetLength(); i++)
					{
						if (grid->GetBlocksAt(i, 2)->size() > 0)
						{
							al_destroy_timer(timer);
							return al_show_native_message_box(display, "GAME OVER", "YOU HAVE LOST", "Play again?", 
								NULL, ALLEGRO_MESSAGEBOX_YES_NO);
						}
					}

					while (DeleteFullRows(grid, startRow))
					{	
						MoveAllDown(grid, startRow);	
						Draw(grid, DISPLAY_GRID_WIDTH, DISPLAY_GRID_HEIGHT, (SCREEN_WIDTH - DISPLAY_GRID_WIDTH) / 2, -1 * DISPLAY_GRID_HEIGHT * (float)(1.0-20.0/22));
						al_draw_bitmap(sideOverlay, 0, 0, 0);
						al_draw_bitmap(sideOverlay, (SCREEN_WIDTH - DISPLAY_GRID_WIDTH) / 2 + 1, 0, 0);
						al_flip_display();
						al_clear_to_color(backgroundColor);
					}
					tempType = (PieceTypeEnum)pieceDistribution(generator);
					//tempType = J;
					inFocusPiece = GeneratePiece(grid, tempType, bitmaps[tempType]);
				}
			}
		}

		float curTime = al_get_timer_count(timer) * 1/FPS;

		if (curTime > 30 * ctr)
		{
			ctr++;
			speeder *= .9;
		}

		delTime += curTime - time;
		time = curTime;

		//Move all pieces in focus down 1 space
		if (delTime > speeder)
		{
			if (!grid->MovePieceToEmpty(inFocusPiece, 0, 1))
			{
				inFocusPiece->SetInFocus(false);
				int startRow;

				for (int i = 0; i < grid->GetLength(); i++)
				{
					if (grid->GetBlocksAt(i, 2)->size() > 0)
					{
						al_destroy_timer(timer);
						return al_show_native_message_box(display, "GAME OVER", "YOU HAVE LOST", "Play again?", 
							NULL, ALLEGRO_MESSAGEBOX_YES_NO);
					}
				}

				while (DeleteFullRows(grid, startRow))
				{	
					MoveAllDown(grid, startRow);	
					Draw(grid, DISPLAY_GRID_WIDTH, DISPLAY_GRID_HEIGHT, (SCREEN_WIDTH - DISPLAY_GRID_WIDTH) / 2, -1 * DISPLAY_GRID_HEIGHT * (float)(1.0-20.0/22));
					al_draw_bitmap(sideOverlay, 0, 0, 0);
					al_draw_bitmap(sideOverlay, (SCREEN_WIDTH - DISPLAY_GRID_WIDTH) / 2 + 1, 0, 0);
					al_flip_display();
					al_clear_to_color(backgroundColor);
				}
				tempType = (PieceTypeEnum)pieceDistribution(generator);
				inFocusPiece = GeneratePiece(grid, tempType, bitmaps[tempType]);
			}

			delTime = 0;
		}

		Draw(grid, DISPLAY_GRID_WIDTH, DISPLAY_GRID_HEIGHT, (SCREEN_WIDTH - DISPLAY_GRID_WIDTH) / 2, -1 * DISPLAY_GRID_HEIGHT * (float)(1.0-20.0/22));
		al_draw_bitmap(sideOverlay, 0, 0, 0);
		al_draw_bitmap(sideOverlay, SCREEN_WIDTH - (SCREEN_WIDTH - DISPLAY_GRID_WIDTH) / 2 + 1, 0, 0);
		al_draw_bitmap(bottomOverlay, 0, DISPLAY_GRID_HEIGHT * (1.0 - (float)(1.0-20.0/22)) - 2, 0);
		al_flip_display();
		al_clear_to_color(backgroundColor);
	}

	al_destroy_timer(timer);
}

int main(int argc, char **argv)
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_DISPLAY_MODE * disp_data = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;

	if (Setup(display, disp_data, event_queue) == -1)
	{
		return -1;
	}
	
	ALLEGRO_BITMAP ** bitmaps = LoadBitmaps(display);
	al_set_target_bitmap(al_get_backbuffer(display));

	if (bitmaps == NULL)
		return 0;

	ALLEGRO_BITMAP * overlay = al_load_bitmap("Overlay.png");
	ALLEGRO_BITMAP * sideOverlay = ResizeBitmap(overlay, (SCREEN_WIDTH - DISPLAY_GRID_WIDTH) / 2 + 1, DISPLAY_GRID_HEIGHT + 2);
	ALLEGRO_BITMAP * bottomOverlay = ResizeBitmap(overlay, SCREEN_WIDTH, SCREEN_HEIGHT);
	al_set_target_bitmap(al_get_backbuffer(display));

	while (PlayGame(display, event_queue, bitmaps, sideOverlay, bottomOverlay) == 1)

	al_uninstall_mouse();
	al_uninstall_keyboard();

	for (int i = 0; i < 7; i++)
	{
		al_destroy_bitmap(bitmaps[i]);
	}

	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

