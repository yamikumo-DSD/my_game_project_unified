//bit_calculation.cpp

#include "bit_calculation.h"
#include <stdexcept>

MyVM::BitCalculation::Endian MyVM::BitCalculation::endian_judge(void)
{
	_int_32 obj;
	obj.i = 0x00000001;
	if (obj.field._0 == static_cast<Byte>(0x01))
	{
		return Endian::LITTLE;
	}
	else
	{
		return Endian::BIG;
	}
}

MyVM::BitCalculation::Byte MyVM::BitCalculation::get_byte(std::int32_t _value, int _index)
{
	_int_32 obj;
	obj.i = _value;
	if (endian_judge() == Endian::LITTLE)
	{
		switch (_index)
		{
		case 0:
			return obj.field._0;
		case 1:
			return obj.field._1;
		case 2:
			return obj.field._2;
		case 3:
			return obj.field._3;
		default:
			throw std::runtime_error("The index is out of range.");
		}
	}
	else
	{
		switch (_index)
		{
		case 0:
			return obj.field._3;
		case 1:
			return obj.field._2;
		case 2:
			return obj.field._1;
		case 3:
			return obj.field._0;
		default:
			throw std::runtime_error("The index is out of range.");
		}
	}
}

MyVM::BitCalculation::Byte MyVM::BitCalculation::get_byte(ByteField _value, int _index)
{
	if (endian_judge() == Endian::LITTLE)
	{
		switch (_index)
		{
		case 0:
			return _value._0;
		case 1:
			return _value._1;
		case 2:
			return _value._2;
		case 3:
			return _value._3;
		case 4:
			return _value._4;
		case 5:
			return _value._5;
		case 6:
			return _value._6;
		case 7:
			return _value._7;
		default:
			throw std::runtime_error("The index is out of range.");
		}
	}
	else
	{
		switch (_index)
		{
		case 0:
			return _value._7;
		case 1:
			return _value._6;
		case 2:
			return _value._5;
		case 3:
			return _value._4;
		case 4:
			return _value._3;
		case 5:
			return _value._2;
		case 6:
			return _value._1;
		case 7:
			return _value._0;
		default:
			throw std::runtime_error("The index is out of range.");
		}
	}
}