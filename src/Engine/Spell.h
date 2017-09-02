/*
 * Spell.h
 *
 *  Created on: Aug 17, 2017
 *      Author: Carl
 */

#ifndef ENGINE_SPELL_H_
#define ENGINE_SPELL_H_

#include <vector>

class Spell {

public:	// Variables

	std::string displayName;	// name of spell for GUI
	int type;					// Particle type that is going to spawn
	float minSize;
	float maxSize;
	float minSpe;
	float maxSpe;
	float damage;
	SDL_Color color;
	float dir;
	float dirSpe;
	float alpha;
	float alphaSpe;
	float deathTimer;
	float deathTimerSpe;
	bool sizeDeath;
	float sizeDeathSpe;
	bool decay;
	float decaySpe;
	bool trail;
	float trailRate;
	SDL_Color trailColor;
	float trailMinSize;
	float trailMaxSize;
	int projectiles;			// Number of projectiles being shot per frame
	float scope;				// The width of the attack (e.g. "360" would be an all around attack)
	// Number of times spell will occur during duration
	// (Get maxDuration and divide it by occurrences to get number of times it will occur during the duration)
	float occurances;
	float manaCost;
	//float currentDuration;
	//float baseCooldown;
	//float cooldownTimer;
	bool activate;
	//bool cooldown;
	float increAngle;			// Increases as spell duration goes on
	float increAngleMax;		// Increases as spell duration goes on
	// (Get maxDuration and divide it by occurrences to get number of times it will occur during the duration)
	float currentDuration;		// Current frame
	float maxDuration;			// Frame length
	std::vector<int> frames;	// During x frames, spell will spawn out
	float timerBeforeMoving;	// Timer before moving spell
	bool goTowardsTarget;		// After timerBeforeMoving is finished, spell will go towards target's last location
	float rate;					// spell rate (i.e. 2.5 equals 2.5 spell casts a second)
	bool spawnsAwayFromCaster;	// If true, the spell will spawn away from the caster by increments (like in the game Gungeon)
	float distanceW;			// Each occurrence, the spell(s) will move X distance(s) away from caster
	float distanceH;
	float castDelay;			// Delay before attack

	// These are specifically used only by Character.cpp class
	float cooldownTimer;
	float baseCooldown;
	bool cooldown;
	bool incrementing;			// If we are going to increment 'increment' value, after each attack
	int increment;				// each time a spell is casted out, this will keep track of which index we are on
	bool playSFXBeforeMoving;	// After waiting to move, should particle play a sound (i.e.: make a swoosh sound)
	int bounces;				// how many times a particle can bounce of objects before dying

	enum SpawnType { OnCaster, OnTarget, OnMouse, Uninitialized };
	SpawnType _spawnType = Uninitialized;

	enum TargetingType {TowardsDefault, TowardsGamePadAxis, TowardsMouse, Uninitialized2};
	TargetingType _targetType = Uninitialized2;

