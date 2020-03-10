#pragma once
#ifndef INCLUDED_thd_h_
#define INCLUDED_thd_h_

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
	};

	std::vector<enemies> enemy;
};
//60fpsを維持しつつ操作を演算(box2D込み)
class ThreadClass {
private:
	std::thread thread_1;
	void calc(input& p_in, output& p_out);
public:
	ThreadClass();
	~ThreadClass();
	void thread_start(input& p_in, output& p_out);
	void thead_stop();
};

#endif 