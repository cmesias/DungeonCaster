/*
 * ThankYou.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: Carl
 */

#include "LTexture.h"
#include "LWindow.h"
#include "Particle.h"
#include <sstream>
#include <vector>
#include <SDL2/SDL.h>

#include "ThankYou.h"


void ThankYou::Show(LWindow &gWindow, SDL_Renderer *gRenderer, ThankYou::ThankYouResult &result) {

	// particle
	static Particle part;
	static Particle particle[8000];
	part.init(particle);
	part.load(gRenderer);
	SDL_Rect room = {0, 0, 512, 512};
	int mx, my;
	bool showDialogue;
	unsigned int dialogueIndex;

	// Upon entry
	showDialogue = true;
	dialogueIndex = 0;
	indicatorTimer = 0;
	indicatorFrame = 0;
	indicator = ">";
	quit = false;
	gFont 	= TTF_OpenFont("resource/fonts/PressStart2P.ttf", 18);
	gTexture.loadFromFile(gRenderer, "resource/gfx/test.png");
	gLightBG.loadFromFile(gRenderer, "resource/gfx/light_bg.png");
	gLight.loadFromFile(gRenderer, "resource/gfx/light.png");
	gDialogueBox.loadFromFile(gRenderer, "resource/gfx/dialogue-box.png");
	gTargetTexture.createBlank( gRenderer, screenWidth, screenHeight, SDL_TEXTUREACCESS_TARGET );
	sTyping = Mix_LoadWAV("sounds/snd_typing.wav");
	Mix_VolumeChunk(sTyping, 30);

	// Events
	SDL_Event event;

	// Dialogue
	std::vector<std::string> dialogue;
	dialogue.push_back("...");
	dialogue.push_back("Someone help please!");
	dialogue.push_back("...");
	dialogue.push_back("You there soldier!");
	dialogue.push_back("I know this may be too much to ask but you are the last of our soldiers...");
	dialogue.push_back("I beg of you bring this final message to the King.");
	dialogue.push_back("I don't have much time left...");
	dialogue.push_back("Tell him...");
	dialogue.push_back("\"Cough cough\"");
	dialogue.push_back("Tell him his daughter lives.");
	dialogue.push_back("That's it.");
	dialogue.push_back("...");
	dialogue.push_back("Me?");
	dialogue.push_back("I will try and fend off these wild beasts for as long as I stand.");
	dialogue.push_back("There are plenty monsters in the castle still but the majority are in front of the gates.");
	dialogue.push_back("Once you deliver this message to the King, if you would like to help, come find me at the gates.");
	dialogue.push_back("I will see you soldier.");
	dialogue.push_back("What was it, your name was?");

	// Around here, have soldier reply back and say something about their name
	std::string name = "Gridin";
	std::stringstream tempss;
	tempss << "Ah "<< name << ", ok, I will remember that.";
	dialogue.push_back(tempss.str().c_str());

	tempss.str(std::string());
	tempss << "After this is over I'll buy you a drink " << name << ".";
	dialogue.push_back(tempss.str().c_str());
	dialogue.push_back("Be seeing you.");

	float timer;
	float rate;
	unsigned int letters;
	std::stringstream dialogueToRender;

	timer = 0;
	rate = 20;
	letters = 0;

	// While loop
	while (!quit) {

		// fps timer
		fps.start();
		// Get mouse position
		SDL_GetMouseState(&mx, &my);

		/* get render width and height
		 * but (mostly used to get actual render height)
		 */
		int renderW = 0;
		int renderHDummy = 0;
		SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummy);
		int en = renderW * 0.4375;
		int renderH = renderW - en;

		// Grid like coordinates of mouse
		// get new mouse coordinates based on render size, and actual screen size
		mx = (screenWidth*mx)/gWindow.getWidth();			// New mouse coordinates, no relation to camera
		my = (screenHeight*my)/gWindow.getHeight();			// New mouse coordinates, no relation to camera

		// Handle events
		while (SDL_PollEvent(&event)) {

			// Global Events
			if (event.type == SDL_QUIT) {
				result = Exit;
				part.free();
				free();
				return;
			}

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Controller button down
			if (event.type == SDL_JOYBUTTONDOWN){

			}

			// Key Pressed
			if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
				switch (event.key.keysym.sym) {
				case SDLK_r:						// reset test-room

					break;
				case SDLK_q:
					//SDL_ShowCursor(SDL_TRUE);
					break;
				case SDLK_e:
					//SDL_ShowCursor(SDL_FALSE);
					break;
				case SDLK_RETURN:				// Cycle through dialogue
					break;
				case SDLK_ESCAPE:
					result = Back;
					break;
				}
			}
			// Key Released
			else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
				switch (event.key.keysym.sym) {
				case SDLK_e:
					break;
				}
			}
			// Mouse Pressed
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					/*part.spawnParticleAngle(particle, "none", 3,
							mx - 4/2,
							my - 4/2,
							4, 4,
							0.0, 1.2,
						   25,
						   {244,255,255}, 1,
						   0, 0,
						   255, 0,
						   60, 1,
						   false, 0.0,
						   false, 0.0);*/
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
				}
			}
			// Mouse Released
			else if (event.type == SDL_MOUSEBUTTONUP) {
				if (event.button.button == SDL_BUTTON_LEFT) {

				}
				if (event.button.button == SDL_BUTTON_RIGHT) {

				}
			}
		}

		// Customize Character results
		switch (result)  {
			case Nothing:		// Nothing

				break;
			case Back:			// Back
				quit = true;
				break;
			case Exit:			// Exit
				quit = true;
				break;
		}

		// Indicator animation
		indicatorTimer += 3;
		if (indicatorTimer > 60) {
			indicatorTimer = 0;
			indicatorFrame++;
			if (indicatorFrame > 1) {
				indicatorFrame = 0;
			}
		}
		if (indicatorFrame == 0) {
			indicator = "";
		}else{
			indicator = ">";
		}

		// Update particle
		part.Update(particle, 0, 0, room.w, room.h, 0, 0, 100, 100);
		part.updateStarParticles(particle, 0, 0, room.w, room.h);
		part.updateBulletParticles(particle, 0, 0, room.w, room.h);

		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////
		//------------------------------------ Draw on Scene here ------------------------------------//
		//Set our Texture as our render target
		gTargetTexture.setAsRenderTarget(gRenderer);
		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Render foreground
			//gTexture.setAlpha(255);
			//gTexture.render(gRenderer, 0, 0, 128, 128);

			// Render particle
			part.renderStarParticle(particle, 1, 1, 1, gRenderer);
			part.RenderBullets(gRenderer, particle, 1, 1, 1);

		//Reset render target
		SDL_SetRenderTarget( gRenderer, NULL );
		//------------------------------------ Draw on Scene here ------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////


		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);


			// Render default lighting background (sets the darkness level
			/*gLightBG.setAlpha(50);
			gLightBG.setColor(255,255,255);
			gLightBG.setBlendMode(SDL_BLENDMODE_ADD);
			gLightBG.render( gRenderer, 0, 0, screenWidth, screenHeight);

			// Render Particle lights
			for (int i = 0; i < part.max; i++) {
				if (particle[i].alive) {
					if (particle[i].damage > 0) {
						gLight.setAlpha(180);
						gLight.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
						gLight.setBlendMode(SDL_BLENDMODE_ADD);
						gLight.render( gRenderer, particle[i].x2-108/2-0, particle[i].y2-108/2-0, 108, 108);
					}else{
						gLight.setAlpha(particle[i].alpha);
						gLight.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
						gLight.setBlendMode(SDL_BLENDMODE_ADD);
						SDL_Rect rparticle = {0,0,8,8};
						gLight.render( gRenderer, particle[i].x2-(particle[i].w/2)*7-0, particle[i].y2-(particle[i].h/2)*7-0, particle[i].w*7, particle[i].h*7, NULL, particle[i].angle);

					}
				}
			}*/

			// Render sceene
			//gTargetTexture.setBlendMode(SDL_BLENDMODE_MOD);
			gTargetTexture.render( gRenderer, 0, 0, screenWidth, screenHeight);



			float x = 0+4;
			float y = screenHeight-24 - 8;

			renderDialogText(gRenderer, "Cmesias",
							 "Thank you for playing my game, I hope you liked it. \nPress Escape to return to the Main Menu.", indicator.c_str(),
						     x, y, 21, 24,
						     x, y, screenWidth-8, 24,
						     {255,255,255}, {255,255,255},
						     {18,18,18}, {60,200,40},
						     {18,18,18}, {60,200,40},
						     gFont, gFont, gText,
							 1000,  true);




			/*if (showDialogue) {
				if ( letters < dialogue[dialogueIndex].size() ) {
					timer += rate;
					if (timer > 60) {
						dialogueToRender << dialogue[dialogueIndex][letters];
						timer = 0;
						letters++;
						// Play typing sound effect
						Mix_PlayChannel(-1, sTyping, 0);
					}
				}
				std::string temps;
				temps = dialogueToRender.str().c_str();
				float x = 0+4;
				float y = screenHeight-24 - 8;

				//gDialogueBox.render(gRenderer, x, y - 9 - 2, 50, 9);
				//gDialogueBox.render(gRenderer, x, y, 262, 24);

				renderDialogText(gRenderer, "Jacky The Jacker",
								 temps.c_str(), indicator.c_str(),
							     x, y, 21, 24,
							     x, y, screenWidth-8, 24,
							     {255,255,255}, {255,255,255},
							     {18,18,18}, {200,100,250},
							     {18,18,18}, {200,100,250},
							     gFont, gFont, gText,
								 1000,  true);

			}*/

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if (cap && fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
			SDL_Delay((1000/FRAMES_PER_SECOND ) - fps.get_ticks());
		}
	}
	part.free();
	free();
}

void ThankYou::free() {
	// Free resources
	TTF_CloseFont(gFont);
	gFont = NULL;
	gText.free();
	gLightBG.free();
	gLight.free();
	gTexture.free();
	gTargetTexture.free();
	gDialogueBox.free();
	Mix_FreeChunk(sTyping);
	sTyping 		= NULL;
}
