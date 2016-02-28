//title_bg_drawer.cpp

#include "title_bg_drawer.h"
#include "title.h"
#include "image_pool.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include <boost/math/constants/constants.hpp>
#include "environmental_constants.h"
#include "find_vacant_object.h"

namespace
{
	using namespace boost::math::constants;
	using namespace MyGameProject;

	template<class RealType>
	RealType theta(RealType OH, RealType distance_to_object, RealType visual_distance)
	{
		using namespace std;
		return atan(OH / (distance_to_object + visual_distance));
	}
}

MyGameProject::TitleBGDrawer::Tile::Tile(Real _angle)
	:angle(_angle),
	p(),
	distance(30000),
	count(0)
{}

MyGameProject::Real MyGameProject::TitleBGDrawer::Tile::get_angle(void) const
{
	return angle;
}

void MyGameProject::TitleBGDrawer::Tile::update(void)
{
	static constexpr Real scroll_speed = static_cast<Real>(40);
	static constexpr Real OD = 7000;
	static constexpr Real d = OD + WW_2<Real>();
	static constexpr Real k = static_cast<Real>(0.2);

	if (count == 0){ img = ImagePool::get("../../data/img/back_ground1.jpg"); }

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
	else{ set_flag_off(); }

	++count;
}

void MyGameProject::TitleBGDrawer::Tile::draw(void) const
{
	gp::DrawModiGraph
		(
		p[3].x(), p[3].y(), // upper left
		p[2].x(), p[2].y(), // upper right
		p[0].x(), p[0].y(), // lower right
		p[1].x(), p[1].y(), // lower left
		img,
		FALSE
		);
}

void MyGameProject::TitleBGDrawer::Tile::preperation(void)
{
	ImagePool::add("../../data/img/back_ground1.jpg");
}

MyGameProject::TitleBGDrawer::Tile::~Tile(void){}

namespace
{
	constexpr size_t MAX_TILE_NUM = 10;
}

MyGameProject::TitleBGDrawer::TitleBGDrawer(const Title& _title)
	:title(_title),
	count(0),
	tiles(MAX_TILE_NUM),
	shade_img(0)
{
}

void MyGameProject::TitleBGDrawer::draw(void) const
{
	for (const auto& tile : tiles)
	{
		if (tile){ tile->draw(); }
	}
	//gp::DrawGraph(0, 200, shade_img, true);
	gp::DrawGraph(0, -140, sky_img, true);
}

void MyGameProject::TitleBGDrawer::update(void)
{
	if (count == 0)
	{
		shade_img = ImagePool::get("../../data/img/shade.png"); 
		sky_img = ImagePool::get("../../data/img/sky_2.png");
	}

	if (count <= 1000) //fast-forwarding until count bacome 1000
	{
		while (count <= 1000)
		{
			if (count % 120 == 0)
			{
				*find_vacant_object(tiles) = gp::make_smart<Tile>(0);
			}

			for (auto& tile : tiles)
			{
				if (tile)
				{
					if (tile->get_flag()){ tile->update(); }
					else{ tile.reset(); }
				}
			}
			++count;
		}
	}
	else
	{
		if (count % 120 == 0)
		{
			*find_vacant_object(tiles) = gp::make_smart<Tile>(0);
		}

		for (auto& tile : tiles)
		{
			if (tile)
			{
				if (tile->get_flag()){ tile->update(); }
				else{ tile.reset(); }
			}
		}
		++count;
	}
}

void MyGameProject::TitleBGDrawer::preperation(void)
{
	Tile::preperation();
	ImagePool::add("../../data/img/sky_2.png");
	ImagePool::add("../../data/img/shade.png");
}

