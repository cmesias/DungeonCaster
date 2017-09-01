/*
 * Player.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#include "Player.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_mouse.h>

//#include "Input.h"


void Player::SetPosition(int newX, int newY) {
	x = newX;
	y = newY;
}

void Player::SetName(std::string newName) {
	tag = newName;
}

void Player::Load(SDL_Renderer* gRenderer){

	// open joystick index 0
    joy = SDL_JoystickOpen(0);

    // load textures
	gPlayer.loadFromFile(gRenderer, "resource/gfx/ByJoseLuisPeiroLima/players-mages.png");
	gShield.loadFromFile(gRenderer, "resource/gfx/shield.png");
	for (int i=0; i<8; i++){setClips(rPlayer[i], i*16, 32, 16, 16);}
	for (int i=0; i<7; i++){setClips(rShield[i], i*48, 0, 48, 48);}
	for (int i=0; i<4; i++){setClips(rThrusters[i], i*15, 0, 15, 30);}
}

void Player::Free(){
    SDL_JoystickClose(joy);
    gPlayer.free();
    gShield.free();
}

void Player::Init() {
	x = 0;
	y = 0;
	vX = 0.0;
	vY = 0.0;
	score = 0;
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	alive = true;
	indexSaved = -1;
	shieldFrame = 1;
	shieldTick = 0;
	shieldT = 300;
	shield = true;

	controls = 0;
	A = false;
	B = false;
	X = false;
	Y = false;
	D_UP = false;
	D_DOWN = false;
	D_LEFT = false;
	D_RIGHT = false;
	LB = false;
	RB = false;
	BACK = false;
	START = false;
	L3 = false;
	R3 = false;
	LAngle = 0.0;
	RAngle = 0.0;
	zAxisLeft = 0.0;
	zAxisRight = 0.0;
	leftclick = false;
	rightclick = false;
	shift = false;
	ctrl = false;

	test = false;
	trigger = false;
	tag = "player1";

	/* Ship */
	shootAngle = 0.0;
	turnSpeed = 4;
	speed = 0.0;
	maxSpeed = 12.0;

	/* Team */
	team = 0;

	// Animations
	facing = 2;
	frameTimer = 0.0;
	frameSpeed = 15;
	frame = 0;
	moving = false;

	// Used in-game
	angle = 90.0;
	keys = 0;
	coins = 0;
	health = 100;
	maxHealth = 100;
	healthDecay = 100;
	maxMana = 300;
	mana = maxMana;
	manaRegenRate = 3.3;
	manaTick = 0.0;
	useKey = false;

	// Create Spells
	// Fireball Spell

	spell.push_back( Spell("Fire Dance") );


	/*spell.push_back( Spell("Fireball",
							2, 1,
							180, 1,
							3, 1.2,
						    75, {255,144,25},
						    0, 0,
						    0, 0,
						    60, 0.68,
						    false, 0.0,
						    false, 0.0,
						    true, 25, {244,144,25},
						    4, 4,
						    45, 60*3) );

	spell.push_back( Spell("Lightning",
							2, 1,
							180, 1,
							2, 1.5,
						    25, {255,144,244},
						    0, 0,
						    0, 0,
						    60, 0.68,
						    false, 0.0,
						    false, 0.0,
						    true, 25, {244,144,244},
						    1, 5,
						    20, 5) );*/

	// Attacks
	spellIndex = 0;
	moveDelay = false;
	moveDelayTimer = 0;
}

