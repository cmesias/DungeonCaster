/*
 * Object.cpp
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#include "LTexture.h"

#include <fstream>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Object.h"

void Object::Init(Object object[]){
	count = 0;
	max = 100;
	for (int i = 0; i < max; i++) {
		object[i].alive 			= false;
	}
}

void Object::Load(SDL_Renderer* gRenderer){
	int i = 0;
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			rClips[i].x = 0 + w * 16;
			rClips[i].y = 0 + h * 16;
			rClips[i].w = 16;
			rClips[i].h = 16;
			i++;
		}
	}
	gObject.loadFromFile(gRenderer, "resource/gfx/torch_key_gems.png");
}

void Object::Free(){
	gObject.free();
}

void Object::Remove(Object object[], int click) {
	for (int i = 0; i < max; i++) {
		if (object[i].alive){
			// Left click
			if (click == 0) {
				if (object[i].mouseBox){
					object[i].alive = false;
					count--;
				}
			// Right click
			}else{
				if (object[i].mouse){
					object[i].alive = false;
					count--;
				}
			}
		}
	}
}

void Object::RemoveAll(Object object[]) {
	for (int i = 0; i < max; i++) {
		if (object[i].alive){
			object[i].alive = false;
		}
	}
	count = 0;
}

void Object::Spawn(Object object[], float x, float y, float w , float h) {
	Remove(object, 0);
	for (int i = 0; i < max; i++) {
		if (!object[i].alive) {
			object[i].x 				= x;
			object[i].y 				= y;
			object[i].w 				= w;
			object[i].h 				= h;
			object[i].vX 				= 0.0;
			object[i].vY 				= 0.0;
			object[i].id 				= id;
			object[i].collision 		= false;
			object[i].mouse 			= false;
			object[i].mouseBox 			= false;
			object[i].onScreen 			= false;
			object[i].alive 			= true;
			count++;
			break;
		}
	}
}

void Object::Update(Object object[], int mouseX, int mouseY, int mx, int my, int camx, int camy) {
	int tileW = 16*1;
	int tileH = 16*1;
	for (int i = 0; i < max; i++) {
		if (object[i].alive) {
			// center of object
			object[i].x2 = object[i].x+object[i].w/2;
			object[i].y2 = object[i].y+object[i].h/2;

			// object target
			/*float bmx2 = object[i].x+object[i].w/2;
			float bmy2 = object[i].y+object[i].h/2;
			float bmx  = player.x2+player.radius;
			float bmy  = player.y2+player.radius;
			// object distance from target
			float distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
			if (distance <= 0.1) {
				distance = 0.1;
			}
			object[i].angle = atan2(bmy - bmy2,bmx - bmx2);
			object[i].angle = object[i].angle * (180 / 3.1416);

			if (object[i].angle < 0) {
				object[i].angle = 360 - (-object[i].angle);
			}*/


			//-----------------------------------------------------------------------------------//

			/* NOTE: we create a different constant variable
			 * for the Object for it to follow the Player.
			 * We create another variable specifically for knocking back the Object.
			 */

			// object movement
			object[i].x += object[i].vX;
			object[i].y += object[i].vY;

			// velocity decay
			//object[i].vX = object[i].vX - object[i].vX * 0.08;
			//object[i].vY = object[i].vY - object[i].vY * 0.08;

			// object screen check
			if (object[i].x + object[i].w >= camx && object[i].x <= camx + screenWidth &&
				object[i].y + object[i].h >= camy && object[i].y <= camy + screenHeight) {
				object[i].onScreen = true;
			}else{
				object[i].onScreen = false;
			}
			//If the mouse+size is on the tile
			if (mouseX+tileW-2 > object[i].x && mouseX+1 < object[i].x + object[i].w &&
					mouseY+tileH-2 > object[i].y && mouseY+1 < object[i].y + object[i].h) {
				object[i].mouseBox = true;
			} else {
				object[i].mouseBox = false;
			}
			//If the mouse is on the tile
			if (mx > object[i].x && mx < object[i].x + object[i].w &&
				my > object[i].y && my < object[i].y + object[i].h) {
				object[i].mouse = true;
			} else {
				object[i].mouse = false;
			}

			// object circle collision check with other objects
			/*for (int j = 0; j < max; j++) {
				if (i !=j) {
					if (object[j].alive) {
						float bmx = object[j].x+object[j].w/2;
						float bmy = object[j].y+object[j].h/2;
						float bmx2 = object[i].x+object[i].w/2;
						float bmy2 = object[i].y+object[i].h/2;
						float angle = atan2(bmy - bmy2,bmx - bmx2);
						angle = angle * (180 / 3.1416);
						if (angle < 0) {
							angle = 360 - (-angle);
						}
						float radians = (3.1415926536/180)*(angle);
						float Cos = floor(cos(radians)*10+0.5)/10;
						float Sin = floor(sin(radians)*10+0.5)/10;
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));
						if (distance <= 1) {
							distance = 1;
						}
						if (distance < object[i].w/2 + object[j].w/2) {
							object[i].x -= 1 * Cos;
							object[i].y -= 1 * Sin;
						}
					}
				}
			}*/
		}
	}
}

//Render object
void Object::Render(SDL_Renderer* gRenderer, Object object[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (object[i].alive) {
			gObject.setAlpha(255);
			gObject.setBlendMode(SDL_BLENDMODE_ADD);
			gObject.render(gRenderer, object[i].x - camx, object[i].y - camy, object[i].w, object[i].h, &rClips[object[i].id]);
			if (object[i].mouse) {
				SDL_Rect tempr = {object[i].x+1 - camx, object[i].y+1 - camy, object[i].w-2, object[i].h-2};
				SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
			if (object[i].mouseBox) {
				SDL_Rect tempr = {object[i].x+2 - camx, object[i].y+2 - camy, object[i].w-4, object[i].h-4};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}
	}
}



void Object::loadTiles(Object object[], int level){
	//Load Tile
	count = 0;
	for (int i = 0; i < max; i++) {
		object[i].collision = false;
		object[i].mouse = false;
		object[i].mouseBox = false;
		object[i].onScreen = false;
		object[i].alive = false;
	}

	// Open Level File
	std::stringstream fileName;
	fileName << "resource/data/maps/";
	fileName << "level" << level;
	fileName << "/Items.mp";
	std::fstream fileTileDataL(fileName.str().c_str());
	// Read first line for Tile Count
	fileTileDataL >> count;
	// Read the rest of the lines for Tile datas
	while( fileTileDataL.good() ){
		for (int i = 0; i < max; i++) {
			if (!object[i].alive) {
				fileTileDataL >>  object[i].x 		>>
								  object[i].y 		>>
								  object[i].w 		>>
								  object[i].h 		>>
								  object[i].id 		>>
								  object[i].alive;
				break;
			}
		}
	}
	fileTileDataL.close();
}

std::string Object::saveTiles(Object object[]){
	// Create new file to store Tile data
	std::ofstream tileDataFile;
	// Create stringstream to store Tile Data
	std::stringstream tempData;
	// Write number of Tiles on first line
	tempData << count << "\n";
	// Write the rest of the tilec Data after the first line
	for (int i = 0; i < max; i++) {
		if (object[i].alive) {
			tempData << object[i].x 		<< " "
					 << object[i].y 		<< " "
					 << object[i].w  		<< " "
					 << object[i].h  		<< " "
					 << object[i].id  		<< " "
			   	   	 << object[i].alive 	<< "\n";
		}
	}
	return tempData.str().c_str();
}
