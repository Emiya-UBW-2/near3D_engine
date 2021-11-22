#include "sub.hpp"

static const float Frame_Rate{ 90.f };


enum Key {
	KEY_M_LEFT,
	KEY_M_RIGHT,
	KEY_NO_1,
	KEY_NO_2,
	KEY_NO_3,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_ESCAPE,
	KEY_PAUSE,
	ACTIVE,
	ON_PAD,
};
struct input {
	std::array<bool, 13> get; /*キー用(一時監視)*/
	int m_x, m_y;
};
//出力
struct output {
	int x, y;
};


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	bool ending{ true };							//終了処理フラグ



	char Path[MAX_PATH];
	// EXEのあるフォルダのパスを取得
	GetModuleFileName(NULL, Path, MAX_PATH);
	// カレントディレクトリの設定
	SetCurrentDirectory(Path);

	using namespace std::literals;
	input in{ 0 };
	Draw::pos2D PlayerPos;
	DINPUT_JOYSTATE info;
	auto OPTPTs = std::make_shared<OPTION>();								//設定読み込み
	auto DrawPts = std::make_shared<DXDraw>("FPS_n2", OPTPTs, Frame_Rate);		//汎用
	auto DebugPTs = std::make_shared<DeBuG>(Frame_Rate);					//デバッグ
	OPTPTs->Set_useVR(DrawPts->use_vr);
	auto drawparts = std::make_unique<Draw>(); /*描画クラス*/
	do {
		//*
		if (!drawparts->write_map("map1")) { 
			return 0;
		}
		//*/
		drawparts->set_map(&PlayerPos.x, &PlayerPos.y,"map1");
		while (ProcessMessage() == 0) {
			const auto waits = GetNowHiPerformanceCount();
			DebugPTs->put_way();
			//入力
			{
				SetJoypadDeadZone(DX_INPUT_PAD1, 0.0);
				GetJoypadDirectInputState(DX_INPUT_PAD1, &info);
				in.get[KEY_PAUSE] = CheckHitKey(KEY_INPUT_P) != 0;
				in.get[ACTIVE] = GetActiveFlag() == TRUE;
				in.get[ON_PAD] = GetJoypadNum() >= 1 && true;
				if (in.get[ON_PAD]) {
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
			//
			{
				if (in.get[KEY_UP])
					PlayerPos.y += 2;
				if (in.get[KEY_DOWN])
					PlayerPos.y -= 2;
				if (in.get[KEY_LEFT])
					PlayerPos.x += 2;
				if (in.get[KEY_RIGHT])
					PlayerPos.x -= 2;
			}
			//出力
			{
				drawparts->set_cam(PlayerPos.x, PlayerPos.y);//カメラ配置設定
				drawparts->move_human(&PlayerPos.x, &PlayerPos.y);//プレイヤー配置設定
				drawparts->ready_player();
				drawparts->put_drw();//描画
			}
			//表示
			SetDrawScreen(DX_SCREEN_BACK);
			ClearDrawScreen();
			{
				drawparts->out_draw(); //表示
				DebugPTs->end_way();
				DebugPTs->debug(10, 100, float(GetNowHiPerformanceCount() - waits) / 1000.f);
			}
			//画面の反映
			DrawPts->Screen_Flip();
			//終了判定
			if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {
				ending = false;
				break;
			}
		}
		drawparts->exit_map();
	} while (ending);
	return 0; // ソフトの終了
}
