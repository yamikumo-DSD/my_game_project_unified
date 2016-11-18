//laser_launcher.cpp

#include "laser_launcher.h"
#include "add_get.h"
#include "draw_order.h"
#include "mathematics.h"

namespace MyGameProject
{
	static constexpr int INITIAL_HEALTH{ 900 };

	struct LaserLauncherImpl
	{
		Real d{0};
	};

	namespace ImageHandle
	{
		static int shaft{ 0 };
		static int right_shell{ 0 };
		static int left_shell{ 0 };
	}

	LaserLauncher::LaserLauncher
	(
		BulletPtrContainer& _bullets,
		SEManager& _se_manager,
		gp::smart_ptr<EnemyOrder> _order,
		const Player& _player
	) noexcept
		:MobEnemy
		(
			_bullets, _se_manager, _order,
			INITIAL_HEALTH, Class::SMALL, _player, ShapeElement::Circle(40)
		)
	{}

	LaserLauncher::LaserLauncher(const LaserLauncher& _laser_launcher) noexcept
		:MobEnemy(_laser_launcher)
	{
	}

	LaserLauncher::~LaserLauncher(void) noexcept = default;

	void LaserLauncher::hit(int _power)
	{
		if (get_count() >= 180)
		{
			damage(_power);
		}
	}
	
	void LaserLauncher::accessory_custom_updater(void) 
	{
		const auto count{get_count()};
		if (count < 120)
		{
			d->d = -12;
		}
		else if (count >= 120 && count < 180)
		{
			d->d += (12.f / 60);
		}
	}

	void LaserLauncher::draw(void) const
	{
		using namespace ImageHandle;

		const auto count{get_count()};
		const auto angle{this->angle()};

		const Point2D P{d->d * Point2D(-sin(angle), cos(angle))};
		gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 0.08, angle, shaft, true);
		gp::DrawRotaGraphF(gp::level(11), pos().x() - P.x(), pos().y() - P.y(), 0.08, angle, right_shell, true);
		gp::DrawRotaGraphF(gp::level(11), pos().x() + P.x(), pos().y() + P.y(), 0.08, angle, left_shell, true);
	}

	void LaserLauncher::preparation(void)
	{
		using namespace ImageHandle;

		shaft = add_get("../../data/img/laser_launcher/shaft.png");
		right_shell = add_get("../../data/img/laser_launcher/right_shell.png");
		left_shell = add_get("../../data/img/laser_launcher/left_shell.png");
	}
}