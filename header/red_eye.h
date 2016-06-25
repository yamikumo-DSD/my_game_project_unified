//red_eye.h

#pragma once

#include "mob_enemy.h"

namespace MyGameProject
{
	class RedEye : public MobEnemy
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
		virtual void accessory_custom_updater(void) override final;
	public:
		RedEye
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
		) noexcept;
		RedEye(const RedEye& _red_eye) noexcept;
		RedEye(RedEye&& _red_eye) noexcept;
		virtual ~RedEye(void) noexcept;

		virtual void draw(void) const override final;
		static void preparation(void);
	};
}