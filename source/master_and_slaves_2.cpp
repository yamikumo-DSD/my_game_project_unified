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
#include "image_pool.h"
#include "debug.h"
#include "shared_object.h"
#include "decl_static_image_handler.h"
#include "history.h"

static constexpr int MAX_HP_MASTER = 2500;
static constexpr int MAX_HP_SLAVE = 100;

/////////////        Slave2        ////////////////

namespace MyGameProject
{
	struct Slave2MemberVars
	{
		Real distance;
		Real angle;
		Real rot{0};
		Real max_distance{0};
		HistoryReserver<10, Point2D, Real> history;
		Slave2MemberVars(Real _distance, Real _angle) :distance(_distance), angle(_angle), history() {}
		Slave2MemberVars(void) :distance(0.f), angle(0.f), history() {}
		STATIC_IMAGE_HANDLER(slave_img)
	};
}

MyGameProject::Master2::Slave2::Slave2
(
	BulletPtrContainer& _bullets,
	SEManager& _se_manager,
	const Player& _player,
	gp::smart_ptr<Master2> _master,
	decltype(vars) _vars,
	Real _max_distance
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
					vars->history.initialize(_this.pos(), angle_of(_this.velocity()));
					if (_master)
					{
						_this.pos() = _master->pos();
					}
				}
				else
				{
					if (_master)
					{
						const auto pre_pos{ _this.pos() };
						const auto pre_dir{angle_of(_this.velocity())};
						Point2D relative_pos(1.0f * vars->distance * std::cos(vars->angle), 1.0f * vars->distance * std::sin(vars->angle));
						_this.pos() = _master->pos() + relative_pos;
						vars->angle += 0.06f;
						if (count > 150)
						{
							if (count % 30 == 0)
							{
								try
								{
									*find_vacant_object(SharedObject::bullets()) = Bullet::create("B2", _this, _player, _this.pos(), angle_of(_master->pos() - _this.pos()), straight_course(2));
								}
								catch (...) {}
							}
						}
						if(vars->distance < vars->max_distance) {vars->distance += vars->max_distance / 100;}
						vars->history.shift_forward(std::move(pre_pos), std::move(pre_dir));
					}
				}
			}
		),
		MAX_HP_SLAVE,
		Class::SMALL,
		_player,
		ShapeElement::Circle(20)
	),
	master(_master),
	vars(gp::make_smart<Slave2MemberVars>())
{
	vars->max_distance = _max_distance;
}

void MyGameProject::Master2::Slave2::accessory_custom_updater(void)
{
	if (master && !master->get_flag()) { set_flag_off(); }
	else
	{
		vars->rot += 0.15f;
	}
}

void MyGameProject::Master2::Slave2::draw(void) const
{
	//gp::SetDrawBlendModeOf
	//(
	//	//gp::DrawCircle(static_cast<Real>(pos().x()), static_cast<Real>(pos().y()), 15, gp::black.get()),
	//	gp::DrawRotaGraphF(gp::level(10), pos().x(), pos().y(), 1.0, vars->rot, Slave2MemberVars::slave_img(), true),
	//	DX_BLENDMODE_ALPHA,
	//	static_cast<int>((255 - 80) - 80 * std::sin(vars->angle))
	//);
		const auto& history{vars->history};
		for (int i = 0; i != history.size(); ++i)
		{
			gp::DrawRotaGraphF
			(
				gp::level(10), 
				get<0>(history[i]).x(), get<0>(history[i]).y(), (vars->history.size() - static_cast<float>(i)) / vars->history.size(), get<1>(history[0]),
				Slave2MemberVars::slave_img(), true
			);
			if (i < history.size() - 1)
			{
				gp::DrawRotaGraphF
				(
					gp::level(10), 
					(get<0>(history[i]).x() + get<0>(history[i + 1]).x()) / 2,
					(get<0>(history[i]).y() + get<0>(history[i + 1]).y()) / 2,
					(vars->history.size() - static_cast<float>(i + 0.5f)) / vars->history.size(),
					get<1>(history[0]),
					Slave2MemberVars::slave_img(), true
				);
			}
		}
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
		static constexpr int SLAVE_NUM = 10;
		for (int i = 0; i != SLAVE_NUM; ++i)
		{
			for (int j = 0; j != 4; ++j)
			{
				auto temp
				{
					gp::make_smart<Slave2>
					(
						get_bullets_ref(),
						get_se_manager_ref(),
						player_ref(),
						shared_from_this(),
						gp::make_smart<Slave2MemberVars>(0, j * two_pi<Real>() / 12 + i * boost::math::constants::two_pi<Real>() / SLAVE_NUM),
						200 + 40 * j
					)
				};
				*find_vacant_object(SharedObject::enemies()) = temp;
				*find_vacant_object(SharedObject::mob_enemies()) = std::move(temp);
			}
		}
	}

	const auto x = pos().x(), y = pos().y();
	if (x < -300 || x > 640 + 300 || y < -300 || y > 480 + 300)
	{
		set_flag_off();
	}
}

enum class Direction
{
	RIGHT = 2,
	LEFT = 3,
};

inline constexpr auto dir2int(Direction _dir) { return static_cast<int>(_dir); }

void MyGameProject::Master2::draw(void) const
{
	const auto vx{ velocity().x() };
	if (vx > 0.2)
	{
		gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 0.5, 0.0, img()[dir2int(Direction::RIGHT)], true);
	}
	else if(vx < -0.2)
	{
		gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 0.5, 0.0, img()[dir2int(Direction::LEFT)], true);
	}
	else
	{
		gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 0.5, 0.0, img()[get_count() % 30 > 15], true);
	}
}

void MyGameProject::Master2::preperation(void)
{
	Slave2::preperation();
	ImagePool::add("../../data/img/zako0.png");
	img()[0] = ImagePool::get("../../data/img/zako0.png");
	ImagePool::add("../../data/img/zako1.png");
	img()[1] = ImagePool::get("../../data/img/zako1.png");
	ImagePool::add("../../data/img/zako_migi.png");
	img()[2] = ImagePool::get("../../data/img/zako_migi.png");
	ImagePool::add("../../data/img/zako_hidari.png");
	img()[3] = ImagePool::get("../../data/img/zako_hidari.png");

	ImagePool::add("../../data/img/dark_force.png");
	Slave2MemberVars::slave_img() = ImagePool::get("../../data/img/dark_force.png");
}

MyGameProject::Master2::~Master2(void) {}

/////////////////////////////////////////////////
