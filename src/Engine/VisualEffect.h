/*
 * VisualEffect.h
 *
 *  Created on: Aug 28, 2017
 *      Author: Carl
 */

#ifndef ENGINE_VISUALEFFECT_H_
#define ENGINE_VISUALEFFECT_H_

#include "Helper.h"
#include "Particle.h"

class VisualEffect: public Helper  {
public:
	const int max = 100;	// able to spawn 100 visual effects in one scene
	int count = 0;

public:
	float x,y,w,h;
	float duration;
	float maxDuration;
	int occurrence;
	float rate;
	bool isAlive;

public:	// functions
	// Load
	void Load(SDL_Renderer* gRenderer);

	// Free
	void Free();

	// Initialize VFX
	void Init(VisualEffect vfx[]);

	// Spawn a VFX
	void Spawn(VisualEffect vfx[], float x, float y,
			 float w, float h,
			 float maxDuration, int occurrence,
			 float rate);

	// Remove VFX
	void Remove(VisualEffect vfx[]);

	// Update VFX
	void Update(VisualEffect vfx[], int mx, int my, int camx, int camy,
				Particle &part, Particle particle[]);

	// Render VFX
	void Render(VisualEffect vfx[], int camx, int camy, SDL_Renderer* gRenderer);

	void PlayVFX(VisualEffect vfx[], Particle &part, Particle particle[]);
};

#endif /* ENGINE_VISUALEFFECT_H_ */