	/*void Create(std::string newDisplayName, int newType,
			int newProjectiles, float newScope,
			float newOccurances,
			float newMinSize, float newMaxSize,
			float newMinSpe, float newMaxSpe,
			float newDamage, SDL_Color newColor,
			float newDir, float newDirSpeed,
			float newAlpha, float newAlphaSpeed,
			float newDeathTimer, float newDeathTimerSpeed,
			bool newSizeDeath, float newSizeDeathSpeed,
			bool newDecay, float newDecaySpeed,
			bool newTrail, float newTrailRate, SDL_Color newTrailColor,
			float newTrailMinSize, float newTrailMaxSize,
			float newManaCost,
			float newincreAngle, float newincreAngleMax,
			float newRate, float newMaxDuration,
			float newTimerBeforeMoving, bool newGoTowardsTarget,
			bool newSpawnsAwayFromCaster, float newDistanceW, float newDistanceH) {
		displayName = newDisplayName;
		type = newType;
		projectiles = newProjectiles;
		scope = newScope;
		occurances = newOccurances;
		minSize = newMinSize;
		maxSize = newMaxSize;
		minSpe = newMinSpe;
		maxSpe = newMaxSpe;
		damage = newDamage;
		color = newColor;
		dir = newDir;
		dirSpe = newDirSpeed;
		alpha = newAlpha;
		alphaSpe = newAlphaSpeed;
		deathTimer = newDeathTimer;
		deathTimerSpe = newDeathTimerSpeed;
		sizeDeath = newSizeDeath;
		sizeDeathSpe = newSizeDeathSpeed;
		decay = newDecay;
		decaySpe = newDecaySpeed;
		trail = newTrail;
		trailRate = newTrailRate;
		trailColor = newTrailColor;
		trailMinSize = newTrailMinSize;
		trailMaxSize = newTrailMaxSize;
		manaCost = newManaCost;
		//baseCooldown = newBaseCooldown;
		//cooldownTimer = newBaseCooldown;
		activate = false;
		//cooldown = false;
		increAngle = newincreAngle;
		increAngleMax = newincreAngleMax;
		rate = newRate;
		maxDuration = newMaxDuration;
		currentDuration = newMaxDuration;
		timerBeforeMoving = newTimerBeforeMoving;
		goTowardsTarget = newGoTowardsTarget;
		spawnsAwayFromCaster = newSpawnsAwayFromCaster;
		distanceW = newDistanceW;
		distanceH = newDistanceH;

		// These are specifically used only by Character.cpp class
		cooldownTimer = 0.0;
		baseCooldown = 0.0;
		cooldown = false;
	}*/

	/*Spell(std::string newDisplayName, int newType,
			int newProjectiles, float newScope,
			float newOccurances,
			float newMinSize, float newMaxSize,
			float newMinSpe, float newMaxSpe,
			float newDamage, SDL_Color newColor,
			float newDir, float newDirSpeed,
			float newAlpha, float newAlphaSpeed,
			float newDeathTimer, float newDeathTimerSpeed,
			bool newSizeDeath, float newSizeDeathSpeed,
			bool newDecay, float newDecaySpeed,
			bool newTrail, float newTrailRate, SDL_Color newTrailColor,
			float newTrailMinSize, float newTrailMaxSize,
			float newManaCost,
			float newincreAngle, float newincreAngleMax,
			float newRate, float newMaxDuration,
			float newTimerBeforeMoving, bool newGoTowardsTarget,
			bool newSpawnsAwayFromCaster, float newDistanceW, float newDistanceH) {
		displayName = newDisplayName;
		type = newType;
		projectiles = newProjectiles;
		scope = newScope;
		occurances = newOccurances;
		minSize = newMinSize;
		maxSize = newMaxSize;
		minSpe = newMinSpe;
		maxSpe = newMaxSpe;
		damage = newDamage;
		color = newColor;
		dir = newDir;
		dirSpe = newDirSpeed;
		alpha = newAlpha;
		alphaSpe = newAlphaSpeed;
		deathTimer = newDeathTimer;
		deathTimerSpe = newDeathTimerSpeed;
		sizeDeath = newSizeDeath;
		sizeDeathSpe = newSizeDeathSpeed;
		decay = newDecay;
		decaySpe = newDecaySpeed;
		trail = newTrail;
		trailRate = newTrailRate;
		trailColor = newTrailColor;
		trailMinSize = newTrailMinSize;
		trailMaxSize = newTrailMaxSize;
		manaCost = newManaCost;
		//baseCooldown = newBaseCooldown;
		//cooldownTimer = newBaseCooldown;
		activate = false;
		//cooldown = false;
		increAngle = newincreAngle;
		increAngleMax = newincreAngleMax;
		rate = newRate;
		maxDuration = newMaxDuration;
		currentDuration = newMaxDuration;
		timerBeforeMoving = newTimerBeforeMoving;
		goTowardsTarget = newGoTowardsTarget;
		spawnsAwayFromCaster = newSpawnsAwayFromCaster;
		distanceW = newDistanceW;
		distanceH = newDistanceH;

		// These are specifically used only by Character.cpp class
		cooldownTimer = 0.0;
		baseCooldown = 0.0;
		cooldown = false;
	}*/

