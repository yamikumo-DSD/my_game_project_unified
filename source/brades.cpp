//brades.cpp

#include "brades.h"
#include "draw_order.h"
#include "shared_object.h"
#include "bullet.h"
#include "image_pool.h"
#include "add_get.h"
#include "environmental_constants.h"
#include "history.h"
#include <dxlib.h>
#include "is_id_enable.h"
#include "find_vacant_object.h"
#include "debug_value.h"
#include "motion.h"
#include "bullet_behavior.h"
#include "player.h"
#include "ranged_uniform_random.h"

namespace MyGameProject
{
	static constexpr int INITIAL_HEALTH{30000};
	using P = Point2D;

	inline decltype(auto) free_bullet(void) { return *find_vacant_object(SharedObject::bullets()); }

	class Force : public Bullet
	{
	private:
		const int master_id;
		virtual void custom_updater(void) override final
		{
			if (!is_id_enable(SharedObject::enemies(), master_id)) { set_flag_off(); }
		}
	public:
		Force(const Enemy& _master, const Player& _player) 
			:Bullet(_master, _player, _master.pos(), 0, ShapeElement::Null(),
				[](Bullet& _bullet) {_bullet.pos() = _bullet.get_master_ref().pos(); }),
			master_id(_master.get_id())
		{
			ShapeElement::Polygon shape;
			shape.outer().emplace_back(-150, -50);
			shape.outer().emplace_back(+150, -50);
			shape.outer().emplace_back(+150, +50);
			shape.outer().emplace_back(-150, +50);
			shape.outer().emplace_back(-150, -50);
			area().get_shape() = std::move(shape);
		}
		//Real theta{0};
		virtual void draw(void) const override final {/* Do notiong */}
		virtual void hit(void) override final {/* Do notiong  */}
		virtual bool is_resistant(void) const { return true; }
		~Force(void) {}
	};

	namespace ImageHandle
	{
		static int brades{0};
		static int core{0};
		static int termination_effect0_img{0};
		static int termination_effect1_img{0};
	}

	enum class Action
	{
		ENTERING = 0,
		SCATTERING,
		STINGER,
		SHADOWING,
	};

	struct Brades::Impl
	{
		Action action{Action::ENTERING};
		int action_count{0};
		LinearMotionOfUniformAcceleration2D<P, int> motion;
		P v;
		int count_after_killed{0};
		std::weak_ptr<Force> force0, force1;
		bool immortal_flag{false};
		int health{INITIAL_HEALTH};
		HistoryReserver<10, P, Real, Real> trails;
		Real t0{ 0.f }, t1{0.f};
		Real shadowing_direction{0};
		void next_action(void)
		{
			action_count = 0;
			const auto prev{action};
			while (true)
			{
				static std::random_device seed_gen;
				static std::minstd_rand0 random_engine{seed_gen()};
				const auto next{ static_cast<Action>(random_engine() % 3 + 1) };
				if (next != prev)
				{
					action = next;
					break;
				}
			}
		}
		Impl(const Enemy& _master, const Player& _player)
		{
			auto f{ std::make_shared<Force>(_master, _player) };
			*find_vacant_object(SharedObject::bullets()) = f;
			force0 = f;
			f->area().get_physical_state().angle = -t0;

			f = std::make_shared<Force>(_master, _player);
			*find_vacant_object(SharedObject::bullets()) = f;
			force1 = f;
			f->area().get_physical_state().angle = -t1;
		}
	};

	MyGameProject::Brades::Brades
	(
		BulletPtrContainer& _bullets,
		ItemPtrContainer& _items,
		std::function<void(const std::string&)> _se_flag_setter,
		std::function<void(float)> _play_speed_changer,
		const Player& _player
	)
		:Boss
		(
			_bullets, _items, _se_flag_setter,
			P(WW_2<Real>(), -300), 0,
			_player, ShapeElement::Circle(150)
		),
		change_play_speed(_play_speed_changer),
		pimpl(std::make_unique<Impl>(*this, _player))
	{
		pimpl->trails.initialize(pos(), pimpl->t0, pimpl->t1);
	}

	void MyGameProject::Brades::hit(int _shot_power)
	{
		if (!pimpl->immortal_flag)
		{
			pimpl->health -= _shot_power;
		}
	}

