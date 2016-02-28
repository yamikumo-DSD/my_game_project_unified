//black_wings_imple.h

#ifndef __BLACK_WINGS_IMPLE_H
#define __BLACK_WINGS_IMPLE_H

#include "mathematics.h"
#include <array>
#include "smart_ptr.h"

namespace MyGameProject
{
	class BlackWings;
	class Bullet;

	class BlackWingsImple
	{
	private:
		BlackWings& boss;
		static constexpr bool LEFT = true,RIGHT = false;
		static constexpr int WAVE_NUM = 4; //exclude wave == 0
		const std::array<int, WAVE_NUM> wave_health;
		int wave;
		int wave_count;
		std::array<Point2D, 6> trail;

		//wave1:
		void wave1(void);
		bool left_or_right;
		int to_and_fro_count;
		//wave2:
		void wave2(void);
		Real theta;
		Point2D p1,p2;
		//wave3:
		void wave3(void);
		Real phi1,phi2;
		gp::smart_ptr<Bullet> camera_frame;
		Point2D shuttered_point;
		bool white_out_flag;
		//wave4:
		void wave4(void);
		Point2D dst;

		Point2D v;
		Point2D start_p, end_p;
		int start_cnt, end_cnt;
		void set_linear_motion(bool _condition,Point2D _destination,int _term);
		void set_linear_motion(Point2D _destination, int _term){ set_linear_motion(true,_destination,_term); }
		void linear_motion(Point2D _departure, Point2D _destination, int _start_count, int _end_count);
		void linear_motion(Point2D _destination, int _start_count, int _end_count)
		{linear_motion(start_p, _destination, _start_count, _end_count);}
		void linear_motion(void);

	public:
		BlackWingsImple(BlackWings& _boss);
		void action(void);
		void draw_charactor(void) const;
	};
}

#endif