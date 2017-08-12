/*
 * CustomizeCharacter.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: Carl
 */

#include "LWindow.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "CustomizeCharacter.h"

void CustomizeCharacter::load(SDL_Renderer *gRenderer) {
	////////////////////////////////////////////////////////////////////////////////////////////////
	//---------------------------------------- Initialize ----------------------------------------//
	// Create RGB bars
	skin.init("Skin", 48, 12);
	eyes.init("Eyes", 48, 12);
	hair.init("Hair", 48, 12);
	shirt.init("Shirt", 48, 12);
	pants.init("Pants", 48, 12);
	// open config file
	std::stringstream tempss;
	// set default directory
	tempss << defDir;
	// set filename
	tempss << "cc.cfg";
	loadCFG(tempss.str().c_str(), eyes.x, eyes.y, 1);
	loadCFG(tempss.str().c_str(), skin.x, skin.y, 2);
	loadCFG(tempss.str().c_str(), hair.x, hair.y, 3);
	loadCFG(tempss.str().c_str(), shirt.x, shirt.y, 4);
	loadCFG(tempss.str().c_str(), pants.x, pants.y, 5);

	// Buttons
	buttonName[0] = "BACK";
	buttonName[1] = "SAVE";
	buttonName[2] = "START";

	// Save index
	index = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////
	//------------------------------------------- Load -------------------------------------------//

	// Fonts
	gFont12 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 8);
	gFont18 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 18);

	// Load Texture scenes
	//Load texture target
	if( !gBlank.createBlank( gRenderer, 144, 144, SDL_TEXTUREACCESS_TARGET ) )
	{
		printf( "Failed to create target texture!\n" );
	}
	if( !gScene.createBlank( gRenderer, 270, 152, SDL_TEXTUREACCESS_TARGET ) )
	{
		printf( "Failed to create target texture!\n" );
	}
	gBG.loadFromFile(gRenderer, "resource/gfx/bg-menu.png");

	gButtons.loadFromFile(gRenderer, "resource/gfx/cc-buttons.png");
	for (int i=0; i<3; i++) {
		bClip[i].x = 0;
		bClip[i].y = 0+i*12;
		bClip[i].w = 48;
		bClip[i].h = 12;
	}

	int i = 0;
	for (int h=0; h<3; h++) {
		for (int w=0; w<3; w++) {
			clip[i].x = 0+w*48;
			clip[i].y = 0+h*48;
			clip[i].w = 48;
			clip[i].h = 48;
			i++;
		}
	}

	// Load Player Textures
	gPlayer[0].loadFromFile(gRenderer, "resource/gfx/player/player_skin.png");
	gPlayer[1].loadFromFile(gRenderer, "resource/gfx/player/player_eyes.png");
	gPlayer[2].loadFromFile(gRenderer, "resource/gfx/player/player_hair.png");
	gPlayer[3].loadFromFile(gRenderer, "resource/gfx/player/player_shirt.png");
	gPlayer[4].loadFromFile(gRenderer, "resource/gfx/player/player_pants.png");
	gPlayerOutline.loadFromFile(gRenderer, "resource/gfx/player/player_outline.png");

	// Load Player colors
	SDL_Color color = {255,255,255};
	// open config file
	std::stringstream dir;
	// set default directory
	dir << defDir;
	// set filename
	dir << "cc.cfg";

	// Skin color
	color = loadColor(dir.str().c_str(), 6);
	skin.color[0] = color.r;
	skin.color[1] = color.g;
	skin.color[2] = color.b;

	// Eyes color
	color = loadColor(dir.str().c_str(), 7);
	eyes.color[0] = color.r;
	eyes.color[1] = color.g;
	eyes.color[2] = color.b;

	// Hair color
	color = loadColor(dir.str().c_str(), 8);
	hair.color[0] = color.r;
	hair.color[1] = color.g;
	hair.color[2] = color.b;

	// Eyes color
	color = loadColor(dir.str().c_str(), 9);
	shirt.color[0] = color.r;
	shirt.color[1] = color.g;
	shirt.color[2] = color.b;

	// Eyes color
	color = loadColor(dir.str().c_str(), 10);
	pants.color[0] = color.r;
	pants.color[1] = color.g;
	pants.color[2] = color.b;

	//initTexture(gTileBorder, 32, 32, white, 110);
	// Load Menu starting coordinates
	/*std::ifstream fileTileNumL("data/cfg/menu.cfg");
	if (fileTileNumL.is_open()) {
		fileTileNumL >> x >> y >>
					 w >> h>>
					 mW >> mH;
	}*/
}


