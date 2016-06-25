#pragma once

#include "object.h"
#include <array>
#include "mathematics.h"
#include "decl_static_image_handler.h"
#include "image_pool.h"
#include <dxlib.h>
#include "draw_order.h"

namespace MyGameProject
{
	class KoyoTexture : public DynamicObject
	{
	public:
		struct CameraPos
		{
			Real x, y, z;
		};
		enum class Side {RIGHT, LEFT, MIDDLE};
	private:
		//COORDINATE PREFIXES
		//x:horizontal right and left
		//y:vertical position
		//z:horizontal front and rear

		template<typename Xa, typename Za, typename Xb, typename Zb>
		static constexpr auto x_intercept(Xa&& _xa, Za&& _za, Xb&& _xb, Zb&& _zb)
		{
			return std::forward<Xa>(_xa) -
				std::forward<Za>(_za) *
				(std::forward<Xb>(_xb) - std::forward<Xa>(_xa)) /
				(std::forward<Zb>(_zb) - std::forward<Za>(_za));
		}

		static CameraPos camera_pos; //shared with all texture object
		static constexpr auto TEX_HIGHT = static_cast<Real>(1000);
		static constexpr auto TEX_WIDTH = static_cast<Real>(3000);
		static constexpr auto SCROLL_SPEED = static_cast<Real>(35);
		static constexpr auto WIN_WIDTH = 640;
		static constexpr auto RADIUS_OF_SPHERE = static_cast<Real>(100'000); //Radius of virtual earth.
		static Real y; //y coord is shared.
		Real z; //z coord is not shared.
		const Real x; //x coord is not shared.

//          | /                 
//          |/    z-TEX_HIGHT         z 
//   -------+------------------------------>z coord
//         /|        :                :   
//        / |        :                :     
//          |        :                :     
//          |        :   /            :   /  
//          |        :  /             :  /   
//          |        : /              : /    
//         y|........._________________ <- texture(from x axis)
//          |                       
//          |                       
//          V                                  
//       y coord                                 

		int count{ 0 };
		static int img;
		STATIC_IMAGE_HANDLER(right_side_img)
		STATIC_IMAGE_HANDLER(left_side_img)
		Side side;
	public:
		//Special functions.
		template<typename RealType1, typename RealType2>
		KoyoTexture(RealType1&& _constant_x_coord, RealType2&& _initial_z_coord) noexcept
			: z(std::forward<RealType2>(_initial_z_coord)),
			x(std::forward<RealType1>(_constant_x_coord)),
			side(Side::MIDDLE){}
		template<typename RealType1, typename RealType2>
		KoyoTexture(RealType1&& _constant_x_coord, RealType2&& _initial_z_coord, Side _side) noexcept
			: z(std::forward<RealType2>(_initial_z_coord)),
			x(std::forward<RealType1>(_constant_x_coord)),
			side(_side){}
		KoyoTexture(const KoyoTexture& _tex) noexcept = default;
		KoyoTexture& operator=(const KoyoTexture& _tex) noexcept { return *this = _tex; }
		KoyoTexture(KoyoTexture&& _tex) noexcept = default;
		KoyoTexture& operator=(KoyoTexture&& _tex) noexcept = default;

		static constexpr auto interval(void) noexcept { return static_cast<int>(TEX_HIGHT / SCROLL_SPEED); }
		static constexpr auto hight(void) noexcept { return TEX_HIGHT; }
		static constexpr auto width(void) noexcept { return TEX_WIDTH; }
		static constexpr auto scroll_speed(void) noexcept { return SCROLL_SPEED; }
		static auto z_horizon(void) noexcept { return std::sqrt(2 * RADIUS_OF_SPHERE * (y - camera_pos.y)); }
		template<typename RealType1, typename RealType2, typename RealType3>
		static void set_camera_pos(RealType1&& _x, RealType2&& _y, RealType3&& _z) noexcept
		{
			camera_pos.x = std::forward<RealType1>(_x);
			camera_pos.y = std::forward<RealType2>(_y);
			camera_pos.z = std::forward<RealType3>(_z);
		}
		static const CameraPos& get_camera_pos(void) { return camera_pos; }
		//Get y coordinate of plane on which texture is.
		static auto get_plane_y_coord(void) noexcept { return y; }
		//Set y coordinate of plane on which texture is.
		template<typename RealType>
		static void set_plane_y_coord(RealType&& _y) noexcept { y = std::forward<RealType>(_y); }
		virtual void update(void) override final
		{
			if (z < 0) { set_flag_off(); }
			z -= SCROLL_SPEED;
			++count;
		}
		virtual void draw(void) const override final
		{
			const auto x1
			{
				z < z_horizon() ?
				KoyoTexture::x_intercept(camera_pos.x, camera_pos.z, x, z)
				:
				KoyoTexture::x_intercept(camera_pos.x, camera_pos.z, x, z_horizon())
			};
			const auto x2{ z - TEX_HIGHT > 0 ? KoyoTexture::x_intercept(camera_pos.x, camera_pos.z, x, z - TEX_HIGHT) : x };
			const auto x3
			{
				z < z_horizon() ?
				KoyoTexture::x_intercept(camera_pos.x, camera_pos.z, x - TEX_WIDTH, z)
				:
				KoyoTexture::x_intercept(camera_pos.x, camera_pos.z, x - TEX_WIDTH, z_horizon())
			};
			const auto x4{ z - TEX_HIGHT > 0 ? KoyoTexture::x_intercept(camera_pos.x, camera_pos.z, x - TEX_WIDTH, z - TEX_HIGHT) : x - TEX_WIDTH };
			const auto y1
			{
				z < z_horizon() ?
				camera_pos.y - camera_pos.z * (camera_pos.y - y) / (camera_pos.z - z)
				:
				camera_pos.y - camera_pos.z * (camera_pos.y - y) / (camera_pos.z - z_horizon())
			};
			const auto y2
			{
				z - TEX_HIGHT > 0 ?
				camera_pos.y - camera_pos.z * (camera_pos.y - y) / (camera_pos.z - z + TEX_HIGHT)
				:
				y
			};
			using P = Point2D;
			const P LEFT_UPPER{ static_cast<Real>(x3), static_cast<Real>(y1) };
			const P RIGHT_UPPER{ static_cast<Real>(x1), static_cast<Real>(y1) };
			const P LEFT_BOTTOM{ static_cast<Real>(x4), static_cast<Real>(y2) };
			const P RIGHT_BOTTOM{ static_cast<Real>(x2), static_cast<Real>(y2) };
			int enable_img_handle{0};
			switch (side)
			{
				case Side::MIDDLE: enable_img_handle = img;  break;
				case Side::RIGHT: enable_img_handle = right_side_img();  break;
				case Side::LEFT: enable_img_handle = left_side_img();  break;
			}
			if (z > 0)
			{
				if (z - TEX_HIGHT > 0)
				{
					if (z > z_horizon())
					{
						int x_size{ 0 }, y_size{ 0 };
						DxLib::GetGraphSize(enable_img_handle, &x_size, &y_size);
						const auto drawable_zone_of_tex{ gp::DerivationGraph(0, y_size * (z - z_horizon()) / TEX_HIGHT, x_size, y_size * (1 - (z - z_horizon()) / TEX_HIGHT), enable_img_handle) };
						gp::DrawModiGraph
							(
								gp::level(1),
								static_cast<int>(LEFT_UPPER.x()), static_cast<int>(LEFT_UPPER.y()), // upper left
								static_cast<int>(RIGHT_UPPER.x()), static_cast<int>(RIGHT_UPPER.y()), // upper right
								static_cast<int>(RIGHT_BOTTOM.x()), static_cast<int>(RIGHT_BOTTOM.y()), // lower right
								static_cast<int>(LEFT_BOTTOM.x()), static_cast<int>(LEFT_BOTTOM.y()), // lower left
								drawable_zone_of_tex,
								TRUE
								);

					}
					else
					{
						gp::DrawModiGraph
							(
								gp::level(1),
								static_cast<int>(LEFT_UPPER.x()), static_cast<int>(LEFT_UPPER.y()), // upper left
								static_cast<int>(RIGHT_UPPER.x()), static_cast<int>(RIGHT_UPPER.y()), // upper right
								static_cast<int>(RIGHT_BOTTOM.x()), static_cast<int>(RIGHT_BOTTOM.y()), // lower right
								static_cast<int>(LEFT_BOTTOM.x()), static_cast<int>(LEFT_BOTTOM.y()), // lower left
								enable_img_handle,
								TRUE
								);
					}
				}
				else
				{
					int x_size{ 0 }, y_size{ 0 };
					DxLib::GetGraphSize(img, &x_size, &y_size);
					const auto drawable_zone_of_tex{ gp::DerivationGraph(0, 0, x_size, y_size * z / TEX_HIGHT, enable_img_handle) };
					gp::DrawModiGraph
						(
								gp::level(1),
							static_cast<int>(LEFT_UPPER.x()), static_cast<int>(LEFT_UPPER.y()), // upper left
							static_cast<int>(RIGHT_UPPER.x()), static_cast<int>(RIGHT_UPPER.y()), // upper right
							static_cast<int>(RIGHT_BOTTOM.x()), static_cast<int>(RIGHT_BOTTOM.y()), // lower right
							static_cast<int>(LEFT_BOTTOM.x()), static_cast<int>(LEFT_BOTTOM.y()), // lower left
							drawable_zone_of_tex,
							TRUE
							);
				}
			}
		}
		static void preparation(void)
		{
			ImagePool::add("../../data/img/koyo.png");
			img = ImagePool::get("../../data/img/koyo.png");
			ImagePool::add("../../data/img/koyo_right.png");
			right_side_img() = ImagePool::get("../../data/img/koyo_right.png");
			ImagePool::add("../../data/img/koyo_left.png");
			left_side_img() = ImagePool::get("../../data/img/koyo_left.png");
		}
		virtual ~KoyoTexture(void) = default;
	};
}
