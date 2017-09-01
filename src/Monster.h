/*
 * Monster.h
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#ifndef GAME_ZOMBIE_H_
#define GAME_ZOMBIE_H_

#include "Engine/Helper.h"
#include "Engine/Spell.h"
#include "Engine/Particle.h"
#include <iostream>
#include <vector>

class Monster : public Helper {
public:	// Resources
	LTexture gMonster;
	/*
	 * 12 monsters in Total
	 * 108 clips
	 * 9 x 12 ( 9 clips going to the right, 12  going down)
	 */
	SDL_Rect clip[108];

	/* Dragon clips
	 * 0: facing right neutral
	 * 1: facing right head-bounce
	 * 2: facing right attack
	 * 3: facing left neutral
	 * 4: facing left head-bounce
	 * 5: facing left attack
	 */
	SDL_Rect dragonClips[6];

public:	// Editor Variables
	int count;
	const int max = 100;
	const int maxTypes = 13;	// max number of unique Monsters
	int multiW = 1;
	int multiH = 1;

public:
	float x2, y2;
	int radius;
	float x,y;
	float w,h;			// Enemy collision size
	float angle;
	float vX,vY;
	float velX,velY;	// Apply to monster for knockback effect
	float speed;
	/* Type of monster we are spawning
	 * 0: Skeleton
	 * 1: Goblin
	 * 2: Ghost Mage
	 * 3: Dino Bear?
	 * 4: Bear looking creature
	 * 5: Lizard
	 * 6: Yellow goblin
	 * 7: Ghoul
	 * 8: Ghost/Reaper
	 * 9: 1-eyed monster
	 * 10: Snake
	 * 11: Pumpkin Head
	 *
	 * 12: Dragon Boss
	 */
	int type;
	/* Type of monster we are spawning
	 * 0: Skeleton: 						shoots 1 magic blast (10 damage)
	 * 1: Goblin: 							shoots 3 magic blasts (5 damage each)
	 * 2: Ghost Mage: 						shoots 9 magic blasts (2.5 damage each)
	 * 3: Dinosaur Bear: 					slashes 3 times (7 damage each strike)
	 * 4: Tiger: 							slashes 1 time (25 damage)
	 * 5: Lizard
	 * 6: Yellow goblin
	 * 7: Ghoul
	 * 8: Ghost/Reaper
	 * 9: 1-eyed monster
	 * 10: Snake
	 * 11: Pumpkin Head
	 *
	 * 12: Dragon Boss
	 */
	double damage;
	double atkSpe;			// Monster attack speed
	int id;					// Monster clip from texture
	double health;
	double healthDecay;
	double maxHealth;
	double timer;			// Used for shooting
	float distance;
	float targetDistanceX;
	float targetDistanceY;
	std::string tag;
	bool alive;
	bool collision;
	bool onScreen;
	bool mouse;				// mouse is on top of Monster
	bool mouseBox;			// if tile placement size of on top of monster
	bool hasVision;			// has vision of target
	bool isSelected;		// being selected by a target

public:	// Spells

	std::vector<Spell> spell;
	unsigned int rJ;
	// Number of times spell will occur during duration
	float cooldownTimer;
	float baseCooldown;
	bool cooldown;

public:	// Patrol Variables
	float thinkTimer;
	float sightRange;	// Sight range, Monster will move towards Target at this range (usually higher than range)
	float atkRange;		// Attack range, Monster will attack at this range
	float duration;		// Attack duration
	bool attack;		// Monster is attacking
	bool follow;		// Target in range, for moving
	bool alert;			// Target in range, for Attacking

public:	// Walking variables
	/*
	 * 0: walk down
	 * 1: walk up
	 * 2: walk right
	 * 3: walk left
	 */
	int facing;
	double frameTimer;
	double frameSpeed;
	int frame;

public:	// variables used for calculating shooting barrel coordinates

	/*
	 * Clone: 		11
	 * Big Boss: 	119
	 */
	int distanceHeadIsFromCenterOfImage;

	/* how many pixels away the Enemy's barrel is from the center of it's image
	 *	Player's Image: 92, 21
	 *	Clone: 57, 17
	 *	Big Boss: 256, -49
	 */
	double bulletW, bulletH;

	/* Location of Enemy's Barrel for shooting */
	double barrelX, barrelY;

public:	// Core functions

	void Init(Monster monster[]);

	void Load(SDL_Renderer* gRenderer);

	void Free();

	void Spawn(Monster monster[], float x, float y,
			   float w, float h,
			   float angle, float speed,
			   double id, double type, double health,
			   int distanceHeadIsFromCenterOfImage, int bulletW, int bulletH);

	void SpawnMultiple(Monster monster[], int newMx, int newMy, int camx, int camy);

	void Remove(Monster monster[], int click);

	void RemoveAll(Monster monster[]);

	void EditorUpdate(Monster monster[], int newMx, int newMy, int mex, int mey, int camx, int camy);

	// Update monsters 0-11
	void Update(Monster monster[], Particle &part, Particle particle[], Mix_Chunk* sLazer, int camx, int camy,
			    float targetX, float targetY);

	// Update minions (index 0-11)
	void UpdatMinions(Monster monster[], Particle &part, Particle particle[], float x, float y);

	// Update Boss (index 12)
	void UpdateBoss(Monster monster[], Particle &part, Particle particle[], float x, float y);

	// Render Monster before rendering Target
	void RenderBehind(SDL_Renderer* gRenderer, Monster monster[], int camx, int camy, float targetY, float targetH);

	// Render Monster after rendering Target
	void RenderInFront(SDL_Renderer* gRenderer, Monster monster[], int camx, int camy, float targetY, float targetH);

	// Render health bars
	void RenderGUI(SDL_Renderer *gRenderer, Monster monster[], int camx, int camy, int playerIdSelected);

	// Render debug informtation
	void RenderDebug(SDL_Renderer *gRenderer, Monster monster[], int camx, int camy);

	// This function will determine a Monster's spells (usually called after loading Monsters)
	void SetSpells(Monster monster[], int i);

	// get id of a monster in index
	int getID(Monster monster[], int mx, int my);

public: // Save functions

	// Load data
	void LoadData(Monster monster[], int level);

	// Save data
	std::string SaveData(Monster monster[]);
};

#endif /* GAME_ZOMBIE_H_ */
