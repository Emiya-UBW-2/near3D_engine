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
	int xo, yo;
	int xt, yt;
	while (true) {
		auto start = std::chrono::system_clock::now(); // 計測スタート時刻を保存

		p_out.ends = p_in.get[0];


		if (p_in.get[KEY_UP]) {
			p_out.y = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? -15 : -45) : ((p_in.get[KEY_M_RIGHT]) ? - 10 : -25);
		}
		if (p_in.get[KEY_DOWN]) {
			p_out.y = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? 15 : 45) : ((p_in.get[KEY_M_RIGHT]) ? 10 : 25);
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
			p_out.x = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? -15 : -45) : ((p_in.get[KEY_M_RIGHT]) ? -10 : -25);
		if (p_in.get[KEY_RIGHT])
			p_out.x = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? 15 : 45) : ((p_in.get[KEY_M_RIGHT]) ? 10 : 25);
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



		if (p_in.get[KEY_NO_2] && !p_out.jf) {
			p_out.ya = (p_in.get[KEY_NO_1]) ? ((p_in.get[KEY_M_RIGHT]) ? 25 : 45) : ((p_in.get[KEY_M_RIGHT]) ? 15 : 35);
			p_out.jf = true;
		}
		if (p_out.jf) {
			p_out.z += p_out.ya;
			p_out.ya--;
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

		xt = int(float(p_out.xa)*cos(deg2rad(p_out.yr)) + float(p_out.za)*sin(deg2rad(-p_out.yr)));
		if (p_out.xadd > xt) {
			p_out.xadd--;
		}
		else if (p_out.xadd < xt) {
			p_out.xadd++;
		}
		//p_out.xadd = xt;

		yt = p_out.ya;
		if (p_out.yadd > yt) {
			p_out.yadd--;
		}
		else if (p_out.yadd < yt) {
			p_out.yadd++;
		}
		//p_out.yadd = yt;

		GetCursorPos(&mm);
		mouse_event(MOUSE_MOVED | MOUSEEVENTF_ABSOLUTE, 65536 / 2, 65536 / 2, 0, 0);

		xo = p_out.xr;
		yo = p_out.yr;
		if (abs(mm.y - dispy / 2) <= 5) {
			p_out.xr = std::clamp<int>(
				p_out.xr + (mm.y - dispy / 2) / 2
				, -45, 45);
		}
		else {
			p_out.xr = std::clamp<int>(
				p_out.xr + std::clamp<int>((mm.y - dispy / 2) / 5,-5,5)
				, -45, 45);
		}
		if (abs(mm.x - dispx / 2) <= 5) {
			p_out.yr -= (mm.x - dispx / 2) / 2;
		}
		else {
			p_out.yr -= std::clamp<int>((mm.x - dispx / 2) / 5, -5, 5);
		}

		p_out.xradd = -(p_out.xr - xo);
		p_out.yradd = (p_out.yr - yo);


		if (p_in.get[KEY_M_LEFT] && p_out.shootf==0) {
			p_out.shootf = 1;
		}
		if (p_out.shootf != 0) {
			p_out.shootc++;
			if (p_out.shootc > 6) {
				p_out.shootc = 0;
				p_out.shootf = 0;
			}
		}








		while (true) {
			if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() >= 1000000 / 60)
				break;
		}
	}
}
