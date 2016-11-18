#pragma once

#include <type_traits>
#include <cstring>
#include <utility>
#include <new>

namespace gp
{
	template<class T>
	class ConstPropPtr
	{
	private:
		T* ptr_;
	public:
		explicit constexpr ConstPropPtr(T* p) : ptr_(p) {}
		~ConstPropPtr() { ptr_->~T(); ptr_ = nullptr; }

		          decltype(auto) operator->(void)       noexcept { return  ptr_; }
		constexpr decltype(auto) operator->(void) const noexcept { return  ptr_; }
		          decltype(auto) operator* (void)       noexcept { return *ptr_; }
		constexpr decltype(auto) operator* (void) const noexcept { return *ptr_; }

		ConstPropPtr(const ConstPropPtr&) = delete;
		ConstPropPtr& operator=(const ConstPropPtr&) = delete;
	};

	template<typename T, std::size_t STORAGE_SIZE = 64, typename Pointer = ConstPropPtr<T>>
	class StaticPimpl
	{
	private:
		std::aligned_storage_t<STORAGE_SIZE> storage;
		Pointer _ptr;
	public:
		template<typename... Ts>
		constexpr StaticPimpl(Ts&&... _args)
			:_ptr(new(reinterpret_cast<T*>(&storage)) T(std::forward<Ts>(_args)...))
		{
			static_assert
			(
				STORAGE_SIZE > sizeof(T),
				"The size of implementation class must be lower than STORAGE_SIZE."
			);
		}
		StaticPimpl(const StaticPimpl& _other) :_ptr(reinterpret_cast<T*>(&storage))
		{
			std::memcpy(&storage, &_other.storage, STORAGE_SIZE);
		}
		StaticPimpl& operator=(const StaticPimpl& _other)
		{
			std::memcpy(&storage, &_other.storage, STORAGE_SIZE);
			return *this;
		}
		          decltype(auto) operator* (void)       noexcept { return *_ptr            ; }
		constexpr decltype(auto) operator* (void) const noexcept { return *_ptr            ; }
		          decltype(auto) operator->(void)       noexcept { return _ptr.operator->(); }
		constexpr decltype(auto) operator->(void) const noexcept { return _ptr.operator->(); }
		~StaticPimpl(void) {}
	};

	template<typename T, std::size_t STORAGE_SIZE = 64>
	struct StaticPimplOwner
	{
		StaticPimpl<T, STORAGE_SIZE> d;
	};
}