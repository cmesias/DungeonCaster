/*
 * TestRoom.h
 *
 *  Created on: Aug 22, 2017
 *      Author: Carl
 */

#ifndef ENGINE_HOWTO_H_
#define ENGINE_HOWTO_H_

#include "Helper.h"
#include "JoyStick.h"
#include <SDL2/SDL_Mixer.h>
#include <SDL2/SDL_TTF.h>

class HowTo : public Helper, public JoyStick {
public:
	bool quit;
	enum HowToResult { StartGame, Nothing, Back, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, HowTo::HowToResult &result);
private:
	void free();
	TTF_Font *gFont 			= NULL;
	LTexture gText;
	LTexture gHowToPlay;
	LTexture gTexture;
	LTexture gTargetTexture;
	LTexture gDialogueBox;
	Mix_Chunk *sTyping 			= NULL;

	float indicatorTimer;
	int indicatorFrame;
	std::string indicator;
};

#endif /* ENGINE_HOWTO_H_ */
