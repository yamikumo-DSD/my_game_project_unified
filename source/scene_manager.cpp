//scene_manager.cpp

#include "scene_manager.h"
#include "scene.h"
#include "msgbox.h"

namespace MyGameProject
{
	SceneManager::SceneManager(gp::smart_ptr<Scene> _initial_scene)
		:current_scene(_initial_scene)
	{
		_initial_scene->load();
	}

	void SceneManager::update(void)
	{
		if (current_scene)
		{
			current_scene->update();
			if (current_scene->change_scene_if())
			{
				change(current_scene->new_scene());
			}
		}
	}

	void SceneManager::play_se(int _loudness) const
	{
		if (current_scene) { current_scene->play_se(_loudness); }
	}

	void SceneManager::draw(void) const
	{
		if (current_scene) { current_scene->draw(); }
	}

	void SceneManager::change(gp::smart_ptr<Scene> _new_scene)
	{
		current_scene->unload();
		current_scene = _new_scene;
		if (current_scene) { current_scene->load(); }
	}

	bool SceneManager::no_scene(void) const
	{
		return !static_cast<bool>(current_scene);
	}
} //namespace MyGameProject