//default_killed_effect.cpp

#include "default_killed_effect.h"
#include "killed_effect_small.h"

namespace MyGameProject
{
	struct KilledEffect::Impl
	{
		int count{ 0 };
	};


	KilledEffect::KilledEffect(const Point2D& _killed_pos) noexcept
		:pimpl(std::make_unique<Impl>()),
		DynamicObject()
	{}

	KilledEffect::KilledEffect(const KilledEffect& _effect) noexcept
		:pimpl(std::make_unique<Impl>(*_effect.pimpl)),
		DynamicObject(_effect)
	{} 

	KilledEffect::KilledEffect(KilledEffect&& _effect) noexcept
		: pimpl(move(_effect.pimpl)),
		DynamicObject(_effect)
	{}

	KilledEffect& KilledEffect::operator=(const KilledEffect& _effect) noexcept
	{
		*pimpl = *_effect.pimpl;
		*static_cast<DynamicObject*>(this) = _effect; //call DynamicObject::operator=
	}

	KilledEffect& KilledEffect::operator=(KilledEffect&& _effect) noexcept
	{
		pimpl = std::move(_effect.pimpl);
		*static_cast<DynamicObject*>(this) = std::move(_effect); //call DynamicObject::operator=
	}

	KilledEffect::~KilledEffect(void) noexcept = default;


	void KilledEffect::update(void)
	{
		custom_updater();
		++pimpl->count;
	}

	int KilledEffect::count(void) const noexcept
	{
		return pimpl->count;
	}

	void KilledEffect::preparation(void) noexcept
	{
		KilledEffectSmall::preparation();
	}

	std::unique_ptr<KilledEffect> make_default_killed_effect(Enemy::Class _class, const Point2D& _killed_pos)
	{
		switch (_class)
		{
		case Enemy::Class::SMALL: return std::make_unique<KilledEffectSmall>(_killed_pos);
		default: return nullptr;
		}
	}

}