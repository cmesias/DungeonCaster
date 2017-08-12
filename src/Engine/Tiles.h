/*
 * Tile.h
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#ifndef LOCAL_TILES_H_
#define LOCAL_TILES_H_

#include "LWindow.h"
#include "LTexture.h"

class Tile {
public:	// editor variables
	/* Tile Variables */
	const int max = 5000;
	int multiW = 1;
	int multiH = 1;
	int tilew = 16;
	int tileh = 16;
	bool hideOtherLayers = false;
	LTexture gTiles;
	LTexture gText;
	TTF_Font *gFont12 = NULL;

public:	// Level Loading Variables
	int tileCount = 0;
	int levelWidth = 64;
	int levelHeight = 64;
	int requiredKeys = 99999;	// number of keys required to move on to the next level


public:	// instance variables
	float x;
	float y;
	int w;
	int h;
	int id;
	int alpha;
	SDL_Rect clip;
	int layer;
	int animTimer;
	int animFrame;
	bool mouse;			// Mouse point
	bool mouseBox;		// Mouse box
	bool screen;
	bool player;
	bool alive;
	std::string side;	// collision with player
	bool collide;		// collision with player

public:	// Initial
	void initTile(Tile tile[]);

	void load(SDL_Renderer *gRenderer);

	void free();

public:	// editor functions
	void placeTile(Tile tile[], float x, float y, int w, int h, int id, int layer, SDL_Rect clip);

	void copyTile(Tile tile[]);

	void ChangeCollision(Tile tile[], int click);

	void changeTileLayer(Tile tile[], int value);

	void removeTile(Tile tile[], int click);

	void removeAllTiles(Tile tile[]);

	/* Place Tile */
	void spawnTile(Tile tile[], int newMx, int newMy, int camx, int camy, SDL_Rect rTiles[]);

public:	// core game functions
	void updateTile(Tile tile[], LWindow &gWindow, int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]);

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	// Tile collision check, player x position
	void checkCollision(Tile tile[], float x, float y, int w, int h, float &coordinateXY, float &velocity);

	void renderTile(SDL_Renderer *gRenderer, Tile tile[], int layer_dummy, int camx, int camy);

	// Render Tile Debug info
	void RenderDebug(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey,
						 int camx, int camy, SDL_Rect rTiles[], int tileSize);

public: // tile saving functions
	// Load Tile data
	void loadTiles(Tile tile[], int level);

	// Save Tile data
	std::string saveTiles(Tile tile[]);
};

#endif /* LOCAL_TILES_H_ */
