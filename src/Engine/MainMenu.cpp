/*
 * MainMenu.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#include "LWindow.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "MainMenu.h"

void MainMenu::Init() {

}

void MainMenu::Load(SDL_Renderer *gRenderer) {
	// Load Video settings from file
	loadVideoCFG();
	// Load Audio settings from file
	loadAudioCFG();
	// Load audio files
	LoadAudioFiles();
	// Apply audio configurations
	applyMasterAudioCFG();
	// Textures
	gMenu.loadFromFile(gRenderer, "resource/gfx/menu.png");
	// Fonts
	gFont 	= TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 18);
	gFont13 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 13);
	gFont26 = TTF_OpenFont("resource/fonts/FredokaOne-Regular.ttf", 26);
	// Buttons
	buttonName[0] = "New Game";
	buttonName[1] = "How To Play";
	buttonName[2] = "Options";
	buttonName[3] = "Credits";
	buttonName[4] = "Exit";
}

void MainMenu::Free() {
	// Free audio files
	FreeAudioFiles();
	gMenu.free();
	gText.free();
	TTF_CloseFont(gFont);
	TTF_CloseFont(gFont13);
	TTF_CloseFont(gFont26);
	gFont = NULL;
	gFont13 = NULL;
	gFont26 = NULL;
}

void MainMenu::Show(LWindow &gWindow, SDL_Renderer *gRenderer, MainMenu::MenuResult &result) {

	// Upon entry
	quit = false;
	leftClick = false;
	confirmKey = false;
	shift = false;
	key	= 0;
	menuIndex = -1;
	result = Nothing;

	// Load resources
	Load(gRenderer);

	//std::cout << "Inside Act Selection Screen" << std::endl;

	// While loop
	while (!quit) {

		// Set level buttons
		for (int i=0; i<5; i++) {
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
					/////////////////////////////////////////////////////////////////////////////////
					///////////////////////// Level selection index navigation //////////////////////
					//-----------------------------------------------------------------------------//
					case SDLK_w:							// Cycle up through levels
						if (menuIndex > 0) {
							menuIndex--;
						}
						break;
					case SDLK_s:							// Cycle down through levels
						if (menuIndex < 4) {
							menuIndex++;
						}
						break;
					case SDLK_UP:							// Cycle up through levels
						if (menuIndex > 0) {
							menuIndex--;
						}
						break;
					case SDLK_DOWN:							// Cycle down through levels
						if (menuIndex < 4) {
							menuIndex++;
						}
						break;
					case SDLK_RETURN:							// Enter button action selection
						confirmKey = true;
						break;
					case SDLK_SPACE:							// Enter button action selection
						confirmKey = true;
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
						confirmKey = false;
						if (menuIndex == 0) {
							result = NewGame;
						} else if (menuIndex == 1) {
						//	result = HowToPlay;
						} else if (menuIndex == 2) {
						//	result = Options;
							start(gWindow,gRenderer);
						} else if (menuIndex == 3) {
						//	result = Credits;
						} else if (menuIndex == 4) {
							result = Exit;
						}
						break;
					case SDLK_SPACE:							// Enter button action selection
						confirmKey = false;
						if (menuIndex == 0) {
							result = NewGame;
						} else if (menuIndex == 1) {
						//	result = HowToPlay;
						} else if (menuIndex == 2) {
							//	result = Options;
							start(gWindow,gRenderer);
						} else if (menuIndex == 3) {
						//	result = Credits;
						} else if (menuIndex == 4) {
							result = Exit;
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
					result = mouseReleased(gWindow, gRenderer, event);
				}

				// Handle results
				switch (result)  {
					case NewGame:
						quit = true;
						break;
					case HowToPlay:
						quit = true;
						break;
					case Options:
						quit = true;
						break;
					case Credits:
						quit = true;
						break;
					case Exit:
						quit = true;
						break;
					case Nothing:

						break;
				}
			}
		}

		// Update Variables
		// Set menuIndex equal to where ever the mouse is, else let the keyboard decide what index it is
		if (key == 1) {
			for (int i=0; i<5; i++) {
				if (checkCollision(mx, my, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
					menuIndex = i;
				}
			}
		}

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			gMenu.render(gRenderer, 0, 0, 270, 152);

			Render(gRenderer);

		// Update screen
		SDL_RenderPresent(gRenderer);
	}
	// Free everything
	Free();
}

void MainMenu::Render(SDL_Renderer *gRenderer) {

	// render level titles and button reactions
	for (int i=0; i<5; i++) {
		// Render background
		SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
		SDL_SetRenderDrawColor(gRenderer, 65, 65, 65, 255);
		SDL_RenderFillRect(gRenderer, &tempr);

		// Render border around title menu item
		if (key == 0) {
			if (menuIndex==i) {
				if (confirmKey) {
					SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
					SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}else{
					SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
					SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
			}
		}else{
			if (checkCollision(mx, my, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
				if (leftClick) {
					SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
					SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}else{
					SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
					SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
			}
		}




		// Render level index we are currently on
		/*if (menuIndex==i) {
			if (leftClick) {
				SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
				SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}else{
				SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
				SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}else{
			SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
			SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);
		}*/
		//Render text input
		std::stringstream tempSS;
		tempSS << buttonName[i];
		gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {255,255,255}, gFont13);
		int newWidth = gText.getWidth()/4;
		int newHeight = gText.getHeight()/4;
		gText.render(gRenderer, levelsBox[i].x+5, levelsBox[i].y+levelsBox[i].h/2-newHeight/2,
				newWidth, newHeight);
	}
}


// Mouse Pressed
MainMenu::MenuResult MainMenu::mousePressed(SDL_Event event){
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
			for (int i=0; i<5; i++) {
				if (checkCollision(mx, my, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
					menuIndex = i;
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
MainMenu::MenuResult MainMenu::mouseReleased(LWindow gWindow, SDL_Renderer *gRenderer, SDL_Event event){
	MainMenu::MenuResult result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
			// Perform actions
			for (int i=0; i<5; i++) {
				// If mouse is hovering over menu item then render specifically
				if (checkCollision(mx, my, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
					if (i == 0) {
						result = NewGame;
					} else if (i == 1) {
					//	result = HowToPlay;
					} else if (i == 2) {
					//	result = Options;
						start(gWindow,gRenderer);
					} else if (i == 3) {
					//	result = Credits;
					} else if (i == 4) {
						result = Exit;
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