	/*	Name: 			Skelly Blast
	 * 	Type:			Ability, Instant damage
	 * 	Area:			Linear
	 *	Cooldown:		??? second 	(60 frames @ 60 fps)
	 *	Duration:		??? seconds 	(120 frames @ 60 fps)
	 *	Description:	???
	 */
	void SkellyBlast() {
		displayName = "Skelly Blast", type = 3;
		projectiles = 1, scope = 1.0;
		occurances = 1;
		minSize = 4, maxSize = 4;
		minSpe = 1.2, maxSpe = 1.2;
		damage = 10, color = {200,20,180};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 0;
		deathTimer = 60, deathTimerSpe = 0.5;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {200,20,180};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 45;
		activate = false;
		increAngle = 0, increAngleMax = 0.0;
		rate = 1, maxDuration = 1; currentDuration = 1;
		timerBeforeMoving = 0.0, goTowardsTarget = false;
		spawnsAwayFromCaster = false, distanceW = 0, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsDefault;
		increment = 0;
	}

	/*	Name: 			Goble Blast
	 * 	Type:			Ability, Instant damage
	 * 	Area:			Linear
	 *	Cooldown:		??? second 	(60 frames @ 60 fps)
	 *	Duration:		??? seconds 	(120 frames @ 60 fps)
	 *	Description:	???
	 */
	void GobleBlast() {
		displayName = "Goble Blast", type = 3;
		projectiles = 3, scope = 45.0;
		occurances = 1;
		minSize = 4, maxSize = 4;
		minSpe = 1.1, maxSpe = 1.1;
		damage = 5, color = {20,200,30};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 0;
		deathTimer = 60, deathTimerSpe = 0.5;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {20,200,30};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 20;
		activate = false;
		increAngle = 0, increAngleMax = 0.0;
		rate = 1, maxDuration = 1; currentDuration = 1;
		timerBeforeMoving = 0.0, goTowardsTarget = false;
		spawnsAwayFromCaster = false, distanceW = 0, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsDefault;
		increment = 0;
	}

	/*	Name: 			Fire Blast
	 * 	Type:			Ability, Instant damage
	 * 	Area:			AOE
	 *	Cooldown:		1 second 	(60 frames @ 60 fps)
	 *	Duration:		1 second 	(120 frames @ 60 fps)
	 *	Description:
	 */
	void IceBlast() {
		displayName = "Ice Blast", type = 3;
		projectiles = 25, scope = 45.0;
		occurances = 10;
		minSize = 3, maxSize = 6;
		minSpe = 0.8, maxSpe = 1.7;
		damage = 10, color = {20,30,200};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 1;
		deathTimer = 60, deathTimerSpe = 0.1;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {20,30,200};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 45;
		activate = false;
		increAngle = 0, increAngleMax = 0.0;
		rate = 1, maxDuration = 60*3; currentDuration = 60*3;
		timerBeforeMoving = 0.0, goTowardsTarget = false;
		spawnsAwayFromCaster = false, distanceW = 0, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsDefault;
		increment = 0;
	}

	/*	Name: 			Lightning
	 * 	Type:			Instant damage
	 * 	Area:			???
	 *	Cooldown:		??? second 	(60 frames @ 60 fps)
	 *	Duration:		??? second 	(120 frames @ 60 fps)
	 *	Description:	???
	 */
	void Lightning() {
		displayName = "Lightning", type = 3;
		projectiles = 1, scope = 1.0;
		occurances = 14;
		minSize = 3, maxSize = 6;
		minSpe = 0, maxSpe = 0;
		damage = 10, color = {200,200,10};
		dir = 1, dirSpe = 0;
		alpha = 255, alphaSpe = 0.1;
		deathTimer = 60, deathTimerSpe = 0.3;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {200,200,10};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 45;
		activate = false;
		increAngle = 0, increAngleMax = 0.0;
		rate = 1, maxDuration = 60; currentDuration = 60;
		timerBeforeMoving = 0.0, goTowardsTarget = false;
		spawnsAwayFromCaster = false, distanceW = 6, distanceH = 0;
		incrementing = true;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsDefault;
		increment = 0;
	}

