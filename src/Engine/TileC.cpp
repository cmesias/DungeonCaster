/*
 * TileC.cpp
 *
 *  Created on: Jul 4, 2017
 *      Author: Carl
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <fstream>
#include <sstream>

#include "TileC.h"



void TileC::init(TileC tilec[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		tilec[i].w = 300;
		tilec[i].h = 300;
		tilec[i].type = 0;
		tilec[i].layer = -1;
		tilec[i].mouse = false;
		tilec[i].mouseBox = false;
		tilec[i].screen = false;
		tilec[i].alive = false;
		tilec[i].health = 100;
	}
}

void TileC::place(TileC tilec[], float x, float y, int w, int h) {
	for (int i = 0; i < max; i++) {
		if (!tilec[i].alive){
			tilec[i].x 		= x;
			tilec[i].y 		= y;
			if (type == 0) {
				tilec[i].w 		= 16;
				tilec[i].h 		= 13;
			}
			if (type == 1) {
				tilec[i].w 		= 16;
				tilec[i].h 		= 16;
			}
			if (type == 2) {
				tilec[i].w 		= 16;
				tilec[i].h 		= 13;
			}
			tilec[i].type 	= type;
			tilec[i].layer 	= layer;
			tilec[i].mouse 	= false;
			tilec[i].mouseBox = false;
			tilec[i].screen = false;
			tilec[i].alive 	= true;
			tilec[i].health = 100;
			count++;
			break;
		}
	}
}

void TileC::spawn(TileC tilec[], int newMx, int newMy, int camx, int camy) {
	remove(tilec, 0);
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			int x = int(newMx + j * tilew+camx);
			int y = int(newMy + h * tileh+camy);
			place(tilec, x, y, tilew, tileh);
		}
	}
}

void TileC::copy(TileC tilec[]) {
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			if (tilec[i].mouse){
				if (tilec[i].layer == layer) {
					type = tilec[i].type;
					tilew = tilec[i].w;
					tileh = tilec[i].h;
				}
			}
		}
	}
}

void TileC::remove(TileC tilec[], int click) {
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			// Left click
			if (click == 0) {
				if (tilec[i].mouseBox){
					if (tilec[i].layer == layer) {
						tilec[i].alive = false;
						count--;
					}
				}
			// Right click
			}else{
				if (tilec[i].mouse){
					if (tilec[i].layer == layer) {
						tilec[i].alive = false;
						count--;
					}
				}
			}
		}
	}
}

void TileC::RemoveAll(TileC tilec[]) {
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			tilec[i].alive = false;
		}
	}
	count = 0;
}


bool TileC::checkCollisionRect( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA,   leftB;
    int rightA,  rightB;
    int topA, 	 topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA 	= a.x;
    rightA 	= a.x + a.w;
    topA 	= a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB 	= b.x;
    rightB 	= b.x + b.w;
    topB 	= b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA < topB )
    {
        return false;
    }

    if( topA > bottomB )
    {
        return false;
    }

    if( rightA < leftB )
    {
        return false;
    }

    if( leftA > rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void TileC::checkCollision(TileC tilec[], float x, float y, int w, int h, float &targetAxis, float &targetVelocity){
	SDL_Rect rectA;
	rectA.x = x;
	rectA.y = y;
	rectA.w = w;
	rectA.h = h;

	bool moveBack;
	moveBack = false;

	for (int i = 0; i < 2096; i++) {
		if (tilec[i].alive){
			SDL_Rect rectB;
			rectB.x = tilec[i].x;
			rectB.y = tilec[i].y;
			rectB.w = tilec[i].w;
			rectB.h = tilec[i].h;
			if  ( checkCollisionRect( rectA, rectB )) {
				moveBack = true;
			}
		}
	}

	if (moveBack){
    	targetAxis -= targetVelocity;
	}
}

void TileC::update(TileC tilec[], int newMx, int newMy, int mex, int mey, int camx, int camy, int screenWidth, int screenHeight) {
	int tileW = tilew*multiW;
	int tileH = tileh*multiH;
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			//If the tilec is in the screen
			if (tilec[i].x + tilec[i].w > camx-64 && tilec[i].x < camx + screenWidth+64
			 && tilec[i].y + tilec[i].h > camy-64 && tilec[i].y < camy + screenHeight+64) {
				tilec[i].screen = true;
			} else {
				tilec[i].screen = false;
			}
			//If the mouse+size is on the tilec
			if (newMx+tileW-2 > tilec[i].x && newMx+1 < tilec[i].x + tilec[i].w &&
				newMy+tileH-2 > tilec[i].y && newMy+1 < tilec[i].y + tilec[i].h) {
				tilec[i].mouseBox = true;
			} else {
				tilec[i].mouseBox = false;
			}
			//If the mouse is on the tilec
			if (mex > tilec[i].x && mex < tilec[i].x + tilec[i].w &&
				mey > tilec[i].y && mey < tilec[i].y + tilec[i].h) {
				tilec[i].mouse = true;
			} else {
				tilec[i].mouse = false;
			}
		}
	}
}

void TileC::render(SDL_Renderer *gRenderer, TileC tilec[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			// Player hits this
			if (tilec[i].type == 0) {
				SDL_Rect tempr = {tilec[i].x - camx, tilec[i].y - camy, 4, 4};
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
				SDL_RenderFillRect(gRenderer, &tempr);
			}
			// Particles hit this
			if (tilec[i].type == 1) {
				SDL_Rect tempr = {tilec[i].x+4 - camx, tilec[i].y - camy, 4, 4};
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
				SDL_RenderFillRect(gRenderer, &tempr);
			}
			// Other's hit this
			if (tilec[i].type == 2) {
				SDL_Rect tempr = {tilec[i].x+8 - camx, tilec[i].y - camy, 4, 4};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
				SDL_RenderFillRect(gRenderer, &tempr);
			}
			// Tile border
			SDL_Rect tempr = {tilec[i].x - camx, tilec[i].y - camy, 16, 16};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);

			if (tilec[i].mouseBox) {
				SDL_Rect tempr = {tilec[i].x+tilec[i].w-4 - camx, tilec[i].y+ - camy, 4, 4};
				SDL_SetRenderDrawColor(gRenderer, 244, 144, 25, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}
	}
}

//void TileC::renderDebug(SDL_Renderer *gRenderer, TileC tilec[], int newMx, int newMy, int camx, int camy){

//

void TileC::loadTiles(TileC tilec[], int level){
	//Load Tile
	count = 0;
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive) {
			tilec[i].alive = false;
		}
	}

	// Open Level File
	std::stringstream fileName;
	fileName << "resource/data/maps/";
	fileName << "level" << level;
	fileName << "/TileC.mp";
	std::fstream fileTileDataL(fileName.str().c_str());
	// Read first line for Tile Count
	fileTileDataL >> count;
	// Read the rest of the lines for Tile datas
	while( fileTileDataL.good() ){
		for (int i = 0; i < max; i++) {
			if (!tilec[i].alive) {
				fileTileDataL >> tilec[i].x 		>>
								  tilec[i].y 		>>
								  tilec[i].w 		>>
								  tilec[i].h 		>>
								  tilec[i].type 	>>
								  tilec[i].layer 	>>
								  tilec[i].mouse 	>>
								  tilec[i].screen 	>>
								  tilec[i].alive;
				break;
			}
		}
	}
	fileTileDataL.close();
}

std::string TileC::saveTiles(TileC tilec[]){
	// Create new file to store Tile data
	std::ofstream tileDataFile;
	// Create stringstream to store Tile Data
	std::stringstream tempData;
	// Write number of Tiles on first line
	tempData << count << "\n";
	// Write the rest of the tilec Data after the first line
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive) {
			tempData << tilec[i].x 			<< " "
					 << tilec[i].y 			<< " "
					 << tilec[i].w  		<< " "
					 << tilec[i].h  		<< " "
					 << tilec[i].type  		<< " "
					 << tilec[i].layer 		<< " "
					 << tilec[i].mouse 		<< " "
					 << tilec[i].screen 	<< " "
			   	   	 << tilec[i].alive 		<< "\n";
		}
	}
	return tempData.str().c_str();
}
