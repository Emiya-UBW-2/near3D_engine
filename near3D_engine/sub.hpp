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
		pos3D dist;
		std::array<pos3D,4> pos;
		int graphhandle;
	};
	std::vector<con> zcon;

	pos3D campos,camvec;
	float fov;
public:
	Draw_fps();
	~Draw_fps();

	void set_cam(pos3D cams, pos3D vecs, int fovs);

	void draw_dot(int sx, int sy, int sz);
	void draw_line(int sx, int sy, int sz , int ex, int ey, int ez);

	void draw_boad(int sx, int sy, int sz, int graphhandle = -1);//一辺
	void drw_rect(int sx, int sy, int sz, int graphhandle = -1);//柱
	/*zソート対応*/
	void set_drw_rect(int sx, int sy, int sz, int graphhandle = -1);//柱
	void put_drw(void);
	/**/
	inline pos3D getsub(pos3D pos1, pos3D pos2) {
		pos3D pos3 = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };
		return pos3;
	}

	inline int getdist(pos3D pos1) {
		return int(sqrt<int>((pos1.x)*(pos1.x) + (pos1.y)*(pos1.y) + (pos1.z)*(pos1.z)));
	}
	inline int getdist(pos3D pos1, pos3D pos2) {
		return int(sqrt<int>((pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y) + (pos1.z - pos2.z)*(pos1.z - pos2.z)));
	}

	inline pos3D getcross(pos3D pos1, pos3D pos2) {
		pos3D p = { pos1.y*pos2.z - pos1.z*pos2.y,pos1.z*pos2.x - pos1.x*pos2.z,pos1.x*pos2.y - pos1.y*pos2.x };
		return p;
	}

	inline float getdot(pos3D pos1, pos3D pos2) {
		return float((pos1.x)*(pos2.x) + (pos1.y)*(pos2.y) + (pos1.z)*(pos2.z)) / float(getdist(pos1) * getdist(pos2));
	}

	inline pos3D getsin(pos3D pos1) {
		return getcross(getsub(pos1, campos), getsub(camvec, campos));
	}
	inline float getcos(pos3D pos1) {
		return getdot(getsub(pos1, campos), getsub(camvec, campos));
	}

	inline float getsin_x(pos3D pos1) {
		pos3D up = { 0,1,0 };
		const auto sub1 = getsub(pos1, campos);
		const auto sub = getsub(camvec, campos);

		const auto subu = getcross(getcross(up, sub), sub);
		const auto sub2 = getcross(sub1, sub);

		const auto dst = getdist(sub2);
		//return getdot(sub2, subu);
		return float(sub2.y)/ float(sqrt<int>((sub1.x)*(sub1.x) + (sub1.z)*(sub1.z)) * sqrt<int>((sub.x)*(sub.x) + (sub.z)*(sub.z)));

	}
	inline float getsin_y(pos3D pos1) {
		const auto sub1 = getsub(pos1, campos);
		const auto sub = getsub(camvec, campos);

		//if(getcos(pos1) > (cos(fov / 2.f)*dispy / dispx))
		//	return sin(fov / 2.f);

		return float(float(sqrt<int>((sub1.x)*(sub1.x) + (sub1.z)*(sub1.z)))*sub.y - sub1.y*float(sqrt<int>((sub.x)*(sub.x) + (sub.z)*(sub.z)))) / float(getdist(sub1) * getdist(sub));
	}
	inline pos3D getpos(pos3D pos) {
		const auto rdn_x = getsin_x(pos);
		const auto rdn_y = getsin_y(pos);
		const auto rdn_z = getcos(pos);

		pos3D p;
		p.x = dispx / 2 + int(float(dispx/2) * (rdn_x)/ sin(fov / 2.f));
		p.y = dispy / 2 + int(float(dispx/2) * (rdn_y)/ (sin(fov / 2.f)*dispy / dispx));
		if(abs(rdn_y)> (sin(fov / 2.f)*dispy / dispx) || abs(rdn_x) > sin(fov / 2.f))
			p.z = -1;
		else
			p.z = int(rdn_z*1000.f);
		return p;
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
