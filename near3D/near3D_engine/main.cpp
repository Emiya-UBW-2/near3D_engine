#include "sub.hpp"

//設定するフレームレート
static const float Frame_Rate{ 90.f };

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	bool ending{ true };													//終了処理フラグ
	std::array<switchs, (int)Key::NUM> KEY;									/*キー用(一時監視)*/
	int m_x, m_y;
	DINPUT_JOYSTATE info;
	Near3D::Vector2D_I CameraPos;
	float X = 0, Y = 0, RM_PX = 0, RM_PY = 0, RM_PressTimer = 0.f;
	GraphHandle Screen, Screen_buf;
	shaders::shader_Vertex Screen_vertex;							// 頂点データ
	std::array<shaders, 1> shader2D;								//シェーダー
	OPTION::Create();														//設定読み込み
	auto* OptionParts = OPTION::Instance();
	OptionParts->Load();
	DXDraw::Create("FPS_n2", Frame_Rate);			//汎用
	DeBuG::Create(Frame_Rate);					//デバッグ
	auto* DrawParts = DXDraw::Instance();
	OptionParts->Set_useVR(DrawParts->use_vr);
	auto Near3DPts = std::make_unique<Near3D::Near3DControl>();		//描画クラス
	auto Near3DEdit = std::make_unique<Near3D::Near3DEditer>();		//エディター用クラス
	Screen = GraphHandle::Make(DrawParts->disp_x, DrawParts->disp_y, true);
	Screen_buf = GraphHandle::Make(DrawParts->disp_x, DrawParts->disp_y, true);
	Screen_vertex.Set();												// 頂点データの準備
	shader2D[0].Init("VS_Vignette.vso", "PS_Vignette.pso");					//ケラレ
	//if (!Near3DEdit->Chara_Editer(1)) { return 0; }
	/*
	std::string Mapname = "map00_00";
	if (!Near3DEdit->Map_Editer_Select(&Mapname)) { return 0; }
	if (!Near3DEdit->Map_Editer(Mapname)) { return 0; }
	//*/
	//Near3DEdit->Map_builder();
	Near3DPts->Start(0, Near3D::Vector2D_I::Get(0, 0));						//地形読み込み
	CameraPos = Near3DPts->PlayerPos()*-1;
	X = (float)CameraPos.x;
	Y = (float)CameraPos.y;
	do {
		Near3DPts->Ready();//開始時処理
		while (ProcessMessage() == 0) {
			clsDx();
			const auto waits = GetNowHiPerformanceCount();
			DeBuG::Instance()->put_way();
			//入力
			{
				SetJoypadDeadZone(DX_INPUT_PAD1, 0.0);
				GetJoypadDirectInputState(DX_INPUT_PAD1, &info);
				KEY[(int)Key::KEY_PAUSE].GetInput(CheckHitKey_M(KEY_INPUT_P) != 0);
				KEY[(int)Key::ACTIVE].GetInput(GetActiveFlag() == TRUE);
				KEY[(int)Key::ON_PAD].GetInput(GetJoypadNum() >= 1);
				if (KEY[(int)Key::ON_PAD].press()) {
					m_x = info.Z;
					m_y = info.Rz;
					KEY[(int)Key::KEY_M_LEFT].GetInput(info.Buttons[7] != 0);
					//KEY[(int)Key::KEY_M_MID].GetInput(info.Buttons[5] != 0);
					KEY[(int)Key::KEY_M_RIGHT].GetInput(info.Buttons[5] != 0);
					KEY[(int)Key::KEY_NO_1].GetInput(info.Buttons[6] != 0);
					KEY[(int)Key::KEY_NO_2].GetInput(info.Buttons[10] != 0);
					KEY[(int)Key::KEY_NO_3].GetInput(info.Buttons[2] != 0);
					//KEY[(int)Key::KEY_NO_4].GetInput(info.Buttons[2] != 0);
					KEY[(int)Key::KEY_UP].GetInput(info.Y <= -500);
					KEY[(int)Key::KEY_DOWN].GetInput(info.Y >= 500);
					KEY[(int)Key::KEY_LEFT].GetInput(info.X <= -500);
					KEY[(int)Key::KEY_RIGHT].GetInput(info.X >= 500);
				}
				else {
					KEY[(int)Key::KEY_M_LEFT].GetInput((GetMouseInput_M() & MOUSE_INPUT_LEFT) != 0);
					KEY[(int)Key::KEY_M_MID].GetInput((GetMouseInput_M() & MOUSE_INPUT_MIDDLE) != 0);
					KEY[(int)Key::KEY_M_RIGHT].GetInput((GetMouseInput_M() & MOUSE_INPUT_RIGHT) != 0);
					KEY[(int)Key::KEY_NO_1].GetInput(CheckHitKey_M(KEY_INPUT_LSHIFT) != 0);
					KEY[(int)Key::KEY_NO_2].GetInput(CheckHitKey_M(KEY_INPUT_SPACE) != 0);
					KEY[(int)Key::KEY_NO_3].GetInput(CheckHitKey_M(KEY_INPUT_R) != 0);
					KEY[(int)Key::KEY_NO_4].GetInput(CheckHitKey_M(KEY_INPUT_X) != 0);
					KEY[(int)Key::KEY_NO_5].GetInput(CheckHitKey_M(KEY_INPUT_L) != 0);
					KEY[(int)Key::KEY_NO_6].GetInput(CheckHitKey_M(KEY_INPUT_F) != 0);
					KEY[(int)Key::KEY_UP].GetInput((CheckHitKey_M(KEY_INPUT_W) != 0 || CheckHitKey_M(KEY_INPUT_UP) != 0));
					KEY[(int)Key::KEY_DOWN].GetInput((CheckHitKey_M(KEY_INPUT_S) != 0 || CheckHitKey_M(KEY_INPUT_DOWN) != 0));
					KEY[(int)Key::KEY_LEFT].GetInput((CheckHitKey_M(KEY_INPUT_A) != 0 || CheckHitKey_M(KEY_INPUT_LEFT) != 0));
					KEY[(int)Key::KEY_RIGHT].GetInput((CheckHitKey_M(KEY_INPUT_D) != 0 || CheckHitKey_M(KEY_INPUT_RIGHT) != 0));
				}
			}
			//出力
			{
				Near3D::Vector2D_I Playervec;
				Playervec.set(0, 0);
				if (KEY[(int)Key::KEY_UP].press())
					Playervec.y -= 1;
				if (KEY[(int)Key::KEY_DOWN].press())
					Playervec.y += 1;
				if (KEY[(int)Key::KEY_LEFT].press())
					Playervec.x -= 1;
				if (KEY[(int)Key::KEY_RIGHT].press())
					Playervec.x += 1;
				if (KEY[(int)Key::KEY_M_MID].press()) {
					RM_PressTimer += 1.f / FPS;
					if (RM_PressTimer > 0.1f) {
						int x_m, y_m;
						GetMousePoint(&x_m, &y_m);
						x_m -= DrawParts->disp_x / 2;
						y_m -= DrawParts->disp_y / 2;
						easing_set(&RM_PX, (float)-x_m / 2, 0.9f);
						easing_set(&RM_PY, (float)-y_m / 2, 0.9f);
					}
				}
				else {
					RM_PressTimer = 0.f;
					easing_set(&RM_PX, 0.f, 0.9f);
					easing_set(&RM_PY, 0.f, 0.9f);
				}
				easing_set(&X, (float)(Near3DPts->PlayerPos()*-1.f).x + RM_PX, 0.925f);
				easing_set(&Y, (float)(Near3DPts->PlayerPos()*-1.f).y + RM_PY, 0.925f);
				CameraPos.set((int)X, (int)Y);
				int Info = 0;
				Info |= (KEY[(int)Key::KEY_NO_4].trigger()) ? (1 << 0) : 0;
				Info |= (KEY[(int)Key::KEY_NO_1].press()) ? (1 << 1) : 0;
				Info |= (KEY[(int)Key::KEY_M_RIGHT].on()) ? (1 << 2) : 0;
				Info |= (KEY[(int)Key::KEY_M_LEFT].press()) ? (1 << 3) : 0;
				Info |= (KEY[(int)Key::KEY_NO_3].press()) ? (1 << 4) : 0;
				Info |= (KEY[(int)Key::KEY_NO_2].press()) ? (1 << 5) : 0;
				Info |= (KEY[(int)Key::KEY_NO_5].trigger()) ? (1 << 6) : 0;
				Info |= (KEY[(int)Key::KEY_NO_6].trigger()) ? (1 << 7) : 0;
				Near3DPts->Update(Playervec, Info, CameraPos);	//更新
			}
			//表示
			Screen_buf.SetDraw_Screen(true);
			{
				Near3DPts->Output(); //表示
			}
			SetUseTextureToShader(0, Screen_buf.get());	//使用するテクスチャをセット
			{
				//レンズ描画
				shader2D[0].Set_dispsize();
				shader2D[0].Set_param(float(DrawParts->disp_x) / 2.f, float(DrawParts->disp_y) / 2.f, 300, 3);
				Screen.SetDraw_Screen();
				{
					shader2D[0].Draw(Screen_vertex);
				}
			}
			SetUseTextureToShader(0, -1);	//使用するテクスチャをセット
			GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
			{
				Screen.DrawGraph(0, 0, true);
				//UI
				Near3DPts->Output_UI();
				//
				DeBuG::Instance()->end_way();
				DeBuG::Instance()->debug(10, 100, float(GetNowHiPerformanceCount() - waits) / 1000.f);
			}
			//画面の反映
			printfDx("%d,%d\n", Near3DPts->PlayerPos().x, Near3DPts->PlayerPos().y);
			DrawParts->Screen_Flip();
			//終了判定
			if (CheckHitKey_M(KEY_INPUT_ESCAPE) != 0) {
				ending = false;
				break;
			}
			//次のステージへ
			{
				auto OLD_POS = Near3DPts->PlayerPos();
				if (Near3DPts->Get_NextStage()) {
					CameraPos -= (OLD_POS - Near3DPts->PlayerPos())*-1;
					X = (float)CameraPos.x;
					Y = (float)CameraPos.y;
					Near3DPts->NextStage();//次マップ指定
					break;
				}
			}
		}
	} while (ending);
	Near3DPts->Dispose();
	return 0; // ソフトの終了
}
