//bullet_behavior.h

#include "bullet.h"

namespace MyGameProject
{
	extern Bullet::Behavior course0;
	extern Bullet::Behavior course1;
	extern Bullet::Behavior course2;
	extern Bullet::Behavior course3;
	extern Bullet::Behavior course4;
	extern Bullet::Behavior course5;
	extern Bullet::Behavior course6;
	extern Bullet::Behavior course7;
	extern Bullet::Behavior course8;
	extern Bullet::Behavior course9;
	extern Bullet::Behavior course10;

	Bullet::Behavior straight_course(Real _speed);
	Bullet::Behavior accurate_homing(const MovingObject& _target, Real _speed, int _keep_time);
	Bullet::Behavior parabola(Real _V0,Real _G);
	Bullet::Behavior acceleration(Real _V0,Real _a,Real _v_limit);
	Bullet::Behavior log_spiral(Real _A,Real _B,Real _theta);
	Bullet::Behavior curvature_adjusted_homing(const MovingObject& _target,Real _speed,Real _omega,int _keep_time);
}
