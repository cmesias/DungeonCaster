/*
 * Particle.cpp
 *
 *  Created on: Dec 16, 2016
 *      Author: Carl
 */

#include "Particle.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

#include "../Player.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// ENEMY BULLETS //////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//

void Particle::load(SDL_Renderer* gRenderer){
	setClips(cParticles[0], 0, 0, 8, 8);
	setClips(cParticles[1], 8, 0, 8, 8 );
	gParticles.loadFromFile(gRenderer, "resource/gfx/particles.png");
	gParticles.setBlendMode(SDL_BLENDMODE_ADD);
	sGrenadeExplode = Mix_LoadWAV("sounds/snd_grenade_explode.wav");
}

void Particle::free(){
	gParticles.free();
	Mix_FreeChunk(sGrenadeExplode);
	sGrenadeExplode 		= NULL;
}

void Particle::init(Particle particle[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		particle[i].x 				= 0;
		particle[i].y 				= 0;
		particle[i].w 				= 4;
		particle[i].h 				= 4;
		particle[i].alpha 			= 255;
		particle[i].time 			= 0;
		particle[i].timeri 			= 0;
		particle[i].alphaspeed 		= 0;
		particle[i].deathTimer 		= 100;
		particle[i].deathTimerSpeed = 1;
		particle[i].speed 			= 0.00;
		particle[i].vX 				= 0.00;
		particle[i].vY 				= 0.00;
		particle[i].speed 			= 0.00;
		particle[i].angle 			= 0.00;
		particle[i].angleSpe 		= 0;
		particle[i].angleDir 		= 0;
		particle[i].onScreen 		= false;
		particle[i].collide 		= false;
		particle[i].decay 			= false;
		particle[i].decaySpeed 		= 0.0;
		particle[i].alive 			= false;
		particle[i].sizeDeath 		= false;
		particle[i].deathSpe 		= 0;
		particle[i].trail 			= false;
		particle[i].trailRate 		= 0;
		particle[i].side 			= "";
		particle[i].type 			= 0;
		particle[i].damage 			= 0;
		particle[i].color 			= { 255, 255, 255, 255 };
	}
}
void Particle::RemoveAll(Particle particle[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		particle[i].alive 			= false;
	}
}

