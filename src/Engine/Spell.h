/*
 * Spell.h
 *
 *  Created on: Aug 17, 2017
 *      Author: Carl
 */

#ifndef ENGINE_SPELL_H_
#define ENGINE_SPELL_H_

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
	enum SpellTarget {casterCenter, mouseCenter, targetCenter};
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
	int randIndex;				// before activation, choose 1 spell to attack with from list of spells
	float increAngle;			// Increases as spell duration goes on
	float increAngleMax;		// Increases as spell duration goes on
	// (Get maxDuration and divide it by occurrences to get number of times it will occur during the duration)
	float currentDuration;
	float maxDuration;			// Spell frame length
	float timerBeforeMoving;	// Timer before moving spell
	bool goTowardsTarget;		// After timerBeforeMoving is finished, spell will go towards target's last location
	float rate;					// spell rate (i.e. 2.5 equals 2.5 spell casts a second)
	bool spawnsAwayFromCaster;	// If true, the spell will spawn away from the caster by increments (like in the game Gungeon)
	float distanceW;			// Each occurrence, the spell(s) will move X distance(s) away from caster
	float distanceH;
	Spell(std::string newDisplayName, int newType,
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
		randIndex = 0;
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
