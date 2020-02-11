#define NOMINMAX
#include "sub.hpp"
#include "useful.hpp"
#include "make_thread.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	input key{ 0 };
	output out{ 0 };
	switches aim, map, vch; /*視点変更*/

	int xs = 10, ys = 10;

	int xp, yp;
	int ratio = 32;
	float high = 1.f;

	auto threadparts = std::make_unique<ThreadClass>(); /*演算クラス*/
	auto parts = std::make_unique<MainClass>(); /*汎用クラス*/
	auto drawparts = std::make_unique<Draw>(); /*描画クラス*/
	auto uiparts = std::make_unique<UIS>(); /*汎用クラス*/

	const auto font72 = FontHandle::Create(x_r(72), y_r(72 / 3), DX_FONTTYPE_ANTIALIASING);

	const auto screen = MakeScreen(dispx, dispy*2);

	int graphs[32];
	GraphHandle::LoadDiv("data/Chip/Wall/1.bmp", 32, 16, 2, 16, 32, graphs);//改良
	do {
		aim.flug = false; /*照準*/
		map.flug = false; /*マップ*/
		vch.flug = false; /**/

		out.x = 0.f;
		out.y = 0.f;
		threadparts->thread_start(key, out);
		while (ProcessMessage() == 0 && !out.ends) {
			const auto waits = GetNowHiPerformanceCount();
			SetDrawScreen(screen);
			ClearDrawScreen();
			//
			high = 1.f;
			ratio = 32;
			xp = out.x;
			yp = out.y;


			for (int y = 0; y < 100; y+=2) {
				drawparts->drw_rect(xp, yp, 15, y, ratio, 32, graphs[2]);
			}

			//drawparts->drw_rect(xp, yp, 15, 5, ratio, 32, graphs[2]);
			//drawparts->drw_prism(0,xp, yp, 16, 5, ratio, 32, graphs[2]);
			//drawparts->drw_prism(1, xp, yp, 17, 5, ratio, 32, graphs[2]);
			//drawparts->drw_prism(2, xp, yp, 18, 5, ratio, 32, graphs[2]);
			//drawparts->drw_prism(3, xp, yp, 19, 5, ratio, 32, graphs[2]);

			/*
			for (int x = xs - 1; x >= 0 && (xp + ratio * x >= dispx / 2); --x) {
				if (xp + ratio * x > dispx)
					continue;

				for (int y = 0; y < ys; ++y) {
					//drawparts->drw_rect(xp, yp, x, y, ratio, int(32.f), graphs[2]);
					drawparts->drw_rect(xp, yp, x, y, ratio, int(32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys))), graphs[2]);
					//drawparts->drw_prism(3, xp, yp, x, y, ratio, int(32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys))), graphs[2]);
				}
			}

			for (int x = 0; x < xs && (xp + ratio * x <= dispx / 2); ++x) {
				if (xp + ratio * x + ratio < 0)
					continue;

				for (int y = 0; y < ys; ++y) {
					//drawparts->drw_rect(xp, yp, x, y, ratio, int(32.f), graphs[2]);
					drawparts->drw_rect(xp, yp, x, y, ratio, int(32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys))), graphs[2]);
					//drawparts->drw_prism(3, xp, yp, x, y, ratio, int(32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys))), graphs[2]);
				}
			}

			//*/
			/*
			for (int x = xs - 1; x >= 0 && (xp + ratio * x >= dispx / 2); --x) {
				if (xp + ratio * x > dispx)
					continue;

				for (int y = ys - 1; y >= 0 && (yp + ratio * y >= dispy / 2); --y) {
					if (yp + ratio * y > dispy)
						continue;
					//drawparts->drw_rect(xp, yp, x, y, ratio, 32.f, graphs[2]);
					drawparts->drw_rect(xp, yp, x, y,ratio, 32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys)), graphs[2]);
					//drawparts->drw_prism(3, xp, yp, x, y,ratio, 32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys)), graphs[2]);
				}
				for (int y = 0; y < ys && (yp + ratio * y <= dispy / 2); ++y) {
					if (yp + ratio * y + ratio < 0)
						continue;
					//drawparts->drw_rect(xp, yp, x, y, ratio, 32.f, graphs[2]);
					drawparts->drw_rect(xp, yp, x, y,ratio, 32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys)), graphs[2]);
					//drawparts->drw_prism(3, xp, yp, x, y, ratio, 32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys)), graphs[2]);
				}
			}

			for (int x = 0; x < xs && (xp + ratio * x <= dispx / 2); ++x) {
				if (xp + ratio * x + ratio < 0)
					continue;

				for (int y = ys - 1; y >= 0 && (yp + ratio * y >= dispy / 2); --y) {
					if (yp + ratio * y > dispy)
						continue;
					//drawparts->drw_rect(xp, yp, x, y, ratio, 32.f, graphs[2]);
					drawparts->drw_rect(xp, yp, x, y,ratio, 32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys)), graphs[2]);
					//drawparts->drw_prism(3, xp, yp, x, y, ratio, 32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys)), graphs[2]);
				}
				for (int y = 0; y < ys && (yp + ratio * y <= dispy / 2); ++y) {
					if (yp + ratio * y + ratio < 0)
						continue;
					//drawparts->drw_rect(xp, yp, x, y, ratio, 32.f, graphs[2]);
					drawparts->drw_rect(xp, yp, x, y,ratio, 32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys)), graphs[2]);
					//drawparts->drw_prism(3, xp, yp, x, y, ratio, 32.f*(float(x%xs) / float(xs))*(float(y%ys) / float(ys)), graphs[2]);
				}
			}
			//*/
			//
			SetDrawScreen(DX_SCREEN_BACK);
			ClearDrawScreen();
			DrawGraph(0, 0, screen,TRUE);
			//font72.DrawStringFormat(0, 0, GetColor(255, 255, 255), "%f", GetFPS());
			uiparts->debug(GetFPS(), float(GetNowHiPerformanceCount() - waits)*0.001);
			parts->Screen_Flip(waits);

			if (GetActiveFlag() == TRUE) {
				SetMouseDispFlag(FALSE);
				key.get[0] = CheckHitKey(KEY_INPUT_ESCAPE) != 0;
				key.get[1] = CheckHitKey(KEY_INPUT_P) != 0;
				key.get[2] = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
				key.get[3] = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;
                                key.get[4] = CheckHitKey(KEY_INPUT_LSHIFT) != 0;
                                key.get[5] = CheckHitKey(KEY_INPUT_Z) != 0;
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
