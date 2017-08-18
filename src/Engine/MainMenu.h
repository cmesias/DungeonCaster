/*
 * MainMenu.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "LTexture.h"
#include "Helper.h"
#include "Options.h"

#include <fstream>
#include <limits>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class MainMenu: public Helper, public Options {
public:	// Resources
	LTexture gMenu;
	LTexture gText;
public:
	enum MenuResult { Nothing, NewGame, HowToPlay, Options, Credits, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, MainMenu::MenuResult &result);
	MainMenu::MenuResult mousePressed(SDL_Event event);
	MainMenu::MenuResult mouseReleased(LWindow gWindow, SDL_Renderer *gRenderer, SDL_Event event);
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
	 * 0: New Game
	 * 1: How To Play
	 * 2: Options
	 * 3: Credits
	 * 4: Exit
	 */
	std::string buttonName[5];
	SDL_Rect levelsBox[5];
	int menuIndex;				// Which level the Player is currently on
	int key;
	int mx, my;
	bool leftClick;
	bool confirmKey;
	bool quit;
	bool shift;
	SDL_Event event;
};

#endif /* MAINMENU_H_ */
