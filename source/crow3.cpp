//crow3.cpp

#include "crow3.h"
#include "image_pool.h"
#include "decl_static_image_handler.h"
#include "draw_order.h"
#include "mathematics.h"
#include "ranged_uniform_random.h"
#include "se_manager.h"
#include "add_get.h"

namespace MyGameProject
{
	constexpr int INITIAL_HEALTH{40};

	struct Crow3::Impl
	{
		STATIC_IMAGE_HANDLER_LIST(img_left, 4)
		STATIC_IMAGE_HANDLER_LIST(img_right, 4)
	};

	Crow3::Crow3
	(
		BulletPtrContainer& _bullets,
		SEManager& _se_manager,
		gp::smart_ptr<EnemyOrder> _order,
		const Player& _player
	) noexcept
		:MobEnemy(_bullets, _se_manager, _order, INITIAL_HEALTH, Class::SMALL, _player, ShapeElement::Circle(20)),
		pimpl(std::make_unique<Impl>())
	{
	}

	Crow3::~Crow3(void) noexcept = default;

	void Crow3::accessory_custom_updater(void)
	{
		const auto count{get_count()};
		if (count == 0)
		{
		}
		else
		{
			const auto x{ pos().x() }, y{ pos().y() };
			if (count > 300 && (x < -100 || x > 640 + 100 || y < -100 || y > 480 + 100))
			{
				set_flag_off();
			}
		}
		if (count == gp::safe_rand<int>(1, 200))
		{
			get_se_manager_ref().erect_play_flag_of("../../data/sound/crow.wav");
		}

	}

	void Crow3::draw(void) const
	{
		const auto count{ get_count() };
		if (count > 0)
		{
			if (calc_velocity().x() > 0)
			{
				gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 1.3, 0.0, Impl::img_right()[(count / 8) % 4], true);
			}
			else
			{
				gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 1.3, 0.0, Impl::img_left()[(count / 8) % 4], true);
			}
		}
	}

	void Crow3::preparation(void)
	{
		auto&& img_left{Impl::img_left()};
		auto&& img_right{Impl::img_right()};

		img_left[0] = add_get("../../data/img/crow2_00.png");
		img_left[1] = add_get("../../data/img/crow2_01.png");
		img_left[2] = add_get("../../data/img/crow2_02.png");
		img_left[3] = add_get("../../data/img/crow2_01.png");

		img_right[0] = add_get("../../data/img/crow2_00_.png");
		img_right[1] = add_get("../../data/img/crow2_01_.png");
		img_right[2] = add_get("../../data/img/crow2_02_.png");
		img_right[3] = add_get("../../data/img/crow2_01_.png");
	}
}