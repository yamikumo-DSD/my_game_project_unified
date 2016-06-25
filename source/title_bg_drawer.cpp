//title_bg_drawer.cpp

#include "title_bg_drawer.h"
#include "title.h"
#include "image_pool.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include <boost/math/constants/constants.hpp>
#include "environmental_constants.h"
#include "find_vacant_object.h"
#include "debug.h"
#include "dxsstream.h"

namespace MyGameProject
{
	using namespace boost::math::constants;

	template<class RealType>
	RealType theta(RealType OH, RealType distance_to_object, RealType visual_distance)
	{
		using namespace std;
		return atan(OH / (distance_to_object + visual_distance));
	}

	TitleBGDrawer::Tile::Tile(Real _angle)
		:angle(_angle),
		p(),
		distance(30000),
		count(0)
	{}

	Real TitleBGDrawer::Tile::get_angle(void) const
	{
		return angle;
	}

	void TitleBGDrawer::Tile::update(void)
	{
		static constexpr Real scroll_speed = static_cast<Real>(40);
		static constexpr Real OD = 7000;
		static constexpr Real d = OD + WW_2<Real>();
		static constexpr Real k = static_cast<Real>(0.2);

		if (count == 0) { img = ImagePool::get("../../data/img/back_ground1.jpg"); }

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
		//else{ set_flag_off(); }

		++count;
	} //TitleBGDrawer::Tile::update

	void TitleBGDrawer::Tile::draw(void) const
	{
		//	gp::DrawModiGraph
		//		(
		//		p[3].x(), p[3].y(), // upper left
		//		p[2].x(), p[2].y(), // upper right
		//		p[0].x(), p[0].y(), // lower right
		//		p[1].x(), p[1].y(), // lower left
		//		img,
		//		FALSE
		//		);
		using P = Point2D;
		constexpr auto SCROLL_SPEED{ static_cast<Real>(10) };
		constexpr auto WIN_WIDTH{ static_cast<Real>(640) };
		constexpr auto TEX_WIDTH{ static_cast<Real>(2000) };
		constexpr auto TEX_HIGHT{ static_cast<Real>(1000) };
		int mouse_x{ 0 }, mouse_y{ 0 };
		DxLib::GetMousePoint(&mouse_x, &mouse_y);
		auto y_cam{ static_cast<Real>(mouse_y) };
		auto z_cam{ static_cast<Real>(-mouse_x) };
		auto y_tex{ static_cast<Real>(300) }; //plane
		auto z_tex{ static_cast<Real>(2000 - SCROLL_SPEED * count) };
		const auto x1{ 0.5 * (WIN_WIDTH - z_cam * TEX_WIDTH / (z_tex - z_cam)) };
		const auto x2
		{
			z_tex - TEX_HIGHT > 0 ?
			0.5 * (WIN_WIDTH - z_cam * TEX_WIDTH / (z_tex - z_cam - TEX_HIGHT))
			:
			(WIN_WIDTH + TEX_WIDTH) / 2
		};
		const auto x3{ 0.5 * (WIN_WIDTH + z_cam * TEX_WIDTH / (z_tex - z_cam)) };
		const auto x4
		{
			z_tex - TEX_HIGHT > 0 ?
			0.5 * (WIN_WIDTH + z_cam * TEX_WIDTH / (z_tex - z_cam - TEX_HIGHT))
			:
			(WIN_WIDTH - TEX_WIDTH) / 2
		};
		const auto y1{ y_cam - z_cam * (y_cam - y_tex) / (z_cam - z_tex) };
		const auto y2
		{
			z_tex - TEX_HIGHT > 0 ?
			y_cam - z_cam * (y_cam - y_tex) / (z_cam - z_tex + TEX_HIGHT)
			:
			y_tex
		};
		const P LEFT_UPPER{ static_cast<Real>(x3), static_cast<Real>(y1) };
		const P RIGHT_UPPER{ static_cast<Real>(x1), static_cast<Real>(y1) };
		const P LEFT_BOTTOM{ static_cast<Real>(x4), static_cast<Real>(y2) };
		const P RIGHT_BOTTOM{ static_cast<Real>(x2), static_cast<Real>(y2) };
		if (z_tex > 0)
		{
			if (z_tex - TEX_HIGHT > 0)
			{
				gp::DrawModiGraph
					(
						LEFT_UPPER.x(), LEFT_UPPER.y(), // upper left
						RIGHT_UPPER.x(), RIGHT_UPPER.y(), // upper right
						RIGHT_BOTTOM.x(), RIGHT_BOTTOM.y(), // lower right
						LEFT_BOTTOM.x(), LEFT_BOTTOM.y(), // lower left
						img,
						TRUE
						);
			}
			else
			{
				const auto drawable_zone_of_tex{ gp::MakeScreen(TEX_WIDTH, static_cast<int>(z_tex), 1) };
				gp::SetDrawScreenOf
					(
						gp::DrawModiGraph
						(
							0, 0,
							TEX_WIDTH, 0,
							TEX_WIDTH, TEX_HIGHT,
							0, TEX_HIGHT,
							img, TRUE
							),
						drawable_zone_of_tex
						);
				gp::DrawModiGraph
					(
						LEFT_UPPER.x(), LEFT_UPPER.y(), // upper left
						RIGHT_UPPER.x(), RIGHT_UPPER.y(), // upper right
						RIGHT_BOTTOM.x(), RIGHT_BOTTOM.y(), // lower right
						LEFT_BOTTOM.x(), LEFT_BOTTOM.y(), // lower left
						drawable_zone_of_tex,
						TRUE
						);
			}
		}
		gp::dxout(150, 150) << z_tex << gp::end;
	} //TitleBGDrawer::Tile::draw