	/*	Name: 			Fire Dance
	 * 	Type:			Ability, Instant damage
	 * 	Area:			AOE
	 *	Cooldown:		1 second 	(60 frames @ 60 fps)
	 *	Duration:		2 seconds 	(120 frames @ 60 fps)
	 *	Description:	Shoot out 3 fire-balls once
	 */
	void FirePulse() {
		displayName = "Fire Pulse", type = 3;
		projectiles = 3, scope = 360.0;
		occurances = 10;
		minSize = 4, maxSize = 4;
		minSpe = 1.2, maxSpe = 1.2;
		damage = 10, color = {200,30,20};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 0;
		deathTimer = 60, deathTimerSpe = 0.5;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {255,255,255};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 45;
		activate = false;
		increAngle = 0, increAngleMax = 360.0;
		rate = 1, maxDuration = 1; currentDuration = 1;
		timerBeforeMoving = 0.0, goTowardsTarget = false;
		spawnsAwayFromCaster = false, distanceW = 0, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsDefault;
		increment = 0;
	}

	/*	Name: 			Fire Dance
	 * 	Type:			Ability, Instant damage
	 * 	Area:			AOE
	 *	Cooldown:		1 second 	(60 frames @ 60 fps)
	 *	Duration:		2 seconds 	(120 frames @ 60 fps)
	 *	Description:	Shoot out 3 fire-balls every 0.12 seconds 10 times
	 */
	void FireDance() {
		displayName = "Fire Dance", type = 3;
		projectiles = 3, scope = 360.0;
		occurances = 10;
		minSize = 4, maxSize = 4;
		minSpe = 1.2, maxSpe = 1.2;
		damage = 10, color = {200,30,20};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 0;
		deathTimer = 60, deathTimerSpe = 0.5;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {255,255,255};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 45;
		activate = false;
		increAngle = 0, increAngleMax = 360.0;
		rate = 1, maxDuration = 60*2; currentDuration = 60*2;
		timerBeforeMoving = 0.0, goTowardsTarget = false;
		spawnsAwayFromCaster = false, distanceW = 0, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsDefault;
		increment = 0;
	}

	/*	Name: 			Fire Blast
	 * 	Type:			Ability, Instant damage
	 * 	Area:			AOE
	 *	Cooldown:		1 second 	(60 frames @ 60 fps)
	 *	Duration:		1 second 	(120 frames @ 60 fps)
	 *	Description:
	 */
	void FireBlast() {
		displayName = "Fire Blast", type = 3;
		projectiles = 5, scope = 360.0;
		occurances = 5;
		minSize = 4, maxSize = 4;
		minSpe = 1.2, maxSpe = 1.2;
		damage = 10, color = {200,30,20};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 0;
		deathTimer = 60, deathTimerSpe = 0.5;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {255,255,255};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 80;
		activate = false;
		increAngle = 0, increAngleMax = 45.0;
		rate = 1, maxDuration = 60*2; currentDuration = 60*2;
		timerBeforeMoving = 60*2+30, goTowardsTarget = false;
		spawnsAwayFromCaster = true, distanceW = 15, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsDefault;
		increment = 0;
	}


	///////////////////////////////////////////////////////////////////////////////////////
	//---------------------- These Spells only work with Character.cpp ------------------//