void Player::fire(Particle particle[], Particle &p_dummy, int mx, int my,
		 	 	   Mix_Chunk* sLazer, Mix_Chunk* sGrenade, Mix_Chunk* sGrenadePickup,
				   Mix_Chunk* sPistolReload){

	// Determine controls
	/*if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
	}else if(controls==2){
		trigger = initialshot;
	}

	// Get shoot trigger
	if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
		A = false;
	}else if(controls==2){
		trigger = initialshot;
		initialshot = false;
	}*/

	// Get player angle based on mouse coordinates (used for shooting)
	shootAngle = atan2(my - y-h/2,mx - x-w/2);
	shootAngle = shootAngle * (180 / 3.1416);
	//Set player angle max limits
	if (shootAngle < 0) {
		shootAngle = 360 - (-shootAngle);
	}

	// Get radiant from angle (used for shooting)
	radians = (3.1415926536/180)*(shootAngle);
	radianCos = floor(cos(radians)*10+0.5)/10;
	radianSin = floor(sin(radians)*10+0.5)/10;

	// get player new center position
	//x2 = (x+w/2 + (41 * (-radianCos)) - radius);
	//y2 = (y+h/2 + (41 * (-radianSin)) - radius);
	int newW = 41 * (-radianCos);
	int newH = 41 * (-radianSin);
	//x2 = x+w/2 + newW - radius;
	//y2 = y+h/2 + newH - radius;
	//x2 = x+w/2 - radius;
	//y2 = y+h/2 - radius;
	x2 = x+w/2;
	y2 = y+h/2;


	/* If the object is at a rotation of 0.0 (facing right),
	 * then this will be the distance the particle
	 * will spawn from the center of the object,
	 *
	 * This will provide the correct distance from the center
	 * when the object is rotated from its center.
	 *
	 * With this distance you may add this to your objects x and y center,
	 * and this will be where your particle will spawn
	 */
	/*barrelW  = (bulletW * cos(radians) ) - (bulletH * sin(radians) );	// add this to center of zombie (this will give us the guns barrel position)
	barrelH  = (bulletW * sin(radians) ) + (bulletH * cos(radians) );

	int wDifference = w;
	int hDifference = h;

	//barrelX = x+realw/2-wDifference/2 - particleW/2 + barrelW;
	//barrelY = y+realh/2-hDifference/2 - particleH/2 + barrelH;*/

	/*int newmx = mx-particleW/2;
	int newmy = my-particleH/2;
	float distance = sqrt((newmx - barrelX) * (newmx - barrelX)+
						  (newmy - barrelY) * (newmy - barrelY));
	if (distance <= 1) {
		distance = 1;
	}
	float newAngle = atan2(newmy - barrelY,newmx - barrelX);
	newAngle = newAngle * (180 / 3.1416);
	//Set player angle max limits
	if (newAngle < 0) {
		newAngle = 360 - (-newAngle);
	}*/

	///////////////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------//
	//---------------------- Handle Basic Shooting ----------------------//
	/*
	//Set Turret Position
	float turret1w = 30;
	float turret1h = 30;
	float frigateAngle = angle;
	float radians   = (3.1415926536/180)*(angle);
	float Cos 		= floor(cos(radians)*100+0.05)/100;
	float Sin 		= floor(sin(radians)*100+0.05)/100;
	// 1st turret
	float barrelW  = (0 * Cos ) - (0 * Sin );
	float barrelH  = (0 * Sin ) + (0 * Cos );
	float barrelX = x2 + barrelW;
	float barrelY = y2 + barrelH;
	//turret1x = barrelX - turret1w/2;
	//turret1y = barrelY - turret1h/2;


	// Get turrets nose angle (get the exact position even when the player rotates)
	frigateAngle = shootAngle;
	radians   = (3.1415926536/180)*(shootAngle);
	barrelW  = (bulletW * cos(radians) ) - (bulletH * sin(radians) );	// add this to center of zombie (this will give us the guns barrel position)
	barrelH  = (bulletW * sin(radians) ) + (bulletH * cos(radians) );
	//barrelX = turret1x + turret1w/2 - particleW/2 + barrelW;
	//barrelY = turret1y + turret1h/2 - particleH/2 + barrelH;
	 */

	// Handle casting Spells
	for (unsigned int i = 0; i < spell.size(); i++) {
		if (spell[i].activate) {
			// If Player has enough mana, cast spell, otherwise stop spell casting
			if (mana > spell[i].manaCost) {
				// Stop casting certain spell
				spell[i].activate = false;
				// Subtract mana from Player
				mana -= spell[i].manaCost;
				// Spawn Spell as a particle
				float spe = randDouble(spell[ spellIndex ].minSpe, spell[ spellIndex ].maxSpe);
				float size = randDouble(spell[ spellIndex ].minSize, spell[ spellIndex ].maxSize);
				p_dummy.spawnParticleAngle(particle, tag, spell[i].type,
						x2 - size/2,
						y2 - size/2,
						size, size,
						angle, spe,
						spell[i].damage,
						spell[i].color, 1,
						spell[i].dir, spell[i].dirSpe,
						spell[i].alpha, spell[i].alphaSpe,
						spell[i].deathTimer, spell[i].deathTimerSpe,
						spell[i].sizeDeath, spell[i].sizeDeathSpe,
						spell[i].decay, spell[i].decaySpe,
						spell[i].trail, spell[i].trailRate, spell[i].trailColor,
						spell[i].trailMinSize, spell[i].trailMaxSize);
				// play audio
				Mix_PlayChannel(-1, sLazer, 0);
				// If spell not on cooldown, cast spell
				/*if (!spell[i].cooldown) {
					// Start cooldown
					spell[i].cooldown = true;
					// Reset cooldown timer
					spell[i].cooldownTimer = 0;
					// play audio
					Mix_PlayChannel(-1, sLazer, 0);
				}
				// Spell is on cooldown, handle timer
				else if (spell[i].cooldown) {
					if (spell[i].cooldownTimer < spell[i].baseCooldown) {
						// Begin cooldown count down
						spell[i].cooldownTimer += 1.0;
					}
					// If cool down is equal to baseCooldown0, the Player may activate again
					else{
						// Set Spell to no longer be on cooldown
						spell[i].cooldown = false;
						// Stop casting certain spell
						spell[i].activate = false;
					}
				}*/
			// Not enough mana to cast spell, stop activation
			} else {
				// Stop spell actication
				spell[i].activate = false;
				// Reset cooldown timer
				//spell[i].cooldownTimer = spell[i].baseCooldown;
			}
		}
	}

	// Replenish mana
	if (mana < maxMana) {
		manaTick += manaRegenRate;
		if (manaTick > 60) {
			manaTick = 0;
			mana += manaRegenRate;
		}
	}
	if (mana > maxMana) {
		mana = maxMana;
	}




	// Do a spellIndex cast
	/*if (casting) {
		if (!shootAttack) {
			shootAttack = true;
			// play audio
			Mix_PlayChannel(-1, sLazer, 0);
			// spawn particle
			// Determine spellIndex
			if (spellIndex == 0 && mana > 20) {
				p_dummy.spawnParticleAngle(particle, tag, 2,
						x2 - particleW/2,
						y2 - particleH/2,
						particleW, particleH,
						angle, 1.2,
					   25,
					   {244,144,25}, 1,
					   0, 0,
					   0, 0,
					   60, 0.68,
					   false, 0.0,
					   false, 0.0,
					   true, 25, {244,144,25},
					   4, 4);
				// Reduce mana amount
				mana -= 20;
			}
			else if (spellIndex == 1 && mana > 50) {
				p_dummy.spawnParticleAngle(particle, tag, 2,
						x2 - particleW/2,
						y2 - particleH/2,
						particleW, particleH,
						angle, 1.8,
					   50,
					   {255,120,105}, 1,
					   0, 0,
					   0, 0,
					   60, 1.4,
					   false, 0.0,
					   false, 0.0,
					   true, 25, {255,120,105},
					   4, 4);
				// Reduce mana amount
				mana -= 50;
			}
			else if (spellIndex == 2 && mana > 100) {
				p_dummy.spawnParticleAngle(particle, tag, 2,
						x2 - particleW/2,
						y2 - particleH/2,
						particleW, particleH,
						angle, 2.3,
					   100,
					   {105,120,255}, 1,
					   0, 0,
					   0, 0,
					   60, 3,
					   false, 0.0,
					   false, 0.0,
					   true, 30, {105,120,255},
					   4, 4);
				// Reduce mana amount
				mana -= 100;
			}
		}
		attackTimer += attackSpeed;
		if (attackTimer > 60) {
			attackTimer = 0;
			casting = false;
			shootAttack = false;
		}
	}*/

	// Player move delay
	if (moveDelay) {
		moveDelayTimer += 2;
		if (moveDelayTimer > 60) {
			moveDelayTimer = 0;
			moveDelay = false;
		}
	}

	//---------------------- Handle Basic Shooting ----------------------//
	//-------------------------------------------------------------------//
	///////////////////////////////////////////////////////////////////////

	/*bool trigger = false;
	// Player shoot
	if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
	}*/
}