	void TitleBGDrawer::Tile::preperation(void)
	{
		ImagePool::add("../../data/img/back_ground1.jpg");
	}

	TitleBGDrawer::Tile::~Tile(void) {}

	constexpr size_t MAX_TILE_NUM = 10;

	TitleBGDrawer::TitleBGDrawer(const Title& _title)
		:title(_title),
		count(0),
		tiles(MAX_TILE_NUM),
		shade_img(0)
	{
	}

	void TitleBGDrawer::draw(void) const
	{
		for (const auto& tile : tiles)
		{
			if (tile) { tile->draw(); }
		}
		gp::DrawGraph(0, -140, sky_img, true);
	}

	void TitleBGDrawer::update(void)
	{
		if (count == 0)
		{
			shade_img = ImagePool::get("../../data/img/shade.png");
			sky_img = ImagePool::get("../../data/img/sky_2.png");
		}

		//	if (count <= 1000) //fast-forwarding until count bacome 1000
		//	{
		//		while (count <= 1000)
		//		{
		//			if (count % 120 == 0)
		//			{
		//				*find_vacant_object(tiles) = gp::make_smart<Tile>(0);
		//			}

		//			for (auto& tile : tiles)
		//			{
		//				if (tile)
		//				{
		//					if (tile->get_flag()){ tile->update(); }
		//					else{ tile.reset(); }
		//				}
		//			}
		//			++count;
		//		}
		//	}
		//	else
		//	{
		//		if (count % 120 == 0)
		//		{
		//			*find_vacant_object(tiles) = gp::make_smart<Tile>(0);
		//		}

		//		for (auto& tile : tiles)
		//		{
		//			if (tile)
		//			{
		//				if (tile->get_flag()){ tile->update(); }
		//				else{ tile.reset(); }
		//			}
		//		}
		//		++count;
		//	}
		if (count == 10)
		{
			*find_vacant_object(tiles) = gp::make_smart<Tile>(0);
		}
		for (auto& tile : tiles)
		{
			if (tile)
			{
				if (tile->get_flag()) { tile->update(); }
				else { tile.reset(); }
			}
		}
		++count;
	} //TitleBGDrawer::update

	void TitleBGDrawer::preperation(void)
	{
		Tile::preperation();
		ImagePool::add("../../data/img/sky_2.png");
		ImagePool::add("../../data/img/shade.png");
	}

} //namespace MyGameProject
