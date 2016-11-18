//helli.cpp

#include "helli.h"
#include "add_get.h"
#include "draw_order.h"
#include <boost/format.hpp>

namespace MyGameProject
{
	static constexpr int INITIAL_HEALTH{ 900 };

	enum Direction
	{
		RIGHT = 0, FRONT = 1, LEFT = 2,
	};

	struct HelliImpl
	{
		float index{ 1 };
	};

	namespace ImageHandle
	{
		static std::array<int, 3> body;
		static std::array<std::array<int, 3>, 3> fan;
	}

	Helli::Helli
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

	Helli::Helli(const Helli& _helli) noexcept
		:MobEnemy(_helli)
	{
	}

	Helli::~Helli(void) noexcept = default;

	void Helli::hit(int _power)
	{
		damage(_power);
	}
	
	void Helli::accessory_custom_updater(void) 
	{
		const auto count{get_count()};

		if (velocity().x() > 0) { d->index -= 0.1f; }
		else if (velocity().x() < 0) { d->index += 0.1f; }

		if (d->index > 2) { d->index = 2; }
		else if (d->index < 0) { d->index = 0; }
	}

	void Helli::draw(void) const
	{
		using namespace ImageHandle;

		const auto count{get_count()};

		gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 0.3, 0.0, body[static_cast<int>(d->index)]           , true);
		gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 0.3, 0.0, fan [static_cast<int>(d->index)][count % 3], true);
	}

	void Helli::preparation(void)
	{
		using namespace ImageHandle;
		using namespace std::literals;
		using namespace boost;

		body[RIGHT] = add_get("../../data/img/helli/body_right.png");
		body[LEFT] = add_get("../../data/img/helli/body_left.png");
		body[FRONT] = add_get("../../data/img/helli/body_front.png");

		for (int i = 0; i != fan.size(); ++i)
		{
			fan[RIGHT][i] = add_get((format("../../data/img/helli/fan%d_right.png") %i).str());
			fan[LEFT ][i] = add_get((format("../../data/img/helli/fan%d_left.png" ) %i).str());
			fan[FRONT][i] = add_get((format("../../data/img/helli/fan%d_front.png") %i).str());
		}
	}
}
