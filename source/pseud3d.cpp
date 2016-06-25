//pseud3d.cpp

#include "pseud3d.h"
#include <cmath>
#include "mathematics.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "environmental_constants.h"
#include <boost/math/constants/constants.hpp>
#include "find_vacant_object.h"
#include "image_pool.h"

namespace MyGameProject
{
	using namespace boost::math::constants;

	template<class RealType>
	RealType theta(RealType OH, RealType distance_to_object, RealType visual_distance)
	{
		using namespace std;
		return atan(OH / (distance_to_object + visual_distance));
	}

	Pseud3DTile::Pseud3DTile(Real _angle)
		:angle(_angle),
		p(),
		distance(3400)
	{}

	Real Pseud3DTile::get_angle(void) const
	{
		return angle;
	}

	void Pseud3DTile::update(void)
	{
		static constexpr Real scroll_speed = static_cast<Real>(12);
		static constexpr Real OD = 3000;
		static constexpr Real d = OD + WW_2<Real>();
		static constexpr Real k = 0.5;

		angle = theta(WH<Real>(), distance, OD);

		// lower left
		p[1].x((WW_2<Real>() * k - d * tan(angle)) / (k - tan(angle)));
		p[1].y(-k * p[1].x() + WW_2<Real>() * k);
		// upper right
		p[2].x((WW_2<Real>() * k + d * tan(angle)) / (k + tan(angle)));
		p[2].y(k * p[2].x() - WW_2<Real>() * k);
		// lower right
		p[0].x(WW<Real>() - p[1].x());
		p[0].y(p[1].y());
		// upper left
		p[3].x(WW<Real>() - p[2].x());
		p[3].y(p[2].y());

		if (p[3].y() < WH<Real>())distance -= scroll_speed;
		else { set_flag_off(); }
	}

	void Pseud3DTile::draw(void) const
	{
		gp::DrawModiGraph
			(
				p[3].x(), p[3].y(), // upper left
				p[2].x(), p[2].y(), // upper right
				p[0].x(), p[0].y(), // lower right
				p[1].x(), p[1].y(), // lower left
				ImagePool::get("../../data/img/sky_00009.jpg"),
				FALSE
				);
	}

	void Pseud3DTile::preperation(void)
	{
		ImagePool::add("../../data/img/sky_00009.jpg");
	}

	Pseud3DTile::~Pseud3DTile(void) {}


	namespace
	{
		constexpr size_t MAX_TILE_NUM = 100;
	}

	Pseud3D::Pseud3D(void)
		:tiles(MAX_TILE_NUM)
	{}

	void Pseud3D::custom_updater(void)
	{
		if (get_count() % 110 == 0)
		{
			*find_vacant_object(tiles) = gp::make_smart<Pseud3DTile>(0);
		}

		for (auto& tile : tiles)
		{
			if (tile)
			{
				if (tile->get_flag()) { tile->update(); }
				else { tile.reset(); }
			}
		}
	}


	void Pseud3D::draw_rear(void) const
	{
		for (const auto& tile : tiles)
		{
			if (tile) { tile->draw(); }
		}
		gp::DrawGraph(0, 0, ImagePool::get("../../data/img/sky.png"), TRUE);
		//gp::DrawGraph(-300, 0, ImagePool::get("../../data/img/sun_burst.png"), TRUE);
		gp::DrawGraph(0, WH<int>() - 100, ImagePool::get("../../data/img/window_low_border.png"), TRUE);
	}

	void Pseud3D::draw_front(void) const
	{
		gp::DrawGraph(-300, 0, ImagePool::get("../../data/img/sun_burst.png"), TRUE);
	}

	void Pseud3D::preperation(void)
	{
		Pseud3DTile::preperation();
		ImagePool::add("../../data/img/sky.png");
		ImagePool::add("../../data/img/sun_burst.png");
		ImagePool::add("../../data/img/window_low_border.png");
	}

	Pseud3D::~Pseud3D(void) {}

	ThroughCloud::ThroughCloud(void)
		:y_1_1(0),
		y_1_2(0),
		y_2_1(0),
		y_2_2(0)
	{}

	void ThroughCloud::custom_updater(void)
	{
		static constexpr int REAR_SCROLL_SPEED = 20;
		static constexpr int FRONT_SCROLL_SPEED = 40;
		if (get_count() == 0)
		{
			img_1 = ImagePool::get("../../data/img/through_cloud1.png");
			img_2 = ImagePool::get("../../data/img/through_cloud2.png");
		}

		y_1_1 = (y_1_1 + REAR_SCROLL_SPEED) % WH<int>();
		y_2_1 = (y_2_1 + FRONT_SCROLL_SPEED) % WH<int>();
	}

	void ThroughCloud::draw_rear(void) const
	{
		using namespace gp;
		gp::SetDrawBlendModeOf
		(
			gp::DrawGraph(gp::level(2), 0, y_1_1, img_1, false) >>
			gp::DrawGraph(gp::level(2), 0, y_1_1 - WH<int>(), img_1, false),
			DX_BLENDMODE_ALPHA,
			get_count() * (255.f / 400) < 255? get_count() * (255.f / 400) : 255
		);
	}

	void ThroughCloud::draw_front(void) const
	{
		using namespace gp;
		for (int i = 0; i != 2; ++i)
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawGraph(gp::level(22), 0, y_2_1, img_2, true) >>
				gp::DrawGraph(gp::level(22), 0, y_2_1 - WH<int>(), img_2, true),
				DX_BLENDMODE_ALPHA,
				get_count() * (255.f / 800) < 255? get_count() * (255.f / 800) : 255
			);
		}
	}

	void ThroughCloud::preperation(void)
	{
		ImagePool::add("../../data/img/through_cloud1.png");
		ImagePool::add("../../data/img/through_cloud2.png");
	}

	ThroughCloud::~ThroughCloud(void) {}
} //namespace MyGameProject
