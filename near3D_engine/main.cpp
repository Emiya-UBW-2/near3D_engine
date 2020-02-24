#define NOMINMAX
#include "sub.hpp"
#include "useful.hpp"
#include "make_thread.hpp"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
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
	int ammoc = 30;
	int ammoall = ammoc;
	int animetime=0;
	int cf = 0;

	struct enemiesframe {
		int time;
		std::array<MainClass::pos3D, 6> frame[2];
		MainClass::pos3D bodyframe;
	};
	struct enemies{
		MainClass::pos3D pos;
		int rad;
		std::array<MainClass::pos3D,6> frame[2];
		MainClass::pos3D bodyframe;
	};
	std::vector<enemies> enemy;
	std::vector<enemiesframe> enemyframe;

	const auto mdata = FileRead_open("data/anime.txt", FALSE);
	for (size_t j = 0; j < 2;j++) {
		enemyframe.resize(enemyframe.size() + 1);
		enemyframe[j].time = getparam_i(mdata);
		for (size_t i = 0; i < 6; i++) {
			enemyframe[j].frame[0][i] = { getparam_i(mdata),getparam_i(mdata),getparam_i(mdata) };
		}
		enemyframe[j].bodyframe = { getparam_i(mdata),getparam_i(mdata),getparam_i(mdata) };
		for (size_t i = 0; i < 6; i++) {
			enemyframe[j].frame[1][i] = { getparam_i(mdata),getparam_i(mdata),getparam_i(mdata) };
		}
	}
	FileRead_close(mdata);

	enemy.resize(1);
	enemy[0].pos.x = 1000;
	enemy[0].pos.y = 0;
	enemy[0].pos.z = 1000;
	enemy[0].rad = 0;

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
					//fpsparts->set_drw_rect(x + 400, 2000, z + 400, x, 0, z);
				}
			}
			{
				enemy[0].pos.x = 0;
				enemy[0].pos.y = 0;
				enemy[0].pos.z = 1000;
				//enemy[0].rad++;

				animetime++;
				if (animetime >= enemyframe[cf].time) {
					animetime = 0;
					cf++;
					cf %= enemyframe.size();
				}

				for (size_t i = 0; i < 6 * 2; i++) {
					//footframe[i / 6][i % 6] = {
					enemy[0].frame[i / 6][i % 6].x = enemyframe[cf].frame[i / 6][i % 6].x + (enemyframe[(cf + 1) % enemyframe.size()].frame[i / 6][i % 6].x - enemyframe[cf].frame[i / 6][i % 6].x)*animetime / enemyframe[cf].time;
					enemy[0].frame[i / 6][i % 6].y = enemyframe[cf].frame[i / 6][i % 6].y + (enemyframe[(cf + 1) % enemyframe.size()].frame[i / 6][i % 6].y - enemyframe[cf].frame[i / 6][i % 6].y)*animetime / enemyframe[cf].time;
					enemy[0].frame[i / 6][i % 6].z = enemyframe[cf].frame[i / 6][i % 6].z + (enemyframe[(cf + 1) % enemyframe.size()].frame[i / 6][i % 6].z - enemyframe[cf].frame[i / 6][i % 6].z)*animetime / enemyframe[cf].time;
				}
				enemy[0].bodyframe.x = enemyframe[cf].bodyframe.x + (enemyframe[(cf + 1) % enemyframe.size()].bodyframe.x - enemyframe[cf].bodyframe.x)*animetime / enemyframe[cf].time;
				enemy[0].bodyframe.y = enemyframe[cf].bodyframe.y + (enemyframe[(cf + 1) % enemyframe.size()].bodyframe.y - enemyframe[cf].bodyframe.y)*animetime / enemyframe[cf].time;
				enemy[0].bodyframe.z = enemyframe[cf].bodyframe.z + (enemyframe[(cf + 1) % enemyframe.size()].bodyframe.z - enemyframe[cf].bodyframe.z)*animetime / enemyframe[cf].time;


				std::array<MainClass::pos3D, 6> footframe[2];

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

				fpsparts->set_drw_line(footframe[0][0], footframe[0][1]);
				fpsparts->set_drw_line(footframe[0][2], footframe[0][1]);
				fpsparts->set_drw_line(footframe[0][3], footframe[0][1]);
				fpsparts->set_drw_line(footframe[0][3], footframe[0][4]);
				fpsparts->set_drw_line(footframe[0][3], body);
				fpsparts->set_drw_line(footframe[0][5], body);

				fpsparts->set_drw_line(footframe[1][0], footframe[1][1]);
				fpsparts->set_drw_line(footframe[1][2], footframe[1][1]);
				fpsparts->set_drw_line(footframe[1][3], footframe[1][1]);
				fpsparts->set_drw_line(footframe[1][3], footframe[1][4]);
				fpsparts->set_drw_line(footframe[1][3], body);
				fpsparts->set_drw_line(footframe[1][5], body);
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
				if (!gunflug[gunc]) {
					int xrn = 0;
					int yrn = 0;
					//xrn = -1 + GetRand(1 * 2);
					//yrn = -1 + GetRand(1 * 2);
					if (!key.get[3]) {
						xrn = -2 + GetRand(2 * 2);
						yrn = -2 + GetRand(2 * 2);
					}
					startpos[gunc] = ct;
					endpos[gunc].x = startpos[gunc].x - int(500.f*cos(deg2rad(out.xr + xrn))*sin(deg2rad(out.yr + yrn)));
					endpos[gunc].y = startpos[gunc].y - int(500.f*sin(deg2rad(out.xr + xrn)));
					endpos[gunc].z = startpos[gunc].z - int(500.f*cos(deg2rad(out.xr + xrn))*cos(deg2rad(out.yr + yrn)));
					gunflug[gunc] = true;
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
					if (gunflug[i]) {
						const auto fx = fpsparts->getpos(startpos[i]);
						const auto ex = fpsparts->getpos(endpos[i]);
						if (fx.z >= 0 && ex.z >= 0) {
							DrawLine(fx.x, fx.y, ex.x, ex.y, GetColor(255, 255, 0), 10000 / fpsparts->getdist(startpos[i], campos));
						}
						const auto oldg = startpos[i];
						startpos[i] = endpos[i];
						endpos[i].x -= (oldg.x - startpos[i].x);
						endpos[i].y -= (oldg.y - startpos[i].y);
						endpos[i].z -= (oldg.z - startpos[i].z);

						for (size_t j = 0; j < wsize; j++) {
							if (((wall[j].mpos.x - endpos[i].x)*(wall[j].mpos.x - endpos[i].x) + (wall[j].mpos.z - endpos[i].z)*(wall[j].mpos.z - endpos[i].z)
								) < 400 * 400) {
								guncnt[i] = 0;
								gunflug[i] = false;
							}
						}

						if (endpos[i].y <= 0) {
							endpos[i].y += (oldg.y - startpos[i].y) * 2;
						}
					}
					if (gunflug[i]) {
						guncnt[i]++;
						if (guncnt[i] > 60) {
							guncnt[i] = 0;
							gunflug[i] = false;
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

			//uiparts->debug(GetFPS(), float(GetNowHiPerformanceCount() - waits)*0.001f);
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
