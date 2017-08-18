/*
 * Monster.cpp
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#include "Engine/LTexture.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Monster.h"
#include "Engine/Particle.h"


void Monster::Init(Monster monster[]){
	int i = 0;
	for (int h = 0; h < 12; h++) {
		for (int w = 0; w < 9; w++) {
			clip[i].x = 0 + w * 16;
			clip[i].y = 0 + h * 16;
			clip[i].w = 16;
			clip[i].h = 16;
			i++;
		}
	}
	count = 0;
	id = 0;
	for (int i = 0; i < max; i++) {
		//------------------------------------------------------------------------//
		//------------------------- Set default of Variables ---------------------//
		monster[i].frameTimer 		= 0;
		monster[i].frameSpeed 		= 5;
		monster[i].frame 			= 0;
		monster[i].moving 			= false;
		monster[i].angle 			= 0.0;
		monster[i].facing 			= 2;
		monster[i].vX 				= 0.0;
		monster[i].vY 				= 0.0;
		monster[i].velX 			= 0.0;
		monster[i].velY 			= 0.0;
		monster[i].timer 			= 0.0;
		monster[i].distance 		= 0.0;
		monster[i].alive 			= false;
		monster[i].collision 		= false;
		monster[i].onScreen 		= false;
		monster[i].alert 			= false;
		monster[i].thinkTimer		= 0;
		monster[i].thinkTimer		= 0;
		monster[i].health 			= 100;
		monster[i].healthDecay 		= 100;
		monster[i].maxHealth 		= 100;
		monster[i].damage 			= 75;
		//------------------------- Set default of Variables ---------------------//
		//------------------------------------------------------------------------//
		monster[i].x 				= -6000;
		monster[i].y 				= -6000;
		monster[i].w 				= 16;
		monster[i].h 				= 16;
		monster[i].speed 			= 0.0;
		monster[i].alive 			= false;
		monster[i].collision 		= false;
		monster[i].onScreen 		= false;
		monster[i].mouse 			= false;
		monster[i].mouseBox 		= false;
		monster[i].id 				= 0;
	}
}

void Monster::Load(SDL_Renderer* gRenderer){
	gMonster.loadFromFile(gRenderer, "resource/gfx/monsters.png");
	gBlueDragon.loadFromFile(gRenderer, "resource/gfx/blue_dragon_and_iceblasts.png");
}

void Monster::Free(){
	gMonster.free();
	gBlueDragon.free();
}

void Monster::Spawn(Monster monster[], float x, float y,
		           float w, float h,
				   float angle, float speed,
				   double id, double type,
				   double health,
				   int distanceHeadIsFromCenterOfImage, int bulletW, int bulletH) {
	for (int i = 0; i < max; i++)
	{
		if (!monster[i].alive)
		{
			//------------------------------------------------------------------------//
			//------------------------- Set default of Variables ---------------------//
			monster[i].frameTimer 		= 0;
			monster[i].frameSpeed 		= 5;
			monster[i].frame 			= 0;
			monster[i].facing 			= 2;
			monster[i].angle 			= 0.0;
			monster[i].moving 			= false;
			monster[i].vX 				= 0.0;
			monster[i].vY 				= 0.0;
			monster[i].velX 			= 0.0;
			monster[i].velY 			= 0.0;
			monster[i].timer 			= 0.0;
			monster[i].distance 		= 0.0;
			monster[i].collision 		= false;
			monster[i].onScreen 		= false;
			monster[i].alert 			= false;
			monster[i].mouse 			= false;
			monster[i].mouseBox 		= false;
			monster[i].thinkTimer		= 0;
			monster[i].damage 			= 75;
			//------------------------- Set default of Variables ---------------------//
			//------------------------------------------------------------------------//
			monster[i].tag 				= "enemy";
			monster[i].x 				= x;
			monster[i].y 				= y;
			monster[i].w 				= w;
			monster[i].h 				= h;
			monster[i].radius 			= w/2;
			monster[i].x2 				= x + w/2;
			monster[i].y2 				= y + h/2;
			monster[i].speed 			= speed;
			//monster[i].vX 				= cos( (3.14159265/180)*(angle) );
			//monster[i].vY 				= sin( (3.14159265/180)*(angle) );
			monster[i].bulletW 			= bulletW;
			monster[i].bulletH 			= bulletH;
			//------------------------------------------------------------------------//
			/* initial centering of monster's circle collision shape */
			float radians 				= (3.1415926536/180)*(0.0);
			float Cos 					= floor(cos(radians)*10+0.5)/10;
			float Sin 					= floor(sin(radians)*10+0.5)/10;
			monster[i].distanceHeadIsFromCenterOfImage	= distanceHeadIsFromCenterOfImage;
			int newW					= distanceHeadIsFromCenterOfImage * (-Cos);
			int newH 					= distanceHeadIsFromCenterOfImage * (-Sin);
			monster[i].x2 				= x+w/2 + newW - monster[i].radius;
			monster[i].y2 				= y+h/2 + newH - monster[i].radius;
			//------------------------------------------------------------------------//
			monster[i].health 			= health;
			monster[i].healthDecay 		= health;
			monster[i].maxHealth 		= health;
			monster[i].id 				= id;
			monster[i].type 			= type;
			monster[i].alive 			= true;
			count++;
			break;
		}
	}
}

