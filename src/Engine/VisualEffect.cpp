/*
 * VisualEffect.cpp
 *
 *  Created on: Aug 28, 2017
 *      Author: Carl
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "VisualEffect.h"

void VisualEffect::PlayVFX(VisualEffect vfx[], Particle &part, Particle particle[]) {
	/*// Do attacks
	for (unsigned int i=0; i<spell.size(); i++) {
		if (spell[i].activate) {
			// Do spell duration count down
			if (spell[i].currentDuration > 0) {
				spell[i].currentDuration -= spell[i].rate;

				// Do attacks per frame
				for (int l = 0; l < spell[i].maxDuration; l += spell[i].maxDuration/spell[i].occurances) {

					if (spell[i].currentDuration == l) {

						int flipDuration = spell[i].maxDuration - spell[i].currentDuration;
						int increment = flipDuration / (spell[i].maxDuration/spell[i].occurances);
						std::cout << "currentDuration: " << spell[i].maxDuration - spell[i].currentDuration
								   << ", maxDuration: " << spell[i].maxDuration
								   << ", occurrences: " << spell[i].occurances
								   << ", increment: " << increment-1 << "\n";
						// Boss
						//int randProj = rand() % spell[i].projectiles;
						for (double k = 0.0; k < spell[i].scope; k += spell[i].scope/spell[i].projectiles) {
							//if (type == 12) {
								int rands = 4;
								float spe = randDouble(spell[i].minSpe, spell[i].maxSpe);
								float size = randDouble(spell[i].minSize, spell[i].maxSize);
								float newX = (x + w / 2) - size/2;
								float newY = (y + h / 2) - size/2;

								switch (spell[i]._spawnType) {
									case Spell::OnCaster:
										newX = (x + w / 2) - size/2;
										newY = (y + h / 2) - size/2;
										break;
									case Spell::OnTarget:
										newX = *target.x - size/2;
										newY = *target.y - size/2;
										break;
									case Spell::OnMouse:
										newX = mx - size/2;
										newY = my - size/2;
										break;
								}


								// If we're not incrementing
								float finalAngle;
								float radians;
								float distanceW = increment * spell[i].distanceW;
								float distanceH = increment * spell[i].distanceH;
								double barrelW = 0;
								double barrelH = 0;
								if (spell[i].increAngleMax == 0) {
									finalAngle = (angle) + k - spell[i].scope/2;
								}
								// If we are incrementing
								else{
									finalAngle = (angle) + k;
									finalAngle += spell[i].increAngle;
									finalAngle -= spell[i].increAngleMax/2;
									radians = ( M_PI/180) * (finalAngle);
									barrelW  = (distanceW * cos(radians) ) - (distanceH * sin(radians) );
									barrelH  = (distanceW * sin(radians) ) + (distanceH * cos(radians) );
								}
								//finalAngle += 13;
								// Spawn Spell as a particle
								part.spawnParticleAngle(particles, tag, spell[i].type,
										newX + barrelW, newY + barrelH,
										size, size,
										finalAngle, spe,
										spell[i].damage,
										spell[i].color, 1,
										spell[i].dir, spell[i].dirSpe,
										spell[i].alpha, spell[i].alphaSpe,
										spell[i].deathTimer, spell[i].deathTimerSpe,
										spell[i].sizeDeath, spell[i].sizeDeathSpe,
										spell[i].decay, spell[i].decaySpe,
										spell[i].trail, spell[i].trailRate, spell[i].trailColor,
										spell[i].trailMinSize, spell[i].trailMaxSize,
										spell[i].timerBeforeMoving,
										false, 0, 0);
							//}

						}
						// play audio
						//Mix_PlayChannel(-1, sLazer, 0);
						//Mix_FadeInChannelTimed(-1, sLazer, 1, 1, 1);
						/// Increment shoot angle after shots
						// Get number of times it will tick until duration is over
						float ticks = spell[i].maxDuration/spell[i].occurances;
						// Then get real ticks by dividing max duration with above ^^^
						ticks = spell[i].maxDuration/ticks;
						// With the number above, set the number og ticks to match increasing the increment angle
						float increValue = spell[i].increAngleMax / ticks;
						// Increment angle to shoot all around but incrementally going up
						if (spell[i].increAngle < spell[i].increAngleMax) {
							spell[i].increAngle += increValue;
						}
					} // end check for specific duration




				}		// end forlk check
			}
			// Duration time is up, stop doing spell
			else {
				// Stop casting so Character can move
				casting = false;
				// Set Spell to no longer be on cooldown
				spell[i].activate = false;
				// Start cooldown handling
				spell[i].cooldown = true;
			}
		}
		// Not attacking, do spell cooldowns
		else{
			// If Boss is on attack cooldown
			if (spell[i].cooldown) {
				// If spell cooldown timer is not the same as baseCooldown, keep ticking
				if (spell[i].cooldownTimer < spell[i].baseCooldown) {
					// Begin cooldown count down
					spell[i].cooldownTimer += 1.0;
				}
				// If cool down is equal to baseCooldown, the PMonster may cast that spell again
				else{
					// Set Spell to no longer be on cooldown
					spell[i].cooldown = false;
					// Reset duration
					spell[i].currentDuration = spell[i].maxDuration;
					// Reset incrementing angle
					spell[i].increAngle = 0.0;
				}
			}
		}
	}*/
}












