/*
 * Player.h
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#ifndef LOCAL_PLAYER_H_
#define LOCAL_PLAYER_H_

#include "Engine/Helper.h"
#include "Engine/LTexture.h"
#include "Engine/LWindow.h"
#include "Engine/TileC.h"
#include "Engine/Tiles.h"
#include "Engine/Particle.h"

#include <SDL2/SDL_ttf.h>

#include <vector>

//Player
class Player: public Helper {
public:
	enum Result { QUIT, LOADMENU, PLAYGAME, QUITPLAYGAME };

public:	// resources
	// Local resources
	LTexture gPlayer;
	LTexture gShield;
	SDL_Rect rShield[7];
	SDL_Rect rThrusters[4];
	/* 0-1: walk down
	 * 2-3: walk up
	 * 4-5: walk right
	 * 6-7: walk left */
	SDL_Rect rPlayer[8];

public:	// Spell class
	class Spell {
	public:
		std::string displayName;	// name of spell for GUI
		int type;
		float size;
		float speed;
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
	    float manaCost;
	    float baseCooldown;
	    float cooldownTimer;
	    bool activate;
	    bool cooldown;
	    Spell(std::string newDisplayName,
	    		int newType, float newSize, float newSpeed,
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
			size = newSize;
			speed = newSpeed;
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
	    }
	};
	// Create vector to store Spells
	std::vector<Spell> spell;

public:	// Animations
	/*
	 * 0: walk down
	 * 1: walk up
	 * 2: walk right
	 * 3: walk left
	 */
	int facing;
	double frameTimer;			// Current timer
	double frameSpeed;			// Frame speed
	int frame;					// Current frame

public:	// Attacks

	/*
	 * 0: Fireball Spell (Yellow colored, Normal Spell)
	 * 1: Blue Spell (Lightning?)
	 * 2: Copper-reddish Spell (Maybe like a big burst?)
	 */
	int spellIndex;
	bool moveDelay;
	double moveDelayTimer;

public:	// In-game variables
	int coins;					// Number of coins the Player has
	int keys;					// Number of keys in Player inventory
	int collectedKeys;			// Number of keys accumulated for that Stage Level
	double health;
	double maxHealth;
	double healthDecay;			 // gives the effect of a decaying health bar
	double mana;				// Current mana
	double maxMana;				// Max mana capacity
	double manaRegenRate;		// Mana regeneration rate
	double manaTick;			// Current tick

public:	// variables
	std::string tag;
	int timer = 0;
	int w 		= 10;
	int h 		= 6;			// render size in pixels
	float x 	= 0;
	float y 	= 0;
	float x2;
	float y2;


	int panTimer 		= 0;
	int shoottimer 		= 0;
	float radians 		= 0.0,
		   radianCos 	= 0.0,
		   radianSin 	= 0.0;
	float vX = 0.0,
		  vY = 0.0,
		  thrustSpeed 	= 0.0;
	bool camlocked 		= true;

public:	// Ship Variables
	float angle;			// This angle is used for moving
	float shootAngle;		// This angle is used to shoot
	float turnSpeed;
	float speed;
	float maxSpeed;

public:	// Movement Variables
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;

public: // Variables


	bool shift 			= false;
	bool ctrl 			= false;
	int timer2 			= 0;
	double time 		= 0;
	bool alive			= false;
	Uint8 alpha			= 255;
	unsigned int score	= 0;
	unsigned int highscore = 0;

	// Fire-rate
	//string power-up 		= "LAZER";
	int powerup 		= 1;
	double barrelW		= 0;	// whether to add width or subtract from gun barrel position
	double barrelH		= 0;


	/*double bulletW 		= 15 + 210/3 + 10;	// position the bullet will come out of the player ONLY if player
	double bulletH 		= 0;	// is centered (have angle at 0.0 to test)
	double particleW 	= 210;*/

	double bulletW 		= 15;	// position the bullet will come out of the player ONLY if player
	double bulletH 		= 0;	// is centered (have angle at 0.0 to test)
	double particleW 	= 3;
	double particleH 	= 3;

	bool initialshot 	= false;
	bool trigger 		= false;
	bool renderFlash	= false; // shuts off right after 1 frame

	// Shield
	int shieldFrame;
	int shieldTick;
	int shieldT;
	bool shield;

	// High-Score display
	std::string highList[10];
	int indx 		= 0;
	int indexSaved 	= -1;

	bool moving = false;