void Monster::SpawnMultiple(Monster monster[], int newMx, int newMy, int camx, int camy) {
	Remove(monster, 0);
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			int x = int(newMx + j * 16 + camx);
			int y = int(newMy + h * 16 + camy);
			Spawn(monster, x, y,
					16, 16,
					0.0, 1,
					0, type, 100,
					0, 0, 0);
		}
	}
}

void Monster::Remove(Monster monster[], int click) {
	for (int i = 0; i < max; i++) {
		if (monster[i].alive){
			if (click == 0) {
				if (monster[i].mouseBox){
					monster[i].x = -6000;
					monster[i].y = -6000;
					monster[i].alive = false;
					count--;
				}
			}else{
				if (monster[i].mouse){
					monster[i].x = -6000;
					monster[i].y = -6000;
					monster[i].alive = false;
					count--;
				}
			}
		}
	}
}

void Monster::RemoveAll(Monster monster[]){
	count = 0;
	for (int i = 0; i < max; i++) {
		monster[i].x = -6000;
		monster[i].y = -6000;
		monster[i].alive = false;
	}
}

void Monster::EditorUpdate(Monster monster[], int newMx, int newMy, int mex, int mey, int camx, int camy) {
	// mouse and mouseBox check
	int tileW = 16*multiW;
	int tileH = 16*multiH;
	for (int i = 0; i < max; i++) {
		if (monster[i].alive) {
			////////////////////////////////////////////////////////////////////////////////////////
			//------------------------------------ Editor stuff ----------------------------------//

			//If the mouse+size is on the tile
			if (newMx+tileW-2 > monster[i].x && newMx+1 < monster[i].x + monster[i].w &&
					newMy+tileH-2 > monster[i].y && newMy+1 < monster[i].y + monster[i].h) {
				monster[i].mouseBox = true;
			} else {
				monster[i].mouseBox = false;
			}
			//If the mouse is on the tile
			if (mex > monster[i].x && mex < monster[i].x + monster[i].w &&
				mey > monster[i].y && mey < monster[i].y + monster[i].h) {
				monster[i].mouse = true;
			} else {
				monster[i].mouse = false;
			}
			//------------------------------------ Editor stuff ----------------------------------//
			////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}

void Monster::Update(Monster monster[], Particle particle[], Particle &p_dummy, Player &player, Mix_Chunk* sLazer,
		int camx, int camy) {
	// Player correct size and coordinates
	int playerX = player.x - 3;
	int playerY = player.y - 9;
	int playerW = 16;
	int playerH = 16;
	// mouse and mouseBox check
	int tileW = 16*multiW;
	int tileH = 16*multiH;
	for (int i = 0; i < max; i++) {
		if (monster[i].alive) {
			//  Center of Monster and Target
			float bmx2 = monster[i].x+monster[i].w/2;
			float bmy2 = monster[i].y+monster[i].h/2;
			float bmx  = playerX+playerW/2;
			float bmy  = playerY+playerH/2;
			// Distance from Target
			monster[i].distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
			if (monster[i].distance <= 1) {
				monster[i].distance = 1;
			}

			// Get direction towards Target if distance is < 100
			if (monster[i].distance <= 168) {
				// Angle towardsTarget
				monster[i].angle = atan2(bmy - bmy2,bmx - bmx2);
				monster[i].angle = monster[i].angle * (180 / 3.1416);
				if (monster[i].angle < 0) {
					monster[i].angle = 360 - (-monster[i].angle);
				}

				// Set direction monster is facing based on angle
				if (monster[i].angle >= 315 || monster[i].angle < 45) {					// Right
					monster[i].facing = 2;
				}else if (monster[i].angle >= 45 && monster[i].angle < 135) {			// Down
					monster[i].facing = 0;
				}else if (monster[i].angle >= 135 && monster[i].angle < 225) {			// Left
					monster[i].facing = 3;
				}else if (monster[i].angle >= 225 && monster[i].angle < 315) {			// Up
					monster[i].facing = 1;
				}
				monster[i].alert = true;
			}else{
				monster[i].alert = false;
			}


			// If not alert, patrol around
			if (!monster[i].alert) {
				/*monster[i].thinkTimer += 1;
				if (monster[i].thinkTimer > 60*3) {
					monster[i].thinkTimer = 0;
					int randi = rand() % 2;
					if (randi == 0) {
						monster[i].vX = 1;
					}else{
						monster[i].vX = -1;
					}
				}
				monster[i].vX = monster[i].vX - monster[i].vX * 0.08;*/
			}

			// If alert, move towards target if distance is greater than 48 pixels
			if (monster[i].alert && monster[i].distance >= 20) {
				float radians   = (3.1415926536/180)*( monster[i].angle);
				float Cos 		= floor(cos(radians)*100+0.05)/100;
				float Sin 		= floor(sin(radians)*100+0.05)/100;
				monster[i].targetDistanceX = sqrt((bmx - bmx2) * (bmx - bmx2));
				monster[i].targetDistanceY = sqrt((bmy - bmy2) * (bmy - bmy2));
				if (monster[i].targetDistanceX < 0.1) {
					monster[i].targetDistanceX = 0.1;
				}
				if (monster[i].targetDistanceY < 0.1) {
					monster[i].targetDistanceY = 0.1;
				}
				// Get correct velocity towards Target
				monster[i].vX = 0.25 * (bmx - bmx2) / monster[i].targetDistanceX;
				monster[i].vY = 0.25 * (bmy - bmy2) / monster[i].targetDistanceY;
				// handle movement in controller
				// inside PlayGame.h
				// Monster is moving, set moving to TRUE
				monster[i].moving = true;

				/*constVX = monster[i].speed * (bmx - bmx2) / monster[i].distance;
				constVY = monster[i].speed * (bmy - bmy2) / monster[i].distance;
				//monster[i].x += monster[i].speed * (bmx - bmx2) / monster[i].distance;
				//monster[i].y += monster[i].speed * (bmy - bmy2) / monster[i].distance;

				monster[i].angle = atan2(bmy - bmy2,bmx - bmx2);
				monster[i].angle = monster[i].angle * (180 / 3.1416);

				if (monster[i].angle < 0) {
					monster[i].angle = 360 - (-monster[i].angle);
				}*/
			}else{
				monster[i].moving = false;
				monster[i].vX = 0;
				monster[i].vY = 0;
			}

			// monster shoots if target is 768 pixels away
			if (monster[i].distance <= 96) {

				// calculate some things for shooting
				float radians = (3.1415926536/180)*(monster[i].angle);
				float Cos = floor(cos(radians)*10+0.5)/10;
				float Sin = floor(sin(radians)*10+0.5)/10;
				int newW = monster[i].distanceHeadIsFromCenterOfImage * (-Cos);
				int newH = monster[i].distanceHeadIsFromCenterOfImage * (-Sin);
				monster[i].x2 = monster[i].x+monster[i].w/2 + newW - monster[i].radius;
				monster[i].y2 = monster[i].y+monster[i].h/2 + newH - monster[i].radius;

				/*
				// calculate some things for shooting
				float radians = (3.1415926536/180)*(monster[i].angle);
				float Cos = floor(cos(radians)*10+0.5)/10;
				float Sin = floor(sin(radians)*10+0.5)/10;

				// get player new center position


				// calculate some things for shooting
				float radians = (3.1415926536/180)*(monster[i].angle);
				float Cos = floor(cos(radians)*10+0.5)/10;
				float Sin = floor(sin(radians)*10+0.5)/10;
				int newW = 11 * (-Cos);
				int newH = 11 * (-Sin);
				monster[i].x2 = monster[i].x+monster[i].w/2 + newW - monster[i].radius;
				monster[i].y2 = monster[i].y+monster[i].h/2 + newH - monster[i].radius;

				double barrelW  = (57 * cos(radians) ) - (16 * sin(radians) );				// add this to center of monster (this will give us the guns barrel position)
				double barrelH  = (57 * sin(radians) ) + (16 * cos(radians) );
				int realw = 0;															// monster actual size in pixels
				int realh = 0;															// monster actual size in pixels
				if (monster[i].id == 0) {
					realw = 128;															// monster actual size in pixels
					realh = 128;
				}
				else if (monster[i].id == 1) {
					realw = 512;															// monster actual size in pixels
					realh = 512;
				}


				double particleW = 24;														// particle collision size
				double particleH = 24;
				int wDifference = realw - monster[i].w;										// get difference between monster collision size and actual size (used to center bullets)
				int hDifference = realh - monster[i].h;
				double barrelX = monster[i].x+realw/2-wDifference/2 - particleW/2 + barrelW;	// get final position to spawn a particle
				double barrelY = monster[i].y+realh/2-hDifference/2 - particleH/2 + barrelH;
				*/

				// Enemy shooting
				/*monster[i].timer += 1;
				if (monster[i].timer > 45){
					//monster[i].vX = Cos * 11 - rand() % 8 + 5;
					//monster[i].vY = Sin * 11 - rand() % 8 + 5;
					// restart shoo ttimer
					monster[i].timer = 0;
					// play shoot sound effect
					Mix_PlayChannel(3, sLazer, 0);
					// spawn particle
					p_dummy.spawnParticleAngle(particle, 1,
							barrelX,
							barrelY,
							particleW, particleH,
							monster[i].angle, 11,
						   5,
						   {255, 255,0}, 1,
						   0.0, 0.0,
						   255, 0,
						   100, 1,
						   false, 0);
				}*/

				// Skeleton
				if (monster[i].type == 0) {
					double particleW = 4;														// particle collision size
					double particleH = 4;

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
					/*double barrelW  = (monster[i].bulletW * cos(radians) ) - (monster[i].bulletH * sin(radians) );	// add this to center of monster (this will give us the guns barrel position)
					double barrelH  = (monster[i].bulletW * sin(radians) ) + (monster[i].bulletH * cos(radians) );

					int wDifference = monster[i].realw - monster[i].w;
					int hDifference = monster[i].realh - monster[i].h;

					double barrelX = monster[i].x+monster[i].realw/2-wDifference/2 - particleW/2 + barrelW;
					double barrelY = monster[i].y+monster[i].realh/2-hDifference/2 - particleH/2 + barrelH;*/

					monster[i].timer += 0.25;
					if (monster[i].timer > 60){
						//monster[i].vX = Cos * 11 - rand() % 8 + 5;
						//monster[i].vY = Sin * 11 - rand() % 8 + 5;
						// restart shoo ttimer
						monster[i].timer = 0;
						// play shoot sound effect
						Mix_PlayChannel(2, sLazer, 0);
						// spawn particle
						p_dummy.spawnParticleAngle(particle, monster[i].tag, 4,
								monster[i].x+monster[i].w/2 - particleW/2,
								monster[i].y+monster[i].h/2 - particleH/2+4,
								particleW, particleH,
								monster[i].angle, 0.68,
								monster[i].damage,
							   {180,20,180}, 1,
							   0, 0,
							   255, 0,
							   60, 0.33,
							   false, 0.0,
							   false, 0.0,
							   false, 7, {180,20,180},
							   1, 6);
					}
				}

				// Goblin
				if (monster[i].type == 1) {
					double particleW = 4;
					double particleH = 4;
					monster[i].timer += 0.25;
					if (monster[i].timer == 10.25){
						// play shoot sound effect
						Mix_PlayChannel(-1, sLazer, 0);
						// spawn particle
						p_dummy.spawnParticleAngle(particle, monster[i].tag, 4,
								monster[i].x+monster[i].w/2 - particleW/2,
								monster[i].y+monster[i].h/2 - particleH/2,
								particleW, particleH,
								monster[i].angle-5, 0.88,
								20,
							   {20,180,20}, 1,
							   0, 0,
							   255, 0,
							   60, 0.33,
							   false, 0.0,
							   false, 0.0,
							   false, 7, {20,180,20},
							   1, 6);
						// spawn particle
						p_dummy.spawnParticleAngle(particle, monster[i].tag, 4,
								monster[i].x+monster[i].w/2 - particleW/2,
								monster[i].y+monster[i].h/2 - particleH/2,
								particleW, particleH,
								monster[i].angle, 0.88,
								20,
							   {20,180,20}, 1,
							   0, 0,
							   255, 0,
							   60, 0.33,
							   false, 0.0,
							   false, 0.0,
							   false, 7, {20,180,20},
							   1, 6);
						// spawn particle
						p_dummy.spawnParticleAngle(particle, monster[i].tag, 4,
								monster[i].x+monster[i].w/2 - particleW/2,
								monster[i].y+monster[i].h/2 - particleH/2,
								particleW, particleH,
								monster[i].angle+5, 0.8,
								20,
							   {20,180,20}, 1,
							   0, 0,
							   255, 0,
							   60, 0.33,
							   false, 0.0,
							   false, 0.0,
							   false, 7, {20,180,20},
							   1, 6);
					}
					if (monster[i].timer > 60){
						monster[i].timer = 0;
					}
				}

				// Ghost Mage
				if (monster[i].type == 2) {
					double particleW = 4;
					double particleH = 4;
					monster[i].timer += 0.25;
					for (double k=0; k<60; k += 20) {
						if (monster[i].timer == k){
							// play shoot sound effect
							Mix_PlayChannel(-1, sLazer, 0);
							// spawn particle
							for (double j=0; j<360; j += 40) {
								p_dummy.spawnParticleAngle(particle, monster[i].tag, 4,
										monster[i].x+monster[i].w/2 - particleW/2,
										monster[i].y+monster[i].h/2 - particleH/2,
										particleW, particleH,
										j, 0.55,
										monster[i].damage,
									   {180,0,0}, 1,
									   1, 3,
									   255, 0,
									   60, 0.33,
									   false, 0.0,
									   false, 0.0);
							}
						}
					}
					if (monster[i].timer > 60){
						monster[i].timer = 0;
					}
				}
			}

			//-----------------------------------------------------------------------------------//

			// Monster is moving, set current Frames
			if (monster[i].moving) {
				monster[i].frameTimer += monster[i].frameSpeed;
				if (monster[i].frameTimer > 60) {
					monster[i].frameTimer = 0.0;
					monster[i].frame++;
					if (monster[i].frame > 1) {
						monster[i].frame = 0;
					}
				}
			}



			/* NOTE: we create a different constant variable
			 * for the Monster for it to follow the Player.
			 * We create another variable specifically for knocking back the Monster.
			 */

			// monster movement
			//monster[i].x += monster[i].vX;
			//monster[i].y += monster[i].vY;

			// This is not Monster movement, this is velocity
			// if the Monster gets knocked backed from another force
			/*monster[i].x += monster[i].velX;
			monster[i].y += monster[i].velY;
			monster[i].velX = monster[i].velX - monster[i].velX * 0.08;
			monster[i].velY = monster[i].velY - monster[i].velY * 0.08;*/

			// monster screen check
			if (monster[i].x + monster[i].w >= camx && monster[i].x <= camx + screenWidth &&
				monster[i].y + monster[i].h >= camy && monster[i].y <= camy + screenHeight) {
				monster[i].onScreen = true;
			}else{
				monster[i].onScreen = false;
			}

			// monster death
			if (monster[i].health <= 0)
			{
				player.score += 20;
				// spawn blood particle effect
				/*for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
					int rands = rand() % 9 + 2;
					float newX = monster[i].x+monster[i].w/2;
					float newY = monster[i].y+monster[i].h/2;
					p_dummy.spawnParticleAngle(particle, 2,
										newX-rands/2,
										newY-rands/2,
									   rands, rands,
									   h, randDouble(2.1, 7.1),
									   0.0,
									   {255, 255, 0, 255}, 1,
									   1, 1,
									   rand() % 100 + 150, rand() % 2 + 7,
									   rand() % 50 + 90, 0,
									   true, 0.11,
									   true, randDouble(0.07, 0.69));
				}*/
				monster[i].alive = false;
				count--;
			}

			// monster circle collision check with other zombies
			for (int j = 0; j < max; j++) {
				if (i !=j) {
					if (monster[j].alive) {
						float bmx = monster[j].x+monster[j].w/2;
						float bmy = monster[j].y+monster[j].h/2;
						float bmx2 = monster[i].x+monster[i].w/2;
						float bmy2 = monster[i].y+monster[i].h/2;
						float angle = atan2(bmy - bmy2,bmx - bmx2);
						angle = angle * (180 / 3.1416);
						if (angle < 0) {
							angle = 360 - (-angle);
						}
						float radians = (3.1415926536/180)*(angle);
						float Cos = floor(cos(radians)*10+0.5)/10;
						float Sin = floor(sin(radians)*10+0.5)/10;
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));
						if (distance <= 1) {
							distance = 1;
						}
						if (distance < monster[i].w/2 + monster[j].w/2) {
							monster[i].vX -= 1 * Cos;
							monster[i].vY -= 1 * Sin;
						}
					}
				}
			}
		}
	}
}

void Monster::RenderBehind(SDL_Renderer* gRenderer, Monster monster[], int camx, int camy, float targetY, float targetH) {
	int playerY = targetY - 9;
	int playerH = 16;
	for (int i = 0; i < max; i++) {
		// Only render 16x16 monsters
		if (monster[i].type >= 0 && monster[i].type < 12) {
			if (monster[i].alive) {
				if (monster[i].y+monster[i].h <= playerY+playerH) {
					// Render all Monsters
					gMonster.setAlpha(255);
					// Monster moving, render correct frame
					if (monster[i].moving) {
						int incrementToNextRowAmount = 9;
						int monsterId = (monster[i].frame+2*monster[i].facing) + monster[i].type * incrementToNextRowAmount;
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&clip[monsterId]);
					}
					// Monster stopped moving, render still sprite
					else{
						int numberOfFramesPerRow = monster[i].id + 2 * monster[i].facing ;
						int incrementToNextRowAmount = 9;
						int monsterId = (monster[i].type * incrementToNextRowAmount) + numberOfFramesPerRow;
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&clip[monsterId]);
					}
				}
			}
			// Render Monster death sprite
			else{
				if (monster[i].y+monster[i].h <= playerY+playerH) {
					int numberOfFramesPerRow = 8;
					int incrementToNextRowAmount = 9;
					int monsterId = (monster[i].type * incrementToNextRowAmount) + numberOfFramesPerRow;
					gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
							monster[i].w, monster[i].h,
							&clip[monsterId]);
				}
			}
		}
	}
}

