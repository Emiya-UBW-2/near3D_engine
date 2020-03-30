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
#include <thread> 

#include <chrono>

class MainClass {
private:
	/*setting*/
	int refrate;
	bool USE_YSync;	       /*垂直同期*/
	int frate;	       /*fps*/
	int se_vol;	       /**/
	int bgm_vol;	       /**/
	bool use_pad;		/**/
	std::optional<LONGLONG> time;
public:
	/*setting*/
	inline const auto get_se_vol(void) { return se_vol; }
	inline const auto get_bgm_vol(void) { return bgm_vol; }
	inline const auto get_use_pad(void) { return use_pad; }
	bool write_setting(void);
	MainClass(void);
	~MainClass(void);
	/*draw*/
	void Start_Screen(void);
	void Screen_Flip(void);
	void Screen_Flip(LONGLONG waits);
};

class Draw{
private:
	//2Dベクトル関連
	class pos2D {
	public:
		int x;
		int y;
		//加算した値を返す
		inline pos2D operator+(pos2D o)noexcept {
			pos2D temp;
			temp.x = this->x + o.x;
			temp.y = this->y + o.y;
			return temp;
		}
		//加算する
		inline pos2D& operator+=(pos2D o)noexcept {
			this->x += o.x;
			this->y += o.y;
			return *this;
		}
		//減算した値を変えす
		inline pos2D operator-(pos2D o)noexcept {
			pos2D temp;
			temp.x = this->x - o.x;
			temp.y = this->y - o.y;
			return temp;
		}
		//減算する
		inline pos2D& operator-=(pos2D o)noexcept {
			this->x -= o.x;
			this->y -= o.y;
			return *this;
		}
		//距離の2乗を取得する
		inline int hydist()noexcept {
			return this->x*this->x + this->y*this->y;
		}
		// 内積
		inline int dot(pos2D* v2)noexcept {
			return this->x * v2->x + this->y * v2->y;
		}
		// 外積
		inline int cross(pos2D* v2)noexcept {
			return this->x * v2->y - this->y * v2->x;
		}
	};
	//線分衝突
	inline bool ColSeg(pos2D &pos1, pos2D &vec1, pos2D &pos2, pos2D &vec2) {
		auto Crs_v1_v2 = vec1.cross(&vec2);
		if (Crs_v1_v2 == 0) {
			return false;// 平行
		}
		pos2D v = pos2 - pos1;
		auto Crs_v_v1 = v.cross(&vec1);
		auto Crs_v_v2 = v.cross(&vec2);
		if (Crs_v_v2 < 0 || Crs_v_v2 > Crs_v1_v2 || Crs_v_v1 < 0 || Crs_v_v1 > Crs_v1_v2) {
			return false;// 交差X
		}
		return true;
	}
	//座標変換
	inline pos2D getpos(int xp, int yp, int high) {
		pos2D p;
		p.x = dispx / 2 + (camhigh * xp) / std::max(camhigh - high, 1);
		p.y = dispy / 2 + (camhigh * yp) / std::max(camhigh - high, 1);
		return p;
	}
private:
	const int camhigh = 256;
	const int tilesize = 32;
	struct con {
		std::array<pos2D, 4> top;
		std::array<pos2D, 4> floor;
		uint8_t use;// rect = -1 else prism = 0~3,4~7
		pos2D cpos;
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
	struct maps {
		int plx, ply;
		char wall_name[64];
		char floor_name[64];
		float light_yrad;
	};
	struct enemies {
		int xp, yp;
	};
	std::vector<Status> ans;
	std::vector<std::vector<con>> zcon;
	int xpos = 0, ypos = 0;
	float light_yrad=0.0f;
	std::vector<GraphHandle> walls;
	std::vector<GraphHandle> floors;
	int shadow_graph;
	int screen;
	struct Bonesdata {
		int parent;
		int xp, yp, zp;
		float xr, yr, zr;
		float xrad,yrad,zrad;
		float xdist,ydist,zdist;
		bool edit;
	};
	struct Animesdata {
		int time=0;
		std::vector<Bonesdata> bone;
	};
	struct Humans {
		std::vector<GraphHandle> Graphs;
		std::vector<Bonesdata> bone;
		std::vector<pair> sort;
		std::vector<std::vector<Animesdata>> anime;
		pos2D vtmp = { 0,0 };
		pos2D vec = { 0,0 };
		float yrad = 0.f;
		pos2D pos = { 0,0 };
		int animeframe=0;
		int animetime=1;
		int animesel=0;
	};
	std::vector<Humans> human;
	size_t player_id=0;
public:
	Draw(void){
		shadow_graph = MakeScreen(dispx, dispy, TRUE);
		screen = MakeScreen(dispx, dispy,FALSE);
	}
	~Draw(void){
		exit_map();
	}
	//mapエディター
	inline bool write_map(std::string mapname) {
		using namespace std::literals;
		std::fstream file;
		const auto font40 = FontHandle::Create(x_r(40), y_r(40 / 3), DX_FONTTYPE_ANTIALIASING);
		int map_x = 0, map_y = 0;
		bool wallorfloor = false;
		size_t wofcnt = 0;

		//mapデータプリセット
		std::vector<Status> n;
		{
			for (unsigned int y = 0; y < 40; y++) {
				for (unsigned int x = 0; x < 40; x++) {
					const int btm = 0;
					const int mid = 0;
					n.resize(n.size() + 1);
					n.back() = { x, y, btm,mid, 2, 2, -1 };
					map_x = std::max<int>(n.back().xp, map_x);
					map_y = std::max<int>(n.back().yp, map_y);
				}
			}
			map_x++;
			map_y++;
		}
		maps mapdata;
		{
			mapdata.plx = 32;
			mapdata.ply = 32;
			mapdata.light_yrad = deg2rad(0);
			strcpy_s(mapdata.wall_name, "data/Chip/Wall/1.bmp");
			strcpy_s(mapdata.floor_name, "data/Chip/Floor/1.bmp");
		}
		std::vector<enemies> e;

		//mapデータ1書き込み(マップチップ)
		for (unsigned int y = 0; y < 40; y += 5) {
			for (unsigned int x = 0; x < 40; x += 5) {
				const int btm = 0;
				const int mid = 0;
				const int hig = 80 * (x + y * 40) / (40 * 40);
				n[(x + 2) + (y + 1) * 40] = { x + 2, y + 1, btm, hig, 2, 2, 0 };
				n[(x + 1) + (y + 2) * 40] = { x + 1, y + 2, btm, hig, 2, 2, 1 };
				n[(x + 2) + (y + 2) * 40] = { x + 2, y + 2, btm, hig, 2, 1, -1 };
				n[(x + 3) + (y + 2) * 40] = { x + 3, y + 2, btm, hig, 2, 2, 3 };
				n[(x + 2) + (y + 3) * 40] = { x + 2, y + 3, btm, hig, 2, 2, 2 };
			}
		}
		//mapデータ2書き込み(プレイヤー初期位置、使用テクスチャ指定)
		{
			mapdata.plx = 32;
			mapdata.ply = 32;
			//mapdata.light_yrad = deg2rad(0);
			mapdata.light_yrad = deg2rad(45);
			strcpy_s(mapdata.wall_name, "data/Chip/Wall/1.bmp");
			strcpy_s(mapdata.floor_name, "data/Chip/Floor/1.bmp");
		}
		//mapデータ3書き込み(敵情報)
		{
			for (int i = 1; i < 8; i++) {
				e.resize(e.size() + 1);
				e.back().xp = 32 * 5 * i + 16;
				e.back().yp = 32 * 5 * i + 16;
			}
		}

		while (ProcessMessage() == 0) {
			SetDrawScreen(DX_SCREEN_BACK);
			ClearDrawScreen();

			int mousex, mousey;
			GetMousePoint(&mousex, &mousey);
			{
				for (auto& m : n) {
					int xs = int(m.xp*dispy / map_x);
					int ys = int(m.yp*dispy / map_y);
					int xe = int((m.xp + 1)*dispy / map_x);
					int ye = int((m.yp + 1)*dispy / map_y);

					if (inm(xs, ys, xe, ye)) {
						if (m.bottom != m.hight) {
							DrawBox(xs, ys, xe, ye, GetColor(255 * (camhigh - m.hight) / camhigh, 0, 0), TRUE);
						}
						else {
							DrawBox(xs, ys, xe, ye, GetColor(255 * (camhigh - m.hight) / camhigh, 0, 0), TRUE);
						}
						font40.DrawStringFormat(dispy, 0, GetColor(255, 255, 255), "(%03d,%03d)", m.xp, m.yp);
					}
					else {
						if (m.bottom != m.hight) {
							DrawBox(xs, ys, xe, ye, GetColor(255 * (camhigh - m.hight) / camhigh, 255 * (camhigh - m.hight) / camhigh, 0), TRUE);
						}
						else {
							DrawBox(xs, ys, xe, ye, GetColor(255 * (camhigh - m.hight) / camhigh, 255 * (camhigh - m.hight) / camhigh, 255 * (camhigh - m.hight) / camhigh), TRUE);
						}
					}

					if (m.bottom != m.hight) {
						DrawBox(xs, ys, xe, ye, GetColor(0, 0, 0), FALSE);
					}
				}
			}
			//壁か床か
			{
				int xs = x_r(1080 + 40);
				int ys = y_r(1080 - 600);
				int xe = x_r(1080 + 340);
				int ye = y_r(1080 - 560);
				if (inm(xs, ys, xe, ye)) {
					DrawBox(xs, ys, xe, ye, GetColor(255, 0, 0), TRUE);
					font40.DrawString(xs + (xe - xs - font40.GetDrawWidth(wallorfloor ? "壁" : "床")) / 2, ys, wallorfloor ? "壁" : "床", GetColor(255, 255, 0));

					wofcnt = std::min<size_t>(wofcnt + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0);
					if (wofcnt == 1) {
						wallorfloor ^= 1;
					}
				}
				else {
					DrawBox(xs, ys, xe, ye, GetColor(255, 255, 0), TRUE);
					font40.DrawString(xs + (xe - xs - font40.GetDrawWidth(wallorfloor ? "壁" : "床")) / 2, ys, wallorfloor ? "壁" : "床", GetColor(255, 0, 0));
					if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {}
				}
			}
			//終了
			{
				int xs = x_r(1080 + 40);
				int ys = y_r(1080 - 400);
				int xe = x_r(1080 + 340);
				int ye = y_r(1080 - 360);
				if (inm(xs, ys, xe, ye)) {
					DrawBox(xs, ys, xe, ye, GetColor(255, 0, 0), TRUE);
					font40.DrawString(xs + (xe - xs - font40.GetDrawWidth("保存せず終了")) / 2, ys, "保存せず終了", GetColor(255, 255, 0));
					if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

						return false;
					}
				}
				else {
					DrawBox(xs, ys, xe, ye, GetColor(255, 255, 0), TRUE);
					font40.DrawString(xs + (xe - xs - font40.GetDrawWidth("保存せず終了")) / 2, ys, "保存せず終了", GetColor(255, 0, 0));
					if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {}
				}
			}
			//終了
			{
				int xs = x_r(1080 + 40);
				int ys = y_r(1080 - 200);
				int xe = x_r(1080 + 340);
				int ye = y_r(1080 - 160);
				if (inm(xs, ys, xe, ye)) {
					DrawBox(xs, ys, xe, ye, GetColor(255, 0, 0), TRUE);
					font40.DrawString(xs + (xe - xs - font40.GetDrawWidth("保存して終了")) / 2, ys, "保存して終了", GetColor(255, 255, 0));
					if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { break; }
				}
				else {
					DrawBox(xs, ys, xe, ye, GetColor(255, 255, 0), TRUE);
					font40.DrawString(xs + (xe - xs - font40.GetDrawWidth("保存して終了")) / 2, ys, "保存して終了", GetColor(255, 0, 0));
					if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {}
				}
			}


			ScreenFlip();

		}

