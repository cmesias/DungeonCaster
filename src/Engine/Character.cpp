/*
 * Character.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: Carl
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Character.h"

Character::Character() {
	// TODO Auto-generated constructor stub
	x = -6000;
	y = -6000;
	w = 300;
	h = 300;
	vX = 0.0;
	vY = 0.0;
	hp = 1;
	angle = 90.0;
	tag = "";
	isAlive = false;
	spellIndex = 0;
	useKey = false;
	camlocked = false;
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	facing = 0;
	frame = 0;
	frameTimer = 0.0;
	frameSpeed = 15;
	moving = false;
	casting = false;
	compiledFrame = 0;
	//targetID = 0;
	castDelay = 0;
	maxDuration = 0;
	bmx=0,bmy=0;
	basicAttack = false;
	basicAttackDelay = false;
	bATimer = 0.0;
	atkSpe = 2.4;

	// Used in-game
	angle = 90.0;
	keys = 0;
	collectedKeys = 0;
	coins = 0;
	health = 100;
	maxHealth = 100;
	healthDecay = 100;
	maxMana = 300;
	mana = maxMana;
	manaRegenRate = 3.3;
	manaTick = 0.0;

	// Attacks
	spellIndex = 0;

	leftclick = false;
	rightclick = false;
	shift = false;
	ctrl = false;

	// controls
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
    joy = NULL;
    joyStickID = 0;
}

Character::~Character() {
	// TODO Auto-generated destructor stub
	x = -6000;
	y = -6000;
	w = 0;
	h = 0;
	vX = 0.0;
	vY = 0.0;
	hp = 0;
	angle = 0.0;
	tag = "";
	isAlive = false;
	shift = false;
	ctrl = false;
	useKey = false;
	camlocked = false;
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	facing = 0;
	frame = 0;
	frameTimer = 0.0;
	frameSpeed = 1;
	moving = false;
	casting = false;
}

void Character::setParam(SDL_Renderer *gRenderer, std::string newTag, float newHP,
						 float newX, float newY,
						 float newW, float newH) {
	// TODO Auto-generated constructor stub
	tag = newTag;
	hp = newHP;
	x = newX;
	y = newY;
	w = newW;
	h = newH;
	isAlive = true;
	moving = false;
	casting = false;
	gArrowTexture.loadFromFile(gRenderer, "resource/gfx/arrow.png");
}

void Character::setJoyStick(int newJoyStickID) {
	joyStickID = newJoyStickID;
    joy = SDL_JoystickOpen(newJoyStickID);
	//Get controller haptic device
    haptic = SDL_HapticOpenFromJoystick( joy );
	if( haptic == NULL )
	{
		printf( "Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Get initialize rumble
		if( SDL_HapticRumbleInit( haptic ) < 0 )
		{
			printf( "Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError() );
		}
	}
}

void Character::setTexture(LTexture &newTexture, std::vector<SDL_Rect> newClips) {
	gTexture = newTexture;
	clips = newClips;
}
void Character::Free() {
	// Clean up
	SDL_HapticClose( haptic );
    SDL_JoystickClose(joy);
	gTexture.free();
	joy = NULL;
	haptic = NULL;
}

void Character::Update(Particle &part, Particle particles[], int mx, int my, Mix_Chunk *newSShoot) {

	// Determine what will trigger attack
	bool keySpellCast = false;
	if (controls==0){
		//keySpellCast = spaceBar;
	}else if(controls==1){
	//	keySpellCast = A;
	}

	// After trigger, turn off
	if (keySpellCast) {
		///keySpellCast = false;
	}

	/*
	// Y axis
	if (!casting) {
		if (moveUp) {
			// If Player holds control, they will not move
			if (!ctrl) {
				if (vY > -0.8) {
					vY -= 0.10;
				}
				moving = true;
			}
			// If player holds shift, they will stay in current direction
			if (!shift) {
				//angle = 270.0;
				facing = 1;
			}
		}
		else if (moveDown) {
			if (!ctrl) {
				if (vY < 0.8) {
					vY += 0.10;
				}
				moving = true;
			}
			if (!shift) {
				//angle = 90.0;
				facing = 0;
			}
		}

		// X axis
		if (moveLeft) {
			if (!ctrl) {
				if (vX > -0.8) {
					vX -= 0.10;
				}
				moving = true;
			}
			if (!shift) {
				//angle = 180.0;
				facing = 3;
			}
		}
		else if (moveRight) {
			if (!ctrl) {
				if (vX < 0.8) {
					vX += 0.10;
				}
				moving = true;
			}
			if (!shift) {
				//angle = 0.0;
				facing = 2;
			}
		}
		// Set direction to shoot where Player's mouse is, unless we are casting
		bmx = mx;
		bmy = my;
	}else{

	}

	// Get angle towards target
	angle = atan2(bmy - y-h/2, bmx - x-w/2);
	angle = angle * (180/M_PI);
	if (angle < 0) { angle = 360 - (-angle); }

	// Movement deceleration
	if (moveUp && moveLeft) {
		if (!shift) {
			//angle = 225.0;
		}
	}
	if (moveUp && moveRight) {
		if (!shift) {
			//angle = 315.0;
		}
	}
	if (moveDown && moveLeft) {
		if (!shift) {
			//angle = 135.0;
		}
	}
	if (moveDown && moveRight) {
		if (!shift) {
			//angle = 45.0;
		}
	}

	// Character movement
	x += vX;
	y += vY;

	// Is Player moving?
	if (!moveLeft && !moveRight) {
		vX = vX - vX * 0.16;
	}
	if (!moveUp && !moveDown) {
		vY = vY - vY * 0.16;
	}
	if (!moveUp && !moveDown && !moveLeft && !moveRight) {
		moving = false;
	}
	*/

	///////////////////////////////////////////////////////////////////////////////////////
	//-------------------------------------- Mana ---------------------------------------//



	// Doing an Ability Attack
	if (casting) {
		// Frame 8 done (Do spell cast delay)
		if (frame == 8) {
			frameTimer += 1;
			if (frameTimer > castDelay) {
				frameTimer = 0;
				// next image_index
				frame++;
			}
		}

		// Casting arms out, for 3 seconds
		else if (frame == 9) {

			// Do spell attack
			castSpell(spell, spellIndex, frameTimer, part, particles, newSShoot);

			// Next frame
			frameTimer += 1;

			// Frame 9 done (attack)
			if (frameTimer > maxDuration) {
				frameTimer = 0.0;
				// next image_index
				frame++;
				// if casting reaches last image_index, restart animation (or end it)
				if (frame > 9) {
					frame = 0;
					casting = false;
					// not matter what spell was casted, turn all of them off
					for (unsigned int i = 0; i < spell.size(); i++) {
						spell[i].activate = false;
					}

					// cast spell
					//castSpell();
				}
			}
		}
		// Get image_index to render
		compiledFrame = frame;
	}
	// Do a Basic Attack
	else if (basicAttack) {
		if (!basicAttackDelay) {
			// start delay
			basicAttackDelay = true;
			// stop attack
			basicAttack = false;
			// Spawn attack
			castSpell(spell, "Fireball", part, particles, newSShoot);
		}
	}
	// Moving
	else if (moving) {
		frameTimer += 15;
		if (frameTimer > 45) {
			frameTimer = 0;
			// next image_index
			frame++;
			if (frame > 1) {
				frame = 0;
			}
		}
	}

	if (!casting) {
		// Get image_index to render
		compiledFrame = frame+2*facing;
	}

	// Basic attack delay
	if (basicAttackDelay) {
		bATimer += atkSpe;
		if (bATimer > 60) {
			bATimer = 0.0;
			basicAttackDelay = false;
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
}

void Character::Render(SDL_Renderer *gRenderer, float camx, float camy) {
	gTexture.render(gRenderer, x-camx, y-camy, w, h, &clips[compiledFrame], 0.0);
}

void Character::RenderDebug(SDL_Renderer *gRenderer, float camx, float camy, TTF_Font *gFont) {
	//Render joystick 8 way angle
	gArrowTexture.render( gRenderer, 0, 0, 32, 32, NULL, LAngle);
	gArrowTexture.render( gRenderer, 0, 0, 32, 32, NULL, RAngle);

	for (int i=0; i<25; i++){
		std::stringstream tempss;
		tempss << "i: " << i << " -        " << SDL_JoystickGetButton(joy, i);
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
		gText.render(gRenderer, 5, 5+i*5, gText.getWidth()/4, gText.getHeight()/4);
	}

	std::stringstream tempss;
	tempss.str(std::string());
	tempss << "Axis: " << 0 << " -        " << SDL_JoystickGetAxis(joy, 0);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
	gText.render(gRenderer, 60, 0, gText.getWidth()/4, gText.getHeight()/4);

	tempss.str(std::string());
	tempss << "Axis: " << 1 << " -        " << SDL_JoystickGetAxis(joy, 1);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
	gText.render(gRenderer, 60, 10, gText.getWidth()/4, gText.getHeight()/4);

	tempss.str(std::string());
	tempss << "Axis: " << 2 << " -        " << SDL_JoystickGetAxis(joy, 2);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
	gText.render(gRenderer, 60, 20, gText.getWidth()/4, gText.getHeight()/4);

	tempss.str(std::string());
	tempss << "Axis: " << 3 << " -        " << SDL_JoystickGetAxis(joy, 3);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
	gText.render(gRenderer, 60, 30, gText.getWidth()/4, gText.getHeight()/4);

	tempss.str(std::string());
	tempss << "Axis: " << 4 << " -        " << SDL_JoystickGetAxis(joy, 4);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
	gText.render(gRenderer, 60, 40, gText.getWidth()/4, gText.getHeight()/4);

	tempss.str(std::string());
	tempss << "Axis: " << 5 << " -        " << SDL_JoystickGetAxis(joy, 5);
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont);
	gText.render(gRenderer, 60, 50, gText.getWidth()/4, gText.getHeight()/4);
}

