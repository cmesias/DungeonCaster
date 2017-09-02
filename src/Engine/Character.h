/*
 * Character.h
 *
 *  Created on: Aug 25, 2017
 *      Author: Carl
 */

#ifndef ENGINE_CHARACTER_H_
#define ENGINE_CHARACTER_H_

#include "Helper.h"
#include "LTexture.h"
#include "Particle.h"
#include "Spell.h"
#include <SDL2/SDL_Mixer.h>

class Character: public Helper {
	LTexture gTexture;
	LTexture gText;
	LTexture gArrowTexture;
	std::vector<SDL_Rect> clips;
public:
	Character();
	virtual ~Character();

public:	// Object Variables
	float x, y;			// Coordinates
	//float x2, y2;		// Center of character
	float w, h;			// Size
	float vX, vY;		// Velocity
	float hp;			// Health
	float angle;		// Angle
	std::string tag;	// object tag
	bool isAlive;
	/*bool collision;
	bool mouse;
	bool mouseBox;
	bool onScreen;*/

public: // user statistics that the Player sees
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
	//int targetID;				// ID of target being selected in world
	PointEr target;				// Location of target

public:	// Spells

	// Vector class for spells
	std::vector<Spell> spell;
	int spellIndex;

public:	// Controls
	bool useKey;				// Used to interact with World Objects and doors
	bool camlocked;

public:	// Movement
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;

public:	// Animations (Make into a class?)
	/*	Player
	 * 0: walk down
	 * 1: walk up
	 * 2: walk right
	 * 3: walk left
	 * 8-9: attack
	 */
	int facing;
	int frame;					// Current frame
	int compiledFrame;			// image_index, the texture id
	float frameTimer;			// Current timer
	float maxDuration;			// duration of attack (this will be replaced by spell before attack)
	float frameSpeed;			// Frame speed
	float castDelay;			// duration of delay before attack (this will be replaced by spell before attack)
	bool casting;				// Casting a spell (ability attack)
	bool basicAttack;			// Basic attacks
	bool basicAttackDelay;		// basic attack delay
	float bATimer;				// Basic attack timer
	float atkSpe;				// Basic attack attack speed
	bool moving;				// if object is moving
	float bmx, bmy;				// Position to shoot towards
	std::string spellName;		// Used for basic attack

	// test animation variables
	//int image_inidex;
	//int sprite_index;

public:	// Universal controls
	bool *key_castspell=NULL;

public:	// Keyboard controls
	bool spaceBar;

public:	// controls
	// Player controls
	int joyStickID;
	int controls;		// [0] Keyboard, [1] Xbox 360 Controller
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
	bool shift;
	bool ctrl;

public:	// Core functions

	void Free();

	void Update(Particle &part, Particle particles[], int mx, int my,
			 	Mix_Chunk *newSShoot);

	void Render(SDL_Renderer *gRenderer, float camx, float camy);

	void RenderDebug(SDL_Renderer *gRenderer, float camx, float camy, TTF_Font *gFont);

public:	// Mutator functions

	// Upon target creation
	void setParam(SDL_Renderer *gRenderer, std::string newTag, float hp,
				  float newX, float newY,
				  float newW, float newH);

	// Set joystick ID
	void setJoyStick(int newJoystickId);

	// Get texture for character
	void setTexture(LTexture &newTexture, std::vector<SDL_Rect> clips);

	// Completely stop movement, no decay
	void stopMovement();

	// Activate spell cast
	void activateSpell();

	// Activate basic attack
	void activateBasicAtack();

	// Cast a spell (might make this a Universal function and put it in Helper.cpp class
	void castSpell(std::vector<Spell> &spell, int spellIndex, int currentFrame, Particle &part, Particle particle[], Mix_Chunk *sSpellSFX);

	// Cast a spell (using displayName of spell to cast, and instantly)
	void castSpell(std::vector<Spell> &spell, std::string displayName, Particle &part, Particle particle[], Mix_Chunk *sSpellSFX);

	// Set target for Character
	void setTarget(float *newTargetX, float *newTargetY, int newTargetID);

	// Add health
	void addHealth(float amount);

	// Add mana
	void addMana(float amount);

	// Minus health
	void minusHealth(float amount);

	// Minus health
	void minusMana(float amount);

public:	// Accessor functions

	// Get Character target details
	int getTargetID();

	// get x
	float getX();

	// get y
	float getY();

	// get x center
	float getCenterX();

	// get y center
	float getCenterY();

	// get center of cursor x-axis
	float getCenterCursorX();

	// get center of cursor y-axis
	float getCenterCursorY();

	// get center of mouse x-axis
	float getCenterMouseX();

	// get center of cursor y-axis
	float getCenterMouseY();


public:	// Control functions

	SDL_Joystick *joy=NULL;
	SDL_Haptic *haptic=NULL;
	const int JOYSTICK_DEAD_ZONE = 8000;
	const int LTRIGGER_DEAD_ZONE = 25000;
	const int RTRIGGER_DEAD_ZONE = 25000;
	void OnKeyDown( SDL_Keycode sym );
	void OnKeyUp( SDL_Keycode sym );
	void MouseClick( SDL_Event &e );
	void updateJoystick( SDL_Event &e );
};

#endif /* ENGINE_CHARACTER_H_ */
