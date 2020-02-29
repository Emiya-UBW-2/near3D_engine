#pragma once
#ifndef INCLUDED_thd_h_
#define INCLUDED_thd_h_

#include "sub.hpp"
#include "useful.hpp"
#include <thread> 
#include <chrono>
#include <array> 
//����
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
	std::array<bool, 13> get; /*�L�[�p(�ꎞ�Ď�)*/

	int m_x, m_y;

	struct enemiesframe {
		int time;
		std::array<MainClass::pos3D, 6> frame[2];
		MainClass::pos3D bodyframe;
	};
	std::vector<enemiesframe> enemyframe[2];//�A�j���[�V����

	struct rectcon {
		std::array<MainClass::pos3D, 2> pos;
		MainClass::pos3D mpos;
	};
	std::vector<rectcon> rcon;//��
};
struct output {
	struct gunes {
		bool hitflug;
		unsigned int hitcnt;
		bool gunflug;
		unsigned int guncnt;
		MainClass::pos3D startpos;
		MainClass::pos3D endpos;
		MainClass::pos3D hitpos;
	};


	bool ends{ false }; /*�I���t���O*/
	LONGLONG time;/*��������*/

	MainClass::pos3D pos;//���W
	MainClass::pos3D ace;//�����x
	MainClass::pos3D inp;//����

	int xadd, yadd;//�u��
	int xradd, yradd;//�u��

	bool jf{ false };//�W�����v�t���O
	uint8_t jump{ 0 };//�W�����v�񐔃J�E���^�[

	int xr = 0, yr = 180;//���_��]�p
	int xo = 0, yo = 180;//���_��]�p

	uint8_t hitp = 20;//�̗�

	bool shootf{ false };//�ˌ��t���O
	int shootc = 0;//�ˌ��J�E���g
	int ammoc;//�e��
	int ammoall;//���e��

	std::vector<gunes> gun;//�e���
	size_t gunc = 0;//�g�p�e�J�E���^

	//���C��
	int rr;
	int st;
	std::vector<int> rtt[2];
	MainClass::pos3D ct = { 0,0,0 };

	struct enemies {
		size_t id;
		MainClass::pos3D pos;
		int rad;
		int radr;

		int biimrad;

		int fbspeed;
		int sidespeed;
		int chose;
		int oldchose;

		int animetime = 0;
		int cf = 0;

		std::array<MainClass::pos3D, 6> frame[2];
		MainClass::pos3D bodyframe;
		std::vector<gunes> gun;
		int gunc = 0;
		bool shootf{ false };
		int shootc = 0;

		std::array<MainClass::pos3D, 6> footframe[2];
		MainClass::pos3D body;
	};

	std::vector<enemies> enemy;
};
//60fps���ێ�����������Z(box2D����)
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