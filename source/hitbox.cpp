//hitbox.cpp

#define BOOST_UBLAS_NDEBUG

#include "hitbox.h"
#include <boost/geometry/strategies/transform.hpp>

MyGameProject::ShapeElement::Circle::Circle(void) :radius(0){}

MyGameProject::ShapeElement::Circle::Circle(Real _radius) : radius(_radius){}

MyGameProject::PhysicalState::PhysicalState(void) : angle(0), p(Point2D(0, 0)){}

MyGameProject::PhysicalState::PhysicalState(Real _angle,const Point2D& _p) : angle(_angle), p(_p){}

MyGameProject::HitBox::HitBox(void):shape(ShapeElement::Null()),physical_state(){}

MyGameProject::HitBox::HitBox(const Shape& _shape, const PhysicalState& _physical_state)
	:shape(_shape), physical_state(_physical_state)
{}

const MyGameProject::Shape& MyGameProject::HitBox::get_shape(void) const{return shape;}

MyGameProject::Shape& MyGameProject::HitBox::get_shape(void){return shape;}

const MyGameProject::PhysicalState& MyGameProject::HitBox::get_physical_state(void) const{return physical_state;}

MyGameProject::PhysicalState& MyGameProject::HitBox::get_physical_state(void){return physical_state;}

bool MyGameProject::intersect(const PhysicalState& _state1, ShapeElement::Circle _shape1, const PhysicalState& _state2, ShapeElement::Circle _shape2)
{
	return bg::distance(_state1.p,_state2.p) < _shape1.radius + _shape2.radius;
}

bool MyGameProject::intersect(const PhysicalState& _state1, ShapeElement::Circle  _shape1, const PhysicalState& _state2, const ShapeElement::Polygon& _shape2)
{
	using namespace bg::strategy::transform;
	using boost::numeric::ublas::prod;

	rotate_transformer<bg::radian, Real, 2, 2> rotate(_state2.angle);
	translate_transformer<Real, 2, 2> translate(_state2.p.x(), _state2.p.y());
	ublas_transformer<Real, 2, 2> composite(prod(translate.matrix(), rotate.matrix())); //Arguments of function prod() must be well-considered to get a correct result.
	ShapeElement::Polygon temp;
	bg::transform(_shape2, temp, composite);
	return bg::distance(_state1.p, temp) < _shape1.radius;
}

bool MyGameProject::intersect(const PhysicalState& _state1, const ShapeElement::Polygon& _shape1, const PhysicalState& _state2, const ShapeElement::Polygon& _shape2)
{
	using namespace bg::strategy::transform;
	using boost::numeric::ublas::prod;
	rotate_transformer<bg::radian, Real, 2, 2> rotate1(_state1.angle), rotate2(_state2.angle);
	translate_transformer<Real, 2, 2> translate1(_state1.p.x(), _state1.p.y()), translate2(_state2.p.x(), _state2.p.y());
	ublas_transformer<Real, 2, 2> composite1(prod(translate1.matrix(), rotate1.matrix())), composite2(prod(translate2.matrix(), rotate2.matrix()));
	ShapeElement::Polygon temp1, temp2;
	bg::transform(_shape1, temp1, composite1); bg::transform(_shape2, temp2, composite2);
	for (const auto& p : temp1.outer()){ if (bg::within(p, temp2)){ return true; } }
	for (const auto& p : temp2.outer()){ if (bg::within(p, temp1)){ return true; } }
	return false;
}

MyGameProject::HitBoxIntersectVisitor::HitBoxIntersectVisitor(const PhysicalState& _state1, const PhysicalState& _state2)
	:state1(_state1), state2(_state2)
{}

bool MyGameProject::intersect(const HitBox& _hitbox1, const HitBox& _hitbox2)
{
	return boost::apply_visitor(HitBoxIntersectVisitor(_hitbox1.get_physical_state(), _hitbox2.get_physical_state()), _hitbox1.get_shape(), _hitbox2.get_shape());
}
