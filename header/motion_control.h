//motion_control.h

#include "motion.h"
#include "object.h"

namespace MyGameProject
{
	template<class ObjectType>
	void motion_control
		(
		ObjectType& _object,
		const Point2D& _end_point,
		int _start_count,
		int _end_count
		)
	{
		const auto motion = create_linear_motion_of_uniform_acceleration_2D(_object.pos(),_end_point,_end_count - _start_count);
		if (_object.get_count() <= _end_count && _object.get_count() >= _start_count)
		{
		}
	}
}