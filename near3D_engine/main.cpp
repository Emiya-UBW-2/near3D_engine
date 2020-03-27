#define NOMINMAX
#include "sub.hpp"
#include "useful.hpp"
#include "make_thread.hpp"
#include <iostream>
#include <fstream>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	using namespace std::literals;
	input in{ 0 };
	output out{ 0 };
	DINPUT_JOYSTATE info;

	auto threadparts = std::make_unique<ThreadClass>(); /*演算クラス*/
	auto parts = std::make_unique<MainClass>(); /*汎用クラス*/
	auto drawparts = std::make_unique<Draw>(x_r(64)); /*描画クラス*/
	auto debug = std::make_unique<DeBuG>(); /*デバッグ描画クラス*/

	if (parts->write_setting())
		return 0;

	const auto font72 = FontHandle::Create(x_r(72), y_r(72 / 3), DX_FONTTYPE_ANTIALIASING);
	const auto screen = MakeScreen(dispx, dispy * 2);
	threadparts->thread_start(in, out);
	do {
		drawparts->set_map(&out.x, &out.y);
		while (ProcessMessage() == 0 && !out.ends) {
			const auto waits = GetNowHiPerformanceCount();
			debug->put_way();
			SetDrawScreen(screen);
			ClearDrawScreen();
			{
				drawparts->set_cam(out.x, out.y);//カメラ配置設定
				drawparts->put_map();
				drawparts->setpos_player(out.x, out.y);
				drawparts->put_drw();
			}
			SetDrawScreen(DX_SCREEN_BACK);
			ClearDrawScreen();
			{
				DrawGraph(0, 0, screen, TRUE);
				debug->end_way();
				debug->debug(GetFPS(), float(GetNowHiPerformanceCount() - waits)*0.001f);
			}
			parts->Screen_Flip(waits);
			{
				SetJoypadDeadZone(DX_INPUT_PAD1, 0.0);
				GetJoypadDirectInputState(DX_INPUT_PAD1, &info);
				in.get[KEY_ESCAPE] = CheckHitKey(KEY_INPUT_ESCAPE) != 0;
				in.get[KEY_PAUSE] = CheckHitKey(KEY_INPUT_P) != 0;
				in.get[ACTIVE] = GetActiveFlag() == TRUE;
				in.get[ON_PAD] = GetJoypadNum() >= 1 && parts->get_use_pad();
				if (in.get[8]) {
					in.m_x = info.Z;
					in.m_y = info.Rz;
					in.get[KEY_M_LEFT] = info.Buttons[7] != 0;
					in.get[KEY_M_RIGHT] = info.Buttons[5] != 0;
					in.get[KEY_NO_1] = info.Buttons[6] != 0;
					in.get[KEY_NO_2] = info.Buttons[10] != 0;
					in.get[KEY_NO_3] = info.Buttons[2] != 0;
					in.get[KEY_UP] = info.Y <= -500;
					in.get[KEY_DOWN] = info.Y >= 500;
					in.get[KEY_LEFT] = info.X <= -500;
					in.get[KEY_RIGHT] = info.X >= 500;
				}
				else {
					in.get[KEY_M_LEFT] = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
					in.get[KEY_M_RIGHT] = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;
					in.get[KEY_NO_1] = CheckHitKey(KEY_INPUT_LSHIFT) != 0;
					in.get[KEY_NO_2] = CheckHitKey(KEY_INPUT_SPACE) != 0;
					in.get[KEY_NO_3] = CheckHitKey(KEY_INPUT_R) != 0;
					in.get[KEY_UP] = (CheckHitKey(KEY_INPUT_W) != 0 || CheckHitKey(KEY_INPUT_UP) != 0);
					in.get[KEY_DOWN] = (CheckHitKey(KEY_INPUT_S) != 0 || CheckHitKey(KEY_INPUT_DOWN) != 0);
					in.get[KEY_LEFT] = (CheckHitKey(KEY_INPUT_A) != 0 || CheckHitKey(KEY_INPUT_LEFT) != 0);
					in.get[KEY_RIGHT] = (CheckHitKey(KEY_INPUT_D) != 0 || CheckHitKey(KEY_INPUT_RIGHT) != 0);
				}
			}
		}
		drawparts->exit_map();
	} while (ProcessMessage() == 0 && !out.ends);
	threadparts->thead_stop();
	return 0; // ソフトの終了
}