void CustomizeCharacter::save() {
	// Load Menu starting coordinates
	/*std::ofstream fileTileNumL("data/cfg/menu.cfg");
	if (fileTileNumL.is_open()) {
		fileTileNumL << x  << " " <<
					    y  << " " <<
						w  << " " <<
						h  << " " <<
						mW << " " <<
						mH;
	}*/
}

void CustomizeCharacter::free() {
	// Free resources
	gBlank.free();
	gScene.free();
	gBG.free();
	gButtons.free();
	gPlayer[0].free();
	gPlayer[1].free();
	gPlayer[2].free();
	gPlayer[3].free();
	gPlayer[4].free();
	gPlayerOutline.free();
	gText.free();
	TTF_CloseFont(gFont12);
	TTF_CloseFont(gFont18);
	gFont12 = NULL;
	gFont18 = NULL;
}

void CustomizeCharacter::Show(LWindow &gWindow, SDL_Renderer *gRenderer, CustomizeCharacter::Result &result) {

	// Upon entry
	quit = false;
	leftClick = false;
	shift = false;

	// Load resources
	load(gRenderer);

	// While loop
	while (!quit) {

		// Set buttons, Quit Save Character, and Start Game
		w = 48;
		h = 16;
		x = 72;
		y = screenHeight - h - 2;
		button[0].x = screenWidth/2-48/2 - 48;	// Quit
		button[1].x = screenWidth/2-48/2;		// Save Character
		button[2].x = screenWidth/2-48/2 + 48;	// Start Game

		for (int i=0; i<3; i++) {
			button[i].w = w;
			button[i].h = h;
			button[i].y = y;
		}

		// Get mouse position
		SDL_GetMouseState(&mx, &my);

		// Grid like coordinates of mouse
		mex = (screenWidth*mx)/gWindow.getWidth();				// New mouse coordinates, no relation to camera
		mey = (screenHeight*my)/gWindow.getHeight();			// New mouse coordinates, no relation to camera

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Back Game
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				free();
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_s: {

						//SDL_Surface* loadedSurface = IMG_Load("player.png");
						//SDL_SaveBMP(loadedSurface, "temp.bmp");


						//SDL_Surface *ss = SDL_CreateRGBSurface(0, 700, 700, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
						/*std::string test = "resource/player.png";
						SDL_Surface* loadedSurface = IMG_Load(test.c_str());
						IMG_SavePNG(loadedSurface, "temp.png");*/
						//SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ABGR8888, ss, 0 );
						//SDL_SaveBMP(loadedSurface, "test.bmp");
						/*std::string test = "resource/player.png";
						SDL_Surface* loadedSurface = IMG_Load(test.c_str());
						std::stringstream testt;
						testt << IMG_Load(test.c_str());

						std::ofstream fileS;
						fileS.open("test.png");
						fileS << testt.str().c_str();
						fileS.close();

						std::ofstream fileS2;
						fileS2.open("testSTR.png");
						fileS2 << loadedSurface;
						fileS2.close();*/
						break;
					}
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
						break;
					case SDLK_q:		// save-index decrease
						index++;
						if (index > 4)
							index = 0;
						break;
					case SDLK_x:
						if (index==0) {									// Save eye-color position
							skin.x = mex;
							skin.y = mey;
						}else if (index==1) {							// Save body-color position
							eyes.x = mex;
							eyes.y = mey;
						}else if (index==2) {							// Save hair-color position
							hair.x = mex;
							hair.y = mey;
						}else if (index==3) {							// Save hair-color position
							shirt.x = mex;
							shirt.y = mey;
						}else if (index==4) {							// Save hair-color position
							pants.x = mex;
							pants.y = mey;
						}
						break;
					}
				}
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					}
				}

				// Mouse Pressed
				mousePressed(event);

				// Mouse Released
				result = mouseReleased(event);

				// Customize Character results
				switch (result)  {
					case Back:				// Back (to Main Menu)
						quit = true;
						break;
					case Nothing:			// Nothing

						break;
					case StartGame:			// Start Game
						quit = true;
						break;
				}
			}
		}

		// Update Variables
		skin.update(leftClick, mex, mey);
		eyes.update(leftClick, mex, mey);
		hair.update(leftClick, mex, mey);
		shirt.update(leftClick, mex, mey);
		pants.update(leftClick, mex, mey);



		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		gBlank.setAsRenderTarget(gRenderer);
		//Clear render screen, white color
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
		SDL_RenderClear(gRenderer);
			// Render Player Texture
			SDL_Rect rect = {192, 128, 64, 64};
			// Outline
			gPlayerOutline.setColor(255,255,255);
			gPlayerOutline.render(gRenderer, 0, 0, 144, 144, &rect);
			// Skin
			gPlayer[0].setColor(skin.color[0], skin.color[1], skin.color[2]);
			gPlayer[0].render(gRenderer, 0, 0, 144, 144, &rect);
			// Eyes
			gPlayer[1].setColor(eyes.color[0], eyes.color[1], eyes.color[2]);
			gPlayer[1].render(gRenderer, 0, 0, 144, 144, &rect);
			// Hair
			gPlayer[2].setColor(hair.color[0], hair.color[1], hair.color[2]);
			gPlayer[2].render(gRenderer, 0, 0, 144, 144, &rect);
			// Shirt
			gPlayer[3].setColor(shirt.color[0], shirt.color[1], shirt.color[2]);
			gPlayer[3].render(gRenderer, 0, 0, 144, 144, &rect);
			// Pants
			gPlayer[4].setColor(pants.color[0], pants.color[1], pants.color[2]);
			gPlayer[4].render(gRenderer, 0, 0, 144, 144, &rect);
		//Reset render target
		SDL_SetRenderTarget( gRenderer, NULL );
		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		gScene.setAsRenderTarget(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);

			// Render everything
			Render(gRenderer);

		SDL_SetRenderTarget( gRenderer, NULL );
		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////


		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Render Scene
			gScene.render(gRenderer,0,0,270,152);

			// Render Text
			RenderText(gRenderer);

		// Update screen
		SDL_RenderPresent(gRenderer);
	}
	// Free everything
	free();
}


