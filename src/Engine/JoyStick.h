/*
 * JoyStick.h
 *
 *  Created on: Aug 31, 2017
 *      Author: Carl
 */

#ifndef ENGINE_JOYSTICK_H_
#define ENGINE_JOYSTICK_H_

class JoyStick {
public:
	SDL_Joystick *joy=NULL;

	JoyStick();
	virtual ~JoyStick();
};

#endif /* ENGINE_JOYSTICK_H_ */
