#include "sub.hpp"

bool MainClass::write_setting(void) {
	DINPUT_JOYSTATE info;
	std::array<bool, 13> get; /*キー用(一時監視)*/

	bool out = false;
	int x = 0;//1920 - 700;
	int y = 1080 - 580;
	int addx;

	const auto font72 = FontHandle::Create(x_r(72), y_r(72 / 3), DX_FONTTYPE_NORMAL);

	int selpos[5] = { 0 }, selaim_o[5] = { 0 }, selaim[5] = { 0 }, selpadd[5] = {0};
	int selup[5],seldn[5];
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
					if (select == 1) { frate = std::max(frate - 30, 30);}
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
					if (select == 1) { frate = std::min(frate + 30, 120);}
				}
				if (select == 2) { se_vol =std::min(se_vol+10,100); }
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
			font72.DrawString(x_r(x + addx+600+100), y_r(y + 72 * 1), "垂直同期 の 設定", GetColor(255, 255, 255));
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
	//ChangeWindowMode(TRUE);		    /*窓表示*/
	ChangeWindowMode(FALSE);		    /*窓表示*/
	SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);/**/
	DxLib_Init();					    /*init*/
	SetAlwaysRunFlag(TRUE);				    /*background*/
	//SetSysCommandOffFlag(TRUE)				//強制ポーズ対策()
}
MainClass::~MainClass(void) {
	DxLib_End();
}
void MainClass::Screen_Flip(LONGLONG waits) {
	ScreenFlip();
	if (!USE_YSync)
		while (GetNowHiPerformanceCount() - waits < 1000000 / frate) {}
}

