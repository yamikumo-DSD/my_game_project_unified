//scene.h

#ifndef __SCENE_H
#define __SCENE_H

#include <stack>
#include "smart_ptr.h"

namespace MyGameProject
{
	class Scene
	{
	private:
		gp::smart_ptr<Scene> new_scene_ptr;
		bool pop_flag;
	public:
		Scene(void);
		gp::smart_ptr<Scene> new_scene(void);
		bool change_scene_if(void) const;
		void set_new_scene(gp::smart_ptr<Scene> _new_scene);
		virtual void play_se(int _loudness) const = 0;
		virtual void draw(void) const = 0;
		virtual void update(void) = 0;
		virtual void load(void) = 0;
		virtual void unload(void) = 0;
		virtual ~Scene(void);
	};
}

#endif