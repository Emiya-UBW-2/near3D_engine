#define NOMINMAX
#include "sub.hpp"
#include "useful.hpp"
#include "make_thread.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	input key{ 0 };
	output out{ 0 };
	switches aim, map, vch; /*視点変更*/

	int xs = 10, ys = 10;

	int xp = 0, yp = 0, zp = 0;
	int xa = 0, ya = 0, za = 0;
	int mx, my, xr = 0, yr = 180;
	int tile=32;

	auto threadparts = std::make_unique<ThreadClass>(); /*演算クラス*/
	auto parts = std::make_unique<MainClass>(); /*汎用クラス*/
	auto drawparts = std::make_unique<Draw_lookdown>(); /*見下ろし描画クラス*/
	auto uiparts = std::make_unique<UIS>(); /*汎用クラス*/

	auto fpsparts = std::make_unique<Draw_fps>(); /*fps描画クラス*/

	MainClass::pos3D campos = { 0,0,500 };
	MainClass::pos3D camvec = { 0,0,0 };

	const auto font72 = FontHandle::Create(x_r(72), y_r(72 / 3), DX_FONTTYPE_ANTIALIASING);
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
			if (!out.jf) {
				xa = int(float(out.x)*cos(deg2rad(yr)) + float(out.y)*sin(deg2rad(yr)));
				za = int(-float(out.x)*sin(deg2rad(yr)) + float(out.y)*cos(deg2rad(yr)));
			}
			xp += xa;
			yp += za;

			//zp = out.z;
			GetMousePoint(&my, &mx);
			SetMousePoint(dispx/2,dispy/2);
			mx -= dispy / 2;
			my -= dispx / 2;
			xr += mx;
			yr -= my;
			xr = std::clamp<int>(xr, -45, 45);
			campos.x = xp;
			campos.y = 400+out.z;
			campos.z = yp;
			camvec.x = campos.x -int(100.f*cos(deg2rad(xr))*sin(deg2rad(yr)));
			camvec.y = campos.y -int(100.f*sin(deg2rad(xr)));
			camvec.z = campos.z -int(100.f*cos(deg2rad(xr))*cos(deg2rad(yr)));
			uiparts->end_way();

			fpsparts->set_cam(campos, camvec,110);
			uiparts->end_way();

			//todo:床はすべてlineで
			for (int x = -4800; x <= 4800; x += 400) {
				fpsparts->set_drw_line(-4800, 0,     x, 4800, 0,    x);
				fpsparts->set_drw_line(    x, 0, -4800,    x, 0, 4800);
			}
			//*
			for (int x = -1200; x <= 1200; x += 800) {
				for (int z = -1200; z <= 1200; z += 800) {
					fpsparts->set_drw_rect(x + 400, 800, z + 400, x, 0, z);
				}
			}
			//*/

			//*
			//fpsparts->set_drw_rect(600, 400, 600, 200, 0, 200);
			//fpsparts->set_drw_rect(-200, 400, -200, -600, 0, -600);
			//fpsparts->set_drw_rect(600, 400, -200, 200, 0, -600);
			//fpsparts->set_drw_rect(-200, 400, 600, -600, 0, 200);
			//*/


			uiparts->end_way();
			fpsparts->put_drw();
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