void Monster::RenderInFront(SDL_Renderer* gRenderer, Monster monster[], int camx, int camy, float targetY, float targetH) {
	int playerY = targetY - 9;
	int playerH = 16;
	for (int i = 0; i < max; i++) {
		// Only render 16x16 monsters
		if (monster[i].type >= 0 && monster[i].type < 12) {
			if (monster[i].alive) {
				if (monster[i].y+monster[i].h > playerY+playerH) {
					// Render all Monsters
					gMonster.setAlpha(255);
					// Monster moving, render correct frame
					if (monster[i].moving) {
						int incrementToNextRowAmount = 9;
						int monsterId = (monster[i].frame+2*monster[i].facing) + monster[i].type * incrementToNextRowAmount;
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&clip[monsterId]);
					}
					// Monster stopped moving, render still sprite
					else{
						int numberOfFramesPerRow = monster[i].id + 2 * monster[i].facing ;
						int incrementToNextRowAmount = 9;
						int monsterId = (monster[i].type * incrementToNextRowAmount) + numberOfFramesPerRow;
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&clip[monsterId]);
					}
				}
			}
			// Render Monster death sprite
			else{
				if (monster[i].y+monster[i].h > playerY+playerH) {
					int numberOfFramesPerRow = 8;
					int incrementToNextRowAmount = 9;
					int monsterId = (monster[i].type * incrementToNextRowAmount) + numberOfFramesPerRow;
					gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
							monster[i].w, monster[i].h,
							&clip[monsterId]);
				}
			}
		}
	}
}

