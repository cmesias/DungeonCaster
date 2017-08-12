/*
 * Players.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_mouse.h>

//#include "Input.h"

#include "Players.h"

// Check collision between 2 objects
bool Players::checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2){
	bool collide;
	if (x+w > x2 && x < x2 + w2 && y+h > y2 && y < y2 + h2){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

// Load asteroid resources
void Players::Load(SDL_Renderer* gRenderer){

	// open joystick index 0
    joy = SDL_JoystickOpen(0);

    // load textures
	gPlayer.loadFromFile(gRenderer, "resource/gfx/players-mages.png");
	gShield.loadFromFile(gRenderer, "resource/gfx/shield.png");
	for (int i=0; i<8; i++){setClips(rPlayer[i], i*16, 32, 16, 16);}
	for (int i=0; i<7; i++){setClips(rShield[i], i*48, 0, 48, 48);}
	for (int i=0; i<4; i++){setClips(rThrusters[i], i*15, 0, 15, 30);}
}

void Players::Free(){
    SDL_JoystickClose(joy);
    gPlayer.free();
    gShield.free();
}

void Players::SetPosition(int newX, int newY) {
	x = newX;
	y = newY;
}

void Players::resetHighScore(){
	// Overwrite "highscores.txt"
	std::ofstream fileS;
	fileS.open("highscores.txt");
	for (int i=0; i<10; i++){
		fileS << "AAA 0" << "\n";
	}
	fileS.close();
}

// Load Player high score data
void Players::loadScore(){
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
}

// Save high score
void Players::saveHighScore() {
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
				t_name.insert(t_name.begin()+i, name);
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

// Reset game
void Players::Init(std::string newName, bool respawn) {
	x = 0;
	y = 0;
	name = newName;
	accuracy = 0.0;
	hits = 0.0;
	totalShot = 0.0;
	vX = 0.0;
	vY = 0.0;
	initialshot = false;
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	deathScreen = false;
	alive = true;
	returned = false;
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
	test = false;
	trigger = false;
	renderFlash = false;
	tag = "player";

	/* Health */
	lives = 1;
	health = 225;
	maxHealth = 225;
	healthDecay = 225;

	/* Ship */
	angle = 0.0;
	shootAngle = 0.0;
	turnSpeed = 4;
	speed = 0.0;
	maxSpeed = 12.0;

	/* Team */
	team = 0;

	/* Player abilities */
	grenades = 3;
	grenadesMax = 3;
	grenadesCD = 0;
	grenadesCDSpeed = 1;
	grenadesCDMax = 60 * 5;
	grenadeTrigger = false;

	// Animations
	facing = 2;
	frameTimer = 0.0;
	frameSpeed = 15;
	frame = 0;
	moving = false;

	// Attacks
	spell = 0;
	casting = false;
	shootAttack = false;
	delay = false;
	moveDelay = false;
	attackTimer = 0.0;
	attackSpeed = 4.5;
	delayT = 0;
	moveDelayTimer = 0;

	// keys
	keys = 0;
	coins = 0;

	// Mana
	maxMana = 300;
	mana = maxMana;
	manaRegenRate = 3.3;
	manaTick = 0.0;

	if (!respawn) {
		score = 0;
		wave = 0;
		increment = 35;
		lives = 1;
	}
}

// Applies a shield to Player
void Players::applyShield(){
	shieldFrame			= 1;
	shieldTick			= 0;
	shieldT				= 300;
	shield				= true;
}

