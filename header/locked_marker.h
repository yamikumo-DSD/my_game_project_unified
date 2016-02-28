//locked_marker.h

#pragma once

#include "object.h"
#include "smart_ptr.h"
#include <memory>

namespace MyGameProject
{
	class Enemy;
	struct LockedMarkerVars;

	class LockedMarker : public DynamicObject, public MovingObject
	{
	private:
		gp::smart_ptr<Enemy> target;
		std::unique_ptr<LockedMarkerVars> vars;
		int m;
	public:
		LockedMarker(decltype(target) _target, int _m = 1);
		virtual void update(void) override final;
		virtual void draw(void) const override final;
		virtual ~LockedMarker(void);
		static void preperation(void);
	};
}
