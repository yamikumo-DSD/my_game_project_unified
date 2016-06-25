//shared_object.h

#pragma once

#include <vector>
#include <memory>

#ifndef ALIAS_OBJECT_CONTAINER
#define ALIAS_OBJECT_CONTAINER(obj_class, alias) class obj_class; using alias = std::vector<std::shared_ptr<obj_class>>;

#ifndef ALIAS_WEAK_REFERENCE_CONTAINER
#define ALIAS_WEAK_REFERENCE_CONTAINER(obj_class, alias) class obj_class; using alias = std::vector<std::weak_ptr<obj_class>>;

#ifndef ALIAS_OBJECT
#define ALIAS_OBJECT(obj_class, alias) class obj_class; using alias = std::shared_ptr<obj_class>;

#ifndef ALIAS_WEAK_REFERENCE
#define ALIAS_WEAK_REFERENCE(obj_class, alias) class obj_class; using alias = std::weak_ptr<obj_class>;

namespace MyGameProject
{
	ALIAS_OBJECT_CONTAINER(Item, Items)
	ALIAS_OBJECT_CONTAINER(Enemy, Enemies)
	ALIAS_WEAK_REFERENCE_CONTAINER(MobEnemy, MobEnemies)
	ALIAS_WEAK_REFERENCE(Boss, Boss_)
	ALIAS_OBJECT_CONTAINER(Bullet, Bullets)

	namespace SharedObject
	{
		void init_all(void) noexcept;
		Items& items(void) noexcept;
		Enemies& enemies(void) noexcept;
		MobEnemies& mob_enemies(void) noexcept;
		Boss_& boss(void) noexcept;
		Bullets& bullets(void) noexcept;
	};
}

#endif //#ifndef ALIAS_WEAK_REFERENCE
#endif //#ifndef ALIAS_OBJECT
#endif //#ifndef ALIAS_WEAK_REFERENCE_CONTAINER
#endif //#ifndef ALIAS_OBJECT_CONTAINER