/*
 * Helper.h
 *
 *  Created on: May 18, 2017
 *      Author: Carl
 */

#ifndef LOCAL_HELPER_H_
#define LOCAL_HELPER_H_

#include "Timers.h"

#include <SDL2/SDL.h>

class Helper {
public:
    Timer fps;

    // Define Infinite (Using INT_MAX caused overflow problems)
    #define INF 10000

    struct Point
    {
        int x;
        int y;
    };
public:
    // fps
    bool cap 					= true;
	int frame 					= 0;
    const int FRAMES_PER_SECOND = 60;
	//const int screenWidth = 2880;
	//const int screenHeight = 1620;
    //const int screenWidth = 2304;
    //const int screenHeight = 1296;
	//const int screenWidth = 480;
	//const int screenHeight = 270;
	//const int screenWidth = 1280;
	//const int screenHeight = 720;
	const int screenWidth = 270;
	const int screenHeight = 152;
	//const int screenWidth = 640;
	//const int screenHeight = 360;
public:

	// Check sollision
	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	// Generate a random double number
	double randDouble(double fMin, double fMax);

	// Set Texture clips
	void setClips(SDL_Rect &tempRect, int x, int y, int w, int h);


public:	// Check if 2 line segments intersect

	bool onSegment(Point p, Point q, Point r);

	int orientation(Point p, Point q, Point r);

	bool doIntersect(Point p1, Point q1, Point p2, Point q2);

};

#endif /* LOCAL_HELPER_H_ */