// Player shoot
void Players::fire(Particle particle[], Particle &p_dummy, int mx, int my,
		 	 	   Mix_Chunk* sLazer, Mix_Chunk* sGrenade, Mix_Chunk* sGrenadePickup,
				   Mix_Chunk* sPistolReload){

	// Determine controls
	if (controls==0){
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
	}

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

	// Replenish mana
	if (mana < maxMana) {
		manaTick += manaRegenRate;
		if (manaTick > 60) {
			manaTick = 0;
			mana += manaRegenRate;
		}
	}

	///////////////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------//
	//---------------------- Handle Basic Shooting ----------------------//
	/***** Set Turret Position *****/
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
	/***** Set Turret Position *****/


	/***** Get turrets nose angle (get the exact position even when the player rotates) *****/
	frigateAngle = shootAngle;
	radians   = (3.1415926536/180)*(shootAngle);
	barrelW  = (bulletW * cos(radians) ) - (bulletH * sin(radians) );	// add this to center of zombie (this will give us the guns barrel position)
	barrelH  = (bulletW * sin(radians) ) + (bulletH * cos(radians) );
	//barrelX = turret1x + turret1w/2 - particleW/2 + barrelW;
	//barrelY = turret1y + turret1h/2 - particleH/2 + barrelH;
	/***** Get turrets nose angle *****/

	// Do a spell cast
	if (casting) {
		if (!shootAttack) {
			shootAttack = true;
			// play audio
			Mix_PlayChannel(-1, sLazer, 0);
			// spawn particle
			// Determine spell
			if (spell == 0 && mana > 20) {
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
			else if (spell == 1 && mana > 50) {
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
			else if (spell == 2 && mana > 100) {
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

		/*if (attack) {
			casting = true;
			attack = false;
			// Fireball
			if (spell == 0) {
				// play audio
				Mix_PlayChannel(-1, sLazer, 0);
				// spawn particle
				p_dummy.spawnParticleAngle(particle, tag, 2,
						barrelX,
						barrelY-1,
						particleW, particleH,
						angle, 1.8,
					   1,
					   {255,255,0}, 1,
					   0, 0,
					   0, 0,
					   60, 1,
					   false, 0.0,
					   false, 0.0,
					   true, 15, {244,144,25});
			}
		}*/
	}

	// Player move delay
	if (moveDelay) {
		moveDelayTimer += 2;
		if (moveDelayTimer > 60) {
			moveDelayTimer = 0;
			moveDelay = false;
		}
	}

	// Handle attack delay
	/*else {
		delayT += attackSpeed;
		if (delayT > 60) {
			delayT = 0;
			casting = false;
		}
	}*/

	//---------------------- Handle Basic Shooting ----------------------//
	//-------------------------------------------------------------------//
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------//
	//---------------------- Handle Grenade Throw -----------------------//
	if (grenadeTrigger) {
		grenadeTrigger = false;
		if (grenades > 0) {
			// play audio
			Mix_PlayChannel(5, sGrenade, 0);
			// spawn particle
			p_dummy.spawnParticleAngle(particle, tag, 3,
					barrelX,
					barrelY,
					2, 2,
					angle, 11,
				   0,
				   {255, 255,255}, 1,
				   1, 1,
				   255, 0,
				   60*2, 1,
				   false, 0,
				   true, 0.45);
			// subtract remaining grenades
			grenades--;
		}
	}
	// Replenish grenades
	if (grenades < 3) {
		grenadesCD += grenadesCDSpeed;
		if (grenadesCD > grenadesCDMax) {
			grenadesCD = 0;
			// play audio
			Mix_PlayChannel(6, sGrenadePickup, 0);
			// add grenade
			grenades++;
		}
	}
	//---------------------- Handle Grenade Throw -----------------------//
	//-------------------------------------------------------------------//
	///////////////////////////////////////////////////////////////////////



	/*bool trigger = false;
	// Player shoot
	if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
	}*/

	//if (test){
		//if (!delay) {
			//if (A) {
				//A = false;
				//trigger = false;
				//delay = true;

				// Remove shield
				/*if (shield){
					shield 	= false;
					shieldT 	= 300;
				}*/

				// Shoot particle
				/*if (powerup == "LAZER") {
					totalShot++;
					Mix_PlayChannel(-1, sLazer, 0);

					// TODO [ ] - Create power up drops, and health drops
					// TODO [ ] - Create option for Player to use Game Controller or Keyboard and Mouse
					// TODO [ ] - Create proper title screen with options to setttings etc.
					p_dummy.spawnParticleAngle(particle,
							   x+w/2-4/2+(cos(radians)*9)+(sin(radians)*12),
							   y+h/2-4/2+(sin(radians)*9)-(cos(radians)*12),
							   4, 4,
							   angle, 21,
							   255, 0,
							   100, 2,
							   100, "LAZER",
							   {255, 255,0}, 1,
							   1, 1,
							   false, 0);
					p_dummy.spawnParticleAngle(particle,
							   x+w/2-4/2+(cos(radians)*9)-(sin(radians)*12),
							   y+h/2-4/2+(sin(radians)*9)+(cos(radians)*12),
							   4, 4,
							   angle, 21,
							   255, 0,
							   100, 2,
							   100, "LAZER",
							   {255, 255,0}, 1,
							   1, 1,
							   false, 0);
				}*/
			//}
		/*}
		//Start delay timer after our first shot!
		if (delay) {
			delayT += AttackSpeed;
			if (delayT > 60) {
				delay = false;
				delayT = 0;
			}
		}*/
	//}
}

// Controls
void Players::move(Particle particle[], Particle &p_dummy,
				   TileC &tc, TileC tilec[],
				   Tile &tl, Tile tile[],
				   int mx, int my){

	bool trigger = false;

	// Get Angle
	if (controls==0){
	//	trigger = thrust;
	}else if(controls==1){
	//	angle = LAngle;
	//	trigger = RB;
	}else if(controls==2){
	//	trigger = thrust;
	}

	//Accelerate Player
	/*if (trigger){
		vX += cos(radians) * 0.05;
		vY += sin(radians) * 0.05;

		// Thrust effect
		thrustT += 1;
		if (thrustT > 2){
			thrustT = 0;
			float x1, x2, y1, y2, anglet;
			int rands, randd, randdd;
			//SDL_Color tempColor = { rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1};
			SDL_Color tempColor = { 255, 255, 255 };
			x1 				= x+w/2,
			y1 				= y+h/2;
			x2 				= x+w/2 + cos(radians),
			y2 				= y+h/2 + sin(radians);
			anglet 			= atan2(y1 - y2, x1 - x2);
			anglet 			= anglet * (180 / 3.1416);
			rands 			= rand() % 3 + 6;
			randd			= rand() % 2 + 1;
			if (randd==1) {randdd = 1;} else {randdd = -1;}
			p_dummy.spawnParticleAngle(particle, 2,
							   x+w/2-rands/2, y+h/2-rands/2,
							   rands, rands,
							   anglet, rand() % 2 + 2,
							   0.0,
							   tempColor, 1,
							   rand() % 4 + 5, randdd,
							   rand() % 150 + 50, 4,
							   100, 0,
							   false, 0);
		}
	}else{
	    // Use Stokes' law to apply drag to the ship
		vX = vX - vX * 0.001;
		vY = vY - vY * 0.001;
	}*/

	/*zombie[i].angle = atan2(bmy - bmy2,bmx - bmx2);
	zombie[i].angle = zombie[i].angle * (180 / 3.1416);
	if (zombie[i].angle < 0) {
		zombie[i].angle = 360 - (-zombie[i].angle);
	}
	*/

	// Player movement (moved to PlayGame.h)







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

// Update Player
void Players::Update(Particle particle[], Particle &p_dummy,
					 TileC &tc, TileC tilec[],
					 Tile &tl, Tile tile[],
					 int mx, int my, int camx, int camy,
					 LWindow gWindow, SDL_Renderer* gRenderer,
					 LTexture gText, TTF_Font *gFont, SDL_Color color,
					 Mix_Chunk *sAtariBoom, Mix_Chunk* sLazer, Mix_Chunk* sGrenade,
					 Mix_Chunk* sGrenadePickup, Mix_Chunk* sPistolReload)
{
	// Reset upon leaving pause menu
	if (returned){
		returned 	= false;
		leftclick 	= false;
		initialshot = false;
		moveLeft	= false;
		moveRight	= false;
		moveUp		= false;
		moveDown	= false;
		A			= false;
		RB			= false;
	}

	// Player alive
	if (alive)
	{
		// Movinig
		move(particle, p_dummy, tc, tilec, tl, tile, mx+camx, my+camy);

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

		// Check high score MAX
		if (score > 999999999){
			score = 999999999;
		}

		// Player death
		/*if (health <=0)
		{
			//Spawn explosion after asteroid death
			// spawn blood particle effect
			for (double i=0.0; i< 360.0; i+=rand() % 10 + 10){
				int rands = rand() % 9 + 2;
				float newX = x+w/2;
				float newY = y+h/2;
				p_dummy.spawnParticleAngle(particle, tag, 2,
									newX-rands/2,
									newY-rands/2,
								   rands, rands,
								   i, randDouble(2.1, 5.1),
								   0.0,
								   {255, 0, 0, 255}, 1,
								   1, 1,
								   rand() % 100 + 150, rand() % 2 + 5,
								   rand() % 50 + 90, 0,
								   true, 0.11);
			}

			// Take away lives
			lives-= 1;

			// Reset Player
			Init(name, true);

			// Player ran out of lives, que Death Screen
			if (lives<=0){

				// Set variables
				shieldT 	= 300;
				shield 		= true;
				alive 		= false;
				deathScreen = true;

				// SAVE HIGH SCORE
				saveHighScore();
			}
		}*/

		// Update Player score and Wave
		if (score > highscore){
			highscore = score;
		}
		/*
		if (wave > HW){
			HW 	= wave;
		}*/
	}else{
		// High-Score moving
		travel += 0.05 * dir;
		if (travel > 10){
			dir = -1;
		}
		if (travel < -10){
			dir = 1;
		}

		// Continue YES or NO Screen
		if (deathScreen)
		{
			SDL_ShowCursor(true);
			// Set button position
			continueButton[0] = {0 + screenWidth/2 -96/2-100, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[1] = {0 + screenWidth/2 -96/2+100, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[2] = {0 + screenWidth/2 -256/2 , screenHeight-gText.getHeight()-72, 256, 39};

			// High Score display position
			position  = 0+travel;
			position2 = continueButton[1].x+continueButton[1].w-gText.getWidth()+travel;

			// Left click
			if (leftclick)
			{
				// Option: Yes, reset everything
				if (checkCollision(mx, my, 1, 1, continueButton[0].x, continueButton[0].y, continueButton[0].w, continueButton[0].h))
				{
					leftclick			= false;

					// Reset Player
					std::string newName;
					newName="AAA";
					/////////input.getInput(gameLoop, quit, newName, gWindow, gRenderer);
					Init(newName, false);

					// Clear Asteroids & Enemies
					SDL_ShowCursor(false);
				}

				// Option: No, go to Main Menu
				if (checkCollision(mx, my, 1, 1, continueButton[1].x, continueButton[1].y, continueButton[1].w, continueButton[1].h))
				{
					score = 0;
					leftclick  = false;
					deathScreen = false;
				}

				// Option: Reset high scores
				if (checkCollision(mx, my, 1, 1, continueButton[2].x, continueButton[2].y, continueButton[2].w, continueButton[2].h))
				{
					// Reset high scores
					resetHighScore();

					// Load again
					loadScore();
				}
			}
		// Menu Screen
		}else{
			SDL_ShowCursor(true);
			// Set button position
			continueButton[0] = {screenWidth/2-96/2, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[2] = {screenWidth/2-256/2 , screenHeight-gText.getHeight()-72, 256, 39};

			// High Score display position
			position 	= continueButton[0].w/2-gText.getWidth()/2-100+travel;
			position2 = continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2+100+travel;

			// Left click
			if (leftclick)
			{
				// Option: Play
				if (checkCollision(mx, my, 1, 1, continueButton[0].x, continueButton[0].y, continueButton[0].w, continueButton[0].h))
				{
					// Reset Player
					std::string newName;
					newName="AAA";
					///////////input.getInput(gameLoop, quit, newName, gWindow, gRenderer);
					Init(newName, false);

					// Clear Asteroids & Enemies
					SDL_ShowCursor(false);
				}

				// Option: Reset high scores
				if (checkCollision(mx, my, 1, 1, continueButton[2].x, continueButton[2].y, continueButton[2].w, continueButton[2].h))
				{
					// Reset high scores
					resetHighScore();

					// Load again
					loadScore();
				}
			}
		}
	}
}

// Render Player
void Players::Render(int mx, int my, int camx, int camy, LWindow gWindow, SDL_Renderer* gRenderer,
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



	}else{
		// Continue YES or NO Screen
		if (deathScreen)
		{
			// Render Text
			gText.loadFromRenderedText(gRenderer, "You have died. Continue?", color, gFont2);
			gText.render(gRenderer, screenWidth/2-gText.getWidth()/2, screenHeight/2-gText.getHeight()/2-50, gText.getWidth(), gText.getHeight());

			// Render buttons: Yes
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[0]);

			// Render buttons: No
			SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[1]);

			// Render buttons: ResetHighScore
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[2]);

			// Render button texts: Yes or No
			gText.loadFromRenderedText(gRenderer, "Yes", color, gFont2);
			gText.render(gRenderer,  continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2,
									 continueButton[0].y+continueButton[0].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());

			gText.loadFromRenderedText(gRenderer, "No", color, gFont2);
			gText.render(gRenderer,  continueButton[1].x+continueButton[1].w/2-gText.getWidth()/2,
									 continueButton[1].y+continueButton[1].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());

			// Render Text
			gText.loadFromRenderedText(gRenderer, "Reset High Scores", color, gFont2);
			gText.render(gRenderer, continueButton[2].x+continueButton[2].w/2-gText.getWidth()/2,
									 continueButton[2].y+continueButton[2].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());
		// Player Menu screen
		}else{

			// Render buttons: Play
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[0]);

			// Render buttons: ResetHighScore
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &continueButton[2]);

			// Render Text
			gText.loadFromRenderedText(gRenderer, "PLAY", color, gFont2);
			gText.render(gRenderer, continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2,
									 continueButton[0].y+continueButton[0].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());

			// Render Text
			gText.loadFromRenderedText(gRenderer, "Reset High Scores", color, gFont2);
			gText.render(gRenderer, continueButton[2].x+continueButton[2].w/2-gText.getWidth()/2,
									 continueButton[2].y+continueButton[2].h/2-gText.getHeight()/2,
									 gText.getWidth(), gText.getHeight());
		}

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



// Key Pressed
void Players::OnKeyDown( Players &player, SDL_Keycode sym )
{
	switch (sym){
	case SDLK_w:					// Thrust space ship
		player.controls = 0;
		if (!player.casting) {
		}
		player.moveUp = true;
		break;
	case SDLK_s:					// Thrust space ship
		player.controls = 0;
		if (!player.casting) {
		}
		player.moveDown = true;
		break;
	case SDLK_a:					// turn left
		player.controls = 0;
		if (!player.casting) {
		}
		player.moveLeft = true;
		break;
	case SDLK_d:					// turn right
		player.controls = 0;
		if (!player.casting) {
		}
		player.moveRight = true;
		break;
	case SDLK_1:					// Fireball Spell
		player.controls = 0;
		//if (!player.casting && player.mana > 20) {
		//	player.casting = true;
			player.spell = 0;
		//	player.moveDelay = true;
		//}
		break;
	case SDLK_2:					// Fireball Spell
		player.controls = 0;
		//if (!player.casting && player.mana > 50) {
		//	player.casting = true;
			player.spell = 1;
		//	player.moveDelay = true;
		//}
		break;
	case SDLK_3:					// Fireball Spell
		player.controls = 0;
		//if (!player.casting && player.mana > 100) {
	//		player.casting = true;
			player.spell = 2;
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
		//player.initialshot = true;
		player.controls = 0;
		if (!player.casting) {
			player.casting = true;
		//	player.moveDelay = true;
		}
		break;
	case SDLK_LSHIFT:
		player.shift = true;
		break;
	}
}

// Key Released
void Players::OnKeyUp( Players &player, SDL_Keycode sym )
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
	case SDLK_d:				// turn right
		player.moveRight = false;
		break;
	case SDLK_SPACE:
		//player.initialshot 	= false;
		break;
	case SDLK_LSHIFT:
		player.shift 		= false;
		break;
	}
}

// Player Mouse Pressed
void Players::mouseClickState(Players &player, SDL_Event &e){
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
void Players::updateJoystick(Players &player, SDL_Event &e){

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
