/*
 * Basics.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef BASICS_H_
#define BASICS_H_

class Basics {
public:	// Parent object Variables
	int count;		// number of instances
	int max;		// max number of instances
	int id;			// id of instances

public:	// Object Variables
	float x, y;
	float x2, y2;
	float w, h;
	float vX, vY;
	bool collision;
	bool mouse;
	bool mouseBox;
	bool onScreen;
	bool alive;
};

#endif /* BASICS_H_ */