	/*	Name: 			Fire Ball
	 * 	Type:			Basic Attack, Instant damage
	 * 	Area:			AOE
	 *	Cooldown:		N/A (dependent on attack speed)
	 *	Duration:		N/A
	 *	Description:	Shoots out a fire ball at a direction
	 */
	void Fireball() {
		displayName = "Fireball", type = 3;
		projectiles = 1, scope = 1.0;
		occurances = 5;
		minSize = 4, maxSize = 4;
		minSpe = 1.1, maxSpe = 1.1;
		damage = 25, color = {200,30,20};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 0;
		deathTimer = 60, deathTimerSpe = 0.5;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {200,30,20};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 0;
		activate = false;
		increAngle = 0, increAngleMax = 90.0,  rate = 1,
		maxDuration = 1, currentDuration = 1,
		timerBeforeMoving = 0, goTowardsTarget = false;
		spawnsAwayFromCaster = true, distanceW = 0, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 0; baseCooldown = 0, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsGamePadAxis;
		incrementing = false, increment = 0, castDelay = 0;
		playSFXBeforeMoving = false;
		for (int i = 0; i<=maxDuration; i+= 20){
			frames.push_back(i);
		}
		bounces = 2;
	}

	/*	Name: 			StarScorch
	 * 	Type:			Ability, Instant damage
	 * 	Area:			AOE
	 *	Cooldown:		1 second 	(60 frames @ 60 fps)
	 *	Duration:		1 second 	(120 frames @ 60 fps)
	 *	Description:
	 */
	void StarScorch() {
		displayName = "Star Scorch", type = 3;
		projectiles = 5, scope = 360.0;
		occurances = 5;
		minSize = 4, maxSize = 4;
		minSpe = 2.2, maxSpe = 3.2;
		damage = 18, color = {200,30,20};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 0;
		deathTimer = 60, deathTimerSpe = 0.5;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {200,30,20};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 30;
		activate = false;
		increAngle = 0, increAngleMax = 0.0,  rate = 1,
		maxDuration = 30, currentDuration = 30,
		frames.push_back(30), frames.push_back(20), frames.push_back(10),
		timerBeforeMoving = 30+15, goTowardsTarget = false;
		spawnsAwayFromCaster = true, distanceW = 10, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsDefault;
		incrementing = true, increment = 0, castDelay = 10;
		playSFXBeforeMoving = false;
		bounces = 0;
	}

	/*	Name: 			Fire Blast
	 * 	Type:			Ability, Instant damage
	 * 	Area:			AOE
	 *	Cooldown:		1 second 	(60 frames @ 60 fps)
	 *	Duration:		1 second 	(120 frames @ 60 fps)
	 *	Description:
	 */
	void Blaze() {
		displayName = "Blaze", type = 3;
		projectiles = 5, scope = 360.0;
		occurances = 21;
		minSize = 4, maxSize = 4;
		minSpe = 1.1, maxSpe = 1.1;
		damage = 13, color = {200,30,20};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 0;
		deathTimer = 60, deathTimerSpe = 0.5;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {200,30,20};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 60;
		activate = false;
		increAngle = 0, increAngleMax = 45.0,  rate = 1,
		maxDuration = 60, currentDuration = 60,
		timerBeforeMoving = 0, goTowardsTarget = false;
		spawnsAwayFromCaster = true, distanceW = 0, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnMouse;
		_targetType = Spell::TowardsDefault;
		incrementing = false, increment = 0, castDelay = 1;
		playSFXBeforeMoving = false;
		for (int i = 0; i<=maxDuration; i+= 5){
			frames.push_back(i);
		}
		bounces = 0;
	}

