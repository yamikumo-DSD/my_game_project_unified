#include "fps.h"
#include "dxlib.h"
#include <chrono>
#include <array>
#include <thread>

namespace
{
	using namespace std::chrono;
	const auto EPOCH = high_resolution_clock::time_point();
	milliseconds sum;
	using namespace std::literals;
}

void gp::fps_wait(void)
{
	static int fps_count(0);
	const auto gnt = high_resolution_clock::now();
	milliseconds term(0);
	static high_resolution_clock::time_point t,count0t; //Default constructor, creates a time_point with a value of Clock's epoch.
	static std::array<milliseconds,gp::FLAME> dts;

	if (fps_count == 0)
	{
		if (t != EPOCH)
		{
			term = duration_cast<milliseconds>(count0t - high_resolution_clock::now()) + milliseconds(1000);
		}
	}
	else
	{
		term = duration_cast<milliseconds>(count0t - high_resolution_clock::now()) + milliseconds(fps_count * (1000 / FLAME));
	}

	if (term > 0ms){ std::this_thread::sleep_for(term); }

	if (fps_count == 0){count0t = gnt;}

	dts[fps_count] = duration_cast<milliseconds>(gnt - t);

	if (fps_count == gp::FLAME - 1)
	{
		sum = 0ms;
		for (auto dt : dts){ sum += dt; }
	}

	t = gnt;

	fps_count = (++fps_count) % gp::FLAME; //0->1->2->...->58->59->0->1->2->...
}

double gp::get_fps(void)
{
	const auto ave = static_cast<double>(sum.count()) / gp::FLAME;
	return 1000 / ave;
}