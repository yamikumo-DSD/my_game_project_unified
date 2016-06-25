//sky.cpp

#include "sky.h"
#include "object.h"
#include <array>
#include "mathematics.h"
#include "decl_static_image_handler.h"
#include "image_pool.h"
#include "draw_order.h"
#include <dxlib.h>
#include "find_vacant_object.h"
#include "debug.h"
#include "player.h"

namespace MyGameProject
{
	class Texture : public DynamicObject
	{
	public:
		struct CameraPos
		{
			Real x, y, z;
		};
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
		static constexpr auto SCROLL_SPEED = static_cast<Real>(15);
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
	public:
		//Special functions.
		template<typename RealType1, typename RealType2>
		Texture(RealType1&& _constant_x_coord, RealType2&& _initial_z_coord) noexcept 
			: z(std::forward<RealType2>(_initial_z_coord)),
			x(std::forward<RealType1>(_constant_x_coord)){}
		Texture(const Texture& _tex) noexcept = default;
		Texture& operator=(const Texture& _tex) noexcept { return *this = _tex; }
		Texture(Texture&& _tex) noexcept = default;
		Texture& operator=(Texture&& _tex) noexcept = default;

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
				z < z_horizon()?
				Texture::x_intercept(camera_pos.x, camera_pos.z, x, z) 
				:
				Texture::x_intercept(camera_pos.x, camera_pos.z, x, z_horizon()) 
			};
			const auto x2{ z - TEX_HIGHT > 0? Texture::x_intercept(camera_pos.x, camera_pos.z, x, z - TEX_HIGHT ) : x };
			const auto x3
			{
				z < z_horizon()?
				Texture::x_intercept(camera_pos.x, camera_pos.z, x - TEX_WIDTH, z) 
				:
				Texture::x_intercept(camera_pos.x, camera_pos.z, x - TEX_WIDTH, z_horizon()) 
			};
			const auto x4{ z - TEX_HIGHT > 0? Texture::x_intercept(camera_pos.x, camera_pos.z, x - TEX_WIDTH, z - TEX_HIGHT ) : x - TEX_WIDTH};
			const auto y1
			{
				z < z_horizon()?
				camera_pos.y - camera_pos.z * (camera_pos.y - y) / (camera_pos.z - z)
				:
				camera_pos.y - camera_pos.z * (camera_pos.y - y) / (camera_pos.z - z_horizon())
			};
			const auto y2
			{
				z - TEX_HIGHT > 0?
				camera_pos.y - camera_pos.z * (camera_pos.y - y) / (camera_pos.z - z + TEX_HIGHT)
				:
				y
			};
			using P = Point2D;
			const P LEFT_UPPER  {static_cast<Real>(x3), static_cast<Real>(y1)};
			const P RIGHT_UPPER {static_cast<Real>(x1), static_cast<Real>(y1)};
			const P LEFT_BOTTOM {static_cast<Real>(x4), static_cast<Real>(y2)};
			const P RIGHT_BOTTOM{static_cast<Real>(x2), static_cast<Real>(y2)};
			if (z > 0)
			{
				if (z - TEX_HIGHT > 0)
				{
					if (z > z_horizon())
					{
						int x_size{ 0 }, y_size{ 0 };
						DxLib::GetGraphSize(img, &x_size, &y_size);
						const auto drawable_zone_of_tex{gp::DerivationGraph(0, y_size * (z - z_horizon()) / TEX_HIGHT, x_size, y_size * (1 - (z - z_horizon()) / TEX_HIGHT), img)};
						gp::DrawModiGraph
						(
							static_cast<int>(LEFT_UPPER.x()  ), static_cast<int>(LEFT_UPPER.y()  ) , // upper left
							static_cast<int>(RIGHT_UPPER.x() ), static_cast<int>(RIGHT_UPPER.y() ) , // upper right
							static_cast<int>(RIGHT_BOTTOM.x()), static_cast<int>(RIGHT_BOTTOM.y()) , // lower right
							static_cast<int>(LEFT_BOTTOM.x() ), static_cast<int>(LEFT_BOTTOM.y() ) , // lower left
							drawable_zone_of_tex,
							TRUE
						);

					}
					else
					{
						gp::DrawModiGraph
							(
								static_cast<int>(LEFT_UPPER.x()), static_cast<int>(LEFT_UPPER.y()), // upper left
								static_cast<int>(RIGHT_UPPER.x()), static_cast<int>(RIGHT_UPPER.y()), // upper right
								static_cast<int>(RIGHT_BOTTOM.x()), static_cast<int>(RIGHT_BOTTOM.y()), // lower right
								static_cast<int>(LEFT_BOTTOM.x()), static_cast<int>(LEFT_BOTTOM.y()), // lower left
								img,
								TRUE
							);
					}
				}
				else
				{
					int x_size{ 0 }, y_size{ 0 };
					DxLib::GetGraphSize(img, &x_size, &y_size);
					const auto drawable_zone_of_tex{gp::DerivationGraph(0, 0, x_size, y_size * z / TEX_HIGHT, img)};
					gp::DrawModiGraph
					(
						static_cast<int>(LEFT_UPPER.x()  ), static_cast<int>(LEFT_UPPER.y()  ) , // upper left
						static_cast<int>(RIGHT_UPPER.x() ), static_cast<int>(RIGHT_UPPER.y() ) , // upper right
						static_cast<int>(RIGHT_BOTTOM.x()), static_cast<int>(RIGHT_BOTTOM.y()) , // lower right
						static_cast<int>(LEFT_BOTTOM.x() ), static_cast<int>(LEFT_BOTTOM.y() ) , // lower left
						drawable_zone_of_tex,
						TRUE
					);
				}
			}
		}
		static void preparation(void)
		{
			ImagePool::add("../../data/img/sky_00009.jpg");
			img = ImagePool::get("../../data/img/sky_00009.jpg");
		}
		virtual ~Texture(void) = default;
	};

	int Texture::img;
	Texture::CameraPos Texture::camera_pos;
	Real Texture::y;

	struct Sky::Impl
	{
		static constexpr auto TEXTURE_NUM = 256;
		std::array<std::unique_ptr<Texture>, TEXTURE_NUM> textures;

		//special functions
		Impl(void) noexcept = default;
		Impl(Impl&& _impl) = default;
		Impl(const Impl& _impl) noexcept
		{ for (int i = 0; i != TEXTURE_NUM; ++i) { *textures[i] = *_impl.textures[i]; } }
		Impl& operator=(const Impl& _lhs) noexcept
		{ for (int i = 0; i != TEXTURE_NUM; ++i) { *textures[i] = *_lhs.textures[i]; } }

		static int sky_2_img;
	};

	int Sky::Impl::sky_2_img;

	Sky::Sky(const Player& _player) noexcept
		:impl(std::make_unique<Impl>()),
		player(_player)
	{}

	Sky::Sky(const Sky& _sky) noexcept
		:impl(std::make_unique<Impl>(*_sky.impl)),
		player(_sky.player)
	{}

	Sky::Sky(Sky&& _sky) noexcept
		:impl(std::move(_sky.impl)),
		player(_sky.player)
	{
	}

	Sky& Sky::operator=(const Sky& _lhs) noexcept
	{
		*impl = *_lhs.impl;
		return *this;
	}

	Sky& Sky::operator=(Sky&& _lhs) noexcept
	{
		impl = std::move(_lhs.impl);
		player = _lhs.player;
	}

	void Sky::custom_updater(void)
	{
		const auto count{get_count()};

		constexpr auto BASE_POS{static_cast<Real>(30000)};
		constexpr auto CENTER_POS{static_cast<Real>(1500)};

		if (count == 0)
		{
			Real generating_pos{};
			for (int i = 0; (generating_pos = BASE_POS - i * Texture::hight()) > 0; ++i)
			{
				try
				{
					*find_vacant_object(impl->textures) = std::make_unique<Texture>(CENTER_POS, generating_pos);
					*find_vacant_object(impl->textures) = std::make_unique<Texture>(CENTER_POS - Texture::width(), generating_pos);
					*find_vacant_object(impl->textures) = std::make_unique<Texture>(CENTER_POS + Texture::width(), generating_pos);
				}
				catch (NoSpace) { }
			}
		}
		else
		{
			if (count % Texture::interval() == 0)
			{
				try
				{
					*find_vacant_object(impl->textures) = std::make_unique<Texture>(CENTER_POS, BASE_POS);
					*find_vacant_object(impl->textures) = std::make_unique<Texture>(CENTER_POS - Texture::width(), BASE_POS);
					*find_vacant_object(impl->textures) = std::make_unique<Texture>(CENTER_POS + Texture::width(), BASE_POS);
				}
				catch (NoSpace) {}
			}
			else
			{
				for (auto& texture : impl->textures)
				{
					if (texture)
					{
						if (texture->get_flag()) { texture->update(); }
						else { texture.reset(); }
					}
				}

				Texture::set_camera_pos
					(
						400 - player->pos().x() / 6,
						[count = count, py = player->pos().y(), current_camera_y_coord = Texture::get_camera_pos().y]()
						{
							if (count == 0 || count == 1)
							{
								return -800.f;
							}
							else
							{
								auto player_y_coord_projected_to_camera_z_coord { (480 - py) / 2 };
								decltype(current_camera_y_coord) returned_value{};
								if (player_y_coord_projected_to_camera_z_coord > current_camera_y_coord)
								{
									return current_camera_y_coord + static_cast<Real>(1.5);
								}
								else
								{
									return current_camera_y_coord - static_cast<Real>(1.5);
								}
							}
						}(),
						-1000
					);
				Texture::set_plane_y_coord(480 + count < 800? 480 + count : 800);
			}
		}
	}

	void Sky::draw_rear(void) const
	{
		gp::DrawModiGraph(0, 0, 640, 0, 640, 480, 0, 480, Impl::sky_2_img);
		for (const auto& texture : impl->textures)
		{
			if (texture){ texture->draw(); }
		}
	}

	void Sky::draw_front(void) const
	{
	}

	void Sky::preparation(void)
	{
		Texture::preparation();
		ImagePool::add("../../data/img/sky_2.png");
		Impl::sky_2_img = ImagePool::get("../../data/img/sky_2.png");
	}

	Sky::~Sky(void) = default;
}