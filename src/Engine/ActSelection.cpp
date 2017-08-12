/*
 * ActSelection.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#include "LWindow.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "ActSelection.h"

void ActSelection::Init() {

}

void ActSelection::Load(SDL_Renderer *gRenderer) {
	gFont13 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 18);
	gFont26 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 26);

	// Buttons
	buttonName[0] = "BACK";
	buttonName[1] = "START";
}

void ActSelection::Free() {
	gText.free();
	TTF_CloseFont(gFont13);
	TTF_CloseFont(gFont26);
	gFont13 = NULL;
	gFont26 = NULL;
}

void ActSelection::Show(LWindow &gWindow, SDL_Renderer *gRenderer, ActSelection::Result &result, int &levelToLoad) {

	// Upon entry
	quit = false;
	leftClick = false;
	enterKey = false;
	shift = false;
	key	= 0;
	levelIndex = 0;
	levelSelection = 0;
	leftAndRightIndex = 0;

	// Load resources
	Load(gRenderer);

	//std::cout << "Inside Act Selection Screen" << std::endl;

	// While loop
	while (!quit) {

		// Set buttons, Quit Save Character, and Start Game
		int w = 48;
		int h = 16;
		int x = 72;
		int y = screenHeight - h - 2;
		button[0].x = screenWidth - w * 2 - 4;	// Quit
		button[1].x = screenWidth - w - 2;		// Start Level

		for (int i=0; i<2; i++) {
			button[i].w = w;
			button[i].h = h;
			button[i].y = y;
		}

		// Set level buttons
		for (int i=0; i<maxLevels; i++) {
			levelsBox[i].w = 84;
			levelsBox[i].h = 12;
			levelsBox[i].x = 5;
			levelsBox[i].y = 5+i*(levelsBox[i].h+2);
		}

		// Get mouse position
		SDL_GetMouseState(&mx, &my);

		// get new mouse coordinates based on render size, and actual screen size
		int renderW = 0;
		int renderHDummy = 0;
		SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummy);
		int en = renderW * 0.4375;
		int renderH = renderW - en;
		mx = (screenWidth*mx)/renderW;	// New mouse coordinates, no relation to camera
		my = (screenHeight*my)/renderH;	// New mouse coordinates, no relation to camera

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Back Game
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				Free();
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					key = 0;
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
						break;
					case SDLK_a:							// Cycle left through selections
						if (leftAndRightIndex > 0) {
							leftAndRightIndex--;
						}
						break;
					case SDLK_d:							// Cycle right through selections
						if (leftAndRightIndex < 2) {
							leftAndRightIndex++;
						}
						break;
					case SDLK_LEFT:							// Cycle left through selections
						if (leftAndRightIndex > 0) {
							leftAndRightIndex--;
						}
						break;
					case SDLK_RIGHT:						// Cycle right through selections
						if (leftAndRightIndex < 2) {
							leftAndRightIndex++;
						}
						break;
					/////////////////////////////////////////////////////////////////////////////////
					///////////////////////// Level selection index navigation //////////////////////
					//-----------------------------------------------------------------------------//
					case SDLK_w:							// Cycle up through levels
						if (levelIndex > 0 && leftAndRightIndex == 0) {
							levelIndex--;
						}
						break;
					case SDLK_s:							// Cycle down through levels
						if (levelIndex < maxLevels-1 && leftAndRightIndex == 0) {
							levelIndex++;
						}
						break;
					case SDLK_UP:							// Cycle up through levels
						if (levelIndex > 0 && leftAndRightIndex == 0) {
							levelIndex--;
						}
						break;
					case SDLK_DOWN:							// Cycle down through levels
						if (levelIndex < maxLevels-1 && leftAndRightIndex == 0) {
							levelIndex++;
						}
						break;
					case SDLK_RETURN:							// Enter button action selection
						enterKey = true;

						break;
					//-----------------------------------------------------------------------------//
					///////////////////////// Level selection index navigation //////////////////////
					/////////////////////////////////////////////////////////////////////////////////
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
					case SDLK_RETURN:							// Enter button action selection
						enterKey = false;
						if (leftAndRightIndex == 0) {			// Select x level
							levelSelection = levelIndex;
						} else if (leftAndRightIndex == 1) {	// Go Back
							result = Back;
						} else if (leftAndRightIndex == 2) {	// Start Game
							result = StartGame;
						}
						break;
					}
				}

				// Mouse motion?
				if (event.type == SDL_MOUSEMOTION) {
					key = 1;
				}

				// Get results from mouse only if we are moving the mouse
				if (key == 1) {
					// Mouse Pressed
					mousePressed(event);

					// Mouse Released
					result = mouseReleased(event);
				}

				// Customize Character results
				switch (result)  {
					case Back:				// Back (to Main Menu)
						quit = true;
						break;
					case Nothing:			// Nothing

						break;
					case StartGame:			// Start Game
						levelToLoad = levelSelection+1;
						quit = true;
						break;
				}
			}
		}

		// Update Variables
		// Set levelIndex equal to whereever the mouse is, else let the keyboard decide what index it is
		if (key == 1) {
			for (int i=0; i<maxLevels; i++) {
				if (checkCollision(mx, my, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
					levelIndex = i;
				}
			}
		}

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			Render(gRenderer);

		/*for (int i=0; i<maxLevels; i++) {
			if (levelIndex == i) {
				SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
				SDL_RenderFillRect(gRenderer, &tempr);
			}else{
				SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderFillRect(gRenderer, &tempr);
			}
		}*/

		// Update screen
		SDL_RenderPresent(gRenderer);
	}
	// Free everything
	Free();
}

