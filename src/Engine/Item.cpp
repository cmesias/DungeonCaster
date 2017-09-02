/*
 * Item.cpp
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

#include "Item.h"

void Item::Init(Item item[]){
	count = 0;
	max = 100;
	for (int i = 0; i < max; i++) {
		item[i].hoverAmount 	= 9;
		item[i].hoverDir 		= 1;
		item[i].alive 			= false;
	}
}

void Item::Load(SDL_Renderer* gRenderer){
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
	gItem.loadFromFile(gRenderer, "resource/gfx/By Scott Matott/torch_key_gems.png");
}

void Item::Free(){
	gItem.free();
}

void Item::Remove(Item item[], int click) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive){
			// Left click
			if (click == 0) {
				if (item[i].mouseBox){
					item[i].alive = false;
					count--;
				}
			// Right click
			}else{
				if (item[i].mouse){
					item[i].alive = false;
					count--;
				}
			}
		}
	}
}

void Item::RemoveAll(Item item[]) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive){
			item[i].alive = false;
		}
	}
	count = 0;
}

void Item::Spawn(Item item[], float x, float y, float w , float h) {
	Remove(item, 0);
	for (int i = 0; i < max; i++) {
		if (!item[i].alive) {
			item[i].x 				= x;
			item[i].y 				= y;
			item[i].w 				= w;
			item[i].h 				= h;
			item[i].vX 				= 0.0;
			item[i].vY 				= 0.0;
			item[i].id 				= id;
			item[i].collision 		= false;
			item[i].mouse 			= false;
			item[i].mouseBox 			= false;
			item[i].onScreen 			= false;
			item[i].alive 			= true;
			count++;
			break;
		}
	}
}

void Item::Spawn(Item item[], float x, float y, float w , float h, int id) {
	Remove(item, 0);
	for (int i = 0; i < max; i++) {
		if (!item[i].alive) {
			item[i].x 				= x;
			item[i].y 				= y;
			item[i].w 				= w;
			item[i].h 				= h;
			item[i].vX 				= 0.0;
			item[i].vY 				= 0.0;
			item[i].id 				= id;
			item[i].collision 		= false;
			item[i].mouse 			= false;
			item[i].mouseBox 			= false;
			item[i].onScreen 			= false;
			item[i].alive 			= true;
			count++;
			break;
		}
	}
}

void Item::Update(Item item[], int mouseX, int mouseY, int mx, int my, int camx, int camy) {
	int tileW = 16*1;
	int tileH = 16*1;
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {
			// center of item
			item[i].x2 = item[i].x+item[i].w/2;
			item[i].y2 = item[i].y+item[i].h/2;

			// item target
			/*float bmx2 = item[i].x+item[i].w/2;
			float bmy2 = item[i].y+item[i].h/2;
			float bmx  = player.x2+player.radius;
			float bmy  = player.y2+player.radius;
			// item distance from target
			float distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
			if (distance <= 0.1) {
				distance = 0.1;
			}
			item[i].angle = atan2(bmy - bmy2,bmx - bmx2);
			item[i].angle = item[i].angle * (180 / 3.1416);

			if (item[i].angle < 0) {
				item[i].angle = 360 - (-item[i].angle);
			}*/


			//-----------------------------------------------------------------------------------//

			/* NOTE: we create a different constant variable
			 * for the Item for it to follow the Player.
			 * We create another variable specifically for knocking back the Item.
			 */

			// item movement
			item[i].x += item[i].vX;
			item[i].y += item[i].vY;

			// velocity decay
			//item[i].vX = item[i].vX - item[i].vX * 0.08;
			//item[i].vY = item[i].vY - item[i].vY * 0.08;

			// item screen check
			if (item[i].x + item[i].w >= camx && item[i].x <= camx + screenWidth &&
				item[i].y + item[i].h >= camy && item[i].y <= camy + screenHeight) {
				item[i].onScreen = true;
			}else{
				item[i].onScreen = false;
			}
			//If the mouse+size is on the tile
			if (mouseX+tileW-2 > item[i].x && mouseX+1 < item[i].x + item[i].w &&
					mouseY+tileH-2 > item[i].y && mouseY+1 < item[i].y + item[i].h) {
				item[i].mouseBox = true;
			} else {
				item[i].mouseBox = false;
			}
			//If the mouse is on the tile
			if (mx > item[i].x && mx < item[i].x + item[i].w &&
				my > item[i].y && my < item[i].y + item[i].h) {
				item[i].mouse = true;
			} else {
				item[i].mouse = false;
			}

			// item circle collision check with other items
			/*for (int j = 0; j < max; j++) {
				if (i !=j) {
					if (item[j].alive) {
						float bmx = item[j].x+item[j].w/2;
						float bmy = item[j].y+item[j].h/2;
						float bmx2 = item[i].x+item[i].w/2;
						float bmy2 = item[i].y+item[i].h/2;
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
						if (distance < item[i].w/2 + item[j].w/2) {
							item[i].x -= 1 * Cos;
							item[i].y -= 1 * Sin;
						}
					}
				}
			}*/
		}
	}
}