void Monster::RenderGUI(SDL_Renderer *gRenderer, Monster monster[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (monster[i].alive){

			renderStatusBar(gRenderer, monster[i].x+monster[i].w/2-camx, monster[i].y-camy, 8, 3,
							1,
							monster[i].health, monster[i].healthDecay, monster[i].maxHealth,
							{80,0,0}, {255,20,255},
							{200,20,20}, {255,255,255}, true);
		}
	}
}

void Monster::RenderDebug(SDL_Renderer *gRenderer, Monster monster[], int camx, int camy) {
	// Render Debug information
	for (int i = 0; i < max; i++) {
		if (monster[i].alive){
			// Render monster box
			SDL_Rect tempRect = {monster[i].x-camx, monster[i].y-camy, monster[i].w, monster[i].h};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
			// Render monster Tile collision box
			tempRect = {monster[i].x+3-camx, monster[i].y+10-camy, 11, 6};
			SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
			// if mouse is hovering
			if (monster[i].mouse) {
				tempRect = {monster[i].x+2 - camx, monster[i].y - camy, 2, 2};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
				/*std::stringstream tempss;
				tempss << monster[i].id;
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont12);
				gText.setAlpha(255);
				gText.render(gRenderer, monster[i].x - camx, monster[i].y - camy, gText.getWidth(), gText.getHeight());*/
			}else{
				tempRect = {monster[i].x+2 - camx, monster[i].y - camy, 2, 2};
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}
			// Identify if the size of the Tiles we are placing are over mouse
			if (monster[i].mouseBox) {
				tempRect = {monster[i].x+4 - camx, monster[i].y - camy, 2, 2};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}else{
				tempRect = {monster[i].x+4 - camx, monster[i].y - camy, 2, 2};
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}
			if (monster[i].alert) {
				SDL_Rect tempRect = {monster[i].x-camx, monster[i].y-4-2-camy, monster[i].w, 4};
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}else{
				SDL_Rect tempRect = {monster[i].x-camx, monster[i].y-4-2-camy, monster[i].w, 4};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}
		}
	}
}

