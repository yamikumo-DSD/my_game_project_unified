//adapt_class.h

#include "play_game.h"
#include "bindclass.h"
#include "enemy_act_pattern.h"
#include "concrete_enemy.h"
#include "enemy_order.h"
#include "shared_object.h"
#include "find_vacant_object.h"
#include "concrete_bullet.h"

#include "debug_value.h"
#include <type_traits>
#include "se_manager.h"

namespace MyGameProject
{
	namespace Utilities
	{
		inline auto make_order(double _x, double _y)
		{
			return EnemyOrder::make_order("", 0, Point2D(static_cast<Real>(_x), static_cast<Real>(_y)), 0, default_pattern);
		}

		inline void register_mob(std::shared_ptr<MobEnemy> _mob)
		{
			try
			{
				*find_vacant_object(SharedObject::mob_enemies()) = _mob;
				*find_vacant_object(SharedObject::enemies()) = _mob;
			}
			//Ignore if no space.
			catch (NoSpace) {}
		}

		inline void register_boss(std::shared_ptr<Boss> _boss)
		{
			try
			{
				SharedObject::boss() = _boss;
				*find_vacant_object(SharedObject::enemies()) = _boss;
			}
			catch (NoSpace) 
			{
				throw std::runtime_error("Could not summon boss.");
			}
		}

		inline void register_bullet(std::shared_ptr<Bullet> _bullet)
		{
			try
			{
				*find_vacant_object(SharedObject::bullets()) = _bullet;
			}
			//Ignore if no space.
			catch (NoSpace) {}
		}

		//The word 'cluster' means the ctor of the type takes reference to enemy container.
		template<typename MobEnemyType>
		struct is_cluster
		{
			static constexpr bool value
			{
				std::is_same<MobEnemyType,         Master >::value ||
				std::is_same<MobEnemyType,         Master2>::value ||
				std::is_same<MobEnemyType, SmallBattleShip>::value
			};
		};

		//Add set_pos, and get_pos.
		template<typename PositionDefined>
		struct PositionChange : public PositionDefined
		{
			using PositionDefined::PositionDefined;
			void set_pos(double _x, double _y)
			{
				pos().x(static_cast<Real>(_x)); pos().y(static_cast<Real>(_y));
			}
			LuaUtilities::Struct<double, double> get_pos(void)
			{
				return LuaUtilities::Struct<double, double>(pos().x(), pos().y());
			}
		};

		//Add count.
		template<typename HasCount>
		struct Counter : public HasCount
		{
			using HasCount::HasCount;
			int count(void)
			{
				return get_count();
			}
		};

		//Add get_flag.
		template<typename HasFlag>
		struct Flagged : public HasFlag
		{
			using HasFlag::HasFlag;
			bool get_flag(void)
			{
				return HasFlag::get_flag();
			}
		};

		//Simple mob enemy proxy template.
		//Ctor of MobEnemyType must be the following style.
		//MobEnemyType::MobEnemyType
		//(
		//	BulletPtrContainer& _bullets, 
		//  SEManager& _se_manager, 
		//  gp::smart_ptr<EnemyOrder> _order, 
		//  const Player& _player
		//)
		template<typename MobEnemyType>
		struct NonClusterMobProxy : public PositionChange<Flagged<Counter<MobEnemyType>>>
		{
			using PositionChange::PositionChange;
			static std::shared_ptr<NonClusterMobProxy> make_shared(void* _this_game, double _x, double _y)
			{
				auto this_game{ static_cast<PlayGame*>(_this_game) };
				auto mob{ std::make_shared<NonClusterMobProxy>(SharedObject::bullets(), *this_game->se_manager_handle(), make_order(_x, _y), *this_game->player_handle()) };
				register_mob(mob);
				return mob;
			}
		};

		//MobEnemyType::MobEnemyType
		//(
		//	BulletPtrContainer& _bullets, 
		//  SEManager& _se_manager, 
		//  gp::smart_ptr<EnemyOrder> _order, 
		//  const Player& _player,
		//	std::vector<gp::smart_ptr<Enemy>>& _enemies
		//)
		template<typename MobEnemyType>
		struct ClusterMobProxy : public PositionChange<Flagged<Counter<MobEnemyType>>>
		{
			using PositionChange::PositionChange;
			static std::shared_ptr<ClusterMobProxy> make_shared(void* _this_game, double _x, double _y)
			{
				auto this_game{ static_cast<PlayGame*>(_this_game) };
				auto mob{ std::make_shared<ClusterMobProxy>(SharedObject::bullets(), *this_game->se_manager_handle(), make_order(_x, _y), *this_game->player_handle(), SharedObject::enemies()) };
				register_mob(mob);
				return mob;
			}
		};

