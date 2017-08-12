/*
 * Bar.h
 *
 *  Created on: Apr 28, 2017
 *      Author: Carl
 */

#ifndef BAR_H_
#define BAR_H_

#include "LTexture.h"

#include <sstream>

struct Bar {
public:
	int x;				// x position of bar
	int y;				// y position of bar
	int width;			// bar width
	int height;			// bar height
	int color[3];		// bar color
	int maxColor;
	std::string name;
	SDL_Rect bar[3];
	LTexture gText;			// Font texture
	TTF_Font *gFont12;		// Font
	TTF_Font *gFont18;		// Font

	void load();
	void free();

	// Get value from a bar relative to mouse
	int getValueFromBar(int mx, int barX, int widthOnScreen, int maxValue);

	// initialize
	void init(std::string newName, int newWidth, int newHeight);

	void update(bool leftClick, int mex, int mey);

	// Render bars
	void Render(SDL_Renderer *gRenderer);

	// Render Bar Texts
	void RenderText(SDL_Renderer *gRenderer);
};

#endif /* BAR_H_ */
