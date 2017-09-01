/*
 * TestRoom.h
 *
 *  Created on: Aug 22, 2017
 *      Author: Carl
 */

#ifndef ENGINE_TESTROOM_H_
#define ENGINE_TESTROOM_H_

#include "Helper.h"
#include "JoyStick.h"
#include <SDL2/SDL_Mixer.h>
#include <SDL2/SDL_TTF.h>

class TestRoom : public Helper, public JoyStick {
public:
	bool quit;
	enum TestResult { StartGame, Nothing, Back, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, TestRoom::TestResult &result, int &levelToLoad);
private:
	void free();
	TTF_Font *gFont 			= NULL;
	LTexture gText;
	LTexture gLightBG;
	LTexture gLight;
	LTexture gTexture;
	LTexture gTargetTexture;
	LTexture gDialogueBox;
	Mix_Chunk *sTyping 			= NULL;


	float indicatorTimer;
	int indicatorFrame;
	std::string indicator;
};

#endif /* ENGINE_TESTROOM_H_ */