void Particle::spawnParticleAngle(Particle particle[], std::string tag, int type,
		float spawnX, float spawnY,
		float spawnW, float spawnH,
		double angle, double speed,
		double damage,
		SDL_Color color, int layer,
		int angleSpe, int angleDir,
		double alpha, double alphaspeed,
		double deathTimer, double deathTimerSpeed,
		bool sizeDeath, float deathSpe,
		bool decay, float decaySpeed,
		bool trail, float trailRate, SDL_Color trailColor,
		float trailMinSize, float trailMaxSize) {
	for (int i = 0; i < max; i++)
	{
		if (!particle[i].alive)
		{
			particle[i].tag 			= tag;
			particle[i].type 			= type;
			particle[i].x 				= spawnX;
			particle[i].y 				= spawnY;
			particle[i].w 				= spawnW;
			particle[i].h 				= spawnH;
			particle[i].x2 				= spawnX + spawnW/2;
			particle[i].y2 				= spawnY + spawnH/2;
			particle[i].time 			= 0;
			particle[i].timeri 			= 0;
			particle[i].angle 			= angle;
			particle[i].speed 			= speed;
			particle[i].vX 				= (cos( (3.14159265/180)*(angle) ));
			particle[i].vY 				= (sin( (3.14159265/180)*(angle) ));
			particle[i].damage 			= damage;
			//particle[i].x 				= spawnX + (rand() % 4 + 2 * (cos( (3.14159265/180)*(angle) )));
			//particle[i].y 				= spawnY + (rand() % 4 + 2 * (sin( (3.14159265/180)*(angle) )));
			//particle[i].x 				= spawnX + cos( (3.14159265/180)*(angle) );
			//particle[i].y 				= spawnY + sin( (3.14159265/180)*(angle) );

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////// Set Corners /////////////////////////////
			float particleCX = spawnX+spawnW/2;
			float particleCY = spawnY+spawnH/2;
			float particleAngle = angle;
			float radians   = (3.1415926536/180)*(particle[i].angle);
			float Cos 		= floor(cos(radians)*100+0.05)/100;
			float Sin 		= floor(sin(radians)*100+0.05)/100;
			// Top Right corner
		    float barrelW  = ((spawnW/2) * Cos ) - (-(spawnH/2) * Sin );
		    float barrelH  = ((spawnW/2) * Sin ) + (-(spawnH/2) * Cos );
		    float barrelX = particleCX + barrelW;
		    float barrelY = particleCY + barrelH;
			particle[i].A.x = barrelX;
			particle[i].A.y = barrelY;
			// Bottom Right corner
			barrelW  = ((spawnW/2) * Cos ) - ((spawnH/2) * Sin );
			barrelH  = ((spawnW/2) * Sin ) + ((spawnH/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			particle[i].B.x = barrelX;
			particle[i].B.y = barrelY;
			// Top Left corner
			barrelW  = (-(spawnW/2) * Cos ) - (-(spawnH/2) * Sin );
			barrelH  = (-(spawnW/2) * Sin ) + (-(spawnH/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			particle[i].C.x = barrelX;
			particle[i].C.y = barrelY;
			// Bottom Left corner
			barrelW  = (-(spawnW/2) * Cos ) - ((spawnH/2) * Sin );
			barrelH  = (-(spawnW/2) * Sin ) + ((spawnH/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			particle[i].D.x = barrelX;
			particle[i].D.y = barrelY;
			//////////////////////////////// Set Corners /////////////////////////////
			//////////////////////////////////////////////////////////////////////////

			particle[i].side 			= "";
			particle[i].onScreen 		= false;
			particle[i].collide 		= false;
			particle[i].decay 			= decay;
			particle[i].decaySpeed 		= decaySpeed;
			particle[i].color 			= color;
			particle[i].layer 			= layer;
			particle[i].alphaspeed 		= alphaspeed;
			particle[i].alpha 			= alpha;
			particle[i].deathTimer 		= deathTimer;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].angleSpe		= angleSpe;
			particle[i].angleDir		= angleDir;
			particle[i].sizeDeath 		= sizeDeath;
			particle[i].deathSpe 		= deathSpe;
			particle[i].trail 			= trail;
			particle[i].trailTimer 		= 0;
			particle[i].trailRate 		= trailRate;
			particle[i].trailColor 		= trailColor;
			particle[i].trailMinSize 	= trailMinSize;
			particle[i].trailMaxSize 	= trailMaxSize;
			particle[i].alive 			= true;
			count++;
			break;
		}
	}
}

// Spawn a Particle: moves based on a fixed velocity given and fixed speed
/*void Particle::fireParticle(Particle particle[], int type, int damage,
									   int spawnx, int spawny, int w, int h,
									   double speed, float vX, float vY,
									   int alphaspeed, int alpha,
									   int deathTimer, int deathTimerSpeed, double angle, double angleSpe, double angleDir,
									   SDL_Color color, int layer) {
	for (int i = 0; i < 1024; i++) {
		if (!particle[i].alive) {
			particle[i].damage 			= damage;
			particle[i].type 			= type;
			particle[i].alphaspeed 		= alphaspeed;
			particle[i].alpha  			= alpha;
			particle[i].x  				= spawnx;
			particle[i].y  				= spawny;
			particle[i].w  				= w;
			particle[i].h  				= h;
			particle[i].speed  			= speed;
			particle[i].angle 			= angle;
			particle[i].angleSpe		= angleSpe;
			particle[i].angleDir		= angleDir;
			particle[i].vX  			= vX;
			particle[i].vY  			= vY;
			particle[i].deathTimer 		= deathTimer;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].sizeDeath 		= false;
			particle[i].deathSpe 		= 0;
			particle[i].time 			= 0;
			particle[i].side 			= "";
			particle[i].onScreen 		= false;
			particle[i].collide 		= false;
			particle[i].alive 			= true;
			particle[i].color 			= color;
			particle[i].layer 			= layer;
			//particle[i].angle 		= atan2 ( radianSin, radianCos) * 180 / PI;
			PARTICLES++;
			break;
		}
	}
}*/

// Generate Stars
void Particle::genStars(Particle particle[], int startX, int startY, int endW, int endH){
	//this->ptimer += 1;
	//if (this->ptimer > 10){
	//	this->ptimer = 0;
	for (int i=0; i<65; i++){
		int randl = rand() % 5 + 3;
		int rands = rand() % 3 + 4;
		int randW = rand() % 2304;
		int randH = rand() % 1296;
		//iint randW = rand() % 8000;
		//iint randH = rand() % 3300;
		int randc = rand() %  100 + 150;
		SDL_Color tempColor = {randc,randc,randc};
		spawnParticleAngle(particle, "none", 2,
				startX/randl + randW,
				startY/randl + randH,
						   10 - randl, 10 - randl,
						   0.0, 0.0,
						   0,
						   tempColor, randl,
						   1, 1,
						   rand() % 100 + 50, randDouble(0.5, 1.1),
						   100, 0,
						   true, 0.08);
	}



		/*for (int i=0; i<65; i++){
			int randAlpha = rand() % 100+150;
			int cW = screnWidth + 200;
			int cH = screenHeight + 200;
			int randW = rand() % cW;
			int randH = rand() % cH;
			SDL_Color tempColor = { rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1};
			int randl = rand() % 5 + 5;
			int randw = rand() % 2 + 2;

			fireParticle(particle, 2, 0,
									0-100 + randW, 0-100 + randH,
									10/randl, 10/randl,
									0,
									0.0,0.0, 1, randAlpha,
									100, 1, 0.0, 0.0, 0.0,
									tempColor, randl);
		}*/
	//}
}

// Spawn a Particle: moves based on location of a target given
/*void Particle::fireParticle2(Particle particle[], int type, int damage, int spawnx, int spawny, int w, int h, int targetx,int targety,
										int alphaspeed, int alpha,
										int deathTimer, int deathTimerSpeed,
										double speed,
										SDL_Color color, int layer) {
	for (int i = 0; i < 1024; i++) {
		if (!particle[i].alive) {
			particle[i].damage 			= damage;
			particle[i].alpha  			= alpha;
			particle[i].deathTimer 		= deathTimer;
			particle[i].alphaspeed 		= alphaspeed;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].type 			= type;
			particle[i].x 				= spawnx;
			particle[i].y 				= spawny;
			particle[i].w 				= w;
			particle[i].h 				= h;
			particle[i].speed 			= speed;
			float bmx 					= targetx;
			float bmy 					= targety;
			float distance 				= sqrt((bmx - spawnx) * (bmx - spawnx - particle[i].w / 2) +
											   (bmy - spawny) * (bmy - spawny - particle[i].h / 2));
			particle[i].vX 				= particle[i].speed  *  (bmx - spawnx - particle[i].w / 2) / distance;
			particle[i].vY 				= particle[i].speed  *  (bmy - spawny - particle[i].h / 2) / distance;
			particle[i].angle 			= atan2(bmy - particle[i].y, bmx - particle[i].x);
			particle[i].angle 			= particle[i].angle * (180 / 3.1416);
			particle[i].angleSpe		= 0;
			particle[i].angleDir		= 0;
			particle[i].sizeDeath 		= false;
			particle[i].deathSpe 		= 0;
			particle[i].time 			= 0;
			particle[i].side 			= "";
			particle[i].collide 		= false;
			particle[i].onScreen 		= false;
			particle[i].alive 			= true;
			particle[i].color 			= color;
			particle[i].layer 			= layer;
			PARTICLES++;
			break;
		}
	}
}*/

void Particle::Update(Particle particle[], int mapX, int mapY, int mapW, int mapH, Mix_Chunk* sSpellExplode) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive)
		{
			// get particle radius
			particle[i].radius = particle[i].w;

			// Particle movement
			particle[i].x += particle[i].vX * particle[i].speed;
			particle[i].y += particle[i].vY * particle[i].speed;

			// Particle trail
			if (particle[i].trail) {
				particle[i].trailTimer += particle[i].trailRate;
				if (particle[i].trailTimer > 60) {
					particle[i].trailTimer = 0;
					float trajAngle = atan2(particle[i].vY,particle[i].vX);
					trajAngle = trajAngle * (180 / 3.1416);
					//Set player angle max limits
					if (trajAngle < 0) {
						trajAngle = 360 - (-trajAngle);
					}
					float radians = (3.1415926536/180)*(trajAngle);
					////////////////////////////////////////////////
					float barrelW  = (5 * cos(radians) ) - (0 * sin(radians) );	// add this to center of zombie (this will give us the guns barrel position)
					float barrelH  = (5 * sin(radians) ) + (0 * cos(radians) );
					////////////////////////////////////////////////
					// spawn explosion particle effect
					for (double h=0.0; h< 90.0; h+=rand() % 5 + 2){
						int rands = randDouble(particle[i].trailMinSize, particle[i].trailMaxSize);
						float newX = particle[i].x+particle[i].w/2;
						float newY = particle[i].y+particle[i].h/2;
						spawnParticleAngle(particle, "none", 4,
											newX-rands/2 + barrelW,
											newY-rands/2 + barrelH,
										   rands, rands,
										   (trajAngle+180)+h-45, randDouble(1.1, 2.1),
										   0.0,
										   particle[i].trailColor, 1,
										   1, 1,
										   rand() % 100 + 150, rand() % 10 + 10,
										   60, 0,
										   true, 0.11,
										   true, randDouble(0.005, 0.6));
					}
				}
			}

			// Speed decay of grenade
			if (particle[i].decay) {
				particle[i].speed = particle[i].speed - particle[i].speed * particle[i].decaySpeed;
			}

			///////////////////////////////////////////////////////////////////////////////
			/////////////////////////// Set Corners of a Particle /////////////////////////
			//---------------------------------------------------------------------------//
			float particleCX = particle[i].x+particle[i].w/2;
			float particleCY = particle[i].y+particle[i].h/2;
			float particleAngle = particle[i].angle;
			float radians   = (3.1415926536/180)*(particle[i].angle);
			float Cos 		= floor(cos(radians)*100+0.05)/100;
			float Sin 		= floor(sin(radians)*100+0.05)/100;

			// Top Right corner
		    float barrelW  = ((particle[i].w/2) * Cos ) - (-(particle[i].h/2) * Sin );
		    float barrelH  = ((particle[i].w/2) * Sin ) + (-(particle[i].h/2) * Cos );
		    float barrelX = particleCX + barrelW;
		    float barrelY = particleCY + barrelH;
			particle[i].A.x = barrelX;
			particle[i].A.y = barrelY;

			// Bottom Right corner
			barrelW  = ((particle[i].w/2) * Cos ) - ((particle[i].h/2) * Sin );
			barrelH  = ((particle[i].w/2) * Sin ) + ((particle[i].h/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			particle[i].B.x = barrelX;
			particle[i].B.y = barrelY;

			// Top Left corner
			barrelW  = (-(particle[i].w/2) * Cos ) - (-(particle[i].h/2) * Sin );
			barrelH  = (-(particle[i].w/2) * Sin ) + (-(particle[i].h/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			particle[i].C.x = barrelX;
			particle[i].C.y = barrelY;

			// Bottom Left corner
			barrelW  = (-(particle[i].w/2) * Cos ) - ((particle[i].h/2) * Sin );
			barrelH  = (-(particle[i].w/2) * Sin ) + ((particle[i].h/2) * Cos );
			barrelX = particleCX + barrelW;
			barrelY = particleCY + barrelH;
			particle[i].D.x = barrelX;
			particle[i].D.y = barrelY;

			// Particle map collision
			if (particle[i].x+particle[i].w < mapX) {
				if (particle[i].damage > 0) {
					Mix_PlayChannel(-1, sSpellExplode, 0);
					SpawnExplosion(particle, particle[i].x2, particle[i].y2, particle[i].color);
				}
				particle[i].alive = false;
				count--;
				//particle[i].x = mapX+mapW-particle[i].w;
			}
			if (particle[i].x > mapX+mapW) {
				if (particle[i].damage > 0) {
					Mix_PlayChannel(-1, sSpellExplode, 0);
					SpawnExplosion(particle, particle[i].x2, particle[i].y2, particle[i].color);
				}
				particle[i].alive = false;
				count--;
				//particle[i].x = mapX-particle[i].w;
			}
			if (particle[i].y+particle[i].h < mapY) {
				if (particle[i].damage > 0) {
					Mix_PlayChannel(-1, sSpellExplode, 0);
					SpawnExplosion(particle, particle[i].x2, particle[i].y2, particle[i].color);
				}
				particle[i].alive = false;
				count--;
				//particle[i].y = mapY+mapH-particle[i].h;
			}
			if (particle[i].y > mapY+mapH) {
				if (particle[i].damage > 0) {
					Mix_PlayChannel(-1, sSpellExplode, 0);
					SpawnExplosion(particle, particle[i].x2, particle[i].y2, particle[i].color);
				}
				particle[i].alive = false;
				count--;
				//particle[i].y = mapY-particle[i].h;
			}

		}
	}
}

void Particle::updateBulletParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive)
		{
			// particle center
			particle[i].x2 = particle[i].x + particle[i].w/2;
			particle[i].y2 = particle[i].y + particle[i].h/2;

			// Player particle
			if (particle[i].type == 0)
			{
				// get particle radius
				particle[i].radius = particle[i].w;

				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death
				particle[i].time += particle[i].deathTimerSpeed;

				// Update particles angle based on its X and Y velocities
				particle[i].angle = atan2 ( particle[i].vY, particle[i].vX) * 180 / 3.14159265;

				// Particle death
				if (particle[i].time > particle[i].deathTimer) {
					particle[i].alive = false;
					count--;
				}
			}

			// Enemy particle
			if (particle[i].type == 1)
			{
				// get particle radius
				particle[i].radius = particle[i].w;

				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death
				particle[i].time += particle[i].deathTimerSpeed;

				// Particle death
				if (particle[i].time > particle[i].deathTimer) {
					particle[i].alive = false;
					count--;
				}
			}

			// Enemy particle II
			if (particle[i].type == 4)
			{
				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death timer, Time
				particle[i].time += particle[i].deathTimerSpeed;

				// Particle death, Time
				if (particle[i].time > particle[i].deathTimer) {
					particle[i].alive = false;
					count--;
				}

				// Particle death timer, transparency
				particle[i].alpha -= particle[i].alphaspeed;
				if (particle[i].alpha < 0) {
					particle[i].alive = false;
					count--;
				}

				// Particle death upon size
				if (particle[i].sizeDeath) {
					particle[i].w -= particle[i].deathSpe;
					particle[i].h -= particle[i].deathSpe;

					if (particle[i].w <= 0 || particle[i].h <= 0){
						particle[i].alive = false;
						count--;
					}
				}
			}

			// Grenade particle
			/*if (particle[i].type == 3)
			{
				// get particle radius
				particle[i].radius = particle[i].w;

				// Particle movement
				particle[i].x += particle[i].vX * particle[i].speed;
				particle[i].y += particle[i].vY * particle[i].speed;

				if (particle[i].decay) {
					particle[i].speed = particle[i].speed - particle[i].speed * 0.09;
				}

				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death
				particle[i].time += particle[i].deathTimerSpeed;

				// Particle map collision
				if (particle[i].x+particle[i].w < mapX) {
					particle[i].x = mapX+mapW-particle[i].w;
				}
				if (particle[i].x > mapX+mapW) {
					particle[i].x = mapX-particle[i].w;
				}
				if (particle[i].y+particle[i].h < mapY) {
					particle[i].y = mapY+mapH-particle[i].h;
				}
				if (particle[i].y > mapY+mapH) {
					particle[i].y = mapY-particle[i].h;
				}

				// Particle death
				if (particle[i].time > particle[i].deathTimer) {
					// play sound effect
					Mix_PlayChannel(3, sGrenadeExplode, 0);
					// spawn explosion particle effect
					for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
						int rands = rand() % 9 + 2;
						float newX = particle[i].x+particle[i].w/2;
						float newY = particle[i].y+particle[i].h/2;
						spawnParticleAngle(particle, 2,
											newX-rands/2,
											newY-rands/2,
										   rands, rands,
										   h, randDouble(2.1, 9.1),
										   0.0,
										   {255, 144, 10, 255}, 1,
										   1, 1,
										   rand() % 100 + 150, rand() % 2 + 5,
										   rand() % 50 + 90, 0,
										   true, 0.11,
										   rand() % 25 + 2, 0.15);
					}
					for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
						int rands = rand() % 9 + 2;
						float newX = particle[i].x+particle[i].w/2;
						float newY = particle[i].y+particle[i].h/2;
						spawnParticleAngle(particle, 2,
											newX-rands/2,
											newY-rands/2,
										   rands, rands,
										   h, randDouble(2.1, 9.1),
										   0.0,
										   {255, 255, 0, 255}, 1,
										   1, 1,
										   rand() % 100 + 150, rand() % 2 + 5,
										   rand() % 50 + 90, 0,
										   true, 0.11,
										   rand() % 25 + 2, 0.15);
					}
					// remove particle
					particle[i].alive = false;
					count--;
				}
			}*/
		}
	}
}

// Update Particles
void Particle::updateStarParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive)
		{
			// Star particles
			if (particle[i].type == 2)
			{
				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death timer, Time
				particle[i].time += particle[i].deathTimerSpeed;

				// Particle death, Time
				if (particle[i].time > particle[i].deathTimer) {
					particle[i].alive = false;
					count--;
				}

				// Particle death timer, transparency
				particle[i].alpha -= particle[i].alphaspeed;
				if (particle[i].alpha < 0) {
					particle[i].alive = false;
					count--;
				}

				// Particle death upon size
				if (particle[i].sizeDeath) {
					particle[i].w -= particle[i].deathSpe;
					particle[i].h -= particle[i].deathSpe;

					if (particle[i].w <= 0 || particle[i].h <= 0){
						particle[i].alive = false;
						count--;
					}
				}
			}
		}
	}
}