void VisualEffect::Load(SDL_Renderer* gRenderer){

}

void VisualEffect::Free(){

}

void VisualEffect::Init(VisualEffect vfx[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		vfx[i].x 			= 0;
		vfx[i].y 			= 0;
		vfx[i].w 			= 0;
		vfx[i].h 			= 0;
		vfx[i].duration 	= 0;
		vfx[i].rate 		= 0;
		vfx[i].isAlive 		= false;
	}
}

void VisualEffect::Spawn(VisualEffect vfx[], float x, float y,
		 float w, float h,
		 float maxDuration, int occurrence,
		 float rate) {
	for (int i = 0; i < max; i++){
		if (!vfx[i].isAlive){
			vfx[i].x 			= x;
			vfx[i].y 			= y;
			vfx[i].w 			= w;
			vfx[i].h 			= h;
			vfx[i].maxDuration 	= maxDuration;
			vfx[i].duration 	= maxDuration;
			vfx[i].occurrence 	= occurrence;
			vfx[i].rate 		= rate;
			vfx[i].isAlive 		= true;
			count++;
			break;
		}
	}
}

void VisualEffect::Remove(VisualEffect vfx[]) {
	for (int i = 0; i < max; i++) {
		vfx[i].isAlive = false;
		count = 0;
	}
}

void VisualEffect::Update(VisualEffect vfx[], int mx, int my, int camx, int camy, Particle &part, Particle particle[]) {
	for (int i = 0; i < max; i++) {
		if (vfx[i].isAlive) {



			// Play Visual Effect
			for (int l=0; l<vfx[i].maxDuration; l += vfx[i].maxDuration/vfx[i].occurrence) {
				if (vfx[i].duration == l) {
					float newX = vfx[i].x+vfx[i].w/2;
					float newY = vfx[i].y+vfx[i].h/2;
					for (double j=0; j<360.0; j+= rand() % 10+10) {
						int rands = rand() % 4 + 2;
						int r = rand() % 255;
						int g = rand() % 255;
						int b = rand() % 255;
						SDL_Color color = {r,g,b};
						part.spawnParticleAngle(particle, "none", 4,
											newX-rands/2,
											newY-rands/2,
										   rands, rands,
										   j, randDouble(0.1, 3.1),
										   0,
										   color, 1,
										   1, 1,
										   rand() % 100 + 150, rand() % 2 + 2,
										   rand() % 50 + 90, 0,
										   true, 0.0,
										   true, 0.0);
					}
				}
			}





			// Duration death
			if (vfx[i].duration > 0) {
				vfx[i].duration -= vfx[i].rate;
				// play VFX
			}else{
				vfx[i].isAlive = false;
				count--;
			}
		}
	}
}

void VisualEffect::Render(VisualEffect vfx[], int camx, int camy, SDL_Renderer* gRenderer) {
	for (int i = 0; i < max; i++) {
		if (vfx[i].isAlive) {
			SDL_Rect vfxRect = { vfx[i].x-camx, vfx[i].y-camy, vfx[i].w, vfx[i].h};
			SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &vfxRect);
		}
	}
}


