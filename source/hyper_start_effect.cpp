//hyper_start_effect.cpp

#include "hyper_start_effect.h"
#include "player.h"
#include <boost/optional.hpp>
#include "object.h"
#include "mathematics.h"
#include "ranged_uniform_random.h"
#include "image_pool.h"
#include "decl_static_image_handler.h"
#include "draw_order.h"
#include "find_vacant_object.h"
#include <dxlib.h>

namespace MyGameProject
{
	template<typename... ValType>
	using optional = boost::optional<ValType...>;
	using namespace boost::math::constants;

	class Particle final : public DynamicObject, public MovingObject
	{
	private:
		enum class Color { BLACK = 0, BLUE, RED, WHITE, YELLOW, }color;
		static constexpr auto int2color(int _color_index) noexcept { return static_cast<Color>(_color_index); }
		static constexpr auto color2int(Color _color) noexcept { return static_cast<int>(_color); }
		const Player& player;
		int count{0};
		Real dir{0};
		Point2D rational_pos;
		static constexpr Real INITIAL_DISTANCE{200};
		static constexpr int DURATION{13};
		STATIC_IMAGE_HANDLER_LIST(img, 5);
	public:
		Particle(int _color_index, const Player& _player) noexcept 
			:color(int2color(_color_index)), 
			player(_player), 
			rational_pos(),
			dir(gp::safe_rand(static_cast<Real>(0), two_pi<Real>())){}
		virtual ~Particle(void) noexcept = default;

		virtual void update(void) override final
		{
			if (count == DURATION) { set_flag_off(); }
			rational_pos = INITIAL_DISTANCE * (1 - static_cast<Real>(count) / DURATION) * Point2D(std::cos(dir), std::sin(dir));
			pos() = player.pos() + rational_pos;
			++count;
		}
		virtual void draw(void) const override final
		{
			if (count > 0)
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(14), pos().x(), pos().y(), 1.8 * (1 - static_cast<Real>(count) / DURATION), 0.0, img()[color2int(color)], true),
					DX_BLENDMODE_MULA,
					100
				);
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(14), pos().x(), pos().y(), 1.8 * (1 - static_cast<Real>(count) / DURATION), 0.0, img()[color2int(color)], true),
					DX_BLENDMODE_ADD,
					100
				);
			}
		}

		static void preparation(void) noexcept
		{
			using namespace std::literals;
			constexpr auto ROOT{"../../data/img"};
			ImagePool::add(ROOT + "/white_ball_black.png"s ); img()[color2int(Color::BLACK )] = ImagePool::get(ROOT + "/white_ball_black.png"s );
			ImagePool::add(ROOT + "/white_ball_green.png"s ); img()[color2int(Color::BLUE  )] = ImagePool::get(ROOT + "/white_ball_green.png"s );
			ImagePool::add(ROOT + "/white_ball_red.png"s   ); img()[color2int(Color::RED   )] = ImagePool::get(ROOT + "/white_ball_red.png"s   );
			ImagePool::add(ROOT + "/white_ball_white.png"s ); img()[color2int(Color::WHITE )] = ImagePool::get(ROOT + "/white_ball_white.png"s );
			ImagePool::add(ROOT + "/white_ball_yellow.png"s); img()[color2int(Color::YELLOW)] = ImagePool::get(ROOT + "/white_ball_yellow.png"s);
		}
	};

	struct HyperStartEffect::Impl
	{
		optional<const Player&> player;
		Impl(const Player& _player) noexcept :player(_player), particles(), count(DURATION + 1){}
		static constexpr std::size_t PARTICLE_NUM{30};
		std::array<std::shared_ptr<Particle>, PARTICLE_NUM> particles;
		int count{ 0 };
		static constexpr int DURATION{100};
	};

	HyperStartEffect::HyperStartEffect(const Player& _player) noexcept
		:pimpl(std::make_unique<Impl>(_player))
	{}

	HyperStartEffect::HyperStartEffect(HyperStartEffect&& _effect) noexcept
		: pimpl(std::move(_effect.pimpl))
	{}

	HyperStartEffect::HyperStartEffect(const HyperStartEffect& _effect) noexcept
		: pimpl(std::make_unique<Impl>(*_effect.pimpl))
	{}

	HyperStartEffect& HyperStartEffect::operator=(HyperStartEffect&& _effect) noexcept { pimpl = std::move(_effect.pimpl); }

	HyperStartEffect& HyperStartEffect::operator=(const HyperStartEffect& _effect) noexcept { *pimpl = *_effect.pimpl; }

	HyperStartEffect::~HyperStartEffect(void) noexcept = default;

	void HyperStartEffect::start_playing(void) noexcept { pimpl->count = 0; }

	void HyperStartEffect::update(void) noexcept
	{
		for (auto& p : pimpl->particles) { if (p && p->get_flag()) { p->update(); } }

		if (pimpl->count < Impl::DURATION)
		{
			if (pimpl->count % 2 == 0) { *find_vacant_object(pimpl->particles) = std::make_shared<Particle>(pimpl->count % 6, *pimpl->player); }
		}

		++pimpl->count;
	}

	void HyperStartEffect::draw(void) noexcept
	{
		for (const auto& p : pimpl->particles) { if (p && p->get_flag()) { p->draw(); } }
	}

	void HyperStartEffect::preparation(void) noexcept
	{
		Particle::preparation();
	}
}