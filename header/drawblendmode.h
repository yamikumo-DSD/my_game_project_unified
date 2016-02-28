//drawblendmode.h

#ifndef __DRAW_BLEND_MODE_H
#define __DRAW_BLEND_MODE_H

#include "dxlib.h"
#include "boost/noncopyable.hpp"

namespace gp
{
	class ScopedDrawBlendMode : public boost::noncopyable
	{
	public:
		ScopedDrawBlendMode(int _DX_BLEND_MODE, int _Pal)
		{
			SetDrawBlendMode(_DX_BLEND_MODE, _Pal);
		}
		~ScopedDrawBlendMode(void)
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	};
}

#endif
