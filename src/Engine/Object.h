/*
 * Object.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Carl
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Basics.h"
#include "Helper.h"
#include <SDL2/SDL.h>

class Object: public Basics, public Helper  {

public:	// Resources
	LTexture gObject;
	SDL_Rect rClips[56];
	int WIDTH = 7;
	int HEIGHT = 8;

public:	// Core functions

	// Initialize
	void Init(Object object[]);

	// Load
	void Load(SDL_Renderer *gRenderer);

	// Load
	void Free();

	// Remove
	void Remove(Object object[], int click);

	// Remove all
	void RemoveAll(Object object[]);

	// Spawn
	void Spawn(Object object[], float x, float y, float w , float h);

	// Update
	void Update(Object object[], int mouseX, int mouseY, int mx, int my, int camx, int camy);

	// Render
	void Render(SDL_Renderer *gRenderer, Object object[], int camx, int camy);

	std::string saveTiles(Object object[]);

	void loadTiles(Object object[], int level);

};

#endif /* OBJECT_H_ */