	void MyGameProject::Brades::custom_updater(void)
	{
		const auto count{get_count()};
		if (get_health() > 0)
		{
			if (count == 0)
			{
				pimpl->motion = create_linear_motion_of_uniform_acceleration_2D(pos(), P(WW_2<Real>(), 200), 80);
				pimpl->v = pimpl->motion.v0();
			}
			else if (count > 0 && count <= 80)
			{
				pimpl->t0 += 0.05f;
				pimpl->t1 -= 0.05f;
				pimpl->v += pimpl->motion.a();
				if (count == 80) { pimpl->action = Action::SCATTERING; }
			}
			else
			{
				switch (pimpl->action)
				{
				case Action::ENTERING:
				{
					pimpl->t0 += 0.05f;
					pimpl->t1 -= 0.05f;
				} break;
				case Action::SCATTERING:
				{
					if (count % 6 == 0)
					{
						free_bullet() = Bullet::create("B1", *this, player_ref(), pos() + 100 * P(cos(pimpl->t0), sin(pimpl->t0)), pimpl->t0, straight_course(4));
						free_bullet() = Bullet::create("B1", *this, player_ref(), pos() - 100 * P(cos(pimpl->t0), sin(pimpl->t0)), pimpl->t0 + pi<Real>(), straight_course(4));
						free_bullet() = Bullet::create("B1", *this, player_ref(), pos() + 100 * P(cos(pimpl->t1), sin(pimpl->t1)), pimpl->t1, straight_course(4));
						free_bullet() = Bullet::create("B1", *this, player_ref(), pos() - 100 * P(cos(pimpl->t1), sin(pimpl->t1)), pimpl->t1 + pi<Real>(), straight_course(4));
					}
					pimpl->t0 += 0.05f;
					pimpl->t1 -= 0.05f;
					if (pimpl->action_count == 300)
					{
						pimpl->next_action();
					}
					pimpl->v = P(0, 0);
					++pimpl->action_count;
				} break;
				case Action::STINGER:
				{
					if (pimpl->action_count < 90)
					{
						//Transforming
						const auto cross{ cos(pimpl->t0) * sin(pimpl->t1) - sin(pimpl->t0) * cos(pimpl->t1) };
						pimpl->t0 += (0.05f * cross);
						pimpl->t1 -= (0.05f * cross);
						pimpl->v = P(0, 0);
					}
					else
					{
						const auto c{ (pimpl->action_count - 90) % 200 };
						auto& t{ pimpl->t0 };
						if (c < 120)
						{
							const auto dir{ player_ref().pos() - pos() };
							const auto d{ norm(player_ref().pos() - pos()) };
							const auto cross{ cos(t) * dir.y() - sin(t) * dir.x() };
							t += (0.05 * cross / d);
							pimpl->v = P(0, 0);
							if (pimpl->action_count > 900)
							{
								pimpl->next_action();
							}
						}
						else if (c == 120)
						{
							const auto d{ norm(player_ref().pos() - pos()) };
							pimpl->motion = create_linear_motion_of_uniform_acceleration_2D(pos(), pos() + (d + 50) * P(cos(t), sin(t)), 60);
							pimpl->v = pimpl->motion.v0();
						}
						else if (c > 120 && c < 180)
						{
							pimpl->v += pimpl->motion.a();
						}
						pimpl->t1 = pimpl->t0;
					}
					++pimpl->action_count;
				} break;
				case Action::SHADOWING:
				{
					const auto dir{ player_ref().pos() - pos() };
					const auto d{ norm(player_ref().pos() - pos()) };
					const auto cross{ cos(pimpl->shadowing_direction) * dir.y() - sin(pimpl->shadowing_direction) * dir.x() };
					pimpl->shadowing_direction += 0.02 * cross / d;
					pimpl->t0 += 0.05f;
					pimpl->t1 -= 0.05f;
					pimpl->v = 2 * P(cos(pimpl->shadowing_direction), sin(pimpl->shadowing_direction));
					if (pimpl->action_count % 20 == 0)
					{
						const auto t{ gp::safe_rand(0.f, two_pi<Real>()) };
						for (int i = 0; i != 5; ++i)
						{
							free_bullet() = Bullet::create("B1", *this, player_ref(), pos(), t + two_pi<Real>() / 5 * i, acceleration(2, 0.05, 7));
						}
					}
					if (pimpl->action_count == 600)
					{
						pimpl->next_action();
					}
					++pimpl->action_count;
				} break;
				default:
				{
				} break;
				}
			}
			pimpl->trails.shift_forward(pos(), pimpl->t0, pimpl->t1);
			if (!pimpl->force0.expired())
			{
				auto p{ pimpl->force0.lock() };
				p->area().get_physical_state().angle = -pimpl->t0;
				p->area().get_physical_state().p = pos();
			}
			if (!pimpl->force1.expired())
			{
				auto p{ pimpl->force1.lock() };
				p->area().get_physical_state().angle = -pimpl->t1;
				p->area().get_physical_state().p = pos();
			}
			pos() += pimpl->v;
		}
		else
		{
			if (pimpl->count_after_killed == 2)
			{
				erect_se_flag_of("../../data/sound/bomb.wav");
				pimpl->force0.lock()->area().get_shape() = pimpl->force1.lock()->area().get_shape() 
				= area().get_shape() = ShapeElement::Null();
			}
			if (++pimpl->count_after_killed > 400)
			{
				set_flag_off();
			}
		}
	}

