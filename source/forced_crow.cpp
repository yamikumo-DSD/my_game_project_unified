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

namespace MyGameProject
{
	constexpr int INITIAL_HEALTH{40};

	class Force : public Bullet
	{
	private:
		const int master_id;
		virtual void custom_updater(void) override final
		{
			if (!is_id_enable(SharedObject::enemies(), master_id))
			{
				set_flag_off();
			}
		}
	public:
		Force(const Enemy& _master, const Player& _player) 
			:Bullet(_master, _player, _master.pos(), 0, ShapeElement::Circle(20), [](Bullet& _bullet) {_bullet.pos() = _bullet.get_master_ref().pos(); }),
			master_id(_master.get_id())
		{
		}
		virtual void draw(void) const override final {/* Do notiong */}
		virtual void hit(void) override final {/* Do notiong  */}
		virtual bool is_resistant(void) const { return true; }
		~Force(void) {}
	};

	struct ForcedCrow::Impl
	{
		STATIC_IMAGE_HANDLER(img)
		STATIC_IMAGE_HANDLER(trail_img)
		static constexpr std::size_t TRAIL_NUM{5};
		std::array<std::pair<Point2D, Real>, TRAIL_NUM> trails;
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
			*find_vacant_object(SharedObject::bullets()) = std::make_shared<Force>(*this, player_ref());

			for (auto& p : pimpl->trails) { p.first = pos(); p.second = angle_of(velocity());}
		}
		else
		{
			auto& trails{pimpl->trails};
			for (int i = trails.size() - 1; i != 0; --i) { trails[i] = trails[i - 1]; ; }
			trails[0].first = pos(); trails[0].second = angle_of(velocity());
		}
	}

	void ForcedCrow::draw(void) const
	{
		const auto dir{angle_of(velocity())};
		for (const auto& p : pimpl->trails)
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraphF(gp::level(10), p.first.x(), p.first.y(), 1.0, boost::math::constants::half_pi<Real>() + p.second, Impl::trail_img(), true),
				DX_BLENDMODE_ALPHA,
				100
			);
		}
		gp::DrawRotaGraphF(gp::level(10), pos().x(), pos().y(), 1.0, boost::math::constants::half_pi<Real>() + dir, Impl::img(), true);
	}

	void ForcedCrow::preparation(void)
	{
		ImagePool::add("../../data/img/crow1.png");
		Impl::img() = ImagePool::get("../../data/img/crow1.png");
		ImagePool::add("../../data/img/crow_trail.png");
		Impl::trail_img() = ImagePool::get("../../data/img/crow_trail.png");
	}
}