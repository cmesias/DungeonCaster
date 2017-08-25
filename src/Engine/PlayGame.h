/*
 * PlayGame.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef PLAYGAME_H_
#define PLAYGAME_H_

#include "../Monster.h"
#include "../Player.h"
#include "Spawners.h"
#include "Helper.h"
#include "Item.h"
#include "Options.h"
#include "TileBar.h"
#include "Tiles.h"
//#include "TileC.h"

class PlayGame : public Helper, public Options {

public: // System variables
	enum Result { Back, Nothing, StartGame, ShowingMenu, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, PlayGame::Result &result, int levelToLoad);
	PlayGame::Result mousePressed(SDL_Event event);
	PlayGame::Result mouseReleased(SDL_Event event);

public:	// Scene textures
	LTexture gDoor;
	/*
	 * 0: Closed
	 * 1: Open
	 * 2: Spawn tile
	 */
	SDL_Rect rDoor[3];
	LTexture gBG;
	LTexture gTargetTexture;
	LTexture gLightBG;		// This isi rendered first before lights, this sets the default lighting
	LTexture gLight;
	LTexture gCircles;
	SDL_Rect rCircles[2];	// 0: Blue, 1: Red

	// Light dimming variables
	float dimSize;
	float dimDir;

	class Text {
	public:
		int count;
		float x;
		float y;
		int w;
		int h;
		int alpha;
		int timer;
		bool alive;
		float vX;
		float vY;
		std::string textfield;
		SDL_Color color;
		void init(Text text[]) {
			count =0;
			for (int i=0; i<100; i++) {
				text[i].alive = false;
				text[i].color = {255,255,255};
			}
		}
		void spawn(Text text[], float x, float y, float vX, float vY, int alpha, std::string textfield, SDL_Color color = {255,255,255}) {
			for (int i=0; i<100; i++) {
				if (!text[i].alive) {
					text[i].x = x;
					text[i].y = y;
					text[i].w = 16;
					text[i].h = 16;
					text[i].vX = vX;
					text[i].vY = vY;
					text[i].timer = timer;
					text[i].color = color;
					text[i].alpha = alpha;
					text[i].textfield = textfield;
					text[i].alive = true;
					count++;
					break;
				}
			}
		}
		void update(Text text[]) {
			for (int i=0; i<100; i++) {
				if (text[i].alive) {
					text[i].x += text[i].vX;
					text[i].y += text[i].vY;

					text[i].alpha -= 5;
					if (text[i].alpha <= 0) {
						text[i].alive = false;
						count--;
					}
				}
			}
		}
	};

	Text tex;
	Text text[100];

public:	// Other classes

	// Monsters
	Monster mon;
	Monster monster[100];
	//  Spawners
	Spawner spaw;
	Spawner spawner[200];
	// Player
	Player player;
	// Tiles
	Tile tl;
	Tile tile[3000];

	// Collision Tiles
	//TileC tc;
	//TileC tilec[2096];
	SDL_Rect rClips[64]; 		// doorWidth * doorHeight = 64

	// Tilebar
	TileBar tb;
	TileBar tilebar[80];
	SDL_Rect rTiles[80];		// how many unique tiles are, on the tile-bar

	// Items
	Item obj;
	Item item[100];
public:
    // camera
    float camx;
    float camy;
    bool camlock;

public:	// Core functions

	// Initialize
	void Init(Particle &part, Particle particles[]);

	// Load resources
	void Load(LWindow &gWindow, SDL_Renderer *gRenderer, Particle &part, Particle particles[]);

	// Free resources
	void Free(Particle &part);

	// Update everything
	void Update(LWindow &gWindow, SDL_Renderer *gRenderer, Particle &part, Particle particles[]);

