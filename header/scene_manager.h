//scene_manager.h

#ifndef __SCENE_MANAGER_H
#define __SCENE_MANAGER_H

#include "smart_ptr.h"

namespace MyGameProject
{
	class Scene;

	class SceneManager
	{
	private:
		gp::smart_ptr<Scene> current_scene;
		void change(gp::smart_ptr<Scene> _new_scene);
	public:
		SceneManager(gp::smart_ptr<Scene> _initial_scene);
		void play_se(int _loudness) const;
		void draw(void) const;
		void update(void);
		bool no_scene(void) const;
	};
}

#endif