void Player::move(Particle particle[], Particle &p_dummy,
				   Tile &tl, Tile tile[],
				   int mx, int my){

	/*bool trigger = false;
	// Get Angle
	if (controls==0){
		trigger = thrust;
	}else if(controls==1){
		angle = LAngle;
		trigger = RB;
	}else if(controls==2){
		trigger = thrust;
	}*/


	// Is Player moving?
	if (!moveUp && !moveDown && !moveLeft && !moveRight) {
		moving = false;
	}

	// Player moving
	if (moving) {
		frameTimer += frameSpeed;
		if (frameTimer > 60) {
			frameTimer = 0.0;
			frame++;
			if (frame > 1) {
				frame = 0;
			}
		}
	}
	// Player not moving
	else{

	}
}

void Player::Update(Particle particle[], Particle &p_dummy,
					 Tile &tl, Tile tile[],
					 int mx, int my, int camx, int camy,
					 LWindow gWindow, SDL_Renderer* gRenderer,
					 LTexture gText, TTF_Font *gFont, SDL_Color color,
					 Mix_Chunk *sAtariBoom, Mix_Chunk* sLazer, Mix_Chunk* sGrenade,
					 Mix_Chunk* sGrenadePickup, Mix_Chunk* sPistolReload) {
	// Player alive
	if (alive)
	{
		// Movinig
		move(particle, p_dummy, tl, tile, mx+camx, my+camy);

		// Shooting
		fire(particle, p_dummy, mx+camx, my+camy, sLazer, sGrenade, sGrenadePickup, sPistolReload);


		// Shield animation
		if (shield){
			shieldT -= 1;
			if (shieldT <=0){
				shieldT= 300;
				shield = false;
			}
			shieldTick += 5;
			if (shieldTick>15){
				shieldTick = 0;
				shieldFrame += 1;
			}
			if (shieldFrame>7){
				shieldFrame = 1;
				shieldTick = 0;
			}
		}

		// Set current score max
		if (score > 999999999){
			score = 999999999;
		}
	}

	// if Player is dead
	else{

	}
}

