//bomber.cpp

#include "bomber.h"
#include "add_get.h"
#include "draw_order.h"
#include "mathematics.h"
#include "debug_value.h"
#include "border_check.h"

namespace MyGameProject
{
	static constexpr int INITIAL_HEALTH{ 900 };

	struct BomberImpl
	{
		Real shell_opening_speed{0};
		bool is_shell_motion_proceeding{false};
		static constexpr Real OPEN_POS { 0};
		static constexpr Real CLOSE_POS{-7};
		Real d{CLOSE_POS};
		void shell_motion(void)
		{
			if (is_shell_motion_proceeding)
			{
				d += shell_opening_speed;
				if (d > OPEN_POS) 
				{
					d = OPEN_POS; 
					is_shell_motion_proceeding = false;
					shell_opening_speed = 0;
				}
				else if (d < CLOSE_POS) 
				{
					d = CLOSE_POS; 
					is_shell_motion_proceeding = false;
					shell_opening_speed = 0;
				}
			}
		}
		enum class ShellMotion {CLOSE, OPEN,};
		bool set_shell_motion(int _time, ShellMotion _motion) noexcept
		{
			if (!is_shell_motion_proceeding)
			{
				if (_motion == ShellMotion::OPEN)
				{
					shell_opening_speed = +(OPEN_POS - CLOSE_POS) / _time;
				}
				else
				{
					shell_opening_speed = -(OPEN_POS - CLOSE_POS) / _time;
				}
				is_shell_motion_proceeding = true;
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	namespace ImageHandle
	{
		static int core{0};
		static int upper_shell{0};
		static int lower_shell{0};
	}

	Bomber::Bomber
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
	{
	}

	Bomber::Bomber(const Bomber& _bomber) noexcept
		:MobEnemy(_bomber)
	{
	}

	Bomber::~Bomber(void) noexcept
	{
	}

	void Bomber::hit(int _power)
	{
		if (d->d > -2) { damage(_power); }
	}
	
	void Bomber::accessory_custom_updater(void) 
	{
		const auto count{get_count()};

		d->shell_motion();

		//Rotate.
		angle(angle() + 0.1 * sin(count / 180.f));

		if (!is_within_window(pos(), 100)) { set_flag_off(); }
	}

	void Bomber::open_shell(int _time)
	{
		d->set_shell_motion(_time, BomberImpl::ShellMotion::OPEN);
	}

	void Bomber::close_shell(int _time)
	{
		d->set_shell_motion(_time, BomberImpl::ShellMotion::CLOSE);
	}

	void Bomber::draw(void) const
	{
		using namespace ImageHandle;

		const auto count{get_count()};
		const auto angle{this->angle()};

		const Point2D P{d->d * Point2D(-sin(angle), cos(angle))};
		gp::DrawRotaGraphF(gp::level(11), pos().x()        , pos().y()        , 1.0, angle, core       , true);
		gp::DrawRotaGraphF(gp::level(11), pos().x() - P.x(), pos().y() - P.y(), 1.0, angle, upper_shell, true);
		gp::DrawRotaGraphF(gp::level(11), pos().x() + P.x(), pos().y() + P.y(), 1.0, angle, lower_shell, true);
	}

	void Bomber::preparation(void)
	{
		using namespace ImageHandle;
		core = add_get("../../data/img/bomber/core.png");
		upper_shell = add_get("../../data/img/bomber/upper_shell.png");
		lower_shell = add_get("../../data/img/bomber/lower_shell.png");
	}
}
