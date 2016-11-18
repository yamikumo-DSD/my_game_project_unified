//emerge_from_cloud.cpp

#define _AFX_SECURE_NO_WARNINGS
#define D_SCL_SECURE_NO_WARNINGS

#include "emerge_from_cloud.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include "find_vacant_object.h"
#include "diffuse_point.h"
#include "add_get.h"
#include "se_manager.h"
#include "border_check.h"
#include "debug_value.h"

namespace MyGameProject
{
	constexpr int MAX_HP = 70;
	constexpr int TURNING_POINT = 180;

	EmergeFromCloud::EmergeFromCloud
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
			)
		:MobEnemy(_bullets, _se_manager, _order, MAX_HP, Class::SMALL, _player, ShapeElement::Null())
	{
	}

	int EmergeFromCloud::released_p_item_num(void) const
	{
		return 6;
	}

	void EmergeFromCloud::draw(void) const
	{
		const auto count{ get_count() };
			static auto switcher
			{
				[](auto _c)
				{
					if (_c < 5) { return 0; }
					else if (_c >= 5 && _c < 10) { return 1; }
					else if (_c >= 10 && _c < 15) { return 2; }
					else { return 1; }
				}
			};
			if (velocity().x() > 3)
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraph
					(
						gp::level(11), pos().x(), pos().y(), 1.3, angle(),
						img() [3],
						TRUE
					),
					DX_BLENDMODE_ALPHA,
					2 * (count - TURNING_POINT + 60)
				);
			}
			else if (velocity().x() < -3)
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraph
					(
						gp::level(11), pos().x(), pos().y(), 1.3, angle(),
						img() [4],
						TRUE
					),
					DX_BLENDMODE_ALPHA,
					2 * (count - TURNING_POINT + 60)
				);
			}
			else
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraph
					(
						gp::level(11), pos().x(), pos().y(), 1.3, angle(),
						img() [switcher(count % 20)],
						TRUE
					),
					DX_BLENDMODE_ALPHA,
					2 * (count - TURNING_POINT + 60)
				);
			}

		for (const auto& following : followings)
		{
			if (following && following->get_flag())
			{
				following->draw();
			}
		}
	}

	void EmergeFromCloud::accessory_custom_updater(void)
	{
		const auto x = pos().x(), y = pos().y();
		const auto count{get_count()};
		if (!is_within_window(pos(), 50)) { set_flag_off(); }

		if (count <= TURNING_POINT)
		{
			for (int i = 0; i != 2; ++i)
			{
				*find_vacant_object(followings) = gp::make_smart<Tail>(diffuse_point_rect(pos() - Point2D(30, 30), pos() + Point2D(30, 30)), 10);
			}
		}

		if (count == TURNING_POINT)
		{
			area().get_shape() = ShapeElement::Circle(20);
			get_se_manager_ref().erect_play_flag_of("../../data/sound/crow.wav");
		}

		for (auto& following : followings)
		{
			if (following)
			{
				if (following->get_flag()) { following->update(); }
				else { following.reset(); }
			}
		}
	}

	void EmergeFromCloud::preperation(void)
	{
		ImagePool::add_multi("../../data/img/crow.png", 3, 3, 1, 400, 400);
		//ImagePool::add("../../data/img/crow0.png");
		//img()[0] = ImagePool::get("../../data/img/crow0.png");
		//ImagePool::add("../../data/img/crow1.png");
		//img()[1] = ImagePool::get("../../data/img/crow1.png");
		img()[0] = add_get("../../data/img/crow/crow_000.png");
		img()[1] = add_get("../../data/img/crow/crow_001.png");
		img()[2] = add_get("../../data/img/crow/crow_002.png");
		img()[3] = add_get("../../data/img/crow/crow_003.png");
		img()[4] = add_get("../../data/img/crow/crow_004.png");

		Tail::preperation();
	}

	EmergeFromCloud::~EmergeFromCloud(void) {}

	EmergeFromCloud::Tail::Tail(const Point2D& _init_pos, int _tc)
		:count(0),
		termination_count(_tc)
	{
		pos() = _init_pos;
	}

	void EmergeFromCloud::Tail::update(void)
	{
		if (count == 0) { img = ImagePool::get("../../data/img/particle.png"); }
		if (count == termination_count) { set_flag_off(); }
		pos().y(pos().y() + 5);
		++count;
	}

	void EmergeFromCloud::Tail::draw(void) const
	{
		gp::DrawRotaGraph(gp::level(9), pos().x(), pos().y(), 2 - 2 * count / termination_count, 0, img, true);
	}

	void EmergeFromCloud::Tail::preperation(void)
	{
		ImagePool::add("../../data/img/particle.png");
	}

	EmergeFromCloud::Tail::~Tail(void) {}
}