void Player::applyShield(){
	shieldFrame			= 1;
	shieldTick			= 0;
	shieldT				= 300;
	shield				= true;
}

void Player::CastSpell() {
	/*p_dummy.spawnParticleAngle(particle, tag, 2,
			x2 - particleW/2,
			y2 - particleH/2,
			particleW, particleH,
			angle, 1.2,
		   25,
		   {244,144,25}, 1,
		   0, 0,
		   0, 0,
		   60, 0.68,
		   false, 0.0,
		   false, 0.0,
		   true, 25, {244,144,25},
		   4, 4);*/
}

void Player::Render(int mx, int my, int camx, int camy, LWindow gWindow, SDL_Renderer* gRenderer,
					TTF_Font *gFont, TTF_Font *gFont2, SDL_Color color, int &PARTICLES, LTexture gText) {
	gText.setAlpha(255);
	// If alive
	if (alive){

		/* Get difference of real size and collision size,
		 * with this you can center the player by subtracting
		 * this with the player's position
		 */
		/*int wDifference = w;				// get difference between player's collision size and actual size in pixels
		int hDifference = h;
		int newX = x-wDifference/2;					// player starting position
		int newY = y-hDifference/2;*/
		// left arm

		// Render thrusters
		/*if (thrust) {
			float radians   = (3.1415926536/180)*(angle);
			float Cos 		= floor(cos(radians)*100+0.05)/100;
			float Sin 		= floor(sin(radians)*100+0.05)/100;
			float barrelW  = (-21 * Cos ) - (0 * Sin );
			float barrelH  = (-21 * Sin ) + (0 * Cos );
			float barrelX = x2 + barrelW;
			float barrelY = y2 + barrelH;
			float thrustersX = barrelX - 15/2;
			float thrustersY = barrelY - 30/2;
			gThrusters.render(gRenderer, thrustersX-camx, thrustersY-camy, 15, 30, &rThrusters[tFrame], angle);
		}*/

		// If Player is moving
		// NOTE: the "2" is "(16 - 10)/2" ( (renderWidth - collisionWidth / 2) )
		//       This is to center the Player when rendering
		if (moveDelay) {
			gPlayer.render(gRenderer, x-3-camx, y-3-6-camy, 16, 16, &rPlayer[1+2*facing], 0.0);
		}
		// Player not attacking
		else{
			if (moving) {
				gPlayer.render(gRenderer, x-3-camx, y-3-6-camy, 16, 16, &rPlayer[frame+2*facing], 0.0);
			}
			// Player not moving
			else{
				gPlayer.render(gRenderer, x-3-camx, y-6-camy-3, 16, 16, &rPlayer[0+2*facing], 0.0);
			}
		}

		// muzzle flash
		/*if (renderFlash) {
			renderFlash = false;
			double barrelW  = (((bulletW+45) * cos(radians) ) - (bulletH * sin(radians) )) - 120/2;
			double barrelH  = (((bulletW+45) * sin(radians) ) + (bulletH * cos(radians) )) - 120/2;
			// muzzle flash
			spr_player_muzzle_flash.setBlendMode(SDL_BLENDMODE_ADD);
			spr_player_muzzle_flash.render(gRenderer, x+w/2 + barrelW-camx, y+h/2 + barrelH-camy, 120, 120, NULL, angle);
		}*/



		/*SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);
		SDL_RenderDrawLine(gRenderer, x+w/2 + barrelW-camx,
									  y+h/2 + barrelH-camy,
									  mx, my);*/

		// Player sight to mouse
		//SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		//SDL_RenderDrawLine(gRenderer, x+w/2-camx, y+h/2-camy, mx, my);

		// Render Shield
		if (shield){

			gShield.render(gRenderer, x+w/2 - 30/2 - camx, y+h/2 - 30/2 - camy, 30, 30, &rShield[shieldFrame]);

			/*SDL_Rect tempRect = {X-camx, Y-16-camy, (w*shieldT)/300, 3};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);*/
		}

		// Render number of lives left
		/*for (int i=0; i<lives; i++){
			//gPlayer.render(gRenderer, screenWidth-32-i*16, 90, 16, 16);
			spr_player_head.render(gRenderer, screenWidth/2+i*16, 72, 16, 16);
		}*/


		// reticle
		/*double wedth = 21 * radianSin;
		double hedth = 19 * radianCos;
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
			SDL_RenderDrawLine(gRenderer, mx-16-wedth, my+hedth, mx+16-wedth, my+hedth);
			SDL_RenderDrawLine(gRenderer, mx-wedth, my-16+hedth, mx-wedth, my+16+hedth);*/

			/*SDL_Rect tempRect = {mx-24-wedth, my-24+hedth, 48, 48};
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);*/



	}
	// Player is currently dead
	else{

	}

	// Render Visual Collision Box
	/*int playerX = x - 3;
	int playerY = y - 9;
	int playerW = 16;
	int playerH = 16;
	SDL_Rect rect = {playerX-camx, playerY-camy, playerW, playerH};
	SDL_SetRenderDrawColor(gRenderer, 25, 255, 25, 255);
	SDL_RenderDrawRect(gRenderer, &rect);

	// Render Player Actual Collision Box
	rect = {x-camx, y-camy, w, h};
	SDL_SetRenderDrawColor(gRenderer, 25, 255, 25, 255);
	SDL_RenderDrawRect(gRenderer, &rect);*/

	/*std::stringstream tempsi;
	tempsi.str( std::string() );
	tempsi << "moveDelay: " << moveDelay;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {244, 144, 20}, gFont2);
	gText.render(gRenderer, 0, 0, gText.getWidth(), gText.getHeight());

	tempsi.str( std::string() );
	tempsi << "Score: " << score;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont2);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 31, gText.getWidth(), gText.getHeight());

	tempsi.str( std::string() );
	tempsi << "Wave: " << wave;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont2);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 57, gText.getWidth(), gText.getHeight());*/

	/*tempsi.str( std::string() );
	tempsi << "Grenades: " << grenades;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont2);
	gText.render(gRenderer, 10, screenHeight-gText.getHeight(), gText.getWidth(), gText.getHeight());*/

	/*tempsi.str( std::string() );
	tempsi << "Health: " << health;
	gText.loadFromRenderedText(tempsi.str().c_str(), {0,255,0}, gFont2, gRenderer);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 58, gText.getWidth(), gText.getHeight());*/

	// Check what keys are pressed
	/*for (int i=0; i<25; i++){
		std::std::stringstream tempss;
		tempss << "i: " << i << " -        " << SDL_JoystickGetButton(joy, i);
		gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
		gText.render(gRenderer, 5, 5+i*15, gText.getWidth(), gText.getHeight());
	}

	std::std::stringstream tempss;
	tempss.str(std::string());
	tempss << "Axis: " << 0 << " -        " << SDL_JoystickGetAxis(joy, 0);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 5, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 1 << " -        " << SDL_JoystickGetAxis(joy, 1);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 20, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 2 << " -        " << SDL_JoystickGetAxis(joy, 2);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 35, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 3 << " -        " << SDL_JoystickGetAxis(joy, 3);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 50, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 4 << " -        " << SDL_JoystickGetAxis(joy, 4);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 65, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 5 << " -        " << SDL_JoystickGetAxis(joy, 5);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 80, gText.getWidth(), gText.getHeight());*/
}

