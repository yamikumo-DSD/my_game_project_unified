//environmental_constants

namespace MyGameProject
{
	template<class Type>
	constexpr Type WW(void){ return static_cast<Type>(640); }
	template<class Type>
	constexpr Type WH(void){ return static_cast<Type>(480); }
	template<class Type>
	constexpr Type WW_2(void){ return WW<Type>() / 2; }
	template<class Type>
	constexpr Type WH_2(void){ return WH<Type>() / 2; }

}