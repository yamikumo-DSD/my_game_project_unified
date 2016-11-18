//small_battle_ship.cpp

#include "small_battle_ship.h"
#include "header_set_for_mob_enemy.h"
#include "enemy_order.h"
#include "debug.h"
#include "add_get.h"
#include "color.h"
#include "border_check.h"
#include "player.h"
#include "bullet_behavior.h"
#include "se_manager.h"

namespace MyGameProject
{
	static constexpr int INITIAL_HEALTH{2000};
	using P = Point2D;

	namespace ImageHandle
	{
		static std::array<int, 3> flag;
		static int sasabune{0};
		static int trail{0};
		enum {FRONT = 0, BACK = 1, RIGHT = 2, LEFT = 3,};
		static std::array<int, 4> fairy;
	}

	class Crew : public MobEnemy
	{
	private:
		std::weak_ptr<SmallBattleShip> master;
		Real dir{0};
		const Point2D relative_pos;
	public:
		static constexpr auto INITIAL_HEALTH{300};
		Crew
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			const Player& _player,
			decltype(master) _master,
			Point2D _relative_pos
		) noexcept
			:MobEnemy
			(
				_bullets, _se_manager,
				EnemyOrder::make_order
				(
					"Crew",
					0,
					_master.lock()->pos() + _relative_pos,
					0,
					[](MobEnemy& _this)
					{
						const auto count = _this.get_count();
						if (count == 0)
						{
						}
						else
						{
						}
					} //act pattern def
				), //EnemyOrder::make_order
				INITIAL_HEALTH, Class::SMALL, _player, ShapeElement::Circle(15)
			),
			relative_pos(_relative_pos),
			master(_master)
		{}
		virtual void accessory_custom_updater(void) override final
		{
			if (!master.expired())
			{
				auto master_{master.lock()};
				if (!master_->get_flag()) { set_flag_off(); }
				else
				{
					dir = angle_of(player_ref().pos() - pos());
					dir = std::fmod(dir, two_pi<Real>());
					if (dir < 0) { dir += two_pi<Real>(); }
					pos() = master_->pos() + relative_pos;
				}

				const auto count{get_count()};
				if (count > 0)
				{
					if (count % 70 == 0 && is_within_window(pos()) && norm(player_ref().pos() - pos()) > 100)
					{
						*find_vacant_object(SharedObject::bullets()) = Bullet::create("B0", *this, player_ref(), pos(), dir, straight_course(6));
						get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot_01.wav");
					}
				}
			}
		}
		virtual void draw(void) const override final
		{
			const auto count{get_count()};
			if (count > 0)
			{
				using namespace ImageHandle;
				static constexpr auto pi_4{pi<Real>() / 4};
				if ((dir >= 0 && dir < pi_4) || (dir >= 7 * pi_4 && dir < 8 * pi_4))
				{
					gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 2.0, 0.0, fairy[RIGHT], true);
				}
				else if (dir >= pi_4 && dir < 3 * pi_4)
				{
					gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 2.0, 0.0, fairy[FRONT], true);
				}
				else if (dir >= 3 * pi_4 && dir < 5 * pi_4)
				{
					gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 2.0, 0.0, fairy[LEFT], true);
				}
				else
				{
					gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 2.0, 0.0, fairy[BACK], true);
				}
			}
		}
		virtual ~Crew(void) noexcept = default;
	};

	struct TrailParticle
	{
		Point2D pos;
		int pal{0};
		int count{0};
		double scale{1.2};
		void draw(void) const noexcept 
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraphF(gp::level(12), pos.x(), pos.y(), scale, 0.0, ImageHandle::trail, true),
				DX_BLENDMODE_ADD,
				pal
			);
		}
	};

	struct SmallBattleShip::Impl 
	{
		static constexpr auto TRAIL_PARTICLE_NUM{10};
		std::array<TrailParticle, 10> trail_particles;
		std::array<gp::smart_ptr<Crew>, 3> crew;
	};

	SmallBattleShip::SmallBattleShip
	(
		BulletPtrContainer& _bullets,
		SEManager& _se_manager,
		gp::smart_ptr<EnemyOrder> _order,
		const Player& _player,
		std::vector<gp::smart_ptr<Enemy>>& _enemies
	) noexcept
		:MobEnemy(_bullets, _se_manager, _order, INITIAL_HEALTH, Class::MIDDLE, _player, ShapeElement::Null(), _enemies),
		pimpl(std::make_unique<Impl>())
	{
	}

	SmallBattleShip::SmallBattleShip(const SmallBattleShip& _small_battle_ship) noexcept
		:MobEnemy(_small_battle_ship),
		pimpl(std::make_unique<Impl>(*_small_battle_ship.pimpl))
	{
	}

	SmallBattleShip::SmallBattleShip(SmallBattleShip&& _small_battle_ship) noexcept = default;

	SmallBattleShip::~SmallBattleShip(void) noexcept = default;
	
	void SmallBattleShip::accessory_custom_updater(void) 
	{
		auto&& count{get_count()};

		//update trail particles.
		if (count == 0)
		{
			for (int i = 0; i != Impl::TRAIL_PARTICLE_NUM; ++i)
			{
				pimpl->trail_particles[i].count = i;
			}

			//Register crew.
			for (int i = -1; i != 2; ++i)
			{
				auto temp
				{
					gp::make_smart<Crew>
					(
						get_bullets_ref(),
						get_se_manager_ref(),
						player_ref(),
						shared_from_this(),
						Point2D(0, 45 * i)
					)
				};
				*find_vacant_object(SharedObject::enemies()) = temp;
				pimpl->crew[i + 1] = temp;
				*find_vacant_object(SharedObject::mob_enemies()) = std::move(temp);
			}
		}
		for (int i = 0; i != Impl::TRAIL_PARTICLE_NUM; ++i)
		{
			if (pimpl->trail_particles[i].count % 10 == 0)
			{
				pimpl->trail_particles[i].pos = pos();
				pimpl->trail_particles[i].pal = 50;
				pimpl->trail_particles[i].scale = 1.2;
			}
			++pimpl->trail_particles[i].count;
			pimpl->trail_particles[i].pal -= 4;
			pimpl->trail_particles[i].pos += Point2D(0, 20);
			pimpl->trail_particles[i].scale += 0.1;
		}
		if (count > 0)
		{
			static const auto crew_num
			{
				[](auto&& _crew)
				{
					std::size_t num{0};
					for (auto&& c : _crew)
					{
						if (c && c->get_flag()) { ++num; }
					}
					return num;
				}
			};
			if (crew_num(pimpl->crew) == 0 || (is_within_window(pos(), 300) && count > 1000)) { set_flag_off(); }
		}
	}

	void SmallBattleShip::draw(void) const
	{
		auto&& count{get_count()};
		if (count > 0)
		{
			//Draw trail.
			for (auto&& trail_particle : pimpl->trail_particles) { trail_particle.draw(); }

			gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 0.5, 0.0, ImageHandle::sasabune, true);
			auto index{0};
			auto i{count % 20};
			if (i < 5) { index = 0; }
			else if (i >= 5 && i < 10) { index = 1; }
			else if (i >= 10 && i < 15) { index = 2; }
			else { index = 1; }

			gp::DrawRotaGraphF (gp::level(12), pos().x() + 7, pos().y() - 118, 2.0, 0.0, ImageHandle::flag[index], true );
		}
	}

	void SmallBattleShip::preparation(void)
	{
		ImageHandle::flag[0]  = add_get("../../data/img/sasabune/flag0.png"   );
		ImageHandle::flag[1]  = add_get("../../data/img/sasabune/flag1.png"   );
		ImageHandle::flag[2]  = add_get("../../data/img/sasabune/flag2.png"   );
		ImageHandle::sasabune = add_get("../../data/img/sasabune/sasabune.png");
		ImageHandle::trail    = add_get("../../data/img/sasabune/trail.png"   );
		ImageHandle::fairy[ImageHandle::FRONT] = add_get("../../data/img/sasabune/fairy0.png"  );
		ImageHandle::fairy[ImageHandle::RIGHT] = add_get("../../data/img/sasabune/fairy1.png"  );
		ImageHandle::fairy[ImageHandle::BACK ] = add_get("../../data/img/sasabune/fairy2.png"  );
		ImageHandle::fairy[ImageHandle::LEFT ] = add_get("../../data/img/sasabune/fairy3.png"  );
	}
}
