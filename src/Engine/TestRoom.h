/*
 * TestRoom.h
 *
 *  Created on: Aug 22, 2017
 *      Author: Carl
 */

#ifndef ENGINE_TESTROOM_H_
#define ENGINE_TESTROOM_H_

#include "Helper.h"
#include <SDL2/SDL_Mixer.h>
#include <SDL2/SDL_TTF.h>

class TestRoom : public Helper {
public:
	bool quit;
	enum TestResult { Nothing, Back, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, TestRoom::TestResult &result);
private:
	void free();
	TTF_Font *gFont 			= NULL;
	LTexture gLightBG;
	LTexture gLight;
	LTexture gTexture;
	LTexture gTargetTexture;
	LTexture gText;
	Mix_Chunk *sTyping 			= NULL;
};

#endif /* ENGINE_TESTROOM_H_ */
