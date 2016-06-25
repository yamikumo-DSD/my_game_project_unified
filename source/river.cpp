//river.cpp

#include "river.h"
#include "draw_order.h"
#include "find_vacant_object.h"
#include "debug.h"
#include "river_tex.h"
#include "koyo_tex.h"
#include "cloud_tex.h"

namespace MyGameProject
{

	int RiverTexture::img;
	RiverTexture::CameraPos RiverTexture::camera_pos;
	Real RiverTexture::y;
	int KoyoTexture::img;
	KoyoTexture::CameraPos KoyoTexture::camera_pos;
	Real KoyoTexture::y;
	int CloudTexture::img;
	CloudTexture::CameraPos CloudTexture::camera_pos;
	Real CloudTexture::y;

	struct River::Impl
	{
		static constexpr auto TEXTURE_NUM = 150;
		std::array<std::unique_ptr<RiverTexture>, TEXTURE_NUM> river_textures;
		std::array<std::unique_ptr<KoyoTexture>, TEXTURE_NUM> koyo_textures;
		//std::array<std::unique_ptr<CloudTexture>, TEXTURE_NUM> cloud_textures;

		//special functions
		Impl(void) noexcept = default;
		Impl(Impl&& _impl) = default;
		Impl(const Impl& _impl) noexcept
		{ for (int i = 0; i != TEXTURE_NUM; ++i) { *river_textures[i] = *_impl.river_textures[i]; *koyo_textures[i] = *_impl.koyo_textures[i];/**cloud_textures[i] = *_impl.cloud_textures[i];*/} }
		Impl& operator=(const Impl& _lhs) noexcept
		{ for (int i = 0; i != TEXTURE_NUM; ++i) { *river_textures[i] = *_lhs.river_textures[i]; *koyo_textures[i] = *_lhs.koyo_textures[i];/**cloud_textures[i] = *_lhs.cloud_textures[i];*/} }

		static int sky_2_img;
	};

	int River::Impl::sky_2_img;

	River::River(void) noexcept
		:impl(std::make_unique<Impl>())
	{}

	River::River(const River& _sky) noexcept
		:impl(std::make_unique<Impl>(*_sky.impl))
	{}

	River::River(River&& _sky) noexcept
		:impl(std::move(_sky.impl))
	{ }

	River& River::operator=(const River& _lhs) noexcept
	{
		*impl = *_lhs.impl;
		return *this;
	}

	River& River::operator=(River&& _lhs) noexcept
	{
		impl = std::move(_lhs.impl);
	}

	void River::custom_updater(void)
	{
		const auto count{get_count()};

		constexpr auto BASE_POS{static_cast<Real>(25000)};
		constexpr auto CENTER_POS{static_cast<Real>(2100)};

		if (count == 0)
		{
			Real generating_pos{};
			for (int i = 0; (generating_pos = BASE_POS - i * RiverTexture::hight()) > 0; ++i)
			{
				try
				{
					*find_vacant_object(impl->koyo_textures) = std::make_unique<KoyoTexture>(CENTER_POS + RiverTexture::width() - 200 + RiverTexture::width(), generating_pos, KoyoTexture::Side::MIDDLE);
					*find_vacant_object(impl->koyo_textures) = std::make_unique<KoyoTexture>(CENTER_POS + RiverTexture::width() - 200, generating_pos, KoyoTexture::Side::RIGHT);
					*find_vacant_object(impl->river_textures) = std::make_unique<RiverTexture>(CENTER_POS, generating_pos);
					*find_vacant_object(impl->koyo_textures) = std::make_unique<KoyoTexture>(CENTER_POS - RiverTexture::width() + 200, generating_pos, KoyoTexture::Side::LEFT);
					*find_vacant_object(impl->koyo_textures) = std::make_unique<KoyoTexture>(CENTER_POS - RiverTexture::width() + 200 - RiverTexture::width(), generating_pos, KoyoTexture::Side::MIDDLE);

					//*find_vacant_object(impl->cloud_textures) = std::make_unique<CloudTexture>(CENTER_POS, generating_pos);
				}
				catch (NoSpace) { }
			}
		}
		else
		{
			if (count % RiverTexture::interval() == 0)
			{
				try
				{
					*find_vacant_object(impl->koyo_textures) = std::make_unique<KoyoTexture>(CENTER_POS + RiverTexture::width() - 200 + RiverTexture::width(), BASE_POS, KoyoTexture::Side::MIDDLE);
					*find_vacant_object(impl->koyo_textures) = std::make_unique<KoyoTexture>(CENTER_POS + RiverTexture::width() - 200, BASE_POS, KoyoTexture::Side::RIGHT);
					*find_vacant_object(impl->river_textures) = std::make_unique<RiverTexture>(CENTER_POS, BASE_POS);
					*find_vacant_object(impl->koyo_textures) = std::make_unique<KoyoTexture>(CENTER_POS - RiverTexture::width() + 200, BASE_POS, KoyoTexture::Side::LEFT);
					*find_vacant_object(impl->koyo_textures) = std::make_unique<KoyoTexture>(CENTER_POS - RiverTexture::width() + 200 - RiverTexture::width(), BASE_POS, KoyoTexture::Side::MIDDLE);

					//*find_vacant_object(impl->cloud_textures) = std::make_unique<CloudTexture>(CENTER_POS, BASE_POS);
				}
				catch (NoSpace) {}
			}
			else
			{
				for (auto& river_texture : impl->river_textures)
				{
					if (river_texture)
					{
						if (river_texture->get_flag()) { river_texture->update(); }
						else { river_texture.reset(); }
					}
				}
				for (auto& koyo_texture : impl->koyo_textures)
				{
					if (koyo_texture)
					{
						if (koyo_texture->get_flag()) { koyo_texture->update(); }
						else { koyo_texture.reset(); }
					}
				}
				//for (auto& cloud_texture : impl->cloud_textures)
				//{
				//	if (cloud_texture)
				//	{
				//		if (cloud_texture->get_flag()) { cloud_texture->update(); }
				//		else { cloud_texture.reset(); }
				//	}
				//}

				constexpr std::array<int, 3> camera_pos{320, -400, -1000};

				RiverTexture::set_camera_pos (camera_pos[0], camera_pos[1], camera_pos[2]);
				RiverTexture::set_plane_y_coord(800 + count);
				KoyoTexture::set_camera_pos (camera_pos[0], camera_pos[1], camera_pos[2]);
				KoyoTexture::set_plane_y_coord(800 + count);

				//CloudTexture::set_camera_pos (camera_pos[0], camera_pos[1], camera_pos[2]);
				//CloudTexture::set_plane_y_coord(-1000 + count);
			}
		}
	}

	void River::draw_rear(void) const
	{
		for (const auto& river_texture : impl->river_textures)
		{
			if (river_texture){ river_texture->draw(); }
		}
		for (const auto& koyo_texture : impl->koyo_textures)
		{
			if (koyo_texture){ koyo_texture->draw(); }
		}
		//for (const auto& cloud_texture : impl->cloud_textures)
		//{
		//	if (cloud_texture){ cloud_texture->draw(); }
		//}
	}

	void River::draw_front(void) const
	{
	}

	void River::preparation(void)
	{
		RiverTexture::preparation();
		KoyoTexture::preparation();
		CloudTexture::preparation();
	}

	River::~River(void) = default;
}
