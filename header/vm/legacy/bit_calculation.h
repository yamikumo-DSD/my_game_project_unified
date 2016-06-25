//bit_calculation.h

#pragma once

#include <cstdint>
#include <typeinfo>
#include <stdexcept>

namespace MyVM
{
	namespace BitCalculation
	{
		using Byte = unsigned char; //CAUTION! Type changed from signed char to unsigned char.

		enum class Endian
		{
			BIG,
			LITTLE,
		};

		struct _int_32_1_byte_split
		{
			Byte _0 : 8;//8 bit
			Byte _1 : 8;//16 bit
			Byte _2 : 8;//24 bit
			Byte _3 : 8;//32 bit
		};

		union _int_32
		{
			std::int32_t i;
			_int_32_1_byte_split field;
		};

		Endian endian_judge(void);

		//When index is 0, this will return first 1byte of the target value.
		Byte get_byte(std::int32_t _value, int _index);

		struct ByteField
		{
			Byte _0 : 8; //8 bit
			Byte _1 : 8; //16 bit
			Byte _2 : 8; //24 bit
			Byte _3 : 8; //32 bit
			Byte _4 : 8; //40 bit
			Byte _5 : 8; //48 bit
			Byte _6 : 8; //56 bit
			Byte _7 : 8; //64 bit
		};

		Byte get_byte(ByteField _field, int _index);

		template<class T>
		union T2Byte
		{
			T2Byte(const T& _t) :t(_t) {}
			T2Byte(void){}
			T t;
			ByteField field;
		};

		template<class T>
		Byte get_byte(const T& _value, int _index)
		{
			T2Byte<T> obj = _value;
			if (endian_judge() == Endian::LITTLE)
			{
				switch (_index)
				{
				case 0: return obj.field._0;
				case 1: return obj.field._1;
				case 2: return obj.field._2;
				case 3: return obj.field._3;
				case 4: return obj.field._4;
				case 5: return obj.field._5;
				case 6: return obj.field._6;
				case 7: return obj.field._7;
				default: throw std::runtime_error("The index is out of range.");
				}
			}
			else
			{
				switch (_index)
				{
				case 0: return obj.field._7;
				case 1: return obj.field._6;
				case 2: return obj.field._5;
				case 3: return obj.field._4;
				case 4: return obj.field._3;
				case 5: return obj.field._2;
				case 6: return obj.field._1;
				case 7: return obj.field._0;
				default: throw std::runtime_error("The index is out of range.");
				}
			}
		}

		template<class Converted, class Original>
		Converted bit_cast(const Original& _original)
		{
			if (sizeof(Original) >= 8 || sizeof(Converted) >= 8)
			{
				throw std::runtime_error
				(
					std::string("Conversion error. The size of either ") +
					typeid(Original).name() +
					" or " +
					typeid(Original).name() +
					" is larger than 8byte is not allowed."
				);
			}
			T2Byte<Original> src = _original;
			//src.t = _original;
			T2Byte<Converted> dst;
			dst.field._0 = src.field._0;
			dst.field._1 = src.field._1;
			dst.field._2 = src.field._2;
			dst.field._3 = src.field._3;
			dst.field._4 = src.field._4;
			dst.field._5 = src.field._5;
			dst.field._6 = src.field._6;
			dst.field._7 = src.field._7;
			return dst.t;
		}
	}
}
