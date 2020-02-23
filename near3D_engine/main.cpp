#define NOMINMAX
#include "sub.hpp"
#include "useful.hpp"
#include "make_thread.hpp"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	input key{ 0 };
	output out{ 0 };
	switches aim, map, vch; /*視点変更*/

	int xs = 10, ys = 10;
	int tile=32;

	float rr;
	int st;
	std::vector<float> rtt[2];
	int xpf, ypf;

	auto threadparts = std::make_unique<ThreadClass>(); /*演算クラス*/
	auto parts = std::make_unique<MainClass>(); /*汎用クラス*/
	//auto drawparts = std::make_unique<Draw_lookdown>(); /*見下ろし描画クラス*/
	auto uiparts = std::make_unique<UIS>(); /*汎用クラス*/

	auto fpsparts = std::make_unique<Draw_fps>(); /*fps描画クラス*/

	MainClass::pos3D campos = { 0,0,0 };
	MainClass::pos3D ct = { 0,0,0 };


	const int guns = 32;
	std::array<bool, guns> gunflug;
	std::array<unsigned int, guns> guncnt;
	std::array<MainClass::pos3D, guns> startpos;
	std::array<MainClass::pos3D, guns> endpos;
	int gunc = 0;
	for (int i = 0; i < guns; i++) {
		gunflug[i] = false;
		guncnt[i] = 0;
		startpos[i] = campos;
		endpos[i] = campos;
	}


	//const auto font72 = FontHandle::Create(x_r(72), y_r(72 / 3), DX_FONTTYPE_ANTIALIASING);
	const auto screen = MakeScreen(dispx, dispy*2);

	int graphs[32];
	GraphHandle::LoadDiv("data/Chip/Wall/1.bmp", 32, 16, 2, 16, 32, graphs);//改良
	do {
		aim.flug = false; /*照準*/
		map.flug = false; /*マップ*/
		vch.flug = false; /**/

		out.x = 0;
		out.y = 0;
		out.z = 0;
		threadparts->thread_start(key, out);
		while (ProcessMessage() == 0 && !out.ends) {
			const auto waits = GetNowHiPerformanceCount();
			uiparts->put_way();
			SetDrawScreen(screen);
			ClearDrawScreen();
			//
			/*
			//見下ろしサンプル
			tile = 32;
			xp = out.x;
			yp = out.y;
			for (int y = 0; y < 40; y+=2) {
				for (int x = 0; x < 40; x +=2) {
					drawparts->set_drw_rect(xp, yp, x	, y	, tile, 64*(x+y*40)/(40*40), graphs[2]);
					drawparts->set_drw_rect(xp, yp, x + 1	, y	, tile, 0, graphs[2]);
					drawparts->set_drw_rect(xp, yp, x	, y + 1	, tile, 0, graphs[2]);
					drawparts->set_drw_rect(xp, yp, x + 1	, y + 1	, tile, 0, graphs[2]);
				}
			}
			drawparts->put_drw();
			*/
			//fpsサンプル

			uiparts->end_way();

			campos.x = out.xp;
			campos.y = 400 + out.z;
			campos.z = out.yp;
			fpsparts->set_cam(campos, out.xr, out.yr, 0, 110);

			uiparts->end_way();

			//do:床はすべてlineで
			for (int x = -16000; x <= 16000; x += 400) {
				fpsparts->set_drw_line(-16000, 0, x, 16000, 0, x);
				fpsparts->set_drw_line(x, 0, -16000, x, 0, 16000);
			}
			//壁
			for (int x = 0; x <= 1000; x += 200) {
				fpsparts->set_drw_line(-16000, x, 16000, 16000, x, 16000);
				fpsparts->set_drw_line(16000, x, -16000, 16000, x, 16000);
				fpsparts->set_drw_line(-16000, x, -16000, -16000, x, 16000);
				fpsparts->set_drw_line(-16000, x, -16000, 16000, x, -16000);
			}
			//柱
			for (int x = -16000; x < 16000; x += 4000) {
				for (int z = -16000; z < 16000; z += 4000) {
					fpsparts->set_drw_rect(x + 400, 1000, z + 400, x, 0, z);
				}
			}



			uiparts->end_way();

			fpsparts->put_drw();

			if (out.shootf == 0) {

				rr = deg2rad(GetRand(90));
				st = 3 + GetRand(7);
				rtt[0].resize(st);
				for (auto& r:rtt[0])
					r = float(50 + GetRand(150));
				rtt[1].resize(st);
				for (auto& r : rtt[1])
					r = float(30 + GetRand(90));

				ct.x = campos.x - int(100.f*cos(deg2rad(out.xr))*sin(deg2rad(out.yr)));
				ct.y = campos.y - int(100.f*sin(deg2rad(out.xr)));
				ct.z = campos.z - int(100.f*cos(deg2rad(out.xr))*cos(deg2rad(out.yr)));

			}
			if (out.shootf == 1) {
				if (!gunflug[gunc]) {
					startpos[gunc] = ct;
					endpos[gunc].x = startpos[gunc].x - int(500.f*cos(deg2rad(out.xr))*sin(deg2rad(out.yr)));
					endpos[gunc].y = startpos[gunc].y - int(500.f*sin(deg2rad(out.xr)));
					endpos[gunc].z = startpos[gunc].z - int(500.f*cos(deg2rad(out.xr))*cos(deg2rad(out.yr)));
					gunflug[gunc] = true;
				}
				gunc++;
				gunc %= guns;
				out.shootf = 2;
			}
			if (out.shootf != 0 && out.shootc<=4) {
				const auto t = fpsparts->getpos(ct);
				xpf = t.x - dispx / 2;
				ypf = t.y - dispy / 2;
				for (int i = 0; i < st; i++) {
					DrawLine(
						dispx / 2 + x_r(xpf) + int(rtt[0][i] * cos(rr + DX_PI_F * 2 * (i) / st)),
						dispy / 2 + y_r(ypf) + int(rtt[0][i] * sin(rr + DX_PI_F * 2 * (i) / st)),
						dispx / 2 + x_r(xpf) + int(rtt[0][(i + 1) % st] * cos(rr + DX_PI_F * 2 * (i + 1) / st)),
						dispy / 2 + y_r(ypf) + int(rtt[0][(i + 1) % st] * sin(rr + DX_PI_F * 2 * (i + 1) / st)),
						GetColor(255 * out.shootc / 4, 255 * out.shootc / 4, 0));
					DrawLine(
						dispx / 2 + x_r(xpf) + int(rtt[1][i] * cos(rr + DX_PI_F * 2 * (i) / st)),
						dispy / 2 + y_r(ypf) + int(rtt[1][i] * sin(rr + DX_PI_F * 2 * (i) / st)),
						dispx / 2 + x_r(xpf) + int(rtt[1][(i + 1) % st] * cos(rr + DX_PI_F * 2 * (i + 1) / st)),
						dispy / 2 + y_r(ypf) + int(rtt[1][(i + 1) % st] * sin(rr + DX_PI_F * 2 * (i + 1) / st)),
						GetColor(255 * out.shootc / 4, 0, 0));
				}
			}

			for (int i = 0; i < guns; i++) {
				if (gunflug[i]) {
					const auto fx = fpsparts->getpos(startpos[i]);
					const auto ex = fpsparts->getpos(endpos[i]);
					if (fx.z >= 0 && ex.z >= 0) {
						DrawLine(fx.x, fx.y, ex.x, ex.y, GetColor(255, 255, 0),10000/fpsparts->getdist(startpos[i],campos));
					}
					const auto oldg = startpos[i];
					startpos[i] = endpos[i];
					endpos[i].x -= (oldg.x - startpos[i].x);
					endpos[i].y -= (oldg.y - startpos[i].y);
					endpos[i].z -= (oldg.z - startpos[i].z);
					guncnt[i]++;
					if (guncnt[i] > 60) {
						guncnt[i] = 0;
						gunflug[i] = false;
					}
				}
			}
			//照星
			DrawLine(x_r(957 + out.xadd - out.yradd), y_r(540 + out.yadd - out.xradd), x_r(957 + out.xadd - out.yradd), y_r(560 + out.yadd - out.xradd), GetColor(255, 255, 255));
			DrawLine(x_r(957 + out.xadd - out.yradd), y_r(540 + out.yadd - out.xradd), x_r(963 + out.xadd - out.yradd), y_r(540 + out.yadd - out.xradd), GetColor(255, 255, 255));
			DrawLine(x_r(963 + out.xadd - out.yradd), y_r(540 + out.yadd - out.xradd), x_r(963 + out.xadd - out.yradd), y_r(560 + out.yadd - out.xradd), GetColor(255, 255, 255));
			//照門
			DrawLine(x_r(940 + out.xadd + out.yradd), y_r(540 + out.yadd + out.xradd), x_r(950 + out.xadd + out.yradd), y_r(540 + out.yadd + out.xradd), GetColor(255, 255, 255));
			DrawLine(x_r(950 + out.xadd + out.yradd), y_r(540 + out.yadd + out.xradd), x_r(950 + out.xadd + out.yradd), y_r(550 + out.yadd + out.xradd), GetColor(255, 255, 255));
			DrawLine(x_r(950 + out.xadd + out.yradd), y_r(550 + out.yadd + out.xradd), x_r(970 + out.xadd + out.yradd), y_r(550 + out.yadd + out.xradd), GetColor(255, 255, 255));
			DrawLine(x_r(970 + out.xadd + out.yradd), y_r(540 + out.yadd + out.xradd), x_r(970 + out.xadd + out.yradd), y_r(550 + out.yadd + out.xradd), GetColor(255, 255, 255));
			DrawLine(x_r(970 + out.xadd + out.yradd), y_r(540 + out.yadd + out.xradd), x_r(980 + out.xadd + out.yradd), y_r(540 + out.yadd + out.xradd), GetColor(255, 255, 255));
			//
			uiparts->end_way();

			SetDrawScreen(DX_SCREEN_BACK);
			ClearDrawScreen();
			DrawGraph(0, 0, screen,TRUE);

			uiparts->debug(GetFPS(), float(GetNowHiPerformanceCount() - waits)*0.001f);
			parts->Screen_Flip(waits);

			if (GetActiveFlag() == TRUE) {
				SetMouseDispFlag(FALSE);
				key.get[0] = CheckHitKey(KEY_INPUT_ESCAPE) != 0;
				key.get[1] = CheckHitKey(KEY_INPUT_P) != 0;
				key.get[2] = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
				key.get[3] = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;
                                key.get[4] = CheckHitKey(KEY_INPUT_LSHIFT) != 0;
                                key.get[5] = CheckHitKey(KEY_INPUT_SPACE) != 0;
                                key.get[6] = CheckHitKey(KEY_INPUT_X) != 0;
                                key.get[7] = CheckHitKey(KEY_INPUT_Q) != 0;
                                key.get[8] = CheckHitKey(KEY_INPUT_E) != 0;

				key.get[9] = (CheckHitKey(KEY_INPUT_W) != 0 ||
					CheckHitKey(KEY_INPUT_UP) != 0);
				key.get[10] =
					(CheckHitKey(KEY_INPUT_S) != 0 ||
						CheckHitKey(KEY_INPUT_DOWN) != 0);
				key.get[11] =
					(CheckHitKey(KEY_INPUT_A) != 0 ||
						CheckHitKey(KEY_INPUT_LEFT) != 0);
				key.get[12] =
					(CheckHitKey(KEY_INPUT_D) != 0 ||
						CheckHitKey(KEY_INPUT_RIGHT) != 0);
				/*指揮*/
				if (map.flug)
					SetMouseDispFlag(TRUE);
			}
			else {
				SetMouseDispFlag(TRUE);
			}
		}
		threadparts->thead_stop();
	} while (ProcessMessage() == 0 && !out.ends);

	return 0; // ソフトの終了
}
