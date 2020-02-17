#pragma once

#ifndef INCLUDED_define_h_
#define INCLUDED_define_h_

#define NOMINMAX
#include "DxLib.h"
#include "useful.hpp"
#include "SoundHandle.hpp"
#include "GraphHandle.hpp"
#include "FontHandle.hpp"
#include <windows.h>
#include <fstream>
#include <string_view>
#include <optional>
#include <array>
#include <vector>
#include <utility>

constexpr size_t ammoc = 32; /*砲弾確保数*/
constexpr size_t waypc = 4;  /*移動確保数*/
#define TEAM 1		     /*味方ID*/
#define ENEMY 2		     /*敵ID*/
constexpr auto divi = 2;     /*人の物理処理*/
constexpr auto EXTEND = 4;   /*ブルーム用*/

enum Key {
	KEY_ESCAPE = 0,
	KEY_PAUSE = 1,
	KEY_M_LEFT = 2,
	KEY_M_RIGHT = 3,
	KEY_NO_1 = 4,
	KEY_NO_2 = 5,
	KEY_NO_3 = 6,
	KEY_NO_4 = 7,
	KEY_NO_5 = 8,
	KEY_UP = 9,
	KEY_DOWN = 10,
	KEY_LEFT = 11,
	KEY_RIGHT = 12
};
struct switches {
	bool flug{ false };
	uint8_t cnt{ 0 };
};
class MainClass {
private:
	/*setting*/
	bool USE_YSync;	       /*垂直同期*/
	float frate;	       /*fps*/
	bool USE_windowmode;   /*ウィンドウor全画面*/
	float se_vol;	       /**/
	float bgm_vol;	       /**/
	/**/		       /**/
public:
	struct pos2D {
		int x;
		int y;
	};
	struct pos3D {
		int x;
		int y;
		int z;
		int logs;
	};
	/*setting*/
	inline const auto get_frate(void) { return frate; }
	inline const auto get_se_vol(void) { return se_vol; }
	inline const auto get_bgm_vol(void) { return bgm_vol; }
	void write_setting(void);
	MainClass(void);
	~MainClass(void);
	/*draw*/
	void Screen_Flip(void);
	void Screen_Flip(LONGLONG waits);
};

class Draw_lookdown : MainClass{
private:
	struct con {
		pos2D dist1;
		pos2D dist2;
		int use;// rect = -1 else prism = 0~3
		int sx, sy;
		int px,py;
		int size;
		int hight;
		int graphhandle;
	};
	std::vector<std::vector<con>> zcon;

	const int siz = 40;//タイルの数
	const int camhigh = 128;//カメラの高さ
public:
	Draw_lookdown();
	~Draw_lookdown();
	void draw_wall(int UorL, con cont);//一辺
	void drw_rect(con cont);//柱
	void drw_prism(con cont);//三角柱
	/*zソート対応*/
	void set_drw_rect(int sx, int sy, int px, int py, int size, int hight, int graphhandle = -1);//柱
	void set_drw_prism(int UorL, int sx, int sy, int px, int py, int size, int hight, int graphhandle = -1);//三角柱
	void put_drw(void);
	/**/
	inline pos2D getpos(int xpos, int ypos, int high, int camhigh) {
		pos2D p;
		p.x = dispx / 2 + int(float(camhigh) * tanf(atan2f(xpos - float(dispx / 2), float(camhigh - high))));
		p.y = dispy / 2 + int(float(camhigh) * tanf(atan2f(ypos - float(dispy / 2), float(camhigh - high))));
		return p;
	}
};