public:	// Team Variables

	/* Team color
	 * 0: Blue
	 * 1: Red
	 */
	int team;

public: // Player abilities
	/* Grenade */
	int grenades;
	int grenadesMax;
	double grenadesCD;
	double grenadesCDSpeed;
	double grenadesCDMax;
	bool grenadeTrigger;

	/* Machine Gun Ammo */
	float AttackSpeed 	= 1.5;
	bool reload = false;		// reload
	double reloadSpeed = 30.0;	// reload speed
	double reloadTimer = 0.0;	// reload timer

public:	// controls
	// Player controls
	int controls;		// [0] Keyboard, [1] Xbox 360 Controller
	SDL_Joystick *joy;
	bool A;
	bool B;
	bool X;
	bool Y;
	bool D_UP;
	bool D_DOWN;
	bool D_LEFT;
	bool D_RIGHT;
	bool LB;
	bool RB;
	bool BACK;
	bool START;
	bool L3;
	bool R3;
	double LAngle;
	double RAngle;
	double zAxisLeft;
	double zAxisRight;
	bool leftclick;
	bool rightclick;
	bool test;

public:	// Core Functions

	// Set position
	void SetPosition(int newX, int newY);

	// Set name
	void SetName(std::string newName);

	// Initialize
	void Init();

	// Load resources
	void Load(SDL_Renderer* gRenderer);

	// Free resources
	void Free();

	// Update
	void Update(Particle particle[], Particle &p_dummy,
				TileC &tc, TileC tilec[],
				Tile &tl, Tile tile[],
				int mx, int my, int camx, int camy,
				LWindow gWindow, SDL_Renderer* gRenderer,
				LTexture gText, TTF_Font *gFont, SDL_Color color,
				Mix_Chunk *sAtariBoom, Mix_Chunk* sLazer, Mix_Chunk* sGrenade,
				Mix_Chunk* sGrenadePickup, Mix_Chunk* sPistolReload);

	// Render
	void Render(int mx, int my, int camx, int camy, LWindow gWindow,
				SDL_Renderer* gRenderer,
				TTF_Font *gFont, TTF_Font *gFont2,
				SDL_Color color, int &PARTICLES, LTexture gText);

	// Player shoot
	void fire(Particle particle[], Particle &p_dummy, int mx, int my,
			  Mix_Chunk* sLazer, Mix_Chunk* sGrenade, Mix_Chunk* sGrenadePickup,
			  Mix_Chunk* sPistolReload);

	// Player controls
	void move(Particle particle[], Particle &p_dummy,
			   TileC &tc, TileC tilec[],
			   Tile &tl, Tile tile[],
			   int mx, int my);

public:	// Game-player functions

	// Applies a shield to Player
	void applyShield();

	// Cast a spell (think of this function as the structure of a Pistol, and what you
	// put in as bullets are the Spells info, such as Velocity and Speed (the bullet)
	void CastSpell();

public:	// Other functions

	// Reset highscore
	void resetHighScore();

	// Load highscore
	void loadScore();

	// Save Highscore
	void saveHighScore();

	// Check collision
	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

public:	// Control functions

	// Joystick Sensitivity
	const int JOYSTICK_DEAD_ZONE = 8000;
	void OnKeyDown( Player &player, SDL_Keycode sym );
	void OnKeyUp( Player &player, SDL_Keycode sym );
	void mouseClickState(Player &player, SDL_Event &e);
	void updateJoystick(Player &player, SDL_Event &e);

};

#endif /* LOCAL_PLAYER_H_ */
