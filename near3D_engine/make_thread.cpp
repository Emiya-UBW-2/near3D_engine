#include "make_thread.hpp"

ThreadClass::ThreadClass() {
}
ThreadClass::~ThreadClass() {
	if(thread_1.joinable())
		thread_1.detach();
}

void ThreadClass::thread_start(input& p_in, output& p_out) {
	thread_1 = std::thread([&] { calc(p_in, p_out); });
}

void ThreadClass::thead_stop() {
	thread_1.detach();
}

void ThreadClass::calc(input& p_in, output& p_out) {


	const int guns = 32;

	p_out.gun.resize(guns);
	for(auto& g : p_out.gun){
		g.hitflug = false;
		g.hitcnt = 0;
		g.gunflug = false;
		g.guncnt = 0;
	}
	p_out.gunc = 0;

	p_out.ammoc = 30;
	p_out.ammoall = p_out.ammoc;
	
	for (size_t i = 0; i < 10; i++) {
		p_out.enemy.resize(p_out.enemy.size() + 1);
		p_out.enemy[i].id = i;
		p_out.enemy[i].pos.x = -10000 + int(i) * 2000;
		p_out.enemy[i].pos.y = 0;
		p_out.enemy[i].pos.z = 3000;
		p_out.enemy[i].rad = 0;
		p_out.enemy[i].fbspeed = 0;
		p_out.enemy[i].sidespeed = 0;
		p_out.enemy[i].gun.resize(guns);
		for (auto& g : p_out.enemy[i].gun) {
			g.hitflug = false;
			g.hitcnt = 0;
			g.gunflug = false;
			g.guncnt = 0;
		}
	}



	POINT mm;
	int xo, yo;
	int xt, yt;
	while (true) {
		auto start = std::chrono::system_clock::now(); // 計測スタート時刻を保存

		p_out.ends = p_in.get[0];


		if (p_in.get[KEY_UP]) {
			p_out.y = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? -15 : -45) : ((p_in.get[KEY_M_RIGHT]) ? - 10 : -25);
		}
		if (p_in.get[KEY_DOWN]) {
			p_out.y = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? 15 : 45) : ((p_in.get[KEY_M_RIGHT]) ? 10 : 25);
		}
		if (!p_in.get[KEY_DOWN] && !p_in.get[KEY_UP] && !p_out.jf) {
			if (p_out.y > 0) {
				p_out.y--;
				if (p_out.y <= 0)
					p_out.y = 0;
			}
			if (p_out.y < 0) {
				p_out.y++;
				if (p_out.y >= 0)
					p_out.y = 0;
			}
		}

		if (p_in.get[KEY_LEFT])
			p_out.x = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? -15 : -45) : ((p_in.get[KEY_M_RIGHT]) ? -10 : -25);
		if (p_in.get[KEY_RIGHT])
			p_out.x = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? 15 : 45) : ((p_in.get[KEY_M_RIGHT]) ? 10 : 25);
		if (!p_in.get[KEY_LEFT] && !p_in.get[KEY_RIGHT] && !p_out.jf) {
			if (p_out.x > 0) {
				p_out.x--;
				if (p_out.x <= 0)
					p_out.x = 0;
			}
			if (p_out.x < 0) {
				p_out.x++;
				if (p_out.x >= 0)
					p_out.x = 0;
			}
		}



		if (p_in.get[KEY_NO_2] && !p_out.jf && p_out.jump<=2) {
			p_out.ya = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? 25 : 55) : ((p_in.get[KEY_M_RIGHT]) ? 15 : 35);
			p_out.jf = true;
			p_out.jump ++;
		}

		p_out.z += p_out.ya;
		p_out.ya--;
		if (p_out.z <= 0) {
			p_out.z = 0;
			p_out.ya = 0;
			p_out.jump = 0;
		}

		if (p_out.jf) {
			if (p_out.z <= 0) {
				p_out.jf = false;
			}
		}

		if (!p_out.jf) {
			p_out.xa = int(float(p_out.x)*cos(deg2rad(p_out.yr)) + float(p_out.y)*sin(deg2rad(p_out.yr)));
			p_out.za = int(-float(p_out.x)*sin(deg2rad(p_out.yr)) + float(p_out.y)*cos(deg2rad(p_out.yr)));
		}
		p_out.xp += p_out.xa;
		p_out.yp += p_out.za;

		xt = int(float(p_out.xa)*cos(deg2rad(p_out.yr)) + float(p_out.za)*sin(deg2rad(-p_out.yr)));
		if (p_out.xadd > xt) {
			p_out.xadd--;
		}
		else if (p_out.xadd < xt) {
			p_out.xadd++;
		}
		//p_out.xadd = xt;

		yt = p_out.ya;
		if (p_out.yadd > yt) {
			p_out.yadd--;
		}
		else if (p_out.yadd < yt) {
			p_out.yadd++;
		}
		//p_out.yadd = yt;

		GetCursorPos(&mm);
		mouse_event(MOUSE_MOVED | MOUSEEVENTF_ABSOLUTE, 65536 / 2, 65536 / 2, 0, 0);

		xo = p_out.xr;
		yo = p_out.yr;
		if (abs(mm.y - dispy / 2) <= 5) {
			p_out.xr = std::clamp<int>(
				p_out.xr + (mm.y - dispy / 2) / 2
				, -45, 45);
		}
		else {
			p_out.xr = std::clamp<int>(
				p_out.xr + std::clamp<int>((mm.y - dispy / 2) / 5,-5,5)
				, -45, 45);
		}
		if (abs(mm.x - dispx / 2) <= 5) {
			p_out.yr -= (mm.x - dispx / 2) / 2;
		}
		else {
			p_out.yr -= std::clamp<int>((mm.x - dispx / 2) / 5, -5, 5);
		}

		p_out.xradd = -(p_out.xr - xo);
		p_out.yradd = (p_out.yr - yo);


		if (p_in.get[KEY_M_LEFT] && p_out.shootf==0) {
			p_out.shootf = 1;
		}
		if (p_out.shootf != 0) {
			p_out.shootc++;
			if (p_out.shootc > 6) {
				p_out.shootc = 0;
				p_out.shootf = 0;
			}
		}


		if (p_out.shootf == 0) {

			p_out.rr = deg2rad(GetRand(90));
			p_out.st = 3 + GetRand(7);
			p_out.rtt[0].resize(p_out.st);
			for (auto& r : p_out.rtt[0])
				r = float(50 + GetRand(150));
			p_out.rtt[1].resize(p_out.st);
			for (auto& r : p_out.rtt[1])
				r = float(30 + GetRand(90));

			p_out.ct.x = (p_out.xp) - int(100.f*cos(deg2rad(p_out.xr))*sin(deg2rad(p_out.yr)));
			p_out.ct.y = (400 + p_out.z) - int(100.f*sin(deg2rad(p_out.xr)));
			p_out.ct.z = (p_out.yp) - int(100.f*cos(deg2rad(p_out.xr))*cos(deg2rad(p_out.yr)));
			/*
			p_out.ct.x = campos.x - int(100.f*cos(deg2rad(p_out.xr))*sin(deg2rad(p_out.yr)));
			p_out.ct.y = campos.y - int(100.f*sin(deg2rad(p_out.xr)));
			p_out.ct.z = campos.z - int(100.f*cos(deg2rad(p_out.xr))*cos(deg2rad(p_out.yr)));
			*/
		}
		if (p_out.shootf == 1 && p_out.ammoc > 0) {
			if (!p_out.gun[p_out.gunc].gunflug) {
				int xrn = 0;
				int yrn = 0;
				//xrn = -1 + GetRand(1 * 2);
				//yrn = -1 + GetRand(1 * 2);
				if (!p_in.get[3]) {
					xrn = -2 + GetRand(2 * 2);
					yrn = -2 + GetRand(2 * 2);
				}

				p_out.gun[p_out.gunc].startpos = p_out.ct;
				p_out.gun[p_out.gunc].endpos.x = p_out.gun[p_out.gunc].startpos.x - int(500.f*cos(deg2rad(p_out.xr + xrn))*sin(deg2rad(p_out.yr + yrn)));
				p_out.gun[p_out.gunc].endpos.y = p_out.gun[p_out.gunc].startpos.y - int(500.f*sin(deg2rad(p_out.xr + xrn)));
				p_out.gun[p_out.gunc].endpos.z = p_out.gun[p_out.gunc].startpos.z - int(500.f*cos(deg2rad(p_out.xr + xrn))*cos(deg2rad(p_out.yr + yrn)));
				p_out.gun[p_out.gunc].gunflug = true;
			}
			p_out.gunc++;
			p_out.gunc %= guns;

			p_out.ammoc--;

			p_out.shootf = 2;
		}

		for (auto& g : p_out.gun) {
			if (g.gunflug) {
				const auto oldg = g.startpos;
				g.startpos = g.endpos;
				g.endpos.x -= (oldg.x - g.startpos.x);
				g.endpos.y -= (oldg.y - g.startpos.y);
				g.endpos.z -= (oldg.z - g.startpos.z);

				for (const auto& r : p_in.rcon) {
					if (((r.mpos.x - g.endpos.x)*(r.mpos.x - g.endpos.x) + (r.mpos.z - g.endpos.z)*(r.mpos.z - g.endpos.z)) < 400 * 400) {
						g.guncnt = 0;
						g.gunflug = false;
						g.hitpos = g.endpos;
						g.hitflug = true;
					}
				}

				if (g.endpos.y <= 0) {
					g.endpos.y += (oldg.y - g.startpos.y) * 2;
					g.hitpos = g.endpos;
					g.hitpos.y = 0;
					g.hitflug = true;
				}
			}
			if (g.gunflug) {
				g.guncnt++;
				if (g.guncnt > 60) {
					g.guncnt = 0;
					g.gunflug = false;
				}
			}
			if (g.hitflug) {
				g.hitcnt++;
				if (g.hitcnt > 60 * 2) {
					g.hitcnt = 0;
					g.hitflug = false;
				}
			}
		}



		if (p_in.get[KEY_NO_3]) {//リロード
			p_out.ammoc = p_out.ammoall;
		}


		for (auto& e : p_out.enemy) {
			{
				auto radd = int(rad2deg(float(atan2<int, int>(e.pos.x - p_out.xp, p_out.yp - e.pos.z)) - deg2rad(e.rad)));
				if (radd < 0) { e.rad--; }
				else if (radd > 0) { e.rad++; }

				auto distd = int(sqrt(pow(e.pos.x - (p_out.xp), 2) + pow((p_out.yp) - e.pos.z, 2)));
				//auto distd = int(sqrt(pow(e.pos.x - campos.x, 2) + pow(campos.z - e.pos.z, 2)));
				if (distd >= 2000) {
					e.fbspeed = 10;
					e.sidespeed = 0;
				}
				else {
					e.fbspeed = -5;
					e.sidespeed = 10;
				}

				for (auto& t : p_out.enemy) {
					if (t.id != e.id && int(sqrt(pow(t.pos.x - e.pos.x, 2) + pow(t.pos.z - e.pos.z, 2))) < 2000) {
						e.pos.x -= (t.pos.x - e.pos.x) / 100;
						t.pos.x -= (e.pos.x - t.pos.x) / 100;
						e.pos.y -= (t.pos.y - e.pos.y) / 100;
						t.pos.y -= (e.pos.y - t.pos.y) / 100;
						e.pos.z -= (t.pos.z - e.pos.z) / 100;
						t.pos.z -= (e.pos.z - t.pos.z) / 100;
					}
				}
			}
			e.chose = p_in.get[1];
			{
				for (const auto& r : p_in.rcon) {
					auto distd = int(sqrt(pow(r.mpos.x - e.pos.x, 2) + pow(r.mpos.z - e.pos.z, 2)));
					if (distd <= 700) {
						auto yr = atan2f(float(e.pos.x - r.mpos.x), float(e.pos.z - r.mpos.z));
						e.pos.x += int(sin(yr)*float(-distd + 700));
						e.pos.z += int(cos(yr)*float(-distd + 700));
					}
				}
				e.pos.x = std::clamp(e.pos.x, -16000 + 200, 16000 - 200);
				e.pos.z = std::clamp(e.pos.z, -16000 + 200, 16000 - 200);

				//do : アニメーションを変更する際は以下二つを実行
				if (e.oldchose != e.chose) {
					e.animetime = 0;
					e.cf = 0;
				}

				if (e.rad <= -180)
					e.rad = 180 - (180 + e.rad);

				if (e.rad >= 180)
					e.rad = -180 + (180 - e.rad);

				e.pos.x += -int(float(e.fbspeed)*sin(deg2rad(e.rad))) + int(float(e.sidespeed)*cos(deg2rad(e.rad)));
				e.pos.y = 0;
				e.pos.z += int(float(e.fbspeed)*cos(deg2rad(e.rad))) + int(float(e.sidespeed)*sin(deg2rad(e.rad)));

				e.animetime++;
				if (e.animetime >= p_in.enemyframe[e.chose][e.cf].time) {
					e.animetime = 0;
					e.cf++;
					e.cf %= p_in.enemyframe[e.chose].size();
				}

				for (size_t i = 0; i < 6 * 2; i++) {
					e.frame[i / 6][i % 6].x = p_in.enemyframe[e.chose][e.cf].frame[i / 6][i % 6].x + (p_in.enemyframe[e.chose][(e.cf + 1) % p_in.enemyframe[e.chose].size()].frame[i / 6][i % 6].x - p_in.enemyframe[e.chose][e.cf].frame[i / 6][i % 6].x)*e.animetime / p_in.enemyframe[e.chose][e.cf].time;
					e.frame[i / 6][i % 6].y = p_in.enemyframe[e.chose][e.cf].frame[i / 6][i % 6].y + (p_in.enemyframe[e.chose][(e.cf + 1) % p_in.enemyframe[e.chose].size()].frame[i / 6][i % 6].y - p_in.enemyframe[e.chose][e.cf].frame[i / 6][i % 6].y)*e.animetime / p_in.enemyframe[e.chose][e.cf].time;
					e.frame[i / 6][i % 6].z = p_in.enemyframe[e.chose][e.cf].frame[i / 6][i % 6].z + (p_in.enemyframe[e.chose][(e.cf + 1) % p_in.enemyframe[e.chose].size()].frame[i / 6][i % 6].z - p_in.enemyframe[e.chose][e.cf].frame[i / 6][i % 6].z)*e.animetime / p_in.enemyframe[e.chose][e.cf].time;
				}
				e.bodyframe.x = p_in.enemyframe[e.chose][e.cf].bodyframe.x + (p_in.enemyframe[e.chose][(e.cf + 1) % p_in.enemyframe[e.chose].size()].bodyframe.x - p_in.enemyframe[e.chose][e.cf].bodyframe.x)*e.animetime / p_in.enemyframe[e.chose][e.cf].time;
				e.bodyframe.y = p_in.enemyframe[e.chose][e.cf].bodyframe.y + (p_in.enemyframe[e.chose][(e.cf + 1) % p_in.enemyframe[e.chose].size()].bodyframe.y - p_in.enemyframe[e.chose][e.cf].bodyframe.y)*e.animetime / p_in.enemyframe[e.chose][e.cf].time;
				e.bodyframe.z = p_in.enemyframe[e.chose][e.cf].bodyframe.z + (p_in.enemyframe[e.chose][(e.cf + 1) % p_in.enemyframe[e.chose].size()].bodyframe.z - p_in.enemyframe[e.chose][e.cf].bodyframe.z)*e.animetime / p_in.enemyframe[e.chose][e.cf].time;

				e.oldchose = e.chose;

				for (size_t i = 0; i < 6 * 2; i++) {
					if (i % 6 < 3) {
						e.footframe[i / 6][i % 6] = {
							e.pos.x + int(float(e.frame[i / 6][i % 6].x) * cos(deg2rad(e.rad))) - int(float(e.frame[i / 6][i % 6].z) * sin(deg2rad(e.rad))),
							e.pos.y + e.frame[i / 6][i % 6].y,
							e.pos.z + int(float(e.frame[i / 6][i % 6].z) * cos(deg2rad(e.rad))) + int(float(e.frame[i / 6][i % 6].x) * sin(deg2rad(e.rad))) };
					}
				}

				auto radd = int(rad2deg(float(atan2<int, int>(e.pos.x - p_out.xp, p_out.yp - e.pos.z)) - deg2rad(e.rad)));
				if (radd < -90 || radd>90) {
					if (e.radr > 0)
						e.radr -= 2;
					else if (e.radr < 0)
						e.radr += 2;
				}
				else {
					if (e.radr > radd)
						e.radr -= 2;
					else if (e.radr < radd)
						e.radr += 2;
				}
				for (size_t i = 0; i < 6 * 2; i++) {
					if (i % 6 >= 3) {
						e.footframe[i / 6][i % 6] = {
							e.pos.x + int(float(e.frame[i / 6][i % 6].x) * cos(deg2rad(e.rad + e.radr))) - int(float(e.frame[i / 6][i % 6].z) * sin(deg2rad(e.rad + e.radr))),
							e.pos.y + e.frame[i / 6][i % 6].y,
							e.pos.z + int(float(e.frame[i / 6][i % 6].z) * cos(deg2rad(e.rad + e.radr))) + int(float(e.frame[i / 6][i % 6].x) * sin(deg2rad(e.rad + e.radr))) };
					}
				}
				e.body = {
					e.pos.x + int(float(e.bodyframe.x) * cos(deg2rad(e.rad + e.radr))) - int(float(e.bodyframe.z) * sin(deg2rad(e.rad + e.radr))),
					e.pos.y + e.bodyframe.y,
					e.pos.z + int(float(e.bodyframe.z) * cos(deg2rad(e.rad + e.radr))) + int(float(e.bodyframe.x)* sin(deg2rad(e.rad + e.radr))) };

				if ((radd > -90 && radd < 90) && e.shootf == 0) { e.shootf = 1; }
				if (e.shootf != 0) {
					e.shootc++;
					if (e.shootc > 12) {
						e.shootc = 0;
						e.shootf = 0;
					}
				}


				if (e.shootf == 0) {
					e.rr = deg2rad(GetRand(90));
					e.st = 3 + GetRand(7);
					e.rtt[0].resize(e.st);
					for (auto& r : e.rtt[0])
						r = float(50 + GetRand(150));
					e.rtt[1].resize(e.st);
					for (auto& r : e.rtt[1])
						r = float(30 + GetRand(90));
				}
				if (e.shootf == 1) {
					if (!e.gun[e.gunc].gunflug) {
						int xrn = 0;
						int yrn = 0;
						xrn = -2 + GetRand(2 * 2);
						yrn = -2 + GetRand(2 * 2);
						auto xr = atan2f(float(e.body.y - (400 + p_out.z)), float(sqrt(pow<int, int>(e.body.x - (p_out.xp), 2) + pow<int, int>((p_out.yp) - e.body.z, 2))));
						//auto xr = atan2f(float(e.body.y - campos.y), float(sqrt(pow<int, int>(e.body.x - campos.x, 2) + pow<int, int>(campos.z - e.body.z, 2))));
						//auto yr = atan2f(e.body.x - campos.x, campos.z - e.body.z);
						e.gun[e.gunc].startpos = e.body;
						e.gun[e.gunc].endpos.x = e.gun[e.gunc].startpos.x - int(100.f*cos(xr + deg2rad(xrn))*sin(deg2rad(e.rad + e.radr + yrn)));
						e.gun[e.gunc].endpos.y = e.gun[e.gunc].startpos.y - int(100.f*sin(xr + deg2rad(xrn)));
						e.gun[e.gunc].endpos.z = e.gun[e.gunc].startpos.z + int(100.f*cos(xr + deg2rad(xrn))*cos(deg2rad(e.rad + e.radr)));
						e.gun[e.gunc].gunflug = true;
					}
					e.gunc++;
					e.gunc %= guns;

					e.shootf = 2;
				}

			}

			for (auto& g : e.gun) {
				if (g.gunflug) {
					const auto oldg = g.startpos;
					g.startpos = g.endpos;
					g.endpos.x -= (oldg.x - g.startpos.x);
					g.endpos.y -= (oldg.y - g.startpos.y);
					g.endpos.z -= (oldg.z - g.startpos.z);

					for (const auto& r : p_in.rcon) {
						if (((r.mpos.x - g.endpos.x)*(r.mpos.x - g.endpos.x) + (r.mpos.z - g.endpos.z)*(r.mpos.z - g.endpos.z)) < 400 * 400) {
							g.guncnt = 0;
							g.gunflug = false;
							g.hitpos = g.endpos;
							g.hitflug = true;
						}
					}

					if (g.endpos.y <= 0) {
						g.endpos.y += (oldg.y - g.startpos.y) * 2;
						g.hitpos = g.endpos;
						g.hitpos.y = 0;
						g.hitflug = true;
					}
				}
				if (g.gunflug) {
					g.guncnt++;
					if (g.guncnt > 60 * 3) {
						g.guncnt = 0;
						g.gunflug = false;
					}
				}
				if (g.hitflug) {
					g.hitcnt++;
					if (g.hitcnt > 60 * 5) {
						g.hitcnt = 0;
						g.hitflug = false;
					}
				}
			}
		}



		while (true) {
			if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() >= 1000000 / 60)
				break;
		}
	}
}
