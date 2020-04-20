#pragma once

#include "sub.hpp"
#include "useful.hpp"
#include <thread> 
#include <chrono>
#include <array> 


//入力
enum Key {
	KEY_ESCAPE = 0,
	KEY_PAUSE = 1,
	KEY_M_LEFT = 2,
	KEY_M_RIGHT = 3,
	KEY_NO_1 = 4,
	KEY_NO_2 = 5,
	KEY_NO_3 = 6,
	ACTIVE = 7,
	ON_PAD = 8,
	KEY_UP = 9,
	KEY_DOWN = 10,
	KEY_LEFT = 11,
	KEY_RIGHT = 12
};
struct input {
	std::array<bool, 13> get; /*キー用(一時監視)*/

	int m_x, m_y;

	struct enemiesframe {
		int time;
		std::array<Draw_fps::pos3D, 6> frame[2];
		Draw_fps::pos3D bodyframe;
	};
	std::array<std::vector<enemiesframe>,2> enemyframe;//アニメーション

	struct rectcon {
		std::array<Draw_fps::pos3D, 2> pos;
		Draw_fps::pos3D mpos;
	};
	std::vector<rectcon> rcon;//柱
};
struct output {
	struct gunes {
		bool hitflug;
		unsigned int hitcnt;
		bool gunflug;
		unsigned int guncnt;
		Draw_fps::pos3D startpos;
		Draw_fps::pos3D endpos;
		Draw_fps::pos3D hitpos;
	};


	bool ends{ false }; /*終了フラグ*/
	bool respawn{ false }; /*ゲームオーバーフラグ*/
	LONGLONG time;/*処理時間*/

	Draw_fps::pos3D pos;//座標
	Draw_fps::pos3D ace;//加速度
	Draw_fps::pos3D inp;//入力

	int xadd, yadd;//ブレ
	int xradd, yradd;//ブレ

	bool jf{ false };//ジャンプフラグ
	uint8_t jump{ 0 };//ジャンプ回数カウンター

	int xr = 0, yr = 180;//視点回転角
	int xo = 0, yo = 180;//視点回転角

	int8_t hitp = 20;//体力

	bool shootf{ false };//射撃フラグ
	int shootc = 0;//射撃カウント
	int ammoc;//弾数
	int ammoall;//装弾数

	bool reloadf{ false };//射撃フラグ
	int reloadc = 0;//射撃カウント
	int reloadall;//リロードタイム

	std::vector<gunes> gun;//弾情報
	size_t gunc = 0;//使用弾カウンタ

	//発砲炎
	int rr;
	int st;
	std::vector<int> rtt[2];
	Draw_fps::pos3D ct = { 0,0,0 };

	struct enemies {
		size_t id;
		Draw_fps::pos3D pos;
		int rad;
		int radr;

		int biimrad;

		int fbspeed;
		int sidespeed;
		int chose;
		int oldchose;

		int animetime = 0;
		int cf = 0;

		std::array<Draw_fps::pos3D, 6> frame[2];
		Draw_fps::pos3D bodyframe;
		std::vector<gunes> gun;
		int gunc = 0;
		bool shootf{ false };
		int shootc = 0;

		std::array<Draw_fps::pos3D, 6> footframe[2];
		Draw_fps::pos3D body;

		std::array<int, 6> hitp[2];
	};