void Player::OnKeyDown( Player &player, SDL_Keycode sym )
{
	switch (sym){
	player.controls = 0;
	case SDLK_w:					// Thrust space ship
		player.moveUp = true;
		break;
	case SDLK_s:					// Thrust space ship
		player.moveDown = true;
		break;
	case SDLK_a:					// turn left
		player.moveLeft = true;
		break;
	case SDLK_d:					// turn right
		player.moveRight = true;
		break;
	case SDLK_e:					// turn right
		player.useKey = true;
		break;
	case SDLK_1:					// Fireball Spell
		//if (!player.casting && player.mana > 20) {
		//	player.casting = true;
			player.spellIndex = 0;
		//	player.moveDelay = true;
		//}
		break;
	case SDLK_2:					// Fireball Spell
		//if (!player.casting && player.mana > 50) {
		//	player.casting = true;
			player.spellIndex = 1;
		//	player.moveDelay = true;
		//}
		break;
	case SDLK_3:					// Fireball Spell
		//if (!player.casting && player.mana > 100) {
	//		player.casting = true;
			player.spellIndex = 2;
		//	player.moveDelay = true;
		//}
		break;
	case SDLK_r:					// reload
		/*player.controls = 0;
		if (!player.reload && player.ammo < 30) {
			player.reload = true;
		}*/
		break;
	case SDLK_q:					// grenade throw
		//player.controls = 0;
		//player.grenadeTrigger = true;
		break;
	case SDLK_h:
		//debug = ( !debug );
		break;
	case SDLK_y:
		player.camlocked = ( !player.camlocked );
		break;
	case SDLK_SPACE:
		player.controls = 0;
		if (!player.spell[player.spellIndex].activate) {
			player.spell[player.spellIndex].activate = true;
			//	player.moveDelay = true;
		}
		/*if (!player.casting) {
			player.casting = true;
		//	player.moveDelay = true;
		}*/
		break;
	case SDLK_LSHIFT:
		player.shift = true;
		break;
	case SDLK_LCTRL:
		player.ctrl = true;
		player.moving = false;
		player.vX = 0;
		player.vY = 0;
		break;
	}
}

