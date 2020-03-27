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
#include <string_view>
#include <string>
#include <iostream>
#include <fstream>

class MainClass {
private:
	/*setting*/
	int refrate;
	bool USE_YSync;	       /*垂直同期*/
	int frate;	       /*fps*/
	int se_vol;	       /**/
	int bgm_vol;	       /**/
	bool use_pad;		/**/
public:
	/*setting*/
	inline const auto get_se_vol(void) { return se_vol; }
	inline const auto get_bgm_vol(void) { return bgm_vol; }
	inline const auto get_use_pad(void) { return use_pad; }
	bool write_setting(void);
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
		std::array<pos2D, 4> top;
		std::array<pos2D, 4> floor;
		std::array<pos2D, 4> zero;
		int use;// rect = -1 else prism = 0~3
		int px,py;
		int bottom;
		int hight;
		GraphHandle* wallhandle;
		GraphHandle* floorhandle;
	};
	struct Status {
		unsigned int xp, yp;
		int bottom, hight;
		int wall_id, floor_id;
		int dir;
	};
	std::vector<Status> ans;
	std::vector<std::vector<con>> zcon;
	int xpos, ypos;
	float light_yrad=0;
	int camhigh = 64;
	int tilesize = 32;
	std::vector<GraphHandle> walls;
	std::vector<GraphHandle> floors;
	int shadow_graph;
	struct Bonesdata {
		int parent;
		int xp, yp, zp;
		float xr, yr, zr;
		float xrad,yrad,zrad;
		float xdist,ydist,zdist;
		bool edit;
	};
	struct Animesdata {
		int time;
		std::vector<Bonesdata> bone;
	};
	struct Humans {
		std::vector<GraphHandle> Graphs;
		std::vector<Bonesdata> bone;
		std::vector<pair> sort;
		std::vector<std::vector<Animesdata>> anime;
		pos2D vtmp;
		pos2D vec;
		float yrad;
		pos2D pos;
		int animeframe;
		int animetime;
	};
	std::vector<Humans> human;
	size_t player_id;
