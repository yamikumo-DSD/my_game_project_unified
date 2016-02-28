//base_class.h

#ifndef __BASE_CLASS_H
#define __BASE_CLASS_H

#include "makedxlib.h"
#include "smart_ptr.h"

namespace MyGameProject
{
	class SceneManager;

	class GameMain final : public gp::MakeDxLib
	{
	private:
		enum class State
		{
			INITIATION,
			NORMAL,
			FINALIZATION,
		}state;
		gp::smart_ptr<SceneManager> scene;
		void draw(void) const;
		void draw_fps(void) const;
		void draw_glid(int _interval) const;
	public:
		GameMain(int _window_mode);
		virtual void one_loop(void) override;
		virtual bool fin_loop_if(void) override;
		virtual ~GameMain(void);
	};
}

#endif