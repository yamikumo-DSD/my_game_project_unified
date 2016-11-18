//pattern113.cpp

#include "enemy_act_pattern.h"
#include "player.h"
#include "lazer.h"

namespace MyGameProject
{
	using P = Point2D;

	struct Pattern113 final : Pattern
	{
		OVERRIDE_PARENTHESIS
		{
			static constexpr Real W{0.02f};
			ALIASES;

			const auto angle{_enemy.angle()};
			const auto dir{player.pos() - pos};
			const auto d{norm(dir)};

			if (d != 0)
			{
				const auto cross{(cos(angle) * dir.y() - sin(angle) * dir.x()) / d};
				if (std::abs(cross) > W)
				{
					if (cross > 0) { _enemy.angle(angle + W); }
					else { _enemy.angle(angle - W); }
				}
				else
				{
					_enemy.angle(angle + cross);
				}
			}

			if (count >= 200 && count % 200 == 0)
			{
				*find_vacant_object(SharedObject::bullets()) =
				std::make_unique<Lazer>
				(
					_enemy, player, 
					pos + 40 * P(cos(_enemy.angle()), sin(_enemy.angle())),
					_enemy.angle(), 
					[&pos, &_enemy](Bullet& _b) {_b.pos() = _enemy.pos() + 40 * P(cos(_enemy.angle()), sin(_enemy.angle())); },
					50, 150,
					[&_enemy](auto&& _b, auto&& _dir)
					{
						_dir = _enemy.angle();
					}
				);
			}
		};
	};

	MobEnemy::ActPattern pattern113(const std::vector<Point2D>& _args) 
	{
		return Pattern113(); 
	}
}
