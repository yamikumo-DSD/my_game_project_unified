//option.h

#ifndef __OPTION_H
#define __OPTION_H

#include "object.h"
#include "mathematics.h"

namespace MyGameProject
{
	class Player;

	class Option final : public MovingObject
	{
	private:
		const Player& player;
		Point2D rational_p;  //Rational point based on the player's coordinate.
		int count;
		Real angle;
		Real direction;
	public:
		Option(const Player& _player,const Point2D& _rational_p); 
		Real dir(void) const;
		virtual void draw(void) const override;
		virtual void update(void) override;
		static void preperation(void);
		virtual ~Option(void);
	};
}

#endif