	void MyGameProject::Brades::draw_charactor(void) const
	{
		if (pimpl->count_after_killed < 150)
		{
			const auto count{ get_count() };
			if (count > 0)
			{
				for (auto&& t : pimpl->trails)
				{
					using namespace gp;
					auto&& p{ std::get<0>(t) };
					auto&& t0{ std::get<1>(t) };
					auto&& t1{ std::get<2>(t) };
					gp::SetDrawBlendModeOf
					(
						gp::DrawRotaGraphF(gp::level(11), p.x(), p.y(), 0.30, t0, ImageHandle::brades, true) >>
						gp::DrawRotaGraphF(gp::level(11), p.x(), p.y(), 0.30, t1, ImageHandle::brades, true),
						DX_BLENDMODE_ADD,
						100
					);
				}
				gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 0.30, pimpl->t0, ImageHandle::brades, true);
				gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 0.30, pimpl->t1, ImageHandle::brades, true);
				gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), 0.34, 0.0, ImageHandle::core, true);
			}
		}
		//Draw killed effect.
		if (pimpl->count_after_killed > 0)
		{
			static const auto r = [](const int _c)
			{
				return _c / 50.f - _c / 50;
			};
			static const auto p = [](const int _c)
			{
				return 255 * (1 - (_c / 50.f - _c / 50));
			};
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), r(pimpl->count_after_killed), 0, ImageHandle::termination_effect0_img, true),
					DX_BLENDMODE_ADD,
					p(pimpl->count_after_killed)
				);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), pimpl->count_after_killed / 200.f, 0.1 * pimpl->count_after_killed * pimpl->count_after_killed, ImageHandle::termination_effect1_img, true),
					DX_BLENDMODE_ADD,
					255 * (1 - pimpl->count_after_killed / 400.f)
				);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), pos().x(), pos().y(), pimpl->count_after_killed / 200.f, 0.1 * pimpl->count_after_killed, ImageHandle::termination_effect1_img, true),
					DX_BLENDMODE_ADD,
					255 * (1 - pimpl->count_after_killed / 400.f)
				);
		}
	}

	void MyGameProject::Brades::draw_health_bar(void) const
	{
	}

	MyGameProject::Brades::~Brades(void) {}

	void MyGameProject::Brades::preperation(void)
	{
		ImageHandle::brades = add_get("../../data/img/brades/brades.png");
		ImageHandle::core = add_get("../../data/img/brades/core.png");
		ImageHandle::termination_effect0_img = add_get("../../data/img/killed_0.png");
		ImageHandle::termination_effect1_img = add_get("../../data/img/killed_1.png");
	}

	int MyGameProject::Brades::get_health(void) const { return pimpl->health; }

	BackGroundControl::OptionalSignal Brades::notify(void) const noexcept
	{
		return boost::none;
	}
}