DeBuG::DeBuG(void){
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
void DeBuG::debug(float fps, float time) {
	deb[0][0] = time;
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
	const auto c_ffff00 = GetColor(255, 255, 0);
	DrawBox(x_r(100), y_r(100 + 0), x_r(100 + 60 * 5), y_r(100 + 200), GetColor(255, 0, 0), FALSE);
	for (int j = 0; j < 60 - 1; ++j) {
		for (int i = 0; i < 6; ++i) {
			DrawLine(x_r(100 + j * 5), y_r(100 + (int)(200.f - deb[j][i + 1] * 10.f)), x_r(100 + (j + 1) * 5), y_r(100 + (int)(200.f - deb[j + 1][i + 1] * 10.f)), GetColor(50, 128 + 127 * i / 6, 50));
		}
		DrawLine(x_r(100 + j * 5), y_r(100 + (int)(200.f - deb[j][0] * 10.f)), x_r(100 + (j + 1) * 5), y_r(100 + (int)(200.f - deb[j + 1][0] * 10.f)), c_ffff00);
	}
	const auto c_ffffff = GetColor(255, 255, 255);
	DrawLine(x_r(100), y_r(100 + 200 - 166), x_r(100 + 60 * 5), y_r(100 + 200 - 166), GetColor(0, 255, 0));
	font.DrawStringFormat(x_r(100), y_r(100 + 0), c_ffffff, "%05.2ffps ( %.2fms)(total %.2fms)", fps, time, 1000.0f / fps);

	font.DrawStringFormat(x_r(100), y_r(100 + 33), c_ffffff, "%d(%.2fms)", 0, waydeb[0]);
	for (size_t j = 1; j < std::size(waydeb); ++j) {
		font.DrawStringFormat(x_r(100), y_r(int(100 + 33 + j * 33)), c_ffffff, "%d(%.2fms)", j, waydeb[j] - waydeb[j - 1u]);
	}
}

Draw_fps::Draw_fps() {
	campos = { 0, 0, 0 };
	camvec = { 0, 0, 100 };
	fov = 90;
}

Draw_fps::~Draw_fps() {
	wcon.clear();
	lcon.clear();
}

void Draw_fps::set_cam(pos3D cams, int xrad, int yrad, int zrad,int fovs){
	campos = cams;
	camvec.x = campos.x - sin_i(yrad, cos_i(xrad, 100));
	camvec.y = campos.y - sin_i(xrad, 100);
	camvec.z = campos.z - cos_i(yrad, cos_i(xrad, 100));
	fov = fovs;
}

void Draw_fps::draw_dot(int sx, int sy, int sz, bool hide){
	const auto d = getpos(sx, sy, sz);
	if(d.z>0)
		DrawPixel(d.x, d.y, GetColor(255, 255, 255));
}

void Draw_fps::draw_line(pos3D s, pos3D e, short dist, int chose){
	pos3D t = s, ot = t;
	bool on = false;
	bool off[8];
	int gg = -1;
	const int div1 = (getdist(s,e)/400);
	for (int i = 1; i <= div1; ++i) {
		auto d1 = getpos(t);
		int col = std::clamp(255 - 255 * getdist(t, campos) / dist, 0, 255);
		int distd = std::max(getdist(s, e), 1);

		ot = t;
		t = { s.x + (e.x - s.x)*i / div1,s.y + (e.y - s.y)*i / div1,s.z + (e.z - s.z)*i / div1 };
		auto d2 = getpos(t);

		if (col > 0) {
			for (int p = 0; p < 8; p++)
				off[p] = false;
			//*
			for (size_t w = 0; w < wsize && w < chose; w++) {

				if (getdist(wcon[w].mpos, campos) > 10000)
					continue;

				gethit_rect(wcon[w], d1, d2);
				for (int p = 0; p < 8; p++)
					off[p] = ((wcon[w].res & (1 << p)) != 0) ? true : off[p];//左
			}
			//*/
			if (!off[3]) {
				for (size_t w = 0; w < wsize && w < chose; w++) {
					if (((wcon[w].res & 2) != 0)) {
						if (d2.z >= 0) {
							d2.x = wcon[w].ep.x;
							d2.y = wcon[w].ep.y;
						}
					}
					if (((wcon[w].res & 4) != 0)) {
						if (d2.z >= 0) {
							d1.x = wcon[w].sp.x;
							d1.y = wcon[w].sp.y;
						}
					}
				}
			}

			gg = -1;
			if (off[1] || off[2]) { gg = 0; }
			if (off[3] && (off[5] || off[6] || off[7])) { gg = 1; }
			//*
			if (gg == 0) {
				if (d2.z > 0) {
					int col = std::clamp(255 - 255 * getdist(t, campos) / dist, 0, 255);
					int distd = std::max(getdist(s, e), 1);
					DrawLine(d1.x, d1.y, d2.x, d2.y,
						GetColor(
							col*int(abs(s.x - e.x)) / distd,
							col*int(abs(s.y - e.y)) / distd,
							col*int(abs(s.z - e.z)) / distd));
				}
			}
			//*/
			if (gg >= 0)
				continue;

			if (d2.z < 0) {
				if (on) {
					pos3D pt = ot;
					for (int j = 1; j <= div2; ++j) {
						const auto d1 = getpos(pt);
						int col = std::clamp(255 - 255 * getdist(pt, campos) / dist, 0, 255);
						int distd = std::max(getdist(s, e), 1);
						pt = { ot.x + (t.x - ot.x)*j / div2,ot.y + (t.y - ot.y)*j / div2,ot.z + (t.z - ot.z)*j / div2 };
						const auto d2 = getpos(pt);
						DrawLine(d1.x, d1.y, d2.x, d2.y, GetColor(
							col*int(abs(s.x - e.x)) / distd,
							col*int(abs(s.y - e.y)) / distd,
							col*int(abs(s.z - e.z)) / distd));
						if (d2.z < 0)
							break;
					}
					on = false;
				}
				else {
					pos3D tt = t;
					for (; i <= div1; ++i) {
						tt = { s.x + (e.x - s.x)*i / div1,s.y + (e.y - s.y)*i / div1,s.z + (e.z - s.z)*i / div1 };
						const auto d2 = getpos(tt);
						if (d2.z < 0)
							continue;
						t = { s.x + (e.x - s.x)*i / div1,s.y + (e.y - s.y)*i / div1,s.z + (e.z - s.z)*i / div1 };
						break;
					}
					pos3D pt = t;
					pos3D st = { s.x + (e.x - s.x)*(i - 1) / div1,s.y + (e.y - s.y)*(i - 1) / div1,s.z + (e.z - s.z)*(i - 1) / div1 };
					for (int j = div2; j > 0; --j) {
						const auto d1 = getpos(pt);
						int col = std::clamp(255 - 255 * getdist(pt, campos) / dist, 0, 255);
						int distd = std::max(getdist(s, e), 1);
						pt = { st.x + (t.x - st.x)*j / div2,st.y + (t.y - st.y)*j / div2,st.z + (t.z - st.z)*j / div2 };
						const auto d2 = getpos(pt);
						DrawLine(d1.x, d1.y, d2.x, d2.y, GetColor(
							col*int(abs(s.x - e.x)) / distd,
							col*int(abs(s.y - e.y)) / distd,
							col*int(abs(s.z - e.z)) / distd));
						if (d2.z < 0) {
							break;
						}
					}
					on = true;
				}
				continue;
			}
			else {
				on = true;
			}
			if (on) {
				DrawLine(d1.x, d1.y, d2.x, d2.y, GetColor(
					col*int(abs(s.x - e.x)) / distd,
					col*int(abs(s.y - e.y)) / distd,
					col*int(abs(s.z - e.z)) / distd));
			}
		}
	}
}

void Draw_fps::draw_wall(int sx, int sy, int sz, int ex, int ey, int ez, int chose, short dist) {
	pos3D e = { ex, ey, ez };
	pos3D s = { sx, sy, sz };

	if ((ez - sz)*(campos.x - sx) - (ex - sx)*(campos.z - sz) >= 0) {
		pos3D a = { ex, sy, ez };
		pos3D b = { sx, ey, sz };
		draw_line(s, a, dist, chose);
		draw_line(a, e, dist, chose);
		draw_line(e, b, dist, chose);
		draw_line(b, s, dist, chose);
	}
}

void Draw_fps::drw_rect(pos3D s, pos3D e, int chose, short dist) {
	draw_wall(e.x, s.y, e.z, s.x, e.y, e.z, chose, dist);//左
	draw_wall(e.x, s.y, s.z, e.x, e.y, e.z, chose, dist);//右
	draw_wall(s.x, s.y, e.z, s.x, e.y, s.z, chose, dist);//後
	draw_wall(s.x, s.y, s.z, e.x, e.y, s.z, chose, dist);//前
}

void Draw_fps::set_drw_line(pos3D s, pos3D e){
	pos3D t = s;
	const int div1 = getdist(s, e) / 400;//
	for (int i = 1; i <= div1; ++i) {
		auto d1 = getpos(t);
		t = { s.x + (e.x - s.x)*i / div1,s.y + (e.y - s.y)*i / div1,s.z + (e.z - s.z)*i / div1 };
		auto d2 = getpos(t);

		if (d2.z >= 0) {
			if (lsize >= lcon.size()) { lcon.resize(lsize + 1); }
			lcon[lsize].pos[0] = s;
			lcon[lsize].pos[1] = e;
			lsize++;
			break;
		}
	}
}
//lconとwconに貯めた描画物を一気に描画する
void Draw_fps::set_drw_line(int sx, int sy, int sz, int ex, int ey, int ez){
	const pos3D s = { sx,sy,sz };
	const pos3D e = { ex,ey,ez };
	set_drw_line(s, e);
}
void Draw_fps::set_drw_rect(pos3D s, pos3D e){
	pos3D m = { (s.x + e.x) / 2, (s.y + e.y) / 2, (s.z + e.z) / 2 };
	if (getdot(getsub(m, campos), getsub(camvec, campos)) > 0) {
		wsize++;
		if (wsize - 1 >= wcon.size()) { wcon.resize(wsize); }
		size_t i = 0;
		if (wsize > 1) {
			const auto b = getdist(m, campos);
			for (i = 0; i < wsize - 1; i++) {
				if (b < getdist(wcon[i].mpos, campos)) {
					for (int j = int(wsize) - 1; j > i; j--) {
						wcon[j].pos[0] = wcon[j - 1].pos[0];
						wcon[j].pos[1] = wcon[j - 1].pos[1];
						wcon[j].mpos = wcon[j - 1].mpos;
					}
					break;
				}
			}
		}
		wcon[i].pos[0] = s;
		wcon[i].pos[1] = e;
		wcon[i].mpos = m;
	}
}
void Draw_fps::set_drw_rect(int sx, int sy, int sz, int ex, int ey, int ez) {
	pos3D m = { (sx + ex) / 2, (sy + ey) / 2, (sz + ez) / 2 };
	if (getdot(getsub(m, campos), getsub(camvec, campos)) > 0) {
		wsize++;
		if (wsize - 1 >= wcon.size()) { wcon.resize(wsize); }
		size_t i = 0;
		if (wsize > 1) {
			const auto b = getdist(m, campos);
			for (i = 0; i < wsize - 1; i++) {
				if (b < getdist(wcon[i].mpos, campos)) {
					for (int j = int(wsize) - 1; j > i; j--) {
						wcon[j].pos[0] = wcon[j - 1].pos[0];
						wcon[j].pos[1] = wcon[j - 1].pos[1];
						wcon[j].mpos = wcon[j - 1].mpos;
					}
					break;
				}
			}
		}
		wcon[i].pos[0] = { sx, sy, sz };
		wcon[i].pos[1] = { ex, ey, ez };
		wcon[i].mpos = m;
	}
}
void Draw_fps::put_drw(void){
	for (size_t i = 0; i < lsize; i++)
		draw_line(lcon[i].pos[0], lcon[i].pos[1],5000);
	for (int i = 0; i < wsize; i++)
		drw_rect(wcon[i].pos[0], wcon[i].pos[1], i,7500);
}

void Draw_fps::end_drw(void){
	lsize = 0;
	wsize = 0;
}

