//mega_crow.h

#pragma once

#include <memory>
#include "boss.h"

namespace MyGameProject
{
	struct MegaCrowImple;

	class MegaCrow : public Boss
	{
	private:
		friend struct MegaCrowImple;
		std::unique_ptr<MegaCrowImple> imple;

		virtual void custom_updater(void) override final;
		virtual void draw_charactor(void) const override final;
		virtual void draw_health_bar(void) const override final;
		int health;
		bool immortal_flag;
		std::function<void(float)> change_play_speed;
	public:
		MegaCrow
		(
			BulletPtrContainer& _bullets,
			ItemPtrContainer& _items,
			std::function<void(const std::string&)> _se_flag_setter,
			std::function<void(float)> _play_speed_changer,
			const Player& _player
		);
		static void preperation(void);
		virtual void hit(int _shot_power) override final;
		virtual ~MegaCrow(void);
		virtual int get_health(void) const override final;
		virtual BackGroundControl::OptionalSignal notify(void) const noexcept override final;
	};
}
