/*
 * CustomizeCharacter.h
 *
 *  Created on: Apr 20, 2017
 *      Author: Carl
 */

#ifndef CUSTOMIZECHARACTER_H_
#define CUSTOMIZECHARACTER_H_

#include "Bar.h"
#include "Helper.h"
#include "LTexture.h"

#include <fstream>
#include <limits>
#include <sstream>

class CustomizeCharacter : public Bar, public Helper {
public:
	enum Result { Back, Nothing, StartGame, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, CustomizeCharacter::Result &result);
	CustomizeCharacter::Result mousePressed(SDL_Event event);
	CustomizeCharacter::Result mouseReleased(SDL_Event event);
	bool checkCollision(SDL_Rect a, SDL_Rect b);
private:
	void load(SDL_Renderer *gRenderer);
	void save();
	void free();
	const std::string defDir = "resource/data/cfg/";
	/*
	 * 0: Player skin
	 * 1: Player eyes
	 * 2: Player hair
	 * 3: Player shirt
	 * 4: Player pants
	 */
	LTexture gPlayer[5];		// Load textures
	LTexture gPlayerOutline;	// Player outline
	LTexture gBlank;			// New player.png texture file
	LTexture gScene;			// Scene texture
	//LTexture gText;				// Font texture
	LTexture gBG;				// Background texture
	LTexture gButtons;			// Buttons texture
	SDL_Event event;			// Events
	int x, y;					// Button start position
	int w, h;					// Button button size
	int mx,  my;				// Mouse position
	int mex,  mey;				// Mouse position based on screen wise and render size
	bool leftClick;				// Mouse click
	bool quit;
	bool shift;

	/* Main Menu items
	 * 0: Quit
	 * 1: Save Character
	 * 2: Start Game
	 */
	SDL_Rect button[3];
	std::string buttonName[3];

	/* Texture clips
	 * 0: Up 1
	 * 1: Up 2
	 * 2: Up 3
	 * 3: Right 1
	 * 4: Right 2
	 * 5: Right 3
	 * 6: Down 1
	 * 7: Down 2
	 * 8: Down 3
	 */
	SDL_Rect clip[9];

	/* Button texture clips
	 * 0: Quit
	 * 1: Save Character
	 * 2: Start Game
	 */
	SDL_Rect bClip[3];

	/*
	 * 0: r
	 * 1: g
	 * 2: b
	 */

	/* Save index
	 * 0: skin
	 * 1: eyes
	 * 2: hair
	 * 3: shirt
	 * 4: pants
	 */
	int index;

public:		// File functions
	std::ifstream& GotoLine(std::ifstream& file, unsigned int num){
	    file.seekg(std::ios::beg);
	    for(unsigned int i=0; i < num - 1; ++i){
	        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
	    }
	    return file;
	}

	// Load Player colors
	SDL_Color loadColor(std::string fileName, int line) {
		// Individual colors
		int red = 255;
		int green = 255;
		int blue = 255;
		// Temp stringstream
		std::stringstream tempss;
		tempss.str(std::string());
		// Set File location
		tempss << "data/cfg/" << fileName.c_str();
		// Create File
		std::ifstream tempFile(tempss.str().c_str());
		// Get certain line from File
		GotoLine(tempFile, line);
		// Get data from line
		tempFile >> red >> green >> blue;
		// Close File
		tempFile.close();
		// store color
		SDL_Color color = {red, green, blue};
		/// return color
		return color;
	}

	// Load CFG file
	void loadCFG(std::string fileName, int &x, int &y, int line) {
		std::stringstream tempss;
		tempss.str(std::string());
		// Set File location
		tempss << "data/cfg/" << fileName.c_str();
		// Create File
		std::ifstream tempFile(tempss.str().c_str());
		// Get certain line from File
		GotoLine(tempFile, line);
		// Get data from line
		tempFile >> x >> y;
		// Close File
		tempFile.close();
	}

	// Save Player colors
	/*void saveColor(std::string fileName, int red, int green, int blue, int line){
		// Final data to save
		std::stringstream tempss;
		// Current line index
		int currentLine = 0;
		// Current line data
		// File Path
		std::stringstream dir;
		// Set defult path
		dir << "data/cfg/";
		// Set file name
		dir << fileName.c_str();
		std::string str;
		// Open File
		std::ifstream file(dir.str().c_str());
		while (getline(file, str)){
			// Read current line
			std::istringstream iss(str);

			// Temp string to store Name & Score
			std::string temps[3];

			// Store Name & Score in temp string
			iss >> temps[0] >> temps[1] >> temps[2];

			// Overwrite line
			if (line-1 == currentLine) {
				tempss << red << " " << green << " " << blue << "\n";
			}
			// Keep old line
			else{
				tempss << temps[0] << " " << temps[1] << " " << temps[2] << "\n";
			}
			// Keep track of current line
			currentLine++;
		}
		// Close File
		file.close();

		// Overwrite File
		std::ofstream fileS;
		fileS.open(dir.str().c_str());
		fileS << tempss.str().c_str();
		fileS.close();
	}*/

public:		// Bar class;
	Bar skin;
	Bar eyes;
	Bar hair;
	Bar shirt;
	Bar pants;
	int getValueFromBar(int mx, int barX, int widthOnScreen, int maxValue);
	void update(bool leftClick);
	void render(SDL_Renderer *gRenderer);

public:	// Core functions

	void Render(SDL_Renderer *gRenderer);

	void RenderText(SDL_Renderer *gRenderer);
};

#endif /* CUSTOMIZECHARACTER_H_ */
