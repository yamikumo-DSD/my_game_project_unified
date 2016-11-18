//amenbo.cpp

#include "amenbo.h"
#include "header_set_for_mob_enemy.h"
#include "debug.h"
#include "add_get.h"
#include "color.h"
#include "border_check.h"

namespace MyGameProject
{
	static constexpr int INITIAL_HEALTH{100};
	using P = Point2D;

	namespace ImageHandle
	{
		static int body{0};
		static int foot{0};
		static int joint{0};
		static int trail{0};
	}

	struct Amenbo::Impl 
	{
		enum { LEFT_TOP = 0, RIGHT_TOP = 1, LEFT_BOTTOM = 2, RIGHT_BOTTOM = 3, };
		std::array<Real, 4> proximal_leg_seg_angle, distal_leg_seg_angle;
		const static std::array<P, 4> LEG_ROOT_POS_RELATIVE;
		static constexpr auto PROXIMAL_LEG_SEG_LENGTH{30};
		static constexpr auto DISTAL_LEG_SEG_LENGTH{30};
		static constexpr auto pi_4{pi<Real>() / 4};
		static constexpr auto pi_2{pi<Real>() / 2};
		static constexpr Real PROXIMAL_BASE_ANGLE[4]{5 * pi_4, 7 * pi_4, 3 * pi_4, 1 * pi_4};
		static constexpr Real DISTAL_BASE_ANGLE[4]{3 * pi_2, 3 * pi_2, pi_2, pi_2};
	};

	const std::array<P, 4> Amenbo::Impl::LEG_ROOT_POS_RELATIVE{P(-3, -3), P(+3, -3), P(-3, +3), P(+3, +3)};

	Amenbo::Amenbo
	(
		BulletPtrContainer& _bullets,
		SEManager& _se_manager,
		gp::smart_ptr<EnemyOrder> _order,
		const Player& _player
	) noexcept
		:MobEnemy(_bullets, _se_manager, _order, INITIAL_HEALTH, Class::SMALL, _player, ShapeElement::Circle(15)),
		pimpl(std::make_unique<Impl>())
	{
		for (int i = 0; i != 4; ++i) { pimpl->proximal_leg_seg_angle[i] = Impl::PROXIMAL_BASE_ANGLE[i]; }
		for (int i = 0; i != 4; ++i) { pimpl->distal_leg_seg_angle[i] = Impl::DISTAL_BASE_ANGLE[i]; }
	}

	Amenbo::Amenbo(const Amenbo& _amenbo) noexcept
		:MobEnemy(_amenbo),
		pimpl(std::make_unique<Impl>(*_amenbo.pimpl))
	{
		for (int i = 0; i != 4; ++i) { pimpl->proximal_leg_seg_angle[i] = Impl::PROXIMAL_BASE_ANGLE[i]; }
		for (int i = 0; i != 4; ++i) { pimpl->distal_leg_seg_angle[i] = Impl::DISTAL_BASE_ANGLE[i]; }
	}

	Amenbo::Amenbo(Amenbo&& _amenbo) noexcept = default;

	Amenbo::~Amenbo(void) noexcept = default;
	
	void Amenbo::accessory_custom_updater(void) 
	{
		auto&& count{get_count()};

		for (int i = 0; i != 4; ++i)
		{
			auto&& p_angle{pimpl->proximal_leg_seg_angle[i]};
			auto&& d_angle{pimpl->distal_leg_seg_angle[i]};

			const auto v{calc_velocity()};

			if (norm(v) < 1)
			{
				if (p_angle > Impl::PROXIMAL_BASE_ANGLE[i]) { p_angle -= 0.01f; }
				else { p_angle += 0.01f; }

				if (d_angle > Impl::DISTAL_BASE_ANGLE[i]) { d_angle -= 0.01f; }
				else { d_angle += 0.01f; }
			}
			else
			{
				if (v.x() * sin(p_angle) - v.y() * cos(p_angle) > 0)
				{
					if (p_angle < Impl::PROXIMAL_BASE_ANGLE[i] + pi<Real>() / 8) { p_angle += 0.02f; }
				}
				else
				{
					if (p_angle > Impl::PROXIMAL_BASE_ANGLE[i] - pi<Real>() / 8) { p_angle -= 0.02f; }
				}

				if (v.x() * sin(d_angle) - v.y() * cos(d_angle) > 0)
				{
					if (d_angle < Impl::DISTAL_BASE_ANGLE[i] + pi<Real>() / 12) { d_angle += 0.02f; }
				}
				else
				{
					if (d_angle > Impl::DISTAL_BASE_ANGLE[i] - pi<Real>() / 12) { d_angle -= 0.02f; }
				}
			}
		}

		if (count > 300 && !is_within_window(pos(), 100)) { set_flag_off(); }
	}

	void Amenbo::draw(void) const
	{
		auto&& count{get_count()};
		if (count > 0)
		{
			using namespace ImageHandle;

			for (int i = 0; i != 4; ++i)
			{
				const auto leg_root_pos{ pos() + Impl::LEG_ROOT_POS_RELATIVE[i] };
				const auto joint_pos
				{
					leg_root_pos +
					Impl::PROXIMAL_LEG_SEG_LENGTH * P(cos(pimpl->proximal_leg_seg_angle[i]), sin(pimpl->proximal_leg_seg_angle[i]))
				};
				const auto foot_pos
				{
					joint_pos +
					Impl::DISTAL_LEG_SEG_LENGTH * P(cos(pimpl->distal_leg_seg_angle[i]), sin(pimpl->distal_leg_seg_angle[i]))
				};

				for (int j = 0; j != 10; ++j)
				{
					const auto p_y{ 2 * ((count + 3 * j) % 30) };
					gp::SetDrawBlendModeOf
					(
						gp::DrawRotaGraphF(gp::level(12), foot_pos.x(), foot_pos.y() + p_y, 1.5, 0.0, trail, true),
						DX_BLENDMODE_ADD,
						150 - (150 / 60.f) * p_y
					);
				}

				static constexpr auto BASE_COLOR{gp::Color(50, 50, 50)};
				static constexpr auto TOP_COLOR{gp::Color(100, 100, 100)};

				gp::DrawLine(gp::level(12), leg_root_pos.x(), leg_root_pos.y(), joint_pos.x(), joint_pos.y(), BASE_COLOR, 4);
				gp::DrawLine(gp::level(12), joint_pos.x(), joint_pos.y(), foot_pos.x(), foot_pos.y(), BASE_COLOR, 4);
				gp::DrawLine(gp::level(12), leg_root_pos.x(), leg_root_pos.y(), joint_pos.x(), joint_pos.y(), TOP_COLOR, 2);
				gp::DrawLine(gp::level(12), joint_pos.x(), joint_pos.y(), foot_pos.x(), foot_pos.y(), TOP_COLOR, 2);
				gp::DrawRotaGraphF(gp::level(12), joint_pos.x(), joint_pos.y(), 1.0, 0.0, joint, true);
				gp::DrawRotaGraphF(gp::level(12), foot_pos.x(), foot_pos.y(), 1.0, pimpl->distal_leg_seg_angle[i] + half_pi<Real>(), foot, true);
			}
			gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 1.0, 0.0, body, true);
		}
	}

	void Amenbo::preparation(void)
	{
		using namespace ImageHandle;

		body = add_get("../../data/img/amenbo/body.png");
		foot = add_get("../../data/img/amenbo/foot.png");
		joint = add_get("../../data/img/amenbo/joint.png");
		trail = add_get("../../data/img/amenbo/trail.png");
	}
}
