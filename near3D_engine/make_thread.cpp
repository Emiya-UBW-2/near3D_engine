#include "make_thread.hpp"

ThreadClass::ThreadClass() {
}
ThreadClass::~ThreadClass() {
}

void ThreadClass::thread_start(input& p_in, output& p_out) {
	thread_1 = std::thread([&] { calc(p_in, p_out); });
}

void ThreadClass::thead_stop() {
	thread_1.detach();
}

void ThreadClass::calc(input& p_in, output& p_out) {
	while (true) {
		auto start = std::chrono::system_clock::now(); // 計測スタート時刻を保存

		p_out.ends = p_in.get[0];

		if (p_in.get[KEY_UP])
			p_out.y += 2;
		if (p_in.get[KEY_DOWN])
			p_out.y-= 2;
		if (p_in.get[KEY_LEFT])
			p_out.x+= 2;
		if (p_in.get[KEY_RIGHT])
			p_out.x-= 2;

		while (true) {
			if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() >= 1000000 / 60)
				break;
		}
	}
}
