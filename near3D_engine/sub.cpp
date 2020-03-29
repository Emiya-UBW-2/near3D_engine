#include "sub.hpp"

bool MainClass::write_setting(void) {
	DINPUT_JOYSTATE info;
	std::array<bool, 13> get; /*キー用(一時監視)*/

	bool out = false;
	int x = 0;//1920 - 700;
	int y = 1080 - 580;
	int addx;

	const auto font72 = FontHandle::Create(x_r(72), y_r(72 / 3), DX_FONTTYPE_NORMAL);

	int selpos[5] = { 0 }, selaim_o[5] = { 0 }, selaim[5] = { 0 }, selpadd[5] = { 0 };
	int selup[5], seldn[5];
	int select = 0;
	int choseup = 0, chosedn = 0;

	for (int i = 0; i < 5; i++) {
		selup[i] = 1;
		seldn[i] = 1;
	}

	while (ProcessMessage() == 0) {
		const auto waits = GetNowHiPerformanceCount();

		SetJoypadDeadZone(DX_INPUT_PAD1, 0.0);
		GetJoypadDirectInputState(DX_INPUT_PAD1, &info);

		get[0] = CheckHitKey(KEY_INPUT_ESCAPE) != 0;
		get[1] = CheckHitKey(KEY_INPUT_P) != 0;
		get[8] = GetJoypadNum() >= 1 && use_pad;
		if (get[8]) {
			get[2] = info.Buttons[7] != 0;
			get[3] = info.Buttons[5] != 0;
			get[4] = info.Buttons[6] != 0;
			get[5] = info.Buttons[10] != 0;
			get[6] = info.Buttons[2] != 0;
			get[7] = info.Buttons[1] != 0;
			get[9] = info.Y <= -500;
			get[10] = info.Y >= 500;
			get[11] = info.X <= -500;
			get[12] = info.X >= 500;
		}
		else {
			get[2] = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
			get[3] = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;
			get[4] = CheckHitKey(KEY_INPUT_LSHIFT) != 0;
			get[5] = CheckHitKey(KEY_INPUT_SPACE) != 0;
			get[6] = CheckHitKey(KEY_INPUT_R) != 0;
			get[7] = CheckHitKey(KEY_INPUT_SPACE) != 0;
			get[9] = (CheckHitKey(KEY_INPUT_W) != 0 || CheckHitKey(KEY_INPUT_UP) != 0);
			get[10] = (CheckHitKey(KEY_INPUT_S) != 0 || CheckHitKey(KEY_INPUT_DOWN) != 0);
			get[11] = (CheckHitKey(KEY_INPUT_A) != 0 || CheckHitKey(KEY_INPUT_LEFT) != 0);
			get[12] = (CheckHitKey(KEY_INPUT_D) != 0 || CheckHitKey(KEY_INPUT_RIGHT) != 0);
		}

		if (get[0]) {
			out = true;
			break;
		}
		if (get[7])
			break;

		for (int i = 0; i < 5; i++) {
			if (selup[i] == 1 || seldn[i] == 1) {
				if (selpadd[i] != 0) {
					selaim[i] = selpos[i];
				}
				selaim_o[i] = selaim[i];
				if (i == 0) { selaim[0] = 600 * USE_YSync; }
				if (i == 1) { selaim[1] = 600 * frate / 120; }
				if (i == 2) { selaim[2] = 600 * se_vol / 100; }
				if (i == 3) { selaim[3] = 600 * bgm_vol / 100; }
				if (i == 4) { selaim[4] = 600 * use_pad; }
				selpadd[i] = int(sqrt(2 * abs(selaim[i] - selaim_o[i]))) * ((selaim[i] - selaim_o[i] >= 0) ? 1 : -1);
				selup[i] = 2;
				seldn[i] = 2;
			}
		}

		if (get[9]) {
			if (choseup == 0) {
				select--;
				if (select < 0)
					select = 5 - 1;
				choseup = 1;
			}
		}
		else {
			choseup = 0;
		}

		if (get[10]) {
			if (chosedn == 0) {
				select++;
				if (select > 5 - 1)
					select = 0;
				chosedn = 1;
			}
		}
		else {
			chosedn = 0;
		}

		if (get[11]) {
			if (selup[select] == 0) {
				if (select == 0) {
					USE_YSync ^= 1;
					if (USE_YSync) {
						frate = refrate;
						selup[1] = 1;
					}
				}
				if (!USE_YSync) {
					if (select == 1) { frate = std::max(frate - 30, 30); }
				}
				if (select == 2) { se_vol = std::max(se_vol - 10, 0); }
				if (select == 3) { bgm_vol = std::max(bgm_vol - 10, 0); }
				if (select == 4) { use_pad ^= 1; }
				selup[select] = 1;
			}
		}
		else {
			selup[select] = 0;
		}

		if (get[12]) {
			if (seldn[select] == 0) {
				if (select == 0) {
					USE_YSync ^= 1;
					if (USE_YSync) {
						frate = refrate;
						selup[1] = 1;
					}
				}
				if (!USE_YSync) {
					if (select == 1) { frate = std::min(frate + 30, 120); }
				}
				if (select == 2) { se_vol = std::min(se_vol + 10, 100); }
				if (select == 3) { bgm_vol = std::min(bgm_vol + 10, 100); }
				if (select == 4) { use_pad ^= 1; }
				seldn[select] = 1;
			}
		}
		else {
			seldn[select] = 0;
		}

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();

		font72.DrawStringFormat(0, y_r(72 * 0), GetColor(255, 255, 255), "press space %d", select);

		addx = ((select == 0) ? 100 : 0);
		DrawLine(x_r(x), y_r(y + 72 * 1 + 71), x_r(x + addx + 600), y_r(y + 72 * 1 + 71), GetColor(128, 128, 128));
		DrawLine(x_r(x), y_r(y + 72 * 1 + 71), x_r(x + selpos[0] * (addx + 600) / 600), y_r(y + 72 * 1 + 71), GetColor(0, 255, 0));
		font72.DrawStringFormat(x_r(x + addx), y_r(y + 72 * 1), GetColor(255, 255, 255), "Sync    : %s", USE_YSync ? "true" : "false");
		if (select == 0) {
			font72.DrawString(x_r(x + addx + 600 + 100), y_r(y + 72 * 1), "垂直同期 の 設定", GetColor(255, 255, 255));
		}
		addx = ((select == 1) ? 100 : 0);
		DrawLine(x_r(x), y_r(y + 72 * 2 + 71), x_r(x + addx + 600), y_r(y + 72 * 2 + 71), GetColor(128, 128, 128));
		DrawLine(x_r(x), y_r(y + 72 * 2 + 71), x_r(x + selpos[1] * (addx + 600) / 600), y_r(y + 72 * 2 + 71), ((!USE_YSync) ? GetColor(0, 255, 0) : GetColor(0, 128, 0)));
		font72.DrawStringFormat(x_r(x + addx), y_r(y + 72 * 2), ((!USE_YSync) ? GetColor(255, 255, 255) : GetColor(128, 128, 128)), "Fps     : %d", frate);
		if (select == 1) {
			font72.DrawString(x_r(x + addx + 600 + 100), y_r(y + 72 * 2), "フレームレート", GetColor(255, 255, 255));
		}

		addx = ((select == 2) ? 100 : 0);
		DrawLine(x_r(x), y_r(y + 72 * 3 + 71), x_r(x + addx + 600), y_r(y + 72 * 3 + 71), GetColor(128, 128, 128));
		DrawLine(x_r(x), y_r(y + 72 * 3 + 71), x_r(x + selpos[2] * (addx + 600) / 600), y_r(y + 72 * 3 + 71), GetColor(0, 255, 0));
		font72.DrawStringFormat(x_r(x + addx), y_r(y + 72 * 3), GetColor(255, 255, 255), "vol se  : %d", se_vol);
		if (select == 2) {
			font72.DrawString(x_r(x + addx + 600 + 100), y_r(y + 72 * 3), "サウンド エフェクト", GetColor(255, 255, 255));
		}

		addx = ((select == 3) ? 100 : 0);
		DrawLine(x_r(x), y_r(y + 72 * 4 + 71), x_r(x + addx + 600), y_r(y + 72 * 4 + 71), GetColor(128, 128, 128));
		DrawLine(x_r(x), y_r(y + 72 * 4 + 71), x_r(x + selpos[3] * (addx + 600) / 600), y_r(y + 72 * 4 + 71), GetColor(0, 255, 0));
		font72.DrawStringFormat(x_r(x + addx), y_r(y + 72 * 4), GetColor(255, 255, 255), "vol bgm : %d", bgm_vol);
		if (select == 3) {
			font72.DrawString(x_r(x + addx + 600 + 100), y_r(y + 72 * 4), "バック ミュージック", GetColor(255, 255, 255));
		}

		addx = ((select == 4) ? 100 : 0);
		DrawLine(x_r(x), y_r(y + 72 * 5 + 71), x_r(x + addx + 600), y_r(y + 72 * 5 + 71), GetColor(128, 128, 128));
		DrawLine(x_r(x), y_r(y + 72 * 5 + 71), x_r(x + selpos[4] * (addx + 600) / 600), y_r(y + 72 * 5 + 71), GetColor(0, 255, 0));
		font72.DrawStringFormat(x_r(x + addx), y_r(y + 72 * 5), GetColor(255, 255, 255), "GamePad : %s", use_pad ? "true" : "false");
		if (select == 4) {
			font72.DrawString(x_r(x + addx + 600 + 100), y_r(y + 72 * 5), "ゲームパッドorキーボード", GetColor(255, 255, 255));
		}

		for (int i = 0; i < 5; i++) {
			if (selpadd[i] > 0) {
				selpos[i] += selpadd[i];
				selpadd[i]--;
			}
			else if (selpadd[i] < 0) {
				selpos[i] += selpadd[i];
				selpadd[i]++;
			}
			else {
				selpos[i] = selaim[i];
			}
		}
		Screen_Flip(waits);
	}
	if (!out) {
		std::ofstream outputfile("data/setting.txt");
		outputfile << "YSync(1or0)=" + std::to_string(USE_YSync) + "\n";
		outputfile << "fps(30or60or120)=" + std::to_string(frate) + "\n";
		outputfile << "se_vol(100~0)=" + std::to_string(se_vol) + "\n"; //
		outputfile << "bgm_vol(100~0)=" + std::to_string(bgm_vol) + "\n"; //
		outputfile << "use_gamepad(1or0)=" + std::to_string(use_pad) + "\n"; //
		outputfile.close();
	}
	return out;
}
MainClass::MainClass(void) {
	using namespace std::literals;
	SetOutApplicationLogValidFlag(FALSE); /*log*/
	refrate = GetRefreshRate();
	const auto mdata = FileRead_open("data/setting.txt", FALSE);
	{
		USE_YSync = bool(getparam_i(mdata));
		if (USE_YSync) {
			frate = refrate;
			getparam_i(mdata);
		}
		else {
			frate = getparam_i(mdata);
		}
		se_vol = getparam_i(mdata);
		bgm_vol = getparam_i(mdata);
		use_pad = bool(getparam_i(mdata));
	}
	FileRead_close(mdata);

	//SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);

	//SetWindowStyleMode(4);			    /**/
	//SetWindowUserCloseEnableFlag(FALSE);		    /*alt+F4対処*/
	SetMainWindowText("near3D");			    /*name*/
	SetAeroDisableFlag(TRUE);			    /**/
	SetUseDirect3DVersion(DX_DIRECT3D_9);		    /*directX ver*/
	SetEnableXAudioFlag(FALSE);			    /**/
	Set3DSoundOneMetre(1.0f);			    /*3Dsound*/
	SetGraphMode(dispx, dispy, 32);			    /*解像度*/
	SetWaitVSyncFlag(USE_YSync);			    /*垂直同期*/
	ChangeWindowMode(TRUE);		    /*窓表示*/
	//ChangeWindowMode(FALSE);		    /*窓表示*/
	SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);/**/
	DxLib_Init();					    /*init*/
	SetAlwaysRunFlag(TRUE);				    /*background*/
	//SetSysCommandOffFlag(TRUE)				//強制ポーズ対策()
}
MainClass::~MainClass(void) {
	DxLib_End();
}
void MainClass::Start_Screen(void){
	time = GetNowHiPerformanceCount();
}
void MainClass::Screen_Flip(void) {
	ScreenFlip();
	if (!USE_YSync && time.has_value()) {
		while (GetNowHiPerformanceCount() - time.value() < 1000000.0f / frate) {}
		time.reset();
	}
}
void MainClass::Screen_Flip(LONGLONG waits) {
	ScreenFlip();
	if (!USE_YSync)
		while (GetNowHiPerformanceCount() - waits < 1000000.0f / frate) {}
}

