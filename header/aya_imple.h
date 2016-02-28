#pragma once

#include <array>
#include <memory>
#include "motion.h"
#include "mathematics.h"
#include "decl_static_image_handler.h"
#include "bullet.h"

namespace MyGameProject
{
	class Aya;
	
	struct AyaImple
	{
		const Real image_size = 30.f;

		LinearMotionOfUniformAcceleration2D<Point2D, int> motion;
		Point2D v;
		Real mag = 0.f;
		int wave = 0;
		std::array<int, 3> wave_count;

		Real dmag1 = 0.f;

		Aya& boss;
		AyaImple(Aya& _boss):boss(_boss), wave_count() {}
		void action(void);
		void draw_charactor(void) const;

		int wave2_index = 0;

		static void preperation(void);

		//image handlers
		STATIC_IMAGE_HANDLER_LIST(approach_img, 4);
		STATIC_IMAGE_HANDLER(standing_img);
		STATIC_IMAGE_HANDLER_LIST(wings_img, 3);
	};

	class CloseUpBullet : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
		static constexpr Real SPEED_0 = 1.f;
		static constexpr Real SPEED_1 = 2.f;
		static constexpr int T = 30;
		static constexpr int HITABLE_COUNT = 200;
		const int wait_time = 0;
		Real r = 6.f;
	public:
		CloseUpBullet
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			int _wait_time
		);
		virtual void draw(void) const override final;
		virtual bool is_resistant(void) const override final { return true; }
		virtual ~CloseUpBullet(void) = default;
	};
}