void Character::stopMovement() {
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	vX = 0.0;
	vY = 0.0;
}

void Character::activateSpell() {
	// If we have enough mana
	if (mana >= spell[spellIndex].manaCost) {
		// if we're not already casting an ability
		if (!casting) {
			// cast ability
			casting = true;
			// subtract mana with mana cost
			mana -= spell[spellIndex].manaCost;
			// start on frame before attacking
			frame = 8;
			// Activate certain spell
			spell[spellIndex].activate = true;
			// reset increment angle
			spell[spellIndex].increAngle = 0;
			spell[spellIndex].increment = 1;
			// Get attack duration from spell
			maxDuration = spell[spellIndex].maxDuration;
			// Get cast delay of spell
			castDelay = spell[spellIndex].castDelay;
			stopMovement();
		}
	}
}

void Character::activateBasicAtack() {
	if (!basicAttackDelay) {
		if (!basicAttack) {
			basicAttack = true;
		}
	}
}

void Character::castSpell(std::vector<Spell> &spell, int spellIndex, int currentFrame, Particle &part, Particle particle[], Mix_Chunk *sSpellSFX) {
	//for (unsigned int i = 0; i < spell.size(); i++) {
		//if (spell[i].activate) {
			// Do spell duration count down
			//if (spell[i].currentDuration > 0) {
			//	spell[spellIndex].currentDuration -= spell[spellIndex].rate;

			// Do attacks at x frame(s)
			for (unsigned int j = 0; j < spell[spellIndex].frames.size(); j++) {
				if (currentFrame == spell[spellIndex].frames[j]) {
					for (double k = 0.0; k < spell[spellIndex].scope; k += spell[spellIndex].scope / spell[spellIndex].projectiles) {
						//if (type == 12) {
						int rands = 4;
						float spe = randDouble(spell[spellIndex].minSpe,
								spell[spellIndex].maxSpe);
						float size = randDouble(spell[spellIndex].minSize,
								spell[spellIndex].maxSize);
						float newX = (x + w / 2) - size / 2;
						float newY = (y + h / 2) - size / 2;

						switch (spell[spellIndex]._spawnType) {
						case Spell::OnCaster:
							newX = (x + w / 2) - size / 2;
							newY = (y + h / 2) - size / 2;
							break;
						case Spell::OnTarget:
							newX = *target.x - size / 2;
							newY = *target.y - size / 2;
							break;
						case Spell::OnMouse:
							newX = bmx - size / 2;
							newY = bmy - size / 2;
							break;
						case Spell::Uninitialized:
							//
							break;
						}

						// If we're not incrementing
						float finalAngle;		// Will help particles move around the caster, makes different effects
						float shootAngle;		// Which way all the particles will go regardless
						float radians;
						float distanceW = spell[spellIndex].increment * spell[spellIndex].distanceW;
						float distanceH = spell[spellIndex].increment * spell[spellIndex].distanceH;
						double barrelW = 0;
						double barrelH = 0;

						// Get first angle
						finalAngle = (angle) + k;

						// Do barrelW and barrelH before adjusting angle
						// if you want it to look like a spiral then do this after adjusting angle
						radians = ( M_PI / 180) * (finalAngle);
						barrelW = (distanceW * cos(radians)) - (distanceH * sin(radians));
						barrelH = (distanceW * sin(radians)) + (distanceH * cos(radians));
						finalAngle += spell[spellIndex].increment*15;
						finalAngle -= 45 / 2;


						// If we are incrementing
						if (spell[spellIndex].increAngleMax > 0) {
							//finalAngle = (angle) + h;
							finalAngle += spell[spellIndex].increAngle;
							finalAngle -= spell[spellIndex].increAngleMax/2;
							radians = ( M_PI/180) * (finalAngle);
							barrelW  = (distanceW * cos(radians) ) - (distanceH * sin(radians) );
							barrelH  = (distanceW * sin(radians) ) + (distanceH * cos(radians) );
						}

						// Determine which way spell will go towards
						switch (spell[spellIndex]._targetType) {
						case Spell::TowardsDefault:
							// finalAngle untouched
							shootAngle = finalAngle;
							break;
						case Spell::TowardsGamePadAxis:
							shootAngle = angle;
							break;
						case Spell::TowardsMouse:
							// finalAngle untouched
							shootAngle = finalAngle;
							break;
						case Spell::Uninitialized2:
							// finalAngle untouched
							shootAngle = finalAngle;
							break;
						}
						// Spawn Spell as a particle
						part.spawnParticleAngle(particle, tag,
								spell[spellIndex].type, newX + barrelW,
								newY + barrelH, size, size, shootAngle, spe,
								spell[spellIndex].damage, spell[spellIndex].color, 1,
								spell[spellIndex].dir, spell[spellIndex].dirSpe,
								spell[spellIndex].alpha, spell[spellIndex].alphaSpe,
								spell[spellIndex].deathTimer, spell[spellIndex].deathTimerSpe,
								spell[spellIndex].sizeDeath, spell[spellIndex].sizeDeathSpe,
								spell[spellIndex].decay, spell[spellIndex].decaySpe,
								spell[spellIndex].trail, spell[spellIndex].trailRate,
								spell[spellIndex].trailColor, spell[spellIndex].trailMinSize,
								spell[spellIndex].trailMaxSize,
								spell[spellIndex].timerBeforeMoving,
								spell[spellIndex].goTowardsTarget, bmx, bmy,
								spell[spellIndex].playSFXBeforeMoving, spell[spellIndex].bounces);
						//}
					}
					// vibrate controller
					if (controls == 1) {
						// Play effect at 50% strength for 2 seconds
						if( SDL_HapticRumblePlay( haptic, 1.0, 250 ) != 0 )
						{
							printf( "Warning: Unable to play rumble! %s\n", SDL_GetError() );
						}
					}
					// play audio
					Mix_PlayChannel(-1, sSpellSFX, 0);
					//Mix_FadeInChannelTimed(-1, sLazer, 1, 1, 1);
					/// Increment shoot angle after shots
					// Get number of times it will tick until duration is over
					float ticks = spell[spellIndex].maxDuration
							/ spell[spellIndex].frames.size();
					// Then get real ticks by dividing max duration with above ^^^
					ticks = spell[spellIndex].maxDuration / ticks;
					// With the number above, set the number og ticks to match increasing the increment angle
					float increValue = spell[spellIndex].increAngleMax / ticks;
					// Increment angle to shoot all around but incrementally going up
					if (spell[spellIndex].increAngle < spell[spellIndex].increAngleMax) {
						spell[spellIndex].increAngle += increValue;
					}
					// Increment if we have to
					if (spell[spellIndex].incrementing) {
						spell[spellIndex].increment++;
					}
				} 	// end check for specific duration
			}		// do for, attack frames
		//}			// spell activated
	//}
}

