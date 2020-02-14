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


		if (p_in.get[KEY_UP]) {
			p_out.y = (p_in.get[KEY_NO_2]) ? -50 : -20;
		}
		if (p_in.get[KEY_DOWN]) {
			p_out.y = (p_in.get[KEY_NO_2]) ? 50 : 20;
		}
		if (!p_in.get[KEY_DOWN] && !p_in.get[KEY_UP]) {
			if (p_out.y > 0) {
				p_out.y--;
				if (p_out.y <= 0)
					p_out.y = 0;
			}
			if (p_out.y < 0) {
				p_out.y++;
				if (p_out.y >= 0)
					p_out.y = 0;
			}
		}

		if (p_in.get[KEY_LEFT])
			p_out.x = (p_in.get[KEY_NO_2]) ? -50 : -20;
		if (p_in.get[KEY_RIGHT])
			p_out.x = (p_in.get[KEY_NO_2]) ? 50 : 20;
		if (!p_in.get[KEY_LEFT] && !p_in.get[KEY_RIGHT]) {
			if (p_out.x > 0) {
				p_out.x--;
				if (p_out.x <= 0)
					p_out.x = 0;
			}
			if (p_out.x < 0) {
				p_out.x++;
				if (p_out.x >= 0)
					p_out.x = 0;
			}
		}



		if (p_in.get[KEY_NO_1] && !p_out.jf) {
			p_out.zadd = 25;
			p_out.jf = true;
		}
		if (p_out.jf) {
			p_out.z += p_out.zadd;
			p_out.zadd--;
			if (p_out.z <= 0) {
				p_out.z = 0;
				p_out.jf = false;
			}
		}

		while (true) {
			if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() >= 1000000 / 60)
				break;
		}
	}
}
