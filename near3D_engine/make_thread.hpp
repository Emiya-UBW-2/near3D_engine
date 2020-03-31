#pragma once
#ifndef INCLUDED_thd_h_
#define INCLUDED_thd_h_

#include "useful.hpp"
#include <thread> 
#include <chrono>

enum Key {
	KEY_M_LEFT,
	KEY_M_RIGHT,
	KEY_NO_1,
	KEY_NO_2,
	KEY_NO_3,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_ESCAPE,
	KEY_PAUSE,
	ACTIVE,
	ON_PAD,
};
//����
struct input {
	std::array<bool, 13> get; /*�L�[�p(�ꎞ�Ď�)*/
	int m_x, m_y;
};
//�o��
struct output {
	bool ends{ false }; /*�I���t���O*/
	int x,y;
};
//60fps���ێ�����������Z
class ThreadClass {
private:
	std::thread thread_1;
	void calc(input& p_in, output& p_out) {
		while (true) {
			auto start = std::chrono::system_clock::now(); // �v���X�^�[�g������ۑ�

			p_out.ends = p_in.get[KEY_ESCAPE];

			if (p_in.get[KEY_UP])
				p_out.y += 2;
			if (p_in.get[KEY_DOWN])
				p_out.y -= 2;
			if (p_in.get[KEY_LEFT])
				p_out.x += 2;
			if (p_in.get[KEY_RIGHT])
				p_out.x -= 2;

			while (true) {
				if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() >= 1000000 / 60)
					break;
			}
		}
	}
public:
	ThreadClass() {
	}
	~ThreadClass() {
		thead_stop();
	}
	void thread_start(input& p_in, output& p_out) {
		thread_1 = std::thread([&] { calc(p_in, p_out); });
	}
	void thead_stop() {
		if (thread_1.joinable()) {
			thread_1.detach();

		}
	}
};

#endif 