//Render item
void Item::Render(SDL_Renderer* gRenderer, Item item[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {
			gItem.setAlpha(255);
			float newY = item[i].y;
			if (item[i].id == 0) {
				item[i].hoverAmount += 0.1 * item[i].hoverDir;
				if (item[i].hoverAmount > 4) {
					item[i].hoverDir = -1;
				}
				else if (item[i].hoverAmount < 1) {
					item[i].hoverDir = 1;
				}

				newY = item[i].y - item[i].hoverAmount;
			}

			gItem.render(gRenderer, item[i].x - camx, newY - camy, item[i].w, item[i].h, &rClips[item[i].id]);
			/*if (item[i].mouse) {
				SDL_Rect tempr = {item[i].x+1 - camx, item[i].y+1 - camy, item[i].w-2, item[i].h-2};
				SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
			if (item[i].mouseBox) {
				SDL_Rect tempr = {item[i].x+2 - camx, item[i].y+2 - camy, item[i].w-4, item[i].h-4};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}*/
		}
	}
}



void Item::LoadData(Item item[], int level){
	//Load Tile
	count = 0;
	for (int i = 0; i < max; i++) {
		item[i].collision = false;
		item[i].mouse = false;
		item[i].mouseBox = false;
		item[i].onScreen = false;
		item[i].alive = false;
	}

	// Open Level File
	std::stringstream fileName;
	fileName << "resource/data/maps/";
	fileName << "level" << level;
	fileName << "/Item.mp";
	std::fstream fileTileDataL(fileName.str().c_str());
	// Read first line for Tile Count
	fileTileDataL >> count;
	// Read the rest of the lines for Tile datas
	while( fileTileDataL.good() ){
		for (int i = 0; i < max; i++) {
			if (!item[i].alive) {
				fileTileDataL >>  item[i].x 		>>
								  item[i].y 		>>
								  item[i].w 		>>
								  item[i].h 		>>
								  item[i].id 		>>
								  item[i].alive;
				break;
			}
		}
	}
	fileTileDataL.close();
}

std::string Item::SaveData(Item item[]){
	// Create new file to store Tile data
	std::ofstream tileDataFile;
	// Create stringstream to store Tile Data
	std::stringstream tempData;
	// Write number of Tiles on first line
	tempData << count << "\n";
	// Write the rest of the tilec Data after the first line
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {
			tempData << item[i].x 		<< " "
					 << item[i].y 		<< " "
					 << item[i].w  		<< " "
					 << item[i].h  		<< " "
					 << item[i].id  		<< " "
			   	   	 << item[i].alive 	<< "\n";
		}
	}
	return tempData.str().c_str();
}