// Mouse Pressed
CustomizeCharacter::Result CustomizeCharacter::mousePressed(SDL_Event event){
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return Nothing;
}

// Mouse Released
CustomizeCharacter::Result CustomizeCharacter::mouseReleased(SDL_Event event){
	CustomizeCharacter::Result result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
			// Perform actions
			for (int i=0; i<3; i++) {
				// Check Mouse hover w/ Menu item
				SDL_Rect a = {mex, mey, 1, 1};
				SDL_Rect b = {button[i].x, button[i].y, button[i].w, button[i].h};
				// If mouse is hovering over menu item then render specifically
				if (checkCollision(a, b)) {
					if (i == 0) {			// Back
						result = Back;
					}else if (i == 1) {		// Save Character
						// function here to save customized character to file
						result = Nothing;
						/////////////////////////////////////////////////////////////////////
						// Final data to save
						std::stringstream tempss;
						// File path
						std::stringstream dir;
						// set default directory
						dir << defDir;
						// set filename
						dir << "cc.cfg";
						/////////////////////////////////////////////////////////////////////
						tempss << skin.x << " " << skin.y << "\n";
						tempss << eyes.x << " " << eyes.y << "\n";
						tempss << hair.x << " " << hair.y << "\n";
						tempss << shirt.x << " " << shirt.y << "\n";
						tempss << pants.x << " " << pants.y << "\n";
						/////////////////////////////////////////////////////////////////////
						tempss << skin.color[0] << " " << skin.color[1] << " " << skin.color[2] << "\n";
						tempss << eyes.color[0] << " " << eyes.color[1] << " " << eyes.color[2] << "\n";
						tempss << hair.color[0] << " " << hair.color[1] << " " << hair.color[2] << "\n";
						tempss << shirt.color[0] << " " << shirt.color[1] << " " << shirt.color[2] << "\n";
						tempss << pants.color[0] << " " << pants.color[1] << " " << pants.color[2] << "\n";
						/////////////////////////////////////////////////////////////////////
						// Overwrite File
						std::ofstream fileS;
						fileS.open(dir.str().c_str());
						fileS << tempss.str().c_str();
						fileS.close();
					}else if (i == 2) {		// Start Game
						result = StartGame;
					}
				}
			}
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return result;
}

