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
	KEY_NO_4 = 7,
	KEY_NO_5 = 8,
	KEY_UP = 9,
	KEY_DOWN = 10,
	KEY_LEFT = 11,
	KEY_RIGHT = 12
};
struct input {
	std::array<bool, 13> get; /*キー用(一時監視)*/
};
struct output {
	int xp = 0, yp = 0, zp = 0;
	int xa = 0, ya = 0, za = 0;

	int xadd, yadd;

	bool ends{ false }; /*終了フラグ*/
	int x,y,z;
	bool jf{ false };
	int xr = 0, yr = 180;
	int xradd = 0, yradd = 0;

	uint8_t shootf=0;
	int shootc = 0;
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