//gogyo.cpp

#include "gogyo.h"
#include "decl_static_image_handler.h"
#include "image_pool.h"
#include "draw_order.h"

namespace MyGameProject
{
	constexpr auto MAX_HEALTH{ 40 };

	struct Gogyo::Impl
	{
		STATIC_IMAGE_HANDLER(img)
		static constexpr Real OMEGA{static_cast<Real>(0.1)};
		Real angle{0};
		Real scale{static_cast<Real>(1.3)};
	};

	Gogyo::Gogyo
	(
		BulletPtrContainer& _bullets,
		SEManager& _se_manager,
		gp::smart_ptr<EnemyOrder> _order,
		const Player& _player
	) noexcept
		:MobEnemy(_bullets, _se_manager, _order, MAX_HEALTH, Class::SMALL, _player, ShapeElement::Circle(30)),
		pimpl(std::make_unique<Impl>())
	{}

	Gogyo::~Gogyo(void) noexcept = default;

	void Gogyo::accessory_custom_updater(void)
	{
		const auto count{get_count()};
		const auto x{pos().x()};
		const auto y{pos().y()};
		pimpl->angle += Impl::OMEGA;
		if (count > 200 && (x < -100 || x > 640 + 100 || y < -100 || y > 480 + 100))
		{
			set_flag_off();
		}
	}

	void Gogyo::draw(void) const
	{
		gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 1.0, pimpl->angle, Impl::img(), true);
	}

	void Gogyo::preperation(void)
	{
		ImagePool::add("../../data/img/gogyo.png");
		Impl::img() = ImagePool::get("../../data/img/gogyo.png");
	}
}