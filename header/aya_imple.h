#pragma once

#include <array>
#include <memory>
#include "motion.h"
#include "mathematics.h"
#include "decl_static_image_handler.h"
#include "bullet.h"
#include "back_ground_control.h"
#include "b_234.h"

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
		std::array<int, 6> wave_count;

		Real dmag1 = 0.f;

		Aya& boss;
		AyaImple(Aya& _boss):boss(_boss), wave_count() {}
		void action(void);
		void draw_charactor(void) const;

		int wave2_index = 0;

		static void preperation(void);

		//image handlers
		STATIC_IMAGE_HANDLER_LIST(approach_img, 4)
		STATIC_IMAGE_HANDLER(standing_img)
		STATIC_IMAGE_HANDLER_LIST(wings_img, 3)
		STATIC_IMAGE_HANDLER_LIST(bombom_img, 3)
		STATIC_IMAGE_HANDLER_LIST(skirt_img, 3)
		STATIC_IMAGE_HANDLER_LIST(twist_img, 8)

		//for wave5
		Point2D wave5_dst_pos;

		BackGroundControl::OptionalSignal notify(void) const noexcept;

		Point2D pre_pos;
		const Point2D& velocity(void) const noexcept;
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

	class ComingBullet : public Bullet
	{
	private:
		STATIC_IMAGE_HANDLER(img)
		virtual void custom_updater(void) override final;
	private:
		struct CameraPos { Real x, y, z; } camera_pos;
		Real z;
	public:
		ComingBullet
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
		) noexcept;
		virtual void draw(void) const override final;
		virtual bool is_resistant(void) const override final { return true; }
		virtual ~ComingBullet(void) noexcept;
		static void preparation(void) noexcept;
	};

	class RedEdges : public Bullet
	{
	private:
		STATIC_IMAGE_HANDLER(img)
		virtual void custom_updater(void) override final;
	private:
		struct CameraPos { Real x, y, z; } camera_pos;
		Real z;
	public:
		RedEdges
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
		) noexcept;
		virtual void draw(void) const override final;
		virtual bool is_resistant(void) const override final { return true; }
		virtual ~RedEdges(void) noexcept;
		static void preparation(void) noexcept;
	};

	class ForcedCrowBullet : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
		STATIC_IMAGE_HANDLER(img)
		STATIC_IMAGE_HANDLER(trail_img)
		static constexpr std::size_t TRAIL_NUM{5};
		std::array<std::pair<Point2D, Real>, TRAIL_NUM> trails;
	public:
		ForcedCrowBullet
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			const Behavior& _behavior
		) noexcept;
		virtual void draw(void) const override final;
		virtual bool is_resistant(void) const override final { return true; }
		virtual ~ForcedCrowBullet(void) = default;
		static void preparation(void) noexcept;
		virtual void hit(void) override final {}
	};

	class BulletStorm : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
		std::vector<std::weak_ptr<Bullet>> registered_bullets;
		int count_bullet_accessible(void) const noexcept;
	public:
		BulletStorm
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
		) noexcept;
		virtual void draw(void) const override final;
		virtual bool is_resistant(void) const override final { return true; }
		virtual ~BulletStorm(void) noexcept;
	};

	class B2_ : public B234<2>
	{
	private:
		std::function<Point2D(void)> v_base;
		STATIC_IMAGE_HANDLER_LIST(img_handle, 3)
	public:
		B2_
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior,
			decltype(v_base) v_base
		) noexcept;
		virtual void draw(void) const override final;
		virtual ~B2_(void) noexcept = default;
		static void preparation(void) noexcept;
	};

	class B3_ : public B234<3>
	{
	private:
		std::function<Point2D(void)> v_base;
		STATIC_IMAGE_HANDLER_LIST(img_handle, 3)
	public:
		B3_
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior,
			decltype(v_base) v_base
		) noexcept;
		virtual void draw(void) const override final;
		virtual ~B3_(void) noexcept = default;
		static void preparation(void) noexcept;
	};
}
