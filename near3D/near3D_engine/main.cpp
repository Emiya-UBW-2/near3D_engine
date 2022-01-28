#include "sub.hpp"

static const float Frame_Rate{ 90.f };

int CheckHitKey_M(int KeyCode) {
	if (GetWindowActiveFlag()) {
		return CheckHitKey(KeyCode);
	}
	else {
		return 0;
	}
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	bool ending{ true };													//�I�������t���O
	std::array<switchs, (int)Key::NUM> KEY;									/*�L�[�p(�ꎞ�Ď�)*/
	int m_x, m_y;
	DINPUT_JOYSTATE info;
	Near3D::Vector2D_I CameraPos;
	float X = 0, Y = 0, RM_PX = 0, RM_PY = 0, RM_PressTimer = 0.f;
	GraphHandle Screen;
	GraphHandle Screen2;
	//�V�F�[�_�[
	Near3D::shaders::shader_Vertex Screen_vertex;					// ���_�f�[�^
	std::array<Near3D::shaders, 2> shader2D;

	auto OPTPTs = std::make_shared<OPTION>();								//�ݒ�ǂݍ���
	auto DrawPts = std::make_shared<DXDraw>("FPS_n2", OPTPTs, Frame_Rate);	//�ėp
	auto DebugPTs = std::make_shared<DeBuG>(Frame_Rate);					//�f�o�b�O
	OPTPTs->Set_useVR(DrawPts->use_vr);
	auto Near3DPts = std::make_unique<Near3D::Near3DControl>(DrawPts);		//�`��N���X
	auto Near3DEdit = std::make_unique<Near3D::Near3DEditer>(DrawPts);		//�G�f�B�^�[�p�N���X

	//�V�F�[�_�[
	Screen = GraphHandle::Make(DrawPts->disp_x, DrawPts->disp_y, true);
	Screen2 = GraphHandle::Make(DrawPts->disp_x, DrawPts->disp_y, true);
	Screen_vertex.Set(DrawPts);																					// ���_�f�[�^�̏���
	shader2D[0].Init("VS_lens.vso", "PS_lens.pso");																//�����Y

	/*
	if (!Near3DEdit->Chara_Editer(1)) {
		return 0;
	}
	//*/
	/*
	if (!Near3DEdit->Map_Editer("map1")) {
		return 0;
	}
	//*/
	Near3DPts->Start(0, 0, 0);//�n�`�ǂݍ���
	LONGLONG wait5_t = GetNowHiPerformanceCount();
	do {
		wait5_t -= GetNowHiPerformanceCount();
		Near3DPts->Ready();//�J�n������
		while (ProcessMessage() == 0) {
			clsDx();
			const auto waits = GetNowHiPerformanceCount();
			DebugPTs->put_way();
			//����
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
					KEY[(int)Key::KEY_M_LEFT].GetInput((GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
					KEY[(int)Key::KEY_M_MID].GetInput((GetMouseInput() & MOUSE_INPUT_MIDDLE) != 0);
					KEY[(int)Key::KEY_M_RIGHT].GetInput((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0);
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
			//�o��
			{
				Near3D::Vector2D_I Playervec;
				{
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
						if (RM_PressTimer > 1.f) {
							int x_m, y_m;
							GetMousePoint(&x_m, &y_m);
							x_m -= DrawPts->disp_x / 2;
							y_m -= DrawPts->disp_y / 2;
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
				}
				//�X�V
				int Info = 0;
				Info |= (KEY[(int)Key::KEY_NO_4].trigger()) ? (1 << 0) : 0;
				Info |= (KEY[(int)Key::KEY_NO_1].press()) ? (1 << 1) : 0;
				Info |= (KEY[(int)Key::KEY_M_RIGHT].on()) ? (1 << 2) : 0;
				Info |= (KEY[(int)Key::KEY_M_LEFT].press()) ? (1 << 3) : 0;
				Info |= (KEY[(int)Key::KEY_NO_3].press()) ? (1 << 4) : 0;
				Info |= (KEY[(int)Key::KEY_NO_2].press()) ? (1 << 5) : 0;
				Info |= (KEY[(int)Key::KEY_NO_5].trigger()) ? (1 << 6) : 0;
				Info |= (KEY[(int)Key::KEY_NO_6].trigger()) ? (1 << 7) : 0;
				printfDx("%d \n", Info);

				Near3DPts->Update(
					Playervec,
					Info,
					/*
					KEY[(int)Key::KEY_NO_4].trigger(),
					KEY[(int)Key::KEY_NO_1].press(),
					KEY[(int)Key::KEY_M_RIGHT].on(),
					KEY[(int)Key::KEY_M_LEFT].press(),
					KEY[(int)Key::KEY_NO_3].press(),
					KEY[(int)Key::KEY_NO_2].press(),
					KEY[(int)Key::KEY_NO_5].trigger(),
					KEY[(int)Key::KEY_NO_6].trigger(),
					*/
					CameraPos);
			}
			//�\��
			Screen2.SetDraw_Screen(true);
			{
				Near3DPts->Output(); //�\��
			}
			SetUseTextureToShader(0, Screen2.get());	//�g�p����e�N�X�`�����Z�b�g
			{
				//�����Y�`��
				shader2D[0].Set_dispsize(DrawPts->disp_x, DrawPts->disp_y);
				shader2D[0].Set_param(float(DrawPts->disp_x) / 2.f, float(DrawPts->disp_y) / 2.f, 300, 3);
				Screen.SetDraw_Screen();
				{
					shader2D[0].Draw(Screen_vertex);
				}
			}
			SetUseTextureToShader(0, -1);	//�g�p����e�N�X�`�����Z�b�g
			GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
			{
				Screen.DrawGraph(0, 0, true);

				Near3DPts->Output_UI(); //�\��

				DebugPTs->end_way();
				DebugPTs->debug(10, 100, float(GetNowHiPerformanceCount() - waits) / 1000.f);

				printfDx("Load:%fms \n", float(-wait5_t) / 1000.f);
			}
			//��ʂ̔��f
			DrawPts->Screen_Flip();
			//�I������
			if (CheckHitKey_M(KEY_INPUT_ESCAPE) != 0) {
				ending = false;
				break;
			}
			//���̃X�e�[�W��
			{
				auto OLD_POS = Near3DPts->PlayerPos();
				if (Near3DPts->GetNextStage()) {
					CameraPos -= (OLD_POS - Near3DPts->PlayerPos())*-1;
					X = (float)CameraPos.x;
					Y = (float)CameraPos.y;
					wait5_t = GetNowHiPerformanceCount();
					Near3DPts->NextStage();//���}�b�v�w��
					break;
				}
			}
		}
	} while (ending);
	Near3DPts->Dispose();
	return 0; // �\�t�g�̏I��
}
