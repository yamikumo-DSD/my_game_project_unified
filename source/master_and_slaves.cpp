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
#include "shared_object.h"
#include "item.h"
#include "image_pool.h"
#include "decl_static_image_handler.h"
#include "history.h"

static constexpr int MAX_HP_MASTER = 500;
static constexpr int MAX_HP_SLAVE = 150;

/////////////        Slave        ////////////////

namespace MyGameProject
{
		struct SlaveMemberVars
		{
			Real distance;
			Real angle;
			SlaveMemberVars(Real _distance, Real _angle) :distance(_distance), angle(_angle) {}
			SlaveMemberVars(void) :distance(0.f), angle(0.f) {}
			STATIC_IMAGE_HANDLER_LIST(img, 3)
			STATIC_IMAGE_HANDLER(slave_img)
			Real rot {0};
			HistoryReserver<7, Point2D, Real> history;
		};

	Master::Slave::Slave
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
							_this.pos() = _master->pos() + Point2D(vars->distance * std::cos(vars->angle), vars->distance * std::sin(vars->angle));
							vars->angle += 0.1f;
							if(vars->distance < 150) {vars->distance += 0.5f;}
							vars->history.shift_forward(std::move(pre_pos), std::move(pre_dir));
							if(vars->distance >= 40 && vars->distance < 140)
							{
								if(count % 5 == 0){ *find_vacant_object(SharedObject::bullets()) = Bullet::create("B2", _this, _player, _this.pos(), vars->angle, straight_course(4));}
							}
						}
					}
				} //act pattern def
			), //EnemyOrder::make_order
			MAX_HP_SLAVE,
			Class::SMALL,
			_player,
			ShapeElement::Circle(20)
		), //Master::Slave::MobEnemy
		master(_master),
		vars(gp::make_smart<SlaveMemberVars>())
	{} //Master::Slave::Slave

	void Master::Slave::accessory_custom_updater(void)
	{
		if (master && !master->get_flag()) { set_flag_off(); }
		else
		{
			vars->rot += 0.1f;
		}
	}

	int Master::Slave::released_p_item_num(void) const
	{
		return 6;
	}

	void Master::Slave::draw(void) const
	{
		gp::DrawRotaGraphF(gp::level(10), pos().x(), pos().y(), 1.0, vars->rot, SlaveMemberVars::slave_img(), true);
		if (master) {gp::DrawLine(pos().x(), pos().y(), master->pos().x(), master->pos().y(), gp::black.get()); }

		const auto& history{vars->history};
		for (int i = 0; i != history.size(); ++i)
		{
			gp::DrawRotaGraphF
			(
				gp::level(10), 
				get<0>(history[i]).x(), get<0>(history[i]).y(), (vars->history.size() - static_cast<float>(i)) / vars->history.size(), get<1>(history[0]),
				SlaveMemberVars::slave_img(), true
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
					SlaveMemberVars::slave_img(), true
				);
			}
		}
	}

	void Master::Slave::preperation(void)
	{
	}


	Master::Slave::~Slave(void) {}

	//////////////////////////////////////////////////


	/////////////      Master         ////////////////

	Master::Master
	(
		BulletPtrContainer& _bullets,
		SEManager& _se_manager,
		gp::smart_ptr<EnemyOrder> _order,
		const Player& _player,
		std::vector<gp::smart_ptr<Enemy>>& _enemies
	)
		:MobEnemy(_bullets, _se_manager, _order, MAX_HP_MASTER, Class::MIDDLE, _player, ShapeElement::Circle(20), _enemies)
	{}

	void Master::accessory_custom_updater(void)
	{
		if (get_count() == 0)
		{
				static constexpr int SLAVE_NUM = 6;
				for (int i = 0; i != SLAVE_NUM; ++i)
				{
					auto temp
					{
						gp::make_smart<Slave>
						(
							get_bullets_ref(),
							get_se_manager_ref(),
							player_ref(),
							shared_from_this(),
							gp::make_smart<SlaveMemberVars>(0, i * boost::math::constants::two_pi<Real>() / SLAVE_NUM)
						) 
					};
					*find_vacant_object(SharedObject::enemies()) = temp;
					*find_vacant_object(SharedObject::mob_enemies()) = std::move(temp);
				}
		}

		const auto x = pos().x(), y = pos().y();
		if (x < -100 || x > 640 + 100 || y < -100 || y > 480 + 100)
		{
			set_flag_off();
		}
	} //Master::accessory_cutom_updater

	void Master::draw(void) const
	{
		if (norm(velocity()) < 3) //if *this is moving slowly...
		{
			gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 1.3, 0.0, SlaveMemberVars::img()[(get_count() / 30) % 2], true);
		}
		else
		{
			gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 1.3, angle_of(velocity()) + boost::math::constants::half_pi<Real>(), SlaveMemberVars::img()[2], true);
		}
	}

	void Master::preperation(void)
	{
		Slave::preperation();
		ImagePool::add("../../data/img/red_little_master0.png");
		SlaveMemberVars::img()[0] = ImagePool::get("../../data/img/red_little_master0.png");
		ImagePool::add("../../data/img/red_little_master1.png");
		SlaveMemberVars::img()[1] = ImagePool::get("../../data/img/red_little_master1.png");
		ImagePool::add("../../data/img/red_little_master2.png");
		SlaveMemberVars::img()[2] = ImagePool::get("../../data/img/red_little_master2.png");

		ImagePool::add("../../data/img/dark_force.png");
		SlaveMemberVars::slave_img() = ImagePool::get("../../data/img/dark_force.png");
	}

	Master::~Master(void) {}
}

/////////////////////////////////////////////////