void Character::castSpell(std::vector<Spell> &spell, std::string displayName, Particle &part, Particle particle[], Mix_Chunk *sSpellSFX) {
	for (unsigned int i = 0; i < spell.size(); i++) {
		if (spell[i].displayName == displayName) {
			// Do spell duration count down
			//if (spell[i].currentDuration > 0) {
			//	spell[i].currentDuration -= spell[i].rate;

			// Do attacks at x frame(s)
			for (unsigned int j = 0; j < spell[i].frames.size(); j++) {
				int rands = 4;
				float spe = randDouble(spell[i].minSpe,
						spell[i].maxSpe);
				float size = randDouble(spell[i].minSize,
						spell[i].maxSize);
				float newX = (x + w / 2) - size / 2;
				float newY = (y + h / 2) - size / 2;

				switch (spell[i]._spawnType) {
				case Spell::OnCaster:
					newX = (x + w / 2) - size / 2;
					newY = (y + h / 2) - size / 2;
					break;
				case Spell::OnTarget:
					newX = *target.x - size / 2;
					newY = *target.y - size / 2;
					break;
				case Spell::OnMouse:
					newX = bmx - size / 2;
					newY = bmy - size / 2;
					break;
				case Spell::Uninitialized:
					//
					break;
				}

				// If we're not incrementing
				float finalAngle;
				// Get first angle
				finalAngle = (angle);

				// Do barrelW and barrelH before adjusting angle
				// if you want it to look like a spiral then do this after adjusting angle
				float targetX=500;
				float targetY=500;
				if (target.id == NULL) {
					targetX = bmx;
					targetY = bmy;
				}else{
					targetX = *target.x;
					targetY = *target.y;
				}

				// Determine which way spell will go towards
				float shootAngle;
				switch (spell[spellIndex]._targetType) {
				case Spell::TowardsDefault:
					// finalAngle untouched
					shootAngle = finalAngle;
					break;
				case Spell::TowardsGamePadAxis:
					shootAngle = angle;
					break;
				case Spell::TowardsMouse:
					// finalAngle untouched
					shootAngle = finalAngle;
					break;
				case Spell::Uninitialized2:
					// finalAngle untouched
					shootAngle = finalAngle;
					break;
				}

				// Spawn Spell as a particle
				part.spawnParticleAngle(particle, tag,
						spell[i].type, newX,
						newY, size, size, shootAngle, spe,
						spell[i].damage, spell[i].color, 1,
						spell[i].dir, spell[i].dirSpe,
						spell[i].alpha, spell[i].alphaSpe,
						spell[i].deathTimer, spell[i].deathTimerSpe,
						spell[i].sizeDeath, spell[i].sizeDeathSpe,
						spell[i].decay, spell[i].decaySpe,
						spell[i].trail, spell[i].trailRate,
						spell[i].trailColor, spell[i].trailMinSize,
						spell[i].trailMaxSize,
						spell[i].timerBeforeMoving,
						spell[i].goTowardsTarget, targetX, targetY,
						spell[i].playSFXBeforeMoving, spell[i].bounces);
				// vibrate controller
				if (controls == 1) {
					// Play effect at 50% strength for 2 seconds
					if( SDL_HapticRumblePlay( haptic, 1.0, 250 ) != 0 )
					{
						printf( "Warning: Unable to play rumble! %s\n", SDL_GetError() );
					}
				}
				// play audio
				Mix_PlayChannel(-1, sSpellSFX, 0);
				//Mix_FadeInChannelTimed(-1, sLazer, 1, 1, 1);
				/// Increment shoot angle after shots
				// Get number of times it will tick until duration is over
				float ticks = spell[i].maxDuration
						/ spell[i].frames.size();
				// Then get real ticks by dividing max duration with above ^^^
				ticks = spell[i].maxDuration / ticks;
				// With the number above, set the number og ticks to match increasing the increment angle
				float increValue = spell[i].increAngleMax / ticks;
				// Increment angle to shoot all around but incrementally going up
				if (spell[i].increAngle < spell[i].increAngleMax) {
					spell[i].increAngle += increValue;
				}
				// Increment if we have to
				if (spell[i].incrementing) {
					spell[i].increment++;
				}
				break;
			}		// do for, attack frames
		}			// spell activated
	}
}

