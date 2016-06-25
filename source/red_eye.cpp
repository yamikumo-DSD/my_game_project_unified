//red_eye.cpp

#include "red_eye.h"
#include "header_set_for_mob_enemy.h"
#include "debug.h"

namespace MyGameProject
{
	static constexpr int INITIAL_HEALTH{100};
	enum ImgChannel {CLOSE = 0, INTERMEDIATE, OPEN };

	struct RedEye::Impl 
	{
		STATIC_IMAGE_HANDLER_LIST(img, 3)
		Real rot{0};
		ImgChannel img_channel{CLOSE};
	};

	RedEye::RedEye
	(
		BulletPtrContainer& _bullets,
		SEManager& _se_manager,
		gp::smart_ptr<EnemyOrder> _order,
		const Player& _player
	) noexcept
		:MobEnemy(_bullets, _se_manager, _order, INITIAL_HEALTH, Class::SMALL, _player, ShapeElement::Circle(15)),
		pimpl(std::make_unique<Impl>())
	{}

	RedEye::RedEye(const RedEye& _red_eye) noexcept
		:MobEnemy(_red_eye),
		pimpl(std::make_unique<Impl>(*_red_eye.pimpl))
	{}

	RedEye::RedEye(RedEye&& _red_eye) noexcept = default;

	RedEye::~RedEye(void) noexcept = default;

	
	void RedEye::accessory_custom_updater(void) 
	{
		const auto count{get_count()};
		const auto drot{static_cast<Real>(0.2 * std::sin(count / 100.0))};
		pimpl->rot += drot;
		//dealing image channel
		static constexpr auto CYCLE_TERM{200};
		const auto cycle_count{count % CYCLE_TERM};
		if (cycle_count >= 20 && cycle_count < 30) { pimpl->img_channel = INTERMEDIATE; }
		else if (cycle_count >= 30 && cycle_count < 90) { pimpl->img_channel = OPEN; }
		else if (cycle_count >= 90 && cycle_count < 100) { pimpl->img_channel = INTERMEDIATE; }
		else { pimpl->img_channel = CLOSE; }
	}

	void RedEye::draw(void) const
	{
		gp::DrawRotaGraphF
		(
			gp::level(11), 
			pos().x(), pos().y(), 1.0, pimpl->rot,
			Impl::img()[pimpl->img_channel], true
		);
	}

	void RedEye::preparation(void)
	{
		ImagePool::add("../../data/img/bit01.png");
		Impl::img()[OPEN] = ImagePool::get("../../data/img/bit01.png");
		ImagePool::add("../../data/img/bit02.png");
		Impl::img()[INTERMEDIATE] = ImagePool::get("../../data/img/bit02.png");
		ImagePool::add("../../data/img/bit03.png");
		Impl::img()[CLOSE] = ImagePool::get("../../data/img/bit03.png");
	}
}