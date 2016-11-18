//mob_enemy.cpp

#include "mob_enemy.h"
#include "enemy_order.h"
#include "concrete_enemy.h"
#include <exception>
#include "se_manager.h"
#include "bullet.h"
#include "shared_object.h"
#include "item.h"
#include "ranged_uniform_random.h"
#include "mathematics.h"
#include "find_vacant_object.h"
#include "draw_order.h"
#include "color.h"
#include "diffuse_point.h"
#include "default_killed_effect.h"
#include "debug.h"

namespace MyGameProject
{
	constexpr auto DURATION = 40;
	using namespace boost::math::constants;

	struct MobEnemy::Impl
	{
		int hp;
		ActPattern act_pattern;
		BulletPtrContainer& bullets;
		SEManager& se_manager;
		gp::smart_ptr<EnemyOrder> order;
		Point2D pre_pos;
		Point2D v; //Velocity
		int count_after_killed{0};
		Point2D killed_pos;
		std::unique_ptr<KilledEffect> killed_effect;

		struct Particle
		{
			bool flag{false};
			Real radius{0};
			int pal{0};
			Point2D pos;
			int count{ 0 };
		};
		std::array<Particle, 10> particles;

		template<typename HP, typename ActPattern, typename EnemyOrderPtr>
		Impl(HP&& _hp, ActPattern&& _act_pattern, BulletPtrContainer& _bullets, SEManager& _se_manager, EnemyOrderPtr&& _enemy_order)
			:hp(std::forward<HP>(_hp)),
			act_pattern(std::forward<ActPattern>(_act_pattern)),
			bullets(_bullets),
			se_manager(_se_manager),
			order(std::forward<EnemyOrderPtr>(_enemy_order)),
			v(),
			pre_pos(),
			killed_pos(),
			particles(),
			killed_effect()
		{}
		Impl(const Impl& _impl) noexcept
			:hp(_impl.hp), act_pattern(_impl.act_pattern), 
			bullets(_impl.bullets), se_manager(_impl.se_manager),
			order(_impl.order), v(_impl.v), pre_pos(_impl.pre_pos),
			count_after_killed(_impl.count_after_killed), killed_pos(_impl.killed_pos), 
			particles(_impl.particles),
			killed_effect()
		{ 
			*killed_effect = *_impl.killed_effect;
		}
		Impl& operator=(const Impl& _impl) noexcept
		{
			hp = _impl.hp;
			act_pattern = _impl.act_pattern; 
			bullets = _impl.bullets; 
			se_manager = _impl.se_manager;
			order = _impl.order; 
			v = _impl.v; 
			pre_pos = _impl.pre_pos;
			count_after_killed = _impl.count_after_killed;
			killed_pos = _impl.killed_pos; 
			particles = _impl.particles;
			*killed_effect = *_impl.killed_effect;
		}
	};

	MobEnemy::MobEnemy
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			int _initial_health, Class _enemy_class,
			const Player& _player,
			const Shape& _hitbox_shape
			)
		: Enemy(_order->initial_pos, _order->initial_angle, _enemy_class, _player, _hitbox_shape),
		pimpl(std::make_unique<Impl>(_initial_health, _order->act_pattern, _bullets, _se_manager, _order))
	{
	}

	MobEnemy::MobEnemy
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			int _initial_health, Class _enemy_class,
			const Player& _player,
			const Shape& _hitbox_shape,
			std::vector<gp::smart_ptr<Enemy>>& _enemies
			)
		: Enemy(_order->initial_pos, _order->initial_angle, _enemy_class, _player, _hitbox_shape, _enemies),
		pimpl(std::make_unique<Impl>(_initial_health, _order->act_pattern, _bullets, _se_manager, _order))
	{
	}

	MobEnemy::MobEnemy(const MobEnemy& _mob_enemy) noexcept
		: pimpl(std::make_unique<Impl>(*_mob_enemy.pimpl)), Enemy(_mob_enemy)
	{}

	MobEnemy::MobEnemy(MobEnemy&& _mob_enemy) noexcept = default;

	MobEnemy::~MobEnemy(void) = default;

	void MobEnemy::custom_updater(void)
	{
		if (pimpl->hp <= 0)
		{
			update_after_killed();
		}
		accessory_custom_updater();
		pimpl->act_pattern(*this);
		pimpl->pre_pos = pos();
		pimpl->v = pos() - pimpl->pre_pos;
	}

	const Point2D& MobEnemy::velocity(void) const { return pimpl->v; }

	Point2D& MobEnemy::velocity(void) { return pimpl->v; }

	Point2D MobEnemy::calc_velocity(void) const { return pos() - pimpl->pre_pos; }

	int MobEnemy::released_p_item_num(void) const { return 6; }

	void MobEnemy::hit(int _shot_power) { damage(_shot_power); }

	void MobEnemy::damage(int _shot_power) { pimpl->hp -= _shot_power; }

	MobEnemy::BulletPtrContainer& MobEnemy::get_bullets_ref(void) { return pimpl->bullets; }

	SEManager& MobEnemy::get_se_manager_ref(void) { return pimpl->se_manager; }

	void MobEnemy::preperation(void)
	{
		Gogyo::preperation();
		Crow::preperation();
		Crow2::preperation();
		Crow3::preparation();
		Master::preperation();
		EmergeFromCloud::preperation();
		Master2::preperation();
		ForcedCrow::preparation();
		RedEye::preparation();
		Fairy_1::preparation();
		Fairy_2::preparation();
		Fairy_3::preparation();
		KilledEffect::preparation();
		Amenbo::preparation();
		SmallBattleShip::preparation();
		LaserLauncher::preparation();
		Dragonfly::preparation();
		Bomber::preparation();
		Helli::preparation();
		RedMaster::preperation();
		BlackSpirit::preperation();
		GreenMaster::preperation();
	}


	int MobEnemy::get_health(void) const { return pimpl->hp; }

	//default behavior
	void MobEnemy::update_after_killed(void)
	{
		auto& count_after_killed{pimpl->count_after_killed};
		if (count_after_killed == DURATION) { set_flag_off(); }
		else if (count_after_killed == 1)
		{
			//release point items
			for (int i = 0; i != released_p_item_num(); ++i)
			{
				*find_vacant_object(SharedObject::items()) = std::make_shared<Point>(player_ref(), pos(), gp::safe_rand<Real>(0, two_pi<Real>()));
			}
			//keep position where *this has been destroyed.
			pimpl->killed_pos = pos();
			//play SE
			get_se_manager_ref().erect_play_flag_of("../../data/sound/shot_down.wav");
		}

		if (pimpl->killed_effect && pimpl->killed_effect->get_flag())
		{
			pimpl->killed_effect->update();
		}
		if (count_after_killed == 1)
		{
			pimpl->killed_effect = make_default_killed_effect(get_class(), pos());
		}

		++count_after_killed;
	}

	void MobEnemy::draw_killed_effect(void) const
	{
		if (pimpl->killed_effect && pimpl->killed_effect->get_flag())
		{
			pimpl->killed_effect->draw();
		}
	}

	bool MobEnemy::is_just_killed_now(void) const noexcept
	{
		return pimpl->count_after_killed == 1;
	}

	void MobEnemy::kill(void) noexcept
	{
		state(State::KILLED_BUT_AVAILABLE);
	}
} //namespace MyGameProject