void Player::OnKeyUp( Player &player, SDL_Keycode sym )
{
	switch (sym){
	case SDLK_w:					// Thrust space ship
		player.moveUp = false;
		break;
	case SDLK_s:					// Thrust space ship
		player.moveDown = false;
		break;
	case SDLK_a:					// turn left
		player.moveLeft = false;
		break;
	case SDLK_d:					// turn right
		player.moveRight = false;
		break;
	case SDLK_e:					// turn off useKey
		player.useKey = false;
		break;
	case SDLK_SPACE:
		//
		break;
	case SDLK_LSHIFT:
		player.shift 		= false;
		break;
	case SDLK_LCTRL:
		player.ctrl = false;
		break;
	}
}

void Player::mouseClickState(Player &player, SDL_Event &e){
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			player.controls = 0;
			player.leftclick = true;
			//player.initialshot = true;
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			player.controls = 0;
			player.rightclick = true;
		}
	}else if (e.type == SDL_MOUSEBUTTONUP) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			player.leftclick = false;
			//player.initialshot = false;
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			player.rightclick = false;
		}
	}
}

// Update XBOX 360 controls
void Player::updateJoystick(Player &player, SDL_Event &e){

	/* Xbox 360 Controls */

	// Get Left Analog Angle
	if (((SDL_JoystickGetAxis(player.joy, 0) < -JOYSTICK_DEAD_ZONE) || (SDL_JoystickGetAxis(player.joy, 0) > JOYSTICK_DEAD_ZONE)) ||
		((SDL_JoystickGetAxis(player.joy, 1) < -JOYSTICK_DEAD_ZONE) || (SDL_JoystickGetAxis(player.joy, 1) > JOYSTICK_DEAD_ZONE))){
		player.LAngle = atan2(SDL_JoystickGetAxis(player.joy, 1), SDL_JoystickGetAxis(player.joy, 0)) * ( 180.0 / M_PI );
	}

	// Xbox 360 Controls
	if (e.type == SDL_JOYBUTTONDOWN && e.jbutton.state == SDL_PRESSED && e.jbutton.which == 0){
		switch(e.jbutton.button){
		case 0:
			player.D_UP = true;
			player.moveUp = true;
			break;
		case 1:
			player.D_DOWN = true;
			player.moveDown = true;
			break;
		case 2:
			player.D_LEFT = true;
			player.moveLeft = true;
			break;
		case 3:
			player.D_RIGHT = true;
			player.moveRight = true;
			break;
		case 4:
			player.START = true;
			break;
		case 5:
			player.BACK = true;
			break;
		case 6:
			player.L3 = true;
			break;
		case 7:
			player.R3 = true;
			break;
		case 8:
			player.LB = true;
			break;
		case 9:
			player.RB = true;
			break;
		case 10:
			player.A = true;
			break;
		case 11:
			player.B = true;
			break;
		case 12:
			player.X = true;
			break;
		case 13:
			player.Y = true;
			break;
		}
	}else if (e.type == SDL_JOYBUTTONUP && e.jbutton.state == SDL_RELEASED && e.jbutton.which == 0){
		switch(e.jbutton.button){
		case 0:
			player.D_UP = false;
			player.moveUp = false;
			break;
		case 1:
			player.D_DOWN = false;
			player.moveDown = false;
			break;
		case 2:
			player.D_LEFT = false;
			player.moveLeft = false;
			break;
		case 3:
			player.D_RIGHT = false;
			player.moveRight = false;
			break;
		case 4:
			player.START = false;
			break;
		case 5:
			player.BACK = false;
			break;
		case 6:
			player.L3 = false;
			break;
		case 7:
			player.R3 = false;
			break;
		case 8:
			player.LB = false;
			break;
		case 9:
			player.RB = false;
			break;
		case 10:
			player.A = false;
			break;
		case 11:
			player.B = false;
			break;
		case 12:
			player.X = false;
			break;
		case 13:
			player.Y = false;
			break;
		}
	}
}