void ActSelection::Render(SDL_Renderer *gRenderer) {

	// render level titles and button reactions
	for (int i=0; i<maxLevels; i++) {
		SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
		SDL_SetRenderDrawColor(gRenderer, 65, 65, 65, 255);
		SDL_RenderFillRect(gRenderer, &tempr);
		// Render level index we are currently on
		if (levelIndex==i && leftAndRightIndex == 0) {
			SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
			SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);
		}else{
			SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
			SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);
		}
		// Render chosen level index
		if (levelSelection == i) {
			SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
			SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);
		}
		//Render text input
		std::stringstream tempSS;
		tempSS << "Level " << i+1;
		gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {255,255,255}, gFont13);
		int newWidth = gText.getWidth()/4;
		int newHeight = gText.getHeight()/4;
		gText.render(gRenderer, levelsBox[i].x+5, levelsBox[i].y+levelsBox[i].h/2-newHeight/2,
				newWidth, newHeight);
	}

	// Render buttons
	SDL_Color color = {255,255,255};
	for (int i=0; i<2; i++) {
		if ((mx+1 >= button[i].x && mx <= button[i].x+button[i].w && my+1 >= button[i].y && my <= button[i].y+button[i].h) ||
			(leftAndRightIndex-1 == i)) {
			// Mouse click, hover
			if (leftClick || enterKey) {
				color = {0,255,0};
			}
			// No mouse click, hover
			else{
				color = {244,144,25};
			}
		}else{
			// No mouse click, no hover
			color = {255,255,255};
		}
		// Render each button
		SDL_Rect tempRect = {button[i].x, button[i].y, button[i].w, button[i].h};
		SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);

		std::stringstream tempss;
		tempss << buttonName[i];
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont13);
		gText.setAlpha(255);
		int newWidth = gText.getWidth()/4;
		int newHeight = gText.getHeight()/4;
		gText.render(gRenderer, button[i].x+button[i].w/2 - newWidth/2,
				                button[i].y+button[i].h/2 - newHeight/2,
								newWidth ,newHeight );

	}
}


// Mouse Pressed
ActSelection::Result ActSelection::mousePressed(SDL_Event event){
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
			for (int i=0; i<maxLevels; i++) {
				if (checkCollision(mx, my, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
					levelSelection = i;
				}
			}
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return Nothing;
}

// Mouse Released
ActSelection::Result ActSelection::mouseReleased(SDL_Event event){
	ActSelection::Result result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
			// Perform actions
			for (int i=0; i<2; i++) {
				// If mouse is hovering over menu item then render specifically
				if (checkCollision(mx, my, 1, 1, button[i].x, button[i].y, button[i].w, button[i].h)) {
					if (i == 0) {			// Back
						result = Back;
					}else if (i == 1) {		// Start Level
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
