//common.h

#pragma once

#include <type_traits>
#include <lua.hpp>
#include <boost/fusion/include/vector.hpp>
#include <sstream>
#include <boost/format.hpp>

namespace MyGameProject
{
	namespace LuaUtilities
	{
		template<typename... Members>
		using Struct = boost::fusion::vector<Members...>;

		using ThreadStatus = int;

		//if index given is negative, convert it into positive index.
		//if index given is positive, convert it into negative index.
		//if index given is 0, return 0.
		inline int reverse_index(int _top, int _index)
		{
			if (_index > 0) { return _index - (1 + _top); }
			else if(_index < 0){ return _index + (1 + _top); }
			return 0;
		}

		inline std::string stack_info(lua_State* _state, const char* _title = "####  STACK INFORMATION  ####")
		{
			std::stringstream msg;
			msg << _title << std::endl;
			const auto top{lua_gettop(_state)};

			if (top == 0)
			{
				msg << "Empty" << std::endl;
				return msg.str().c_str();
			}

			for (int i = 1; i <= top; ++i)
			{
				auto type{ lua_type(_state, i) };
				msg << boost::format("[%1%(%2%)]: %3%") %i %reverse_index(top, i) %lua_typename(_state, type) << std::endl;
			}

			return msg.str();
		}

		namespace detail
		{
			template<typename T>
			struct is_plain_type
			{
				static constexpr auto value
				{
					std::is_same<T, double     >::value ||
					std::is_same<T, int        >::value ||
					std::is_same<T, bool       >::value ||
					std::is_same<T, const char*>::value ||
					std::is_same<T, std::string>::value ||
					std::is_same<T, void*      >::value ||
					std::is_same<T, lua_State* >::value //Threads.
				};
			};

			template<typename T>
			inline T to_cvalue(lua_State* _state, int _index);

			template<>
			inline double to_cvalue<double>(lua_State* _state, int _index)
			{
				return static_cast<double>(lua_tonumber(_state, _index));
			}

			template<>
			inline int to_cvalue<int>(lua_State* _state, int _index)
			{
				return static_cast<int>(lua_tonumber(_state, _index));
			}

			template<>
			inline bool to_cvalue<bool>(lua_State* _state, int _index)
			{
				return lua_toboolean(_state, _index) != 0;
			}

			template<>
			inline const char* to_cvalue<const char*>(lua_State* _state, int _index)
			{
				return static_cast<const char*>(lua_tostring(_state, _index));
			}

			template<>
			inline std::string to_cvalue<std::string>(lua_State* _state, int _index)
			{
				return std::string(lua_tostring(_state, _index));
			}

			template<>
			inline void* to_cvalue<void*>(lua_State* _state, int _index)
			{
				return lua_touserdata(_state, _index);
			}

			template<>
			inline lua_State* to_cvalue<lua_State*>(lua_State* _state, int _index)
			{
				return lua_tothread(_state, _index);
			}

			template<int N, typename... Ts>
			struct ToCValues;

			template<typename... Ts>
			struct ToCValues<0, Ts...>
			{
				static auto apply(lua_State* _state, int _index)
				{
					return std::make_tuple();
				}
			};

			template<int N, typename THead, typename... TBody>
			struct ToCValues<N, THead, TBody...>
			{
				static auto apply(lua_State* _state, int _index)
				{
					return std::tuple_cat
					(
						std::make_tuple(to_cvalue<THead>(_state, _index)),
						ToCValues<N - 1, TBody...>::apply(_state, _index + 1)
					);
				}
			};

			template<typename... Ts>
			inline auto to_cvalues(lua_State* _state, int _index_from)
			{
				return ToCValues<sizeof...(Ts), Ts...>::apply(_state, _index_from);
			}

			template<int N>
			struct CallWithExpandedTupleImpl
			{
				template<typename Func, typename Tuple, typename... Args>
				static auto apply(Func _f, Tuple _t, Args... _args)
				{
					return CallWithExpandedTupleImpl<N - 1>::apply(_f, _t, std::get<N - 1>(_t), _args...);
				}
			};

			template<>
			struct CallWithExpandedTupleImpl<0>
			{
				template<typename Func, typename Tuple, typename... Args>
				static auto apply(Func _f, Tuple _t, Args... _args)
				{
					return _f(_args...);
				}
			};

			template<typename Func, typename Tuple>
			inline auto call_with_expanded_tuple(Func _f, Tuple _t)
			{
				return CallWithExpandedTupleImpl<std::tuple_size<Tuple>::value>::apply(_f, _t);
			}

			template<typename T>
			inline void push_cvalue(lua_State* _state, T _cvalue);

			template<>
			inline void push_cvalue<double>(lua_State* _state, double _cvalue)
			{
				lua_pushnumber(_state, _cvalue);
			}

			template<>
			inline void push_cvalue<int>(lua_State* _state, int _cvalue)
			{
				lua_pushnumber(_state, _cvalue);
			}

			template<>
			inline void push_cvalue<bool>(lua_State* _state, bool _cvalue)
			{
				lua_pushboolean(_state, _cvalue);
			}

			template<>
			inline void push_cvalue<const char*>(lua_State* _state, const char* _cvalue)
			{
				lua_pushstring(_state, _cvalue);
			}

			template<>
			inline void push_cvalue<std::string>(lua_State* _state, std::string _cvalue)
			{
				lua_pushstring(_state, _cvalue.c_str());
			}

			template<>
			inline void push_cvalue<void*>(lua_State* _state, void* _cvalue)
			{
				lua_pushlightuserdata(_state, _cvalue);
			}

			inline void push_cvalue(lua_State* _state)
			{
				lua_pushthread(_state);
			}

			//Copy-construct new userdata on the stack.
			template<typename T>
			inline void construct_userdata(lua_State* _state, const T& _userdata)
			{
				//Allocate memory, not initialized.
				auto ptr{static_cast<T*>(lua_newuserdata(_state, sizeof(T)))};
				//Initialize(call constructor of T).
				new(ptr) T(_userdata);
			}

			template<typename To>
			inline To upvalue_cast(lua_State* _state, int _upvalue_index)
			{
				return static_cast<To>(lua_touserdata(_state, lua_upvalueindex(_upvalue_index)));
			}
		}
	}
}