void Player::resetHighScore(){
	// Overwrite "highscores.txt"
	std::ofstream fileS;
	fileS.open("highscores.txt");
	for (int i=0; i<10; i++){
		fileS << "AAA 0" << "\n";
	}
	fileS.close();
}

void Player::loadScore(){
	indx = 0;
	bool getHighest = true;
	std::ifstream fileO("highscores.txt");
	std::string str;
	while (std::getline(fileO, str))
	{
		// Read current line
		std::stringstream iss(str);
		std::string temps[2];
		iss >> temps[0] >> temps[1];

		// Store first line as highscore
		if (getHighest){
			getHighest = false;
			highscore = atoi(temps[1].c_str());
		}

		// Load highscore data
		std::stringstream tempss;
		tempss << temps[0] << " " << temps[1];
		highList[indx] = tempss.str().c_str();
		indx++;
	}

	fileO.close();
	/////////////////////////////////////////////////////////////
	/*
	// Render High Score text
	for (int i=0; i<10; i++){
		std::stringstream tempString(highList[i].c_str());
		std::string line;
		while (getline(tempString, line)) {
			std::stringstream iss(line);
			std::string temps[2];
			iss >> temps[0] >> temps[1];

			// Show Player where they are ranked
			if (indexSaved==i){
				gText.loadFromRenderedText(gRenderer, temps[0].c_str(), {244,144,20}, gFont);
				gText.setAlpha(255-i*10);
				gText.render(gRenderer, continueButton[0].x+position,
						continueButton[0].y+continueButton[0].h+20+i*14,
						gText.getWidth(), gText.getHeight());
			}else{
				gText.loadFromRenderedText(gRenderer, temps[0].c_str(), color, gFont);
				gText.setAlpha(255-i*10);
				gText.render(gRenderer, continueButton[0].x+position,
						continueButton[0].y+continueButton[0].h+20+i*14,
						gText.getWidth(), gText.getHeight());
			}

			gText.loadFromRenderedText(gRenderer, temps[1].c_str(), color, gFont);
			gText.setAlpha(255-i*10);
			gText.render(gRenderer, position2,
					continueButton[1].y+continueButton[1].h+20+i*14,
					gText.getWidth(), gText.getHeight());
		}
	}
	 */
}

