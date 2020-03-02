#define NOMINMAX
#include "sub.hpp"
#include "useful.hpp"
#include "make_thread.hpp"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	using namespace std::literals;
	input in{ 0 };
	output out{ 0 };

	Draw_fps::pos3D campos = { 0,0,0 };
	DINPUT_JOYSTATE info;

	auto threadparts = std::make_unique<ThreadClass>(); /*演算クラス*/
	auto parts = std::make_unique<MainClass>(); /*汎用クラス*/
	auto fpsparts = std::make_unique<Draw_fps>(); /*fps描画クラス*/

	auto debug = std::make_unique<DeBuG>(); /*デバッグ描画クラス*/

	if (parts->write_setting())
		return 0;

	for (int i = 0; i < 60; i++) {
		int x, z;
		x = -40 + 5 + GetRand(80 - 5 * 2);
		z = -40 + 5 + GetRand(80 - 5 * 2);
		if (i != 0) {
			do {
				x = -40 + 5 + GetRand(80 - 5 * 2);
			} while (abs(x * 400 - in.rcon.back().pos[1].x) <= 200);
			do {
				z = -40 + 5 + GetRand(80 - 5 * 2);
			} while (abs(z * 400 - in.rcon.back().pos[1].z) <= 200);
		}
		in.rcon.resize(in.rcon.size() + 1);
		in.rcon.back().pos[0] = { x * 400 + 400, 4000, z * 400 + 400 };
		in.rcon.back().mpos = { x * 400 + 200, 2000, z * 400 + 200 };
		in.rcon.back().pos[1] = { x * 400 , 0, z * 400 };

	}
	/*
	for (int x = -40+5; x < 40; x += 10) {
	
		for (int z = -40+5; z < 40; z += 10) {
			in.rcon.resize(in.rcon.size() + 1);
			in.rcon.back().pos[0] = { x * 400 + 400, 4000, z * 400 + 400 };
			in.rcon.back().mpos = { x * 400 + 200, 2000, z * 400 + 200 };
			in.rcon.back().pos[1] = { x * 400 , 0, z * 400 };
		}
	}
	//*/

	for (size_t k = 0; k < 2; k++) {
		const auto mdata = FileRead_open(("data/enemy_animetion/"s + std::to_string(k) + ".txt").c_str(), FALSE);
		int size = getparam_i(mdata);
		for (size_t j = 0; j < size; j++) {
			in.enemyframe[k].resize(in.enemyframe[k].size() + 1);
			in.enemyframe[k][j].time = getparam_i(mdata);
			for (size_t i = 0; i < 6; i++)
				in.enemyframe[k][j].frame[0][i] = { getparam_i(mdata),getparam_i(mdata),getparam_i(mdata) };
			in.enemyframe[k][j].bodyframe = { getparam_i(mdata),getparam_i(mdata),getparam_i(mdata) };
			for (size_t i = 0; i < 6; i++)
				in.enemyframe[k][j].frame[1][i] = { getparam_i(mdata),getparam_i(mdata),getparam_i(mdata) };
		}
		FileRead_close(mdata);
	}

	const auto font72 = FontHandle::Create(x_r(72), y_r(72 / 3), DX_FONTTYPE_NORMAL);
	const auto screen = MakeScreen(dispx, dispy*2);
	//SetMouseDispFlag(FALSE);

	do {

	//StartJoypadVibration(DX_INPUT_PAD1, 1000, -1);

		threadparts->thread_start(in, out);
		SetBackgroundColor(64, 64, 64);
		while (ProcessMessage() == 0 && !out.ends) {
			const auto waits = GetNowHiPerformanceCount();
			debug->put_way();
			SetDrawScreen(screen);
			ClearDrawScreen();
			//
			campos.x = out.pos.x;
			campos.y = out.pos.y + 400;
			campos.z = out.pos.z;
			fpsparts->set_cam(campos, out.xr, out.yr, 0, in.get[3]? 75 : 110);

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
			for (const auto& r : in.rcon)
				fpsparts->set_drw_rect(r.pos[0], r.pos[1]);
			//敵
			for (auto& e : out.enemy) {
				for (auto& g : e.gun) {
					if (g.gunflug) {
						const auto fx = fpsparts->getpos(g.startpos);
						const auto ex = fpsparts->getpos(g.endpos);
						if (fx.z >= 0 && ex.z >= 0) {
							const auto fr = 10000 / fpsparts->getdist(g.startpos, campos) / 2;
							const auto er = 10000 / fpsparts->getdist(g.endpos, campos) / 2;

							DrawLine(fx.x, fx.y + fr, ex.x, ex.y + er, GetColor(255, 128, 0));
							DrawLine(fx.x, fx.y - fr, ex.x, ex.y - er, GetColor(255, 128, 0));

							if (ex.x < fx.x) {
								//L
								DrawLine(ex.x, ex.y + er, ex.x - er, ex.y, GetColor(255, 128, 0));
								DrawLine(ex.x - er, ex.y, ex.x, ex.y - er, GetColor(255, 128, 0));
								//R
								DrawLine(fx.x, fx.y - fr, fx.x + fr, fx.y, GetColor(255, 128, 0));
								DrawLine(fx.x + fr, fx.y, fx.x, fx.y + fr, GetColor(255, 128, 0));
							}
							else {
								//L
								DrawLine(fx.x, fx.y + fr, fx.x - fr, fx.y, GetColor(255, 128, 0));
								DrawLine(fx.x - fr, fx.y, fx.x, fx.y - fr, GetColor(255, 128, 0));
								//R
								DrawLine(ex.x, ex.y - er, ex.x + er, ex.y, GetColor(255, 128, 0));
								DrawLine(ex.x + er, ex.y, ex.x, ex.y + er, GetColor(255, 128, 0));
							}
						}
					}
					if (g.hitflug) {
						const auto hx = fpsparts->getpos(g.hitpos);
						if (hx.z >= 0) {
							const auto rr = 10000 / fpsparts->getdist(g.hitpos, campos) * 2;
							DrawLine(hx.x, hx.y + rr, hx.x + rr, hx.y, GetColor(255 - 255 * g.hitcnt / (60 * 5), 128 - 128 * g.hitcnt / (60 * 5), 0));
							DrawLine(hx.x + rr, hx.y, hx.x, hx.y - rr, GetColor(255 - 255 * g.hitcnt / (60 * 5), 128 - 128 * g.hitcnt / (60 * 5), 0));
							DrawLine(hx.x, hx.y - rr, hx.x - rr, hx.y, GetColor(255 - 255 * g.hitcnt / (60 * 5), 128 - 128 * g.hitcnt / (60 * 5), 0));
							DrawLine(hx.x - rr, hx.y, hx.x, hx.y + rr, GetColor(255 - 255 * g.hitcnt / (60 * 5), 128 - 128 * g.hitcnt / (60 * 5), 0));
						}
					}
				}

				fpsparts->draw_line(e.footframe[0][0], e.footframe[0][1], 20000, 0);
				fpsparts->draw_line(e.footframe[0][2], e.footframe[0][1], 20000, 0);
				fpsparts->draw_line(e.footframe[0][3], e.footframe[0][1], 20000, 0);
				fpsparts->draw_line(e.footframe[0][3], e.footframe[0][4], 20000, 0);
				fpsparts->draw_line(e.footframe[0][3], e.body, 20000, 0);
				fpsparts->draw_line(e.footframe[0][5], e.body, 20000, 0);

				fpsparts->draw_line(e.footframe[1][0], e.footframe[1][1], 20000, 0);
				fpsparts->draw_line(e.footframe[1][2], e.footframe[1][1], 20000, 0);
				fpsparts->draw_line(e.footframe[1][3], e.footframe[1][1], 20000, 0);
				fpsparts->draw_line(e.footframe[1][3], e.footframe[1][4], 20000, 0);
				fpsparts->draw_line(e.footframe[1][3], e.body, 20000, 0);
				fpsparts->draw_line(e.footframe[1][5], e.body, 20000, 0);
			}

			fpsparts->put_drw();
			//発砲炎
			if (out.shootf && out.shootc<=4) {
				auto t = fpsparts->getpos(out.ct);
				if (t.z >= 0) {
					for (int i = 0; i < out.st; i++) {
						DrawLine(
							t.x + x_r(cos_i(out.rr + 360 * i / out.st, out.rtt[0][i])),
							t.y + y_r(sin_i(out.rr + 360 * i / out.st, out.rtt[0][i])),
							t.x + x_r(cos_i(out.rr + 360 * (i + 1) / out.st, out.rtt[0][(i + 1) % out.st])),
							t.y + y_r(sin_i(out.rr + 360 * (i + 1) / out.st, out.rtt[0][(i + 1) % out.st])),
							GetColor(255 * out.shootc / 4, 255 * out.shootc / 4, 0));
						DrawLine(
							t.x + x_r(cos_i(out.rr + 360 * i / out.st, out.rtt[1][i])),
							t.y + y_r(sin_i(out.rr + 360 * i / out.st, out.rtt[1][i])),
							t.x + x_r(cos_i(out.rr + 360 * (i + 1) / out.st, out.rtt[1][(i + 1) % out.st])),
							t.y + y_r(sin_i(out.rr + 360 * (i + 1) / out.st, out.rtt[1][(i + 1) % out.st])),
							GetColor(255 * out.shootc / 4, 0, 0));
					}
				}
			}

			for(auto& g : out.gun){
				if (g.gunflug) {
					const auto fx = fpsparts->getpos(g.startpos);
					const auto ex = fpsparts->getpos(g.endpos);
					if (fx.z >= 0 && ex.z >= 0) {
						const auto fr = 10000 / fpsparts->getdist(g.startpos, campos) * 2;
						const auto er = 10000 / fpsparts->getdist(g.endpos, campos) * 2;

						DrawLine(fx.x, fx.y + fr, ex.x, ex.y + er, GetColor(255, 255, 0));
						DrawLine(fx.x, fx.y - fr, ex.x, ex.y - er, GetColor(255, 255, 0));

						if (ex.x < fx.x) {
							//L
							DrawLine(ex.x, ex.y + er, ex.x - er, ex.y, GetColor(255, 255, 0));
							DrawLine(ex.x - er, ex.y, ex.x, ex.y - er, GetColor(255, 255, 0));
							//R
							DrawLine(fx.x, fx.y - fr, fx.x + fr, fx.y, GetColor(255, 255, 0));
							DrawLine(fx.x + fr, fx.y, fx.x, fx.y + fr, GetColor(255, 255, 0));
						}
						else {
							//L
							DrawLine(fx.x, fx.y + fr, fx.x - fr, fx.y, GetColor(255, 255, 0));
							DrawLine(fx.x - fr, fx.y, fx.x, fx.y - fr, GetColor(255, 255, 0));
							//R
							DrawLine(ex.x, ex.y - er, ex.x + er, ex.y, GetColor(255, 255, 0));
							DrawLine(ex.x + er, ex.y, ex.x, ex.y + er, GetColor(255, 255, 0));
						}
					}
				}
				if (g.hitflug) {
					const auto hx = fpsparts->getpos(g.hitpos);
					if (hx.z >= 0) {
						const auto rr = 10000 / fpsparts->getdist(g.hitpos, campos) * 2;
						DrawLine(hx.x, hx.y + rr, hx.x + rr, hx.y, GetColor(255 - 255 * g.hitcnt / (60 * 2), 255 - 255 * g.hitcnt / (60 * 2), 0));
						DrawLine(hx.x + rr, hx.y, hx.x, hx.y - rr, GetColor(255 - 255 * g.hitcnt / (60 * 2), 255 - 255 * g.hitcnt / (60 * 2), 0));
						DrawLine(hx.x, hx.y - rr, hx.x - rr, hx.y, GetColor(255 - 255 * g.hitcnt / (60 * 2), 255 - 255 * g.hitcnt / (60 * 2), 0));
						DrawLine(hx.x - rr, hx.y, hx.x, hx.y + rr, GetColor(255 - 255 * g.hitcnt / (60 * 2), 255 - 255 * g.hitcnt / (60 * 2), 0));
					}
				}
			}

			fpsparts->end_drw();
			debug->end_way();

			if(in.get[KEY_M_RIGHT]){
				//照星
				DrawLine(
					dispx / 2 + x_r(-6 + out.xadd - out.yradd),
					dispy / 2 + y_r(0 + out.yadd - out.xradd),
					dispx / 2 + x_r(-6 + out.xadd - out.yradd),
					dispy / 2 + y_r(40 + out.yadd - out.xradd),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(-6 + out.xadd - out.yradd),
					dispy / 2 + y_r(0 + out.yadd - out.xradd),
					dispx / 2 + x_r(6 + out.xadd - out.yradd),
					dispy / 2 + y_r(0 + out.yadd - out.xradd),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(6 + out.xadd - out.yradd),
					dispy / 2 + y_r(0 + out.yadd - out.xradd),
					dispx / 2 + x_r(6 + out.xadd - out.yradd),
					dispy / 2 + y_r(40 + out.yadd - out.xradd),
					GetColor(255, 255, 255));
				//照門
				DrawLine(
					dispx / 2 + x_r(-40 + out.xadd + out.yradd),
					dispy / 2 + y_r(0 + out.yadd + out.xradd),
					dispx / 2 + x_r(-20 + out.xadd + out.yradd),
					dispy / 2 + y_r(0 + out.yadd + out.xradd),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(-20 + out.xadd + out.yradd),
					dispy / 2 + y_r(0 + out.yadd + out.xradd),
					dispx / 2 + x_r(-20 + out.xadd + out.yradd),
					dispy / 2 + y_r(20 + out.yadd + out.xradd),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(-20 + out.xadd + out.yradd),
					dispy / 2 + y_r(20 + out.yadd + out.xradd),
					dispx / 2 + x_r(20 + out.xadd + out.yradd),
					dispy / 2 + y_r(20 + out.yadd + out.xradd),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(20 + out.xadd + out.yradd),
					dispy / 2 + y_r(0 + out.yadd + out.xradd),
					dispx / 2 + x_r(20 + out.xadd + out.yradd),
					dispy / 2 + y_r(20 + out.yadd + out.xradd),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(20 + out.xadd + out.yradd),
					dispy / 2 + y_r(0 + out.yadd + out.xradd),
					dispx / 2 + x_r(40 + out.xadd + out.yradd),
					dispy / 2 + y_r(0 + out.yadd + out.xradd),
					GetColor(255, 255, 255));
			}
			else {
				//照星
				DrawLine(
					dispx / 2 + x_r(-6 + out.xadd - out.yradd),
					dispy / 2 + y_r(-6 + out.yadd - out.xradd),
					dispx / 2 + x_r(6 + out.xadd - out.yradd),
					dispy / 2 + y_r(-6 + out.yadd - out.xradd),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(6 + out.xadd - out.yradd),
					dispy / 2 + y_r(-6 + out.yadd - out.xradd),
					dispx / 2 + x_r(6 + out.xadd - out.yradd),
					dispy / 2 + y_r(6 + out.yadd - out.xradd),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(6 + out.xadd - out.yradd),
					dispy / 2 + y_r(6 + out.yadd - out.xradd),
					dispx / 2 + x_r(-6 + out.xadd - out.yradd),
					dispy / 2 + y_r(6 + out.yadd - out.xradd),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(-6 + out.xadd - out.yradd),
					dispy / 2 + y_r(6 + out.yadd - out.xradd),
					dispx / 2 + x_r(-6 + out.xadd - out.yradd),
					dispy / 2 + y_r(-6 + out.yadd - out.xradd),
					GetColor(255, 255, 255));
			}
			if (out.reloadf) {
				DrawLine(
					dispx / 2 + x_r(-50 + out.xadd - out.yradd),
					dispy / 2 + y_r(50 + out.yadd - out.xradd),
					dispx / 2 + x_r(50 + out.xadd - out.yradd),
					dispy / 2 + y_r(50 + out.yadd - out.xradd),
					GetColor(192, 192, 192));
				DrawLine(
					dispx / 2 + x_r(-50 + out.xadd - out.yradd),
					dispy / 2 + y_r(50 + out.yadd - out.xradd),
					dispx / 2 + x_r(-50+100*out.reloadc/out.reloadall + out.xadd - out.yradd),
					dispy / 2 + y_r(50 + out.yadd - out.xradd),
					GetColor(255, 0, 0));
			}
			//弾薬
			if (!out.reloadf) {
				for (size_t i = 0; i < out.ammoc; i++) {
					if (i != out.ammoc - 1) {
						if (i < 5) {
							DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(40), x_r(10 + i * 12 + 0), dispy - y_r(10), GetColor(255, 0, 0));
							DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(40), x_r(10 + i * 12 + 3), dispy - y_r(50), GetColor(255, 0, 0));
							DrawLine(x_r(10 + i * 12 + 3), dispy - y_r(50), x_r(10 + i * 12 + 6), dispy - y_r(40), GetColor(255, 0, 0));
							DrawLine(x_r(10 + i * 12 + 6), dispy - y_r(40), x_r(10 + i * 12 + 6), dispy - y_r(10), GetColor(255, 0, 0));
							DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(10), x_r(10 + i * 12 + 6), dispy - y_r(10), GetColor(255, 0, 0));
						}
						else if (i != out.ammoc - 1) {
							DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(40), x_r(10 + i * 12 + 0), dispy - y_r(10), GetColor(255, 255, 0));
							DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(40), x_r(10 + i * 12 + 3), dispy - y_r(50), GetColor(255, 255, 0));
							DrawLine(x_r(10 + i * 12 + 3), dispy - y_r(50), x_r(10 + i * 12 + 6), dispy - y_r(40), GetColor(255, 255, 0));
							DrawLine(x_r(10 + i * 12 + 6), dispy - y_r(40), x_r(10 + i * 12 + 6), dispy - y_r(10), GetColor(255, 255, 0));
							DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(10), x_r(10 + i * 12 + 6), dispy - y_r(10), GetColor(255, 255, 0));
						}
					}
					else {
						DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(50), x_r(10 + i * 12 + 0), dispy - y_r(20), GetColor(0, 255, 0));
						DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(50), x_r(10 + i * 12 + 3), dispy - y_r(60), GetColor(0, 255, 0));
						DrawLine(x_r(10 + i * 12 + 3), dispy - y_r(60), x_r(10 + i * 12 + 6), dispy - y_r(50), GetColor(0, 255, 0));
						DrawLine(x_r(10 + i * 12 + 6), dispy - y_r(50), x_r(10 + i * 12 + 6), dispy - y_r(20), GetColor(0, 255, 0));
						DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(20), x_r(10 + i * 12 + 6), dispy - y_r(20), GetColor(0, 255, 0));
					}
				}
				font72.DrawStringFormat(0, dispy - y_r(132), GetColor(255, 255, 255), "%d / %d", out.ammoc, out.ammoall);
			}
			//体力
			for (size_t i = 0; i < out.hitp; i++) {
				if (i < 6) {
					DrawLine(dispx - x_r(130 + i * 14), y_r(20 + 100 / (i + 1)), dispx - x_r(130 + i * 14), y_r(10), GetColor(255, 0, 0));
					DrawLine(dispx - x_r(130 + i * 14), y_r(20 + 100 / (i + 1)), dispx - x_r(139 + i * 14), y_r(20 + 100 / (i + 2)), GetColor(255, 0, 0));
					DrawLine(dispx - x_r(139 + i * 14), y_r(20 + 100 / (i + 2)), dispx - x_r(139 + i * 14), y_r(10), GetColor(255, 0, 0));
					DrawLine(dispx - x_r(130 + i * 14), y_r(10), dispx - x_r(139 + i * 14), y_r(10), GetColor(255, 0, 0));
				}
				else {
					DrawLine(dispx - x_r(130 + i * 14), y_r(20 + 100 / (i + 1)), dispx - x_r(130 + i * 14), y_r(10), GetColor(255, 255, 0));
					DrawLine(dispx - x_r(130 + i * 14), y_r(20 + 100 / (i + 1)), dispx - x_r(139 + i * 14), y_r(20 + 100 / (i + 2)), GetColor(255, 255, 0));
					DrawLine(dispx - x_r(139 + i * 14), y_r(20 + 100 / (i + 2)), dispx - x_r(139 + i * 14), y_r(10), GetColor(255, 255, 0));
					DrawLine(dispx - x_r(130 + i * 14), y_r(10), dispx - x_r(139 + i * 14), y_r(10), GetColor(255, 255, 0));
				}
			}

			font72.DrawStringFormat(0, 0, GetColor(255, 255, 255), "%5.2f ms / %5.2f ms", float(out.time) / 1000.f, float(GetNowHiPerformanceCount() - waits) / 1000.f);

			SetJoypadDeadZone(DX_INPUT_PAD1, 0.0);
			GetJoypadDirectInputState(DX_INPUT_PAD1, &info);
			{
				font72.DrawStringFormat(0, y_r(72), GetColor(255, 255, 255), "(%d,%d)(%d,%d) (%d)", info.X, info.Y, info.Z, info.Rz, info.POV[0]);
				font72.DrawStringFormat(0, y_r(72 * 2), GetColor(255, 255, 255), "shot   = %d", in.get[2]);
				font72.DrawStringFormat(0, y_r(72 * 3), GetColor(255, 255, 255), "reload = %d", in.get[3]);
				font72.DrawStringFormat(0, y_r(72 * 4), GetColor(255, 255, 255), "dash   = %d", in.get[4]);
				font72.DrawStringFormat(0, y_r(72 * 5), GetColor(255, 255, 255), "jamp   = %d", in.get[5]);
				font72.DrawStringFormat(0, y_r(72 * 6), GetColor(255, 255, 255), "reload = %d", in.get[6]);
			}


			SetDrawScreen(DX_SCREEN_BACK);
			ClearDrawScreen();
			DrawGraph(0, 0, screen,TRUE);

			//debug->debug(GetFPS(), float(GetNowHiPerformanceCount() - waits)*0.001f);
			parts->Screen_Flip(waits);

			in.get[0] = CheckHitKey(KEY_INPUT_ESCAPE) != 0;
			in.get[1] = CheckHitKey(KEY_INPUT_P) != 0;
			in.get[7] = GetActiveFlag() == TRUE;
			in.get[8] = GetJoypadNum() >= 1 && parts->get_use_pad();

			if (in.get[8]) {
				in.m_x = info.Z;
				in.m_y = info.Rz;

				in.get[2] = info.Buttons[7] != 0;
				in.get[3] = info.Buttons[5] != 0;
				in.get[4] = info.Buttons[6] != 0;
				in.get[5] = info.Buttons[10] != 0;
				in.get[6] = info.Buttons[2] != 0;
				in.get[9] = info.Y <= -500;
				in.get[10] = info.Y >= 500;
				in.get[11] = info.X <= -500;
				in.get[12] = info.X >= 500;
			}
			else {
				in.get[2] = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
				in.get[3] = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;
				in.get[4] = CheckHitKey(KEY_INPUT_LSHIFT) != 0;
				in.get[5] = CheckHitKey(KEY_INPUT_SPACE) != 0;
				in.get[6] = CheckHitKey(KEY_INPUT_R) != 0;
				in.get[9] = (CheckHitKey(KEY_INPUT_W) != 0 || CheckHitKey(KEY_INPUT_UP) != 0);
				in.get[10] = (CheckHitKey(KEY_INPUT_S) != 0 || CheckHitKey(KEY_INPUT_DOWN) != 0);
				in.get[11] = (CheckHitKey(KEY_INPUT_A) != 0 || CheckHitKey(KEY_INPUT_LEFT) != 0);
				in.get[12] = (CheckHitKey(KEY_INPUT_D) != 0 || CheckHitKey(KEY_INPUT_RIGHT) != 0);
			}
		}
		threadparts->thead_stop();
	} while (ProcessMessage() == 0 && !out.ends);


	//StopJoypadVibration(DX_INPUT_PAD1);

	in.rcon.clear();
	return 0; // ソフトの終了
}