public:
	Draw(int size){
		zcon.resize(40);
		for (auto& z : zcon)
			z.resize(40);
		tilesize = size;

		shadow_graph = MakeScreen(dispx, dispy, TRUE);
	}
	~Draw(){
		zcon.clear();
	}
	inline void set_map(int *player_x, int *player_y) {
		using namespace std::literals;
		unsigned int xp=0, yp=0;
		std::fstream file;
		struct maps {
			int plx, ply;
			char wall_name[64];
			char floor_name[64];
			float light_yrad;
		};

		/*
		//mapデータ1書き込み
		std::vector<Status> n;
		for (unsigned int y = 0; y < 40; y += 5) {
			for (unsigned int x = 0; x < 40; x += 5) {
				const int btm = 0;
				const int mid = 0;//128 * (x + y * 40) / (40 * 40);
				const int hig = 64;//256 * (x + y * 40) / (40 * 40);

				n.resize(n.size() + 1);
				n.back() = { x + 0, y + 0, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 1, y + 0, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 2, y + 0, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 3, y + 0, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 4, y + 0, btm, mid, 2, 2, -1 };

				n.resize(n.size() + 1);
				n.back() = { x + 0, y + 1, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 1, y + 1, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 2, y + 1, btm, hig, 2, 1, 0 };
				n.resize(n.size() + 1);
				n.back() = { x + 3, y + 1, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 4, y + 1, btm, mid, 2, 2, -1 };

				n.resize(n.size() + 1);
				n.back() = { x + 0, y + 2, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 1, y + 2, btm, hig, 2, 2, 1 };
				n.resize(n.size() + 1);
				n.back() = { x + 2, y + 2, btm, hig, 2, 1, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 3, y + 2, btm, hig, 2, 2, 3 };
				n.resize(n.size() + 1);
				n.back() = { x + 4, y + 2, btm, mid, 2, 2, -1 };

				n.resize(n.size() + 1);
				n.back() = { x + 0, y + 3, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 1, y + 3, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 2, y + 3, btm, hig, 2, 1, 2 };
				n.resize(n.size() + 1);
				n.back() = { x + 3, y + 3, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 4, y + 3, btm, mid, 2, 2, -1 };

				n.resize(n.size() + 1);
				n.back() = { x + 0, y + 4, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 1, y + 4, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 2, y + 4, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 3, y + 4, btm, mid, 2, 2, -1 };
				n.resize(n.size() + 1);
				n.back() = { x + 4, y + 4, btm, mid, 2, 2, -1 };
			}
		}
		file.open("data/Map/map1_1.dat", std::ios::binary | std::ios::out);
		for (auto& m : n) {
			file.write((char*)&m, sizeof(m));
		}
		file.close();
		//mapデータ2書き込み
		maps mapdata;
		mapdata.plx = 0;
		mapdata.ply = 0;
		mapdata.light_yrad = deg2rad(45);
		strcpy_s(mapdata.wall_name, "data/Chip/Wall/1.bmp");
		strcpy_s(mapdata.floor_name, "data/Chip/Floor/1.bmp");
		file.open("data/Map/map1_2.dat", std::ios::binary | std::ios::out);
		file.write((char*)&mapdata, sizeof(mapdata));
		file.close();
		//*/


		file.open("data/Map/map1_1.dat", std::ios::binary | std::ios::in);
		while (!file.eof()) {
			ans.resize(ans.size() + 1);
			file.read((char*)&ans.back(), sizeof(ans.back()));
			if (xp <= ans.back().xp) {
				xp = ans.back().xp;
			}
			if (yp <= ans.back().yp) {
				yp = ans.back().yp;
			}
		}
		file.close();
		ans.pop_back();

		zcon.resize(xp + 1);
		for (auto& z : zcon)
			z.resize(yp + 1);

		maps mapb;
		file.open("data/Map/map1_2.dat", std::ios::binary | std::ios::in);
		file.read((char*)&mapb, sizeof(mapb));
		file.close();
		*player_x = mapb.plx;
		*player_y = mapb.ply;
		light_yrad = mapb.light_yrad;
		GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, 16, 32, &walls);
		GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, 16, 16, &floors);

		set_human(&player_id, *player_x, *player_y);
		size_t idb;
		set_human(&idb, 32 * 5 + 16, 32 * 5 + 16);
		size_t idb2;
		set_human(&idb2, 32 * 10 + 16, 32 * 10 + 16);
	}
	inline void put_map(void) {
		for (auto& z : ans) {
			set_drw_rect(z.xp, z.yp, z.bottom, z.hight, z.wall_id, z.floor_id, z.dir);
		}
	}
	inline void exit_map(void) {
		walls.clear();
		floors.clear();
		human.clear();
		return;
	}
	inline pos2D getpos(int xp, int yp, int high) {
		pos2D p;
		p.x = dispx / 2 + (camhigh * (xp - dispx / 2)) * camhigh / std::max(-camhigh * (high - camhigh), 1);
		p.y = dispy / 2 + (camhigh * (yp - dispy / 2)) * camhigh / std::max(-camhigh * (high - camhigh), 1);
		return p;
	}
	inline void draw_wall(int UorL,con* z){//一辺
		
		const auto a00_1 = getpos(dispx / 2 + xpos + tilesize * (z->px + 0), dispy / 2 + ypos + tilesize * (z->py + 0), z->hight);
		const auto a10_1 = getpos(dispx / 2 + xpos + tilesize * (z->px + 1), dispy / 2 + ypos + tilesize * (z->py + 0), z->hight);
		const auto a01_1 = getpos(dispx / 2 + xpos + tilesize * (z->px + 0), dispy / 2 + ypos + tilesize * (z->py + 1), z->hight);
		const auto a11_1 = getpos(dispx / 2 + xpos + tilesize * (z->px + 1), dispy / 2 + ypos + tilesize * (z->py + 1), z->hight);

		const auto a00_0 = getpos(dispx / 2 + xpos + tilesize * (z->px + 0), dispy / 2 + ypos + tilesize * (z->py + 0), z->bottom);//◤
		const auto a10_0 = getpos(dispx / 2 + xpos + tilesize * (z->px + 1), dispy / 2 + ypos + tilesize * (z->py + 0), z->bottom);//◥
		const auto a01_0 = getpos(dispx / 2 + xpos + tilesize * (z->px + 0), dispy / 2 + ypos + tilesize * (z->py + 1), z->bottom);//◣
		const auto a11_0 = getpos(dispx / 2 + xpos + tilesize * (z->px + 1), dispy / 2 + ypos + tilesize * (z->py + 1), z->bottom);//◢
		if (UorL < 20) {
			if (UorL % 4 == 0) {
				int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(0))));
				SetDrawBright(c, c, c);
				switch (UorL) {
				case 0://縦(上)
					if (a00_0.y < a00_1.y && z->hight != z->bottom) {
						DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 4:
					if (a00_0.y < a01_1.y) {
						DrawModiGraph(a01_1.x, a01_1.y, a11_1.x, a11_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 8://上◢
					if (a10_0.y < a10_1.y && z->hight != z->bottom) {
						DrawModiGraph(a10_1.x, a10_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 16://上◣
					if (a00_0.y < a00_1.y && z->hight != z->bottom) {
						DrawModiGraph(a00_1.x, a00_1.y, a00_1.x, a00_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				}
			}
			if (UorL % 4 == 1) {
				int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(90))));
				SetDrawBright(c, c, c);
				switch (UorL) {
				case 1://横(左)
					if (a00_0.x < a00_1.x && z->hight != z->bottom) {
						DrawModiGraph(a01_1.x, a01_1.y, a00_1.x, a00_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 5://左◢
					if (a00_0.x < a00_1.x && z->hight != z->bottom) {
						DrawModiGraph(a01_1.x, a01_1.y, a01_1.x, a01_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 9:
					if (a00_0.x < a11_1.x) {
						DrawModiGraph(a11_1.x, a11_1.y, a10_1.x, a10_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 13://左◥
					if (a00_0.x < a00_1.x && z->hight != z->bottom) {
						DrawModiGraph(a00_1.x, a00_1.y, a00_1.x, a00_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				}
			}
			if (UorL % 4 == 2) {
				int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(180))));
				SetDrawBright(c, c, c);
				switch (UorL) {
				case 2://縦(下)
					if (a11_0.y >= a11_1.y && z->hight != z->bottom) {
						DrawModiGraph(a01_1.x, a01_1.y, a11_1.x, a11_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 10://下◢
					if (a11_0.y >= a11_1.y && z->hight != z->bottom) {
						DrawModiGraph(a11_1.x, a11_1.y, a11_1.x, a11_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 14:
					if (a01_0.y > a10_1.y) {
						DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 18://下◣
					if (a11_0.y >= a11_1.y && z->hight != z->bottom) {
						DrawModiGraph(a01_1.x, a01_1.y, a01_1.x, a01_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				}
			}
			if (UorL % 4 == 3) {
				int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(90))));
				SetDrawBright(c, c, c);
				switch (UorL) {
				case 3://横(右)
					if (a11_0.x >= a11_1.x && z->hight != z->bottom) {
						DrawModiGraph(a11_1.x, a11_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 7://右◢
					if (a11_0.x >= a11_1.x && z->hight != z->bottom) {
						DrawModiGraph(a11_1.x, a11_1.y, a11_1.x, a11_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 15://右◥
					if (a11_0.x >= a11_1.x && z->hight != z->bottom) {
						DrawModiGraph(a10_1.x, a10_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 19:
					if (a10_0.x > a01_1.x) {
						DrawModiGraph(a01_1.x, a01_1.y, a00_1.x, a00_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				}
			}
		}
		else {
			if (z->hight == 0) {
				SetDrawBright(255, 255, 255);
				DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_1.x, a11_1.y, a01_1.x, a01_1.y, z->floorhandle->get(), TRUE);
			}
			else if (z->hight >= camhigh) {
				SetDrawBright(0,0,0);
				DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_1.x, a11_1.y, a01_1.x, a01_1.y, z->floorhandle->get(), TRUE);
			}
			else {
				int c = 255 * (camhigh - z->hight) / camhigh;
				SetDrawBright(c, c, c);
				DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_1.x, a11_1.y, a01_1.x, a01_1.y, z->floorhandle->get(), TRUE);
			}
		}
		SetDrawBright(255, 255, 255);
	}
	inline void drw_rect(con* z){
		switch (z->use) {//三角柱
		case 0://上
			draw_wall(4, z);	//縦(上)//4
			draw_wall(5, z);	//横(左)
			draw_wall(2, z);	//縦(下)
			draw_wall(7, z);	//横(右)
			break;
		case 1://左
			draw_wall(8, z);	//縦(上)//8
			draw_wall(9, z);	//横(左)
			draw_wall(10, z);	//縦(下)
			draw_wall(3, z);	//横(右)
			break;
		case 2://下
			draw_wall(0, z);	//縦(上)//12
			draw_wall(13, z);	//横(左)
			draw_wall(14, z);	//縦(下)
			draw_wall(15, z);	//横(右)
			break;
		case 3://右
			draw_wall(16, z);	//縦(上)//16
			draw_wall(1, z);	//横(左)
			draw_wall(18, z);	//縦(下)
			draw_wall(19, z);	//横(右)
			break;
		default://柱
			draw_wall(0, z);	//縦(上)
			draw_wall(1, z);	//横(左)
			draw_wall(2, z);	//縦(下)
			draw_wall(3, z);	//横(右)
			draw_wall(20, z);	//天井
			break;
		}
	}
	inline void drw_human(con* z) {
		for (auto& pl : human) {
			for (auto& g : pl.sort) {
				auto q = getpos(dispx / 2 + pl.bone[g.first].xp + pl.pos.x, dispy / 2 + pl.bone[g.first].yp + pl.pos.y, 0);
				if (q.x >= z->zero[0].x && q.y >= z->zero[0].y && q.x <= z->zero[3].x && q.y <= z->zero[3].y) {
					auto p = getpos(dispx / 2 + pl.bone[g.first].xp + pl.pos.x, dispy / 2 + pl.bone[g.first].yp + pl.pos.y, pl.bone[g.first].zp - pl.sort[0].second);
					DrawRotaGraph3(p.x, p.y, 16, 16,
						
						1.0 * dispx / 1920 * (camhigh + (pl.bone[g.first].zp - pl.sort[0].second)) / camhigh,
						1.0 * dispx / 1920 * (camhigh + (pl.bone[g.first].zp - pl.sort[0].second)) / camhigh,
						double(pl.bone[g.first].yrad + pl.bone[g.first].yr), pl.Graphs[g.first].get(), TRUE);
				}
			}
		}
	}
	inline void draw_wall_shadow(int UorL, con* z) {//一辺

		const auto a00_1 = getpos(dispx / 2 + xpos + tilesize * (z->px + 0) + int(float(z->hight - z->bottom)*sin(light_yrad)), dispy / 2 + ypos + tilesize * (z->py + 0) + int(float(z->hight - z->bottom)*cos(light_yrad)), z->bottom);
		const auto a10_1 = getpos(dispx / 2 + xpos + tilesize * (z->px + 1) + int(float(z->hight - z->bottom)*sin(light_yrad)), dispy / 2 + ypos + tilesize * (z->py + 0) + int(float(z->hight - z->bottom)*cos(light_yrad)), z->bottom);
		const auto a01_1 = getpos(dispx / 2 + xpos + tilesize * (z->px + 0) + int(float(z->hight - z->bottom)*sin(light_yrad)), dispy / 2 + ypos + tilesize * (z->py + 1) + int(float(z->hight - z->bottom)*cos(light_yrad)), z->bottom);
		const auto a11_1 = getpos(dispx / 2 + xpos + tilesize * (z->px + 1) + int(float(z->hight - z->bottom)*sin(light_yrad)), dispy / 2 + ypos + tilesize * (z->py + 1) + int(float(z->hight - z->bottom)*cos(light_yrad)), z->bottom);

		const auto a00_0 = getpos(dispx / 2 + xpos + tilesize * (z->px + 0), dispy / 2 + ypos + tilesize * (z->py + 0), z->bottom);//◤
		const auto a10_0 = getpos(dispx / 2 + xpos + tilesize * (z->px + 1), dispy / 2 + ypos + tilesize * (z->py + 0), z->bottom);//◥
		const auto a01_0 = getpos(dispx / 2 + xpos + tilesize * (z->px + 0), dispy / 2 + ypos + tilesize * (z->py + 1), z->bottom);//◣
		const auto a11_0 = getpos(dispx / 2 + xpos + tilesize * (z->px + 1), dispy / 2 + ypos + tilesize * (z->py + 1), z->bottom);//◢

		SetDrawBright(0, 0, 0);
		if (UorL < 20) {
			if (UorL % 4 == 0) {
				switch (UorL) {
				case 0://縦(上)
					if (z->hight != z->bottom) {
						DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 4:
					DrawModiGraph(a01_1.x, a01_1.y, a11_1.x, a11_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					break;
				case 8://上◢
					if (z->hight != z->bottom) {
						DrawModiGraph(a10_1.x, a10_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 16://上◣
					if (z->hight != z->bottom) {
						DrawModiGraph(a00_1.x, a00_1.y, a00_1.x, a00_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				}
			}
			if (UorL % 4 == 1) {
				switch (UorL) {
				case 1://横(左)
					if (z->hight != z->bottom) {
						DrawModiGraph(a01_1.x, a01_1.y, a00_1.x, a00_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 5://左◢
					if (z->hight != z->bottom) {
						DrawModiGraph(a01_1.x, a01_1.y, a01_1.x, a01_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 9:
						DrawModiGraph(a11_1.x, a11_1.y, a10_1.x, a10_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					break;
				case 13://左◥
					if (z->hight != z->bottom) {
						DrawModiGraph(a00_1.x, a00_1.y, a00_1.x, a00_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				}
			}
			if (UorL % 4 == 2) {
				switch (UorL) {
				case 2://縦(下)
					if (z->hight != z->bottom) {
						DrawModiGraph(a01_1.x, a01_1.y, a11_1.x, a11_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 10://下◢
					if (z->hight != z->bottom) {
						DrawModiGraph(a11_1.x, a11_1.y, a11_1.x, a11_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 14:
						DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					break;
				case 18://下◣
					if (z->hight != z->bottom) {
						DrawModiGraph(a01_1.x, a01_1.y, a01_1.x, a01_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				}
			}
			if (UorL % 4 == 3) {
				switch (UorL) {
				case 3://横(右)
					if (z->hight != z->bottom) {
						DrawModiGraph(a11_1.x, a11_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 7://右◢
					if (z->hight != z->bottom) {
						DrawModiGraph(a11_1.x, a11_1.y, a11_1.x, a11_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 15://右◥
					if (z->hight != z->bottom) {
						DrawModiGraph(a10_1.x, a10_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 19:
						DrawModiGraph(a01_1.x, a01_1.y, a00_1.x, a00_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->wallhandle->get(), TRUE);
					break;
				}
			}
		}
		SetDrawBright(255, 255, 255);
	}
	inline void drw_rect_shadow(con* z) {
		switch (z->use) {//三角柱
		case 0://上
			draw_wall_shadow(4, z);		//縦(上)//4
			draw_wall_shadow(5, z);		//横(左)
			draw_wall_shadow(2, z);		//縦(下)
			draw_wall_shadow(7, z);		//横(右)
			break;
		case 1://左
			draw_wall_shadow(8, z);		//縦(上)//8
			draw_wall_shadow(9, z);		//横(左)
			draw_wall_shadow(10, z);	//縦(下)
			draw_wall_shadow(3, z);		//横(右)
			break;
		case 2://下
			draw_wall_shadow(0, z);		//縦(上)//12
			draw_wall_shadow(13, z);	//横(左)
			draw_wall_shadow(14, z);	//縦(下)
			draw_wall_shadow(15, z);	//横(右)
			break;
		case 3://右
			draw_wall_shadow(16, z);	//縦(上)//16
			draw_wall_shadow(1, z);		//横(左)
			draw_wall_shadow(18, z);	//縦(下)
			draw_wall_shadow(19, z);	//横(右)
			break;
		default://柱
			draw_wall_shadow(0, z);		//縦(上)
			draw_wall_shadow(1, z);		//横(左)
			draw_wall_shadow(2, z);		//縦(下)
			draw_wall_shadow(3, z);		//横(右)
			draw_wall_shadow(20, z);	//天井
			break;
		}
	}
	inline void drw_human_shadow(con* z) {
		SetDrawBright(0,0,0);
		for (auto& pl : human) {
			for (auto& g : pl.sort) {
				auto q = getpos(dispx / 2 + pl.bone[g.first].xp + pl.pos.x, dispy / 2 + pl.bone[g.first].yp + pl.pos.y, 0);

				if (q.x >= z->zero[0].x && q.y >= z->zero[0].y && q.x <= z->zero[3].x && q.y <= z->zero[3].y) {

					auto p = getpos(
						dispx / 2 + pl.bone[g.first].xp + pl.pos.x + int(float(pl.bone[g.first].zp - pl.sort[0].second)*sin(light_yrad)),
						dispy / 2 + pl.bone[g.first].yp + pl.pos.y + int(float(pl.bone[g.first].zp - pl.sort[0].second)*cos(light_yrad)),
						0);

					DrawRotaGraph3(p.x, p.y, 16, 16,
						1.0 * dispx / 1920 * (camhigh + (pl.bone[g.first].zp - pl.sort[0].second)) / camhigh,
						1.0 * dispx / 1920 * (camhigh + (pl.bone[g.first].zp - pl.sort[0].second)) / camhigh,
						double(pl.bone[g.first].yrad + pl.bone[g.first].yr), pl.Graphs[g.first].get(), TRUE);

				}
			}
		}
		SetDrawBright(255, 255, 255);
	}
	inline void set_cam(int sx, int sy) {
		xpos = sx;
		ypos = sy;
		return;
	}
	inline void set_drw_rect(int px, int py, int bottom, int hight, int walldir, int floordir, int UorL = -1){//三角柱
		auto& z = zcon[px][py];
		z.px = px;
		z.py = py;
		z.bottom = bottom;
		z.hight = hight;

		z.top[0] = getpos(dispx / 2 + xpos + tilesize * (z.px + 0), dispy / 2 + ypos + tilesize * (z.py + 0), z.hight);
		z.top[1] = getpos(dispx / 2 + xpos + tilesize * (z.px + 1), dispy / 2 + ypos + tilesize * (z.py + 0), z.hight);
		z.top[2] = getpos(dispx / 2 + xpos + tilesize * (z.px + 0), dispy / 2 + ypos + tilesize * (z.py + 1), z.hight);
		z.top[3] = getpos(dispx / 2 + xpos + tilesize * (z.px + 1), dispy / 2 + ypos + tilesize * (z.py + 1), z.hight);

		z.floor[0] = getpos(dispx / 2 + xpos + tilesize * (z.px + 0), dispy / 2 + ypos + tilesize * (z.py + 0), z.bottom);
		z.floor[1] = getpos(dispx / 2 + xpos + tilesize * (z.px + 1), dispy / 2 + ypos + tilesize * (z.py + 0), z.bottom);
		z.floor[2] = getpos(dispx / 2 + xpos + tilesize * (z.px + 0), dispy / 2 + ypos + tilesize * (z.py + 1), z.bottom);
		z.floor[3] = getpos(dispx / 2 + xpos + tilesize * (z.px + 1), dispy / 2 + ypos + tilesize * (z.py + 1), z.bottom);

		z.zero[0] = getpos(dispx / 2 + xpos + tilesize * (z.px + 0), dispy / 2 + ypos + tilesize * (z.py + 0), 0);
		z.zero[1] = getpos(dispx / 2 + xpos + tilesize * (z.px + 1), dispy / 2 + ypos + tilesize * (z.py + 0), 0);
		z.zero[2] = getpos(dispx / 2 + xpos + tilesize * (z.px + 0), dispy / 2 + ypos + tilesize * (z.py + 1), 0);
		z.zero[3] = getpos(dispx / 2 + xpos + tilesize * (z.px + 1), dispy / 2 + ypos + tilesize * (z.py + 1), 0);

		z.wallhandle = &walls[walldir];
		z.floorhandle = &floors[floordir];

		z.use = std::clamp(UorL, -1, 4);
	}
	inline void set_human(size_t* id,int xp,int yp) {
		using namespace std::literals;
		*id = human.size();
		human.resize(*id + 1);
		auto& z = human[*id];
		GraphHandle::LoadDiv("data/Char/1.bmp", 33, 11, 3, 32, 32, &z.Graphs);
		z.sort.resize(z.Graphs.size());
		/*
		{//キャラバイナリ書き込み
			std::vector<Bonesdata> n;
			n.clear();
			{
				{//左腕
					n.resize(n.size() + 1);
					n.back().parent = 1;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -2.0f;

					n.resize(n.size() + 1);
					n.back().parent = 2;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -5.0f;

					n.resize(n.size() + 1);
					n.back().parent = 3;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -4.0f;

					n.resize(n.size() + 1);
					n.back().parent = 4;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -5.0f;

					n.resize(n.size() + 1);
					n.back().parent = 5;
					n.back().xdist = -8.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -2.0f;
				}
				n.resize(n.size() + 1);
				n.back().parent = 15;
				n.back().xdist = 0.0f;
				n.back().ydist = 0.0f;
				n.back().zdist = -3.0f;
				{//右腕
					n.resize(n.size() + 1);
					n.back().parent = 5;
					n.back().xdist = 8.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -2.0f;

					n.resize(n.size() + 1);
					n.back().parent = 6;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -5.0f;

					n.resize(n.size() + 1);
					n.back().parent = 7;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -4.0f;

					n.resize(n.size() + 1);
					n.back().parent = 8;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -5.0f;

					n.resize(n.size() + 1);
					n.back().parent = 9;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -2.0f;
				}
			}
			n.resize(n.size() + 1);
			n.resize(n.size() + 1);
			n.resize(n.size() + 1);
			n.resize(n.size() + 1);

			n.resize(n.size() + 1);
			n.back().parent = -1;
			n.back().xdist = 0.0f;
			n.back().ydist = 0.0f;
			n.back().zdist = 0.0f;

			n.resize(n.size() + 1);
			n.back().parent = 5;
			n.back().xdist = 0.0f;
			n.back().ydist = 0.0f;
			n.back().zdist = -3.0f;

			n.resize(n.size() + 1);
			n.resize(n.size() + 1);
			n.resize(n.size() + 1);
			n.resize(n.size() + 1);
			n.resize(n.size() + 1);

			{
				{
					n.resize(n.size() + 1);
					n.back().parent = 23;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -1.0f;

					n.resize(n.size() + 1);
					n.back().parent = 24;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -4.0f;

					n.resize(n.size() + 1);
					n.back().parent = 25;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -3.0f;

					n.resize(n.size() + 1);
					n.back().parent = 26;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -4.0f;

					n.resize(n.size() + 1);
					n.back().parent = 27;
					n.back().xdist = -5.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -3.0f;
					//n.back().yrad = deg2rad(-90);
					//n.back().xrad = deg2rad(-70);
				}
				n.resize(n.size() + 1);
				n.back().parent = 16;
				n.back().xdist = 0.0f;
				n.back().ydist = 0.0f;
				n.back().zdist = -3.0f;
				{
					n.resize(n.size() + 1);
					n.back().parent = 27;
					n.back().xdist = 5.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -3.0f;
					//n.back().yrad = deg2rad(-90);
					//n.back().xrad = deg2rad(-70);

					n.resize(n.size() + 1);
					n.back().parent = 28;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -4.0f;

					n.resize(n.size() + 1);
					n.back().parent = 29;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -3.0f;

					n.resize(n.size() + 1);
					n.back().parent = 30;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -4.0f;

					n.resize(n.size() + 1);
					n.back().parent = 31;
					n.back().xdist = 0.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = -1.0f;
				}
			}

			std::fstream file;
			// 書き込む
			file.open("data/Char/1.dat", std::ios::binary | std::ios::out);
			for (auto& m : n) {
				file.write((char*)&m, sizeof(m));
			}
			file.close();
		}
		//*/

		{//モーションテキスト(直に打ち込めるように)
		z.anime.resize(z.anime.size() + 1);
			const auto mdata = FileRead_open("data/Char/Mot/0.mot", FALSE);
			{
				do {
					int tmp;
					std::string ttt = getcmd(mdata, &tmp);
					if (ttt.find("frame") != std::string::npos) {
						z.anime.back().resize(z.anime.back().size() + 1);
						z.anime.back().back().bone.resize(33);//todo
						z.anime.back().back().time = tmp;
					}
					else if (ttt.find("left_arm_") != std::string::npos) {
						if (ttt.find("x") != std::string::npos) {
							z.anime.back().back().bone[4].xrad = deg2rad(tmp);
						}
						else if (ttt.find("y") != std::string::npos) {
							z.anime.back().back().bone[4].yrad = deg2rad(tmp);
						}
						else if (ttt.find("z") != std::string::npos) {
							z.anime.back().back().bone[4].zrad = deg2rad(tmp);
						}
					}
					else if (ttt.find("right_arm_") != std::string::npos) {
						if (ttt.find("x") != std::string::npos) {
							z.anime.back().back().bone[6].xrad = deg2rad(tmp);
						}
						else if (ttt.find("y") != std::string::npos) {
							z.anime.back().back().bone[6].yrad = deg2rad(tmp);
						}
						else if (ttt.find("z") != std::string::npos) {
							z.anime.back().back().bone[6].zrad = deg2rad(tmp);
						}
					}

					else if (ttt.find("left_leg_") != std::string::npos) {
						if (ttt.find("x") != std::string::npos) {
							z.anime.back().back().bone[26].xrad = deg2rad(tmp);
						}
						else if (ttt.find("y") != std::string::npos) {
							z.anime.back().back().bone[26].yrad = deg2rad(tmp);
						}
						else if (ttt.find("z") != std::string::npos) {
							z.anime.back().back().bone[26].zrad = deg2rad(tmp);
						}
					}
					else if (ttt.find("right_leg_") != std::string::npos) {
						if (ttt.find("x") != std::string::npos) {
							z.anime.back().back().bone[28].xrad = deg2rad(tmp);
						}
						else if (ttt.find("y") != std::string::npos) {
							z.anime.back().back().bone[28].yrad = deg2rad(tmp);
						}
						else if (ttt.find("z") != std::string::npos) {
							z.anime.back().back().bone[28].zrad = deg2rad(tmp);
						}
					}


					else if (ttt.find("end") != std::string::npos) {
						break;
					}
				} while (true);
			}
			FileRead_close(mdata);
		}
		{//キャラバイナリ
			std::fstream file;
			file.open("data/Char/1.dat", std::ios::binary | std::ios::in);
			while (!file.eof()) {
				z.bone.resize(z.bone.size() + 1);
				file.read((char*)&z.bone.back(), sizeof(z.bone.back()));
			}
			z.bone.pop_back();
			file.close();
		}
		z.pos.x = xp;
		z.pos.y = yp;
	}
	inline void setpos_player(int x_pos, int y_pos) {
		human[player_id].vtmp = human[player_id].pos;
		human[player_id].pos.x = -x_pos;
		human[player_id].pos.y = -y_pos;
		human[player_id].vtmp.x -= human[player_id].pos.x;
		human[player_id].vtmp.y -= human[player_id].pos.y;
		move_human();
	}
	inline void move_human() {
		for (int i = 0; i < human.size(); i++) {
			if (i != player_id) {
				human[i].vtmp = human[i].pos;
				//cpu
				//human[i].pos.x = 100*i;
				//human[i].pos.y = 100*i;
				//
				human[i].vtmp.x -= human[i].pos.x;
				human[i].vtmp.y -= human[i].pos.y;
			}
		}
	}
	inline void draw_player(void) {
		for (auto& pl : human) {
			for (auto& g : pl.bone) {
				g.edit = false;
			}

			//ここでアニメーション
			{
				pl.animetime++;

				if (pl.animetime < pl.anime[0][pl.animeframe].time) {
					for (int z = 0; z < pl.bone.size(); z++) {
						pl.bone[z].xrad = pl.anime[0][pl.animeframe].bone[z].xrad + (pl.anime[0][(pl.animeframe + 1) % pl.anime[0].size()].bone[z].xrad - pl.anime[0][pl.animeframe].bone[z].xrad)*pl.animetime / pl.anime[0][pl.animeframe].time;
						pl.bone[z].yrad = pl.anime[0][pl.animeframe].bone[z].yrad + (pl.anime[0][(pl.animeframe + 1) % pl.anime[0].size()].bone[z].yrad - pl.anime[0][pl.animeframe].bone[z].yrad)*pl.animetime / pl.anime[0][pl.animeframe].time;
						pl.bone[z].zrad = pl.anime[0][pl.animeframe].bone[z].zrad + (pl.anime[0][(pl.animeframe + 1) % pl.anime[0].size()].bone[z].zrad - pl.anime[0][pl.animeframe].bone[z].zrad)*pl.animetime / pl.anime[0][pl.animeframe].time;
					}
				}
				else {
					pl.animeframe = (pl.animeframe + 1) % pl.anime[0].size();
					pl.animetime = 0;
				}
			}
			//
			if (pl.vtmp.x != 0 || pl.vtmp.y != 0) {
				pl.vec = pl.vtmp;
			}
			{
				//移動方向に向く
				auto b = std::hypotf(float(pl.vec.x), float(pl.vec.y));
				auto q = (float(pl.vec.x)*cos(pl.yrad) - float(pl.vec.y)* -sin(pl.yrad)) / b;
				if (q > sin(deg2rad(10))) {
					pl.yrad += deg2rad(-5);
				}else if (q < sin(deg2rad(-10))) {
					pl.yrad += deg2rad(5);
				}
				else {
					if (q > sin(deg2rad(5))) {
						pl.yrad += deg2rad(-1);
					}
					else if (q < sin(deg2rad(-5))) {
						pl.yrad += deg2rad(1);
					}
				}
				//真後ろ振り向き
				if ((float(pl.vec.x)* -sin(pl.yrad) + float(pl.vec.y)*cos(pl.yrad)) / b <= -0.5) {
					pl.yrad += deg2rad(10);
				}
			}
			//座標指定
			bool next = false;
			do {
				next = false;
				for (auto& z : pl.bone) {
					auto p = z.parent;
					if (!z.edit) {
						if (p == -1) {
							z.xp = xpos;
							z.yp = ypos;
							z.zp = 0;
							z.xr = z.xrad;
							z.yr = z.yrad + pl.yrad;
							z.zr = z.zrad;
							z.edit = true;
						}
						else {
							if (pl.bone[p].edit) {
								z.xr = pl.bone[p].xrad + pl.bone[p].xr;
								z.yr = pl.bone[p].yrad + pl.bone[p].yr;
								z.zr = pl.bone[p].yrad + pl.bone[p].zr;

								float y1 = cos(z.xr)*z.ydist + sin(z.xr)*z.zdist;
								float z1 = cos(z.xr)*z.zdist - sin(z.xr)*z.ydist;
								float x2 = cos(z.zr)*z.xdist + sin(z.zr)*z1;

								z.xp = pl.bone[p].xp + int(cos(z.yr)*x2 - sin(z.yr)*y1);
								z.yp = pl.bone[p].yp + int(cos(z.yr)*y1 + sin(z.yr)*x2);
								z.zp = pl.bone[p].zp + int(cos(z.zr)*z1 - sin(z.zr)*z.xdist);
								z.edit = true;
							}
							else {
								next = true;
							}
						}
					}
				}
			} while (next);

			//高さでソート
			for (int i = 0; i < pl.sort.size(); i++) {
				pl.sort[i].first = i;
				pl.sort[i].second = pl.bone[i].zp;
			}
			for (int i = 0; i < pl.sort.size(); i++) {
				for (int j = i; j < pl.sort.size(); j++) {
					if (pl.sort[j].second <= pl.sort[i].second) {
						std::swap(pl.sort[j], pl.sort[i]);
					}
				}
			}
		}
	}
	inline void put_drw(void) {
		const auto limmin_x = getpos(-dispy / 4, dispy * 5 / 4, 0).x;
		const auto limmin_y = getpos(-dispy / 4, -dispy / 4, 0).y;
		const auto cam = getpos(dispx / 2, dispy / 2, 0);
		const auto limmax = getpos(dispx * 5 / 4, dispy * 5 / 4, 0);
		draw_player();

		//light_yrad += deg2rad(1);
		//DRAW
		//上階
		for (auto& c : zcon) {
			for (auto& z : c) {
				if (z.top[0].x <= cam.x && z.top[0].y <= cam.y && z.floor[3].y >= limmin_y  && z.floor[3].x >= limmin_x && !(z.bottom == z.hight == 0)) {
					drw_rect(&z);
				}
			}
			for (int y = int(c.size()) - 1; y >= 0; --y) {
				auto& z = c[y];
				if (z.top[0].x <= cam.x && z.top[3].y >= cam.y && z.floor[0].y <= limmax.y  && z.floor[3].x >= limmin_x && !(z.bottom == z.hight == 0)) {
					drw_rect(&z);
				}
			}
		}
		for (int x = int(zcon.size()) - 1; x >= 0; --x) {
			for (auto& z : zcon[x]) {
				if (z.top[3].x >= cam.x && z.top[0].y <= cam.y && z.floor[3].y >= limmin_y && z.floor[0].x <= limmax.x && !(z.bottom == z.hight == 0)) {
					drw_rect(&z);
				}
			}
			for (int y = int(zcon[x].size()) - 1; y >= 0; --y) {
				auto& z = zcon[x][y];
				if (z.top[3].x >= cam.x && z.top[3].y >= cam.y&& z.floor[0].y <= limmax.y  && z.floor[0].x <= limmax.x && !(z.bottom == z.hight == 0)) {
					drw_rect(&z);
				}

			}
		}
		//影
		auto s = GetDrawScreen();
		SetDrawScreen(shadow_graph);
		ClearDrawScreen();
		for (auto& c : zcon) {
			for (auto& z : c) {
				if (z.floor[0].y <= limmax.y  && z.floor[0].x <= limmax.x && z.floor[3].y >= limmin_y && z.floor[3].x >= limmin_x) {
					drw_rect_shadow(&z);
					drw_human_shadow(&z);
				}
			}
		}
		SetDrawScreen(s);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawGraph(0, 0, shadow_graph, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//上階
		for (auto& c : zcon) {
			for (auto& z : c) {
				if (z.top[0].x <= cam.x && z.top[0].y <= cam.y && z.floor[3].y >= limmin_y && z.floor[3].x >= limmin_x) {
					if (z.bottom == z.hight == 0) {
						drw_rect(&z);
					}
					drw_human(&z);
				}
			}
			for (int y = int(c.size()) - 1; y >= 0; --y) {
				auto& z = c[y];
				if (z.top[0].x <= cam.x && z.top[3].y >= cam.y && z.floor[0].y <= limmax.y  && z.floor[3].x >= limmin_x) {
					if (z.bottom == z.hight == 0) {
						drw_rect(&z);
					}
					drw_human(&z);
				}
			}
		}
		for (int x = int(zcon.size()) - 1; x >= 0; --x) {
			for (auto& z : zcon[x]) {
				if (z.top[3].x >= cam.x && z.top[0].y <= cam.y && z.floor[3].y >= limmin_y && z.floor[0].x <= limmax.x) {
					if (z.bottom == z.hight == 0) {
						drw_rect(&z);
					}
					drw_human(&z);
				}
			}
			for (int y = int(zcon[x].size()) - 1; y >= 0; --y) {
				auto& z = zcon[x][y];
				if (z.top[3].x >= cam.x && z.top[3].y >= cam.y&& z.floor[0].y <= limmax.y  && z.floor[0].x <= limmax.x){
					if (z.bottom == z.hight == 0) {
						drw_rect(&z);
					}
					drw_human(&z);
				}
			}
		}



	}
};

class DeBuG {
private:
	float deb[60][6 + 1];
	LONGLONG waypoint;
	float waydeb[6];
	size_t seldeb;
	FontHandle font;
public:
	DeBuG(void);
	void put_way(void);
	void end_way(void);
	void debug(float fps, float time);
};

#endif
