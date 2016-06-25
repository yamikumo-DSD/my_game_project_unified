//add_get.h

#pragma once

#include "image_pool.h"

namespace MyGameProject
{
	template<typename Path>
	inline decltype(auto) add_get(Path&& _path) noexcept
	{
		ImagePool::add(_path);
		return ImagePool::get(std::forward<Path>(_path));
	}
}