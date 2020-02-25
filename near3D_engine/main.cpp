#define NOMINMAX
#include "sub.hpp"
#include "useful.hpp"
#include "make_thread.hpp"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	using namespace std::literals;

	input key{ 0 };
	output out{ 0 };
	switches aim, map, vch; /*視点変更*/

	auto threadparts = std::make_unique<ThreadClass>(); /*演算クラス*/
	auto parts = std::make_unique<MainClass>(); /*汎用クラス*/
	//auto drawparts = std::make_unique<Draw_lookdown>(); /*見下ろし描画クラス*/
	auto uiparts = std::make_unique<UIS>(); /*汎用クラス*/
	auto fpsparts = std::make_unique<Draw_fps>(); /*fps描画クラス*/

	MainClass::pos3D campos = { 0,0,0 };
	MainClass::pos3D ct = { 0,0,0 };


	float rr;
	int st;
	std::vector<float> rtt[2];
	const int guns = 32;

	struct gunes {
		bool hitflug;
		unsigned int hitcnt;
		bool gunflug;
		unsigned int guncnt;
		MainClass::pos3D startpos;
		MainClass::pos3D endpos;
		MainClass::pos3D hitpos;
	};

	std::array<gunes, guns> gun;
	int gunc = 0;
	for (int i = 0; i < guns; i++) {
		gun[i].hitflug = false;
		gun[i].hitcnt = 0;
		gun[i].gunflug = false;
		gun[i].guncnt = 0;
		gun[i].startpos = campos;
		gun[i].endpos = campos;
	}
	int ammoc = 30;
	int ammoall = ammoc;

	struct enemiesframe {
		int time;
		std::array<MainClass::pos3D, 6> frame[2];
		MainClass::pos3D bodyframe;
	};
	struct enemies{
		MainClass::pos3D pos;
		int rad;
		int fbspeed;
		int sidespeed;
		int chose;
		int oldchose;

		int animetime = 0;
		int cf = 0;

		std::array<MainClass::pos3D,6> frame[2];
		MainClass::pos3D bodyframe;
	};
	std::vector<enemies> enemy;
	std::vector<enemiesframe> enemyframe[2];

	std::array<MainClass::pos3D, 6> footframe[2];

	for (size_t k = 0; k < 2; k++) {
		const auto mdata = FileRead_open(("data/enemy_animetion/"s + std::to_string(k) + ".txt").c_str(), FALSE);
		for (size_t j = 0; j < 2; j++) {
			enemyframe[k].resize(enemyframe[k].size() + 1);
			enemyframe[k][j].time = getparam_i(mdata);
			for (size_t i = 0; i < 6; i++) {
				enemyframe[k][j].frame[0][i] = { getparam_i(mdata),getparam_i(mdata),getparam_i(mdata) };
			}
			enemyframe[k][j].bodyframe = { getparam_i(mdata),getparam_i(mdata),getparam_i(mdata) };
			for (size_t i = 0; i < 6; i++) {
				enemyframe[k][j].frame[1][i] = { getparam_i(mdata),getparam_i(mdata),getparam_i(mdata) };
			}
		}
		FileRead_close(mdata);
	}
	enemy.resize(1);
	enemy[0].pos.x = 0;
	enemy[0].pos.y = 0;
	enemy[0].pos.z = 1000;
	enemy[0].rad = 0;
	enemy[0].fbspeed = 0;
	enemy[0].sidespeed = 0;

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
		//SetBackgroundColor(255, 255, 255);
		while (ProcessMessage() == 0 && !out.ends) {
			const auto waits = GetNowHiPerformanceCount();
			uiparts->put_way();
			SetDrawScreen(screen);
			ClearDrawScreen();
			//
			/*
			//見下ろしサンプル
			int tile = 32;
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
			fpsparts->set_cam(campos, out.xr, out.yr, 0, key.get[3]? 75 : 110);

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
					fpsparts->set_drw_rect(x + 400, 2000, z + 400, x, 0, z);
				}
			}
			enemy[0].rad++;
			enemy[0].fbspeed = 10;
			enemy[0].sidespeed = 10;
			enemy[0].chose = key.get[1];
			{
				//todo : アニメーションを変更する際は以下二つを実行
				if (enemy[0].oldchose != enemy[0].chose) {
					enemy[0].animetime = 0;
					enemy[0].cf = 0;
				}

				enemy[0].pos.x += -int(float(enemy[0].fbspeed)*sin(deg2rad(enemy[0].rad))) + int(float(enemy[0].sidespeed)*cos(deg2rad(enemy[0].rad)));
				enemy[0].pos.y = 0;
				enemy[0].pos.z += int(float(enemy[0].fbspeed)*cos(deg2rad(enemy[0].rad))) + int(float(enemy[0].sidespeed)*sin(deg2rad(enemy[0].rad)));

				enemy[0].animetime++;
				if (enemy[0].animetime >= enemyframe[enemy[0].chose][enemy[0].cf].time) {
					enemy[0].animetime = 0;
					enemy[0].cf++;
					enemy[0].cf %= enemyframe[enemy[0].chose].size();
				}

				for (size_t i = 0; i < 6 * 2; i++) {
					enemy[0].frame[i / 6][i % 6].x = enemyframe[enemy[0].chose][enemy[0].cf].frame[i / 6][i % 6].x + (enemyframe[enemy[0].chose][(enemy[0].cf + 1) % enemyframe[enemy[0].chose].size()].frame[i / 6][i % 6].x - enemyframe[enemy[0].chose][enemy[0].cf].frame[i / 6][i % 6].x)*enemy[0].animetime / enemyframe[enemy[0].chose][enemy[0].cf].time;
					enemy[0].frame[i / 6][i % 6].y = enemyframe[enemy[0].chose][enemy[0].cf].frame[i / 6][i % 6].y + (enemyframe[enemy[0].chose][(enemy[0].cf + 1) % enemyframe[enemy[0].chose].size()].frame[i / 6][i % 6].y - enemyframe[enemy[0].chose][enemy[0].cf].frame[i / 6][i % 6].y)*enemy[0].animetime / enemyframe[enemy[0].chose][enemy[0].cf].time;
					enemy[0].frame[i / 6][i % 6].z = enemyframe[enemy[0].chose][enemy[0].cf].frame[i / 6][i % 6].z + (enemyframe[enemy[0].chose][(enemy[0].cf + 1) % enemyframe[enemy[0].chose].size()].frame[i / 6][i % 6].z - enemyframe[enemy[0].chose][enemy[0].cf].frame[i / 6][i % 6].z)*enemy[0].animetime / enemyframe[enemy[0].chose][enemy[0].cf].time;
				}
				enemy[0].bodyframe.x = enemyframe[enemy[0].chose][enemy[0].cf].bodyframe.x + (enemyframe[enemy[0].chose][(enemy[0].cf + 1) % enemyframe[enemy[0].chose].size()].bodyframe.x - enemyframe[enemy[0].chose][enemy[0].cf].bodyframe.x)*enemy[0].animetime / enemyframe[enemy[0].chose][enemy[0].cf].time;
				enemy[0].bodyframe.y = enemyframe[enemy[0].chose][enemy[0].cf].bodyframe.y + (enemyframe[enemy[0].chose][(enemy[0].cf + 1) % enemyframe[enemy[0].chose].size()].bodyframe.y - enemyframe[enemy[0].chose][enemy[0].cf].bodyframe.y)*enemy[0].animetime / enemyframe[enemy[0].chose][enemy[0].cf].time;
				enemy[0].bodyframe.z = enemyframe[enemy[0].chose][enemy[0].cf].bodyframe.z + (enemyframe[enemy[0].chose][(enemy[0].cf + 1) % enemyframe[enemy[0].chose].size()].bodyframe.z - enemyframe[enemy[0].chose][enemy[0].cf].bodyframe.z)*enemy[0].animetime / enemyframe[enemy[0].chose][enemy[0].cf].time;

				enemy[0].oldchose = enemy[0].chose;

				for (size_t i = 0; i < 6 * 2; i++) {
					footframe[i / 6][i % 6] = {
						enemy[0].pos.x + int(float(enemy[0].frame[i / 6][i % 6].x) * cos(deg2rad(enemy[0].rad))) - int(float(enemy[0].frame[i / 6][i % 6].z) * sin(deg2rad(enemy[0].rad))),
						enemy[0].pos.y + enemy[0].frame[i / 6][i % 6].y,
						enemy[0].pos.z + int(float(enemy[0].frame[i / 6][i % 6].z) * cos(deg2rad(enemy[0].rad))) + int(float(enemy[0].frame[i / 6][i % 6].x) * sin(deg2rad(enemy[0].rad))) };
				}
				const MainClass::pos3D body = {
					enemy[0].pos.x + int(float(enemy[0].bodyframe.x) * cos(deg2rad(enemy[0].rad))) - int(float(enemy[0].bodyframe.z) * sin(deg2rad(enemy[0].rad))),
					enemy[0].pos.y + enemy[0].bodyframe.y,
					enemy[0].pos.z + int(float(enemy[0].bodyframe.z) * cos(deg2rad(enemy[0].rad))) + int(float(enemy[0].bodyframe.x)* sin(deg2rad(enemy[0].rad))) };

				fpsparts->draw_line(footframe[0][0], footframe[0][1], 20000, 0);
				fpsparts->draw_line(footframe[0][2], footframe[0][1], 20000, 0);
				fpsparts->draw_line(footframe[0][3], footframe[0][1], 20000, 0);
				fpsparts->draw_line(footframe[0][3], footframe[0][4], 20000, 0);
				fpsparts->draw_line(footframe[0][3], body, 20000, 0);
				fpsparts->draw_line(footframe[0][5], body, 20000, 0);

				fpsparts->draw_line(footframe[1][0], footframe[1][1], 20000, 0);
				fpsparts->draw_line(footframe[1][2], footframe[1][1], 20000, 0);
				fpsparts->draw_line(footframe[1][3], footframe[1][1], 20000, 0);
				fpsparts->draw_line(footframe[1][3], footframe[1][4], 20000, 0);
				fpsparts->draw_line(footframe[1][3], body, 20000, 0);
				fpsparts->draw_line(footframe[1][5], body, 20000, 0);
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
			if (out.shootf == 1 && ammoc > 0) {
				if (!gun[gunc].gunflug) {
					int xrn = 0;
					int yrn = 0;
					//xrn = -1 + GetRand(1 * 2);
					//yrn = -1 + GetRand(1 * 2);
					if (!key.get[3]) {
						xrn = -2 + GetRand(2 * 2);
						yrn = -2 + GetRand(2 * 2);
					}
					gun[gunc].startpos = ct;
					gun[gunc].endpos.x = gun[gunc].startpos.x - int(500.f*cos(deg2rad(out.xr + xrn))*sin(deg2rad(out.yr + yrn)));
					gun[gunc].endpos.y = gun[gunc].startpos.y - int(500.f*sin(deg2rad(out.xr + xrn)));
					gun[gunc].endpos.z = gun[gunc].startpos.z - int(500.f*cos(deg2rad(out.xr + xrn))*cos(deg2rad(out.yr + yrn)));
					gun[gunc].gunflug = true;
				}
				gunc++;
				gunc %= guns;

				ammoc--;

				out.shootf = 2;
			}
			if (out.shootf == 2 && out.shootc<=4) {
				auto t = fpsparts->getpos(ct);
				t.x -= dispx / 2;
				t.y -= dispy / 2;
				if (t.z >= 0) {
					for (int i = 0; i < st; i++) {
						DrawLine(
							dispx / 2 + x_r(t.x + int(rtt[0][i] * cos(rr + DX_PI_F * 2 * (i) / st))),
							dispy / 2 + y_r(t.y + int(rtt[0][i] * sin(rr + DX_PI_F * 2 * (i) / st))),
							dispx / 2 + x_r(t.x + int(rtt[0][(i + 1) % st] * cos(rr + DX_PI_F * 2 * (i + 1) / st))),
							dispy / 2 + y_r(t.y + int(rtt[0][(i + 1) % st] * sin(rr + DX_PI_F * 2 * (i + 1) / st))),
							GetColor(255 * out.shootc / 4, 255 * out.shootc / 4, 0));
						DrawLine(
							dispx / 2 + x_r(t.x + int(rtt[1][i] * cos(rr + DX_PI_F * 2 * (i) / st))),
							dispy / 2 + y_r(t.y + int(rtt[1][i] * sin(rr + DX_PI_F * 2 * (i) / st))),
							dispx / 2 + x_r(t.x + int(rtt[1][(i + 1) % st] * cos(rr + DX_PI_F * 2 * (i + 1) / st))),
							dispy / 2 + y_r(t.y + int(rtt[1][(i + 1) % st] * sin(rr + DX_PI_F * 2 * (i + 1) / st))),
							GetColor(255 * out.shootc / 4, 0, 0));
					}
				}
			}
			{
				const auto wall = fpsparts->getrectpos();
				const auto wsize = fpsparts->getrectsize();

				for (int i = 0; i < guns; i++) {
					if (gun[i].gunflug) {
						const auto fx = fpsparts->getpos(gun[i].startpos);
						const auto ex = fpsparts->getpos(gun[i].endpos);
						if (fx.z >= 0 && ex.z >= 0) {
							const auto fr = 10000 / fpsparts->getdist(gun[i].startpos, campos) / 2;
							const auto er = 10000 / fpsparts->getdist(gun[i].endpos, campos) / 2;

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
						const auto oldg = gun[i].startpos;
						gun[i].startpos = gun[i].endpos;
						gun[i].endpos.x -= (oldg.x - gun[i].startpos.x);
						gun[i].endpos.y -= (oldg.y - gun[i].startpos.y);
						gun[i].endpos.z -= (oldg.z - gun[i].startpos.z);

						for (size_t j = 0; j < wsize; j++) {
							if (((wall[j].mpos.x - gun[i].endpos.x)*(wall[j].mpos.x - gun[i].endpos.x) + (wall[j].mpos.z - gun[i].endpos.z)*(wall[j].mpos.z - gun[i].endpos.z)) < 400 * 400) {
								gun[i].guncnt = 0;
								gun[i].gunflug = false;
								gun[i].hitpos = gun[i].endpos;
								gun[i].hitflug = true;
							}
						}

						if (gun[i].endpos.y <= 0) {
							gun[i].endpos.y += (oldg.y - gun[i].startpos.y) * 2;
							gun[i].hitpos = gun[i].endpos;
							gun[i].hitpos.y = 0;
							gun[i].hitflug = true;
						}
					}
					if (gun[i].gunflug) {
						gun[i].guncnt++;
						if (gun[i].guncnt > 60) {
							gun[i].guncnt = 0;
							gun[i].gunflug = false;
						}
					}
					if (gun[i].hitflug) {
						const auto hx = fpsparts->getpos(gun[i].hitpos);
						if (hx.z >= 0) {
							const auto rr = 10000 / fpsparts->getdist(gun[i].hitpos, campos) * 2;
							DrawLine(hx.x, hx.y + rr, hx.x + rr, hx.y, GetColor(255 - 255 * gun[i].hitcnt / (60 * 2), 255 - 255 * gun[i].hitcnt / (60 * 2), 0));
							DrawLine(hx.x + rr, hx.y, hx.x, hx.y - rr, GetColor(255 - 255 * gun[i].hitcnt / (60 * 2), 255 - 255 * gun[i].hitcnt / (60 * 2), 0));
							DrawLine(hx.x, hx.y - rr, hx.x - rr, hx.y, GetColor(255 - 255 * gun[i].hitcnt / (60 * 2), 255 - 255 * gun[i].hitcnt / (60 * 2), 0));
							DrawLine(hx.x - rr, hx.y, hx.x, hx.y + rr, GetColor(255 - 255 * gun[i].hitcnt / (60 * 2), 255 - 255 * gun[i].hitcnt / (60 * 2), 0));
						}
						gun[i].hitcnt++;
						if (gun[i].hitcnt > 60*2) {
							gun[i].hitcnt = 0;
							gun[i].hitflug = false;
						}
					}
				}
			}

			if (key.get[6]) {
				ammoc = ammoall;
			}

			fpsparts->end_drw();
			//照星
			{
				DrawLine(
					dispx / 2 + x_r(-3 + out.xadd - out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd - out.xradd) * 110 / (key.get[3] ? 75 : 110),
					dispx / 2 + x_r(-3 + out.xadd - out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(20 + out.yadd - out.xradd) * 110 / (key.get[3] ? 75 : 110),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(-3 + out.xadd - out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd - out.xradd) * 110 / (key.get[3] ? 75 : 110),
					dispx / 2 + x_r(3 + out.xadd - out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd - out.xradd) * 110 / (key.get[3] ? 75 : 110),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(3 + out.xadd - out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd - out.xradd) * 110 / (key.get[3] ? 75 : 110),
					dispx / 2 + x_r(3 + out.xadd - out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(20 + out.yadd - out.xradd) * 110 / (key.get[3] ? 75 : 110),
					GetColor(255, 255, 255));
			}
			//照門
			{
				DrawLine(
					dispx / 2 + x_r(-20 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					dispx / 2 + x_r(-10 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(-10 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					dispx / 2 + x_r(-10 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(10 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(-10 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(10 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					dispx / 2 + x_r(10 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(10 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(10 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					dispx / 2 + x_r(10 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(10 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					GetColor(255, 255, 255));
				DrawLine(
					dispx / 2 + x_r(10 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					dispx / 2 + x_r(20 + out.xadd + out.yradd) * 110 / (key.get[3] ? 75 : 110),
					dispy / 2 + y_r(0 + out.yadd + out.xradd) * 110 / (key.get[3] ? 75 : 110),
					GetColor(255, 255, 255));
			}
			//
			uiparts->end_way();

			for (size_t i = 0; i < ammoc; i++) {
				if (i != ammoc - 1) {
					if (i < 5) {
						DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(40), x_r(10 + i * 12 + 0), dispy - y_r(10), GetColor(255, 0, 0));
						DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(40), x_r(10 + i * 12 + 3), dispy - y_r(50), GetColor(255, 0, 0));
						DrawLine(x_r(10 + i * 12 + 3), dispy - y_r(50), x_r(10 + i * 12 + 6), dispy - y_r(40), GetColor(255, 0, 0));
						DrawLine(x_r(10 + i * 12 + 6), dispy - y_r(40), x_r(10 + i * 12 + 6), dispy - y_r(10), GetColor(255, 0, 0));
						DrawLine(x_r(10 + i * 12 + 0), dispy - y_r(10), x_r(10 + i * 12 + 6), dispy - y_r(10), GetColor(255, 0, 0));
					}
					else if (i != ammoc - 1) {
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


			for (size_t i = 0; i < 20; i++) {
					if (i < 6) {
						DrawLine(dispx - x_r(130 + i * 14 + 0), y_r(20 + 100 / (i + 1)), dispx - x_r(130 + i * 14 + 0), y_r(10), GetColor(255, 0, 0));
						DrawLine(dispx - x_r(130 + i * 14 + 0), y_r(20 + 100 / (i + 1)), dispx - x_r(130 + i * 14 + 9), y_r(20 + 100 / (i + 2)), GetColor(255, 0, 0));
						DrawLine(dispx - x_r(130 + i * 14 + 9), y_r(20 + 100 / (i + 2)), dispx - x_r(130 + i * 14 + 9), y_r(10), GetColor(255, 0, 0));
						DrawLine(dispx - x_r(130 + i * 14 + 0), y_r(10), dispx - x_r(130 + i * 14 + 9), y_r(10), GetColor(255, 0, 0));
					}
					else {
						DrawLine(dispx - x_r(130 + i * 14 + 0), y_r(20 + 100 / (i + 1)), dispx - x_r(130 + i * 14 + 0), y_r(10), GetColor(255, 255, 0));
						DrawLine(dispx - x_r(130 + i * 14 + 0), y_r(20 + 100 / (i + 1)), dispx - x_r(130 + i * 14 + 9), y_r(20 + 100 / (i + 2)), GetColor(255, 255, 0));
						DrawLine(dispx - x_r(130 + i * 14 + 9), y_r(20 + 100 / (i + 2)), dispx - x_r(130 + i * 14 + 9), y_r(10), GetColor(255, 255, 0));
						DrawLine(dispx - x_r(130 + i * 14 + 0), y_r(10), dispx - x_r(130 + i * 14 + 9), y_r(10), GetColor(255, 255, 0));
					}
			}

			font72.DrawStringFormat(0, dispy - y_r(132), GetColor(255, 255, 255), "%d / %d", ammoc, ammoall);

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
                                key.get[6] = CheckHitKey(KEY_INPUT_R) != 0;
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
