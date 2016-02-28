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
			explicit MakeDxLib(int _window_mode); //派生クラスのコンストラクタで呼ばれなければならない(C++11ではコンストラクタの継承でも可)
			virtual void one_loop(void) = 0; //派生クラスでオーバーロードして1flameあたりの処理を記述する
			virtual bool fin_loop_if(void) = 0; //ループの終了条件を記述する(評価はone_loopの後に行われる)
			void game_main(void);
			virtual ~MakeDxLib(void);
	};
}

#endif