// Render bullets
void Particle::renderBulletParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive) {

			// Render Player lazer particle
			if (particle[i].type == 0) {
				/*SDL_Rect tempRect = {particle[i].x - camX,
						   particle[i].y - camY,
						   particle[i].w,  particle[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255,255,255,255);
				SDL_RenderFillRect(gRenderer, &tempRect);*/

				//gParticles.setAlpha(particle[i].alpha);
				//gParticles.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				//gParticles.render(gRenderer, particle[i].x - camX,particle[i].y - camY,
				//		 particle[i].w, particle[i].h,
				//		 &cParticles[0], particle[i].angle);



				/*gBullet.setBlendMode(SDL_BLENDMODE_ADD);
				gBullet.setAlpha(110);
				gBullet.render(gRenderer, particle[i].x-16 - camX,particle[i].y-16 - camY,
						 particle[i].w+32, particle[i].h+32,
						NULL, particle[i].angle);*/
				gParticles.setAlpha(particle[i].alpha);
				gParticles.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				gParticles.render(gRenderer, particle[i].x - camX/particle[i].layer,particle[i].y - camY/particle[i].layer,
											 particle[i].w, particle[i].h,
											 &cParticles[0], particle[i].angle);
			}

			// Render Enemy lazer particle
			if (particle[i].type == 1) {
				/*int center = 160 - 30;
				gParticles.setAlpha(particle[i].alpha);
				gParticles.setColor(255, 255, 255);
				gParticles.render(gRenderer, particle[i].x-center/2 - camX,
										     particle[i].y-center/2 - camY,
											 particle[i].w,
											 particle[i].h,
											 &cParticles[3],
											 particle[i].angle);*/
				gParticles.setAlpha(particle[i].alpha);
				gParticles.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				gParticles.render(gRenderer, particle[i].x - camX/particle[i].layer,particle[i].y - camY/particle[i].layer,
											 particle[i].w, particle[i].h,
											 &cParticles[0], particle[i].angle);
			}

			// Render Grenade
			if (particle[i].type == 3) {
				/*spr_grenade.setAlpha(255);
				spr_grenade.setColor(255,255,255);
				spr_grenade.render(gRenderer, particle[i].x - camX,
										  particle[i].y - camY,
										  particle[i].w,
										  particle[i].h,
										  NULL,
										  particle[i].angle);*/
				gParticles.setAlpha(particle[i].alpha);
				gParticles.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				gParticles.render(gRenderer, particle[i].x - camX/particle[i].layer,particle[i].y - camY/particle[i].layer,
											 particle[i].w, particle[i].h,
											 &cParticles[0], particle[i].angle);
			}

			// Render Enemy particles
			if (particle[i].type == 4) {
				gParticles.setAlpha(particle[i].alpha);
				gParticles.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				gParticles.render(gRenderer, particle[i].x - camX/particle[i].layer,particle[i].y - camY/particle[i].layer,
											 particle[i].w, particle[i].h,
											 &cParticles[0], particle[i].angle);
			}
		}
	}
}

