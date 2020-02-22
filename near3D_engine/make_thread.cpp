#include "make_thread.hpp"

ThreadClass::ThreadClass() {
}
ThreadClass::~ThreadClass() {
	if(thread_1.joinable())
		thread_1.detach();
}

void ThreadClass::thread_start(input& p_in, output& p_out) {
	thread_1 = std::thread([&] { calc(p_in, p_out); });
}

void ThreadClass::thead_stop() {
	thread_1.detach();
}

void ThreadClass::calc(input& p_in, output& p_out) {
	POINT mm;
	while (true) {
		auto start = std::chrono::system_clock::now(); // 計測スタート時刻を保存

		p_out.ends = p_in.get[0];


		if (p_in.get[KEY_UP]) {
			p_out.y = (p_in.get[KEY_NO_2]) ? -50 : -25;
		}
		if (p_in.get[KEY_DOWN]) {
			p_out.y = (p_in.get[KEY_NO_2]) ? 50 : 25;
		}
		if (!p_in.get[KEY_DOWN] && !p_in.get[KEY_UP] && !p_out.jf) {
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
			p_out.x = (p_in.get[KEY_NO_2]) ? -50 : -25;
		if (p_in.get[KEY_RIGHT])
			p_out.x = (p_in.get[KEY_NO_2]) ? 50 : 25;
		if (!p_in.get[KEY_LEFT] && !p_in.get[KEY_RIGHT] && !p_out.jf) {
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
			p_out.zadd = (p_in.get[KEY_NO_2]) ? 35 : 25;
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


		if (!p_out.jf) {
			p_out.xa = int(float(p_out.x)*cos(deg2rad(p_out.yr)) + float(p_out.y)*sin(deg2rad(p_out.yr)));
			p_out.za = int(-float(p_out.x)*sin(deg2rad(p_out.yr)) + float(p_out.y)*cos(deg2rad(p_out.yr)));
		}
		p_out.xp += p_out.xa;
		p_out.yp += p_out.za;

		GetCursorPos(&mm);
		mouse_event(MOUSE_MOVED | MOUSEEVENTF_ABSOLUTE, 65536 / 2, 65536 / 2, 0, 0);
		if (abs(mm.y - dispy / 2) <= 5) {
			p_out.xr = std::clamp<int>(p_out.xr + (mm.y - dispy / 2) / 2, -45, 45);
		}
		else {
			p_out.xr = std::clamp<int>(p_out.xr + (mm.y - dispy / 2) / 5, -45, 45);
		}
		if (abs(mm.x - dispx / 2) <= 5) {
			p_out.yr -= (mm.x - dispx / 2) / 2;
		}
		else {
			p_out.yr -= (mm.x - dispx / 2) / 5;
		}


		while (true) {
			if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() >= 1000000 / 60)
				break;
		}
	}
}