DeBuG::DeBuG(void) {
	font = FontHandle::Create(x_r(33), y_r(33 / 3), DX_FONTTYPE_NORMAL);
}
void DeBuG::put_way(void) {
	waypoint = GetNowHiPerformanceCount();
	seldeb = 0;
}
void DeBuG::end_way(void) {
	if (seldeb < 6)
		waydeb[seldeb++] = (float)(GetNowHiPerformanceCount() - waypoint) / 1000.0f;
}
void DeBuG::debug(void) {
	for (size_t j = std::size(deb) - 1; j >= 1; --j) {
		deb[j][0] = deb[j - 1][0];
	}
	for (size_t i = 0; i < std::size(waydeb); ++i) {
		if (seldeb - 1 <= i) {
			waydeb[i] = waydeb[seldeb - 1];
		}
		deb[0][i + 1] = waydeb[i];
		for (size_t j = std::size(deb) - 1; j >= 1; --j) {
			deb[j][i + 1] = deb[j - 1][i + 1];
		}
	}
	DrawBox(x_r(100), y_r(100 + 0), x_r(100 + 60 * 5), y_r(100 + 200), GetColor(255, 0, 0), FALSE);
	for (int j = 0; j < 60 - 1; ++j) {
		for (int i = 0; i < 6; ++i) {
			DrawLine(x_r(100 + j * 5), y_r(100 + (int)(200.f - deb[j][i + 1] * 10.f)), x_r(100 + (j + 1) * 5), y_r(100 + (int)(200.f - deb[j + 1][i + 1] * 10.f)), GetColor(50, 128 + 127 * i / 6, 50));
		}
	}
	const auto c_ffffff = GetColor(255, 255, 255);
	DrawLine(x_r(100), y_r(100 + 200 - 166), x_r(100 + 60 * 5), y_r(100 + 200 - 166), GetColor(0, 255, 0));
	float fps = GetFPS();
	font.DrawStringFormat(x_r(100), y_r(100 + 0), c_ffffff, "%05.2ffps (total %.2fms)", fps, 1000.0f / fps);

	font.DrawStringFormat(x_r(100), y_r(100 + 33), c_ffffff, "%d(%.2fms)", 0, waydeb[0]);
	for (size_t j = 1; j < std::size(waydeb); ++j) {
		font.DrawStringFormat(x_r(100), y_r(int(100 + 33 + j * 33)), c_ffffff, "%d(%.2fms)", j, waydeb[j] - waydeb[j - 1u]);
	}
}