// Render stars
void Particle::renderStarParticle(Particle particle[], int camx, int camy, float playerZ, SDL_Renderer* gRenderer) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive) {

			// Render stars particle
			if (particle[i].type == 2) {

				gParticles.setAlpha(particle[i].alpha);
				gParticles.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				gParticles.render(gRenderer, particle[i].x - camx/particle[i].layer,particle[i].y - camy/particle[i].layer,
											 particle[i].w, particle[i].h,
											 &cParticles[0], particle[i].angle);

				// Top Right corner
			    /*SDL_Rect tempRect = { particle[i].A.x-1  - camx, particle[i].A.y-1 - camy, 2, 2 };
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Bottom Right corner
				tempRect = { particle[i].B.x-1  - camx, particle[i].B.y-1 - camy, 2, 2 };
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Top Left corner
				tempRect = { particle[i].C.x-1  - camx, particle[i].C.y-1 - camy, 2, 2 };
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Bottom Left corner
				tempRect = { particle[i].D.x-1  - camx, particle[i].D.y-1 - camy, 2, 2 };
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);*/

				/*SDL_Rect tempRect = {particle[i].x - camX/particle[i].layer,particle[i].y - camY/particle[i].layer,
						   particle[i].w,  particle[i].h};
				SDL_SetRenderDrawColor(gRenderer, particle[i].color.r, particle[i].color.g, particle[i].color.b,1);
				SDL_RenderDrawRect(gRenderer, &tempRect);*/
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////// ENEMY BULLETS //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------ Function Extensions-----------------------------------------------------//

void Particle::SpawnExplosion(Particle particle[], float x, float y, SDL_Color explosionColor) {
	for (double j=0.0; j< 360.0; j+=rand() % 10 + 10){
		int rands = rand() % 4 + 4;
		spawnParticleAngle(particle, "none", 4,
							x-rands/2,
							y-rands/2,
						   rands, rands,
						   j, randDouble(2.1, 8.1),
						   0.0,
						   explosionColor, 1,
						   1, 1,
						   rand() % 100 + 150, rand() % 10 + 10,
						   60, 0,
						   true, 0.11,
						   true, randDouble(0.005, 0.6));
	}
}

//------------------------------------------------------ Function Extensions-----------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////