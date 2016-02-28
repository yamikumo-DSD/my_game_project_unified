//hyper_mode_back.h

#ifndef __HYPER_MODE_BACK
#define __HYPER_MODE_BACK

#include "back_ground.h"
#include "mathematics.h"
#include "object.h"
#include "smart_ptr.h"
#include <vector>

namespace MyGameProject
{
	class Player;

	class Spirit : public MovingObject, public DynamicObject
	{
	private:
		int brt;
		Real brt_theta;
		int brt_A;
		Real mag;
		Real speed;
	public:
		Spirit(const Point2D& _initial_pos);
		virtual void update(void) override final;
		virtual void draw(void) const override final;
		static void preperation(void);
		virtual ~Spirit(void);
	};

	class HyperModeBack : public BackGround
	{
	private:
		virtual void custom_updater(void) override final;
		Real rot;
		Real mahoujin_rot;
		std::vector<std::unique_ptr<Spirit>> spirits;
		const Player& player;
	public:
		HyperModeBack(const Player& _player);
		virtual void draw_rear(void) const override final;
		static void preperation(void);
		virtual ~HyperModeBack(void);
	};
}

#endif