void Player::saveHighScore() {
	std::ifstream fileO("highscores.txt");
	std::string str;
	std::stringstream tempss;
	std::vector<std::string> t_name;
	std::vector<int> t_score;
	int indx = 0;
	bool replace = true;

	while (getline(fileO, str)){
		// Read current line
		std::stringstream iss(str);

		// Temp string to store Name & Score
		std::string temps[2];

		// Store Name & Score in temp string
		iss >> temps[0] >> temps[1];

		// Now store everything in a vector for checking
		t_name.push_back(temps[0]);
		t_score.push_back( atoi(temps[1].c_str()) );
	}
	fileO.close();


	// If score > saveScore, then insert current score at current index, and remove last index
	for (unsigned int i=0; i<t_name.size(); i++){
		if (score > t_score[i])
		{
			// Insert Player data if we haven't yet
			if (replace)
			{
				replace = false;
				// Insert data at current index
				t_name.insert(t_name.begin()+i, tag);
				t_score.insert(t_score.begin()+i, score);
				indexSaved = i;

				// Remove last index
				t_score.pop_back();
				t_name.pop_back();
			}
		}
	}

	// After replacing data
	for (unsigned int i=0; i<t_name.size(); i++){
		tempss << t_name[i] << " " << t_score[i] << std::endl;
	}

	// Overwrite "highscores.txt"
	std::ofstream fileS;
	fileS.open("highscores.txt");
	fileS << tempss.str().c_str();
	fileS.close();

	// Update score display
	loadScore();
}

bool Player::checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2){
	bool collide;
	if (x+w > x2 && x < x2 + w2 && y+h > y2 && y < y2 + h2){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}
