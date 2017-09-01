/*
 * Game.h
 *
 *  Created on: Apr 19, 2017
 *      Author: healt
 */

#ifndef GAME_H_
#define GAME_H_

#include "Engine/LWindow.h"
#include "Engine/Options.h"
#include "Engine/Helper.h"

#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

class Game : public Options, public Helper {
public:	// globals
	LWindow gWindow;
	SDL_Renderer *gRenderer;
private:
	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;

public:	// Player Level Selection
	int levelToLoad;
public:
	void Init();
	void Load();
	void Free();
	void Start();
private:
	bool IsExiting();
	void GameLoop();

	// Splash Screen
	void ShowSplashScreen();

	// Menu Screen
	void ShowMenu();

	// Customize Character Screen
	void ShowCustomizeCharacter(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Choose Act XXX to play (for example Act I)
	void ShowActSelectionScreen(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Actual game play
	void ShowPlayGame(int levelToLoad);

	// Show text room
	void ShowTestRoom(LWindow &gWindow, SDL_Renderer *gRenderer, int &levelToLoad);

	enum GameState {Uninitialized, ShowingSplash, ShowingMenu,
					ShowingCustomizeCharacter, ShowingActSelection,
					ShowingPlayGame, ShowingTestRoom,
					OptionScene, CreditScene, Exiting };
	static GameState _gameState;
};

#endif /* GAME_H_ */
