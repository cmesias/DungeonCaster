/*
 * Buff.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: Carl
 */

#include <iostream>
#include <list>
#include <vector>
#include "Buff.h"


Buff::Buff(Buff::DamageType _newDamageType, float newDuration) {
	duration = newDuration;
	_damageType = _newDamageType;
	isAlive = true;
}

void Buff::Update(std::vector<Buff*> buffs) {
	/*for (unsigned int i=0; i<buffs.size(); i++) {
		if (buffs[i]->duration > 0) {
			buffs[i]->duration -= rate;
		}
	}*/
}

Buff::DamageType Buff::_damageType = Uninitialized;
