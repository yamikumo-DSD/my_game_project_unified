//forced_crow.cpp

#include "forced_crow.h"
#include "shared_object.h"
#include "bullet.h"
#include "is_id_enable.h"
#include "find_vacant_object.h"
#include "image_pool.h"
#include "decl_static_image_handler.h"
#include "draw_order.h"
#include "mathematics.h"
#include <dxlib.h>
#include "border_check.h"
#include "debug_value.h"

namespace MyGameProject
{
	constexpr int INITIAL_HEALTH{40};

	struct ForcedCrow::Impl
	{
		STATIC_IMAGE_HANDLER(img)
		STATIC_IMAGE_HANDLER(trail_img)
		static constexpr std::size_t TRAIL_NUM{5};
		struct Trail
		{
			Point2D pos;
			Real angle; 
		};
		mutable std::array<Trail, TRAIL_NUM> trails;
	};

	ForcedCrow::ForcedCrow
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

	ForcedCrow::~ForcedCrow(void) noexcept = default;

	void ForcedCrow::accessory_custom_updater(void)
	{
		const auto count{get_count()};
		if (count == 0)
		{
			for (auto& p : pimpl->trails) { p.pos = pos(); p.angle = angle_of(calc_velocity());}
		}

		if (!is_within_window(pos(), 50) && count > 200) { set_flag_off(); }
	}

	void ForcedCrow::draw(void) const
	{
		const auto dir{angle_of(calc_velocity())};
		for (const auto& p : pimpl->trails)
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraphF(gp::level(10), p.pos.x(), p.pos.y(), 1.0, boost::math::constants::half_pi<Real>() + p.angle, Impl::trail_img(), true),
				DX_BLENDMODE_ALPHA,
				100
			);
		}
		gp::DrawRotaGraphF(gp::level(10), pos().x(), pos().y(), 1.0, boost::math::constants::half_pi<Real>() + dir, Impl::img(), true);

		auto& trails{pimpl->trails};
		for (int i = trails.size() - 1; i != 0; --i) { trails[i] = trails[i - 1]; ; }
		trails[0].pos = pos(); trails[0].angle = angle_of(calc_velocity());
	}

	void ForcedCrow::preparation(void)
	{
		ImagePool::add("../../data/img/crow1.png");
		Impl::img() = ImagePool::get("../../data/img/crow1.png");
		ImagePool::add("../../data/img/crow_trail.png");
		Impl::trail_img() = ImagePool::get("../../data/img/crow_trail.png");
	}
}