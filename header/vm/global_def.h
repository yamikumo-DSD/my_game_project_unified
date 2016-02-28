//global_def.h

#pragma once

#include <cstdint>
#include <vector>

namespace MyVM
{
	using jint = std::int32_t;
	using jfloat = float;
	using jchar = char;
	using jbool = bool;
	using jshort = std::int16_t;

	using Byte = unsigned char;
	using Short = std::uint16_t;
	using Word = std::uint32_t;
	using DWord = std::uint64_t;

	using ByteCode = std::vector<Byte>;
}