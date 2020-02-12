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

class Draw{
private:
	struct pos2D {
		int x;
		int y;
	};
	struct con {
		pos2D dist;
		int use;// rect = -1 else prism = 0~3
		float sx;
		float sy;
		int px;
		int py;
		int size;
		int hight;
		int graphhandle;
	};
	std::vector<con> zcon;

	float xrad = deg2rad(60);
	int camhigh = 128;

public:
	Draw();
	~Draw();
	void draw_wall(int UorL, float sx, float sy, int px, int py,int size, int hight, int graphhandle = -1);

	void drw_rect(float sx, float sy, int px, int py, int size, int hight, int graphhandle = -1);//��
	void drw_prism(int UorL, float sx, float sy, int px, int py, int size, int hight, int graphhandle = -1);//�O�p��

	/*z�\�[�g�Ή�*/
	void set_drw_rect(float sx, float sy, int px, int py, int size, int hight, int graphhandle = -1);//��
	void set_drw_prism(int UorL, float sx, float sy, int px, int py, int size, int hight, int graphhandle = -1);//�O�p��
	void put_drw(void);

	inline pos2D getpos(float xpos, float ypos, int high, int camhigh, float xrad) {
		pos2D p;


		const auto yy = dispy / 2 + int(float(camhigh) * tanf(std::clamp<float>(atan2f(ypos - float(dispy / 2), float(camhigh - high)), deg2rad(-89), deg2rad(89))));

		p.x = int(
			float(dispx / 2) + (float(camhigh) *
				tanf(atan2f(xpos - float(dispx / 2), float(camhigh - high)))) +
				((xrad == 0.0) ? 0.0f : (std::hypotf(float(camhigh), float(high)) / float(camhigh)) / sin(xrad))
			*((float(yy) - float(dispy / 2) / float(dispy / 2)))
			*((xpos - float(dispx / 2)) / float(dispx / 2))
			);

		p.y = int(
			float(dispy / 2) + (float(camhigh) / cos(xrad)) *
			tanf(std::clamp<float>(xrad + atan2f(ypos - float(dispy / 2), float(camhigh - high)), deg2rad(-89), deg2rad(89)))
			);

		return p;

		return pos2D();
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