bool CustomizeCharacter::checkCollision(SDL_Rect a, SDL_Rect b){
	bool collide;
	if (a.x+a.w > b.x && a.x < b.x + b.w &&
		a.y+a.h > b.y && a.y < b.y + b.h){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

void CustomizeCharacter::Render(SDL_Renderer *gRenderer) {

}

void CustomizeCharacter::RenderText(SDL_Renderer *gRenderer) {
	// render background
	gBG.render(gRenderer, 0, 0, screenWidth, screenHeight);

	//////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------//
	//---------------------------------- Buttons -------------------------------//
	// Render Quit, Save and Start Buttons
	SDL_Color color = {255,255,255};
	for (int i=0; i<3; i++) {
		if (mex+1 >= button[i].x && mex <= button[i].x+button[i].w &&
			mey+1 >= button[i].y && mey <= button[i].y+button[i].h) {
			// Mouse click, hover
			if (leftClick) {
				color = {244,144,25};
			}
			// No mouse click, hover
			else{
				color = {0,255,0};
			}
		}else{
			// No mouse click, no hover
			color = {255,255,255};
		}
		//gButtons.render(gRenderer, button[i].x, button[i].y, button[i].w, button[i].h, &bClip[i]);
		SDL_Rect tempRect = {button[i].x, button[i].y, button[i].w, button[i].h};
		SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);
	}

	// Render bars
	skin.Render(gRenderer);
	eyes.Render(gRenderer);
	hair.Render(gRenderer);
	shirt.Render(gRenderer);
	pants.Render(gRenderer);
	//---------------------------------- Buttons -------------------------------//
	//--------------------------------------------------------------------------//
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------//
	//------------------------------------ Text --------------------------------//
	// Render buttons
	for (int i=0; i<3; i++) {
		std::stringstream tempss;
		tempss << buttonName[i];
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont18);
		int newWidth = gText.getWidth()/4;
		int newHeight = gText.getHeight()/4;
		gText.setAlpha(255);
		gText.render(gRenderer, button[i].x+button[i].w/2 - newWidth/2,
				                button[i].y+button[i].h/2 - newHeight/2,
								newWidth, newHeight);

	}

	/*// Render text, color-part index
	std::stringstream tempss;
		std::string tempS[5];
		tempS[0] = "Skin";
		tempS[1] = "Eyes";
		tempS[2] = "Hair";
		tempS[3] = "Shirt";
		tempS[4] = "Pants";
		tempss << tempS[index];
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont18);
		int newWidth = gText.getWidth()/4;
		int newHeight = gText.getHeight()/4;
		gText.setAlpha(255);
	gText.render(gRenderer, 10 , 10, newWidth, newHeight);*/

	// Render bar Texts
	skin.RenderText(gRenderer);
	eyes.RenderText(gRenderer);
	hair.RenderText(gRenderer);
	shirt.RenderText(gRenderer);
	pants.RenderText(gRenderer);
	//------------------------------------ Text --------------------------------//
	//--------------------------------------------------------------------------//
	//////////////////////////////////////////////////////////////////////////////
}
