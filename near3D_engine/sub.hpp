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

constexpr size_t ammoc = 32; /*�C�e�m�ې�*/
constexpr size_t waypc = 4;  /*�ړ��m�ې�*/
#define TEAM 1		     /*����ID*/
#define ENEMY 2		     /*�GID*/
constexpr auto divi = 2;     /*�l�̕�������*/
constexpr auto EXTEND = 4;   /*�u���[���p*/

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
	bool USE_YSync;	       /*��������*/
	float frate;	       /*fps*/
	bool USE_windowmode;   /*�E�B���h�Eor�S���*/
	float se_vol;	       /**/
	float bgm_vol;	       /**/
	/**/		       /**/
public:
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

class Draw_lookdown{
private:
	struct pos2D {
		int x;
		int y;
	};
	struct con {
		pos2D dist1;
		pos2D dist2;
		int use;// rect = -1 else prism = 0~3
		int sx;
		int sy;
		int px;
		int py;
		int size;
		int hight;
		int graphhandle;
	};
	std::vector<con> zcon;

	int camhigh = 128;

public:
	Draw_lookdown();
	~Draw_lookdown();
	void draw_wall(int UorL, int sx, int sy, int px, int py,int size, int hight, int graphhandle = -1);//���
	void drw_rect(int sx, int sy, int px, int py, int size, int hight, int graphhandle = -1);//��
	void drw_prism(int UorL, int sx, int sy, int px, int py, int size, int hight, int graphhandle = -1);//�O�p��
	/*z�\�[�g�Ή�*/
	void set_drw_rect(int sx, int sy, int px, int py, int size, int hight, int graphhandle = -1);//��
	void set_drw_prism(int UorL, int sx, int sy, int px, int py, int size, int hight, int graphhandle = -1);//�O�p��
	void put_drw(void);
	/**/
	inline pos2D getpos(int xpos, int ypos, int high, int camhigh) {
		pos2D p;
		p.x = dispx / 2 + int(float(camhigh) * tanf(atan2f(xpos - float(dispx / 2), float(camhigh - high))));
		p.y = dispx / 2 + int(float(camhigh) * tanf(atan2f(ypos - float(dispy / 2), float(camhigh - high))));
		return p;
	}
};

class Draw_fps {
private:
	struct pos3D {
		int x;
		int y;
		int z;
	};
	struct con {
		pos3D dist;
		std::array<pos3D,4> pos;
		int graphhandle;
	};
	std::vector<con> zcon;

	int camhigh = 128;

public:
	Draw_fps();
	~Draw_fps();

	void draw_dot(int sx, int sy, int sz);

	void draw_boad(int sx, int sy, int sz, int graphhandle = -1);//���
	void drw_rect(int sx, int sy, int sz, int graphhandle = -1);//��
	/*z�\�[�g�Ή�*/
	void set_drw_rect(int sx, int sy, int px, int py, int size, int hight, int graphhandle = -1);//��
	void put_drw(void);
	/**/
	inline pos3D getpos(int xpos, int ypos, int high, int camhigh) {
		pos3D p;
		p.x = dispx / 2 + int(float(camhigh) * tanf(atan2f(xpos - float(dispx / 2), float(camhigh - high))));
		p.y = dispx / 2 + int(float(camhigh) * tanf(atan2f(ypos - float(dispy / 2), float(camhigh - high))));
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
