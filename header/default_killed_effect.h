//default_killed_effect.h

#pragma once

#include <memory>
#include "object.h"
#include "mathematics.h"
#include "enemy.h"

namespace MyGameProject
{
	class KilledEffect : public DynamicObject
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
		virtual void custom_updater(void) noexcept = 0;
	protected:
		int count(void) const noexcept;
	public:
		explicit KilledEffect(const Point2D& _killed_pos) noexcept;
		KilledEffect(const KilledEffect&) noexcept;
		KilledEffect(KilledEffect&&) noexcept;
		KilledEffect& operator=(const KilledEffect&) noexcept;
		KilledEffect& operator=(KilledEffect&&) noexcept;
		~KilledEffect(void) noexcept;

		virtual void draw(void) const = 0;
		virtual void update(void) override final;

		static void preparation(void) noexcept;
	};

	std::unique_ptr<KilledEffect> make_default_killed_effect(Enemy::Class _class, const Point2D& _killed_pos);
}
