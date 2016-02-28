//scene.cpp

#include "scene.h"

MyGameProject::Scene::Scene(void) :new_scene_ptr(gp::smart_ptr<Scene>()), pop_flag(false)
{}

gp::smart_ptr<MyGameProject::Scene> MyGameProject::Scene::new_scene(void) 
{
	const auto active_new_scene_ptr = new_scene_ptr;
	new_scene_ptr = gp::smart_ptr<Scene>(); //new_scene_ptr becomes null.
	pop_flag = false;
	return active_new_scene_ptr;
}

bool MyGameProject::Scene::change_scene_if(void) const
{
	return pop_flag;
}

void MyGameProject::Scene::set_new_scene(gp::smart_ptr<Scene> _new_scene)
{
	new_scene_ptr = _new_scene; 
	pop_flag = true;
}

MyGameProject::Scene::~Scene(void){}