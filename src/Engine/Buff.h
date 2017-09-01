/*
 * Buff.h
 *
 *  Created on: Aug 25, 2017
 *      Author: Carl
 */

#ifndef ENGINE_BUFF_H_
#define ENGINE_BUFF_H_

class Buff {
public:
	float duration;
	bool isAlive;
	enum DamageType {Uninitialized, Poisoned, Burned, Stunned};
	static DamageType _damageType;

	// Buff creation
	Buff(Buff::DamageType _newDamageType, float newDuration);

	// Buff updates
	void Update(std::vector<Buff*> buffs);
};


#endif /* ENGINE_BUFF_H_ */
