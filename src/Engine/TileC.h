/*
 * TileC.h
 *
 *  Created on: Jul 4, 2017
 *      Author: Carl
 */

#ifndef TILEC_H_
#define TILEC_H_

class TileC {
public:	// editor variables
	int multiW = 1;
	int multiH = 1;
	int tilew = 16;
	int tileh = 16;
	int count = 0;
	const int max = 2096;

public:	// instance variables
	float x;
	float y;
	int w;
	int h;
	/*
	 * 0: Player collision
	 * 1: Particle collision
	 * 2: Others' collision
	 */
	int type;
	int layer;
	bool mouse;			// Mouse point
	bool mouseBox;		// Mouse box
	bool screen;
	bool alive;
	int health;

public:	// Initial functions
	void init(TileC tilec[]);

	// Place one collision tile
	void place(TileC tilec[], float x, float y, int w, int h);

	/* Place multiple collision tile's */
	void spawn(TileC tilec[], int newMx, int newMy, int camx, int camy);

	void copy(TileC tilec[]);

	void remove(TileC tilec[], int click);

	void RemoveAll(TileC tilec[]);

public: // Core functions

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	void checkCollision(TileC tilec[], float x, float y, int w, int h, float &targetAxis, float &targetVelocity);

	void update(TileC tilec[], int newMx, int newMy, int mex, int mey, int camx, int camy, int screenWidth, int screenHeight);

	void render(SDL_Renderer *gRenderer, TileC tilec[], int camx, int camy);

	//void renderDebug(SDL_Renderer *gRenderer, TileC tilec[], int newMx, int newMy, int camx, int camy);

	std::string saveTiles(TileC tilec[]);

	void loadTiles(TileC tilec[], int level);
};


#endif /* TILEC_H_ */
