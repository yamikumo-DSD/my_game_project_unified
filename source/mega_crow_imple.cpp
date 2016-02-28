//mega_crow_imple.cpp

#include "mega_crow_imple.h"
#include "mega_crow.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include "find_vacant_object.h"
#include "drawblendmode.h"
#include "color.h"
#include "concrete_bullet.h"
#include "mob_enemy.h"
#include "debug_value.h"

namespace MyGameProject
{
	using namespace boost::math::constants;

	void MegaCrowImple::draw_charactor(void) const
	{
		if (count_after_killed <= 200)
		{
			const auto count = boss.get_count();
			if (display_trails)
			{
				for (int i = 0; i != trails.size(); ++i)
				{
					gp::SetDrawBlendModeOf
						(
							gp::DrawRotaGraphF(gp::level(11), trails[i].x(), trails[i].y(), 0.5, angle_histories[i], silhouette_img(), true),
							DX_BLENDMODE_ALPHA,
							200 - 30 * i
							);
				}
			}
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), 0.5, img_angle, body_img(), true),
					DX_BLENDMODE_ALPHA,
					pal
					);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), 0.5, img_angle, tail_img()[tail_index], true),
					DX_BLENDMODE_ALPHA,
					pal
					);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), 0.5, img_angle, left_wing_img()[left_wing_index], true),
					DX_BLENDMODE_ALPHA,
					pal
					);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), 0.5, img_angle, right_wing_img()[right_wing_index], true),
					DX_BLENDMODE_ALPHA,
					pal
					);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), 0.5, img_angle, string_img()[string_index], true),
					DX_BLENDMODE_ALPHA,
					pal
					);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), 0.5, img_angle, hat_img(), true),
					DX_BLENDMODE_ALPHA,
					pal
					);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), 0.5, img_angle, neckless_img(), true),
					DX_BLENDMODE_ALPHA,
					pal
					);

		}

		if (count_after_killed > 0)
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
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), r(count_after_killed), 0, termination_effect0_img(), true),
					DX_BLENDMODE_ADD,
					p(count_after_killed)
				);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), count_after_killed / 200.f, 0.1 * count_after_killed * count_after_killed, termination_effect1_img(), true),
					DX_BLENDMODE_ADD,
					255 * (1 - count_after_killed / 400.f)
				);
			gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(11), boss.pos().x(), boss.pos().y(), count_after_killed / 200.f, 0.1 * count_after_killed, termination_effect1_img(), true),
					DX_BLENDMODE_ADD,
					255 * (1 - count_after_killed / 400.f)
				);
		}
	}

	void MegaCrowImple::preperation(void)
	{
		ImagePool::add("../../data/img/mega_crow0.png");
		ImagePool::add("../../data/img/mega_crow1.png");
		ImagePool::add("../../data/img/mega_crow2.png");
		images()[0] = ImagePool::get("../../data/img/mega_crow0.png");
		images()[1] = ImagePool::get("../../data/img/mega_crow1.png");
		images()[2] = ImagePool::get("../../data/img/mega_crow2.png");

		ImagePool::add("../../data/img/mega_wings/body.png");         body_img()                = ImagePool::get("../../data/img/mega_wings/body.png");
		ImagePool::add("../../data/img/mega_wings/hat.png");          hat_img()                 = ImagePool::get("../../data/img/mega_wings/hat.png");
		ImagePool::add("../../data/img/mega_wings/neckless.png");     neckless_img()            = ImagePool::get("../../data/img/mega_wings/neckless.png");
		ImagePool::add("../../data/img/mega_wings/silhouette3.png");  silhouette_img()          = ImagePool::get("../../data/img/mega_wings/silhouette3.png");
		ImagePool::add("../../data/img/mega_wings/left_wing0.png");   left_wing_img()[0]        = ImagePool::get("../../data/img/mega_wings/left_wing0.png");
		ImagePool::add("../../data/img/mega_wings/left_wing1.png");   left_wing_img()[1]        = ImagePool::get("../../data/img/mega_wings/left_wing1.png");
		ImagePool::add("../../data/img/mega_wings/left_wing2.png");   left_wing_img()[2]        = ImagePool::get("../../data/img/mega_wings/left_wing2.png");
		ImagePool::add("../../data/img/mega_wings/left_wing3.png");   left_wing_img()[3]        = ImagePool::get("../../data/img/mega_wings/left_wing3.png");
		ImagePool::add("../../data/img/mega_wings/right_wing0.png");  right_wing_img()[0]       = ImagePool::get("../../data/img/mega_wings/right_wing0.png");
		ImagePool::add("../../data/img/mega_wings/right_wing1.png");  right_wing_img()[1]       = ImagePool::get("../../data/img/mega_wings/right_wing1.png");
		ImagePool::add("../../data/img/mega_wings/right_wing2.png");  right_wing_img()[2]       = ImagePool::get("../../data/img/mega_wings/right_wing2.png");
		ImagePool::add("../../data/img/mega_wings/right_wing3.png");  right_wing_img()[3]       = ImagePool::get("../../data/img/mega_wings/right_wing3.png");
		ImagePool::add("../../data/img/mega_wings/tail0.png");        tail_img()[0]             = ImagePool::get("../../data/img/mega_wings/tail0.png");
		ImagePool::add("../../data/img/mega_wings/tail1.png");        tail_img()[1]             = ImagePool::get("../../data/img/mega_wings/tail1.png");
		ImagePool::add("../../data/img/mega_wings/string0.png");      string_img()[0]           = ImagePool::get("../../data/img/mega_wings/string0.png");
		ImagePool::add("../../data/img/mega_wings/string1.png");      string_img()[1]           = ImagePool::get("../../data/img/mega_wings/string1.png");
		ImagePool::add("../../data/img/mega_wings/string2.png");      string_img()[2]           = ImagePool::get("../../data/img/mega_wings/string2.png");
		ImagePool::add("../../data/img/killed_0.png");                termination_effect0_img() = ImagePool::get("../../data/img/killed_0.png");
		ImagePool::add("../../data/img/killed_1.png");                termination_effect1_img() = ImagePool::get("../../data/img/killed_1.png");
	}
}