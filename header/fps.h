//fps.h

#ifndef __FPS_H
#define __FPS_H

namespace gp
{
	constexpr int FLAME = 60;
	void fps_wait(void);
	double get_fps(void);
}

#endif