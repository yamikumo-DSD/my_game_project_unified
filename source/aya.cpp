//aya.cpp

#include "aya.h"
#include "mathematics.h"
#include "aya_imple.h"

namespace MyGameProject
{
	Aya::Aya
	(
		BulletPtrContainer& _bullets,
		ItemPtrContainer& _items,
		std::function<void(const std::string&)> _se_flag_setter,
		std::function<void(float)> _play_speed_changer,
		const Player& _player
	)
	:Boss(_bullets, _items, _se_flag_setter, Point2D(0, 0), 0, _player, ShapeElement::Circle(20)),
	health(20000),
	change_play_speed(_play_speed_changer),
	immortal_flag(true),
	imple(std::make_unique<AyaImple>(*this))
	{}

	void Aya::hit(int _shot_power)
	{
		if (!immortal_flag)
		{
			health -= _shot_power;
		}
	}

	void Aya::custom_updater(void)
	{
		imple->action();
	}

	void Aya::draw_charactor(void) const
	{
		imple->draw_charactor();
	}

	void Aya::draw_health_bar(void) const
	{

	}

	Aya::~Aya(void) {}

	void Aya::preperation(void)
	{
		AyaImple::preperation();
	}

	int Aya::get_health(void) const { return health; }
}
