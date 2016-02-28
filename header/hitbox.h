//hitbox.h

#ifndef __HITBOX_H
#define __HITBOX_H

#include "mathematics.h"
#include <boost/geometry/geometry.hpp>
#include <boost/geometry.hpp>
#include <boost/variant.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <type_traits>

namespace MyGameProject
{
	namespace ShapeElement
	{
		struct Null{};
		struct Circle
		{
			Circle(void);
			Circle(Real _radius);
			Real radius; 
		};
		using Polygon = bg::model::polygon < Point2D > ;
	}

	using Shape = boost::variant<ShapeElement::Null, ShapeElement::Circle, ShapeElement::Polygon>;

	struct PhysicalState
	{
		PhysicalState(void);
		PhysicalState(Real _angle,const Point2D& _p);
		Real angle;
		Point2D p;
	};

	class HitBox
	{
	private:
		Shape shape;
		PhysicalState physical_state;
	public:
		HitBox(void);
		HitBox(const Shape& _shape,const PhysicalState& _physical_state);
		const Shape& get_shape(void) const;
		Shape& get_shape(void);
		const PhysicalState& get_physical_state(void) const;
		PhysicalState& get_physical_state(void);
	};

	bool intersect(const PhysicalState& _state1, ShapeElement::Circle         _shape1,  const PhysicalState& _state2, ShapeElement::Circle         _shape2);
	bool intersect(const PhysicalState& _state1, ShapeElement::Circle         _shape1,  const PhysicalState& _state2, const ShapeElement::Polygon& _shape2);
	inline bool intersect(const PhysicalState& _state1, const ShapeElement::Polygon& _shape1,  const PhysicalState& _state2, ShapeElement::Circle         _shape2){ return intersect(_state2,_shape2,_state1,_shape1); }
	bool intersect(const PhysicalState& _state1, const ShapeElement::Polygon& _shape1,  const PhysicalState& _state2, const ShapeElement::Polygon& _shape2);

	template<class R>
	class StaticVisitor
	{
	public:
		using result_type = R;
	};

	class HitBoxIntersectVisitor : public StaticVisitor<bool>
	{
	private:
		const PhysicalState& state1,state2;
	public:
		HitBoxIntersectVisitor(const PhysicalState& _state1, const PhysicalState& _state2);
		template<class Shape1,class Shape2>
		typename std::enable_if<!std::is_same<Shape1, ShapeElement::Null>::value && !std::is_same<Shape2, ShapeElement::Null>::value,bool>::type operator()(const Shape1& _shape1,const Shape2& _shape2) const
		{
			return intersect(state1, _shape1, state2, _shape2);
		}
		template<class Shape1, class Shape2>
		typename std::enable_if<std::is_same<Shape1, ShapeElement::Null>::value || std::is_same<Shape2, ShapeElement::Null>::value, bool>::type operator()(const Shape1& _shape1, const Shape2& _shape2) const
		{
			return false;
		}
	};

	bool intersect(const HitBox& _hitbox1, const HitBox& _hitbox2);
}

#endif
