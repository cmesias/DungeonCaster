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
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Monster.h"
#include "Engine/Particle.h"

// TODO - make Boss spell

void Monster::Init(Monster monster[]){
	// Load monster clips
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
	// Load boss clips
	dragonClips[0] = {0, 192, 48, 16};
	dragonClips[1] = {48, 192, 48, 16};
	dragonClips[2] = {96, 192, 48, 16};

	dragonClips[3] = {96, 208, 48, 16};
	dragonClips[4] = {96, 208, 48, 16};
	dragonClips[5] = {96, 208, 48, 16};

	count = 0;
	id = 0;
	for (int i = 0; i < max; i++) {
		//------------------------------------------------------------------------//
		//------------------------- Set default of Variables ---------------------//
		monster[i].frameTimer 		= 0;
		monster[i].frameSpeed 		= 5;
		monster[i].frame 			= 0;
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
		monster[i].follow 			= false;
		monster[i].alert 			= false;
		monster[i].attack 			= false;
		monster[i].thinkTimer		= 0;
		monster[i].thinkTimer		= 0;
		//monster[i].health 			= 100;
		//monster[i].healthDecay 		= 100;
		//monster[i].maxHealth 		= 100;
		monster[i].damage 			= 75;
		monster[i].atkSpe 			= 10;
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
		monster[i].rJ   = 0;
		monster[i].spell.clear();				// if we don't clear every time, the instances will remain
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
			monster[i].spell.clear();				// if we don't clear every time, the instances will remain
			SetSpells(monster, i);					// Load specific spell for that Monster being spawned
			monster[i].rJ   = 0;
			monster[i].frameTimer 		= 0;
			//monster[i].frameSpeed 		= 5;
			monster[i].frame 			= 0;
			monster[i].facing 			= 2;
			monster[i].angle 			= 0.0;
			monster[i].vX 				= 0.0;
			monster[i].vY 				= 0.0;
			monster[i].velX 			= 0.0;
			monster[i].velY 			= 0.0;
			monster[i].timer 			= 0.0;
			monster[i].distance 		= 0.0;
			monster[i].collision 		= false;
			monster[i].onScreen 		= false;
			monster[i].follow 			= false;
			monster[i].alert 			= false;
			monster[i].attack 			= false;
			monster[i].mouse 			= false;
			monster[i].mouseBox 		= false;
			monster[i].thinkTimer		= 0;
			monster[i].damage 			= 75;
			monster[i].atkSpe 			= 10;
			//------------------------- Set default of Variables ---------------------//
			//------------------------------------------------------------------------//
			monster[i].tag 				= "enemy";
			monster[i].x 				= x;
			monster[i].y 				= y;
			//monster[i].w 				= w;
			//monster[i].h 				= h;
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
			//monster[i].health 			= health;
			//monster[i].healthDecay 		= health;
			//monster[i].maxHealth 		= health;
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

void Monster::Update(Monster monster[], Particle &part, Particle particle[], Player &player, Mix_Chunk* sLazer,
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
			float bmx  = playerX+playerW/2;
			float bmy  = playerY+playerH/2;
			float bmx2 = monster[i].x+monster[i].w/2;
			float bmy2 = monster[i].y+monster[i].h/2;
			//double barrelW  = (57 * cos(radians) ) - (16 * sin(radians) );				// add this to center of monster (this will give us the guns barrel position)
			//double barrelH  = (57 * sin(radians) ) + (16 * cos(radians) );
			// Distance from Target
			monster[i].distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
			if (monster[i].distance <= 1) { monster[i].distance = 1; }
			// If target is within eye sight and not attacking, get angle towards target and follow
			if (monster[i].distance <= monster[i].sightRange && !monster[i].attack) {
				// Face towards target
				monster[i].angle = atan2(bmy - bmy2,bmx - bmx2);
				monster[i].angle = monster[i].angle * (180 / 3.1416);
				if (monster[i].angle < 0) { monster[i].angle = 360 - (-monster[i].angle); }
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
				// Follow target
				monster[i].follow = true;
			}else{
				monster[i].follow = false;
			}

			// If we're following the target
			if (monster[i].follow && !monster[i].attack) {
				// If greater than atkRange get velocity towards target
				if (monster[i].distance >= monster[i].atkRange) {
					// If target is greater than attack range, get correct move velocity towards target
					float radians   = (3.1415926536/180)*( monster[i].angle);
					float Cos 		= floor(cos(radians)*100+0.05)/100;
					float Sin 		= floor(sin(radians)*100+0.05)/100;
					monster[i].targetDistanceX = sqrt((bmx - bmx2) * (bmx - bmx2));
					monster[i].targetDistanceY = sqrt((bmy - bmy2) * (bmy - bmy2));
					if (monster[i].targetDistanceX < 0.1) { monster[i].targetDistanceX = 0.1; }
					if (monster[i].targetDistanceY < 0.1) { monster[i].targetDistanceY = 0.1; }
					// Get correct velocity towards Target
					monster[i].vX = 0.25 * (bmx - bmx2) / monster[i].targetDistanceX;
					monster[i].vY = 0.25 * (bmy - bmy2) / monster[i].targetDistanceY;
					// Do moving animations
					monster[i].frameTimer += monster[i].frameSpeed;
					if (monster[i].frameTimer > 60) {
						monster[i].frameTimer = 0.0;
						monster[i].frame++;
						if (monster[i].frame > 1) {
							monster[i].frame = 0;
						}
					}
				}else{
					monster[i].vX = 0;
					monster[i].vY = 0;
				}
			}
			// Monster not attacking
			else{
				monster[i].vX = 0;
				monster[i].vY = 0;
			}

			// random index spell to cast
			if (!monster[i].attack && !monster[i].cooldown) {
				if (monster[i].distance <= monster[i].atkRange) {
					monster[i].rJ = 2;
					// Reset cooldown timer at 0
					monster[i].cooldownTimer = 0;
					// Start spell attack
					monster[i].attack = true;
				}
			}

			// Attack if in range with target
			/*if (!monster[i].attack) {
				rJ = rand() % monster[i].spell.size();
				if (monster[i].distance <= monster[i].atkRange) {
					// attack
					if (!monster[i].spell[rJ].cooldown) {
						monster[i].attack = true;
						//Mix_PlayChannelTimed(-1, sLazer, -1, 5000);
						if ( rand() % 2 ) {
							//rJ = 0;
						}else{
						//	rJ = 1;
						}
						//rJ = rand() % monster[i].spell.size();
						//std::cout << "Size: " << monster[i].spell.size() << std::endl;
						//std::cout << "rJ: " << rJ << std::endl;
					}else{
					//	monster[i].attack = false;
					}
				}else{
					//monster[i].attack = false;
				}
			}*/

			// Do attacks
			if (monster[i].attack) {
				// Do spell duration count down
				if (monster[i].currentDuration > 0) {
					monster[i].currentDuration -= 1;








					// Do attacks per frame
					for (int l = 0; l < monster[i].maxDuration; l += monster[i].maxDuration/monster[i].spell[ monster[i].rJ ].occurances) {
						if (monster[i].currentDuration == l) {
							int randProj = rand() % monster[i].spell[monster[i].rJ].projectiles + 5;
							for (double h = 0.0; h < monster[i].spell[ monster[i].rJ ].scope; h += monster[i].spell[ monster[i].rJ ].scope/randProj) {
								if (monster[i].type < 12) {
									int rands = 4;
									float spe = randDouble(monster[i].spell[ monster[i].rJ ].minSpe, monster[i].spell[ monster[i].rJ ].maxSpe);
									float size = randDouble(monster[i].spell[ monster[i].rJ ].minSize, monster[i].spell[ monster[i].rJ ].maxSize);
									float finalAngle = (monster[i].angle) + h - (monster[i].spell[ monster[i].rJ ].scope/2);
									float newX = monster[i].x + monster[i].w / 2 - size/2;
									float newY = monster[i].y + monster[i].h / 2 - size/2;
									// Spawn Spell as a particle
									part.spawnParticleAngle(particle, monster[i].tag, monster[i].spell[ monster[i].rJ ].type,
											newX, newY,
											size, size,
											finalAngle, spe,
											monster[i].spell[ monster[i].rJ ].damage,
											monster[i].spell[ monster[i].rJ ].color, 1,
											monster[i].spell[ monster[i].rJ ].dir, monster[i].spell[ monster[i].rJ ].dirSpe,
											monster[i].spell[ monster[i].rJ ].alpha, monster[i].spell[ monster[i].rJ ].alphaSpe,
											monster[i].spell[ monster[i].rJ ].deathTimer, monster[i].spell[ monster[i].rJ ].deathTimerSpe,
											monster[i].spell[ monster[i].rJ ].sizeDeath, monster[i].spell[ monster[i].rJ ].sizeDeathSpe,
											monster[i].spell[ monster[i].rJ ].decay, monster[i].spell[ monster[i].rJ ].decaySpe,
											monster[i].spell[ monster[i].rJ ].trail, monster[i].spell[ monster[i].rJ ].trailRate, monster[i].spell[ monster[i].rJ ].trailColor,
											monster[i].spell[ monster[i].rJ ].trailMinSize, monster[i].spell[ monster[i].rJ ].trailMaxSize);
								}
								if (monster[i].type == 12) {
									int rands = 4;
									float newX;
									float newY = monster[i].y + monster[i].h / 2;
									float spe = randDouble(monster[i].spell[ monster[i].rJ ].minSpe, monster[i].spell[ monster[i].rJ ].maxSpe);
									float size = randDouble(monster[i].spell[ monster[i].rJ ].minSize, monster[i].spell[ monster[i].rJ ].maxSize);
									float shootAngle = 0.0;
									if (monster[i].angle > 270 || monster[i].angle < 90) {
										shootAngle = 0;
										newX = monster[i].x + monster[i].w + 2 - size/2;
									} else {
										shootAngle = 180;
										newX = monster[i].x - 2 - size/2;
									}
									float finalAngle = (shootAngle) + h - (monster[i].spell[ monster[i].rJ ].scope/2);
									// Spawn Spell as a particle
									part.spawnParticleAngle(particle, monster[i].tag, monster[i].spell[ monster[i].rJ ].type,
											newX, newY,
											size, size,
											finalAngle, spe,
											monster[i].spell[ monster[i].rJ ].damage,
											monster[i].spell[ monster[i].rJ ].color, 1,
											monster[i].spell[ monster[i].rJ ].dir, monster[i].spell[ monster[i].rJ ].dirSpe,
											monster[i].spell[ monster[i].rJ ].alpha, monster[i].spell[ monster[i].rJ ].alphaSpe,
											monster[i].spell[ monster[i].rJ ].deathTimer, monster[i].spell[ monster[i].rJ ].deathTimerSpe,
											monster[i].spell[ monster[i].rJ ].sizeDeath, monster[i].spell[ monster[i].rJ ].sizeDeathSpe,
											monster[i].spell[ monster[i].rJ ].decay, monster[i].spell[ monster[i].rJ ].decaySpe,
											monster[i].spell[ monster[i].rJ ].trail, monster[i].spell[ monster[i].rJ ].trailRate, monster[i].spell[ monster[i].rJ ].trailColor,
											monster[i].spell[ monster[i].rJ ].trailMinSize, monster[i].spell[ monster[i].rJ ].trailMaxSize);
								}

							}
							// play audio
							Mix_PlayChannel(2, sLazer, 0);
							//Mix_FadeInChannelTimed(-1, sLazer, 1, 1, 1);
						} // end check for specific duration
					}		// end for k check









				}
				// Duration time is up, stop doing spell
				else {
					// Set Spell to no longer be on cooldown
					monster[i].attack = false;
					// Start cooldown handling
					monster[i].cooldown = true;
				}

				// Attack is happening, do countdown
				/*if (monster[i].duration > 0) {
				 monster[i].duration -= 1;
				 // Do attacks per frame
				 for (int l=0; l <=180; l += 30) {
				 if (monster[i].duration == l) {
				 for (double h=0.0; h< 180.0; h+=rand() % 5 + 2){
				 int rands = randDouble(4, 4);
				 float newX;
				 float newY = monster[i].y+monster[i].h/2;
				 float shootAngle = 0.0;
				 if (monster[i].angle > 270 || monster[i].angle < 90) {
				 shootAngle = 0;
				 newX = monster[i].x+monster[i].w + 5;
				 }else{
				 shootAngle = 180;
				 newX = monster[i].x- 5;
				 }
				 part.spawnParticleAngle(particle, "none", 4,
				 newX-rands/2,
				 newY-rands/2,
				 rands, rands,
				 (shootAngle)+h-90, randDouble(0.6, 0.6),
				 0.0,
				 {255, 200, 25}, 1,
				 1, 1,
				 200, 5,
				 60, 0,
				 true, 0.04);
				 }
				 // play audio
				 Mix_PlayChannel(-1, sLazer, 0);
				 } // end check for specific duration
				 }		// end for k check
				 }*/
				// Attack duration done, stop attack
				//else{
				//	monster[i].attack = false;
				//}
			}
			// Not attacking, do spell cooldowns
			else{
				// If spell is on cooldown
				if (monster[i].cooldown) {
					// If spell cooldown timer is not the same as baseCooldown, keep ticking
					if (monster[i].cooldownTimer < monster[i].baseCooldown) {
						// Begin cooldown count down
						monster[i].cooldownTimer += 1.0;
					}
					// If cool down is equal to baseCooldown, the PMonster may cast that spell again
					else{
						// Set Spell to no longer be on cooldown
						monster[i].cooldown = false;
						// Reset duration
						monster[i].currentDuration = monster[i].maxDuration;
					}
				}
				/*// If spell is on cooldown
				if (monster[i].spell[ monster[i].rJ ].cooldown) {
					// If spell cooldown timer is not the same as baseCooldown, keep ticking
					if (monster[i].spell[ monster[i].rJ ].cooldownTimer < monster[i].spell[ monster[i].rJ ].baseCooldown) {
						// Begin cooldown count down
						monster[i].spell[ monster[i].rJ ].cooldownTimer += 1.0;
					}
					// If cool down is equal to baseCooldown, the PMonster may cast that spell again
					else{
						// Set Spell to no longer be on cooldown
						monster[i].spell[ monster[i].rJ ].cooldown = false;
						// Reset duration
						monster[i].spell[ monster[i].rJ ].currentDuration = monster[i].spell[ monster[i].rJ ].maxDuration;
					}
				}*/
			}

			//-----------------------------------------------------------------------------------//

			// Always get center of an object after it has moved
			monster[i].x2 = monster[i].x+monster[i].w/2;
			monster[i].y2 = monster[i].y+monster[i].h/2;

			// monster screen check
			if (monster[i].x + monster[i].w >= camx && monster[i].x <= camx + screenWidth &&
				monster[i].y + monster[i].h >= camy && monster[i].y <= camy + screenHeight) {
				monster[i].onScreen = true;
			}else{
				monster[i].onScreen = false;
			}

			//-----------------------------------------------------------------------------------//
			/*for (unsigned int j=0; j<monster[i].spell.size(); j++) {
				//double barrelW  = (57 * cos(radians) ) - (16 * sin(radians) );				// add this to center of monster (this will give us the guns barrel position)
				//double barrelH  = (57 * sin(radians) ) + (16 * cos(radians) );
				if (monster[i].spell[j].activate) {
					// If Player has enough mana, cast spell, otherwise stop spell casting
					//if (mana > monster[i].spell[j].manaCost) {
						// If spell not on cooldown, cast spell
						if (!monster[i].spell[j].cooldown) {
							// Start cooldown
							monster[i].spell[j].cooldown = true;
							// Reset cooldown timer
							monster[i].spell[j].cooldownTimer = 0;
							// Determine how many spell blasts will be shot at per shot
							for (double k=0; k<monster[i].spell[j].projectiles; k += 1) {
								// Spawn Spell as a particle
								part.spawnParticleAngle(particle, monster[i].tag, monster[i].spell[j].type,
										monster[i].x2 - monster[i].spell[j].size/2,
										monster[i].y2 - monster[i].spell[j].size/2,
										monster[i].spell[j].size, monster[i].spell[j].size,
										monster[i].angle + k * (monster[i].spell[j].scope/monster[i].spell[j].projectiles) - monster[i].spell[j].scope/2, monster[i].spell[j].speed,
										monster[i].spell[j].damage,
										monster[i].spell[j].color, 1,
										monster[i].spell[j].dir, monster[i].spell[j].dirSpe,
										monster[i].spell[j].alpha, monster[i].spell[j].alphaSpe,
										monster[i].spell[j].deathTimer, monster[i].spell[j].deathTimerSpe,
										monster[i].spell[j].sizeDeath, monster[i].spell[j].sizeDeathSpe,
										monster[i].spell[j].decay, monster[i].spell[j].decaySpe,
										monster[i].spell[j].trail, monster[i].spell[j].trailRate, monster[i].spell[j].trailColor,
										monster[i].spell[j].trailMinSize, monster[i].spell[j].trailMaxSize);
							}
							// Subtract mana from Player
							//mana -= monster[i].spell[j].manaCost;
							// play audio
							Mix_PlayChannel(-1, sLazer, 0);
						}
						// Spell is on cooldown, handle timer
						else if (monster[i].spell[j].cooldown) {
							if (monster[i].spell[j].cooldownTimer < monster[i].spell[j].baseCooldown) {
								// Begin cooldown count down
								monster[i].spell[j].cooldownTimer += 1.0;
							}
							// If cool down is equal to baseCooldown0, the Player may activate again
							else{
								// Set Spell to no longer be on cooldown
								monster[i].spell[j].cooldown = false;
								// Stop casting certain spell
								monster[i].spell[j].activate = false;
							}
						}
					// Not enough mana to cast spell, stop activation
					//} else {
						// Stop spell actication
					//	monster[i].spell[j].activate = false;
						// Reset cooldown timer
					//	monster[i].spell[j].cooldownTimer = monster[i].spell[j].baseCooldown;
					//}
				}
			}*/

			//-----------------------------------------------------------------------------------//



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

			// monster death
			if (monster[i].health <= 0)
			{
				player.score += 20;
				monster[i].alive = false;
				count--;
			}

			// monster circle collision check with other zombies
			for (int j = 0; j < max; j++) {
				if (i !=j) {
					if (monster[j].alive) {
						float bmx = monster[j].x2;
						float bmy = monster[j].y2;
						float bmx2 = monster[i].x2;
						float bmy2 = monster[i].y2;
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
		}	// end check alive
	}
}

void Monster::UpdatMinions(Monster monster[], Particle &part, Particle particle[], float x, float y) {
	for (int i = 0; i < max; i++) {
		//if (monster[i].alive && monster[i].type < 12) {
		//}	// end check alive
	}
}

void Monster::UpdateBoss(Monster monster[], Particle &part, Particle particle[], float x, float y) {
	for (int i = 0; i < max; i++) {
	//	if (monster[i].alive && monster[i].type == 12) {

			//-----------------------------------------------------------------------------------//
	//	}	// end check alive
	}
}

void Monster::RenderBehind(SDL_Renderer* gRenderer, Monster monster[], int camx, int camy, float targetY, float targetH) {
	int playerY = targetY - 9;
	int playerH = 16;
	for (int i = 0; i < max; i++) {
		// Render 16x16 monsters
		if (monster[i].type >= 0 && monster[i].type < 12) {
			if (monster[i].alive) {
				if (monster[i].y+monster[i].h <= playerY+playerH) {
					// Render all Monsters
					gMonster.setAlpha(255);
					// Monster moving, render correct frame
					if (monster[i].follow) {
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
		// Render Blue Dragon Boss
		else if (monster[i].type == 12) {
			if (monster[i].alive) {
				if (monster[i].y+monster[i].h <= playerY+playerH) {
					// Render all Monsters
					gMonster.setAlpha(255);
					// Determine which direction Monster should face
					SDL_RendererFlip flip = SDL_FLIP_NONE;
					// Player is right side of Blue Dragon
					if (monster[i].angle > 270 || monster[i].angle < 90) {
						flip = SDL_FLIP_NONE;
					}
					// Target is left side of Blue Dragon
					else{
						flip = SDL_FLIP_HORIZONTAL;
					}
					// Monster attacking, render correct frame
					if (monster[i].attack) {
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&dragonClips[2], 0.0, NULL, flip);
					}
					// Monster moving, render correct frame
					else if (monster[i].follow) {
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&dragonClips[monster[i].frame], 0.0, NULL, flip);
					}
					// Monster stopped moving, render still sprite
					else{
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&dragonClips[0], 0.0, NULL, flip);
					}
				}
			}
			// Render Monster death sprite
			else{
				/*if (monster[i].y+monster[i].h <= playerY+playerH) {
					gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
							monster[i].w, monster[i].h,
							&dragonClips[monster[i].frame]);
				}*/
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
					if (monster[i].follow) {
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
		// Render Blue Dragon Boss
		else if (monster[i].type == 12) {
			if (monster[i].alive) {
				if (monster[i].y+monster[i].h > playerY+playerH) {
					// Render all Monsters
					gMonster.setAlpha(255);
					// Determine which direction Monster should face
					SDL_RendererFlip flip = SDL_FLIP_NONE;
					// Player is right side of Blue Dragon
					if (monster[i].angle > 270 || monster[i].angle < 90) {
						flip = SDL_FLIP_NONE;
					}
					// Target is left side of Blue Dragon
					else{
						flip = SDL_FLIP_HORIZONTAL;
					}
					// Monster attacking, render correct frame
					if (monster[i].attack) {
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&dragonClips[2], 0.0, NULL, flip);
					}
					// Monster moving, render correct frame
					else if (monster[i].follow) {
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&dragonClips[monster[i].frame], 0.0, NULL, flip);
					}
					// Monster stopped moving, render still sprite
					else{
						gMonster.render(gRenderer, monster[i].x-camx, monster[i].y-camy,
								monster[i].w, monster[i].h,
								&dragonClips[0], 0.0, NULL, flip);
					}
				}
			}
			// Render Monster death sprite
			else{

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

void Monster::SetSpells(Monster monster[], int i) {
	// SPeciifics
	if (monster[i].type == 0) {
		monster[i].currentDuration = 1;
		monster[i].maxDuration = 1;
		monster[i].cooldownTimer = 0;
		monster[i].baseCooldown = 60*2;
		monster[i].cooldown = false;
		monster[i].spell.push_back( Spell("Skely Blast", 4,
								1, 1.0,
								1,
								4, 4,
								0.6, 0.6,
								10, {180,20,180},
								0, 0,
								255, 0,
								60, 0.33,
								false, 0.0,
								false, 0.0,
								false, 7, {180,20,180},
								1, 6,
								0) );
	}
	if (monster[i].type == 1) {
		monster[i].currentDuration = 1;
		monster[i].maxDuration = 1;
		monster[i].cooldownTimer = 0;
		monster[i].baseCooldown = 60*2;
		monster[i].cooldown = false;
		monster[i].spell.push_back( Spell("Goblin Blast", 4,
							    3, 45.0,
								1,
								4, 4,
								0.5, 0.5,
								5, {20,180,20},
								0, 0,
								255, 0,
								60, 0.33,
								false, 0.0,
								false, 0.0,
								false, 7, {20,180,20},
								1, 6,
								0) );
	}
	if (monster[i].type == 2) {
		monster[i].currentDuration = 60;
		monster[i].maxDuration = 60;
		monster[i].cooldownTimer = 0;
		monster[i].baseCooldown = 60*3;
		monster[i].cooldown = false;
		monster[i].spell.push_back( Spell("Ghost Mage Cull", 4,
								9, 360.0,
								3,
								4, 4,
								0.7, 0.7,
								2.5, {180,20,20},
								0, 0,
								255, 0,
								60, 0.33,
								false, 0.0,
								false, 0.0,
								false, 7, {180,20,20},
								1, 6,
								0) );
	}
	// Set most Monsters ranges
	if (monster[i].type < 12) {
		monster[i].w = 16;
		monster[i].h = 16;
		monster[i].atkRange = 48;
		monster[i].sightRange = 100;
		monster[i].health = 100;
		monster[i].healthDecay = 100;
		monster[i].maxHealth = 100;
	}
	// Set Boss ranges
	if (monster[i].type == 12) {
		monster[i].w = 48;
		monster[i].h = 16;
		monster[i].frameSpeed = 5;
		monster[i].atkRange = 64;
		monster[i].sightRange = 10000;
		monster[i].health = 2500;
		monster[i].healthDecay = 2500;
		monster[i].maxHealth = 2500;
		monster[i].currentDuration = 60*3;
		monster[i].maxDuration = 60*3;
		monster[i].cooldownTimer = 0;
		monster[i].baseCooldown = 60*3;
		monster[i].cooldown = false;

		monster[i].spell.push_back( Spell("Heat Waves", 4,
								180, 180.0,
								6,
								4, 6,
								1.4, 1.4,
								2.5, {180,20,20},
								0, 0,
								255, 5,
								60, 0,
								false, 0.0,
								false, 0.0,
								false, 5, {180,20,20},
								4, 4,
								0) );
		monster[i].spell.push_back( Spell("Ice Blast", 4,
								10, 135.0,
								100,
								2, 4,
								0.3, 2.3,
								2.5, {20,20,180},
								1, 1,
								255, 5,
								60, 0,
								false, 0.0,
								false, 0.0,
								false, 5, {20,20,180},
								4, 4,
								0) );
		monster[i].spell.push_back( Spell("Acid Blast", 4,
								10, 135.0,
								30,
								2, 4,
								0.1, 9.6,
								2.5, {20,180,20},
								1, 1,
								255, 2,
								60, 0,
								true, 0.02,
								true, 0.25,
								false, 5, {20,180,20},
								4, 4,
								0) );
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
				fileTileDataL >>  monster[i].x 				>>
								  monster[i].y 				>>
								  monster[i].w 				>>
								  monster[i].h 				>>
								  monster[i].health 		>>
								  monster[i].sightRange		>>
								  monster[i].atkRange 		>>
								  monster[i].tag 			>>
								  monster[i].id 			>>
								  monster[i].type 			>>
								  monster[i].alive;
				break;
			}
		}
	}
	fileTileDataL.close();

	// After loading Enemies, set their Spells
	for (int i = 0; i < max; i++) {
		if (monster[i].alive) {
			SetSpells(monster, i);
		}
	}
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
					 << monster[i].w  			<< " "
					 << monster[i].h  			<< " "
					 << monster[i].health  		<< " "
					 << monster[i].sightRange  	<< " "
					 << monster[i].atkRange  	<< " "
					 << monster[i].tag  		<< " "
					 << monster[i].id  			<< " "
					 << monster[i].type  		<< " "
			   	   	 << monster[i].alive 		<< "\n";
		}
	}
	return tempData.str().c_str();
}

