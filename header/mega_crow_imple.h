#pragma once

#include <array>
#include <memory>
#include "object.h"
#include "motion.h"

namespace MyGameProject
{
	class MegaCrow;
	
	struct MegaCrowImple
	{
		static std::array<int, 10>& images(void) { static std::array<int, 10> images; return images; }
		MegaCrow& boss;
		MegaCrowImple(MegaCrow& _boss):boss(_boss) {}
		void action(void);
		void draw_charactor(void) const;

		LinearMotionOfUniformAcceleration2D<Point2D, int> motion0;
		Point2D v;
		Real pal = 255.f;

		static void preperation(void);

		std::array<Point2D, 5> trails;
		std::array<Real, 5> angle_histories;
		bool display_trails = false;

		static int& body_img(void) { static int i = 0; return i; }
		static int& hat_img(void) { static int i = 0; return i; }
		static int& neckless_img(void) { static int i = 0; return i; }
		static int& silhouette_img(void) { static int i = 0; return i; }
		static std::array<int, 2>& tail_img(void) { static std::array<int, 2> i; return i; }       int tail_index = 0;
		static std::array<int, 3>& string_img(void) { static std::array<int, 3> i; return i; }     int string_index = 0;
		static std::array<int, 4>& left_wing_img(void) { static std::array<int, 4> i; return i; }  int left_wing_index = 0;
		static std::array<int, 4>& right_wing_img(void) { static std::array<int, 4> i; return i; } int right_wing_index = 0;
		static int& termination_effect0_img(void) { static int i = 0; return i; }
		static int& termination_effect1_img(void) { static int i = 0; return i; }

		Real img_angle = 0.f;

		int count_after_killed = 0;
	};
}