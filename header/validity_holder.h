//flag_checked.h

//Type-erasing class to store objects of class which has method "bool get_flag(void)".

#pragma once

#include <utility>
#include <memory>

//For debugging
#include "debug_value.h"

namespace MyGameProject
{
	namespace detail
	{
		//Pure interface class.
		class FlagCheckedHolderBase
		{
		public:
			virtual bool is_valid(void) const = 0;

			//Clone stored object.
			virtual FlagCheckedHolderBase* clone(void) const = 0;

			virtual ~FlagCheckedHolderBase(void) {}
		};

		template<typename ClassWithMethod_get_flag>
		class FlagCheckedHolder : public FlagCheckedHolderBase
		{
		private:
			std::weak_ptr<ClassWithMethod_get_flag> stored;
			FlagCheckedHolder(std::weak_ptr<ClassWithMethod_get_flag> _weak) :stored(_weak) {}
		public:
			FlagCheckedHolder(std::shared_ptr<ClassWithMethod_get_flag> _stored) :stored(_stored) {}

			virtual bool is_valid(void) const override final
			{
				return !stored.expired() && stored.lock()->get_flag();
			};

			//Call the copy ctor of class of the stored object.
			virtual FlagCheckedHolderBase* clone(void) const override final { return new FlagCheckedHolder(stored); }
		};
	}

	//Hold a weak reference to object of class which has method "bool get_flag(void)".
	//operator bool(void) returns...
	//if the value the stored reference points to is not expired, then p->get_flag()
	//else false.
	class ValidityHolder
	{
	private:
		detail::FlagCheckedHolderBase* wrapped = nullptr;
		void msg_ptr(const char* _tag , void* _ptr) const
		{
			//std::stringstream msg;
			//msg << _tag << ": " << _ptr;
			//debug_value(msg.str());
		}
		void msg(const char* _tag = "") const
		{
			msg_ptr(_tag, wrapped);
		}
	public:
		//Can initialize by std::shared_ptr<T>; T is a class which has method "bool get_flag(void)".
		template<typename ClassWithMethod_get_flag>
		ValidityHolder(std::shared_ptr<ClassWithMethod_get_flag> _handle)
		{
			msg_ptr("template ctor(before)", wrapped);
			wrapped = new detail::FlagCheckedHolder<ClassWithMethod_get_flag>(_handle);
			msg_ptr("template ctor(after)", wrapped);
		}

		ValidityHolder(void)
		{
		}
		ValidityHolder(const ValidityHolder& _other) 
		{
			msg_ptr("cpy ctor(lhs)", wrapped);
			msg_ptr("cpy ctor(rhs)", _other.wrapped);
			wrapped = _other.wrapped ? _other.wrapped->clone() : nullptr;
		}
		ValidityHolder& operator=(const ValidityHolder& _other)
		{
			msg_ptr("cpy assign(lhs)", wrapped);
			msg_ptr("cpy assign(rhs)", _other.wrapped);
			ValidityHolder(_other).swap(*this);
			return *this;
		}
		ValidityHolder(ValidityHolder&& _other) :wrapped(std::move(_other.wrapped))
		{
			msg_ptr("mov ctor", this);
		}
		ValidityHolder& operator=(ValidityHolder&& _other)
		{
			msg_ptr("mov assign", this);
			wrapped = std::move(_other.wrapped);
			return *this;
		}

		ValidityHolder& swap(ValidityHolder& _other)
		{
			msg_ptr("swap", this);
			std::swap(wrapped, _other.wrapped);
			return *this;
		}

		bool value(void) const 
		{
			msg_ptr("value", const_cast<ValidityHolder*>(this));
			return wrapped && wrapped->is_valid(); 
		}

		operator bool(void) const { return value(); }

		~ValidityHolder(void) 
		{
			msg_ptr("dtor", this);
		}
	};
}