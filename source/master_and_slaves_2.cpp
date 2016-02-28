//master_and_slave_2.cpp

#include "master_and_slaves_2.h"
#include "enemy_order.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"
#include "find_vacant_object.h"
#include "se_manager.h"
#include "player.h"
#include "bullet.h"
#include "bullet_behavior.h"
#include "drawblendmode.h"
#include "math_constants.h"

static constexpr int MAX_HP_MASTER = 300;
static constexpr int MAS_HP_SLAVE = 100;

/////////////        Slave2        ////////////////

namespace MyGameProject
{
	struct Slave2MemberVars
	{
		Real distance;
		Real angle;
		Slave2MemberVars(Real _distance, Real _angle) :distance(_distance), angle(_angle) {}
		Slave2MemberVars(void) :distance(0.f), angle(0.f) {}
	};
}

MyGameProject::Master2::Slave2::Slave2
(
	BulletPtrContainer& _bullets,
	SEManager& _se_manager,
	const Player& _player,
	gp::smart_ptr<Master2> _master,
	decltype(vars) _vars
)
	:MobEnemy
	(
		_bullets,
		_se_manager,
		EnemyOrder::make_order
		(
			"Slave2",
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
						Point2D relative_pos(1.5f * vars->distance * std::cos(vars->angle), 0.5f * vars->distance * std::sin(vars->angle));
						_this.pos() = _master->pos() + relative_pos;
						vars->angle += 0.06f;
						if(vars->distance < 120) {vars->distance += 0.5f;}
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
	vars(gp::make_smart<Slave2MemberVars>())
{}

void MyGameProject::Master2::Slave2::accessory_custom_updater(void)
{
	if (master && !master->get_flag()) { set_flag_off(); }
}

void MyGameProject::Master2::Slave2::draw(void) const
{
	gp::SetDrawBlendModeOf
	(
		gp::DrawCircle(static_cast<Real>(pos().x()), static_cast<Real>(pos().y()), 15, gp::black.get()),
		DX_BLENDMODE_ALPHA,
		static_cast<int>((255 - 80) - 80 * std::sin(vars->angle))
	);
	if (master) {gp::DrawLine(pos().x(), pos().y(), master->pos().x(), master->pos().y(), gp::black.get()); }
}

void MyGameProject::Master2::Slave2::preperation(void)
{
}

MyGameProject::Master2::Slave2::~Slave2(void) {}

//////////////////////////////////////////////////


/////////////      Master2         ////////////////

MyGameProject::Master2::Master2
(
	BulletPtrContainer& _bullets,
	SEManager& _se_manager,
	gp::smart_ptr<EnemyOrder> _order,
	const Player& _player,
	std::vector<gp::smart_ptr<Enemy>>& _enemies
)
	:MobEnemy(_bullets, _se_manager, _order, MAX_HP_MASTER, Class::MIDDLE, _player, ShapeElement::Circle(20), _enemies)
{}

void MyGameProject::Master2::accessory_custom_updater(void)
{
	if (get_count() == 0)
	{
		if (enemies)
		{
			static constexpr int SLAVE_NUM = 10;
			for (int i = 0; i != SLAVE_NUM; ++i)
			{
				*find_vacant_object(*enemies) =
				gp::make_smart<Slave2>
				(
					get_bullets_ref(),
					get_se_manager_ref(),
					player_ref(),
					shared_from_this(),
					gp::make_smart<Slave2MemberVars>(0, i * boost::math::constants::two_pi<Real>() / SLAVE_NUM)
				);
			}
		}
	}

	const auto x = pos().x(), y = pos().y();
	if (x < -300 || x > 640 + 300 || y < -300 || y > 480 + 300)
	{
		set_flag_off();
	}
}

void MyGameProject::Master2::draw(void) const
{
	gp::DrawCircle(pos().x(), pos().y(), 20, gp::black.get());
}

void MyGameProject::Master2::preperation(void)
{
	Slave2::preperation();
}

MyGameProject::Master2::~Master2(void) {}

/////////////////////////////////////////////////
