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
	KEY_NO_4,
	KEY_NO_5,
	KEY_NO_6,
	NUM,
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	bool ending{ true };							//終了処理フラグ

	std::array<switchs, Key::NUM> KEY; /*キー用(一時監視)*/
	int m_x, m_y;
	DINPUT_JOYSTATE info;

	auto OPTPTs = std::make_shared<OPTION>();								//設定読み込み
	auto DrawPts = std::make_shared<DXDraw>("FPS_n2", OPTPTs, Frame_Rate);	//汎用
	auto DebugPTs = std::make_shared<DeBuG>(Frame_Rate);					//デバッグ
	OPTPTs->Set_useVR(DrawPts->use_vr);

	auto Near3DPts = std::make_unique<Near3D::Near3DControl>(DrawPts);		//描画クラス
	Near3D::Near3DControl::Vector2D_I CameraPos;
	float X=0, Y=0;
	do {
		/*
		if (!Near3DPts->Map_Editer("map1")) {
			return 0;
		}
		//*/
		Near3DPts->Start(0, "map1");
		while (ProcessMessage() == 0) {
			clsDx();

			const auto waits = GetNowHiPerformanceCount();
			DebugPTs->put_way();
			//入力
			{
				SetJoypadDeadZone(DX_INPUT_PAD1, 0.0);
				GetJoypadDirectInputState(DX_INPUT_PAD1, &info);
				KEY[KEY_PAUSE].GetInput(CheckHitKey(KEY_INPUT_P) != 0);
				KEY[ACTIVE].GetInput(GetActiveFlag() == TRUE);
				KEY[ON_PAD].GetInput(GetJoypadNum() >= 1);
				if (KEY[ON_PAD].press()) {
					m_x = info.Z;
					m_y = info.Rz;
					KEY[KEY_M_LEFT].GetInput(info.Buttons[7] != 0);
					KEY[KEY_M_RIGHT].GetInput(info.Buttons[5] != 0);
					KEY[KEY_NO_1].GetInput(info.Buttons[6] != 0);
					KEY[KEY_NO_2].GetInput(info.Buttons[10] != 0);
					KEY[KEY_NO_3].GetInput(info.Buttons[2] != 0);
					//KEY[KEY_NO_4].GetInput(info.Buttons[2] != 0);
					KEY[KEY_UP].GetInput(info.Y <= -500);
					KEY[KEY_DOWN].GetInput(info.Y >= 500);
					KEY[KEY_LEFT].GetInput(info.X <= -500);
					KEY[KEY_RIGHT].GetInput(info.X >= 500);
				}
				else {
					KEY[KEY_M_LEFT].GetInput((GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
					KEY[KEY_M_RIGHT].GetInput((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0);
					KEY[KEY_NO_1].GetInput(CheckHitKey(KEY_INPUT_LSHIFT) != 0);
					KEY[KEY_NO_2].GetInput(CheckHitKey(KEY_INPUT_SPACE) != 0);
					KEY[KEY_NO_3].GetInput(CheckHitKey(KEY_INPUT_R) != 0);
					KEY[KEY_NO_4].GetInput(CheckHitKey(KEY_INPUT_X) != 0);
					KEY[KEY_NO_5].GetInput(CheckHitKey(KEY_INPUT_L) != 0);
					KEY[KEY_NO_6].GetInput(CheckHitKey(KEY_INPUT_F) != 0);
					KEY[KEY_UP].GetInput((CheckHitKey(KEY_INPUT_W) != 0 || CheckHitKey(KEY_INPUT_UP) != 0));
					KEY[KEY_DOWN].GetInput((CheckHitKey(KEY_INPUT_S) != 0 || CheckHitKey(KEY_INPUT_DOWN) != 0));
					KEY[KEY_LEFT].GetInput((CheckHitKey(KEY_INPUT_A) != 0 || CheckHitKey(KEY_INPUT_LEFT) != 0));
					KEY[KEY_RIGHT].GetInput((CheckHitKey(KEY_INPUT_D) != 0 || CheckHitKey(KEY_INPUT_RIGHT) != 0));
				}
			}
			//出力
			{
				Near3D::Near3DControl::Vector2D_I Playervec;
				Playervec.set(0, 0);
				if (KEY[KEY_UP].press())
					Playervec.y -= 1;
				if (KEY[KEY_DOWN].press())
					Playervec.y += 1;
				if (KEY[KEY_LEFT].press())
					Playervec.x -= 1;
				if (KEY[KEY_RIGHT].press())
					Playervec.x += 1;

				Near3DPts->Update_Human(
					Playervec,
					KEY[KEY_NO_4].trigger(),
					KEY[KEY_NO_1].press(),
					KEY[KEY_M_RIGHT].on(),
					KEY[KEY_M_LEFT].press(),
					KEY[KEY_NO_3].press(),
					KEY[KEY_NO_2].press(),
					KEY[KEY_NO_5].trigger(),
					KEY[KEY_NO_6].trigger()
				);//プレイヤー配置設定

				easing_set(&X, (float)(Near3DPts->PlayerPos()*-1.f).x, 0.925f);
				easing_set(&Y, (float)(Near3DPts->PlayerPos()*-1.f).y, 0.925f);
				CameraPos.set(X, Y);
				Near3DPts->Update(CameraPos);//更新
			}
			//表示
			GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
			{
				Near3DPts->Output(); //表示
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
			if (CheckHitKey(KEY_INPUT_P) != 0) {
				break;
			}
		}
		Near3DPts->Dispose();
	} while (ending);
	return 0; // ソフトの終了
}