void Character::setTarget(float *newTargetX, float *newTargetY, int newTargetID) {
	target.x = newTargetX;
	target.y = newTargetY;
	target.id = newTargetID;
}

void Character::addHealth(float amount) {
	health += amount;
	if (health > maxHealth) {
		health = maxHealth;
	}
}

void Character::addMana(float amount) {
	mana += amount;
	if (mana > maxMana) {
		mana = maxMana;
	}
}

void Character::minusHealth(float amount) {
	health -= amount;
	if (health <= 0) {
		health = 0;
	}
}

void Character::minusMana(float amount) {
	mana -= amount;
	if (mana <= 0) {
		mana = 0;
	}
}

int Character::getTargetID() {
	return target.id;
}

float Character::getX() {
	return x;
}

float Character::getY() {
	return y;
}

float Character::getCenterX() {
	return x+w/2;
}

float Character::getCenterY() {
	return y+h/2;
}

float Character::getCenterCursorX() {
	float radians = (3.1415926536/180)*(RAngle);
	float Cos = floor(cos(radians)*100+0.05)/100;
	return x+w/2 + Cos * 32;
}

float Character::getCenterCursorY() {
	float radians = (3.1415926536/180)*(RAngle);
	float Sin = floor(sin(radians)*100+0.05)/100;
	return y+h/2 + Sin * 32;
}

