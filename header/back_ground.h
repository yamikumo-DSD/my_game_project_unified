//back_ground.h

#ifndef __BACK_GROUND_H
#define __BACK_GROUND_H

namespace MyGameProject
{
	class BackGround
	{
	private:
		int count;
		virtual void custom_updater(void) = 0;
	public:
		BackGround(void);
		//virtual void draw(void) const = 0;
		virtual void draw_rear(void) const{}
		virtual void draw_front(void) const{}
		int get_count(void) const;
		void update(void);
		virtual ~BackGround(void);
	};
}

#endif