		//Simple bullet proxy template.
		//Ctor of BulletType must be the following style.
		//BulletType::BulletType
		//(
		//	const Enemy& _master,
		//  const Player& _player,
		//  Point2D _init_pos, 
		//  Real _init_angle,
		//  Bullet::Behavior _behavior
		//)
		template<typename BulletType>
		struct DefaultBulletProxy : public PositionChange<Flagged<Counter<BulletType>>>
		{
			using PositionChange::PositionChange;
			static std::shared_ptr<DefaultBulletProxy> make_shared(void* _this_game, void* _master, double _x, double _y, double _init_angle)
			{
				auto this_game{ static_cast<PlayGame*>(_this_game) };
				auto bullet{ std::make_shared<DefaultBulletProxy>(*static_cast<Enemy*>(_master), *this_game->player_handle(), Point2D(static_cast<Real>(_x), static_cast<Real>(_y)), static_cast<Real>(_init_angle), dummy_course) };
				Utilities::register_bullet(bullet);
				return bullet;
			}
		};

		template<typename BossType>
		struct DefaultBossProxy : public Flagged<BossType>
		{
			using Flagged::Flagged;
			static std::shared_ptr<DefaultBossProxy> make_shared(void* _this_game)
			{
				auto this_game{ static_cast<PlayGame*>(_this_game) };
				auto boss{ std::make_shared<DefaultBossProxy>
				(
					SharedObject::bullets(),
					SharedObject::items(),
					[&se_manager = *this_game->se_manager_handle()](const std::string& _file){se_manager.erect_play_flag_of(_file);}, 
					[this_game](float _speed) {this_game->change_play_speed(_speed); },
					*this_game->player_handle()
				)};
				Utilities::register_boss(boss);
				return boss;
			}
		};

		//Register "set_pos", "get_pos", "count", "get_flag", "destroy".
		template<typename Proxy>
		void register_default_method(LuaUtilities::RegisterCXXClass<Proxy>& _reg)
		{
			_reg.add_memfunc(&Proxy::set_pos     , "set_pos" );
			_reg.add_memfunc(&Proxy::get_pos     , "get_pos" );
			_reg.add_memfunc(&Proxy::get_flag    , "get_flag");
			_reg.add_memfunc(&Proxy::count       , "count"   );
			_reg.add_memfunc(&Proxy::set_flag_off, "destroy" );
		}

		template<typename Proxy>
		void finalize(LuaUtilities::RegisterCXXClass<Proxy>& _reg)
		{
			_reg.finalize(&Proxy::make_shared);
		}

		template<typename Mob>
		struct FindDefaultMobProxy
		{
			using type = std::conditional_t<is_cluster<Mob>::value, ClusterMobProxy<Mob>, NonClusterMobProxy<Mob>>;
		};

		template<typename Entity>
		class FindDefaultProxy
		{
		private:
			template<typename T>
			struct Assert 
			{
				//Delay evaluation of the constant expression by is_same<T, T>.
				static_assert(!std::is_same<T, T>::value, "No proxy for the type.");
			};
		public:
			using type = std::conditional_t
			<
				std::is_base_of<MobEnemy, Entity>::value,
				typename FindDefaultMobProxy<Entity>::type,
				std::conditional_t
				<
					std::is_base_of<Bullet, Entity>::value,
					DefaultBulletProxy<Entity>,
					std::conditional_t
					<
						std::is_base_of<Boss, Entity>::value,
						DefaultBossProxy<Entity>,
						Assert<Entity> //Assert if no default proxy is found.
					>
				>
			>;
		};

		template<typename Entity>
		void register_all_default(lua_State* _state, const char* _name)
		{
			using Proxy = typename FindDefaultProxy<Entity>::type;
			LuaUtilities::RegisterCXXClass<Proxy> reg{_state, _name};
			register_default_method(reg);
			finalize(reg);
		}

	}

