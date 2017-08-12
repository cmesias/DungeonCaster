/*
 * Item.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Carl
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Basics.h"
#include "Helper.h"
#include <SDL2/SDL.h>

class Item: public Basics, public Helper  {

public:	// Resources
	LTexture gItem;
	SDL_Rect rClips[56];
	int WIDTH = 7;
	int HEIGHT = 8;

public:	// Core functions

	// Initialize
	void Init(Item item[]);

	// Load
	void Load(SDL_Renderer *gRenderer);

	// Load
	void Free();

	// Remove
	void Remove(Item item[], int click);

	// Remove all
	void RemoveAll(Item item[]);

	// Spawn
	void Spawn(Item item[], float x, float y, float w , float h);

	// Update
	void Update(Item item[], int mouseX, int mouseY, int mx, int my, int camx, int camy);

	// Render
	void Render(SDL_Renderer *gRenderer, Item item[], int camx, int camy);

public:	// Save functions

	// Load data
	void LoadData(Item item[], int level);

	// Save data
	std::string SaveData(Item item[]);

};

#endif /* OBJECT_H_ */
