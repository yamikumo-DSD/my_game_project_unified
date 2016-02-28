//short_range_weapon.h

#ifndef __SHORT_RANGE_WEAPON_H
#define __SHORT_RANGE_WEAPON_H

#include "object.h"
#include "mathematics.h"

namespace MyGameProject
{
	class Player;

	class ShortRangeWeapon : public MovingObject, public HitableObject
	{
	private:
		Real angle;
		int count;
		const Player& player;
		Real magnification;
		int pow;
	public:
		explicit ShortRangeWeapon(const Player& _player);
		virtual void draw(void) const override;
		virtual void update(void) override;
		int power(void) const;
		static void preperation(void);
		virtual ~ShortRangeWeapon(void);
	};
}

#endif
