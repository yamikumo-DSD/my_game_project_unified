//fairy_123.cpp

#include "fairy_123.h"
#include "header_set_for_mob_enemy.h"
#include "environmental_constants.h"

namespace MyGameProject
{
	struct Fairy_1::Impl
	{
		static constexpr auto INITIAL_HEALTH{200};
		STATIC_IMAGE_HANDLER_LIST(img, 3)
		enum Direction {RIGHT = 0, FRONT, LEFT};
	};

	struct Fairy_2::Impl
	{
		static constexpr auto INITIAL_HEALTH{200};
		STATIC_IMAGE_HANDLER_LIST(img, 3)
		enum Direction {RIGHT = 0, FRONT, LEFT};
	};

	struct Fairy_3::Impl
	{
		static constexpr auto INITIAL_HEALTH{200};
		STATIC_IMAGE_HANDLER_LIST(img, 2)
		enum Direction {FRONT = 0, BACK};
	};

#ifndef IMPL_CTOR_DTOR
#define IMPL_CTOR_DTOR(number, initial_health)\
	Fairy_##number::Fairy_##number\
	(\
		BulletPtrContainer& _bullets,\
		SEManager& _se_manager,\
		gp::smart_ptr<EnemyOrder> _order,\
		const Player& _player\
	) noexcept\
		:MobEnemy(_bullets, _se_manager, _order, initial_health, Class::SMALL, _player, ShapeElement::Circle(15)),\
		pimpl(std::make_unique<Impl>())\
	{}\
\
	Fairy_##number::Fairy_##number(const Fairy_##number& _fairy) noexcept\
		:MobEnemy(_fairy),\
		pimpl(std::make_unique<Impl>(*_fairy.pimpl))\
	{}\
\
	Fairy_##number::Fairy_##number(Fairy_##number&& _fairy) noexcept = default;\
\
	Fairy_##number::~Fairy_##number(void) noexcept = default;

	IMPL_CTOR_DTOR(1, Fairy_1::Impl::INITIAL_HEALTH)
	IMPL_CTOR_DTOR(2, Fairy_2::Impl::INITIAL_HEALTH)
	IMPL_CTOR_DTOR(3, Fairy_3::Impl::INITIAL_HEALTH)
#undef IMPL_CTOR_DTOR
#endif
	

	void Fairy_1::accessory_custom_updater(void) 
	{
		if (get_count() > 200 && (pos().x() < 0 || pos().x() > WW<Real>() || pos().y() < 0 || pos().y() > WH<Real>()))
		{
			set_flag_off();
		}
	}

	void Fairy_1::draw(void) const
	{
		static constexpr auto THRESHOLD{0.5};
		const auto vx{velocity().x()};
		std::size_t image_channel{0};
		if (vx > THRESHOLD) { image_channel = Impl::Direction::RIGHT; }
		else if (vx < -THRESHOLD) { image_channel = Impl::Direction::LEFT; }
		else { image_channel = Impl::Direction::FRONT; }

		gp::DrawRotaGraphF
		(
			gp::level(11), 
			pos().x(), pos().y(), 1.3, 0.0,
			Impl::img()[image_channel], true
		);
	}

	void Fairy_1::preparation(void)
	{
		ImagePool::add("../../data/img/fairy_1_01.png");
		Impl::img()[Impl::Direction::LEFT] = ImagePool::get("../../data/img/fairy_1_01.png");
		ImagePool::add("../../data/img/fairy_1_02.png");
		Impl::img()[Impl::Direction::FRONT] = ImagePool::get("../../data/img/fairy_1_02.png");
		ImagePool::add("../../data/img/fairy_1_03.png");
		Impl::img()[Impl::Direction::RIGHT] = ImagePool::get("../../data/img/fairy_1_03.png");
	}

	int Fairy_1::released_p_item_num(void) const { return 8; }


	void Fairy_2::accessory_custom_updater(void) 
	{
		if
		(
			get_count() > 200 && 
			(
				pos().x() < -100 || 
				pos().x() > WW<Real>() + 100 ||
				pos().y() < -100 ||
				pos().y() > WH<Real>() + 100
			)
		)
		{
			set_flag_off();
		}
	}

	void Fairy_2::draw(void) const
	{
		static constexpr auto THRESHOLD{0.5};
		const auto vx{velocity().x()};
		std::size_t image_channel{0};
		if (vx > THRESHOLD) { image_channel = Impl::Direction::RIGHT; }
		else if (vx < -THRESHOLD) { image_channel = Impl::Direction::LEFT; }
		else { image_channel = Impl::Direction::FRONT; }

		gp::DrawRotaGraphF
		(
			gp::level(11), 
			pos().x(), pos().y(), 1.5, 0.0,
			Impl::img()[image_channel], true
		);
	}

	void Fairy_2::preparation(void)
	{
		ImagePool::add("../../data/img/fairy_2_01.png");
		Impl::img()[Impl::Direction::LEFT] = ImagePool::get("../../data/img/fairy_2_01.png");
		ImagePool::add("../../data/img/fairy_2_02.png");
		Impl::img()[Impl::Direction::FRONT] = ImagePool::get("../../data/img/fairy_2_02.png");
		ImagePool::add("../../data/img/fairy_2_03.png");
		Impl::img()[Impl::Direction::RIGHT] = ImagePool::get("../../data/img/fairy_2_03.png");
	}

	int Fairy_2::released_p_item_num(void) const { return 8; }


	void Fairy_3::accessory_custom_updater(void) 
	{
		if (get_count() > 200 && (pos().x() < 0 || pos().x() > WW<Real>() || pos().y() < 0 || pos().y() > WH<Real>()))
		{
			set_flag_off();
		}
	}

	void Fairy_3::draw(void) const
	{
		static constexpr auto V_THRESHOLD{0.5};
		const auto dir{angle_of(velocity()) + half_pi<Real>()};
		const auto v{norm(velocity())};
		gp::DrawRotaGraphF
		(
			gp::level(11),
			pos().x(), pos().y(), 1.0, 
			v > V_THRESHOLD? dir : 0.0,
			Impl::img()[v > V_THRESHOLD? Impl::Direction::BACK : Impl::Direction::FRONT],
			true
		);
	}

	void Fairy_3::preparation(void)
	{
		ImagePool::add("../../data/img/fairy_3_01.png");
		Impl::img()[Impl::Direction::FRONT] = ImagePool::get("../../data/img/fairy_3_01.png");
		ImagePool::add("../../data/img/fairy_3_02.png");
		Impl::img()[Impl::Direction::BACK] = ImagePool::get("../../data/img/fairy_3_02.png");
	}

	int Fairy_3::released_p_item_num(void) const { return 8; }
}