	/*	Name: 			Fire Blast
	 * 	Type:			Ability, Instant damage
	 * 	Area:			AOE
	 *	Cooldown:		1 second 	(60 frames @ 60 fps)
	 *	Duration:		1 second 	(120 frames @ 60 fps)
	 *	Description:
	 */
	void Inferno() {
		displayName = "Inferno", type = 3;
		projectiles = 5, scope = 360.0;
		occurances = 5;
		minSize = 4, maxSize = 4;
		minSpe = 2.1, maxSpe = 2.1;
		damage = 10, color = {200,30,20};
		dir = 0, dirSpe = 0;
		alpha = 255, alphaSpe = 0;
		deathTimer = 60, deathTimerSpe = 0.5;
		sizeDeath = false, sizeDeathSpe = 0.0;
		decay = false, decaySpe = 0.0;
		trail = false, trailRate = 0.0, trailColor = {200,30,20};
		trailMinSize = 1, trailMaxSize = 6;
		manaCost = 80;
		activate = false;
		increAngle = 0, increAngleMax = 0.0,  rate = 1,
		maxDuration = 60, currentDuration = 60,
		//frames.push_back(40), frames.push_back(30), frames.push_back(20),
		timerBeforeMoving = maxDuration+20, goTowardsTarget = false;
		spawnsAwayFromCaster = true, distanceW = 10, distanceH = 0;
		// These are specifically used only by Character.cpp class
		cooldownTimer = 60; baseCooldown = 60, cooldown = false;
		_spawnType = Spell::OnCaster;
		_targetType = Spell::TowardsGamePadAxis;
		incrementing = true, increment = 0, castDelay = 1;
		playSFXBeforeMoving = true;

		for (int i = 0; i<=maxDuration; i+=2){
			frames.push_back(i*10);
		}
		bounces = 0;
	}

	// Upon spell creation, choose an Ability
	Spell(std::string spellName) {
		if (spellName == "Skelly Blast") {
			SkellyBlast();
		}else if (spellName == "Goble Blast") {
			GobleBlast();
		}else if (spellName == "Ice Blast") {
			IceBlast();
		}else if (spellName == "Lightning") {
			Lightning();
		}else if (spellName == "Fire Dance") {
			FireDance();
		}else if (spellName == "Fire Pulse") {
			FirePulse();
		}else if (spellName == "Fire Blast") {
			FireBlast();
		}else if (spellName == "Star Scorch") {
			StarScorch();
		}else if (spellName == "Blaze") {
			Blaze();
		}else if (spellName == "Inferno") {
			Inferno();
		}else if (spellName == "Fireball") {
			Fireball();
		}
	}

	/*Spell(std::string newDisplayName, int newType,
			int newProjectiles, float newScope,
			float newOccurances, float newMaxDuration,
			float newSize, float newMinSpe, float newMaxSpe,
			float newDamage, SDL_Color newColor,
			float newDir, float newDirSpeed,
			float newAlpha, float newAlphaSpeed,
			float newDeathTimer, float newDeathTimerSpeed,
			bool newSizeDeath, float newSizeDeathSpeed,
			bool newDecay, float newDecaySpeed,
			bool newTrail, float newTrailRate, SDL_Color newTrailColor,
			float newTrailMinSize, float newTrailMaxSize,
			float newManaCost, float newBaseCooldown) {
		displayName = newDisplayName;
		type = newType;
		projectiles = newProjectiles;
		scope = newScope;
		maxDuration = newMaxDuration;
		occurances = newOccurances;
		currentDuration = newMaxDuration;
		size = newSize;
		minSpe = newMinSpe;
		maxSpe = newMaxSpe;
		damage = newDamage;
		color = newColor;
		dir = newDir;
		dirSpe = newDirSpeed;
		alpha = newAlpha;
		alphaSpe = newAlphaSpeed;
		deathTimer = newDeathTimer;
		deathTimerSpe = newDeathTimerSpeed;
		sizeDeath = newSizeDeath;
		sizeDeathSpe = newSizeDeathSpeed;
		decay = newDecay;
		decaySpe = newDecaySpeed;
		trail = newTrail;
		trailRate = newTrailRate;
		trailColor = newTrailColor;
		trailMinSize = newTrailMinSize;
		trailMaxSize = newTrailMaxSize;
		manaCost = newManaCost;
		baseCooldown = newBaseCooldown;
		cooldownTimer = newBaseCooldown;
		activate = false;
		cooldown = false;
		randIndex = 0;
	}*/


};

#endif /* ENGINE_SPELL_H_ */