void Monster::LoadData(Monster monster[], int level){
	//Load Tile
	count = 0;
	Init(monster);

	// Open Level File
	std::stringstream fileName;
	fileName << "resource/data/maps/";
	fileName << "level" << level;
	fileName << "/Monster.mp";
	std::fstream fileTileDataL(fileName.str().c_str());
	// Read first line for monster count
	fileTileDataL >> count;
	// Read the rest of the lines for Tile data
	while( fileTileDataL.good() ){
		for (int i = 0; i < max; i++) {
			if (!monster[i].alive) {
				fileTileDataL >>  monster[i].x 		>>
								  monster[i].y 		>>
								  monster[i].w 		>>
								  monster[i].h 		>>
								  monster[i].health >>
								  monster[i].tag 	>>
								  monster[i].id 	>>
								  monster[i].type 	>>
								  monster[i].alive;
				break;
			}
		}
	}
	fileTileDataL.close();
}

std::string Monster::SaveData(Monster monster[]){
	// Create new file to store Tile data
	std::ofstream tileDataFile;
	// Create stringstream to store Tile Data
	std::stringstream tempData;
	// Write Level Size on first line
	tempData << count << "\n";
	// Write the rest of the Tile Data after the first line
	for (int i = 0; i < max; i++) {
		if (monster[i].alive) {
			tempData << monster[i].x 			<< " "
					 << monster[i].y 			<< " "
					 << monster[i].w  		<< " "
					 << monster[i].h  		<< " "
					 << monster[i].health  		<< " "
					 << monster[i].tag  		<< " "
					 << monster[i].id  			<< " "
					 << monster[i].type  		<< " "
			   	   	 << monster[i].alive 		<< "\n";
		}
	}
	return tempData.str().c_str();
}

