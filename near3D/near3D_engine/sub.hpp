#pragma once
#include "DXLib_ref/DXLib_ref.h"

//フォントプール
class FontPool {
public:
	class Fonthave {
		int size = 0;
		FontHandle handle;
	public:
		const auto& Get_size(void)const noexcept { return size; }
		const auto& Get_handle(void)const noexcept { return handle; }
		void Set(int siz_t) {
			this->size = siz_t;
			SetUseASyncLoadFlag(TRUE);
			this->handle = FontHandle::Create(siz_t, DX_FONTTYPE_NORMAL, -1, 2);
			//this->handle = FontHandle::Create(siz_t, DX_FONTTYPE_EDGE, -1, 2);
			SetUseASyncLoadFlag(FALSE);
		}
	};
private:
	std::vector<Fonthave> havehandle;
	size_t Add(int siz_t) {
		for (auto& h : this->havehandle) {
			if (h.Get_size() == siz_t) {
				return &h - &this->havehandle.front();
			}
		}
		this->havehandle.resize(this->havehandle.size() + 1);
		this->havehandle.back().Set(siz_t);
		return this->havehandle.size() - 1;
	}
public:
	Fonthave& Get(int siz_t) { return this->havehandle[Add(siz_t)]; }
};
FontPool Fonts;

typedef std::pair<size_t, int> pairs;
void Grad_Box(int x1, int y1, int x2, int y2,
	const unsigned char r1 = 255, const unsigned char g1 = 255, const unsigned char b1 = 255,
	const unsigned char r2 = 255, const unsigned char g2 = 255, const unsigned char b2 = 255,
	const unsigned char UorL = 255
) {

	VERTEX2D Vertex[6];
	// 左上の頂点の情報をセット
	Vertex[0].pos.x = (float)x1;
	Vertex[0].pos.y = (float)y1;
	Vertex[0].pos.z = 0.0f;
	Vertex[0].rhw = 1.0f;
	Vertex[0].dif.r = r1;
	Vertex[0].dif.g = g1;
	Vertex[0].dif.b = b1;
	Vertex[0].dif.a = 255;
	Vertex[0].u = 0.0f;
	Vertex[0].v = 0.0f;

	// 右上の頂点の情報をセット
	Vertex[1].pos.x = (float)x2;
	Vertex[1].pos.y = (float)y1;
	Vertex[1].pos.z = 0.0f;
	Vertex[1].rhw = 1.0f;
	Vertex[1].dif.r = r2;
	Vertex[1].dif.g = g2;
	Vertex[1].dif.b = b2;
	Vertex[1].dif.a = 255;
	Vertex[1].u = 0.0f;
	Vertex[1].v = 0.0f;

	Vertex[5] = Vertex[1];

	// 左下の頂点の情報をセット
	Vertex[2].pos.x = (float)x1;
	Vertex[2].pos.y = (float)y2;
	Vertex[2].pos.z = 0.0f;
	Vertex[2].rhw = 1.0f;
	Vertex[2].dif.r = r1;
	Vertex[2].dif.g = g1;
	Vertex[2].dif.b = b1;
	Vertex[2].dif.a = 255;
	Vertex[2].u = 0.0f;
	Vertex[2].v = 0.0f;

	Vertex[4] = Vertex[2];

	// 右下の頂点の情報をセット
	Vertex[3].pos.x = (float)x2;
	Vertex[3].pos.y = (float)y2;
	Vertex[3].pos.z = 0.0f;
	Vertex[3].rhw = 1.0f;
	Vertex[3].dif.a = 255;
	Vertex[3].u = 0.0f;
	Vertex[3].v = 0.0f;

	switch (UorL)
	{
	case 0 + 3:
		// 左上の頂点の情報をセット
		Vertex[0].dif.r = r1;
		Vertex[0].dif.g = g1;
		Vertex[0].dif.b = b1;
		// 右上の頂点の情報をセット
		Vertex[1].dif.r = r2;
		Vertex[1].dif.g = g2;
		Vertex[1].dif.b = b2;
		Vertex[5] = Vertex[1];
		// 左下の頂点の情報をセット
		Vertex[2].dif.r = r1;
		Vertex[2].dif.g = g1;
		Vertex[2].dif.b = b1;
		Vertex[4] = Vertex[2];
		// 右下の頂点の情報をセット
		Vertex[3].dif.r = r2;
		Vertex[3].dif.g = g2;
		Vertex[3].dif.b = b2;
		break;
	case 1 + 1:
		// 左上の頂点の情報をセット
		Vertex[0].dif.r = r1;
		Vertex[0].dif.g = g1;
		Vertex[0].dif.b = b1;
		// 右上の頂点の情報をセット
		Vertex[1].dif.r = r1;
		Vertex[1].dif.g = g1;
		Vertex[1].dif.b = b1;
		Vertex[5] = Vertex[1];
		// 左下の頂点の情報をセット
		Vertex[2].dif.r = r2;
		Vertex[2].dif.g = g2;
		Vertex[2].dif.b = b2;
		Vertex[4] = Vertex[2];
		// 右下の頂点の情報をセット
		Vertex[3].dif.r = r2;
		Vertex[3].dif.g = g2;
		Vertex[3].dif.b = b2;
		break;
	case 2 - 1:
		// 左上の頂点の情報をセット
		Vertex[0].dif.r = r2;
		Vertex[0].dif.g = g2;
		Vertex[0].dif.b = b2;
		// 右上の頂点の情報をセット
		Vertex[1].dif.r = r1;
		Vertex[1].dif.g = g1;
		Vertex[1].dif.b = b1;
		Vertex[5] = Vertex[1];
		// 左下の頂点の情報をセット
		Vertex[2].dif.r = r2;
		Vertex[2].dif.g = g2;
		Vertex[2].dif.b = b2;
		Vertex[4] = Vertex[2];
		// 右下の頂点の情報をセット
		Vertex[3].dif.r = r1;
		Vertex[3].dif.g = g1;
		Vertex[3].dif.b = b1;
		break;
	case 3 - 3:
		// 左上の頂点の情報をセット
		Vertex[0].dif.r = r2;
		Vertex[0].dif.g = g2;
		Vertex[0].dif.b = b2;
		// 右上の頂点の情報をセット
		Vertex[1].dif.r = r2;
		Vertex[1].dif.g = g2;
		Vertex[1].dif.b = b2;
		Vertex[5] = Vertex[1];
		// 左下の頂点の情報をセット
		Vertex[2].dif.r = r1;
		Vertex[2].dif.g = g1;
		Vertex[2].dif.b = b1;
		Vertex[4] = Vertex[2];
		// 右下の頂点の情報をセット
		Vertex[3].dif.r = r1;
		Vertex[3].dif.g = g1;
		Vertex[3].dif.b = b1;
		break;
	default:
		// 左上の頂点の情報をセット
		Vertex[0].dif.r = r1;
		Vertex[0].dif.g = g1;
		Vertex[0].dif.b = b1;
		// 右上の頂点の情報をセット
		Vertex[1].dif.r = r1;
		Vertex[1].dif.g = g1;
		Vertex[1].dif.b = b1;
		Vertex[5] = Vertex[1];
		// 左下の頂点の情報をセット
		Vertex[2].dif.r = r1;
		Vertex[2].dif.g = g1;
		Vertex[2].dif.b = b1;
		Vertex[4] = Vertex[2];
		// 右下の頂点の情報をセット
		Vertex[3].dif.r = r1;
		Vertex[3].dif.g = g1;
		Vertex[3].dif.b = b1;
		break;
	}
	// ポリゴンを2個描画
	DrawPolygon2D(Vertex, 2, DX_NONE_GRAPH, FALSE);
}
int Bright_buf = -1;
void Set_Bright(int p) {
	if (p != Bright_buf) {
		SetDrawBright(p, p, p);
		Bright_buf = p;
	}
}
const int camhigh = 192 * 2;//カメラの高さ
const int tilesize = 128;//タイル一つ一つのサイズ
class Draw {
public:
	//2Dベクトル関連
	class pos2D {
	public:
		int x = 0;
		int y = 0;
		//設定する
		void set(int m_x, int m_y)noexcept {
			this->x = m_x;
			this->y = m_y;
		}
		//加算した値を返す
		const pos2D operator+(pos2D o) const noexcept {
			pos2D temp;
			temp.x = this->x + o.x;
			temp.y = this->y + o.y;
			return temp;
		}
		//加算する
		pos2D& operator+=(pos2D o)noexcept {
			this->x += o.x;
			this->y += o.y;
			return *this;
		}
		//減算した値を変えす
		const pos2D operator-(pos2D o) const noexcept {
			pos2D temp;
			temp.x = this->x - o.x;
			temp.y = this->y - o.y;
			return temp;
		}
		//減算する
		pos2D& operator-=(pos2D o)noexcept {
			this->x -= o.x;
			this->y -= o.y;
			return *this;
		}
		//
		pos2D operator*(float o)noexcept {
			pos2D temp;
			temp.x = (int)((float)(this->x) * o);
			temp.y = (int)((float)(this->y) * o);
			return temp;
		}
		//距離の2乗を取得する
		const int hydist() const noexcept { return this->x * this->x + this->y * this->y; }
		// 内積
		const int dot(const pos2D& v2) const noexcept { return this->x * v2.x + this->y * v2.y; }
		// 外積
		const int cross(const pos2D& v2) const noexcept { return this->x * v2.y - this->y * v2.x; }
		//
		static pos2D Get(int xp, int yp) {
			pos2D p;
			p.set(xp, yp);
			return p;
		}
	};
private:
	class con {
	public:
		std::array<pos2D, 4> top;
		std::array<pos2D, 4> floor;
		std::array<pos2D, 4> zero;
		unsigned char use;// rect = -1 else prism = 0~3,4~7
		pos2D cpos;
		int bottom, hight;
		bool is_wall = false;
		GraphHandle* wallhandle;
		GraphHandle* floorhandle;