float Character::getCenterMouseX() {
	float radians = (3.1415926536/180)*(angle);
	float Cos = floor(cos(radians)*100+0.05)/100;
	return x+w/2 + Cos * 32;
}

float Character::getCenterMouseY() {
	float radians = (3.1415926536/180)*(angle);
	float Sin = floor(sin(radians)*100+0.05)/100;
	return y+h/2 + Sin * 32;
}

void Character::OnKeyDown( SDL_Keycode sym )
{
	controls = 0;
	switch (sym){
	case SDLK_w:					// Thrust space ship
		moveUp = true;
		break;
	case SDLK_s:					// Thrust space ship
		moveDown = true;
		break;
	case SDLK_a:					// turn left
		moveLeft = true;
		break;
	case SDLK_d:					// turn right
		moveRight = true;
		break;
	case SDLK_e:					// turn right
		useKey = true;
		break;
	case SDLK_1:					// cycle through spells
		spellIndex++;
		if (spellIndex > 2) {
			spellIndex = 0;
		}
		break;
	case SDLK_SPACE:
		spaceBar = true;
		activateSpell();
		break;
	case SDLK_y:
		camlocked = ( !camlocked );
		break;
	case SDLK_LSHIFT:
		shift = true;
		break;
	case SDLK_LCTRL:
		ctrl = true;
		moving = false;
		vX = 0;
		vY = 0;
		break;
	}
}