		//mapデータ1書き込み(マップチップ)
		{
			file.open(("data/Map/" + mapname + "/1.dat").c_str(), std::ios::binary | std::ios::out);
			for (auto& m : n) {
				file.write((char*)&m, sizeof(m));
			}
			file.close();
			n.clear();
		}
		//mapデータ2書き込み(プレイヤー初期位置、使用テクスチャ指定)
		{
			file.open(("data/Map/" + mapname + "/2.dat").c_str(), std::ios::binary | std::ios::out);
			file.write((char*)&mapdata, sizeof(mapdata));
			file.close();
		}
		//mapデータ3書き込み(敵情報)
		{
			file.open(("data/Map/" + mapname + "/3.dat").c_str(), std::ios::binary | std::ios::out);
			for (auto& m : e)
				file.write((char*)&m, sizeof(m));
			file.close();
			e.clear();
		}
		return true;
	}
	//map選択
	inline void set_map(int *player_x, int *player_y, std::string mapname) {
		using namespace std::literals;
		std::fstream file;
		int map_x = 0, map_y = 0;
		//mapデータ1読み込み(マップチップ)
		{
			file.open(("data/Map/" + mapname + "/1.dat").c_str(), std::ios::binary | std::ios::in);
			do {
				ans.resize(ans.size() + 1);
				file.read((char*)&ans.back(), sizeof(ans.back()));
				map_x = std::max<int>(ans.back().xp, map_x);
				map_y = std::max<int>(ans.back().yp, map_y);
			} while (!file.eof());
			map_x++;
			map_y++;
			file.close();
			ans.pop_back();

			zcon.resize(map_x);
			for (auto& z : zcon) {
				z.resize(map_y);
			}
		}
		//mapデータ2読み込み(プレイヤー初期位置、使用テクスチャ指定)
		{
			maps mapb;
			file.open(("data/Map/" + mapname + "/2.dat").c_str(), std::ios::binary | std::ios::in);
			file.read((char*)&mapb, sizeof(mapb));
			file.close();
			*player_x = -mapb.plx;
			*player_y = -mapb.ply;
			light_yrad = mapb.light_yrad;
			GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, 16, 32, &walls);
			GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, 16, 16, &floors);
			set_human(&player_id, *player_x, *player_y);
		}
		//mapデータ3読み込み(敵情報)
		{
			file.open(("data/Map/" + mapname + "/3.dat").c_str(), std::ios::binary | std::ios::in);
			do {
				enemies anse;
				file.read((char*)&anse, sizeof(anse));
				size_t idb;
				set_human(&idb, anse.xp, anse.yp);
			} while (!file.eof());
			file.close();
		}
	}
	//mapの設置
	inline void put_map(void) {
		for (auto& z : ans) {
			set_drw_rect(z.xp, z.yp, z.bottom, z.hight, z.wall_id, z.floor_id, z.dir);
		}
	}
	//
	inline void exit_map(void) {
		walls.clear();
		floors.clear();
		human.clear();
		for (auto& z : zcon)
			z.clear();
		zcon.clear();
		return;
	}
	//壁描画
	inline void draw_wall(int UorL,con* z){//一辺
		const auto a00_1 = getpos(xpos + tilesize * (z->cpos.x + 0), ypos + tilesize * (z->cpos.y + 0), z->hight);
		const auto a10_1 = getpos(xpos + tilesize * (z->cpos.x + 1), ypos + tilesize * (z->cpos.y + 0), z->hight);
		const auto a01_1 = getpos(xpos + tilesize * (z->cpos.x + 0), ypos + tilesize * (z->cpos.y + 1), z->hight);
		const auto a11_1 = getpos(xpos + tilesize * (z->cpos.x + 1), ypos + tilesize * (z->cpos.y + 1), z->hight);

		const auto a00_0 = getpos(xpos + tilesize * (z->cpos.x + 0), ypos + tilesize * (z->cpos.y + 0), z->bottom);//◤
		const auto a10_0 = getpos(xpos + tilesize * (z->cpos.x + 1), ypos + tilesize * (z->cpos.y + 0), z->bottom);//◥
		const auto a01_0 = getpos(xpos + tilesize * (z->cpos.x + 0), ypos + tilesize * (z->cpos.y + 1), z->bottom);//◣
		const auto a11_0 = getpos(xpos + tilesize * (z->cpos.x + 1), ypos + tilesize * (z->cpos.y + 1), z->bottom);//◢
		if (UorL < 12) {
			if (UorL % 4 == 0) {
				int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(0))));
				SetDrawBright(c, c, c);
				switch (UorL) {
				case 0://縦(上)
					if (a00_0.y < a00_1.y && z->hight != z->bottom) {
						DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 4://上◢
					if (a10_0.y < a10_1.y && z->hight != z->bottom) {
						DrawModiGraph(a10_1.x, a10_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 8://上◣
					if (a00_0.y < a00_1.y && z->hight != z->bottom) {
						DrawModiGraph(a00_1.x, a00_1.y, a00_1.x, a00_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				}
			}
			if (UorL % 4 == 1) {
				int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(270))));
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
				case 9://左◥
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
				case 6://下◢
					if (a11_0.y >= a11_1.y && z->hight != z->bottom) {
						DrawModiGraph(a11_1.x, a11_1.y, a11_1.x, a11_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				case 10://下◣
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
				case 11://右◥
					if (a11_0.x >= a11_1.x && z->hight != z->bottom) {
						DrawModiGraph(a10_1.x, a10_1.y, a10_1.x, a10_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->wallhandle->get(), TRUE);
					}
					break;
				}
			}
		}
		else if (UorL < 20) {
			if (z->hight != z->bottom) {
				if (UorL % 4 == 0) {
					int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(0))));
					SetDrawBright(c, c, c);
					switch (UorL) {
					case 12:
						if (a00_0.y < a01_1.y) {
							DrawModiGraph(a01_1.x, a01_1.y, a11_1.x, a11_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->wallhandle->get(), TRUE);
						}
						break;
					case 16:
						if (a00_0.y < a01_1.y) {
							DrawModiGraph(a01_1.x, a01_1.y, a11_1.x, a11_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->floorhandle->get(), TRUE);
						}
						break;
					}
				}
				if (UorL % 4 == 1) {
					int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(270))));
					SetDrawBright(c, c, c);
					switch (UorL) {
					case 13:
						if (a00_0.x < a11_1.x) {
							DrawModiGraph(a11_1.x, a11_1.y, a10_1.x, a10_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
						}
						break;
					case 17:
						if (a00_0.x < a11_1.x) {
							DrawModiGraph(a11_1.x, a11_1.y, a10_1.x, a10_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->floorhandle->get(), TRUE);
						}
						break;
					}
				}
				if (UorL % 4 == 2) {
					int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(180))));
					SetDrawBright(c, c, c);
					switch (UorL) {
					case 14:
						if (a01_0.y > a10_1.y) {
							DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->floorhandle->get(), TRUE);
						}
						break;
					case 18:
						if (a01_0.y > a10_1.y) {
							DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->wallhandle->get(), TRUE);
						}
						break;
					}
				}
				if (UorL % 4 == 3) {
					int c = int(128.0f*(1.f + cos(light_yrad + deg2rad(90))));
					SetDrawBright(c, c, c);
					switch (UorL) {
					case 15:
						if (a10_0.x > a01_1.x) {
							DrawModiGraph(a01_1.x, a01_1.y, a00_1.x, a00_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->floorhandle->get(), TRUE);
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
					SetDrawBright(0, 0, 0);
					DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_1.x, a11_1.y, a01_1.x, a01_1.y, z->floorhandle->get(), TRUE);
				}
				else {
					int c = 255 * (camhigh - z->hight) / camhigh;
					SetDrawBright(c, c, c);
					DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_1.x, a11_1.y, a01_1.x, a01_1.y, z->floorhandle->get(), TRUE);
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
	//柱描画
	inline void drw_rect(con* z){
		switch (z->use) {//三角柱
		case 0://上
			draw_wall(12, z);	//縦(上)//12
			draw_wall(5, z);	//横(左)
			draw_wall(2, z);	//縦(下)
			draw_wall(7, z);	//横(右)
			break;
		case 1://左
			draw_wall(4, z);	//縦(上)//4
			draw_wall(13, z);	//横(左)
			draw_wall(6, z);	//縦(下)
			draw_wall(3, z);	//横(右)
			break;
		case 2://下
			draw_wall(0, z);	//縦(上)//12
			draw_wall(9, z);	//横(左)
			draw_wall(18, z);	//縦(下)
			draw_wall(11, z);	//横(右)
			break;
		case 3://右
			draw_wall(8, z);	//縦(上)//8
			draw_wall(1, z);	//横(左)
			draw_wall(10, z);	//縦(下)
			draw_wall(19, z);	//横(右)
			break;


		case 4://上
			draw_wall(16, z);	//縦(上)//12
			draw_wall(5, z);	//横(左)
			draw_wall(2, z);	//縦(下)
			draw_wall(7, z);	//横(右)
			break;
		case 5://左
			draw_wall(4, z);	//縦(上)//4
			draw_wall(17, z);	//横(左)
			draw_wall(6, z);	//縦(下)
			draw_wall(3, z);	//横(右)
			break;
		case 6://下
			draw_wall(0, z);	//縦(上)//12
			draw_wall(9, z);	//横(左)
			draw_wall(14, z);	//縦(下)
			draw_wall(11, z);	//横(右)
			break;
		case 7://右
			draw_wall(8, z);	//縦(上)//8
			draw_wall(1, z);	//横(左)
			draw_wall(10, z);	//縦(下)
			draw_wall(15, z);	//横(右)
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
	//人描画
	inline void drw_human(con* z) {
		for (auto& pl : human) {
			for (auto& g : pl.sort) {
				auto q = getpos(pl.bone[g.first].xp + pl.pos.x, pl.bone[g.first].yp + pl.pos.y, z->bottom);
				if (q.x >= z->floor[0].x && q.y >= z->floor[0].y && q.x <= z->floor[3].x && q.y <= z->floor[3].y) {
					auto p = getpos(pl.bone[g.first].xp + pl.pos.x, pl.bone[g.first].yp + pl.pos.y, pl.bone[g.first].zp - pl.sort[0].second + z->hight);
					DrawRotaGraph3(p.x, p.y, 16, 16,
						1.0 * dispx / 1920 * (camhigh + (pl.bone[g.first].zp - pl.sort[0].second + z->hight) * 5) / camhigh,
						1.0 * dispx / 1920 * (camhigh + (pl.bone[g.first].zp - pl.sort[0].second + z->hight) * 5) / camhigh,
						double(pl.bone[g.first].yrad) + double(pl.bone[g.first].yr), pl.Graphs[g.first].get(), TRUE);
				}
			}
		}
	}
	//壁影描画
	inline void draw_wall_shadow(int UorL, con* z) {//一辺
		const auto add_x = int(float(z->hight - z->bottom)*sin(light_yrad));
		const auto add_y = int(float(z->hight - z->bottom)*cos(light_yrad));
		const auto a00_1 = getpos(xpos + tilesize * (z->cpos.x + 0) + add_x, ypos + tilesize * (z->cpos.y + 0) + add_y, z->bottom);
		const auto a10_1 = getpos(xpos + tilesize * (z->cpos.x + 1) + add_x, ypos + tilesize * (z->cpos.y + 0) + add_y, z->bottom);
		const auto a01_1 = getpos(xpos + tilesize * (z->cpos.x + 0) + add_x, ypos + tilesize * (z->cpos.y + 1) + add_y, z->bottom);
		const auto a11_1 = getpos(xpos + tilesize * (z->cpos.x + 1) + add_x, ypos + tilesize * (z->cpos.y + 1) + add_y, z->bottom);
		const auto a00_0 = getpos(xpos + tilesize * (z->cpos.x + 0), ypos + tilesize * (z->cpos.y + 0), z->bottom);//◤
		const auto a10_0 = getpos(xpos + tilesize * (z->cpos.x + 1), ypos + tilesize * (z->cpos.y + 0), z->bottom);//◥
		const auto a01_0 = getpos(xpos + tilesize * (z->cpos.x + 0), ypos + tilesize * (z->cpos.y + 1), z->bottom);//◣
		const auto a11_0 = getpos(xpos + tilesize * (z->cpos.x + 1), ypos + tilesize * (z->cpos.y + 1), z->bottom);//◢

		SetDrawBright(0, 0, 0);
		if (UorL <= 20) {
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
				case 20:
					if (a00_0.y < a01_1.y) {
						DrawModiGraph(a01_1.x, a01_1.y, a11_1.x, a11_1.y, a10_0.x, a10_0.y, a00_0.x, a00_0.y, z->floorhandle->get(), TRUE);
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
				case 17:
					if (a00_0.x < a11_1.x) {
						DrawModiGraph(a11_1.x, a11_1.y, a10_1.x, a10_1.y, a00_0.x, a00_0.y, a01_0.x, a01_0.y, z->floorhandle->get(), TRUE);
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
				case 6:
					if (a01_0.y > a10_1.y) {
						DrawModiGraph(a00_1.x, a00_1.y, a10_1.x, a10_1.y, a11_0.x, a11_0.y, a01_0.x, a01_0.y, z->floorhandle->get(), TRUE);
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
				case 11:
					if (a10_0.x > a01_1.x) {
						DrawModiGraph(a01_1.x, a01_1.y, a00_1.x, a00_1.y, a10_0.x, a10_0.y, a11_0.x, a11_0.y, z->floorhandle->get(), TRUE);
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
	//柱の影描画
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

		case 4://上
			draw_wall_shadow(20, z);	//縦(上)//4
			draw_wall_shadow(5, z);		//横(左)
			draw_wall_shadow(2, z);		//縦(下)
			draw_wall_shadow(7, z);		//横(右)
			break;
		case 5://左
			draw_wall_shadow(8, z);		//縦(上)//8
			draw_wall_shadow(17, z);	//横(左)
			draw_wall_shadow(10, z);	//縦(下)
			draw_wall_shadow(3, z);		//横(右)
			break;
		case 6://下
			draw_wall_shadow(0, z);		//縦(上)//12
			draw_wall_shadow(13, z);	//横(左)
			draw_wall_shadow(6, z);		//縦(下)
			draw_wall_shadow(15, z);	//横(右)
			break;
		case 7://右
			draw_wall_shadow(16, z);	//縦(上)//16
			draw_wall_shadow(1, z);		//横(左)
			draw_wall_shadow(18, z);	//縦(下)
			draw_wall_shadow(11, z);	//横(右)
			break;

		default://柱
			draw_wall_shadow(0, z);		//縦(上)
			draw_wall_shadow(1, z);		//横(左)
			draw_wall_shadow(2, z);		//縦(下)
			draw_wall_shadow(3, z);		//横(右)
			break;
		}
	}
	//人の影描画
	inline void drw_human_shadow(con* z) {
		SetDrawBright(0,0,0);
		for (auto& pl : human) {
			for (auto& g : pl.sort) {
				auto q = getpos(pl.bone[g.first].xp + pl.pos.x, pl.bone[g.first].yp + pl.pos.y, z->bottom);
				if (q.x >= z->floor[0].x && q.y >= z->floor[0].y && q.x <= z->floor[3].x && q.y <= z->floor[3].y) {
					auto p = getpos(
						pl.bone[g.first].xp + pl.pos.x + int(float(pl.bone[g.first].zp - pl.sort[0].second)*sin(light_yrad)),
						pl.bone[g.first].yp + pl.pos.y + int(float(pl.bone[g.first].zp - pl.sort[0].second)*cos(light_yrad)),
						z->hight);
					DrawRotaGraph3(p.x, p.y, 16, 16,
						1.0 * dispx / 1920 * (camhigh + (pl.bone[g.first].zp - pl.sort[0].second + z->hight) * 5) / camhigh,
						1.0 * dispx / 1920 * (camhigh + (pl.bone[g.first].zp - pl.sort[0].second + z->hight) * 5) / camhigh,
						double(pl.bone[g.first].yrad) + double(pl.bone[g.first].yr), pl.Graphs[g.first].get(), TRUE);

				}
			}
		}
		SetDrawBright(255, 255, 255);
	}
	//カメラ座標設定
	inline void set_cam(int sx, int sy) {
		xpos = sx;
		ypos = sy;
		put_map();
		return;
	}
	//人のデータ作成
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
					n.back().xdist = -12.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = 0.0f;
				}
				n.resize(n.size() + 1);
				n.back().parent = 15;
				n.back().xdist = 0.0f;
				n.back().ydist = 0.0f;
				n.back().zdist = -3.0f;
				{//右腕
					n.resize(n.size() + 1);
					n.back().parent = 5;
					n.back().xdist = 12.0f;
					n.back().ydist = 0.0f;
					n.back().zdist = 0.0f;

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
			for (int i = 0; i < 2; i++) {
				z.anime.resize(z.anime.size() + 1);
				const auto mdata = FileRead_open(("data/Char/Mot/"s+std::to_string(i)+".mot").c_str(), FALSE);
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
		}
		{//キャラバイナリ
			std::fstream file;
			file.open("data/Char/1.dat", std::ios::binary | std::ios::in);
			do {
				z.bone.resize(z.bone.size() + 1);
				file.read((char*)&z.bone.back(), sizeof(z.bone.back()));
			} while (!file.eof());
			z.bone.pop_back();
			file.close();
		}
		z.pos.x = xp;
		z.pos.y = yp;
	}
	//人と壁の判定
	inline void hit_wall(pos2D *pos, pos2D& old) {
		pos->x = std::clamp(pos->x, tilesize / 4, tilesize * int(zcon.size())- tilesize / 4);
		pos->y = std::clamp(pos->y, tilesize / 4, tilesize * int(zcon.back().size())- tilesize / 4);

		for (auto& c : zcon) {
			for (auto& z : c) {
				if (z.hight != z.bottom) {
					const auto x0 = tilesize * z.cpos.x - tilesize / 2;
					const auto y0 = tilesize * z.cpos.y - tilesize / 2;
					const auto x1 = tilesize * z.cpos.x + tilesize * 3 / 2;
					const auto y1 = tilesize * z.cpos.y + tilesize * 3 / 2;
					//*
					auto a = *pos - old;
					{
						pos2D s = { x0,y0 };
						pos2D p = { x0 - s.x ,y1 - s.y };
						if (ColSeg(old, a, s, p)) {
							auto pr = p.hydist();
							pos->x -= -p.y * p.cross(&a) / pr;
							pos->y -= p.x * p.cross(&a) / pr;
							break;
						}
					}
					{
						pos2D s = { x1,y0 };
						pos2D p = { x0 - s.x ,y0 - s.y };
						if (ColSeg(old, a, s, p)) {
							auto pr = p.hydist();
							pos->x -= -p.y * p.cross(&a) / pr;
							pos->y -= p.x * p.cross(&a) / pr;
							break;
						}
					}
					{
						pos2D s = { x1 ,y1 };
						pos2D p = { x1 - s.x ,y0 - s.y };
						if (ColSeg(old, a, s, p)) {
							auto pr = p.hydist();
							pos->x -= -p.y * p.cross(&a) / pr;
							pos->y -= p.x * p.cross(&a) / pr;
							break;
						}
					}
					{
						pos2D s = { x0,y1 };
						pos2D p = { x1 - s.x ,y1 - s.y };
						if (ColSeg(old, a, s, p)) {
							auto pr = p.hydist();
							pos->x -= -p.y * p.cross(&a) / pr;
							pos->y -= p.x * p.cross(&a) / pr;
							break;
						}
					}
					//*/
				}
			}
		}
	}
	//人の移動処理
	inline void move_human(int *x_pos, int *y_pos) {
		for (int i = 0; i < human.size(); i++) {
			human[i].vtmp = human[i].pos;
			if (i != player_id) {
				//todo : cpu
				//human[i].pos.x = 100*i;
				//human[i].pos.y = 100*i;
			}
			else{
				//自機の移動
				human[player_id].pos.x = -*x_pos;
				human[player_id].pos.y = -*y_pos;
			}
			hit_wall(&human[i].pos, human[i].vtmp);
			if (i == player_id) {
				*x_pos = -human[player_id].pos.x;
				*y_pos = -human[player_id].pos.y;
			}
			human[i].vtmp -= human[i].pos;
		}
	}
	//柱設置
	inline void set_drw_rect(int px, int py, int bottom, int hight, int walldir, int floordir, int UorL = -1) {//三角柱
		auto& z = zcon[px][py];
		z.cpos.x = px;
		z.cpos.y = py;
		z.bottom = bottom;
		z.hight = hight;

		z.top[0] = getpos(xpos + tilesize * (z.cpos.x + 0), ypos + tilesize * (z.cpos.y + 0), z.hight);
		z.top[1] = getpos(xpos + tilesize * (z.cpos.x + 1), ypos + tilesize * (z.cpos.y + 0), z.hight);
		z.top[2] = getpos(xpos + tilesize * (z.cpos.x + 0), ypos + tilesize * (z.cpos.y + 1), z.hight);
		z.top[3] = getpos(xpos + tilesize * (z.cpos.x + 1), ypos + tilesize * (z.cpos.y + 1), z.hight);

		z.floor[0] = getpos(xpos + tilesize * (z.cpos.x + 0), ypos + tilesize * (z.cpos.y + 0), z.bottom);
		z.floor[1] = getpos(xpos + tilesize * (z.cpos.x + 1), ypos + tilesize * (z.cpos.y + 0), z.bottom);
		z.floor[2] = getpos(xpos + tilesize * (z.cpos.x + 0), ypos + tilesize * (z.cpos.y + 1), z.bottom);
		z.floor[3] = getpos(xpos + tilesize * (z.cpos.x + 1), ypos + tilesize * (z.cpos.y + 1), z.bottom);

		z.wallhandle = &walls[walldir];
		z.floorhandle = &floors[floordir];
		z.use = UorL;
		//z.use = std::clamp(UorL, -1, 4);
	}
	//人の描画用意
	inline void ready_player(void) {
		for (auto& pl : human) {
			for (auto& g : pl.bone) {
				g.edit = false;
			}

			//ここでアニメーション
			{
				auto& x = pl.anime[pl.animesel];
				auto& a = x[pl.animeframe];
				auto& b = x[(pl.animeframe + 1) % int(x.size())];
				if (pl.animetime < a.time) {
					for (int z = 0; z < pl.bone.size(); z++) {
						pl.bone[z].xrad = a.bone[z].xrad + (b.bone[z].xrad - a.bone[z].xrad)*pl.animetime / a.time;
						pl.bone[z].yrad = a.bone[z].yrad + (b.bone[z].yrad - a.bone[z].yrad)*pl.animetime / a.time;
						pl.bone[z].zrad = a.bone[z].zrad + (b.bone[z].zrad - a.bone[z].zrad)*pl.animetime / a.time;
					}
					pl.animetime++;
				}
				else {
					pl.animeframe = (pl.animeframe + 1) % int(x.size());
					pl.animetime = 0;
				}
			}
			auto o = pl.animesel;

			if (pl.vtmp.x == 0 && pl.vtmp.y == 0) {
				pl.animesel = 1;
			}
			else {
				pl.animesel = 0;
			}

			if (o != pl.animesel) {
				pl.animeframe = 0;
				pl.animetime = 0;
			}
			//
			if (pl.vtmp.x != 0 || pl.vtmp.y != 0) {
				pl.vec = pl.vtmp;
			}
			{
				//移動方向に向く
				auto b = sqrtf(float(pl.vec.hydist()));
				auto q = (float(pl.vec.x)*cos(pl.yrad) - float(pl.vec.y)* -sin(pl.yrad)) / b;
				if (q > sin(deg2rad(10))) {
					pl.yrad += deg2rad(-5);
				}else if (q < sin(deg2rad(-10))) {
					pl.yrad += deg2rad(5);
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
	//一気に描画
	inline void put_drw(void) {
		ready_player();

		const auto limmin = getpos(-dispx * 3 / 4, -dispy * 3 / 4, 0);
		const auto cam = getpos(0, 0, 0);
		const auto limmax = getpos(dispx * 3 / 4, dispy * 3 / 4, 0);

		//light_yrad += deg2rad(1);
		//DRAW
		SetDrawScreen(screen);
		ClearDrawScreen();
		//地面
		for (auto& c : zcon) {
			for (auto& z : c) {
				if (z.top[0].x <= cam.x && z.top[0].y <= cam.y && z.floor[3].y >= limmin.y  && z.floor[3].x >= limmin.x && z.bottom == z.hight) {
					drw_rect(&z);
				}
			}
			for (int y = int(c.size()) - 1; y >= 0; --y) {
				auto& z = c[y];
				if (z.top[0].x <= cam.x && z.top[3].y >= cam.y && z.floor[0].y <= limmax.y  && z.floor[3].x >= limmin.x && z.bottom == z.hight) {
					drw_rect(&z);
				}
			}
		}
		for (int x = int(zcon.size()) - 1; x >= 0; --x) {
			for (auto& z : zcon[x]) {
				if (z.top[3].x >= cam.x && z.top[0].y <= cam.y && z.floor[3].y >= limmin.y && z.floor[0].x <= limmax.x && z.bottom == z.hight) {
					drw_rect(&z);
				}
			}
			for (int y = int(zcon[x].size()) - 1; y >= 0; --y) {
				auto& z = zcon[x][y];
				if (z.top[3].x >= cam.x && z.top[3].y >= cam.y&& z.floor[0].y <= limmax.y  && z.floor[0].x <= limmax.x && z.bottom == z.hight) {
					drw_rect(&z);
				}

			}
		}
		//影
		SetDrawScreen(shadow_graph);
		ClearDrawScreen();
		for (auto& c : zcon) {
			for (auto& z : c) {
				if (z.floor[0].y <= limmax.y  && z.floor[0].x <= limmax.x && z.floor[3].y >= limmin.y && z.floor[3].x >= limmin.x) {
					if (z.bottom != z.hight) {
						drw_rect_shadow(&z);
					}
					drw_human_shadow(&z);
				}
			}
		}
		SetDrawScreen(screen);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawGraph(0, 0, shadow_graph, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//上階
		for (auto& c : zcon) {
			for (auto& z : c) {
				if (z.top[0].x <= cam.x && z.top[0].y <= cam.y && z.floor[3].y >= limmin.y && z.floor[3].x >= limmin.x) {
					if (z.bottom != z.hight) {
						drw_rect(&z);
					}
					drw_human(&z);
				}
			}
			for (int y = int(c.size()) - 1; y >= 0; --y) {
				auto& z = c[y];
				if (z.top[0].x <= cam.x && z.top[3].y >= cam.y && z.floor[0].y <= limmax.y  && z.floor[3].x >= limmin.x) {
					if (z.bottom != z.hight) {
						drw_rect(&z);
					}
					drw_human(&z);
				}
			}
		}
		for (int x = int(zcon.size()) - 1; x >= 0; --x) {
			for (auto& z : zcon[x]) {
				if (z.top[3].x >= cam.x && z.top[0].y <= cam.y && z.floor[3].y >= limmin.y && z.floor[0].x <= limmax.x) {
					if (z.bottom != z.hight) {
						drw_rect(&z);
					}
					drw_human(&z);
				}
			}
			for (int y = int(zcon[x].size()) - 1; y >= 0; --y) {
				auto& z = zcon[x][y];
				if (z.top[3].x >= cam.x && z.top[3].y >= cam.y&& z.floor[0].y <= limmax.y  && z.floor[0].x <= limmax.x){
					if (z.bottom != z.hight) {
						drw_rect(&z);
					}
					drw_human(&z);
				}
			}
		}

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		return;
	}
	//出力
	inline void out_draw(void) {
		DrawGraph(0, 0, screen, TRUE);
	}
};

//デバッグ描画
class DeBuG {
private:
	float deb[60][6 + 1] = { 0.f };
	LONGLONG waypoint=0;
	float waydeb[6] = {0.f};
	size_t seldeb=0;
	FontHandle font;
public:
	DeBuG(void);
	void put_way(void);
	void end_way(void);
	void debug(void);
};

#endif
