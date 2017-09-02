/*
 * JoyStick.cpp
 *
 *  Created on: Aug 31, 2017
 *      Author: Carl
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "JoyStick.h"

JoyStick::JoyStick() {
	// TODO Auto-generated constructor stub
	//printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
    joy = SDL_JoystickOpen(0);
    if (joy == NULL) {
    	//printf("Error opening joystick!");
    }else{
    //	printf("Opened joystick!");
    }
}

JoyStick::~JoyStick() {
	// TODO Auto-generated destructor stub
    SDL_JoystickClose(joy);
	joy = NULL;
}