void Character::OnKeyUp( SDL_Keycode sym )
{
	controls = 0;
	switch (sym){
	case SDLK_w:					// Thrust space ship
		moveUp = false;
		break;
	case SDLK_s:					// Thrust space ship
		moveDown = false;
		break;
	case SDLK_a:					// turn left
		moveLeft = false;
		break;
	case SDLK_d:					// turn right
		moveRight = false;
		break;
	case SDLK_e:					// turn off useKey
		useKey = false;
		break;
	case SDLK_SPACE:
		spaceBar = false;
		break;
	case SDLK_LSHIFT:
		shift 		= false;
		break;
	case SDLK_LCTRL:
		ctrl = false;
		break;
	}
}

void Character::MouseClick(SDL_Event &e){
	if (e.type == SDL_MOUSEMOTION) {
		controls = 0;
	}
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			leftclick = true;
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			rightclick = true;
			activateBasicAtack();
		}
	}else if (e.type == SDL_MOUSEBUTTONUP) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			leftclick = false;
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			rightclick = false;
			basicAttack = false;
		}
	}
}

// Update XBOX 360 controls
void Character::updateJoystick(SDL_Event &e){
	////////////////// Xbox 360 Controls /////////////
	if (e.type == SDL_JOYAXISMOTION) {
		controls = 1;
	}
	if (e.type == SDL_CONTROLLERAXISMOTION) {
		//controls = 1;
	}
	/* Xbox 360 Controls */
	// Left Analog
	if ( ((SDL_JoystickGetAxis(joy, 0) < -8000) || (SDL_JoystickGetAxis(joy, 0) > 8000)) ||
		 ((SDL_JoystickGetAxis(joy, 1) < -8000) || (SDL_JoystickGetAxis(joy, 1) > 8000)) ){
		LAngle = atan2(SDL_JoystickGetAxis(joy, 1), SDL_JoystickGetAxis(joy, 0)) * ( 180.0 / M_PI );
		controls = 1;
	}
	// Right Analog
	if ( ((SDL_JoystickGetAxis(joy, 3) < -8000) || (SDL_JoystickGetAxis(joy, 3) > 8000)) ||
		 ((SDL_JoystickGetAxis(joy, 4) < -8000) || (SDL_JoystickGetAxis(joy, 4) > 8000)) ){
		RAngle = atan2(SDL_JoystickGetAxis(joy, 4), SDL_JoystickGetAxis(joy, 3)) * ( 180.0 / M_PI );
		controls = 1;
	}
	if (LAngle < 0) { LAngle = 360 - (-LAngle); }
	if (RAngle < 0) { RAngle = 360 - (-RAngle); }


	if (controls == 1) {

		//// Left Analog/////
		// Move left, x-axis
		if (SDL_JoystickGetAxis(joy, 0)/30 < -500){
			moveLeft = true;
		}
		// Move right, x-axis
		if (SDL_JoystickGetAxis(joy, 0)/30 > 500){
			moveRight = true;
		}
		// joy range between -500 and 500, no moving
		if (SDL_JoystickGetAxis(joy, 0)/30 >= -500 && SDL_JoystickGetAxis(joy, 0)/30 <= 500){
			moveLeft = false;
			moveRight = false;
		}
		// Move up, y-axis
		if (SDL_JoystickGetAxis(joy, 1)/30 < -500){
			moveUp = true;
		}
		// Move down, y-axis
		if (SDL_JoystickGetAxis(joy, 1)/30 > 500){
			moveDown = true;
		}
		// joy range between -500 and 500, no moving
		if (SDL_JoystickGetAxis(joy, 1)/30 >= -500 && SDL_JoystickGetAxis(joy, 1)/30 <= 500){
			moveUp = false;
			moveDown = false;
		}

		//// Right Analog/////
		// Face left, x-axis
		if (SDL_JoystickGetAxis(joy, 3)/30 < -500){
		//	moveLeft = true;
		}
		// Face right, x-axis
		if (SDL_JoystickGetAxis(joy, 3)/30 > 500){
		//	moveRight = true;
		}
		// Face up, y-axis
		if (SDL_JoystickGetAxis(joy, 4)/30 < -500){
		//	moveUp = true;
		}
		// Face down, y-axis
		if (SDL_JoystickGetAxis(joy, 4)/30 > 500){
		//	moveDown = true;
		}

		//// Triggers Analog/////
		// Left Trigger
		if (SDL_JoystickGetAxis(joy, 2) > -LTRIGGER_DEAD_ZONE){
			activateSpell();
		}
		// Right Trigger
		if (SDL_JoystickGetAxis(joy, 5) > -RTRIGGER_DEAD_ZONE){
			activateBasicAtack();
		}

		//// DPAD Triggers ////
		if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_UP) {
			moveUp = true;
		}
		if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_DOWN) {
			moveDown = true;
		}
		if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_LEFT) {
			moveLeft = true;
		}
		if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_RIGHT) {
			moveRight = true;
		}

		if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_LEFTUP) {
	//
		}
		else if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_RIGHTUP) {
	//
		}
		else if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_LEFTDOWN) {
	//
		}
		else if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_RIGHTDOWN) {
	//
		}

		// Xbox 360 Controls
		if (e.type == SDL_JOYBUTTONDOWN && e.jbutton.state == SDL_PRESSED && e.jbutton.which == 0){
			controls = 1;
			switch(e.jbutton.button){
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				D_UP = true;
				moveUp = true;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				D_DOWN = true;
				moveDown = true;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				D_LEFT = true;
				moveLeft = true;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				D_RIGHT = true;
				moveRight = true;
				break;
			case 4:
				START = true;
				break;
			case 5:
				BACK = true;
				break;
			case 6:
				L3 = true;
				break;
			case 7:
				R3 = true;
				break;
			case 8:
				LB = true;
				break;
			case 9:
				RB = true;
				break;
			case SDL_CONTROLLER_BUTTON_A:
				A = true;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				X = true;
				useKey = true;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				Y = true;
				spellIndex++;
				if (spellIndex > 2) {
					spellIndex = 0;
				}
				break;
		//	case 11:
			//	B = true;
			//	break;
		//	case 12:
		//		X = true;
		//		break;
			}
		}else if (e.type == SDL_JOYBUTTONUP && e.jbutton.state == SDL_RELEASED && e.jbutton.which == 0){
			switch(e.jbutton.button){
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				D_UP = false;
				moveUp = false;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				D_DOWN = false;
				moveDown = false;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				D_LEFT = false;
				moveLeft = false;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				D_RIGHT = false;
				moveRight = false;
				break;
			case 4:
				START = false;
				break;
			case 5:
				BACK = false;
				break;
			case 6:
				L3 = false;
				break;
			case 7:
				R3 = false;
				break;
			case 8:
				LB = false;
				break;
			case 9:
				RB = false;
				break;
			case SDL_CONTROLLER_BUTTON_A:
				A = false;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				X = false;
				useKey = false;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				Y = true;
				break;
			/*case 11:
				B = false;
				break;
			case 12:
				X = false;
				break;
			case 13:
				Y = false;
				break;*/
			}
		}
	}
}