public:	// Render Editor UI

	// Render debug information
	void RenderDebug(SDL_Renderer *gRenderer, Particle &part, Particle particles[]);

	// Render tile in hand
	void RenderHand(SDL_Renderer *gRenderer);

	// Render text
	void RenderText(SDL_Renderer *gRenderer, LWindow &gWindow, Particle &part, Particle particles[]);

	// Render Editor GUI
	void RenderUI(SDL_Renderer *gRenderer);

public:	// Render Player GUI (or what the Player sees including Tiles)

	// Render foreground
	void RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render objects
	void Render(SDL_Renderer *gRenderer, LWindow &gWindow, Particle &part, Particle particles[]);

	// Render breakable textures onto Tiles
	void RenderBreak(SDL_Renderer *gRenderer, int layer);

	// Render lights
	void RenderLights(SDL_Renderer *gRenderer, Particle &part, Particle particles[]);

	// Render Player GUI
	void RenderGUI(SDL_Renderer *gRenderer);


public:	// Functions mixed with other classes

	// Check collision between Particle & Monster
	void checkCollisionParticleMonster(Particle &part, Particle particles[]);

	// Check collision between Tile & Monster
	void checkCollisionTileMonster();

	// Check collision between Particle & Player
	void checkCollisionParticlePlayer(Particle &part, Particle particles[]);

	// Check collision between Grenade Particle & Enemies
	void checkCollisionGrenadePlayer(Particle &part, Particle particles[]);

	// Check collision between Items & Player
	void checkCollisionItemPlayer();

	// Check collision between certain Tiles & Player
	void checkCollisionTilePlayer();

	// Check collision between certain Collision Tiles & Player
	void checkCollisionTilecPlayer();

	// Check collision between Monster & Player
	void checkCollisionMonsterPlayer();

	// Check collision between Particles that do damage & Tiles
	void checkCollisionParticleTile(Particle &part, Particle particles[]);

	// Spawn Asteroids upon destroying all Asteroids
	void spawnAsteroidsNow2();

public:	// Manual Updates

	// Handle collision of objects and Level Size
	void ClampObjectsToLevelSize(Particle &part, Particle particles[]);

	// Update Player
	void UpdatePlayer(Particle &part, Particle particles[]);

	// Have Monster find the shortest path towards the Player
	void UpdateMonsterPlayer(SDL_Renderer *gRenderer);

public:	// Functions that serve multiple purposes

	// Knock-back effect: take the position of the explosion, and the object your are knocking back
	void knockbackEffect(float targetX, float targetY, int targetW, int targetH,
						 float objectX, float objectY, int objectW, int objectH,
						 float &objectVX,float &objectVY, float force);

	// Check distance of 2 objects
	double checkDistance(double x2, double y2, double targetX, double targetY);

public:	// Editor Variables & functions

	int playerStateLevel;			// Player's stage level
	float spawnX = 0, spawnY = 0;
	float doorX;
	float doorY;
	/* Placement types
	 * 0: Tiles
	 * 2: Collision Tiles (depreciated)
	 * 3: Monsters
	 *
	 */
	int place_type;				// 0: Tiles, 1: Collision Tiles
	int clampSize;				// Magnet pixel size
	float mouseX;
	float mouseY;
	int newMx, newMy;
	bool camUp = false;
	bool camDown = false;
	bool camLeft = false;
	bool camRight = false;
	bool leftClick = false;
	bool rightClick = false;

	// Load player spawn point
	void loadSpawnPoint(int level);

	void editorOnKeyDown(SDL_Keycode sym, Particle &part, Particle particles[]);

	void editorOnKeyUp(SDL_Keycode sym);

	// Clear current working level
	void ClearLevel(Particle &part, Particle particles[]);

	// Load a level
	void LoadLevel(int level, Particle &part, Particle particles[]);

	// Set Level Size and Keys required
	void SetLevelSizeAndKeys();

	// Reset Level
	void ResetLevel(Particle &part, Particle particles[]);

private:	// used for some debugging
	bool debug;
	bool editor;
	int mx, my;
	bool quit;
	bool shift;
	SDL_Event event;
};

#endif /* PLAYGAME_H_ */