	std::vector<enemies> enemy;
};
//60fpsを維持しつつ操作を演算(box2D込み)
class ThreadClass {
private:
	std::thread thread_1;
	inline Draw_fps::pos3D getadd(Draw_fps::pos3D pos1, Draw_fps::pos3D pos2) {
		Draw_fps::pos3D pos3 = { pos1.x + pos2.x, pos1.y + pos2.y, pos1.z + pos2.z };
		return pos3;
	}
	inline Draw_fps::pos3D getsub(Draw_fps::pos3D pos1, Draw_fps::pos3D pos2) {
		Draw_fps::pos3D pos3 = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };
		return pos3;
	}
	inline Draw_fps::pos3D getscale(Draw_fps::pos3D pos1, int size) {
		Draw_fps::pos3D pos3 = { pos1.x * size, pos1.y * size, pos1.z * size };
		return pos3;
	}
	inline int getdist(Draw_fps::pos3D pos1) { return CalcDist(CalcDist(pos1.x, pos1.y), pos1.z); }
	inline int getdist(Draw_fps::pos3D pos1, Draw_fps::pos3D pos2) { return getdist(getsub(pos1, pos2)); }
	inline int getdot(Draw_fps::pos3D pos1, Draw_fps::pos3D pos2) { return pos1.x*pos2.x + pos1.y*pos2.y + pos1.z*pos2.z; }
	Draw_fps::pos3D GLPM(Draw_fps::pos3D Point, Draw_fps::pos3D LinePos1, Draw_fps::pos3D LinePos2) {
		auto Line12 = getsub(LinePos2, LinePos1);
		const auto Dot12_1P = getdot(Line12, getsub(Point, LinePos1));
		if (Dot12_1P <= 0) {
			return LinePos1;
		}
		const auto Length12 = getdot(Line12, Line12);
		if (Length12 == 0) {
			return LinePos1;
		}
		if (Length12 < Dot12_1P) {
			return LinePos2;
		}
		Line12.x = Line12.x *Dot12_1P / Length12;
		Line12.y = Line12.y *Dot12_1P / Length12;
		Line12.z = Line12.z *Dot12_1P / Length12;
		return getadd(LinePos1, Line12);
	}
	// 二つの線分がもっとも近づく点の距離を得る
	const auto SSML(Draw_fps::pos3D SA1, Draw_fps::pos3D SA2, Draw_fps::pos3D SB1, Draw_fps::pos3D SB2) {
		const auto da = getsub(SA2, SA1);
		const auto db = getsub(SB2, SB1);
		const auto dc = getsub(SA1, SB1);
		//int 
		const auto a = getdot(da, da);
		const auto b = getdot(da, db);
		const auto d = getdot(db, db);
		const auto e = getdot(da, dc);
		const auto f = getdot(db, dc);

		// SA1 - SA2 または SB1 - SB2 の距離が限りなくゼロに近いかどうかのチェック
		if (abs(-d) == 0) {
			if (abs(a) == 0) {
				return getdist(SA1, SB1);
			}
			else {
				return getdist(GLPM(SB1, SA1, SA2), SB1);
			}
		}
		else {
			if (abs(a) == 0) {
				return getdist(GLPM(SA1, SB1, SB2), SA1);
			}
		}
		int s, t;
		if ((-a * d + b * b) <= 0) {
			// クラーメルの公式
			s = (-b * f + e * d) * 100000000;
			t = (-a * f + e * b) * 100000000;
		}
		else {
			s = -1;
			t = 0;
		}

		// 二つの線分が縮退していたら点と見なして点同士の距離を返す
		if (a <= 0.0f && d <= 0.0f) {
			return getdist(SA1, SB1);
		}
		if (a <= 0.0f) {
			s = 0;
			if (t > 1 || t < 0) {
				t = t > 1;
			}
		}
		else {
			if (d <= 0.0f) {
				t = 0;
				if (s > 1 || s < 0) {
					s = s > 1;
				}
			}
			else {
				if (s > 1 || s < 0) {
					s = s > 1;
					t = (f + b * s) / d;		// ガウス消去法
					if (t > 1 || t < 0) {
						t = t > 1;
						s = -(e - b * t) / a;	// ガウス消去法
						if (s > 1 || s < 0) {
							s = s > 1;
						}
					}
				}
				else {
					if (t > 1 || t < 0) {
						t = t > 1;
						s = -(e - b * t) / a;		// ガウス消去法
						if (s > 1 || s < 0) {
							s = s > 1;
							t = (f + b * s) / d;	// ガウス消去法
							if (t > 1 || t < 0) {
								t = t > 1;
							}
						}
					}
				}
			}
		}
		return getdist(getadd(getscale(da, s), SA1), getadd(getscale(db, t), SB1));
	}

	void calc(input& p_in, output& p_out) {
		const int guns = 32;
		//初期化
		do {
			for (int i = 0; i < 60; i++) {
				int x, z;
				x = -40 + 5 + GetRand(80 - 5 * 2);
				z = -40 + 5 + GetRand(80 - 5 * 2);
				if (i != 0) {
					do {
						x = -40 + 5 + GetRand(80 - 5 * 2);
					} while (abs(x * 400 - p_in.rcon.back().pos[1].x) <= 800);
					do {
						z = -40 + 5 + GetRand(80 - 5 * 2);
					} while (abs(z * 400 - p_in.rcon.back().pos[1].z) <= 800);
				}
				p_in.rcon.resize(p_in.rcon.size() + 1);
				p_in.rcon.back().pos[0] = { x * 400 + 400, 4000, z * 400 + 400 };
				p_in.rcon.back().mpos = { x * 400 + 200, 2000, z * 400 + 200 };
				p_in.rcon.back().pos[1] = { x * 400 , 0, z * 400 };

			}

			p_out.respawn = false;
			p_out.pos = { 0,0,0 };

			p_out.gun.resize(guns);
			for (auto& g : p_out.gun) {
				g.hitflug = false;
				g.hitcnt = 0;
				g.gunflug = false;
				g.guncnt = 0;
			}
			p_out.gunc = 0;
			for (size_t i = 0; i < 10; i++) {
				p_out.enemy.resize(p_out.enemy.size() + 1);
				p_out.enemy[i].id = i;
				p_out.enemy[i].fbspeed = 0;
				p_out.enemy[i].sidespeed = 0;
				p_out.enemy[i].gun.resize(guns);
				for (auto& g : p_out.enemy[i].gun) {
					g.hitflug = false;
					g.hitcnt = 0;
					g.gunflug = false;
					g.guncnt = 0;
				}
				p_out.enemy[i].gunc = 0;
				p_out.enemy[i].hitp[0].fill(1);
				p_out.enemy[i].hitp[1].fill(1);
			}
			//
			p_out.hitp = 20;
			p_out.ammoall = 30;

			p_out.reloadall = 60 * 1;
			p_out.ammoc = p_out.ammoall;
			for (auto& ep : p_out.enemy) {
				ep.pos.x = -10000 + int(ep.id % 10) * 2000;
				ep.pos.y = 0;
				ep.pos.z = 2000 + int(ep.id / 10) * 100;
				ep.rad = 0;
			}



			while (!p_out.respawn) {
				auto start = std::chrono::system_clock::now(); // 計測スタート時刻を保存

				p_out.ends = p_in.get[0];
				if (p_out.ends)
					break;

				if (p_in.get[KEY_UP])
					p_out.inp.z = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? -15 : -35) : ((p_in.get[KEY_M_RIGHT]) ? -10 : -25);
				if (p_in.get[KEY_DOWN])
					p_out.inp.z = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? 15 : 35) : ((p_in.get[KEY_M_RIGHT]) ? 10 : 25);
				if (!p_in.get[KEY_DOWN] && !p_in.get[KEY_UP] && !p_out.jf) {
					if (p_out.inp.z > 0)  --p_out.inp.z;
					if (p_out.inp.z < 0)  ++p_out.inp.z;
				}

				if (p_in.get[KEY_LEFT])
					p_out.inp.x = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? -15 : -35) : ((p_in.get[KEY_M_RIGHT]) ? -10 : -25);
				if (p_in.get[KEY_RIGHT])
					p_out.inp.x = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? 15 : 35) : ((p_in.get[KEY_M_RIGHT]) ? 10 : 25);
				if (!p_in.get[KEY_LEFT] && !p_in.get[KEY_RIGHT] && !p_out.jf) {
					if (p_out.inp.x > 0)  --p_out.inp.x;
					if (p_out.inp.x < 0)  ++p_out.inp.x;
				}

				if (p_in.get[KEY_NO_2] && !p_out.jf) {// && p_out.jump <= 2) {
					p_out.ace.y = ((p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? 25 : 55) : ((p_in.get[KEY_M_RIGHT]) ? 15 : 35))*abs(20000 - p_out.pos.y) / 20000;
					p_out.jf = true;
					p_out.jump++;
				}

				p_out.pos.y += p_out.ace.y;
				p_out.ace.y--;
				if (p_out.pos.y <= 0) {
					p_out.pos.y = 0;
					p_out.ace.y = 0;
					p_out.jump = 0;
					p_out.jf = false;
				}

				if (!p_out.jf) {
					p_out.ace.x = cos_i(p_out.yr, 1000, p_out.inp.x) + sin_i(p_out.yr, 1000, p_out.inp.z);
					p_out.ace.z = cos_i(p_out.yr, 1000, p_out.inp.z) - sin_i(p_out.yr, 1000, p_out.inp.x);
				}

				p_out.pos.x += p_out.ace.x;
				p_out.pos.z += p_out.ace.z;



				for (const auto& r : p_in.rcon) {
					auto distd = CalcDist(r.mpos.x - p_out.pos.x, r.mpos.z - p_out.pos.z);
					if (distd <= 400) {
						auto yr = int(rad2deg(atan2f(float(p_out.pos.x - r.mpos.x), float(p_out.pos.z - r.mpos.z))));
						p_out.pos.x += sin_i(yr, 100, 400 - distd);
						p_out.pos.z += cos_i(yr, 100, 400 - distd);
						p_out.jf = false;
					}
				}
				p_out.pos.x = std::clamp(p_out.pos.x, -16000 + 200, 16000 - 200);
				p_out.pos.z = std::clamp(p_out.pos.z, -16000 + 200, 16000 - 200);









				{
					int xt = -cos_i(p_out.yr, 100, p_out.ace.x) + sin_i(p_out.yr, 100, p_out.ace.z);
					if (p_out.xadd > xt) {
						p_out.xadd--;
					}
					else if (p_out.xadd < xt) {
						p_out.xadd++;
					}
				}
				if (p_out.yadd > p_out.ace.y) {
					p_out.yadd--;
				}
				else if (p_out.yadd < p_out.ace.y) {
					p_out.yadd++;
				}

				if (p_in.get[ACTIVE]) {
					if (p_in.get[ON_PAD]) {
						int8_t m = (p_in.get[KEY_M_RIGHT]) ? 1 : 10;
						int8_t n = (p_in.get[KEY_M_RIGHT]) ? 1 : 4;
						p_out.xr = std::clamp<int>(p_out.xr + ((abs(p_in.m_y / 100) <= m) ? (p_in.m_y / 100) / n : std::clamp<int>((p_in.m_y / 100) / m, -m, m)) * 1000, -40 * 1000, 40 * 1000);
						p_out.yr -= ((abs(p_in.m_x / 100) <= m) ? (p_in.m_x / 100) / n : std::clamp<int>((p_in.m_x / 100) / m, -m, m)) * 1000;
					}
					else {
						int m_x, m_y;
						GetMousePoint(&m_x, &m_y);
						mouse_event(MOUSE_MOVED | MOUSEEVENTF_ABSOLUTE, 65536 / 2, 65536 / 2, 0, 0);
						if (abs(m_y - dispy / 2) <= 5) {
							p_out.xr = std::clamp<int>(p_out.xr + (m_y - dispy / 2) / 2 * 1000, -40 * 1000, 40 * 1000);
						}
						else {
							p_out.xr = std::clamp<int>(p_out.xr + std::clamp<int>((m_y - dispy / 2) / 5, -5, 5) * 1000, -40 * 1000, 40 * 1000);
						}

						if (abs(m_x - dispx / 2) <= 5) {
							p_out.yr -= (m_x - dispx / 2) / 2 * 1000;
						}
						else {
							p_out.yr -= std::clamp<int>((m_x - dispx / 2) / 5, -5, 5) * 1000;
						}
					}
				}


				p_out.xradd = -(p_out.xr - p_out.xo) / 1000;
				p_out.yradd = (p_out.yr - p_out.yo) / 1000;
				p_out.xo = p_out.xr;
				p_out.yo = p_out.yr;

				if (p_in.get[KEY_M_LEFT] && !p_out.shootf && p_out.ammoc > 0 && !p_out.reloadf) {
					p_out.rr = GetRand(90);
					p_out.st = 3 + GetRand(7);
					p_out.rtt[0].resize(p_out.st);
					for (auto& r : p_out.rtt[0])
						r = 50 + GetRand(150);
					p_out.rtt[1].resize(p_out.st);
					for (auto& r : p_out.rtt[1])
						r = 30 + GetRand(90);

					if (p_in.get[KEY_M_RIGHT]) {
						p_out.ct.x = (p_out.pos.x) - sin_i(p_out.yr, 1000, cos_i(p_out.xr, 1000, 100));
						p_out.ct.y = (p_out.pos.y + 400 - 50) - sin_i(p_out.xr, 1000, 100);
						p_out.ct.z = (p_out.pos.z) - cos_i(p_out.yr, 1000, cos_i(p_out.xr, 1000, 100));
					}
					else {
						p_out.ct.x = (p_out.pos.x + cos_i(p_out.yr, 1000, 100) - sin_i(p_out.yr, 1000, cos_i(p_out.xr, 1000, 300))) - sin_i(p_out.yr, 1000, cos_i(p_out.xr, 1000, 100));
						p_out.ct.y = (p_out.pos.y + 400 - 100 - sin_i(p_out.xr, 1000, 300)) - sin_i(p_out.xr, 1000, 100);
						p_out.ct.z = (p_out.pos.z - sin_i(p_out.yr, 1000, 100) - cos_i(p_out.yr, 1000, cos_i(p_out.xr, 1000, 300))) - cos_i(p_out.yr, 1000, cos_i(p_out.xr, 1000, 100));
					}
					/*
					p_out.ct.x = campos.x - int(100.f*cos(deg2rad(p_out.xr))*sin(deg2rad(p_out.yr)));
					p_out.ct.y = campos.y - int(100.f*sin(deg2rad(p_out.xr)));
					p_out.ct.z = campos.z - int(100.f*cos(deg2rad(p_out.xr))*cos(deg2rad(p_out.yr)));
					*/


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
						p_out.gun[p_out.gunc].endpos.x = p_out.ct.x - sin_i(p_out.yr + yrn, 1000, cos_i(p_out.xr + xrn, 1000, 500));
						p_out.gun[p_out.gunc].endpos.y = p_out.ct.y - sin_i(p_out.xr + xrn, 1000, 500);
						p_out.gun[p_out.gunc].endpos.z = p_out.ct.z - cos_i(p_out.yr + yrn, 1000, cos_i(p_out.xr + xrn, 1000, 500));
						p_out.gun[p_out.gunc].gunflug = true;
					}
					p_out.gunc++;
					p_out.gunc %= guns;
					p_out.ammoc--;

					p_out.shootf = true;
				}
				if (p_out.shootf) {
					p_out.shootc++;
					if (p_out.shootc > 6) {
						p_out.shootc = 0;
						p_out.shootf = false;
					}
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

						for (auto& e : p_out.enemy) {
							for (auto i = 0; i < 2; i++) {
								if (SSML(e.footframe[i][0], e.footframe[i][1], g.startpos, g.endpos) < 200) { e.hitp[i][0]--; }
								if (SSML(e.footframe[i][2], e.footframe[i][1], g.startpos, g.endpos) < 200) { e.hitp[i][1]--; }
								if (SSML(e.footframe[i][3], e.footframe[i][1], g.startpos, g.endpos) < 200) { e.hitp[i][2]--; }
								if (SSML(e.footframe[i][3], e.footframe[i][4], g.startpos, g.endpos) < 200) { e.hitp[i][3]--; }
								if (SSML(e.footframe[i][3], e.body, g.startpos, g.endpos) < 200) { e.hitp[i][4]--; }
								if (SSML(e.footframe[i][5], e.body, g.startpos, g.endpos) < 200) { e.hitp[i][5]--; }
							}
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



				if (p_in.get[KEY_NO_3] && !p_out.reloadf && (p_out.ammoc != p_out.ammoall)) {//リロード
					p_out.reloadf = true;
				}
				if (p_out.reloadf) {
					p_out.reloadc++;
					if (p_out.reloadc > p_out.reloadall) {
						p_out.ammoc = p_out.ammoall;
						p_out.reloadc = 0;
						p_out.reloadf = false;
					}
				}


				for (auto& e : p_out.enemy) {
					{
						auto radd = int(rad2deg(float(atan2<int, int>(e.pos.x - p_out.pos.x, p_out.pos.z - e.pos.z)) - deg2rad(e.rad)));
						if (radd < 0) { e.rad--; }
						else if (radd > 0) { e.rad++; }
						if (CalcDist(e.pos.x - p_out.pos.x, e.pos.z - p_out.pos.z) >= 2000) {
							e.fbspeed = 10;
							e.sidespeed = 0;
						}
						else {
							e.fbspeed = -5;
							e.sidespeed = 10;
						}

						for (auto& t : p_out.enemy) {
							if (t.id != e.id && CalcDist(t.pos.x - e.pos.x, t.pos.z - e.pos.z) < 2000) {
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

							auto distd = CalcDist(r.mpos.x - e.pos.x, r.mpos.z - e.pos.z);
							if (distd <= 700) {
								auto yr = int(rad2deg(atan2f(float(e.pos.x - r.mpos.x), float(e.pos.z - r.mpos.z))));
								e.pos.x += sin_i(yr, 1, 700 - distd);
								e.pos.z += cos_i(yr, 1, 700 - distd);


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



						e.pos.x += cos_i(e.rad, 1, e.sidespeed) - sin_i(e.rad, 1, e.fbspeed);
						e.pos.y = 0;
						e.pos.z += cos_i(e.rad, 1, e.fbspeed) + sin_i(e.rad, 1, e.sidespeed);

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
									e.pos.x + cos_i(e.rad,1, e.frame[i / 6][i % 6].x) - sin_i(e.rad,1, e.frame[i / 6][i % 6].z),
									e.pos.y + e.frame[i / 6][i % 6].y,
									e.pos.z + cos_i(e.rad,1, e.frame[i / 6][i % 6].z) + sin_i(e.rad,1, e.frame[i / 6][i % 6].x) };
							}
						}

						auto radd = int(rad2deg(float(atan2<int, int>(e.pos.x - p_out.pos.x, p_out.pos.z - e.pos.z)) - deg2rad(e.rad)));
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
									e.pos.x + cos_i(e.rad + e.radr,1, e.frame[i / 6][i % 6].x) - sin_i(e.rad + e.radr,1, e.frame[i / 6][i % 6].z),
									e.pos.y + e.frame[i / 6][i % 6].y,
									e.pos.z + cos_i(e.rad + e.radr,1, e.frame[i / 6][i % 6].z) + sin_i(e.rad + e.radr,1, e.frame[i / 6][i % 6].x) };
							}
						}
						e.body = {
							e.pos.x + cos_i(e.rad + e.radr,1, e.bodyframe.x) - sin_i(e.rad + e.radr,1, e.bodyframe.z),
							e.pos.y + e.bodyframe.y,
							e.pos.z + cos_i(e.rad + e.radr,1, e.bodyframe.z) + sin_i(e.rad + e.radr,1, e.bodyframe.x) };

						if ((radd > -90 && radd < 90) && !e.shootf) {

							if (!e.gun[e.gunc].gunflug) {
								int xrn = 0;
								int yrn = 0;
								xrn = -2 + GetRand(2 * 2);
								yrn = -5 + GetRand(10 * 2);


								auto xr = int(rad2deg(atan2f(float(e.body.y - (p_out.pos.y + 200)), float(CalcDist(e.body.x - p_out.pos.x, e.body.z - p_out.pos.z)))));
								e.gun[e.gunc].startpos = e.body;
								e.gun[e.gunc].endpos.x = e.gun[e.gunc].startpos.x - sin_i(180 - (e.rad + e.radr + yrn), 1, cos_i(xr + xrn, 1, 200));
								e.gun[e.gunc].endpos.y = e.gun[e.gunc].startpos.y - sin_i(xr + xrn, 1, 200);
								e.gun[e.gunc].endpos.z = e.gun[e.gunc].startpos.z - cos_i(180 - (e.rad + e.radr + yrn), 1, cos_i(xr + xrn, 1, 200));
								e.gun[e.gunc].gunflug = true;
							}
							e.gunc++;
							e.gunc %= guns;
							e.shootf = true;
						}
						if (e.shootf) {
							e.shootc++;
							if (e.shootc > 12) {
								e.shootc = 0;
								e.shootf = false;
							}
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

							if (((p_out.pos.x - g.endpos.x)*(p_out.pos.x - g.endpos.x) + (p_out.pos.y + 400 - g.endpos.y)*(p_out.pos.y + 400 - g.endpos.y) + (p_out.pos.z - g.endpos.z)*(p_out.pos.z - g.endpos.z)) < 25 * 25) {
								g.guncnt = 0;
								g.gunflug = false;
								g.hitpos = g.endpos;
								g.hitflug = true;

								p_out.hitp -= 3;
							}
							// + (p_out.pos.y - g.endpos.y)*(p_out.pos.y - g.endpos.y)
							if ((((p_out.pos.x - g.endpos.x)*(p_out.pos.x - g.endpos.x) + (p_out.pos.z - g.endpos.z)*(p_out.pos.z - g.endpos.z)) < 75 * 75) &&
								((p_out.pos.y - g.endpos.y > -375) && (p_out.pos.y - g.endpos.y <= 0))) {
								g.guncnt = 0;
								g.gunflug = false;
								g.hitpos = g.endpos;
								g.hitflug = true;

								p_out.hitp--;
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

				if (p_out.hitp == 0) {
					p_out.hitp = 20;
					p_out.respawn = true;
				}


				p_out.time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count();

				while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() < 1000000 / 60) {}
			}
			p_out.enemy.clear();
			p_in.rcon.clear();
		} while (!p_out.ends);
		return;
	}
public:
	ThreadClass() {
	}
	~ThreadClass() {
		if (thread_1.joinable())
			thread_1.detach();
	}
	void thread_start(input& p_in, output& p_out) {
		thread_1 = std::thread([&] { calc(p_in, p_out); });
	}
	void thead_stop() {
		thread_1.detach();
	}
};
