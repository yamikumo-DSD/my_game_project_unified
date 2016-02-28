//master_and_slave.cpp

#include "master_and_slaves.h"
#include "enemy_order.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"
#include "find_vacant_object.h"
#include "se_manager.h"
#include "player.h"
#include "bullet.h"
#include "bullet_behavior.h"

static constexpr int MAX_HP_MASTER = 300;
static constexpr int MAS_HP_SLAVE = 100;

/////////////        Slave        ////////////////

namespace MyGameProject
{
	struct SlaveMemberVars
	{
		Real distance;
		Real angle;
		SlaveMemberVars(Real _distance, Real _angle) :distance(_distance), angle(_angle) {}
		SlaveMemberVars(void) :distance(0.f), angle(0.f) {}
	};
}

MyGameProject::Master::Slave::Slave
(
	BulletPtrContainer& _bullets,
	SEManager& _se_manager,
	const Player& _player,
	gp::smart_ptr<Master> _master,
	decltype(vars) _vars
)
	:MobEnemy
	(
		_bullets,
		_se_manager,
		EnemyOrder::make_order
		(
			"Slave",
			0,
			_master->pos() ,
			0,
			[&, _master, _vars](MobEnemy& _this)
			{
				const auto count = _this.get_count();
				if (count == 0)
				{
					vars->angle = _vars->angle;
					vars->distance = _vars->distance;
					if (_master)
					{
						_this.pos() = _master->pos();
					}
				}
				else
				{
					if (_master)
					{
						_this.pos() = _master->pos() + Point2D(vars->distance * std::cos(vars->angle), vars->distance * std::sin(vars->angle));
						vars->angle += 0.06f;
						if(vars->distance < 150) {vars->distance += 0.5f;}
					}
				}
			}
		),
		MAX_HP_MASTER,
		Class::SMALL,
		_player,
		ShapeElement::Circle(20)
	),
	master(_master),
	vars(gp::make_smart<SlaveMemberVars>())
{}

void MyGameProject::Master::Slave::accessory_custom_updater(void)
{
	if (master && !master->get_flag()) { set_flag_off(); }
}

void MyGameProject::Master::Slave::draw(void) const
{
	gp::DrawCircle(static_cast<Real>(pos().x()), static_cast<Real>(pos().y()), 15, gp::black.get());
	if (master) {gp::DrawLine(pos().x(), pos().y(), master->pos().x(), master->pos().y(), gp::black.get()); }
}

void MyGameProject::Master::Slave::preperation(void)
{
}

MyGameProject::Master::Slave::~Slave(void) {}

//////////////////////////////////////////////////


/////////////      Master         ////////////////

MyGameProject::Master::Master
(
	BulletPtrContainer& _bullets,
	SEManager& _se_manager,
	gp::smart_ptr<EnemyOrder> _order,
	const Player& _player,
	std::vector<gp::smart_ptr<Enemy>>& _enemies
)
	:MobEnemy(_bullets, _se_manager, _order, MAX_HP_MASTER, Class::MIDDLE, _player, ShapeElement::Circle(20), _enemies)
{}

void MyGameProject::Master::accessory_custom_updater(void)
{
	if (get_count() == 0)
	{
		if (enemies)
		{
			static constexpr int SLAVE_NUM = 6;
			for (int i = 0; i != SLAVE_NUM; ++i)
			{
				*find_vacant_object(*enemies) =
				gp::make_smart<Slave>
				(
					get_bullets_ref(),
					get_se_manager_ref(),
					player_ref(),
					shared_from_this(),
					gp::make_smart<SlaveMemberVars>(0, i * boost::math::constants::two_pi<Real>() / SLAVE_NUM)
				);
			}
		}
	}

	const auto x = pos().x(), y = pos().y();
	if (x < -100 || x > 640 + 100 || y < -100 || y > 480 + 100)
	{
		set_flag_off();
	}
}

void MyGameProject::Master::draw(void) const
{
	gp::DrawCircle(pos().x(), pos().y(), 20, gp::black.get());
}

void MyGameProject::Master::preperation(void)
{
	Slave::preperation();
}

MyGameProject::Master::~Master(void) {}

/////////////////////////////////////////////////