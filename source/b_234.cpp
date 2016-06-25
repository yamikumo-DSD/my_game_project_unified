//b_234.cpp

#include "b_234.h"
#include "image_pool.h"

namespace MyGameProject
{
	template<>
	std::array<int, 3>& B234<2>::img_handle(void) { static std::array<int, 3> imgs; return imgs; }
	template<>
	std::array<int, 3>& B234<3>::img_handle(void) { static std::array<int, 3> imgs; return imgs; }
	template<>
	std::array<int, 3>& B234<4>::img_handle(void) { static std::array<int, 3> imgs; return imgs; }

	template<>
	void B234<2>::preparation(void) noexcept
	{
		ImagePool::add("../../data/img/b2_001.png");
		img_handle()[0] = ImagePool::get("../../data/img/b2_001.png");
		ImagePool::add("../../data/img/b2_002.png");
		img_handle()[1] = ImagePool::get("../../data/img/b2_002.png");
		ImagePool::add("../../data/img/b2_003.png");
		img_handle()[2] = ImagePool::get("../../data/img/b2_003.png");
	}
	template<>
	void B234<3>::preparation(void) noexcept
	{
		ImagePool::add("../../data/img/b3_001.png");
		img_handle()[0] = ImagePool::get("../../data/img/b3_001.png");
		ImagePool::add("../../data/img/b3_002.png");
		img_handle()[1] = ImagePool::get("../../data/img/b3_002.png");
		ImagePool::add("../../data/img/b3_003.png");
		img_handle()[2] = ImagePool::get("../../data/img/b3_003.png");
	}
	template<>
	void B234<4>::preparation(void) noexcept
	{
		ImagePool::add("../../data/img/b4_001.png");
		img_handle()[0] = ImagePool::get("../../data/img/b4_001.png");
		ImagePool::add("../../data/img/b4_002.png");
		img_handle()[1] = ImagePool::get("../../data/img/b4_002.png");
		ImagePool::add("../../data/img/b4_003.png");
		img_handle()[2] = ImagePool::get("../../data/img/b4_003.png");
	}
}