	void PlayGame::adapt_class(lua_State* _state)
	{
		using namespace Utilities;

		//Register mob enemies.
		register_all_default<Bomber              >(_state, "Bomber"                 );
		register_all_default<Crow                >(_state, "Crow"                   );
		register_all_default<Crow2               >(_state, "Crow2"                  );
		register_all_default<Crow3               >(_state, "Crow3"                  );
		register_all_default<EmergeFromCloud     >(_state, "EFC"                    );
		register_all_default<RedEye              >(_state, "RedEye"                 );
		register_all_default<ForcedCrow          >(_state, "ForcedCrow"             );
		register_all_default<Fairy_1             >(_state, "Fairy1"                 );
		register_all_default<Fairy_2             >(_state, "Fairy2"                 );
		register_all_default<Fairy_3             >(_state, "Fairy3"                 );
		register_all_default<Amenbo              >(_state, "Amenbo"                 );
		register_all_default<Dragonfly           >(_state, "Dragonfly"              );
		register_all_default<LaserLauncher       >(_state, "LaserLauncher"          );
		register_all_default<Helli               >(_state, "Helli"                  );
		register_all_default<Master              >(_state, "MasterAndSlaves"        );
		register_all_default<RedMaster           >(_state, "RedMaster"              );
		register_all_default<Master2             >(_state, "MasterAndSlaves2"       );
		register_all_default<GreenMaster         >(_state, "GreenMaster"            );
		register_all_default<BlackSpirit         >(_state, "BlackSpirit"            );
		register_all_default<SmallBattleShip     >(_state, "SmallBattleShip"        );
		//Register bullets.
		register_all_default<B0                  >(_state, "B0"                     );
		register_all_default<B1                  >(_state, "B1"                     );
		register_all_default<B234<2>             >(_state, "B2"                     );
		register_all_default<B234<3>             >(_state, "B3"                     );
		register_all_default<B234<4>             >(_state, "B4"                     );
		register_all_default<Typical             >(_state, "Typical"                );
		register_all_default<Cutter              >(_state, "Cutter"                 );
		register_all_default<WindSmasher         >(_state, "WindSmasher"            );
		register_all_default<Feather             >(_state, "Feather"                );
		register_all_default<HenyoriElementOrange>(_state, "HenyoriElementOrange"   );
		register_all_default<CameraFrame         >(_state, "CameraFrame"            );

		//Register bosses.
		using MegaCrowProxy = FindDefaultProxy<MegaCrow>::type;
		LuaUtilities::RegisterCXXClass<MegaCrowProxy> reg_mega_crow{_state, "MegaCrow"};
		reg_mega_crow.add_memfunc(&MegaCrowProxy::get_flag, "get_flag");
		finalize(reg_mega_crow);

		using AyaProxy = FindDefaultProxy<Aya>::type;
		LuaUtilities::RegisterCXXClass<AyaProxy> reg_aya{_state, "Aya"};
		reg_aya.add_memfunc(&AyaProxy::get_flag, "get_flag");
		finalize(reg_aya);

		//Others.
		struct IRBProxy : public Counter<PositionChange<Flagged<IRB>>>
		{
			using Counter::Counter;
			static std::shared_ptr<IRBProxy> make_shared(void* _this_game, void* _master, double _x, double _y, double _init_angle, double _radius)
			{
				auto this_game{ static_cast<PlayGame*>(_this_game) };
				auto bullet{ std::make_shared<IRBProxy>(*static_cast<Enemy*>(_master), *this_game->player_handle(), Point2D(static_cast<Real>(_x), static_cast<Real>(_y)), static_cast<Real>(_init_angle), _radius) };
				Utilities::register_bullet(bullet);
				return bullet;
			}
		};
		LuaUtilities::RegisterCXXClass<IRBProxy> reg_irb{ _state, "InvisibleRoundBullet" };
		register_default_method(reg_irb);
		finalize(reg_irb);

		struct ResistantBulletProxy : public FindDefaultProxy<ResistantBullet>::type
		{
			using Base = FindDefaultProxy<ResistantBullet>::type;
			using Base::Base;

			static std::shared_ptr<ResistantBulletProxy> make_shared(void* _this_game, void* _master, double _x, double _y, double _init_angle, std::string _color)
			{
				auto this_game{ static_cast<PlayGame*>(_this_game) };

				using Color = ResistantBullet::Color;
				Color color;
				if      (_color == "Blue" || _color == "BLUE" || _color == "blue") { color = Color::BLUE;}
				else if (_color == "Dark" || _color == "DARK" || _color == "dark") { color = Color::DARK;}
				else                                                               { color = Color::RED ;}

				auto bullet{ std::make_shared<ResistantBulletProxy>(*static_cast<Enemy*>(_master), *this_game->player_handle(), Point2D(static_cast<Real>(_x), static_cast<Real>(_y)), static_cast<Real>(_init_angle), dummy_course, color) };
				Utilities::register_bullet(bullet);
				return bullet;
			}
		};
		LuaUtilities::RegisterCXXClass<ResistantBulletProxy> reg_res{ _state, "ResistantBullet" };
		register_default_method(reg_res);
		finalize(reg_res);
	}
}