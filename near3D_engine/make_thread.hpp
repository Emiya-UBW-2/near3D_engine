#pragma once
#ifndef INCLUDED_thd_h_
#define INCLUDED_thd_h_

#include "sub.hpp"
#include "useful.hpp"
#include <thread> 
#include <chrono>
#include <array> 
//入力
struct input {
	std::array<bool, 13> get; /*キー用(一時監視)*/
};
struct output {
	int xp = 0, yp = 0, zp = 0;
	int xa = 0, ya = 0, za = 0;

	bool ends{ false }; /*終了フラグ*/
	int x,y,z;
	int zadd;
	bool jf{ false };
	int xr = 0, yr = 180;
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