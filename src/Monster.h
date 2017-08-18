/*
 * Monster.h
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#ifndef GAME_ZOMBIE_H_
#define GAME_ZOMBIE_H_

#include "Engine/Helper.h"
#include "Player.h"

class Monster : public Helper {
public:	// Media
	LTexture gMonster;
	LTexture gBlueDragon;
	/*
	 * 12 monsters in Total
	 * 108 clips
	 * 9 x 12 ( 9 clips going to the right, 12  going down)
	 */
	SDL_Rect clip[108];

	/* Dragon clips
	 * 0-2 facing right neutral
	 * 3-5 facing right head-bounce
	 * 6-8 facing right attack
	 */
	SDL_Rect dragonClips[9];
	int count;
	const int max = 100;
	const int maxTypes = 12;	// max number of unique Monsters
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
	int id;					// Monster clip from texture
	double health;
	double healthDecay;
	double maxHealth;
	double damage;
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

public:	// Patrol Variables
	bool alert;				// Monster is alerted
	float thinkTimer;

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
	bool moving = false;

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

	void Update(Monster monster[], Particle particle[], Particle &p_dummy,
				Player &player, Mix_Chunk* sLazer,
				int camx, int camy);

	// Render Monster before rendering Target
	void RenderBehind(SDL_Renderer* gRenderer, Monster monster[], int camx, int camy, float targetY, float targetH);

	// Render Monster after rendering Target
	void RenderInFront(SDL_Renderer* gRenderer, Monster monster[], int camx, int camy, float targetY, float targetH);

	// Render health bars
	void RenderGUI(SDL_Renderer *gRenderer, Monster monster[], int camx, int camy);

	// Render debug informtation
	void RenderDebug(SDL_Renderer *gRenderer, Monster monster[], int camx, int camy);

public: // Save functions

	// Load data
	void LoadData(Monster monster[], int level);

	// Save data
	std::string SaveData(Monster monster[]);
};

#endif /* GAME_ZOMBIE_H_ */
