//title_bg_drawer.h

#pragma once

#include "object.h"
#include "smart_ptr.h"

namespace MyGameProject
{
	class Title;

	class TitleBGDrawer
	{
	private:
		class Tile : public DynamicObject
		{
		private:
			int img;
			int count;
			Real angle;
			Real distance;
			std::array<Point2D, 4> p;
		public:
			explicit Tile(Real _angle);
			virtual void update(void) override final;
			virtual void draw(void) const override final;
			Real get_angle(void) const;
			static void preperation(void);
			virtual ~Tile(void);
		};
	private:
		const Title& title;
		int count;
		std::vector<gp::smart_ptr<Tile>> tiles;
		int shade_img;
		int sky_img;
	public:
		explicit TitleBGDrawer(const Title& _title);
		void draw(void) const;
		void update(void);
		static void preperation(void);
	};
}