		const bool Xin(int x) const noexcept { return x >= top[0].x && x <= top[3].x; }
		const bool Yin(int y) const noexcept { return y >= top[0].y && y <= top[3].y; }
	};
	class Status {
	public:
		pos2D pos;
		int bottom = 0, hight = 0;
		bool is_wall = false;
		int wall_id = 0, floor_id = 0;
		unsigned char dir = 0;
	};
	class maps {
	public:
		int plx, ply;
		char wall_name[MAX_PATH];
		char floor_name[MAX_PATH];
		float light_yrad;
	};
	class Player_Info {
	public:
		pos2D pos_p;
	};
	class shadow_handle {
	public:
		bool isupdate = true;
		GraphHandle handle;
	};
	class Humans {
		class Bonesdata {
		public:
			int parent;
			int xp, yp, zp;
			float xr, yr, zr;
			float xrad, yrad, zrad;
			float xdist, ydist, zdist;
			bool edit;
			void Leap_Rad(const Bonesdata& now, const Bonesdata& next, float per) {
				this->xrad = now.xrad + (next.xrad - now.xrad) * per;
				this->yrad = now.yrad + (next.yrad - now.yrad) * per;
				this->zrad = now.zrad + (next.zrad - now.zrad) * per;
			}
		};
		class Animesdata {
		public:
			int time = 0;
			std::vector<Bonesdata> bone;
			void SetBoneData(int sel, std::string_view ttt, float rad) {
				if (ttt.find("x") != std::string::npos) {
					this->bone[sel].xrad = rad;
				}
				else if (ttt.find("y") != std::string::npos) {
					this->bone[sel].yrad = rad;
				}
				else if (ttt.find("z") != std::string::npos) {
					this->bone[sel].zrad = rad;
				}
			}
		};
		class foots {
		public:
			int ID;
			int xp, yp, zp;
			float yr;
			float yrad;
			float Time = 0.f;
			float MaxTime = 5.f;
			void Set(Bonesdata& foot,const pos2D& pos,int ID_t) {
				ID = ID_t;
				xp = foot.xp + pos.x;
				yp = foot.yp + pos.y;
				zp = foot.zp;
				yr = foot.yr;
				yrad = foot.yrad;
				Time = MaxTime;
			}
		};
		std::vector<GraphHandle> Graphs;
		std::vector<Bonesdata> bone;
		std::vector<pairs> sort;
		std::vector<foots> foot_v;
		int prev_foot = -1;
		std::vector<bool> draw_ok = { false };
		bool draw_end = false;
		float yrad = 0.f;
		int animeframe = 0;
		int animetime = 1;
		int animesel = 0;
	public:
		bool isStand = true;
		bool standup = false;
		float yrad_aim = 0;
		std::vector<std::vector<Animesdata>> anime;
		pos2D pos;
		pos2D spawnpos;
		pos2D vec_buf;
		pos2D vec;
		int hight = 0;
		float foottime = 0;
		void Set(pos2D camerapos) {
			for (auto& g : this->bone) {
				g.edit = false;
			}

			//ここでアニメーション
			{
				auto& x = this->anime[this->animesel];
				auto& now = x[this->animeframe];
				auto& next = x[(this->animeframe + 1) % (int)(x.size())];
				if (this->animetime < now.time) {
					for (int z = 0; z < this->bone.size(); z++) {
						this->bone[z].Leap_Rad(now.bone[z], next.bone[z], (float)this->animetime / (float)now.time);
					}
					this->animetime++;
				}
				else {
					++this->animeframe %= (int)(x.size());
					this->animetime = 0;
				}
			}
			{
				auto o = this->animesel;
				if (isStand) {
					this->animesel = 1;//立ち
					if (this->vec_buf.x != 0 || this->vec_buf.y != 0) {
						if (this->vec_buf.hydist() < (3.f * 1.5f) * (3.f * 1.5f)) {
							this->animesel = 0;//歩き
						}
						else {
							this->animesel = 2;//歩き
						}
					}
				}
				else {
					//伏せ
					this->animesel = 4;//立ち
					if (this->vec_buf.x != 0 || this->vec_buf.y != 0) {
						if (this->vec_buf.hydist() < (3.f * 1.5f) * (3.f * 1.5f)) {
							this->animesel = 3;//歩き
						}
						else {
							//立ち上がる
							isStand = true;
							standup = true;
						}
					}
				}

				this->animesel = 5;//立ち

				if (o != this->animesel) {
					this->animeframe = 0;
					this->animetime = 0;
				}
			}
			//
			if (this->vec_buf.x != 0 || this->vec_buf.y != 0) {
				this->vec = this->vec_buf;
			}
			{
				//移動方向に向く
				auto b = sqrtf(float(this->vec.hydist()));
				auto q = (float(this->vec.x) * cos(this->yrad) - float(this->vec.y) * -sin(this->yrad)) / b;
				if (q > sin(deg2rad(10))) {
					this->yrad += deg2rad(-5);
				}
				else if (q < sin(deg2rad(-10))) {
					this->yrad += deg2rad(5);
				}
				//真後ろ振り向き
				if ((float(this->vec.x) * -sin(this->yrad) + float(this->vec.y) * cos(this->yrad)) / b <= -0.5) {
					this->yrad += deg2rad(10);
				}
			}
			//座標指定
			bool next = false;
			do {
				next = false;
				for (auto& z : this->bone) {
					auto p = z.parent;
					if (!z.edit) {
						if (p == -1) {
							z.xp = camerapos.x;
							z.yp = camerapos.y;
							z.zp = 0;
							z.xr = z.xrad;
							z.yr = z.yrad + this->yrad;
							z.yr += yrad_aim * 2;
							z.zr = z.zrad;
							z.edit = true;
						}
						else {
							if (this->bone[p].edit) {
								const float xd = z.xdist * y_r(tilesize) / 32 * 5 / 6;
								const float yd = z.ydist * y_r(tilesize) / 32 * 10;
								const float zd = z.zdist * y_r(tilesize) / 32;
								const float zd2 = z.zdist * 540 * 2 / desky;

								z.xr = this->bone[p].xrad + this->bone[p].xr;
								z.yr = this->bone[p].yrad + this->bone[p].yr;
								if (p == 15) {
									z.yr -= yrad_aim;
								}
								if (p == 16) {
									z.yr -= yrad_aim;
								}
								z.zr = this->bone[p].zrad + this->bone[p].zr;

								float y1 = cos(z.xr) * yd + sin(z.xr) * zd;
								float z1 = cos(z.xr) * zd2 - sin(z.xr) * yd;
								float x2 = cos(z.zr) * xd + sin(z.zr) * z1;

								z.xp = this->bone[p].xp + (int)(cos(z.yr) * x2 - sin(z.yr) * y1);
								z.yp = this->bone[p].yp + (int)(cos(z.yr) * y1 + sin(z.yr) * x2);
								z.zp = this->bone[p].zp + (int)(cos(z.zr) * z1 - sin(z.zr) * xd);
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
			for (int i = 0; i < this->sort.size(); i++) {
				this->sort[i].first = i;
				this->sort[i].second = this->bone[i].zp;
			}
			std::sort(this->sort.begin(), this->sort.end(), [](const pairs& x, const pairs& y) { return x.second < y.second; });
			//一番低い場所に跡を置く
			if (foottime>=1.f/3.f && prev_foot!= this->sort.front().first) {
				this->foot_v.resize(this->foot_v.size() + 1);
				this->foot_v.back().Set(this->bone[this->sort.front().first], this->pos - camerapos, this->sort.front().first);
				prev_foot = this->sort.front().first;
				foottime = 0.f;
			}
			foottime += 1.f / FPS;
			//*
			bool tt = true;
			while (true) {
				tt = true;
				for(int i=0;i<foot_v.size();i++) {
					foot_v[i].Time -= 1.f / FPS;
					if (foot_v[i].Time < 0.f) {
						foot_v[i] = foot_v.back();
						foot_v.pop_back();
						tt = false;
						break;
					}
				}
				if (tt) { break; }
			}
			//*/
		}
		void First(int xp, int yp) {
			using namespace std::literals;
			GraphHandle::LoadDiv("data/Char/1.bmp", 33, 11, 3, 32, 32, &this->Graphs);
			this->sort.resize(this->Graphs.size());
			this->draw_ok.resize(this->Graphs.size());
			//*
			{//キャラバイナリ書き込み
				std::vector<Bonesdata> n_t;
				n_t.clear();
				{
					{//左腕
						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 1;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -2.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 2;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -5.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 3;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -4.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 4;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -5.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 5;
						n_t.back().xdist = -9.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = 0.0f;
					}
					n_t.resize(n_t.size() + 1);
					n_t.back().parent = 15;
					n_t.back().xdist = 0.0f;
					n_t.back().ydist = 0.0f;
					n_t.back().zdist = -3.0f;
					{//右腕
						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 5;
						n_t.back().xdist = 9.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = 0.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 6;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -5.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 7;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -4.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 8;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -5.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 9;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -2.0f;
					}
				}
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);

				n_t.resize(n_t.size() + 1);
				n_t.back().parent = -1;
				n_t.back().xdist = 0.0f;
				n_t.back().ydist = 0.0f;
				n_t.back().zdist = 0.0f;

				n_t.resize(n_t.size() + 1);
				n_t.back().parent = 5;
				n_t.back().xdist = 0.0f;
				n_t.back().ydist = 0.0f;
				n_t.back().zdist = -3.0f;

				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);

				{
					{
						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 23;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -2.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 24;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -6.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 25;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -5.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 26;
						n_t.back().xdist = 2.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -4.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 27;
						n_t.back().xdist = -5.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -3.0f;
					}
					n_t.resize(n_t.size() + 1);
					n_t.back().parent = 16;
					n_t.back().xdist = 0.0f;
					n_t.back().ydist = 0.0f;
					n_t.back().zdist = -3.0f;
					{
						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 27;
						n_t.back().xdist = 5.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -3.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 28;
						n_t.back().xdist = -2.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -4.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 29;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -5.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 30;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -6.0f;

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 31;
						n_t.back().xdist = 0.0f;
						n_t.back().ydist = 0.0f;
						n_t.back().zdist = -2.0f;
					}
				}

				std::fstream file;
				// 書き込む
				file.open("data/Char/1.dat", std::ios::binary | std::ios::out);
				for (auto& m : n_t) {
					file.write((char*)&m, sizeof(m));
				}
				file.close();
			}
			//*/
			//モーションテキスト(直に打ち込めるように)
			for (int i = 0; i < 6; i++) {
				this->anime.resize(this->anime.size() + 1);
				const auto mdata = FileRead_open(("data/Char/Mot/"s + std::to_string(i) + ".mot").c_str(), FALSE);
				{
					do {
						int tmp;
						std::string ttt = getparams::getcmd(mdata, &tmp);
						if (ttt.find("frame") != std::string::npos) {
							this->anime.back().resize(this->anime.back().size() + 1);
							this->anime.back().back().bone.resize(33);//todo
							this->anime.back().back().time = tmp;
						}
						else if (ttt.find("left_arm2_") != std::string::npos) {
							this->anime.back().back().SetBoneData(3, ttt, deg2rad(tmp));
						}
						else if (ttt.find("left_arm_") != std::string::npos) {
							this->anime.back().back().SetBoneData(4, ttt, deg2rad(tmp));
						}
						else if (ttt.find("Body_Top_") != std::string::npos) {
							this->anime.back().back().SetBoneData(5, ttt, deg2rad(tmp));
						}
						else if (ttt.find("right_arm_") != std::string::npos) {
							this->anime.back().back().SetBoneData(6, ttt, deg2rad(tmp));
						}
						else if (ttt.find("right_arm2_") != std::string::npos) {
							this->anime.back().back().SetBoneData(7, ttt, deg2rad(tmp));
						}
						else if (ttt.find("Body_Head_") != std::string::npos) {
							this->anime.back().back().SetBoneData(15, ttt, deg2rad(tmp));
						}
						else if (ttt.find("Body_Mid_") != std::string::npos) {
							this->anime.back().back().SetBoneData(16, ttt, deg2rad(tmp));
						}
						else if (ttt.find("left_leg3_") != std::string::npos) {
							this->anime.back().back().SetBoneData(22, ttt, deg2rad(tmp));
						}
						else if (ttt.find("left_leg2_") != std::string::npos) {
							this->anime.back().back().SetBoneData(24, ttt, deg2rad(tmp));
						}
						else if (ttt.find("left_leg_") != std::string::npos) {
							this->anime.back().back().SetBoneData(26, ttt, deg2rad(tmp));
						}
						else if (ttt.find("Body_Bottom_") != std::string::npos) {
							this->anime.back().back().SetBoneData(27, ttt, deg2rad(tmp));
						}
						else if (ttt.find("right_leg_") != std::string::npos) {
							this->anime.back().back().SetBoneData(28, ttt, deg2rad(tmp));
						}
						else if (ttt.find("right_leg2_") != std::string::npos) {
							this->anime.back().back().SetBoneData(30, ttt, deg2rad(tmp));
						}
						else if (ttt.find("right_leg3_") != std::string::npos) {
							this->anime.back().back().SetBoneData(32, ttt, deg2rad(tmp));
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
				do {
					this->bone.resize(this->bone.size() + 1);
					file.read((char*)&this->bone.back(), sizeof(this->bone.back()));
				} while (!file.eof());
				this->bone.pop_back();
				file.close();
			}
			this->spawnpos.set(xp, yp);
			this->pos = this->spawnpos;
		}
		void Reset() {
			std::fill<>(this->draw_ok.begin(), this->draw_ok.end(), false);
			this->draw_end = false;
		}
		void Set_Aim(int x, int y) {
			float rad = std::atan2f((float)x, (float)-y) - this->yrad;
			easing_set(&yrad_aim, std::clamp(std::atan2f(sin(rad), cos(rad)), deg2rad(-45), deg2rad(45)), 0.9f);
		}
		void Draw_Foot(const con& z, pos2D& camerapos) {
			for (auto& g : this->foot_v) {
				auto q = GetPos(camerapos.x + g.xp, camerapos.y + g.yp, z.hight);
				if (z.Xin(q.x) && z.Yin(q.y)) {
					DrawRotaGraphFast(q.x, q.y, float(z.hight + camhigh) / camhigh * y_r(tilesize) / 32 / 2, g.yrad + g.yr, this->Graphs[g.ID].get(), TRUE);
				}
			}
		}

		void Draw(const con& z) {
			if (!this->draw_end) {
				bool t = true;
				for (auto& g : this->sort) {
					auto& b = this->bone[g.first];
					auto zh = b.zp - this->sort[0].second;
					auto q = GetPos(b.xp + this->pos.x, b.yp + this->pos.y, z.hight);
					this->draw_ok[g.first] = this->draw_ok[g.first] || (z.Xin(q.x) && z.Yin(q.y));
					if (this->draw_ok[g.first]) {
						auto p = GetPos(b.xp + this->pos.x, b.yp + this->pos.y, z.hight + zh);
						DrawRotaGraphFast(p.x, p.y, float((z.hight + zh) + camhigh) / camhigh * y_r(tilesize) / 32 / 2, b.yrad + b.yr, this->Graphs[g.first].get(), TRUE);
					}
					if (!this->draw_ok[g.first]) { t = false; }
				}
				if (t) {
					this->draw_end = true;
				}
			}
		}
		void Draw_Shadow(const con& z, float light_yrad) {
			for (auto& g : this->sort) {
				auto& b = this->bone[g.first];
				auto zh = b.zp - this->sort[0].second;
				auto q = GetPos(b.xp + this->pos.x, b.yp + this->pos.y, z.hight + zh);
				if (z.Xin(q.x) && z.Yin(q.y)) {
					auto p = GetPos(b.xp + this->pos.x + (int)(float(zh) * sin(light_yrad)), b.yp + this->pos.y + (int)(float(zh) * cos(light_yrad)), z.hight);
					DrawRotaGraphFast(p.x, p.y, float((z.hight + zh) + camhigh) / camhigh * y_r(tilesize) / 32 / 2, b.yrad + b.yr, this->Graphs[g.first].get(), TRUE);
				}
			}
		}
	};
private:
	std::vector<Status> ans;
	std::vector<std::vector<con>> Tile;
	pos2D camerapos = { 0,0 };
	std::vector<Humans> human;
	size_t player_id = 0;
	std::array<pos2D, 8> shadow_pos;
	std::shared_ptr<DXDraw> DrawPts{ nullptr };			//引き継ぐ
	int mouse_x, mouse_y;
	float light_yrad = 0.0f;
	std::array<shadow_handle, 8> shadow_graph;
	std::vector<GraphHandle> walls;
	std::vector<GraphHandle> floors;
	GraphHandle res_floor;
	GraphHandle screen;
private:
	//線分衝突
	bool ColSeg(const pos2D& pos1, const pos2D& vec1, const pos2D& pos2, const pos2D& vec2) {
		auto Crs_v1_v2 = vec1.cross(vec2);
		if (Crs_v1_v2 == 0) { return false; }// 平行
		pos2D v = pos2 - pos1;
		const auto Crs_v_v1 = v.cross(vec1);
		const auto Crs_v_v2 = v.cross(vec2);
		return (!(Crs_v_v2 < 0 || Crs_v_v2 > Crs_v1_v2 || Crs_v_v1 < 0 || Crs_v_v1 > Crs_v1_v2));// 交差X
	}
	bool ColSeg2(pos2D* m_pos, pos2D& pos1, pos2D& p1, pos2D& pos2) {
		pos2D vec1 = *m_pos - pos1;
		pos2D vec2 = p1 - pos2;
		if (ColSeg(pos1, vec1, pos2, vec2)) {
			auto pp = vec2.cross(vec1);
			auto dist = vec2.hydist();
			m_pos->x -= -vec2.y * pp / dist;
			m_pos->y -= vec2.x * pp / dist;
			return true;
		}
		return false;
	}
	//座標変換
	static pos2D GetPos(int xp, int yp, int high) {
		if (camhigh > high) {
			return pos2D::Get(deskx / 2 + camhigh * xp / (camhigh - high), desky / 2 + camhigh * yp / (camhigh - high));
		}
		else {
			return pos2D::Get(deskx / 2 + camhigh * xp, deskx / 2 + camhigh * yp);
		}
	}
	//基幹描画
	void DrawModi_wrap(const pos2D& p1, const pos2D& p2, const pos2D& p3, const pos2D& p4, GraphHandle* g_handle) const noexcept { DrawModiGraph(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, g_handle->get(), TRUE); }
	void DrawExtend_wrap(const pos2D& p1, const pos2D& p2, GraphHandle* g_handle) const noexcept { g_handle->DrawExtendGraph(p1.x, p1.y, p2.x, p2.y, true); }
private:
	//mapエディター
	void change_tile(std::vector<Status>& n_s, Status& m, size_t& x_size_t, size_t& y_size_t) {
		if (!m.is_wall) {
			const size_t s = size_t(m.pos.x) + size_t(m.pos.y) * x_size_t;
			if (m.pos.x >= 1) {
				auto& t = n_s[s - 1];
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 5 : 255;
				}
			}
			if (m.pos.x <= x_size_t - 1 - 1) {
				auto& t = n_s[s + 1];
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 7 : 255;
				}
			}

			if (m.pos.y >= 1) {
				auto& t = n_s[s - x_size_t];
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 4 : 255;
				}
			}
			if (m.pos.y <= y_size_t - 1 - 1) {
				auto& t = n_s[s + x_size_t];
				//下
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 6 : 255;
				}
			}
		}
	}
	//壁描画
	void blend_shadow(const pos2D* p1, const pos2D* p2, int hight, GraphHandle* f_handle) {
		auto& s = shadow_graph[std::clamp<size_t>(hight / 8, 0, shadow_graph.size() - 1)];
		const int g = DerivationGraph(std::max(0, p1->x), std::max(0, p1->y), std::min(DrawPts->disp_x, p2->x - p1->x), std::min(DrawPts->disp_y, p2->y - p1->y), s.handle.get());
		s.isupdate = true;
		GraphBlendBlt(f_handle->get(), g, res_floor.get(), 128, DX_GRAPH_BLEND_NORMAL);
		DeleteGraph(g);
	}
	//壁
	void Draw_Wall(int UorL, const con& z) {
		if (UorL < 20 && z.hight != z.bottom) {
			{
				float rad = abs(cos(atan2f(float(z.hight - z.bottom), float(y_r(tilesize)))));
				int c = (int)(rad * (0.75f + cos(light_yrad + deg2rad((4 - UorL % 4) * 90)) * 0.25f) * 255.f);//
				Set_Bright(c);
			}
			switch (UorL % 4) {
			case 0:
			{
				switch (UorL) {
				case 0://縦(上)
					if (z.floor[0].y < z.top[0].y)
						DrawModi_wrap(z.top[0], z.top[1], z.floor[1], z.floor[0], z.wallhandle);
					break;
				case 4://上◢
					if (z.floor[1].y < z.top[1].y)
						DrawModi_wrap(z.top[1], z.top[1], z.floor[1], z.floor[0], z.wallhandle);
					break;
				case 8://上◣
					if (z.floor[0].y < z.top[0].y)
						DrawModi_wrap(z.top[0], z.top[0], z.floor[1], z.floor[0], z.wallhandle);
					break;
				case 12:
					if (z.floor[0].y < z.top[2].y)
						DrawModi_wrap(z.top[2], z.top[3], z.floor[1], z.floor[0], z.wallhandle);
					break;
				case 16:
					if (z.floor[0].y < z.top[2].y) {
						blend_shadow(&z.top[0], &z.top[3], z.hight, z.floorhandle);
						DrawModi_wrap(z.floor[0], z.floor[1], z.top[3], z.top[2], &res_floor);
					}
					break;
				}
			}
			break;
			case 1:
			{
				switch (UorL) {
				case 1://横(左)
					if (z.floor[0].x < z.top[0].x)
						DrawModi_wrap(z.top[2], z.top[0], z.floor[0], z.floor[2], z.wallhandle);
					break;
				case 5://左◢
					if (z.floor[0].x < z.top[0].x)
						DrawModi_wrap(z.top[2], z.top[2], z.floor[0], z.floor[2], z.wallhandle);
					break;
				case 9://左◥
					if (z.floor[0].x < z.top[0].x)
						DrawModi_wrap(z.top[0], z.top[0], z.floor[0], z.floor[2], z.wallhandle);
					break;
				case 13:
					if (z.floor[0].x < z.top[3].x)
						DrawModi_wrap(z.top[3], z.top[1], z.floor[0], z.floor[2], z.wallhandle);
					break;
				case 17:
					if (z.floor[0].x < z.top[3].x) {
						blend_shadow(&z.top[0], &z.top[3], z.hight, z.floorhandle);
						DrawModi_wrap(z.floor[0], z.top[1], z.top[3], z.floor[2], &res_floor);
					}
					break;
				}
			}
			break;
			case 2:
			{
				switch (UorL) {
				case 2://縦(下)
					if (z.floor[3].y >= z.top[3].y)
						DrawModi_wrap(z.top[2], z.top[3], z.floor[3], z.floor[2], z.wallhandle);
					break;
				case 6://下◢
					if (z.floor[3].y >= z.top[3].y)
						DrawModi_wrap(z.top[3], z.top[3], z.floor[3], z.floor[2], z.wallhandle);
					break;
				case 10://下◣
					if (z.floor[3].y >= z.top[3].y)
						DrawModi_wrap(z.top[2], z.top[2], z.floor[3], z.floor[2], z.wallhandle);
					break;
				case 14:
					if (z.floor[2].y > z.top[1].y) {
						blend_shadow(&z.top[0], &z.top[3], z.hight, z.floorhandle);
						DrawModi_wrap(z.top[0], z.top[1], z.floor[3], z.floor[2], &res_floor);
					}
					break;
				case 18:
					if (z.floor[2].y > z.top[1].y)
						DrawModi_wrap(z.top[0], z.top[1], z.floor[3], z.floor[2], z.wallhandle);
					break;
				}
			}
			break;
			case 3:
			{
				switch (UorL) {
				case 3://横(右)
					if (z.floor[3].x >= z.top[3].x)
						DrawModi_wrap(z.top[3], z.top[1], z.floor[1], z.floor[3], z.wallhandle);
					break;
				case 7://右◢
					if (z.floor[3].x >= z.top[3].x)
						DrawModi_wrap(z.top[3], z.top[3], z.floor[1], z.floor[3], z.wallhandle);
					break;
				case 11://右◥
					if (z.floor[3].x >= z.top[3].x)
						DrawModi_wrap(z.top[1], z.top[1], z.floor[1], z.floor[3], z.wallhandle);
					break;
				case 15:
					if (z.floor[1].x > z.top[2].x) {
						blend_shadow(&z.top[0], &z.top[3], z.hight, z.floorhandle);
						DrawModi_wrap(z.top[0], z.floor[1], z.floor[3], z.top[2], &res_floor);
					}
					break;
				case 19:
					if (z.floor[1].x > z.top[2].x)
						DrawModi_wrap(z.top[2], z.top[0], z.floor[1], z.floor[3], z.wallhandle);
					break;
				}
			}
			break;
			}
		}
		else {
			{
				int c = 255 * (camhigh - std::clamp(z.hight, 0, camhigh)) / camhigh;
				Set_Bright(c);
			}
			if (!z.is_wall) {
				blend_shadow(&z.top[0], &z.top[3], z.hight, z.floorhandle);
				DrawExtend_wrap(z.top[0], z.top[3], &res_floor);
			}
			else {
				DrawExtend_wrap(z.top[0], z.top[3], z.floorhandle);
			}
		}
	}
	//柱描画
	void Draw_Tile(const con& z) {
		switch (z.use) {//三角柱
		case 0://上
			Draw_Wall(12, z);	//縦(上)//12
			Draw_Wall(5, z);	//横(左)
			Draw_Wall(2, z);	//縦(下)
			Draw_Wall(7, z);	//横(右)
			break;
		case 1://左
			Draw_Wall(4, z);	//縦(上)//4
			Draw_Wall(13, z);	//横(左)
			Draw_Wall(6, z);	//縦(下)
			Draw_Wall(3, z);	//横(右)
			break;
		case 2://下
			Draw_Wall(0, z);	//縦(上)//12
			Draw_Wall(9, z);	//横(左)
			Draw_Wall(18, z);	//縦(下)
			Draw_Wall(11, z);	//横(右)
			break;
		case 3://右
			Draw_Wall(8, z);	//縦(上)//8
			Draw_Wall(1, z);	//横(左)
			Draw_Wall(10, z);	//縦(下)
			Draw_Wall(19, z);	//横(右)
			break;
		case 4://上
			Draw_Wall(16, z);	//縦(上)//12
			break;
		case 5://左
			Draw_Wall(17, z);	//横(左)
			break;
		case 6://下
			Draw_Wall(14, z);	//縦(下)
			break;
		case 7://右
			Draw_Wall(15, z);	//横(右)
			break;
		default://柱
			Draw_Wall(0, z);	//縦(上)
			Draw_Wall(1, z);	//横(左)
			Draw_Wall(2, z);	//縦(下)
			Draw_Wall(3, z);	//横(右)
			Draw_Wall(20, z);	//天井
			break;
		}
		Set_Bright(255);
		for (auto& pl : human) { pl.Draw_Foot(z, camerapos); }
		for (auto& pl : human) { pl.Draw(z); }
		//DrawFormatString(z.top[0].x, z.top[0].y, GetColor(255, 255, 255), "%d\n%d,%d", z.use, z.hight, z.bottom);
	}
	//y軸描画
	void Draw_Common(std::vector<con>& T_X, pos2D limmin, pos2D cam, pos2D limmax) {
		for (auto& pl : human) { pl.Reset(); }
		for (auto& z : T_X) {
			if (!(z.top[0].y <= cam.y && z.zero[3].y >= limmin.y)) { continue; }
			Draw_Tile(z);
		}
		for (auto& pl : human) { pl.Reset(); }
		for (int y = (int)(T_X.size()) - 1; y >= 0; --y) {
			auto& z = T_X[y];
			if (!(z.top[3].y >= cam.y && z.zero[0].y <= limmax.y)) { continue; }
			Draw_Tile(z);
		}
	}
	//壁影描画
	void draw_wall_shadow(int UorL, const con& z) {//一辺
		switch (UorL % 4) {
		case 0:
		{
			switch (UorL) {
			case 0://縦(上)
				DrawModi_wrap(shadow_pos[0], shadow_pos[1], shadow_pos[6], shadow_pos[4], z.wallhandle);
				break;
			case 8://上◣
				DrawModi_wrap(shadow_pos[0], shadow_pos[0], shadow_pos[6], shadow_pos[4], z.wallhandle);
				break;
			case 4://上◢
				DrawModi_wrap(shadow_pos[1], shadow_pos[1], shadow_pos[6], shadow_pos[4], z.wallhandle);
				break;
			case 16:
				if (shadow_pos[4].y < shadow_pos[2].y) {
					DrawModi_wrap(shadow_pos[2], shadow_pos[3], shadow_pos[6], shadow_pos[4], z.floorhandle);
				}
				break;
			case 12:
				DrawModi_wrap(shadow_pos[2], shadow_pos[3], shadow_pos[6], shadow_pos[4], z.wallhandle);
				break;
			}
		}
		break;
		case 1:
		{
			switch (UorL) {
			case 1://横(左)
				DrawModi_wrap(shadow_pos[2], shadow_pos[0], shadow_pos[4], shadow_pos[5], z.wallhandle);
				break;
			case 5://左◢
				DrawModi_wrap(shadow_pos[2], shadow_pos[2], shadow_pos[4], shadow_pos[5], z.wallhandle);
				break;
			case 9://左◥
				DrawModi_wrap(shadow_pos[0], shadow_pos[0], shadow_pos[4], shadow_pos[5], z.wallhandle);
				break;
			case 13:
				DrawModi_wrap(shadow_pos[3], shadow_pos[1], shadow_pos[4], shadow_pos[5], z.wallhandle);
				break;
			case 17:
				if (shadow_pos[4].x < shadow_pos[3].x) {
					DrawModi_wrap(shadow_pos[3], shadow_pos[1], shadow_pos[4], shadow_pos[5], z.floorhandle);
				}
				break;
			}
		}
		break;
		case 2:
		{
			switch (UorL) {
			case 2://縦(下)
				DrawModi_wrap(shadow_pos[2], shadow_pos[3], shadow_pos[7], shadow_pos[5], z.wallhandle);
				break;
			case 6://下◢
				DrawModi_wrap(shadow_pos[3], shadow_pos[3], shadow_pos[7], shadow_pos[5], z.wallhandle);
				break;
			case 10://下◣
				DrawModi_wrap(shadow_pos[2], shadow_pos[2], shadow_pos[7], shadow_pos[5], z.wallhandle);
				break;
			case 14:
				if (shadow_pos[5].y > shadow_pos[1].y) {
					DrawModi_wrap(shadow_pos[0], shadow_pos[1], shadow_pos[7], shadow_pos[5], z.floorhandle);
				}
				break;
			case 18:
				DrawModi_wrap(shadow_pos[0], shadow_pos[1], shadow_pos[7], shadow_pos[5], z.wallhandle);
				break;
			}
		}
		break;
		case 3:
		{
			switch (UorL) {
			case 3://横(右)
				DrawModi_wrap(shadow_pos[3], shadow_pos[1], shadow_pos[6], shadow_pos[7], z.wallhandle);
				break;
			case 7://右◢
				DrawModi_wrap(shadow_pos[3], shadow_pos[3], shadow_pos[6], shadow_pos[7], z.wallhandle);
				break;
			case 11://右◥
				DrawModi_wrap(shadow_pos[1], shadow_pos[1], shadow_pos[6], shadow_pos[7], z.wallhandle);
				break;
			case 15:
				if (shadow_pos[6].x > shadow_pos[2].x) {
					DrawModi_wrap(shadow_pos[2], shadow_pos[0], shadow_pos[6], shadow_pos[7], z.floorhandle);
				}
				break;
			case 19:
				DrawModi_wrap(shadow_pos[2], shadow_pos[0], shadow_pos[6], shadow_pos[7], z.wallhandle);
				break;
			}
		}
		break;
		}
	}
	//影を一部分描画
	void Update_Shadow(void) {
		const auto limmin = GetPos(-DrawPts->disp_x * 3 / 4, -DrawPts->disp_y * 3 / 4, 0);
		const auto limmax = GetPos(DrawPts->disp_x * 3 / 4, DrawPts->disp_y * 3 / 4, 0);
		int high = 0;
		for (auto& g : shadow_graph) {
			if (g.isupdate) {
				g.isupdate = false;
				g.handle.SetDraw_Screen();
				Set_Bright(0);
				for (auto& T_X : Tile) {
					if (!(T_X[0].zero[0].x <= limmax.x && T_X[0].zero[3].x >= limmin.x)) {
						continue;
					}
					for (auto& z : T_X) {
						if (!(z.zero[0].y <= limmax.y && z.zero[3].y >= limmin.y)) {
							continue;
						}
						if (z.is_wall) {
							//柱の影描画
							const auto add_x = (int)(float(z.hight - (z.bottom + high)) * sin(light_yrad));
							const auto add_y = (int)(float(z.hight - (z.bottom + high)) * cos(light_yrad));
							const auto xmin = camerapos.x + y_r(tilesize) * (z.cpos.x + 0);
							const auto ymin = camerapos.y + y_r(tilesize) * (z.cpos.y + 0);
							const auto xmax = camerapos.x + y_r(tilesize) * (z.cpos.x + 1);
							const auto ymax = camerapos.y + y_r(tilesize) * (z.cpos.y + 1);
							shadow_pos[0] = GetPos(xmin + add_x, ymin + add_y, z.bottom + high);
							shadow_pos[1] = GetPos(xmax + add_x, ymin + add_y, z.bottom + high);
							shadow_pos[2] = GetPos(xmin + add_x, ymax + add_y, z.bottom + high);
							shadow_pos[3] = GetPos(xmax + add_x, ymax + add_y, z.bottom + high);

							shadow_pos[4] = GetPos(xmin, ymin, z.bottom + high);//◤
							shadow_pos[5] = GetPos(xmin, ymax, z.bottom + high);//◣
							shadow_pos[6] = GetPos(xmax, ymin, z.bottom + high);//◥
							shadow_pos[7] = GetPos(xmax, ymax, z.bottom + high);//◢
							switch (z.use) {//三角柱
							case 0://上
								draw_wall_shadow(12, z);		//縦(上)12
								draw_wall_shadow(5, z);		//横(左)
								draw_wall_shadow(2, z);		//縦(下)
								draw_wall_shadow(7, z);		//横(右)
								break;
							case 1://左
								draw_wall_shadow(4, z);		//縦(上)4
								draw_wall_shadow(13, z);		//横(左)13
								draw_wall_shadow(6, z);		//縦(下)
								draw_wall_shadow(3, z);		//横(右)
								break;
							case 2://下
								draw_wall_shadow(0, z);		//縦(上)
								draw_wall_shadow(9, z);		//横(左)9
								draw_wall_shadow(18, z);		//縦(下)18
								draw_wall_shadow(11, z);		//横(右)
								break;
							case 3://右
								draw_wall_shadow(8, z);		//縦(上)
								draw_wall_shadow(1, z);		//横(左)
								draw_wall_shadow(10, z);		//縦(下)
								draw_wall_shadow(19, z);		//横(右)
								break;
							case 4://上
								draw_wall_shadow(16, z);		//縦(上)//4
								break;
							case 5://左
								draw_wall_shadow(17, z);		//横(左)
								break;
							case 6://下
								draw_wall_shadow(14, z);		//縦(下)
								break;
							case 7://右
								draw_wall_shadow(15, z);		//横(右)
								break;
							default://柱
								draw_wall_shadow(0, z);		//縦(上)
								draw_wall_shadow(1, z);		//横(左)
								draw_wall_shadow(2, z);		//縦(下)
								draw_wall_shadow(3, z);		//横(右)
								break;
							}
						}
						else {
							//*
							for (auto& pl : human) {
								pl.Draw_Shadow(z, light_yrad);
							}
							//*/
						}
					}
				}
			}
			high += 8;
		}
		Set_Bright(255);
	}
	//人と壁の判定
	void hit_wall(pos2D* m_pos, pos2D& old) {
		int radius = y_r(tilesize) / 3;

		m_pos->x = std::clamp(m_pos->x, radius, y_r(tilesize) * (int)(Tile.size()) - radius);
		m_pos->y = std::clamp(m_pos->y, radius, y_r(tilesize) * (int)(Tile.back().size()) - radius);

		for (auto& c : Tile) {
			for (auto& z : c) {
				if (z.is_wall) {
					const auto x0 = y_r(tilesize) * z.cpos.x - radius;
					const auto y0 = y_r(tilesize) * z.cpos.y - radius;
					const auto x1 = y_r(tilesize) * z.cpos.x + y_r(tilesize) * 5 / 4;
					const auto y1 = y_r(tilesize) * z.cpos.y + y_r(tilesize) * 5 / 4;
					pos2D s0 = { x0 ,y0 };
					pos2D s1 = { x0 ,y1 };
					pos2D s2 = { x1 ,y0 };
					pos2D s3 = { x1 ,y1 };
					if (ColSeg2(m_pos, old, s1, s0)) { break; }
					if (ColSeg2(m_pos, old, s0, s2)) { break; }
					if (ColSeg2(m_pos, old, s2, s3)) { break; }
					if (ColSeg2(m_pos, old, s3, s1)) { break; }
				}
			}
		}
		//抜け対策
		for (auto& c : Tile) {
			for (auto& z : c) {
				if (z.is_wall) {
					const auto x0 = y_r(tilesize) * z.cpos.x - radius;
					const auto y0 = y_r(tilesize) * z.cpos.y - radius;
					const auto x1 = y_r(tilesize) * z.cpos.x + y_r(tilesize) * 5 / 4;
					const auto y1 = y_r(tilesize) * z.cpos.y + y_r(tilesize) * 5 / 4;
					pos2D s0 = { x0 ,y0 };
					pos2D s1 = { x0 ,y1 };
					pos2D s2 = { x1 ,y0 };
					pos2D s3 = { x1 ,y1 };
					if (ColSeg2(m_pos, old, s1, s0)) { break; }
					if (ColSeg2(m_pos, old, s0, s2)) { break; }
					if (ColSeg2(m_pos, old, s2, s3)) { break; }
					if (ColSeg2(m_pos, old, s3, s1)) { break; }
				}
			}
		}
	}
	//button
	void button_set(int xs, int ys, int xsize, int ysize, const char* buf, bool on, size_t* cnt, std::function<void()> doing) {
		if (on) {
			if (in2_(mouse_x, mouse_y, xs, ys, xs + xsize, ys + ysize)) { *cnt = std::min<size_t>(*cnt + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0); }
			else { *cnt = 2; }
			if (*cnt == 1) { doing(); }
		}
		DrawBox(xs, ys, xs + xsize, ys + ysize, on ? ((in2_(mouse_x, mouse_y, xs, ys, xs + xsize, ys + ysize)) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
		Fonts.Get(y_r(40)).Get_handle().DrawString_MID(xs + xsize / 2, ys, buf, on ? ((in2_(mouse_x, mouse_y, xs, ys, xs + xsize, ys + ysize)) ? GetColor(255, 255, 0) : GetColor(255, 0, 0)) : GetColor(0, 0, 0));
	}
	//up_down
	void up_down_set(int xs, int ys, const char* buf, size_t* upcnt, size_t* downcnt, std::function<void()> doing1, std::function<void()> doing2) {
		bool on = true;
		int xsize = x_r(40);
		int ysize = y_r(30);
		int x1 = xs;
		int y1 = ys;
		int x2 = xs + Fonts.Get(y_r(40)).Get_handle().GetDrawWidth(buf);
		int y2 = ys + y_r(40);
		//高
		{
			if (in2_(mouse_x, mouse_y, x2, y1, x2 + xsize, y1 + ysize)) { *upcnt = std::min<size_t>(*upcnt + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0); }
			else { *upcnt = 2; }
			if (*upcnt == 1) { doing1(); }
			if (in2_(mouse_x, mouse_y, x2, y2, x2 + xsize, y2 + ysize)) { *downcnt = std::min<size_t>(*downcnt + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0); }
			else { *downcnt = 2; }
			if (*downcnt == 1) { doing2(); }
		}
		Fonts.Get(y_r(40)).Get_handle().DrawString(x1, y1 + y_r(15), buf, GetColor(255, 255, 0));
		DrawTriangle(x2 + xsize / 2, y1, x2, y1 + ysize, x2 + xsize, y1 + ysize, on ? (((in2_(mouse_x, mouse_y, x2, y1, x2 + xsize, y1 + ysize))) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
		DrawTriangle(x2, y2, x2 + xsize, y2, x2 + xsize / 2, y2 + ysize, on ? (((in2_(mouse_x, mouse_y, x2, y2, x2 + xsize, y2 + ysize))) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
	}
	bool save, wallorfloor, isread, smz, isend;
	size_t okcnt, ngcnt, undo, redo, upx, dnx, upy, dny, x_size, y_size, wofcnt, smzcnt, floortex, walltex, mscnt, cslcnt;
	int hight_s, bottom_s, cam_high;
	maps mapdata;
	std::list<std::vector<Status>> n_list;//
	std::vector<Status> n;
	std::vector<Player_Info> e;
	OPENFILENAME ofn;
	TCHAR strFile[MAX_PATH], cdir[MAX_PATH];
	std::list<std::vector<Status>>::iterator itr;
	bool Window1() {
		{
			//
			DrawBox(x_r(960 - 320), y_r(540 - 180), x_r(960 + 320), y_r(540 + 180), GetColor(128, 128, 128), TRUE);
			Fonts.Get(y_r(40)).Get_handle().DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "プリセットを読み込みますか?", GetColor(255, 255, 0));
			//OK
			button_set(x_r(960 + 320 - 340), y_r(540 + 180 - 140), x_r(300), y_r(40), "OK", true, &okcnt, [&]() {okcnt = 3;  });
			if (okcnt == 3) {
				okcnt = 2;
				isread = true;
				return false;
			}
			//NO
			button_set(x_r(960 + 320 - 340), y_r(540 + 180 - 80), x_r(300), y_r(40), "NO", true, &ngcnt, [&]() {ngcnt = 3;  });
			if (ngcnt == 3) {
				ngcnt = 2;
				return false;
			}
		}
		return true;
	}
	bool Window2() {
		using namespace std::literals;
		{
			//マップ描画
			{
				for (auto& m : n) {
					const int xs = DrawPts->disp_y / 40 + (int)(m.pos.x * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
					const int ys = DrawPts->disp_y / 40 + (int)(m.pos.y * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
					const int xe = DrawPts->disp_y / 40 + (int)((m.pos.x + 1) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
					const int ye = DrawPts->disp_y / 40 + (int)((m.pos.y + 1) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
					const unsigned char mh = (unsigned char)(255 * (camhigh - abs(m.hight)) / camhigh);
					const unsigned char mb = (unsigned char)(255 * (camhigh - abs(m.bottom)) / camhigh);

					if (in2_(mouse_x, mouse_y, xs, ys, xe, ye)) {
						if (m.is_wall) {
							Grad_Box(xs, ys, xe, ye, mh, mh / 2, 0u, mb, mb / 2, 0u, m.dir);
						}
						else {
							Grad_Box(xs, ys, xe, ye, mh, mh / 2, mh / 2, mb, mb / 2, mb / 2, m.dir - 4);
						}
						Fonts.Get(y_r(40)).Get_handle().DrawStringFormat((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(40), GetColor(255, 255, 255), "(%03d,%03d)", m.pos.x, m.pos.y);

						if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
							if (wallorfloor) {
								//壁
								m.hight = hight_s;
								m.bottom = bottom_s;
								m.dir = 255;
								m.is_wall = true;
							}
							else {
								//床
								m.hight = bottom_s;
								m.bottom = bottom_s;
								m.dir = 255;
								m.is_wall = false;
								//周りのタイルを変更
								if (smz) {
									change_tile(n, m, x_size, y_size);
								}
							}
							save = true;
						}
						else {
							if (save) {
								if (itr == n_list.end()--) {
									n_list.push_back(n);
									itr = n_list.end();
								}
								else {
									itr++;
									n_list.insert(itr, n);
									itr = n_list.erase(itr, n_list.end());
								}
								itr--;
								//nを保存
							}
							save = false;
						}
					}
					else {
						if (m.is_wall) {
							Grad_Box(xs, ys, xe, ye, mh, mh, 0, mb, mb, 0, m.dir);
						}
						else {
							Grad_Box(xs, ys, xe, ye, mh, mh, mh, mb, mb, mb, m.dir - 4);
							if (smz) {
								for (int i = 0; i < 4; i++) {
									const int xs2 = DrawPts->disp_y / 40 + (int)((m.pos.x +
										(
										(i == 0) ? 0 :
											(i == 1) ? 1 :
											(i == 2) ? 0 : -1
											)
										) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
									const int ys2 = DrawPts->disp_y / 40 + (int)((m.pos.y +
										(
										(i == 0) ? 1 :
											(i == 1) ? 0 :
											(i == 2) ? -1 : 0
											)
										) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
									const int xe2 = DrawPts->disp_y / 40 + (int)((m.pos.x + 1 +
										(
										(i == 0) ? 0 :
											(i == 1) ? 1 :
											(i == 2) ? 0 : -1
											)
										) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
									const int ye2 = DrawPts->disp_y / 40 + (int)((m.pos.y + 1 +
										(
										(i == 0) ? 1 :
											(i == 1) ? 0 :
											(i == 2) ? -1 : 0
											)
										) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
									if (in2_(mouse_x, mouse_y, xs2, ys2, xe2, ye2)) {
										Grad_Box(xs, ys, xe, ye, mh, mh / 2, mh / 2, mb, mb / 2, mb / 2, m.dir - 4);
										break;
									}
								}
							}
						}
					}

					if (m.is_wall) {
						DrawBox(xs, ys, xe, ye, GetColor(0, 0, 0), FALSE);
					}
				}
				DrawCircle(DrawPts->disp_y / 40 + mapdata.plx * (int)(DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size)) / y_r(tilesize), DrawPts->disp_y / 40 + mapdata.ply * (int)(DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size)) / y_r(tilesize), y_r(DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size)), GetColor(0, 255, 0));
				for (auto& m : e) {
					DrawCircle(
						DrawPts->disp_y / 40 + m.pos_p.x * (int)(DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size)) / y_r(tilesize),
						DrawPts->disp_y / 40 + m.pos_p.y * (int)(DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size)) / y_r(tilesize),
						y_r(DrawPts->disp_y / std::max(x_size, y_size)),
						GetColor(255, 0, 0));
				}
			}
			//壁か床か
			button_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(80), x_r(400), y_r(40), "選択タイルを変更", true, &wofcnt, [&]() {wallorfloor ^= 1;  });
			Fonts.Get(y_r(40)).Get_handle().DrawString((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(80) + y_r(40), wallorfloor ? "壁を選択中" : "床を選択中", GetColor(255, 0, 0));
			//壁か床か
			button_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(180), x_r(400), y_r(40), "地形編集", true, &smzcnt, [&]() {smz ^= 1;  });
			Fonts.Get(y_r(40)).Get_handle().DrawString((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(180) + y_r(40), smz ? "台形" : "矩形", GetColor(255, 0, 0));
			//床テクスチャ
			button_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(280), x_r(400), y_r(40), "床テクスチャ選択", GetWindowModeFlag() == TRUE, &floortex, [&]() {
				if (GetOpenFileName(&ofn)) {
					std::string str = strFile;
					if (str.find(cdir) != std::string::npos) {
						static TCHAR *ansFile = &strFile[strlen(cdir) + 1];
						strcpy_s(mapdata.floor_name, ansFile);
					}
					else {
						strcpy_s(mapdata.floor_name, strFile);//フルパス
					}
				}
			});
			Fonts.Get(y_r(30)).Get_handle().DrawString((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(280) + y_r(40), mapdata.floor_name, GetColor(255, 0, 0));
			//壁テクスチャ
			button_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(380), x_r(400), y_r(40), "壁テクスチャ選択", GetWindowModeFlag() == TRUE, &walltex, [&]() {
				if (GetOpenFileName(&ofn)) {
					std::string str = strFile;
					if (str.find(cdir) != std::string::npos) {
						static TCHAR *ansFile = &strFile[strlen(cdir) + 1];
						strcpy_s(mapdata.wall_name, ansFile);
					}
					else {
						strcpy_s(mapdata.wall_name, strFile);//フルパス
					}
				}
			});
			Fonts.Get(y_r(30)).Get_handle().DrawString((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(380) + y_r(40), mapdata.wall_name, GetColor(255, 0, 0));
			//設定する高さ
			{
				//高
				up_down_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(480), ("設定する高さ : "s + std::to_string(hight_s)).c_str(), &upx, &dnx, [&]() {
					if (hight_s < cam_high) {
						hight_s += 8;
					}
					else {
						hight_s = cam_high;
					}
				}, [&]() {
					if (hight_s > -cam_high) {
						hight_s -= 8;
					}
					else {
						hight_s = -cam_high;
					}
				});
				bottom_s = std::min(bottom_s, hight_s - 8);
				//底面
				up_down_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(580 + 15), ("設定する底面 : "s + std::to_string(bottom_s)).c_str(), &upy, &dny, [&]() {
					if (bottom_s < cam_high - 8) {
						bottom_s += 8;
						hight_s = std::max(bottom_s + 8, hight_s);
					}
					else {
						bottom_s = cam_high;
					}
				}, [&]() {
					if (bottom_s > -cam_high) {
						bottom_s -= 8;
					}
					else {
						bottom_s = -cam_high;
					}
				});
			}
			//アンドゥ
			button_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(680), x_r(100), y_r(40), "戻る", (n_list.size() >= 2 && itr != n_list.begin()), &undo, [&]() {itr--; n = *itr;  });
			//リドゥ
			button_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)) + x_r(150), y_r(680), x_r(100), y_r(40), "進む", (n_list.size() >= 2 && std::next(itr, 1) != n_list.end()), &redo, [&]() {itr++; n = *itr;  });
			//終了
			button_set(x_r(1920 - 340), y_r(1080 - 160), x_r(300), y_r(40), "保存せず終了", true, &cslcnt, [&]() {cslcnt = 3;  });
			if (cslcnt == 3) {
				isend = true;
				return false;
			}
			//終了
			button_set(x_r(1920 - 340), y_r(1080 - 80), x_r(300), y_r(40), "保存して続行", true, &mscnt, [&]() {mscnt = 3;  });
			if (mscnt == 3) {
				return false;
			}
			//
		}
		return true;
	}
public:
	//コンストラクタ
	Draw(std::shared_ptr<DXDraw>& DrawPts_t) {
		DrawPts = DrawPts_t;
		for (auto& g : shadow_graph) {
			g.handle = GraphHandle::Make(DrawPts->disp_x, DrawPts->disp_y, true);
			g.isupdate = true;
		}
		screen = GraphHandle::Make(DrawPts->disp_x, DrawPts->disp_y, false);
		res_floor = GraphHandle::Make(16, 16, true);
	}
	//デストラクタ
	~Draw(void) {
		Dispose();
		DrawPts.reset();
	}
	//mapエディター
	bool Map_Editer(std::string mapname) {
		using namespace std::literals;
		std::fstream file;
		wallorfloor = false;
		wofcnt = 0;
		smzcnt = 0;
		floortex = 0;
		walltex = 0;
		mscnt = 0;
		cslcnt = 0;
		//ダイアログ用
		{
			GetCurrentDirectory(MAX_PATH, cdir);
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = GetMainWindowHandle();
			ofn.lpstrFilter =
				TEXT("Picture files {*.bmp}\0*.bmp\0")
				TEXT("Picture files {*.png}\0*.png\0");
			ofn.lpstrCustomFilter = NULL;
			ofn.nMaxCustFilter = NULL;
			ofn.nFilterIndex = 0;
			ofn.lpstrFile = strFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			ofn.lpstrInitialDir = cdir;
			ofn.lpstrTitle = "カレントディレクトリより下層のファイルを指定してください";
		}
		//
		n.clear();
		//mapdata
		e.clear();
		x_size = 0;
		y_size = 0;
		{
			okcnt = 0;
			ngcnt = 0;
			isread = false;
			while (ProcessMessage() == 0) {
				GetMousePoint(&mouse_x, &mouse_y);
				GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
				if (!Window1()) { break; }
				//画面の反映
				DrawPts->Screen_Flip();
			}
			//map読み込み
			if (!isread) {
				//mapデータ1読み込み(マップチップ)
				{
					file.open(("data/Map/" + mapname + "/1.dat").c_str(), std::ios::binary | std::ios::in);
					do {
						n.resize(n.size() + 1);
						file.read((char*)&n.back(), sizeof(n.back()));
						x_size = std::max<size_t>(n.back().pos.x, x_size);
						y_size = std::max<size_t>(n.back().pos.y, y_size);
					} while (!file.eof());
					x_size++;
					y_size++;
					file.close();
					n.pop_back();
				}
				//mapデータ2読み込み(プレイヤー初期位置、使用テクスチャ指定)
				{
					file.open(("data/Map/" + mapname + "/2.dat").c_str(), std::ios::binary | std::ios::in);
					file.read((char*)&mapdata, sizeof(mapdata));
					file.close();
				}
				//mapデータ3読み込み(敵情報)
				{
					file.open(("data/Map/" + mapname + "/3.dat").c_str(), std::ios::binary | std::ios::in);
					do {
						e.resize(e.size() + 1);
						file.read((char*)&e.back(), sizeof(e.back()));
					} while (!file.eof());
					file.close();
					e.pop_back();
				}
			}
			//mapデータプリセット
			else {
				{
					//mapデータ1書き込み(マップチップ)
					x_size = 40;
					y_size = 40;
					{
						upx = 0;
						dnx = 0;
						upy = 0;
						dny = 0;
						while (ProcessMessage() == 0) {
							GetMousePoint(&mouse_x, &mouse_y);
							GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
							{
								DrawBox(x_r(960 - 320), y_r(540 - 180), x_r(960 + 320), y_r(540 + 180), GetColor(128, 128, 128), TRUE);
								Fonts.Get(y_r(40)).Get_handle().DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "マップのサイズは?", GetColor(255, 255, 0));
								//高
								up_down_set(x_r(960 - 320 + 40), y_r(540 - 180 + 60 + 100), ("X : "s + std::to_string(x_size)).c_str(), &upx, &dnx, [&]() {
									x_size++;
								}, [&]() {
									if (x_size > 1) {
										x_size--;
									}
								});
								//底面
								up_down_set(x_r(960 - 320 + 40), y_r(540 - 180 + 60 + 100 + 115), ("Y : "s + std::to_string(y_size)).c_str(), &upy, &dny, [&]() { y_size++; }, [&]() { if (y_size > 1) { y_size--; }});
								{
									int xsz = x_r(280);
									int ysz = y_r(120);
									int xm = x_r(1100);
									int ym = y_r(540);
									if (x_size * ysz / xsz >= y_size) {
										ysz = xsz * (int)(y_size) / (int)(x_size);
									}
									else {
										xsz = ysz * (int)(x_size) / (int)(y_size);
									}
									DrawBox(xm - xsz / 2, ym - ysz / 2, xm + xsz / 2, ym + ysz / 2, GetColor(255, 255, 0), FALSE);
								}
								//終了
								button_set(x_r(960 + 320 - 340), y_r(540 + 180 - 80), x_r(300), y_r(40), "OK", true, &mscnt, [&]() {mscnt = 3;  });
								if (mscnt == 3) {
									mscnt = 2;
									break;
								}
							}
							//画面の反映
							DrawPts->Screen_Flip();
						}

					}
					const int btm = 0;
					const int mid = 0;
					const int hig = 64;
					for (int y = 0; y < y_size; y++) {
						for (int x = 0; x < x_size; x++) {
							n.resize(n.size() + 1);
							n.back() = { pos2D::Get(x,y), btm, mid, false,2, 2, 255 };
						}
					}
					//(マップチップ)
					for (int y = 0; y < y_size; y += 5) {
						for (int x = 0; x < x_size; x += 5) {
							const size_t s = size_t(x) + size_t(y) * x_size;
							/*
							n[s + 2 + size_t(x_size)*1] = { pos2D::Get(x + 2, y + 1), btm, hig, true, 2, 2, -1 };
							n[s + 1 + size_t(x_size)*2] = { pos2D::Get(x + 1, y + 2), btm, hig, true, 2, 2, -1 };
							n[s + 2 + size_t(x_size)*2] = { pos2D::Get(x + 2, y + 2), btm, hig, true, 2, 2, -1 };
							n[s + 3 + size_t(x_size)*2] = { pos2D::Get(x + 3, y + 2), btm, hig, true, 2, 2, -1 };
							n[s + 2 + size_t(x_size)*3] = { pos2D::Get(x + 2, y + 3), btm, hig, true, 2, 2, -1 };
							//*/
							//*
							n[s + 2 + x_size * 1] = { pos2D::Get(x + 2, y + 1), btm, hig, true, 2, 2, 0 };
							n[s + 1 + x_size * 2] = { pos2D::Get(x + 1, y + 2), btm, hig, true, 2, 2, 1 };
							n[s + 2 + x_size * 2] = { pos2D::Get(x + 2, y + 2), btm, hig, true, 2, 1, 255 };
							n[s + 3 + x_size * 2] = { pos2D::Get(x + 3, y + 2), btm, hig, true, 2, 2, 3 };
							n[s + 2 + x_size * 3] = { pos2D::Get(x + 2, y + 3), btm, hig, true, 2, 2, 2 };
							//*/
						}
					}
				}
				{
					//mapデータ2書き込み(プレイヤー初期位置、使用テクスチャ指定)
					mapdata.plx = 32;
					mapdata.ply = 32;
					mapdata.light_yrad = deg2rad(0);
					strcpy_s(mapdata.wall_name, "data/Chip/Wall/1.bmp");
					strcpy_s(mapdata.floor_name, "data/Chip/Floor/1.bmp");
				}
				{
					//mapデータ3書き込み(敵情報)
					for (int i = 1; i < 8; i++) {
						e.resize(e.size() + 1);
						e.back().pos_p.x = y_r(tilesize) * 5 * i + y_r(tilesize) / 2;
						e.back().pos_p.y = y_r(tilesize) * 5 * i + y_r(tilesize) / 2;
					}
				}
			}
		}
		//エディター
		hight_s = 64;
		bottom_s = 0;
		cam_high = camhigh;
		undo = 2;
		redo = 2;
		save = false;
		smz = false;
		n_list.clear();
		upx = 2;
		dnx = 2;
		upy = 2;
		dny = 2;
		n_list.push_back(n);
		itr = n_list.end();
		isend = false;
		while (ProcessMessage() == 0) {
			GetMousePoint(&mouse_x, &mouse_y);
			GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
			if (!Window2()) { break; }
			//画面の反映
			DrawPts->Screen_Flip();
		}
		if (isend) { return false; }
		n_list.clear();
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
	void Start(int* player_x, int* player_y, std::string mapname) {
		using namespace std::literals;
		std::fstream file;
		size_t map_x = 0, map_y = 0;
		//mapデータ1読み込み(マップチップ)
		{
			file.open(("data/Map/" + mapname + "/1.dat").c_str(), std::ios::binary | std::ios::in);
			do {
				ans.resize(ans.size() + 1);
				file.read((char*)&ans.back(), sizeof(ans.back()));
				map_x = std::max<size_t>(ans.back().pos.x, map_x);
				map_y = std::max<size_t>(ans.back().pos.y, map_y);
			} while (!file.eof());
			map_x++;
			map_y++;
			file.close();
			ans.pop_back();

			Tile.resize(map_x);
			for (auto& z : Tile) {
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
			//GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, y_r(tilesize), y_r(tilesize) * 2, &walls);
			//GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, y_r(tilesize), y_r(tilesize), &floors);
			GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, 16, 16 * 2, &walls);
			GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, 16, 16, &floors);
			player_id = human.size();
			human.resize(human.size() + 1);
			human.back().First(*player_x, *player_y);
		}
		//mapデータ3読み込み(敵情報)
		{
			file.open(("data/Map/" + mapname + "/3.dat").c_str(), std::ios::binary | std::ios::in);
			do {
				Player_Info anse;
				file.read((char*)&anse, sizeof(anse));
				human.resize(human.size() + 1);
				human.back().First(anse.pos_p.x/2, anse.pos_p.y/2);
			} while (!file.eof());
			file.close();
		}
	}
	//人の移動処理
	void Update_Human(pos2D* m_pos,bool *isstand) {
		for (auto& pl : human) {
			bool isPlayer = ((size_t)(&pl - &human.front()) == player_id);

			pl.vec_buf = pl.pos;
			if (!isPlayer) {
				//todo : cpu
				//pl.pos.x += GetRand(12) - 6;
				//pl.pos.y += GetRand(12) - 6;
			}
			else {
				//自機の移動
				pl.pos = (*m_pos) * -1.f;
				int x_m, y_m;
				GetMousePoint(&x_m, &y_m);
				pl.Set_Aim(x_m - DrawPts->disp_x / 2, y_m - DrawPts->disp_y / 2);
			}
			hit_wall(&pl.pos, pl.vec_buf);
			if (isPlayer) {
				if (!pl.standup) {
					pl.isStand = *isstand;
				}
				else {
					pl.standup = false;
					*isstand = pl.isStand;
				}
				*m_pos = pl.pos * -1.f;
			}
			pl.vec_buf -= pl.pos;

			pl.hight = Tile[std::max(pl.pos.x / y_r(tilesize), 0)][std::max(pl.pos.y / y_r(tilesize), 0)].hight;
		}
	}
	//更新
	void FirstUpdate(int sx, int sy) {
		//カメラ座標設定
		camerapos.set(sx, sy);
		//mapの設置
		for (auto& at : ans) {
			auto& z = Tile[at.pos.x][at.pos.y];
			z.cpos = at.pos;
			z.bottom = at.bottom;
			z.hight = at.hight;
			const auto xmin = camerapos.x + y_r(tilesize) * (z.cpos.x + 0);
			const auto ymin = camerapos.y + y_r(tilesize) * (z.cpos.y + 0);
			const auto xmax = camerapos.x + y_r(tilesize) * (z.cpos.x + 1);
			const auto ymax = camerapos.y + y_r(tilesize) * (z.cpos.y + 1);
			z.top[0] = GetPos(xmin, ymin, z.hight);
			z.top[1] = GetPos(xmax, ymin, z.hight);
			z.top[2] = GetPos(xmin, ymax, z.hight);
			z.top[3] = GetPos(xmax, ymax, z.hight);
			z.floor[0] = GetPos(xmin, ymin, z.bottom);
			z.floor[1] = GetPos(xmax, ymin, z.bottom);
			z.floor[2] = GetPos(xmin, ymax, z.bottom);
			z.floor[3] = GetPos(xmax, ymax, z.bottom);
			z.zero[0] = GetPos(xmin, ymin, 0);
			z.zero[1] = GetPos(xmax, ymin, 0);
			z.zero[2] = GetPos(xmin, ymax, 0);
			z.zero[3] = GetPos(xmax, ymax, 0);
			z.wallhandle = &walls[at.wall_id];
			z.floorhandle = &floors[at.floor_id];
			z.use = at.dir;
			z.is_wall = at.is_wall;
		}
	}
	void Update(void) {
		//人の描画用意
		for (auto& pl : human) { pl.Set(camerapos); }
		//影
		Update_Shadow();
		//一気に描画
		{
			//DRAW
			const auto limmin = GetPos(-DrawPts->disp_x / 2, -DrawPts->disp_y / 2, 0);
			const auto cam = GetPos(0, 0, 0);
			const auto limmax = GetPos(DrawPts->disp_x / 2, DrawPts->disp_y / 2, 0);
			screen.SetDraw_Screen();
			{
				for (int x = 0; x < (int)(Tile.size()); ++x) {
					auto& T_X = Tile[x];
					if (!(T_X[0].top[0].x <= cam.x && T_X[0].zero[3].x >= limmin.x)) { continue; }
					Draw_Common(T_X, limmin, cam, limmax);
				}
				for (int x = (int)(Tile.size()) - 1; x >= 0; --x) {
					auto& T_X = Tile[x];
					if (!(T_X[0].top[3].x >= cam.x && T_X[0].zero[0].x <= limmax.x)) { continue; }
					Draw_Common(T_X, limmin, cam, limmax);
				}
			}
		}
	}
	//出力
	void Output(void) {
		screen.DrawGraph(0, 0, true);
		/*
		int xp=0, yp=0, xs = 192, ys = 108;
		DrawBox(0, 0, 1920, ys, GetColor(255, 255, 255), TRUE);
		for (auto& g : shadow_graph) {
			g.handle.DrawExtendGraph(xp, yp, xp + xs, yp + ys, true);
			xp += xs;

		}
		//*/
	}
	//mapの後始末
	void Dispose(void) {
		walls.clear();
		floors.clear();
		human.clear();
		for (auto& z : Tile) {
			z.clear();
		}
		Tile.clear();
	}
};