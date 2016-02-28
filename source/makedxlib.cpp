//makedxlib.cpp

#include "makedxlib.h"
#include "fps.h"
#include "msgbox.h"
#include "is_debug_mode.h"

gp::MakeDxLib::MakeDxLib(int _window_mode) :window_mode(_window_mode), kill_flag(1)
{
	if (ChangeWindowMode(_window_mode)){throw std::runtime_error("Fail to change window mode."); };
	if (DxLib_Init()){throw std::runtime_error("Fail to initialize DXƒ‰ƒCƒuƒ‰ƒŠ."); }
	kill_flag = 0; //When superior statements throw an exception, kill_flag remains true.
	if (SetDrawScreen(DX_SCREEN_BACK)){throw std::runtime_error("Fail to set screen back."); };
}

void gp::MakeDxLib::process_loop(void)
{
	if (ScreenFlip() || ClearDrawScreen())
	{ 
		throw std::runtime_error("Fail to update the game screen.");
	}
}

void gp::MakeDxLib::game_main(void)
{
	while (1)
	{
		try{process_loop();}
		catch (const std::exception& _e)
		{
			DxLib_End();
			throw _e;
		}
		one_loop();//Body of the game.
		gp::fps_wait(); // NEW
		if (fin_loop_if() || ProcessMessage() == -1)
		{
#if PUT_BREAK_POINT
			msgbox_ok("Before calling DxLib_End()");
#endif
			DxLib_End();
			break; 
		}
	}
}

gp::MakeDxLib::~MakeDxLib(void)
{
	//if (kill_flag){ DxLib_End();}
}

