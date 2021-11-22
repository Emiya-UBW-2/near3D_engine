#pragma once
#include "DXLib_ref/DXLib_ref.h"
#include <optional>

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

const int camhigh = 192;//カメラの高さ
const int tilesize = 64;//タイル一つ一つのサイズ
class Draw {
private:
public:
	//2Dベクトル関連
	class pos2D{
	public:
		int x = 0;
		int y = 0;
		//設定する
		void set(int m_x,int m_y)noexcept {
			this->x = m_x;
			this->y = m_y;
		}
		//加算した値を返す
		pos2D operator+(pos2D o)noexcept {
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
		pos2D operator-(pos2D o)noexcept {
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

		pos2D operator*(float o)noexcept {
			pos2D temp;
			temp.x = (int)((float)(this->x) * o);
			temp.y = (int)((float)(this->y) * o);
			return temp;
		}

		//距離の2乗を取得する
		int hydist()noexcept {
			return this->x*this->x + this->y*this->y;
		}
		// 内積
		int dot(pos2D* v2)noexcept {
			return this->x * v2->x + this->y * v2->y;
		}
		// 外積
		int cross(pos2D* v2)noexcept {
			return this->x * v2->y - this->y * v2->x;
		}
	};
	//pos2D取得
	static pos2D get_pos2D(int xp, int yp) {
		pos2D p;
		p.set(xp, yp);
		return p;
	}
	//線分衝突
	bool ColSeg(pos2D &pos1, pos2D &vec1, pos2D &pos2, pos2D &vec2) {
		auto Crs_v1_v2 = vec1.cross(&vec2);
		if (Crs_v1_v2 == 0) {
			return false;// 平行
		}
		pos2D v = pos2 - pos1;
		const auto Crs_v_v1 = v.cross(&vec1);
		const auto Crs_v_v2 = v.cross(&vec2);
		if (Crs_v_v2 < 0 || Crs_v_v2 > Crs_v1_v2 || Crs_v_v1 < 0 || Crs_v_v1 > Crs_v1_v2) {
			return false;// 交差X
		}
		return true;
	}
	//座標変換
	static pos2D getpos(int xp, int yp, int high) {
		if (camhigh > high) {
			return get_pos2D(deskx / 2 + camhigh * xp / (camhigh - high), desky / 2 + camhigh * yp / (camhigh - high));
		}
		else {
			return get_pos2D(deskx / 2 + camhigh * xp, desky / 2 + camhigh * yp);
		}
	}
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
	};
	class Status {
	public:
		pos2D pos;
		int bottom = 0, hight = 0;
		bool is_wall = false;
		int wall_id=0, floor_id = 0;
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
	std::vector<Status> ans;
	std::vector<std::vector<con>> zcon;
	pos2D pos_t = { 0,0 };
	int ypos = 0;
	float light_yrad = 0.0f;
	std::vector<GraphHandle> walls;
	std::vector<GraphHandle> floors;
	std::array<GraphHandle, 8> shadow_graph;
	GraphHandle res_floor;
	GraphHandle screen;

