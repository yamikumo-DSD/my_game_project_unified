//add_validity.h

#pragma once

#include "validity_holder.h"

namespace MyGameProject
{
	//Helper class to add validity-checking property.
	template<typename BaseEntity>
	class AddValidity : public BaseEntity
	{
	private:
		ValidityHolder validity;

		//Ban direct use of constructor.
		//Objects of this class must be constructed from "make_shared".
		using BaseEntity::BaseEntity;
	protected:
		//Helper to make use intuitively.
		using Base = AddValidity;

		//Make shared_ptr to the base entity.
		//The arguments is to be passed to the constructor of the base entity.
		template<typename... Args>
		static std::shared_ptr<BaseEntity> make_shared(Args&&... _args)
		{
			auto new_entity{std::make_shared<AddValidity>(std::forward<Args>(_args)...)};
			new_entity->validity = new_entity;
			return static_cast<std::shared_ptr<BaseEntity>>(new_entity);
		}
	public:
		bool is_valid(void)
		{
			return validity.value(); 
		};

		virtual ~AddValidity(void) {}
	};
}