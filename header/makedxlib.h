//makedxlib.h

#ifndef __MAKE_DX_LIB_H
#define __MAKE_DX_LIB_H

#include "DxLib.h"
#include <boost/noncopyable.hpp>
#include <stdexcept>

namespace gp
{
	class MakeDxLib : public boost::noncopyable
	{
		private:
			void process_loop(void);
			int window_mode;
			bool kill_flag;
		public:
			explicit MakeDxLib(int _window_mode); //�h���N���X�̃R���X�g���N�^�ŌĂ΂�Ȃ���΂Ȃ�Ȃ�(C++11�ł̓R���X�g���N�^�̌p���ł���)
			virtual void one_loop(void) = 0; //�h���N���X�ŃI�[�o�[���[�h����1flame������̏������L�q����
			virtual bool fin_loop_if(void) = 0; //���[�v�̏I���������L�q����(�]����one_loop�̌�ɍs����)
			void game_main(void);
			virtual ~MakeDxLib(void);
	};
}

#endif

