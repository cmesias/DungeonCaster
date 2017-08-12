/*
 * ActSelection.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef ACTSELECTION_H_
#define ACTSELECTION_H_

#include "LTexture.h"
#include "Helper.h"

#include <fstream>
#include <limits>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class ActSelection: public Helper {
public:	// Resources
	LTexture gText;
	TTF_Font *gFont13 			= NULL;
	TTF_Font *gFont26 			= NULL;
public:
	enum Result { Back, Nothing, StartGame, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, ActSelection::Result &result, int &levelToLoad);
	ActSelection::Result mousePressed(SDL_Event event);
	ActSelection::Result mouseReleased(SDL_Event event);
public:

	// Initialize
	void Init();

	// Load resources
	void Load(SDL_Renderer *gRenderer);

	// Free resources
	void Free();

	void Render(SDL_Renderer *gRenderer);
private:
	/* Main Menu items
	 * 0: Back
	 * 1: Start Level
	 */
	SDL_Rect button[2];
	std::string buttonName[2];
	SDL_Rect levelsBox[10];
	int levelIndex;				// Which level the Player is currently on
	int levelSelection;			// Which level the Player selected
	/* If Player does not have a mouse, they can start with keyboard
	 * 0: Level selection index
	 * 1: Back selection index
	 * 2: Start Game Selection Index
	 */
	int leftAndRightIndex;
	const int maxLevels = 10;	// Max number of levels
	int key;
	int mx, my;
	bool leftClick;
	bool enterKey;
	bool quit;
	bool shift;
	SDL_Event event;
};

#endif /* ACTSELECTION_H_ */
