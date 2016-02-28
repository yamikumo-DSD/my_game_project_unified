//title.h

#ifndef __TITLE_H
#define __TITLE_H

#include "scene.h"
#include "smart_ptr.h"

namespace MyGameProject
{
	class Device;
	class SEManager;

	class Title : public MyGameProject::Scene
	{
	private:
		friend class TitleBGDrawer;
		gp::smart_ptr<MyGameProject::Device> device;
		int selected_menu;
		static constexpr int MENU_NUM = 5;
		gp::smart_ptr<SEManager> se_manager;
		gp::smart_ptr<TitleBGDrawer> bg_drawer;
	public:
		Title(void);
		virtual void play_se(int _loudness) const override;
		virtual void draw(void) const override;
		virtual void update(void) override;
		virtual void load(void) override;
		virtual void unload(void) override;
		virtual ~Title(void);
	};
}

#endif