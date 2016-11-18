//item.cpp

#include "item.h"
#include "player.h"
#include <exception>
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include <cmath>
#include "color.h"
#include "image_pool.h"
#include "decl_static_image_handler.h"
#include "debug.h"

namespace MyGameProject
{
	using namespace std;

	gp::smart_ptr<Item> Item::create
		(
			const std::string& _name,
			const Player& _player,
			const Point2D& _initial_pos
			)
	{
		if (_name == "ConvertedBullet") { return gp::make_smart<ConvertedBullet>(_player, _initial_pos); }
		//else if (_name == "Point") { return gp::make_smart<Point>(_player, _initial_pos); }
		else { throw std::runtime_error("Undefined type of enemy."); }
	}

	Item::Item
		(
			const Player& _player,
			const Point2D& _initial_pos
			)
		:player(_player),
		MovingObject(_initial_pos),
		HitableObject(HitBox(ShapeElement::Circle(25), PhysicalState(0, _initial_pos))),
		count(0)
	{}

	int Item::get_count(void) const
	{
		return count;
	}

	const Player& Item::get_player_ref(void) const
	{
		return player;
	}

	void Item::update(void)
	{
		custom_updater();
		area().get_physical_state().p = pos();
		++count;
	}

	Item::~Item(void) {}

	void Item::preperation(void)
	{
		ConvertedBullet::preperation();
		Point::preperation();
	}

	ConvertedBullet::ConvertedBullet(const Player& _player, const Point2D& _initial_pos)
		:Item(_player, _initial_pos),
		angle(0)
	{}

	void ConvertedBullet::custom_updater(void)
	{
		static constexpr Real VELOCITY = 13;
		angle = angle_of(get_player_ref().pos() - pos());
		pos().x(pos().x() + VELOCITY * cos(angle));
		pos().y(pos().y() + VELOCITY * sin(angle));
		if (get_count() > 300) { set_flag_off(); }
	}

	void ConvertedBullet::draw(void) const
	{
		ScopedDrawBlendMode blend(DX_BLENDMODE_ALPHA, 150);
		std::string img;
		switch (get_count() % 16)
		{
		case 0:
			img = "../../data/img/converted_bullet.png@0"; break;
		case 1:
			img = "../../data/img/converted_bullet.png@1"; break;
		case 2:
			img = "../../data/img/converted_bullet.png@2"; break;
		case 3:
			img = "../../data/img/converted_bullet.png@3"; break;
		case 4:
			img = "../../data/img/converted_bullet.png@4"; break;
		case 5:
			img = "../../data/img/converted_bullet.png@5"; break;
		case 6:
			img = "../../data/img/converted_bullet.png@6"; break;
		case 7:
			img = "../../data/img/converted_bullet.png@7"; break;
		case 8:
			img = "../../data/img/converted_bullet.png@8"; break;
		case 9:
			img = "../../data/img/converted_bullet.png@7"; break;
		case 10:
			img = "../../data/img/converted_bullet.png@6"; break;
		case 11:
			img = "../../data/img/converted_bullet.png@5"; break;
		case 12:
			img = "../../data/img/converted_bullet.png@4"; break;
		case 13:
			img = "../../data/img/converted_bullet.png@3"; break;
		case 14:
			img = "../../data/img/converted_bullet.png@2"; break;
		case 15:
			img = "../../data/img/converted_bullet.png@1"; break;
		default:
			break;
		}

		if (get_count() < 200) { gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 1.0, 0.0, ImagePool::get(img), true); }
		else
		{
			if (get_count() % 3 == 0) { gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 1.0, 0.0, ImagePool::get(img), true); }
		}
	}

	void ConvertedBullet::preperation(void)
	{
		ImagePool::add_multi("../../data/img/converted_bullet.png", 9, 9, 1, 30, 30);
	}

	ConvertedBullet::~ConvertedBullet(void) {}

	struct Point::Impl
	{
		STATIC_IMAGE_HANDLER_LIST(img, 6)
		Real angle;
		bool is_activated{false};
		static constexpr auto LIFE_TIME = 1000;
	};

	//Ctors
	Point::Point(const Player& _player, const Point2D& _initial_pos, Real _initial_angle) noexcept
		:pimpl(std::make_unique<Impl>()),
		Item(_player, _initial_pos)
	{
		pimpl->angle = _initial_angle;
	}
	Point::Point(const Point& _point) noexcept:pimpl(std::make_unique<Impl>(*_point.pimpl)), Item(_point){} 
	Point::Point(Point&& _point) noexcept:pimpl(std::move(_point.pimpl)),Item(std::move(_point)){}
	//Assign operators
	Point& Point::operator=(const Point& _point) noexcept
	{
		auto tmp{_point};
		*this = std::move(tmp);
		return *this;
	}
	Point& Point::operator=(Point&& _point) noexcept 
	{
		std::swap(*this, _point);
		return *this;
	}
	void Point::draw(void) const
	{
		if (get_count() < Impl::LIFE_TIME - 150)
		{
			gp::DrawRotaGraphF(gp::level(12),pos().x(), pos().y(), 1.0, 0.0, Impl::img()[(get_count() / 4) % 6], true);
		}
		else
		{
			if (get_count() % 2 == 0)
			{
				gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 1.0, 0.0, Impl::img()[(get_count() / 4) % 6], true);
			}
		}
	}
	void Point::custom_updater(void)
	{
		const auto count{get_count()};
		Real v{0};
		if (count < 7) { v = 10; }
		else if (count >= 7 && count < 15) { v = 0; /*stay*/ }
		else
		{
			const auto player_pos{get_player_ref().pos()};
			if (pimpl->is_activated)
			{
				v = 8;
				pimpl->angle = angle_of(player_pos - pos());
			}
			else
			{
				if (norm(player_pos - pos()) < 300) { pimpl->is_activated = true; }
			}
		}
		pos().x(pos().x() + v * cos(pimpl->angle));
		pos().y(pos().y() + v * sin(pimpl->angle));
		if (count > Impl::LIFE_TIME) { set_flag_off(); }
	}
	Point::~Point(void){}
	void Point::preperation(void)
	{
		ImagePool::add("../../data/img/p_item/p_item_001.png");
		ImagePool::add("../../data/img/p_item/p_item_002.png");
		ImagePool::add("../../data/img/p_item/p_item_003.png");
		ImagePool::add("../../data/img/p_item/p_item_004.png");
		ImagePool::add("../../data/img/p_item/p_item_005.png");
		ImagePool::add("../../data/img/p_item/p_item_006.png");
		Impl::img()[0] = ImagePool::get("../../data/img/p_item/p_item_001.png");
		Impl::img()[1] = ImagePool::get("../../data/img/p_item/p_item_002.png");
		Impl::img()[2] = ImagePool::get("../../data/img/p_item/p_item_003.png");
		Impl::img()[3] = ImagePool::get("../../data/img/p_item/p_item_004.png");
		Impl::img()[4] = ImagePool::get("../../data/img/p_item/p_item_005.png");
		Impl::img()[5] = ImagePool::get("../../data/img/p_item/p_item_006.png");
	}

}//namespace MyGameProject
