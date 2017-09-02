/*
 * Credits.h
 *
 *  Created on: Aug 22, 2017
 *      Author: Carl
 */

#ifndef ENGINE_CREDITS_H_
#define ENGINE_CREDITS_H_

#include "Helper.h"
#include "JoyStick.h"
#include <SDL2/SDL_Mixer.h>
#include <SDL2/SDL_TTF.h>

class Credits : public Helper, public JoyStick {
public:
	bool quit;
	enum CreditsResult { StartGame, Nothing, Back, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, Credits::CreditsResult &result);
private:
	void free();
	TTF_Font *gFont 			= NULL;
	LTexture gText;
	LTexture gBG;
	LTexture gBG2;
	LTexture gPoof;
	LTexture gTexture;
	LTexture gTargetTexture;
	LTexture gDialogueBox;
	Mix_Chunk *sTyping 			= NULL;
	SDL_Rect rPoof[7];

	float indicatorTimer;
	int indicatorFrame;
	std::string indicator;

	float timer = 0;
	const unsigned int maxPoofs = 10;

	struct Poof {
		float x, y;
		float timer;
		int frame;
		Poof(float newX, float newY) {
			x = newX;
			y = newY;
			timer = 0;
			frame = 0;
		}
	};

	std::vector<Poof> poofs;

	bool holdUp;
	bool holdDown;
	float holdTimer;

private:
	void updateJoystick(SDL_Renderer *gRenderer, LWindow &gWindow, SDL_Event *e, Credits::CreditsResult &result);

	bool A;									// XBOX a button
	bool LAnalogTrigger;					// this will let us use the joystick as a trigger rather than an axis
	bool RAnalogTrigger;					// this will let us use the joystick as a trigger rather than an axis
	const int JOYSTICK_DEAD_ZONE = 8000;
	const int LTRIGGER_DEAD_ZONE = 25000;
	const int RTRIGGER_DEAD_ZONE = 25000;
	double LAngle;
	double RAngle;

    unsigned int slideIndex;
	std::vector<std::string> wholeText;
};

#endif /* ENGINE_CREDITS_H_ */
