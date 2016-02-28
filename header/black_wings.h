//black_wings.h

#ifndef __BLACK_WINGS_H
#define __BLACK_WINGS_H

#include "boss.h"
#include <memory>
#include <functional>

namespace MyGameProject
{
	class BlackWingsImple;

	class BlackWings : public Boss
	{
	private:
		friend class BlackWingsImple;
		std::unique_ptr<BlackWingsImple> imple;

		virtual void custom_updater(void) override final;
		virtual void draw_charactor(void) const override final;
		virtual void draw_health_bar(void) const override final;
		int health;
		bool immortal_flag;
		std::function<void(float)> change_play_speed;
	public:
		BlackWings
			(
			BulletPtrContainer& _bullets,
			ItemPtrContainer& _items,
			std::function<void(const std::string&)> _se_flag_setter,
			std::function<void(float)> _play_speed_changer,
			const Player& _player
			);
		static void preperation(void);
		virtual void hit(int _shot_power) override final;
		virtual ~BlackWings(void);
		virtual int get_health(void) const override final;
	};
}

#endif