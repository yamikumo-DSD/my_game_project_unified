//dragonfly.cpp

#include "dragonfly.h"
#include "header_set_for_mob_enemy.h"
#include "debug.h"
#include "add_get.h"
#include "color.h"
#include "border_check.h"

namespace MyGameProject
{
	static constexpr int INITIAL_HEALTH{1000};
	using P = Point2D;

	namespace ImageHandle
	{
		enum{RIGHT = 0, LEFT  = 1,};
		enum{UPPER = 0, LOWER = 1,};
		static int body{0};
		static std::array < std::array < std::array<int, 2> , 2 > , 2 > wings;
	}

	struct Dragonfly::Impl 
	{
	};

	Dragonfly::Dragonfly
	(
		BulletPtrContainer& _bullets,
		SEManager& _se_manager,
		gp::smart_ptr<EnemyOrder> _order,
		const Player& _player
	) noexcept
		:MobEnemy(_bullets, _se_manager, _order, INITIAL_HEALTH, Class::SMALL, _player, ShapeElement::Circle(15)),
		pimpl(std::make_unique<Impl>())
	{
	}

	Dragonfly::Dragonfly(const Dragonfly& _dragonfly) noexcept
		:MobEnemy(_dragonfly),
		pimpl(std::make_unique<Impl>(*_dragonfly.pimpl))
	{
	}

	Dragonfly::Dragonfly(Dragonfly&& _dragonfly) noexcept = default;

	Dragonfly::~Dragonfly(void) noexcept = default;
	
	void Dragonfly::accessory_custom_updater(void) 
	{
		auto&& count{get_count()};
		if (count > 300 && !is_within_window(pos(), 100)) { set_flag_off(); }
	}

	void Dragonfly::draw(void) const
	{
		using namespace ImageHandle;
		auto&& count{get_count()};
		if (count > 0)
		{
			auto&& x{ pos().x() };
			auto&& y{ pos().y() };

			gp::DrawRotaGraphF(gp::level(12), x, y, 1.0, 0.0, body, true);

			gp::DrawRotaGraphF(gp::level(12), x + 38, y - 10, 1.0, 0.0, wings[RIGHT][LOWER][(count + 1) % 6 > 2], true);
			gp::DrawRotaGraphF(gp::level(12), x + 38, y - 10, 1.0, 0.0, wings[RIGHT][LOWER][(count + 1) % 6 > 2], true);
			gp::DrawRotaGraphF(gp::level(12), x - 38, y - 10, 1.0, 0.0, wings[LEFT ][LOWER][(count + 1) % 6 > 2], true);
			gp::DrawRotaGraphF(gp::level(12), x - 38, y - 10, 1.0, 0.0, wings[LEFT ][LOWER][(count + 1) % 6 > 2], true);
			gp::DrawRotaGraphF(gp::level(12), x + 38, y - 20, 1.0, 0.0, wings[RIGHT][UPPER][count % 6 > 2], true);
			gp::DrawRotaGraphF(gp::level(12), x + 38, y - 20, 1.0, 0.0, wings[RIGHT][UPPER][count % 6 > 2], true);
			gp::DrawRotaGraphF(gp::level(12), x - 38, y - 20, 1.0, 0.0, wings[LEFT ][UPPER][count % 6 > 2], true);
			gp::DrawRotaGraphF(gp::level(12), x - 38, y - 20, 1.0, 0.0, wings[LEFT ][UPPER][count % 6 > 2], true);
		}
	}

	void Dragonfly::preparation(void)
	{
		using namespace ImageHandle;
		body = add_get("../../data/img/dragonfly/body.png");
		wings[RIGHT][LOWER][0] = add_get("../../data/img/dragonfly/right_wing/lower0.png");
		wings[RIGHT][LOWER][1] = add_get("../../data/img/dragonfly/right_wing/lower1.png");
		wings[RIGHT][UPPER][0] = add_get("../../data/img/dragonfly/right_wing/upper0.png");
		wings[RIGHT][UPPER][1] = add_get("../../data/img/dragonfly/right_wing/upper1.png");
		wings[LEFT ][LOWER][0] = add_get("../../data/img/dragonfly/left_wing/lower0.png" );
		wings[LEFT ][LOWER][1] = add_get("../../data/img/dragonfly/left_wing/lower1.png" );
		wings[LEFT ][UPPER][0] = add_get("../../data/img/dragonfly/left_wing/upper0.png" );
		wings[LEFT ][UPPER][1] = add_get("../../data/img/dragonfly/left_wing/upper1.png" );
	}
}