	class Bonesdata {
	public:
		int parent;
		int xp, yp, zp;
		float xr, yr, zr;
		float xrad, yrad, zrad;
		float xdist, ydist, zdist;
		bool edit;
	};
	class Animesdata {
	public:
		int time = 0;
		std::vector<Bonesdata> bone;
	};
	class Humans {
		std::vector<GraphHandle> Graphs;
		std::vector<Bonesdata> bone;
		std::vector<pairs> sort;
		std::vector<bool> draw_ok = { false };
		bool draw_end=false;
		float yrad = 0.f;
		int animeframe = 0;
		int animetime = 1;
		int animesel = 0;
	public:
		std::vector<std::vector<Animesdata>> anime;
		pos2D pos;
		pos2D vec_buf;
		pos2D vec;
		int hight = 0;
		void Set(pos2D mp_pos) {
			for (auto& g : this->bone) {
				g.edit = false;
			}

			//ここでアニメーション
			{
				auto& x = this->anime[this->animesel];
				auto& now = x[this->animeframe];
				auto& next = x[(this->animeframe + 1) % int(x.size())];
				if (this->animetime < now.time) {
					for (int z = 0; z < this->bone.size(); z++) {
						this->bone[z].xrad = now.bone[z].xrad + (next.bone[z].xrad - now.bone[z].xrad)*this->animetime / now.time;
						this->bone[z].yrad = now.bone[z].yrad + (next.bone[z].yrad - now.bone[z].yrad)*this->animetime / now.time;
						this->bone[z].zrad = now.bone[z].zrad + (next.bone[z].zrad - now.bone[z].zrad)*this->animetime / now.time;
					}
					++this->animetime;
				}
				else {
					++this->animeframe %= int(x.size());
					this->animetime = 0;
				}
			}
			{
				auto o = this->animesel;

				if (this->vec_buf.x == 0 && this->vec_buf.y == 0) {
					this->animesel = 1;
				}
				else {
					this->animesel = 0;
				}

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
				auto q = (float(this->vec.x)*cos(this->yrad) - float(this->vec.y)* -sin(this->yrad)) / b;
				if (q > sin(deg2rad(10))) {
					this->yrad += deg2rad(-5);
				}
				else if (q < sin(deg2rad(-10))) {
					this->yrad += deg2rad(5);
				}
				//真後ろ振り向き
				if ((float(this->vec.x)* -sin(this->yrad) + float(this->vec.y)*cos(this->yrad)) / b <= -0.5) {
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
							z.xp = mp_pos.x;
							z.yp = mp_pos.y;
							z.zp = 0;
							z.xr = z.xrad;
							z.yr = z.yrad + this->yrad;
							z.zr = z.zrad;
							z.edit = true;
						}
						else {
							if (this->bone[p].edit) {
								const float xd = z.xdist*tilesize / 32;
								const float yd = z.ydist*tilesize / 32;
								const float zd = z.zdist * 540 / desky;

								z.xr = this->bone[p].xrad + this->bone[p].xr;
								z.yr = this->bone[p].yrad + this->bone[p].yr;
								z.zr = this->bone[p].yrad + this->bone[p].zr;

								float y1 = cos(z.xr)*yd + sin(z.xr)*zd;
								float z1 = cos(z.xr)*zd - sin(z.xr)*yd;
								float x2 = cos(z.zr)*xd + sin(z.zr)*z1;

								z.xp = this->bone[p].xp + int(cos(z.yr)*x2 - sin(z.yr)*y1);
								z.yp = this->bone[p].yp + int(cos(z.yr)*y1 + sin(z.yr)*x2);
								z.zp = this->bone[p].zp + int(cos(z.zr)*z1 - sin(z.zr)*xd);
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
		}
		void First(int xp, int yp) {
			using namespace std::literals;
			GraphHandle::LoadDiv("data/Char/1.bmp", 33, 11, 3, 32, 32, &this->Graphs);
			this->sort.resize(this->Graphs.size());
			this->draw_ok.resize(this->Graphs.size());
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
						n.back().xdist = -9.0f;
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
						n.back().xdist = 9.0f;
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
			//モーションテキスト(直に打ち込めるように)
			for (int i = 0; i < 2; i++) {
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
						else if (ttt.find("left_arm_") != std::string::npos) {
							if (ttt.find("x") != std::string::npos) {
								this->anime.back().back().bone[4].xrad = deg2rad(tmp);
							}
							else if (ttt.find("y") != std::string::npos) {
								this->anime.back().back().bone[4].yrad = deg2rad(tmp);
							}
							else if (ttt.find("z") != std::string::npos) {
								this->anime.back().back().bone[4].zrad = deg2rad(tmp);
							}
						}
						else if (ttt.find("right_arm_") != std::string::npos) {
							if (ttt.find("x") != std::string::npos) {
								this->anime.back().back().bone[6].xrad = deg2rad(tmp);
							}
							else if (ttt.find("y") != std::string::npos) {
								this->anime.back().back().bone[6].yrad = deg2rad(tmp);
							}
							else if (ttt.find("z") != std::string::npos) {
								this->anime.back().back().bone[6].zrad = deg2rad(tmp);
							}
						}

						else if (ttt.find("left_leg_") != std::string::npos) {
							if (ttt.find("x") != std::string::npos) {
								this->anime.back().back().bone[26].xrad = deg2rad(tmp);
							}
							else if (ttt.find("y") != std::string::npos) {
								this->anime.back().back().bone[26].yrad = deg2rad(tmp);
							}
							else if (ttt.find("z") != std::string::npos) {
								this->anime.back().back().bone[26].zrad = deg2rad(tmp);
							}
						}
						else if (ttt.find("right_leg_") != std::string::npos) {
							if (ttt.find("x") != std::string::npos) {
								this->anime.back().back().bone[28].xrad = deg2rad(tmp);
							}
							else if (ttt.find("y") != std::string::npos) {
								this->anime.back().back().bone[28].yrad = deg2rad(tmp);
							}
							else if (ttt.find("z") != std::string::npos) {
								this->anime.back().back().bone[28].zrad = deg2rad(tmp);
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
				do {
					this->bone.resize(this->bone.size() + 1);
					file.read((char*)&this->bone.back(), sizeof(this->bone.back()));
				} while (!file.eof());
				this->bone.pop_back();
				file.close();
			}
			this->pos.x = xp;
			this->pos.y = yp;
		}
		void Reset() {
			std::fill<>(this->draw_ok.begin(), this->draw_ok.end(), false);
			this->draw_end = false;
		}
		void Draw(con* z) {
			if (!this->draw_end) {
				bool t = true;
				for (auto& g : this->sort) {
					auto& b = this->bone[g.first];
					auto zh = b.zp - this->sort[0].second;
					auto q = getpos(b.xp + this->pos.x, b.yp + this->pos.y, z->hight);
					if ((q.x >= z->top[0].x && q.y >= z->top[0].y && q.x <= z->top[3].x && q.y <= z->top[3].y) || this->draw_ok[g.first]) {
						auto p = getpos(b.xp + this->pos.x, b.yp + this->pos.y, z->hight + zh);
						DrawRotaGraphFast(p.x, p.y, float((z->hight + zh) + camhigh) / camhigh * tilesize / 32 / 2, b.yrad + b.yr, this->Graphs[g.first].get(), TRUE);
						this->draw_ok[g.first] = true;
					}
					if (!this->draw_ok[g.first]) {
						t = false;
					}
				}
				if (t) {
					this->draw_end = true;
				}
			}
		}
		void Draw_Shadow(con* z,float light_yrad) {
			for (auto& g : this->sort) {
				auto& b = this->bone[g.first];
				auto zh = b.zp - this->sort[0].second;
				auto q = getpos(b.xp + this->pos.x, b.yp + this->pos.y, z->hight + zh);
				if (q.x >= z->top[0].x && q.y >= z->top[0].y && q.x <= z->top[3].x && q.y <= z->top[3].y) {
					auto p = getpos(b.xp + this->pos.x + int(float(zh)*sin(light_yrad)), b.yp + this->pos.y + int(float(zh)*cos(light_yrad)), z->hight);
					DrawRotaGraphFast(p.x, p.y, float((z->hight + zh) + camhigh) / camhigh * tilesize / 32 / 2, b.yrad + b.yr, this->Graphs[g.first].get(), TRUE);
				}
			}
		}
	};
	std::vector<Humans> human;
	size_t player_id = 0;
	//mapエディター
	void change_tile(std::vector<Status>&n, Status&m, size_t &x_size, size_t &y_size) {
		if (!m.is_wall) {
			const size_t s = size_t(m.pos.x) + size_t(m.pos.y) * x_size;
			if (m.pos.x >= 1) {
				auto& t = n[s - 1];
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 5 : 255;
				}
			}
			if (m.pos.x <= x_size - 1 - 1) {
				auto& t = n[s + 1];
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 7 : 255;
				}
			}

			if (m.pos.y >= 1) {
				auto& t = n[s - x_size];
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 4 : 255;
				}
			}
			if (m.pos.y <= y_size - 1 - 1) {
				auto& t = n[s + x_size];
				//下
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 6 : 255;
				}
			}
		}
	}
	//柱設置
	void set_drw_rect(Status* p) {//三角柱
		auto& z = zcon[p->pos.x][p->pos.y];
		z.cpos = p->pos;
		z.bottom = p->bottom;
		z.hight = p->hight;

		z.top[0] = getpos(pos_t.x + tilesize * (z.cpos.x + 0), pos_t.y + tilesize * (z.cpos.y + 0), z.hight);
		z.top[1] = getpos(pos_t.x + tilesize * (z.cpos.x + 1), pos_t.y + tilesize * (z.cpos.y + 0), z.hight);
		z.top[2] = getpos(pos_t.x + tilesize * (z.cpos.x + 0), pos_t.y + tilesize * (z.cpos.y + 1), z.hight);
		z.top[3] = getpos(pos_t.x + tilesize * (z.cpos.x + 1), pos_t.y + tilesize * (z.cpos.y + 1), z.hight);

		z.floor[0] = getpos(pos_t.x + tilesize * (z.cpos.x + 0), pos_t.y + tilesize * (z.cpos.y + 0), z.bottom);
		z.floor[1] = getpos(pos_t.x + tilesize * (z.cpos.x + 1), pos_t.y + tilesize * (z.cpos.y + 0), z.bottom);
		z.floor[2] = getpos(pos_t.x + tilesize * (z.cpos.x + 0), pos_t.y + tilesize * (z.cpos.y + 1), z.bottom);
		z.floor[3] = getpos(pos_t.x + tilesize * (z.cpos.x + 1), pos_t.y + tilesize * (z.cpos.y + 1), z.bottom);

		z.zero[0] = getpos(pos_t.x + tilesize * (z.cpos.x + 0), pos_t.y + tilesize * (z.cpos.y + 0), 0);
		z.zero[1] = getpos(pos_t.x + tilesize * (z.cpos.x + 1), pos_t.y + tilesize * (z.cpos.y + 0), 0);
		z.zero[2] = getpos(pos_t.x + tilesize * (z.cpos.x + 0), pos_t.y + tilesize * (z.cpos.y + 1), 0);
		z.zero[3] = getpos(pos_t.x + tilesize * (z.cpos.x + 1), pos_t.y + tilesize * (z.cpos.y + 1), 0);

		z.wallhandle = &walls[p->wall_id];
		z.floorhandle = &floors[p->floor_id];
		z.use = p->dir;
		z.is_wall = p->is_wall;
	}
	//mapの設置
	void put_map(void) {
		for (auto& z : ans) {
			set_drw_rect(&z);
		}
	}
	//壁描画
	void blend_shadow(const pos2D* p1,const pos2D* p2,int*hight,GraphHandle* f_handle) {
		const int g = DerivationGraph(
			std::max(0, p1->x),
			std::max(0, p1->y),
			std::min(deskx, p2->x - p1->x),
			std::min(desky, p2->y - p1->y),
			shadow_graph[std::clamp<size_t>(*hight / 8, 0, shadow_graph.size() - 1)].get()
		);
		GraphBlendBlt(f_handle->get(), g, res_floor.get(), 128, DX_GRAPH_BLEND_NORMAL);
		DeleteGraph(g);
	}
	void DrawModi_wrap(const pos2D &p1, const pos2D &p2, const pos2D &p3, const pos2D &p4, GraphHandle* g_handle) { DrawModiGraph(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, g_handle->get(), TRUE); }
	void DrawExtend_wrap(const pos2D &p1, const pos2D &p2, GraphHandle* g_handle) { g_handle->DrawExtendGraph(p1.x, p1.y, p2.x, p2.y, true); }
	//壁
	void Draw_Wall(int UorL,con* z){
		//float rad = 1.f;
		if (UorL < 20 && z->hight != z->bottom) {
			float rad = abs(cos(atan2f(float(z->hight - z->bottom), float(tilesize))));
			if (UorL % 4 == 0) {
				int c = int(rad * (0.75f + cos(light_yrad + deg2rad(0)) *0.25f) *255.f);
				SetDrawBright(c, c, c);
				switch (UorL) {
				case 0://縦(上)
					if (z->floor[0].y < z->top[0].y)
						DrawModi_wrap(z->top[0], z->top[1], z->floor[1], z->floor[0], z->wallhandle);
					break;
				case 4://上◢
					if (z->floor[1].y < z->top[1].y)
						DrawModi_wrap(z->top[1], z->top[1], z->floor[1], z->floor[0], z->wallhandle);
					break;
				case 8://上◣
					if (z->floor[0].y < z->top[0].y)
						DrawModi_wrap(z->top[0], z->top[0], z->floor[1], z->floor[0], z->wallhandle);
					break;
				case 12:
					if (z->floor[0].y < z->top[2].y)
						DrawModi_wrap(z->top[2], z->top[3], z->floor[1], z->floor[0], z->wallhandle);
					break;
				case 16:
					if (z->floor[0].y < z->top[2].y) {
						blend_shadow(&z->top[0], &z->top[3], &z->hight, z->floorhandle);
						DrawModi_wrap(z->floor[0], z->floor[1], z->top[3], z->top[2], &res_floor);
					}
					break;
				}
			}
			if (UorL % 4 == 1) {
				int c = int(rad * (0.75f + cos(light_yrad + deg2rad(270)) *0.25f) *255.f);
				SetDrawBright(c, c, c);
				switch (UorL) {
				case 1://横(左)
					if (z->floor[0].x < z->top[0].x)
						DrawModi_wrap(z->top[2], z->top[0], z->floor[0], z->floor[2], z->wallhandle);
					break;
				case 5://左◢
					if (z->floor[0].x < z->top[0].x)
						DrawModi_wrap(z->top[2], z->top[2], z->floor[0], z->floor[2], z->wallhandle);
					break;
				case 9://左◥
					if (z->floor[0].x < z->top[0].x)
						DrawModi_wrap(z->top[0], z->top[0], z->floor[0], z->floor[2], z->wallhandle);
					break;
				case 13:
					if (z->floor[0].x < z->top[3].x)
						DrawModi_wrap(z->top[3], z->top[1], z->floor[0], z->floor[2], z->wallhandle);
					break;
				case 17:
					if (z->floor[0].x < z->top[3].x) {
						blend_shadow(&z->top[0], &z->top[3], &z->hight, z->floorhandle);
						DrawModi_wrap(z->floor[0], z->top[1], z->top[3], z->floor[2], &res_floor);
					}
					break;
				}
			}
			if (UorL % 4 == 2) {
				int c = int(rad * (0.75f + cos(light_yrad + deg2rad(180)) * 0.25f) *255.f);//
				SetDrawBright(c, c, c);
				switch (UorL) {
				case 2://縦(下)
					if (z->floor[3].y >= z->top[3].y)
						DrawModi_wrap(z->top[2], z->top[3], z->floor[3], z->floor[2], z->wallhandle);
					break;
				case 6://下◢
					if (z->floor[3].y >= z->top[3].y)
						DrawModi_wrap(z->top[3], z->top[3], z->floor[3], z->floor[2], z->wallhandle);
					break;
				case 10://下◣
					if (z->floor[3].y >= z->top[3].y)
						DrawModi_wrap(z->top[2], z->top[2], z->floor[3], z->floor[2], z->wallhandle);
					break;
				case 14:
					if (z->floor[2].y > z->top[1].y) {
						blend_shadow(&z->top[0], &z->top[3], &z->hight, z->floorhandle);
						DrawModi_wrap(z->top[0], z->top[1], z->floor[3], z->floor[2], &res_floor);
					}
					break;
				case 18:
					if (z->floor[2].y > z->top[1].y)
						DrawModi_wrap(z->top[0], z->top[1], z->floor[3], z->floor[2], z->wallhandle);
					break;
				}
			}
			if (UorL % 4 == 3) {
				int c = int(rad * (0.75f + cos(light_yrad + deg2rad(90)) * 0.25f) *255.f);
				SetDrawBright(c, c, c);
				switch (UorL) {
				case 3://横(右)
					if (z->floor[3].x >= z->top[3].x)
						DrawModi_wrap(z->top[3], z->top[1], z->floor[1], z->floor[3], z->wallhandle);
					break;
				case 7://右◢
					if (z->floor[3].x >= z->top[3].x)
						DrawModi_wrap(z->top[3], z->top[3], z->floor[1], z->floor[3], z->wallhandle);
					break;
				case 11://右◥
					if (z->floor[3].x >= z->top[3].x)
						DrawModi_wrap(z->top[1], z->top[1], z->floor[1], z->floor[3], z->wallhandle);
					break;
				case 15:
					if (z->floor[1].x > z->top[2].x) {
						blend_shadow(&z->top[0], &z->top[3], &z->hight, z->floorhandle);
						DrawModi_wrap(z->top[0], z->floor[1], z->floor[3], z->top[2], &res_floor);
					}
					break;
				case 19:
					if (z->floor[1].x > z->top[2].x)
						DrawModi_wrap(z->top[2], z->top[0], z->floor[1], z->floor[3], z->wallhandle);
					break;
				}
			}
		}
		else {
			int c = 255 * (camhigh - std::clamp(z->hight, 0, camhigh)) / camhigh;
			SetDrawBright(c, c, c);

			if (!z->is_wall) {
				blend_shadow(&z->top[0], &z->top[3], &z->hight, z->floorhandle);
				DrawExtend_wrap(z->top[0], z->top[3], &res_floor);
			}
			else {
				DrawExtend_wrap(z->top[0], z->top[3], z->floorhandle);
			}
		}
		SetDrawBright(255, 255, 255);
	}
	//柱描画
	void Draw_Pillar(con* z){
		switch (z->use) {//三角柱
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
		//DrawFormatString(z->top[0].x, z->top[0].y, GetColor(255, 255, 255), "%d\n%d,%d", z->use, z->hight, z->bottom);
	}
	//人描画
	void Reset_Human(void) {
		for (auto& pl : human) { pl.Reset(); }
	}
	void Draw_Human(con* z) {
		for (auto& pl : human) { pl.Draw(z); }
	}
	//壁影描画
	void draw_wall_shadow(int UorL, con* z, int* hight) {//一辺
		const auto add_x = int(float(z->hight - (z->bottom + *hight))*sin(light_yrad));
		const auto add_y = int(float(z->hight - (z->bottom + *hight))*cos(light_yrad));
		const auto a00_1 = getpos(pos_t.x + tilesize * (z->cpos.x + 0) + add_x, pos_t.y + tilesize * (z->cpos.y + 0) + add_y, z->bottom + *hight);
		const auto a10_1 = getpos(pos_t.x + tilesize * (z->cpos.x + 1) + add_x, pos_t.y + tilesize * (z->cpos.y + 0) + add_y, z->bottom + *hight);
		const auto a01_1 = getpos(pos_t.x + tilesize * (z->cpos.x + 0) + add_x, pos_t.y + tilesize * (z->cpos.y + 1) + add_y, z->bottom + *hight);
		const auto a11_1 = getpos(pos_t.x + tilesize * (z->cpos.x + 1) + add_x, pos_t.y + tilesize * (z->cpos.y + 1) + add_y, z->bottom + *hight);
		const auto a00_0 = getpos(pos_t.x + tilesize * (z->cpos.x + 0), pos_t.y + tilesize * (z->cpos.y + 0), z->bottom + *hight);//◤
		const auto a10_0 = getpos(pos_t.x + tilesize * (z->cpos.x + 1), pos_t.y + tilesize * (z->cpos.y + 0), z->bottom + *hight);//◥
		const auto a01_0 = getpos(pos_t.x + tilesize * (z->cpos.x + 0), pos_t.y + tilesize * (z->cpos.y + 1), z->bottom + *hight);//◣
		const auto a11_0 = getpos(pos_t.x + tilesize * (z->cpos.x + 1), pos_t.y + tilesize * (z->cpos.y + 1), z->bottom + *hight);//◢
		if (UorL % 4 == 0) {
			switch (UorL) {
			case 0://縦(上)
				if (z->is_wall)
					DrawModi_wrap(a00_1, a10_1, a10_0, a00_0, z->wallhandle);
				break;
			case 8://上◣
				if (z->is_wall)
					DrawModi_wrap(a00_1, a00_1, a10_0, a00_0, z->wallhandle);
				break;
			case 4://上◢
				if (z->is_wall)
					DrawModi_wrap(a10_1, a10_1, a10_0, a00_0, z->wallhandle);
				break;
			case 16:
				if (a00_0.y < a01_1.y)
					DrawModi_wrap(a01_1, a11_1, a10_0, a00_0, z->floorhandle);
				break;
			case 12:
				DrawModi_wrap(a01_1, a11_1, a10_0, a00_0, z->wallhandle);
				break;
			}
		}
		if (UorL % 4 == 1) {
			switch (UorL) {
			case 1://横(左)
				if (z->is_wall)
					DrawModi_wrap(a01_1, a00_1, a00_0, a01_0, z->wallhandle);
				break;
			case 5://左◢
				if (z->is_wall)
					DrawModi_wrap(a01_1, a01_1, a00_0, a01_0, z->wallhandle);
				break;
			case 9://左◥
				if (z->is_wall)
					DrawModi_wrap(a00_1, a00_1, a00_0, a01_0, z->wallhandle);
				break;
			case 13:
				DrawModi_wrap(a11_1, a10_1, a00_0, a01_0, z->wallhandle);
				break;
			case 17:
				if (a00_0.x < a11_1.x)
					DrawModi_wrap(a11_1, a10_1, a00_0, a01_0, z->floorhandle);
				break;
			}
		}
		if (UorL % 4 == 2) {
			switch (UorL) {
			case 2://縦(下)
				if (z->is_wall)
					DrawModi_wrap(a01_1, a11_1, a11_0, a01_0, z->wallhandle);
				break;
			case 6://下◢
				if (z->is_wall)
					DrawModi_wrap(a11_1, a11_1, a11_0, a01_0, z->wallhandle);
				break;
			case 10://下◣
				if (z->is_wall)
					DrawModi_wrap(a01_1, a01_1, a11_0, a01_0, z->wallhandle);
				break;
			case 14:
				if (a01_0.y > a10_1.y)
					DrawModi_wrap(a00_1, a10_1, a11_0, a01_0, z->floorhandle);
				break;
			case 18:
				DrawModi_wrap(a00_1, a10_1, a11_0, a01_0, z->wallhandle);
				break;
			}
		}
		if (UorL % 4 == 3) {
			switch (UorL) {
			case 3://横(右)
				if (z->is_wall)
					DrawModi_wrap(a11_1, a10_1, a10_0, a11_0, z->wallhandle);
				break;
			case 7://右◢
				if (z->is_wall)
					DrawModi_wrap(a11_1, a11_1, a10_0, a11_0, z->wallhandle);
				break;
			case 11://右◥
				if (z->is_wall)
					DrawModi_wrap(a10_1, a10_1, a10_0, a11_0, z->wallhandle);
				break;
			case 15:
				if (a10_0.x > a01_1.x)
					DrawModi_wrap(a01_1, a00_1, a10_0, a11_0, z->floorhandle);
				break;
			case 19:
				DrawModi_wrap(a01_1, a00_1, a10_0, a11_0, z->wallhandle);
				break;
			}
		}
	}
	//柱の影描画
	void drw_rect_shadow(con* z,int* hight) {
		switch (z->use) {//三角柱
		case 0://上
			draw_wall_shadow(12, z, hight);		//縦(上)12
			draw_wall_shadow(5, z, hight);		//横(左)
			draw_wall_shadow(2, z, hight);		//縦(下)
			draw_wall_shadow(7, z, hight);		//横(右)
			break;
		case 1://左
			draw_wall_shadow(4, z, hight);		//縦(上)4
			draw_wall_shadow(13, z, hight);		//横(左)13
			draw_wall_shadow(6, z, hight);		//縦(下)
			draw_wall_shadow(3, z, hight);		//横(右)
			break;
		case 2://下
			draw_wall_shadow(0, z, hight);		//縦(上)
			draw_wall_shadow(9, z, hight);		//横(左)9
			draw_wall_shadow(18, z, hight);		//縦(下)18
			draw_wall_shadow(11, z, hight);		//横(右)
			break;
		case 3://右
			draw_wall_shadow(8, z, hight);		//縦(上)
			draw_wall_shadow(1, z, hight);		//横(左)
			draw_wall_shadow(10, z, hight);		//縦(下)
			draw_wall_shadow(19, z, hight);		//横(右)
			break;
		case 4://上
			draw_wall_shadow(16, z, hight);		//縦(上)//4
			break;
		case 5://左
			draw_wall_shadow(17, z, hight);		//横(左)
			break;
		case 6://下
			draw_wall_shadow(14, z, hight);		//縦(下)
			break;
		case 7://右
			draw_wall_shadow(15, z, hight);		//横(右)
			break;
		default://柱
			draw_wall_shadow(0, z, hight);		//縦(上)
			draw_wall_shadow(1, z, hight);		//横(左)
			draw_wall_shadow(2, z, hight);		//縦(下)
			draw_wall_shadow(3, z, hight);		//横(右)
			break;
		}
	}
	//影を一部分描画
	void ready_rect_shadow(void) {
		const auto limmin = getpos(-deskx * 3 / 4, -desky * 3 / 4, 0);
		const auto limmax = getpos(deskx * 3 / 4, desky * 3 / 4, 0);
		int tg = GetDrawScreen();
		int high = 0;
		for (auto& g : shadow_graph) {
			g.SetDraw_Screen();
			SetDrawBright(0, 0, 0);
			for (auto& c : zcon) {
				for (auto& z : c) {
					if (z.zero[0].y <= limmax.y  && z.zero[0].x <= limmax.x && z.zero[3].y >= limmin.y && z.zero[3].x >= limmin.x) {
						if (z.is_wall) {
							drw_rect_shadow(&z, &high);
						}
						else {
							for (auto& pl : human) {
								pl.Draw_Shadow(&z, light_yrad);
							}
						}
					}
				}
			}
			high += 8;
		}
		SetDrawBright(255, 255, 255);
		SetDrawScreen(tg);
	}
	//人と壁の判定
	void hit_wall(pos2D *m_pos, pos2D& old) {
		m_pos->x = std::clamp(m_pos->x, tilesize / 4, tilesize * int(zcon.size())- tilesize / 4);
		m_pos->y = std::clamp(m_pos->y, tilesize / 4, tilesize * int(zcon.back().size())- tilesize / 4);

		for (auto& c : zcon) {
			for (auto& z : c) {
				if (z.is_wall) {
					const auto x0 = tilesize * z.cpos.x - tilesize / 4;
					const auto y0 = tilesize * z.cpos.y - tilesize / 4;
					const auto x1 = tilesize * z.cpos.x + tilesize * 5 / 4;
					const auto y1 = tilesize * z.cpos.y + tilesize * 5 / 4;
					//*
					auto a = *m_pos - old;
					{
						pos2D s = { x0,y0 };
						pos2D p = { x0 - s.x ,y1 - s.y };
						if (ColSeg(old, a, s, p)) {
							auto pr = p.hydist();
							m_pos->x -= -p.y * p.cross(&a) / pr;
							m_pos->y -= p.x * p.cross(&a) / pr;
							break;
						}
					}
					{
						pos2D s = { x1,y0 };
						pos2D p = { x0 - s.x ,y0 - s.y };
						if (ColSeg(old, a, s, p)) {
							auto pr = p.hydist();
							m_pos->x -= -p.y * p.cross(&a) / pr;
							m_pos->y -= p.x * p.cross(&a) / pr;
							break;
						}
					}
					{
						pos2D s = { x1 ,y1 };
						pos2D p = { x1 - s.x ,y0 - s.y };
						if (ColSeg(old, a, s, p)) {
							auto pr = p.hydist();
							m_pos->x -= -p.y * p.cross(&a) / pr;
							m_pos->y -= p.x * p.cross(&a) / pr;
							break;
						}
					}
					{
						pos2D s = { x0,y1 };
						pos2D p = { x1 - s.x ,y1 - s.y };
						if (ColSeg(old, a, s, p)) {
							auto pr = p.hydist();
							m_pos->x -= -p.y * p.cross(&a) / pr;
							m_pos->y -= p.x * p.cross(&a) / pr;
							break;
						}
					}
					//*/
				}
			}
		}
	}
public:
	//コンストラクタ
	Draw(void) {
		for (auto& g : shadow_graph) {
			g = GraphHandle::Make(deskx, desky, true);
		}
		screen = GraphHandle::Make(deskx, desky, false);
		res_floor = GraphHandle::Make(16, 16, true);
	}
	//デストラクタ
	~Draw(void) {
		exit_map();
	}
	//button
	template <typename T>
	void button_set(int& mousex, int& mousey, int xs, int ys, int xsize, int ysize, const char* buf, bool on, size_t *cnt, T doing) {
		if (on) {
			if (in2_(mousex, mousey, xs, ys, xs + xsize, ys + ysize)) {
				*cnt = std::min<size_t>(*cnt + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0);
				if (*cnt == 1) {
					doing();
				}
			}
			else {
				*cnt = 2;
			}
		}
		DrawBox(xs, ys, xs + xsize, ys + ysize, on ? ((in2_(mousex, mousey, xs, ys, xs + xsize, ys + ysize)) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
		Fonts.Get(y_r(40)).Get_handle().DrawString_MID(xs + xsize / 2, ys, buf, on ? ((in2_(mousex, mousey, xs, ys, xs + xsize, ys + ysize)) ? GetColor(255, 255, 0) : GetColor(255, 0, 0)) : GetColor(0, 0, 0));
	}
	//up_down
	template <typename T1, typename T2>
	void up_down_set(int& mousex, int& mousey, int xs, int ys, const char* buf, size_t *upcnt, size_t *downcnt, T1 doing1, T2 doing2) {
		//int xs = int(x_size * desky / std::max(x_size, y_size));
		//int ys = y_r(480);
		bool on = true;
		int x_size1 = Fonts.Get(y_r(40)).Get_handle().GetDrawWidth(buf);
		int xsize = x_r(40);
		int ysize = y_r(30);
		//高
		{
			if (in2_(mousex, mousey, xs + x_size1, ys, xs + x_size1 + xsize, ys + ysize)) {
				*upcnt = std::min<size_t>(*upcnt + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0);
				if (*upcnt == 1) {
					doing1();
				}
			}
			else {
				*upcnt = 2;
			}
		}
		{
			if (in2_(mousex, mousey, xs + x_size1, ys + y_r(40), xs + x_size1 + xsize, ys + y_r(40) + ysize)) {
				*downcnt = std::min<size_t>(*downcnt + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0);
				if (*downcnt == 1) {
					doing2();
				}
			}
			else {
				*downcnt = 2;
			}
		}
		Fonts.Get(y_r(40)).Get_handle().DrawString(xs, ys + y_r(15), buf, GetColor(255, 255, 0));
		DrawTriangle(xs + x_size1 + xsize / 2, ys, xs + x_size1, ys + ysize, xs + x_size1 + xsize, ys + ysize, on ? (((in2_(mousex, mousey, xs + x_size1, ys, xs + x_size1 + xsize, ys + ysize))) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
		DrawTriangle(xs + x_size1, ys + y_r(40), xs + x_size1 + xsize, ys + y_r(40), xs + x_size1 + xsize / 2, ys + y_r(40) + ysize, on ? (((in2_(mousex, mousey, xs + x_size1, ys + y_r(40), xs + x_size1 + xsize, ys + y_r(40) + ysize))) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
	}
	//mapエディター
	bool write_map(std::string mapname) {
		using namespace std::literals;
		std::fstream file;
		bool wallorfloor = false;
		size_t wofcnt = 0, smzcnt = 0, floortex = 0, walltex = 0,mscnt = 0, cslcnt = 0;

		//ダイアログ用
		static TCHAR strFile[MAX_PATH], cdir[MAX_PATH], *ansFile;
		static OPENFILENAME ofn = { 0 };
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
		std::vector<Status> n;
		maps mapdata;
		std::vector<Player_Info> e;
		size_t x_size = 0, y_size = 0;
		{
			size_t okcnt = 0, ngcnt = 0;
			bool read = false;
			while (ProcessMessage() == 0) {
				int mousex, mousey;
				GetMousePoint(&mousex, &mousey);

				SetDrawScreen(DX_SCREEN_BACK);
				ClearDrawScreen();
				//
				DrawBox(x_r(960 - 320), y_r(540 - 180), x_r(960 + 320), y_r(540 + 180), GetColor(128, 128, 128), TRUE);
				Fonts.Get(y_r(40)).Get_handle().DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "プリセットを読み込みますか?", GetColor(255, 255, 0));
				//OK
				button_set(mousex, mousey, x_r(960 + 320 - 340), y_r(540 + 180 - 140), x_r(300), y_r(40), "OK", true, &okcnt, [&okcnt]() {okcnt = 3;  });
				if (okcnt == 3) {
					okcnt = 2;
					read = true;
					break;
				}
				//NG
				button_set(mousex, mousey, x_r(960 + 320 - 340), y_r(540 + 180 - 80), x_r(300), y_r(40), "NG", true, &ngcnt, [&ngcnt]() {ngcnt = 3;  });
				if (ngcnt == 3) {
					ngcnt = 2;
					break;
				}
				//
				ScreenFlip();
			}
			//map読み込み
			if (!read) {
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
						size_t upx = 0, dnx = 0, upy = 0, dny = 0;
						while (ProcessMessage() == 0) {
							SetDrawScreen(DX_SCREEN_BACK);
							ClearDrawScreen();
							int mousex, mousey;
							GetMousePoint(&mousex, &mousey);

							DrawBox(x_r(960 - 320), y_r(540 - 180), x_r(960 + 320), y_r(540 + 180), GetColor(128, 128, 128), TRUE);
							Fonts.Get(y_r(40)).Get_handle().DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "マップのサイズは?", GetColor(255, 255, 0));

							{
								//高
								up_down_set(mousex, mousey, x_r(960 - 320 + 40), y_r(540 - 180 + 60 + 100), ("X : "s + std::to_string(x_size)).c_str(), &upx, &dnx, [&x_size]() {
									x_size++;
								}, [&x_size]() {
									if (x_size > 1) {
										x_size--;
									}
								});
								//底面
								up_down_set(mousex, mousey, x_r(960 -320+ 40), y_r(540 - 180 + 60 + 100 + 115), ("Y : "s + std::to_string(y_size)).c_str(), &upy, &dny, [&y_size]() {
									y_size++;
								}, [&y_size]() {
									if (y_size > 1)
										y_size--;
								});
							}

							{
								int xsz = x_r(280);
								int ysz = y_r(120);
								int xm = x_r(1100);
								int ym = y_r(540);
								if (x_size*ysz / xsz >= y_size) {
									ysz = xsz * int(y_size) / int(x_size);
								}
								else {
									xsz = ysz * int(x_size) / int(y_size);
								}
								DrawBox(xm - xsz / 2, ym - ysz / 2, xm + xsz / 2, ym + ysz / 2, GetColor(255, 255, 0), FALSE);
							}
							//終了
							button_set(mousex, mousey, x_r(960 + 320 - 340), y_r(540 + 180 - 80), x_r(300), y_r(40), "OK", true, &mscnt, [&mscnt]() {mscnt = 3;  });
							if (mscnt == 3) {
								mscnt = 2;
								break;
							}
							//
							ScreenFlip();
						}

					}
					for (int y = 0; y < y_size; y++) {
						for (int x = 0; x < x_size; x++) {
							const int btm = 0;
							const int mid = 0;
							const int hig = 64;
							n.resize(n.size() + 1);
							n.back() = { get_pos2D(x,y), btm,mid, false,2, 2, 255 };
						}
					}

					//(マップチップ)
					for (int y = 0; y < y_size; y += 5) {
						for (int x = 0; x < x_size; x += 5) {
							const int btm = 0;
							const int mid = 0;
							const int hig = 64;
							const size_t s = size_t(x) + size_t(y) * x_size;
							/*
							n[s + 2 + size_t(x_size)*1] = { get_pos2D(x + 2, y + 1), btm, hig,true, 2, 2, -1 };
							n[s + 1 + size_t(x_size)*2] = { get_pos2D(x + 1, y + 2), btm, hig,true, 2, 2, -1 };
							n[s + 2 + size_t(x_size)*2] = { get_pos2D(x + 2, y + 2), btm, hig,true, 2, 2, -1 };
							n[s + 3 + size_t(x_size)*2] = { get_pos2D(x + 3, y + 2), btm, hig,true, 2, 2, -1 };
							n[s + 2 + size_t(x_size)*3] = { get_pos2D(x + 2, y + 3), btm, hig,true, 2, 2, -1 };
							//*/
							//*
							n[s + 2 + x_size * 1] = { get_pos2D(x + 2, y + 1), btm, hig,true, 2, 2, 0 };
							n[s + 1 + x_size * 2] = { get_pos2D(x + 1, y + 2), btm, hig,true, 2, 2, 1 };
							n[s + 2 + x_size * 2] = { get_pos2D(x + 2, y + 2), btm, hig,true, 2, 1, 255 };
							n[s + 3 + x_size * 2] = { get_pos2D(x + 3, y + 2), btm, hig,true, 2, 2, 3 };
							n[s + 2 + x_size * 3] = { get_pos2D(x + 2, y + 3), btm, hig,true, 2, 2, 2 };
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
						e.back().pos_p.x = tilesize * 5 * i + tilesize / 2;
						e.back().pos_p.y = tilesize * 5 * i + tilesize / 2;

					}
				}
			}
		}
		//エディター
		int hight_s = 64, bottom_s = 0,cam_high = camhigh;
		size_t upx = 2, dnx = 2, upy = 2, dny = 2;
		size_t undo = 2, redo = 2;
		bool save = false;
		bool smz = false;
		std::list<std::vector<Status>> n_list;

		n_list.push_back(n);
		auto itr = n_list.end();
		while (ProcessMessage() == 0) {
			SetDrawScreen(DX_SCREEN_BACK);
			ClearDrawScreen();
			int mousex, mousey;
			GetMousePoint(&mousex, &mousey);
			//マップ描画
			{
				for (auto& m : n) {
					const int xs = desky / 40 + int(m.pos.x * desky * 38 / 40 / std::max(x_size, y_size));
					const int ys = desky / 40 + int(m.pos.y * desky * 38 / 40 / std::max(x_size, y_size));
					const int xe = desky / 40 + int((m.pos.x + 1)*desky * 38 / 40 / std::max(x_size, y_size));
					const int ye = desky / 40 + int((m.pos.y + 1)*desky * 38 / 40 / std::max(x_size, y_size));
					const unsigned char mh = (unsigned char)(255 * (camhigh - abs(m.hight)) / camhigh);
					const unsigned char mb = (unsigned char)(255 * (camhigh - abs(m.bottom)) / camhigh);

					if (in2_(mousex, mousey, xs, ys, xe, ye)) {
						if (m.is_wall) {
							Grad_Box(xs, ys, xe, ye, mh, mh / 2, 0u, mb, mb / 2, 0u, m.dir);
						}
						else {
							Grad_Box(xs, ys, xe, ye, mh, mh / 2, mh / 2, mb, mb / 2, mb / 2, m.dir - 4);
						}
						Fonts.Get(y_r(40)).Get_handle().DrawStringFormat(int(x_size*desky / std::max(x_size, y_size)), y_r(40), GetColor(255, 255, 255), "(%03d,%03d)", m.pos.x, m.pos.y);

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
									const int xs2 = desky / 40 + int((m.pos.x +
										(
										(i == 0) ? 0 :
											(i == 1) ? 1 :
											(i == 2) ? 0 : -1
											)
										) * desky * 38 / 40 / std::max(x_size, y_size));
									const int ys2 = desky / 40 + int((m.pos.y +
										(
										(i == 0) ? 1 :
											(i == 1) ? 0 :
											(i == 2) ? -1 : 0
											)
										) * desky * 38 / 40 / std::max(x_size, y_size));
									const int xe2 = desky / 40 + int((m.pos.x + 1 +
										(
										(i == 0) ? 0 :
											(i == 1) ? 1 :
											(i == 2) ? 0 : -1
											)
										)*desky * 38 / 40 / std::max(x_size, y_size));
									const int ye2 = desky / 40 + int((m.pos.y + 1 +
										(
										(i == 0) ? 1 :
											(i == 1) ? 0 :
											(i == 2) ? -1 : 0
											)
										)*desky * 38 / 40 / std::max(x_size, y_size));
									if (in2_(mousex, mousey, xs2, ys2, xe2, ye2)) {
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
				DrawCircle(desky / 40 + mapdata.plx * int(desky * 38 / 40 / std::max(x_size, y_size)) / tilesize, desky / 40 + mapdata.ply * int(desky * 38 / 40 / std::max(x_size, y_size)) / tilesize, y_r(desky * 38 / 40 / std::max(x_size, y_size)), GetColor(0, 255, 0));
				for (auto& m : e) {
					DrawCircle(
						desky / 40 + m.pos_p.x * int(desky * 38 / 40 / std::max(x_size, y_size)) / tilesize,
						desky / 40 + m.pos_p.y * int(desky * 38 / 40 / std::max(x_size, y_size)) / tilesize,
						y_r(desky / std::max(x_size, y_size)),
						GetColor(255, 0, 0));
				}
			}
			//壁か床か
			button_set(mousex, mousey, int(x_size * desky / std::max(x_size, y_size)), y_r(80), x_r(400), y_r(40), "選択タイルを変更", true, &wofcnt, [&wallorfloor]() {wallorfloor ^= 1;  });
			Fonts.Get(y_r(40)).Get_handle().DrawString(int(x_size * desky / std::max(x_size, y_size)), y_r(80) + y_r(40), wallorfloor ? "壁を選択中" : "床を選択中", GetColor(255, 0, 0));
			//壁か床か
			button_set(mousex, mousey, int(x_size * desky / std::max(x_size, y_size)), y_r(180), x_r(400), y_r(40), "地形編集", true, &smzcnt, [&smz]() {smz ^= 1;  });
			Fonts.Get(y_r(40)).Get_handle().DrawString(int(x_size * desky / std::max(x_size, y_size)), y_r(180) + y_r(40), smz ? "台形" : "矩形", GetColor(255, 0, 0));
			//床テクスチャ
			button_set(mousex, mousey, int(x_size * desky / std::max(x_size, y_size)), y_r(280), x_r(400), y_r(40), "床テクスチャ選択", GetWindowModeFlag() == TRUE, &floortex,[&mapdata]() {
				if (GetOpenFileName(&ofn)) {
					std::string str = strFile;
					if (str.find(cdir) != std::string::npos) {
						ansFile = &strFile[strlen(cdir) + 1];
						strcpy_s(mapdata.floor_name, ansFile);
					}
					else {
						strcpy_s(mapdata.floor_name, strFile);//フルパス
					}
				}
			});
			Fonts.Get(y_r(30)).Get_handle().DrawString(int(x_size * desky / std::max(x_size, y_size)), y_r(280) + y_r(40), mapdata.floor_name, GetColor(255, 0, 0));
			//壁テクスチャ
			button_set(mousex, mousey, int(x_size * desky / std::max(x_size, y_size)), y_r(380), x_r(400), y_r(40), "壁テクスチャ選択", GetWindowModeFlag() == TRUE, &walltex,[&mapdata]() {
				if (GetOpenFileName(&ofn)) {
					std::string str = strFile;
					if (str.find(cdir) != std::string::npos) {
						ansFile = &strFile[strlen(cdir) + 1];
						strcpy_s(mapdata.wall_name, ansFile);
					}
					else {
						strcpy_s(mapdata.wall_name, strFile);//フルパス
					}
				}
			});
			Fonts.Get(y_r(30)).Get_handle().DrawString(int(x_size * desky / std::max(x_size, y_size)), y_r(380) + y_r(40), mapdata.wall_name, GetColor(255, 0, 0));
			//設定する高さ
			{
				//高
				up_down_set(mousex, mousey, int(x_size * desky / std::max(x_size, y_size)), y_r(480), ("設定する高さ : "s + std::to_string(hight_s)).c_str(), &upx, &dnx,[&hight_s , cam_high]() {
						if (hight_s < cam_high) {
							hight_s += 8;
						}
						else {
							hight_s = cam_high;
						}
					},[&hight_s, cam_high]() {
						if (hight_s > -cam_high) {
							hight_s -= 8;
						}
						else {
							hight_s = -cam_high;
						}
					});
				bottom_s = std::min(bottom_s, hight_s - 8);
				//底面
				up_down_set(mousex, mousey, int(x_size * desky / std::max(x_size, y_size)), y_r(580+15), ("設定する底面 : "s + std::to_string(bottom_s)).c_str(), &upy, &dny,[&bottom_s,&hight_s, cam_high]() {
						if (bottom_s < cam_high-8) {
							bottom_s += 8;
							hight_s = std::max(bottom_s + 8, hight_s);
						}
						else {
							bottom_s = cam_high;
						}
					},[&bottom_s, cam_high]() {
						if (bottom_s > -cam_high) {
							bottom_s -= 8;
						}
						else {
							bottom_s = -cam_high;
						}
					});
			}
			//アンドゥ
			button_set(mousex, mousey, int(x_size * desky / std::max(x_size, y_size)), y_r(680), x_r(100), y_r(40), "戻る", (n_list.size() >= 2 && itr != n_list.begin()), &undo, [&itr, &n]() {itr--; n = *itr;  });
			//リドゥ
			button_set(mousex, mousey, int(x_size * desky / std::max(x_size, y_size)) + x_r(150), y_r(680), x_r(100), y_r(40), "進む", (n_list.size() >= 2 && std::next(itr, 1) != n_list.end()), &redo, [&itr, &n]() {itr++; n = *itr;  });
			//終了
			button_set(mousex, mousey, x_r(1920 - 340), y_r(1080 - 160), x_r(300), y_r(40), "保存せず終了", true, &cslcnt, [&cslcnt]() {cslcnt = 3;  });
			if (cslcnt == 3) {
				return false;
			}
			//終了
			button_set(mousex, mousey, x_r(1920 - 340), y_r(1080 - 80), x_r(300), y_r(40), "保存して続行", true, &mscnt, [&mscnt]() {mscnt = 3;  });
			if (mscnt == 3) {
				break;
			}
			//
			ScreenFlip();
		}
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
	void set_map(int *player_x, int *player_y, std::string mapname) {
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
			//GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, tilesize, tilesize * 2, &walls);
			//GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, tilesize, tilesize, &floors);
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
				human.back().First(anse.pos_p.x, anse.pos_p.y);
			} while (!file.eof());
			file.close();
		}
	}
	//カメラ座標設定
	void set_cam(int sx, int sy) {
		pos_t.set(sx,sy);
		put_map();
		return;
	}
	//人の移動処理
	void move_human(pos2D* m_pos) {
		for (auto& pl : human) {
			bool isPlayer = ((size_t)(&pl - &human.front()) == player_id);

			pl.vec_buf = pl.pos;
			if (!isPlayer) {
				//todo : cpu
				//pl.pos.x = 100*i;
				//pl.pos.y = 100*i;
			}
			else {
				//自機の移動
				pl.pos = (*m_pos) * -1.f;
			}
			hit_wall(&pl.pos, pl.vec_buf);
			if (isPlayer) {
				*m_pos = pl.pos * -1.f;
			}
			pl.vec_buf -= pl.pos;

			pl.hight = zcon[std::max(pl.pos.x / tilesize, 0)][std::max(pl.pos.y / tilesize, 0)].hight;
		}
	}
	//更新
	void Update(void) {
		//人の描画用意
		for (auto& pl : human) { pl.Set(pos_t); }
		//一気に描画
		{
			const auto limmin = getpos(-deskx / 2, -desky / 2, 0);
			const auto cam = getpos(0, 0, 0);
			const auto limmax = getpos(deskx / 2, desky / 2, 0);
			//light_yrad += deg2rad(1);
			//影
			ready_rect_shadow();
			//DRAW
			screen.SetDraw_Screen();
			{
				for (auto& c : zcon) {
					Reset_Human();
					for (auto& z : c) {
						if (z.top[0].x <= cam.x && z.top[0].y <= cam.y && z.zero[3].y >= limmin.y && z.zero[3].x >= limmin.x) {
							Draw_Pillar(&z);
							Draw_Human(&z);
						}
					}
					Reset_Human();
					for (int y = int(c.size()) - 1; y >= 0; --y) {
						auto& z = c[y];
						if (z.top[0].x <= cam.x && z.top[3].y >= cam.y && z.zero[0].y <= limmax.y  && z.zero[3].x >= limmin.x) {
							Draw_Pillar(&z);
							Draw_Human(&z);
						}
					}
				}
				for (int x = int(zcon.size()) - 1; x >= 0; --x) {
					Reset_Human();
					for (auto& z : zcon[x]) {
						if (z.top[3].x >= cam.x && z.top[0].y <= cam.y && z.zero[3].y >= limmin.y && z.zero[0].x <= limmax.x) {
							Draw_Pillar(&z);
							Draw_Human(&z);
						}
					}
					Reset_Human();
					for (int y = int(zcon[x].size()) - 1; y >= 0; --y) {
						auto& z = zcon[x][y];
						if (z.top[3].x >= cam.x && z.top[3].y >= cam.y&& z.zero[0].y <= limmax.y  && z.zero[0].x <= limmax.x) {
							Draw_Pillar(&z);
							Draw_Human(&z);
						}
					}
				}
			}
		}
	}
	//出力
	void out_draw(void) {
		screen.DrawGraph(0, 0, true);
	}

	//mapの後始末
	void exit_map(void) {
		walls.clear();
		floors.clear();
		human.clear();
		for (auto& z : zcon) {
			z.clear();
		}
		zcon.clear();
		return;
	}
};
