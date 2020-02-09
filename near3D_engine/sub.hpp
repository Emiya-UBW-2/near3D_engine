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
struct pos2D{
	float x;
	float y;
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

class Draw{
private:
public:
	Draw();
	~Draw();
	void draw_wall(int UorL, float sx, float sy, int px, int py,int size, float high, int graphhandle = -1);

	void drw_rect(float sx, float sy, int px, int py, int size, float high, int graphhandle = -1);//��
	void drw_prism(int UorL, float sx, float sy, int px, int py, int size, float high, int graphhandle = -1);//�O�p��
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
