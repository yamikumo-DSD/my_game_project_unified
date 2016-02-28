//decl_static_image_handler.h

#pragma once
#include <array>

#ifndef STATIC_IMAGE_HANDLER
	#define STATIC_IMAGE_HANDLER(NAME) \
	static int& NAME(void){static int i = 0; return i;}
#endif

#ifndef STATIC_IMAGE_HANDLER_LIST
	#define STATIC_IMAGE_HANDLER_LIST(NAME, N) \
	static std::array<int, N>& NAME(void){static std::array<int, N> is; return is;}
#endif