class Draw_fps : MainClass {
private:
	struct con {
		std::array<pos3D,2> pos;
	};
	std::vector<con> wcon;
	std::vector<con> lcon;
	pos3D campos,camvec;
	int fov;
	int distance = 10000;//fog
	const int div1 = 100;//
	const int div2 = 20;//
public:
	Draw_fps();
	~Draw_fps();
	void set_cam(pos3D cams, pos3D vecs, int fovs);
	void draw_dot(int sx, int sy, int sz, bool hide = false);
	void draw_line(int sx, int sy, int sz , int ex, int ey, int ez);//陰線しない
	void draw_line(pos3D s, pos3D e);//陰線する
	void draw_triangle(int p1x, int p1y, int p1z, int p2x, int p2y, int p2z, int p3x, int p3y, int p3z);//壁
	void draw_triangle(pos3D p1, pos3D p2, pos3D p3);//壁
	void draw_wall(int sx, int sy, int sz, int ex, int ey, int ez);//壁
	void draw_wall(pos3D s, pos3D e);//壁
	void drw_rect(pos3D s, pos3D e);//柱
	//lconとwconに貯めた描画物を一気に描画する
	void set_drw_line(int sx, int sy, int sz, int ex, int ey, int ez);
	void set_drw_rect(int sx, int sy, int sz, int ex, int ey, int ez);//柱
	void put_drw(void);
	/**/
	inline pos3D getsub(pos3D pos1, pos3D pos2) {
		pos3D pos3 = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };
		return pos3;
	}
	inline int getdist(pos3D pos1) { return int(sqrt<int>((pos1.x)*(pos1.x) + (pos1.y)*(pos1.y) + (pos1.z)*(pos1.z))); }
	inline int getdist(pos3D pos1, pos3D pos2) { return getdist(getsub(pos1,pos2)); }
	inline pos3D getcross(pos3D pos1, pos3D pos2) {
		pos3D p = { pos1.y*pos2.z - pos1.z*pos2.y,pos1.z*pos2.x - pos1.x*pos2.z,pos1.x*pos2.y - pos1.y*pos2.x };
		return p;
	}
	inline float getdot(pos3D pos1, pos3D pos2) {
		return float((pos1.x)*(pos2.x) + (pos1.y)*(pos2.y) + (pos1.z)*(pos2.z)) / float(getdist(pos1) * getdist(pos2));
	}
	inline float getcos(pos3D pos1) {
		return getdot(getsub(pos1, campos), getsub(camvec, campos));
	}
	inline float getsin_x(pos3D pos1) {
		const auto sub1 = getsub(pos1, campos);
		const auto sub = getsub(camvec, campos);
		return float(getcross(sub1, sub).y)/ float(sqrt<int>((sub1.x)*(sub1.x) + (sub1.z)*(sub1.z)) * sqrt<int>((sub.x)*(sub.x) + (sub.z)*(sub.z)));
	}
	inline float getsin_y(pos3D pos1) {
		const auto sub1 = getsub(pos1, campos);
		const auto sub = getsub(camvec, campos);
		return float(getdot(sub1, sub)*sub.y) / getdist(sub) - float(sub1.y) / getdist(sub1);

	}
	inline pos3D getpos(pos3D pos) {
		const auto rdn_x = getsin_x(pos);
		const auto rdn_y = getsin_y(pos);
		const auto rdn_z = getcos(pos);

		pos3D p;
		p.x = dispx / 2 + int(float(dispx / 2) * (rdn_x) / sin(deg2rad(fov / 2)));
		p.y = dispy / 2 + int(float(dispx / 2) * (rdn_y) / (sin(deg2rad(fov / 2))*dispy / dispx));
		if(abs(rdn_y)> (sin(deg2rad(fov / 2))*dispy / dispx) || abs(rdn_x) > sin(deg2rad(fov / 2)))
			p.z = -1;
		else
			p.z = int(rdn_z*1000.f);
		return p;
	}
	inline pos3D getpos(int sx, int sy, int sz) {
		pos3D pos = { sx,sy,sz };
		return getpos(pos);
	}
	/*陰線処理*/
	struct hitcol{
		bool flag = false;
		int x, y;
	};
	inline hitcol hit_L2L(int xLS1, int yLS1, int xLE1, int yLE1, int xLS2, int yLS2, int xLE2, int yLE2) {
		hitcol h;
		int dBunbo = (xLE1 - xLS1) * (yLE2 - yLS2) - (yLE1 - yLS1) * (xLE2 - xLS2);
		if (dBunbo == 0) {	// 平行
			h.flag = false;
			return h;
		}

		int dR = ((yLE2 - yLS2) * (xLS2 - xLS1) - (xLE2 - xLS2) * (yLS2 - yLS1));//  / dBunbo
		int dS = ((yLE1 - yLS1) * (xLS2 - xLS1) - (xLE1 - xLS1) * (yLS2 - yLS1));//  / dBunbo
		if ((dR <= 0) || (dR >= dBunbo) || (dS <= 0) || (dS >= dBunbo)) {
			h.flag = false;
			return h;
		}
		h.flag = true;
		h.x = xLS1 + (xLE1 - xLS1)*dR / dBunbo;
		h.y = yLS1 + (yLE1 - yLS1)*dR / dBunbo;
		return h;
	}
	inline void gethit(pos3D p1, pos3D p2, pos3D& p3, pos3D& p4 , int& Lin) {
		const auto hit = hit_L2L(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
		if (hit.flag) {
			switch (Lin){
			case 0:
				p4.x = hit.x;
				p4.y = hit.y;
				break;
			case 2:
				p3.x = hit.x;
				p3.y = hit.y;
				break;
			default:
				break;
			}
		}
	}
	inline bool sq_in(pos3D b1, pos3D b2, pos3D b3, pos3D b4, pos3D point) {
		int cnt = 0;
		cnt += ((b2.x - b1.x) * (point.y - b1.y) - (point.x - b1.x) * (b2.y - b1.y) < 0) ? 1 : -1;
		cnt += ((b3.x - b2.x) * (point.y - b2.y) - (point.x - b2.x) * (b3.y - b2.y) < 0) ? 1 : -1;
		cnt += ((b4.x - b3.x) * (point.y - b3.y) - (point.x - b3.x) * (b4.y - b3.y) < 0) ? 1 : -1;
		cnt += ((b1.x - b4.x) * (point.y - b4.y) - (point.x - b4.x) * (b1.y - b4.y) < 0) ? 1 : -1;
		return (cnt == 4 || cnt == -4);
	}
	inline void gethit_wall(int sx, int sy, int sz, int ex, int ey, int ez, pos3D& p3, pos3D& p4, int& Lin) {
		const auto b1 = getpos(sx, sy, sz);//◤
		const auto b2 = getpos(ex, sy, ez);//◥
		const auto b4 = getpos(ex, ey, ez);//◢
		const auto b3 = getpos(sx, ey, sz);//◣
		const auto s_in = sq_in(b1, b2, b4, b3, p3);
		const auto e_in = sq_in(b1, b2, b4, b3, p4);
		//ここで視点と終点が壁内にあるかを見る
		if (Lin == -1) {
			if (!s_in && e_in) { Lin = 0; }
			if (s_in && !e_in) { Lin = 2; }
		}
		if (Lin == 1) {
			if (!s_in && e_in) { Lin = 0; }
			if (s_in && !e_in) { Lin = 2; }
		}
		if (s_in && e_in) { Lin = 1; }
		if (!s_in && !e_in) { Lin = -1; }
		//
		if (Lin == 0 || Lin == 2) {
			gethit(b1, b2, p3, p4, Lin);			//1
			gethit(b1, b3, p3, p4, Lin);			//2
			gethit(b2, b4, p3, p4, Lin);			//3
			gethit(b3, b4, p3, p4, Lin);			//4
		}
		//Lin = -1;
	}
	inline void gethit_rect(con w,pos3D& p3, pos3D& p4, int& Lin) {
		gethit_wall(w.pos[0].x, w.pos[0].y, w.pos[0].z, w.pos[0].x, w.pos[1].y, w.pos[1].z, p3, p4, Lin);//左
	//	gethit_wall(w.pos[1].x, w.pos[0].y, w.pos[0].z, w.pos[1].x, w.pos[1].y, w.pos[1].z, p3, p4, Lin);//右
	//	gethit_wall(w.pos[0].x, w.pos[0].y, w.pos[0].z, w.pos[1].x, w.pos[1].y, w.pos[0].z, p3, p4, Lin);//前
	//	gethit_wall(w.pos[0].x, w.pos[0].y, w.pos[1].z, w.pos[1].x, w.pos[1].y, w.pos[1].z, p3, p4, Lin);//後
	}
};


class UIS {
private:
	float deb[60][6 + 1];
	LONGLONG waypoint;
	float waydeb[6];
	size_t seldeb;
public:
	void put_way(void);
	void end_way(void);
	void debug(float fps, float time);
};

#endif
