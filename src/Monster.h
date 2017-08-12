/*
 * Monster.h
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */

#ifndef GAME_ZOMBIE_H_
#define GAME_ZOMBIE_H_

#include "Engine/Helper.h"
#include "Players.h"

class Monster : public Helper {
public:	// Media
	LTexture gMonster;
	/*
	 * 12 monsters in Total
	 * 108 clips
	 * 9 x 12 ( 9 clips going to the right, 12  going down)
	 */
	SDL_Rect clip[108];
	int count;
	const int max = 100;
	bool asteroidsClear;
public:
	float x2, y2;
	int radius;
	float x,y;
	float w,h;			// Enemy collision size
	int realw, realh;	// Enemy size in pixels being drawn
	float angle;
	float vX,vY;
	float velX,velY;	// Apply to monster for knockback effect
	float speed;
	/* Type of monster we are spawning
	 * 0: Skeleton
	 * 1: Goblin
	 */
	int type;
	int id;				// Monster clip from texture
	double health;
	double damage;
	double timer;			// Used for shooting
	float distance;
	std::string tag;
	bool alive;
	bool collision;
	bool onScreen;

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

public:
	void Init(Monster monster[]);
	void Load(SDL_Renderer* gRenderer);
	void Free();

	void RemoveAll(Monster monster[]);
	void spawn(Monster monster[], float x, float y,
			   float w, float h, int imageW, int imageH,
			   float angle, float speed,
			   double id, double type, double health,
			   int distanceHeadIsFromCenterOfImage, int bulletW, int bulletH);
	void update(Monster monster[], Particle particle[], Particle &p_dummy,
				Players &player, Mix_Chunk* sLazer,
				int camx, int camy);
	void render(SDL_Renderer* gRenderer, Monster monster[], int camx, int camy);

public: // tile saving functions
	// Load Tile data
	void loadTiles(Monster monster[], int level);

	// Save Tile data
	std::string saveTiles(Monster monster[]);
};

#endif /* GAME_ZOMBIE_H_ */
