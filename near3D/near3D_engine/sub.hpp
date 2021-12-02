#pragma once
#include "DXLib_ref/DXLib_ref.h"

FontPool Fonts;

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

int camhigh_r = camhigh;
int tilesize_r = tilesize;
int cam_high = camhigh_r;
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
	class Stat {
	public:
		pos2D pos_tile;
		int bottom = 0, hight = 0;
		bool is_wall = false;
	};
	class TileStatus : public Stat {
	public:
		int wall_id = 0, floor_id = 0;
		unsigned char dir = 0;
	};

	class Edit {
	public:
		std::list<std::vector<TileStatus>> List;//
		std::list<std::vector<TileStatus>>::iterator ListItr;
		std::vector<TileStatus> Data;
		//Dataを保存
		void Save() {
			if (this->ListItr == this->List.end()--) {
				this->List.push_back(this->Data);
				this->ListItr = this->List.end();
			}
			else {
				this->ListItr++;
				this->List.insert(this->ListItr, this->Data);
				this->ListItr = this->List.erase(this->ListItr, this->List.end());
			}
			this->ListItr--;
		}
		//Undo
		bool CanUndo() { return this->List.size() >= 2 && this->ListItr != this->List.begin(); }
		void Undo() {
			this->ListItr--;
			this->Data = *this->ListItr;
		}
		//Redo
		bool CanRedo() { return this->List.size() >= 2 && std::next(this->ListItr, 1) != this->List.end(); }
		void Redo() {
			this->ListItr++;
			this->Data = *this->ListItr;
		}
		//mapデータ1書き込み(マップチップ)
		void Write(const std::string& name) {
			std::fstream file;
			file.open(name.c_str(), std::ios::binary | std::ios::out);
			for (auto& m : this->Data) {
				file.write((char*)&m, sizeof(m));
			}
			file.close();
			this->Data.clear();
		}
	};
	class Tiles : public Stat {
	public:
		std::array<pos2D, 4> top;
		std::array<pos2D, 4> floor;
		std::array<pos2D, 4> zero;
		unsigned char use;// rect = -1 else prism = 0~3,4~7
		GraphHandle* wallhandle;
		GraphHandle* floorhandle;

		const bool Xin(int x) const noexcept { return x >= top[0].x && x <= top[3].x; }
		const bool Yin(int y) const noexcept { return y >= top[0].y && y <= top[3].y; }
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
			pos2D pos;
			int hight;
			float xr, yr, zr;
			float xrad, yrad, zrad;
			float xdist, ydist, zdist;
			bool edit;
		public:
			void SetDist(float m_xdist, float  m_ydist, float  m_zdist) {
				tilesize_r = y_r(tilesize);
				xdist = m_xdist * tilesize_r / 32 * 5 / 6;
				ydist = m_ydist * tilesize_r / 32 * 10;
				zdist = m_zdist;
			}
			void Leap_Rad(const Bonesdata& now, const Bonesdata& next, float per) {
				this->xrad = now.xrad + (next.xrad - now.xrad) * per;
				this->yrad = now.yrad + (next.yrad - now.yrad) * per;
				this->zrad = now.zrad + (next.zrad - now.zrad) * per;
			}
			void SetBoneData(std::string_view ttt, float rad) {
				if (ttt.find("x") != std::string::npos || ttt.find("X") != std::string::npos) {
					this->xrad = rad;
				}
				else if (ttt.find("y") != std::string::npos || ttt.find("Y") != std::string::npos) {
					this->yrad = rad;
				}
				else if (ttt.find("z") != std::string::npos || ttt.find("Z") != std::string::npos) {
					this->zrad = rad;
				}
			}
		};
		class Animesdata {
		public:
			int time = 0;
			std::vector<Bonesdata> bone;
			void SetBoneData(int sel, std::string_view ttt, float rad) {				this->bone[sel].SetBoneData(ttt, rad);			}
		};
		class foots {
		public:
			size_t ID;
			pos2D pos;
			float yr;
			float yrad;
			float Time = 0.f;
			float MaxTime = 5.f;
		public:
			void Set(Bonesdata& foot, const pos2D& m_pos, size_t ID_t) {
				ID = ID_t;
				pos = foot.pos + m_pos;
				yr = foot.yr;
				yrad = foot.yrad;
				Time = MaxTime;
			}
		};
	private:
		std::vector<GraphHandle> Graphs;
		std::vector<Bonesdata> bone;
		typedef std::pair<size_t, int> pairs;
		std::vector<pairs> sort;
		std::vector<foots> foot_v;
		size_t prev_foot = SIZE_MAX;
		std::vector<bool> draw_ok = { false };
		bool draw_end = false;
		int animeframe = 0;
		int animetime = 1;
		int animesel = 0;
		float changingtime = 0.f;
		float yrad_aim = 0;
		std::vector<std::vector<Animesdata>> anime;
		pos2D spawnpos;
		pos2D vec_real;//キャラスプライトが実際に向いている向き
		float y_rad = 0.f;
		float foottime = 0;
	public:
		pos2D pos;
		pos2D vec;//移動方向
		pos2D vec_buf;//移動方向
		int vecrange = 100000;

		bool standup = false;
		bool isStand = true;
		bool changing = false;
	public:
		void Reset() {
			std::fill<>(this->draw_ok.begin(), this->draw_ok.end(), false);
			this->draw_end = false;
		}
	public:
		//開始
		void Init(int xp, int yp) {
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
						n_t.back().SetDist(0.0f, 0.0f, -2.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 2;
						n_t.back().SetDist(0.0f, 0.0f, -5.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 3;
						n_t.back().SetDist(0.0f, 0.0f, -4.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 4;
						n_t.back().SetDist(0.0f, 0.0f, -5.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 5;
						n_t.back().SetDist(-9.0f, 0.0f, 0.0f);
					}
					n_t.resize(n_t.size() + 1);
					n_t.back().parent = 15;
					n_t.back().SetDist(0.0f, 0.0f, -3.0f);
					{//右腕
						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 5;
						n_t.back().SetDist(9.0f, 0.0f, 0.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 6;
						n_t.back().SetDist(0.0f, 0.0f, -5.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 7;
						n_t.back().SetDist(0.0f, 0.0f, -4.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 8;
						n_t.back().SetDist(0.0f, 0.0f, -5.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 9;
						n_t.back().SetDist(0.0f, 0.0f, -2.0f);
					}
				}
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);

				n_t.resize(n_t.size() + 1);
				n_t.back().parent = -1;
				n_t.back().SetDist(0.0f, 0.0f, 0.0f);

				n_t.resize(n_t.size() + 1);
				n_t.back().parent = 5;
				n_t.back().SetDist(0.0f, 0.0f, -3.0f);

				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);
				n_t.resize(n_t.size() + 1);

				{
					{
						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 23;
						n_t.back().SetDist(0.0f, 0.0f, -2.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 24;
						n_t.back().SetDist(0.0f, 0.0f, -6.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 25;
						n_t.back().SetDist(0.0f, 0.0f, -5.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 26;
						n_t.back().SetDist(2.0f, 0.0f, -4.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 27;
						n_t.back().SetDist(-5.0f, 0.0f, -3.0f);
					}
					n_t.resize(n_t.size() + 1);
					n_t.back().parent = 16;
					n_t.back().SetDist(0.0f, 0.0f, -3.0f);
					{
						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 27;
						n_t.back().SetDist(5.0f, 0.0f, -3.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 28;
						n_t.back().SetDist(-2.0f, 0.0f, -4.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 29;
						n_t.back().SetDist(0.0f, 0.0f, -5.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 30;
						n_t.back().SetDist(0.0f, 0.0f, -6.0f);

						n_t.resize(n_t.size() + 1);
						n_t.back().parent = 31;
						n_t.back().SetDist(0.0f, 0.0f, -2.0f);
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
			this->vec_real.set(0, 1);
		}
		//更新
		void Set_Aim(int x, int y) {
			float rad = std::atan2f((float)x, (float)-y) - this->y_rad;
			easing_set(&yrad_aim, std::clamp(std::atan2f(sin(rad), cos(rad)), deg2rad(-45), deg2rad(45)), 0.9f);
		}
		void Update(pos2D camerapos) {
			for (auto& g : this->bone) { g.edit = false; }

			//ここでアニメーション
			{
				auto& anim = this->anime[this->animesel];
				auto& now = anim[this->animeframe];
				auto& next = anim[(this->animeframe + 1) % (int)(anim.size())];
				if (this->animetime < now.time) {
					for (int b = 0; b < this->bone.size(); b++) {
						this->bone[b].Leap_Rad(now.bone[b], next.bone[b], (float)this->animetime / (float)now.time);
					}
					this->animetime++;
				}
				else {
					++this->animeframe %= (int)(anim.size());
					this->animetime = 0;
				}
			}
			{
				auto o = this->animesel;
				if (!changing) {
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
				}
				else {
					changingtime += 1.f / FPS;
					this->animesel = 5;//しゃがみ
					if (changingtime >= 0.1f) {
						changingtime = 0.f;
						changing = false;
					}
				}
				if (o != this->animesel) {
					this->animeframe = 0;
					this->animetime = 0;
				}
			}
			//
			if (this->vec_buf.x != 0 || this->vec_buf.y != 0) {
				this->vec_real = this->vec_buf;
			}
			{
				//移動方向に向く
				vec.set((int)(-sin(this->y_rad) * this->vecrange), (int)(cos(this->y_rad) * this->vecrange));//intで保持しているためvecrange倍
				auto b = (int)(sqrt(this->vec_real.hydist()) * this->vecrange);
				auto q = this->vec_real.cross(vec);
				if (q > sin(deg2rad(10)) * b) {
					this->y_rad -= deg2rad(5);
				}
				else if (q < sin(deg2rad(10)) * -b) {
					this->y_rad += deg2rad(5);
				}
				//真後ろ振り向き
				if (this->vec_real.dot(vec) <= -0.5 * b) {
					this->y_rad += deg2rad(10);
				}
			}
			//座標指定
			bool next = false;
			do {
				next = false;
				for (auto& b : this->bone) {
					auto p = b.parent;
					if (!b.edit) {
						if (p == -1) {
							b.pos = camerapos;
							b.hight = 0;
							b.xr = b.xrad;
							b.yr = b.yrad + this->y_rad + yrad_aim * 2;
							b.zr = b.zrad;
							b.edit = true;
						}
						else {
							if (this->bone[p].edit) {
								tilesize_r = y_r(tilesize);
								const float zd = b.zdist * tilesize_r / 32;
								const auto zd2 = y_r(b.zdist);

								b.xr = this->bone[p].xrad + this->bone[p].xr;
								b.yr = this->bone[p].yrad + this->bone[p].yr;
								if (p == 15 || p == 16) {
									b.yr -= yrad_aim;
								}
								b.zr = this->bone[p].zrad + this->bone[p].zr;

								float y1 = cos(b.xr) * b.ydist + sin(b.xr) * zd;
								float z1 = cos(b.xr) * zd2 - sin(b.xr) * b.ydist;
								float x2 = cos(b.zr) * b.xdist + sin(b.zr) * z1;

								b.pos = this->bone[p].pos;
								b.pos.x += (int)(cos(b.yr) * x2 - sin(b.yr) * y1);
								b.pos.y += (int)(cos(b.yr) * y1 + sin(b.yr) * x2);
								b.hight = this->bone[p].hight + (int)(cos(b.zr) * z1 - sin(b.zr) * b.xdist);
								b.edit = true;
							}
							else {
								next = true;
							}
						}
					}
				}
			} while (next);

			//高さでソート
			for (int i = 0; i < this->sort.size(); i++) { this->sort[i] = { i, this->bone[i].hight }; }
			std::sort(this->sort.begin(), this->sort.end(), [](const pairs& x, const pairs& y) { return x.second < y.second; });
			//一番低い場所に跡を置く
			{
				size_t first = this->sort.front().first;
				if (foottime >= 1.f / 3.f && prev_foot != first) {
					this->foot_v.resize(this->foot_v.size() + 1);
					this->foot_v.back().Set(this->bone[first], this->pos - camerapos, first);
					prev_foot = first;
					foottime = 0.f;
				}
			}
			foottime += 1.f / FPS;
			//*
			bool tt = true;
			while (true) {
				tt = true;
				for (int i = 0; i < foot_v.size(); i++) {
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
		//足跡描画
		void Draw_Foot(const Tiles& ti, pos2D& camerapos) {
			for (auto& g : this->foot_v) {
				auto q = GetPos(camerapos + g.pos, ti.hight);
				if (ti.Xin(q.x) && ti.Yin(q.y)) {
					tilesize_r = y_r(tilesize);
					DrawRotaGraphFast(q.x, q.y, float(ti.hight + camhigh_r) / camhigh_r * tilesize_r / 32 / 2, g.yrad + g.yr, this->Graphs[g.ID].get(), TRUE);
				}
			}
		}
		//描画
		void Draw(const Tiles& ti) {
			if (!this->draw_end) {
				bool t = true;
				for (auto& g : this->sort) {
					auto& b = this->bone[g.first];
					auto zh = b.hight - this->sort.front().second;
					auto q = GetPos(b.pos + this->pos, ti.hight);
					this->draw_ok[g.first] = this->draw_ok[g.first] || (ti.Xin(q.x) && ti.Yin(q.y));
					if (this->draw_ok[g.first]) {
						{
							int c = 255 - 255 * std::clamp(ti.hight + zh, 0, camhigh_r) / camhigh_r;
							Set_Bright(c);
						}
						auto p = GetPos(b.pos + this->pos, ti.hight + zh);
						tilesize_r = y_r(tilesize);
						DrawRotaGraphFast(p.x, p.y, float((ti.hight + zh) + camhigh_r) / camhigh_r * tilesize_r / 32 / 2, b.yrad + b.yr, this->Graphs[g.first].get(), TRUE);
					}
					if (!this->draw_ok[g.first]) { t = false; }
				}
				if (t) {
					this->draw_end = true;
				}
			}
		}
		//影描画
		void Draw_Shadow(const Tiles& ti, float light_yrad) {
			for (auto& g : this->sort) {
				auto& b = this->bone[g.first];
				auto zh = b.hight - this->sort.front().second;
				auto q = GetPos(b.pos + this->pos, ti.hight + zh);
				if (ti.Xin(q.x) && ti.Yin(q.y)) {
					auto p = GetPos(b.pos.x + this->pos.x + (int)(float(zh) * sin(light_yrad)), b.pos.y + this->pos.y + (int)(float(zh) * cos(light_yrad)), ti.hight);
					tilesize_r = y_r(tilesize);
					DrawRotaGraphFast(p.x, p.y, float((ti.hight + zh) + camhigh_r) / camhigh_r * tilesize_r / 32 / 2, b.yrad + b.yr, this->Graphs[g.first].get(), TRUE);
				}
			}
		}
	};
private:
	std::vector<TileStatus> TileData;
	std::vector<std::vector<Tiles>> Tile;
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
	bool save, wallorfloor, isread, smz, isend;
	size_t okcnt, ngcnt, undo, redo, upx, dnx, upy, dny, x_size, y_size, wofcnt, smzcnt, floortex, walltex, mscnt, cslcnt;
	int hight_s, bottom_s;
	maps mapdata;
	Edit TileEdit;
	std::vector<Player_Info> PlayerSpawnPoint;
	OPENFILENAME ofn;
	TCHAR strFile[MAX_PATH], cdir[MAX_PATH];
private:
	//線分衝突
	bool ColSeg2(pos2D* m_pos, pos2D& pos1, pos2D& p1, pos2D& pos2) {
		pos2D vec1 = *m_pos - pos1;
		pos2D vec2 = p1 - pos2;
		auto Crs_v1_v2 = vec1.cross(vec2);
		if (Crs_v1_v2 == 0) { return false; }// 平行
		pos2D v = pos2 - pos1;
		const auto Crs_v_v1 = v.cross(vec1);
		const auto Crs_v_v2 = v.cross(vec2);
		if (!(Crs_v_v2 < 0 || Crs_v_v2 > Crs_v1_v2 || Crs_v_v1 < 0 || Crs_v_v1 > Crs_v1_v2)) {// 交差X
			auto pp = vec2.cross(vec1);
			auto dist = vec2.hydist();
			m_pos->x -= -vec2.y * pp / dist;
			m_pos->y -= vec2.x * pp / dist;
			return true;
		}
		return false;
	}
	//座標変換
	static pos2D GetPos(const pos2D& pos, int high) {
		if (camhigh_r > high) {
			return pos2D::Get(deskx / 2 + camhigh_r * pos.x / (camhigh_r - high), desky / 2 + camhigh_r * pos.y / (camhigh_r - high));
		}
		else {
			return pos2D::Get(deskx / 2 + camhigh_r * pos.x, deskx / 2 + camhigh_r * pos.y);
		}
	}
	static pos2D GetPos(int xp, int yp, int high) {
		if (camhigh_r > high) {
			return pos2D::Get(deskx / 2 + camhigh_r * xp / (camhigh_r - high), desky / 2 + camhigh_r * yp / (camhigh_r - high));
		}
		else {
			return pos2D::Get(deskx / 2 + camhigh_r * xp, deskx / 2 + camhigh_r * yp);
		}
	}
	//基幹描画
	void DrawModi_wrap(const pos2D& p1, const pos2D& p2, const pos2D& p3, const pos2D& p4, GraphHandle* g_handle) const noexcept { DrawModiGraph(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, g_handle->get(), TRUE); }
	void DrawExtend_wrap(const pos2D& p1, const pos2D& p2, GraphHandle* g_handle) const noexcept { g_handle->DrawExtendGraph(p1.x, p1.y, p2.x, p2.y, true); }
private:
	//mapエディター
	void change_tile(std::vector<TileStatus>& n_s, TileStatus& m, size_t& x_size_t, size_t& y_size_t) {
		if (!m.is_wall) {
			const size_t s = size_t(m.pos_tile.x) + size_t(m.pos_tile.y) * x_size_t;
			if (m.pos_tile.x >= 1) {
				auto& t = n_s[s - 1];
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 5 : 255;
				}
			}
			if (m.pos_tile.x <= x_size_t - 1 - 1) {
				auto& t = n_s[s + 1];
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 7 : 255;
				}
			}

			if (m.pos_tile.y >= 1) {
				auto& t = n_s[s - x_size_t];
				if (!t.is_wall) {
					t.hight = m.hight;
					t.dir = (t.hight != t.bottom) ? 4 : 255;
				}
			}
			if (m.pos_tile.y <= y_size_t - 1 - 1) {
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
	void Draw_Wall(int UorL, const Tiles& ti) {
		if (UorL < 20 && ti.hight != ti.bottom) {
			{
				tilesize_r = y_r(tilesize);
				float rad = abs(cos(atan2f(float(ti.hight - ti.bottom), float(tilesize_r))));
				int c = (int)(rad * (0.75f + cos(light_yrad + deg2rad((4 - UorL % 4) * 90)) * 0.25f) * 255.f);//
				Set_Bright(c);
			}
			switch (UorL % 4) {
			case 0:
			{
				switch (UorL) {
				case 0://縦(上)
					if (ti.floor[0].y < ti.top[0].y)
						DrawModi_wrap(ti.top[0], ti.top[1], ti.floor[1], ti.floor[0], ti.wallhandle);
					break;
				case 4://上◢
					if (ti.floor[1].y < ti.top[1].y)
						DrawModi_wrap(ti.top[1], ti.top[1], ti.floor[1], ti.floor[0], ti.wallhandle);
					break;
				case 8://上◣
					if (ti.floor[0].y < ti.top[0].y)
						DrawModi_wrap(ti.top[0], ti.top[0], ti.floor[1], ti.floor[0], ti.wallhandle);
					break;
				case 12:
					if (ti.floor[0].y < ti.top[2].y)
						DrawModi_wrap(ti.top[2], ti.top[3], ti.floor[1], ti.floor[0], ti.wallhandle);
					break;
				case 16:
					if (ti.floor[0].y < ti.top[2].y) {
						blend_shadow(&ti.top[0], &ti.top[3], ti.hight, ti.floorhandle);
						DrawModi_wrap(ti.floor[0], ti.floor[1], ti.top[3], ti.top[2], &res_floor);
					}
					break;
				}
			}
			break;
			case 1:
			{
				switch (UorL) {
				case 1://横(左)
					if (ti.floor[0].x < ti.top[0].x)
						DrawModi_wrap(ti.top[2], ti.top[0], ti.floor[0], ti.floor[2], ti.wallhandle);
					break;
				case 5://左◢
					if (ti.floor[0].x < ti.top[0].x)
						DrawModi_wrap(ti.top[2], ti.top[2], ti.floor[0], ti.floor[2], ti.wallhandle);
					break;
				case 9://左◥
					if (ti.floor[0].x < ti.top[0].x)
						DrawModi_wrap(ti.top[0], ti.top[0], ti.floor[0], ti.floor[2], ti.wallhandle);
					break;
				case 13:
					if (ti.floor[0].x < ti.top[3].x)
						DrawModi_wrap(ti.top[3], ti.top[1], ti.floor[0], ti.floor[2], ti.wallhandle);
					break;
				case 17:
					if (ti.floor[0].x < ti.top[3].x) {
						blend_shadow(&ti.top[0], &ti.top[3], ti.hight, ti.floorhandle);
						DrawModi_wrap(ti.floor[0], ti.top[1], ti.top[3], ti.floor[2], &res_floor);
					}
					break;
				}
			}
			break;
			case 2:
			{
				switch (UorL) {
				case 2://縦(下)
					if (ti.floor[3].y >= ti.top[3].y)
						DrawModi_wrap(ti.top[2], ti.top[3], ti.floor[3], ti.floor[2], ti.wallhandle);
					break;
				case 6://下◢
					if (ti.floor[3].y >= ti.top[3].y)
						DrawModi_wrap(ti.top[3], ti.top[3], ti.floor[3], ti.floor[2], ti.wallhandle);
					break;
				case 10://下◣
					if (ti.floor[3].y >= ti.top[3].y)
						DrawModi_wrap(ti.top[2], ti.top[2], ti.floor[3], ti.floor[2], ti.wallhandle);
					break;
				case 14:
					if (ti.floor[2].y > ti.top[1].y) {
						blend_shadow(&ti.top[0], &ti.top[3], ti.hight, ti.floorhandle);
						DrawModi_wrap(ti.top[0], ti.top[1], ti.floor[3], ti.floor[2], &res_floor);
					}
					break;
				case 18:
					if (ti.floor[2].y > ti.top[1].y)
						DrawModi_wrap(ti.top[0], ti.top[1], ti.floor[3], ti.floor[2], ti.wallhandle);
					break;
				}
			}
			break;
			case 3:
			{
				switch (UorL) {
				case 3://横(右)
					if (ti.floor[3].x >= ti.top[3].x)
						DrawModi_wrap(ti.top[3], ti.top[1], ti.floor[1], ti.floor[3], ti.wallhandle);
					break;
				case 7://右◢
					if (ti.floor[3].x >= ti.top[3].x)
						DrawModi_wrap(ti.top[3], ti.top[3], ti.floor[1], ti.floor[3], ti.wallhandle);
					break;
				case 11://右◥
					if (ti.floor[3].x >= ti.top[3].x)
						DrawModi_wrap(ti.top[1], ti.top[1], ti.floor[1], ti.floor[3], ti.wallhandle);
					break;
				case 15:
					if (ti.floor[1].x > ti.top[2].x) {
						blend_shadow(&ti.top[0], &ti.top[3], ti.hight, ti.floorhandle);
						DrawModi_wrap(ti.top[0], ti.floor[1], ti.floor[3], ti.top[2], &res_floor);
					}
					break;
				case 19:
					if (ti.floor[1].x > ti.top[2].x)
						DrawModi_wrap(ti.top[2], ti.top[0], ti.floor[1], ti.floor[3], ti.wallhandle);
					break;
				}
			}
			break;
			}
		}
		else {
			{
				int c = 255 - 255 * std::clamp(ti.hight, 0, camhigh_r) / camhigh_r;
				Set_Bright(c);
			}
			if (!ti.is_wall) {
				blend_shadow(&ti.top[0], &ti.top[3], ti.hight, ti.floorhandle);
				DrawExtend_wrap(ti.top[0], ti.top[3], &res_floor);
			}
			else {
				DrawExtend_wrap(ti.top[0], ti.top[3], ti.floorhandle);
			}
		}
	}
	//柱描画
	void Draw_Tile(const Tiles& ti) {
		switch (ti.use) {//三角柱
		case 0://上
			Draw_Wall(12, ti);	//縦(上)//12
			Draw_Wall(5, ti);	//横(左)
			Draw_Wall(2, ti);	//縦(下)
			Draw_Wall(7, ti);	//横(右)
			break;
		case 1://左
			Draw_Wall(4, ti);	//縦(上)//4
			Draw_Wall(13, ti);	//横(左)
			Draw_Wall(6, ti);	//縦(下)
			Draw_Wall(3, ti);	//横(右)
			break;
		case 2://下
			Draw_Wall(0, ti);	//縦(上)//12
			Draw_Wall(9, ti);	//横(左)
			Draw_Wall(18, ti);	//縦(下)
			Draw_Wall(11, ti);	//横(右)
			break;
		case 3://右
			Draw_Wall(8, ti);	//縦(上)//8
			Draw_Wall(1, ti);	//横(左)
			Draw_Wall(10, ti);	//縦(下)
			Draw_Wall(19, ti);	//横(右)
			break;
		case 4://上
			Draw_Wall(16, ti);	//縦(上)//12
			break;
		case 5://左
			Draw_Wall(17, ti);	//横(左)
			break;
		case 6://下
			Draw_Wall(14, ti);	//縦(下)
			break;
		case 7://右
			Draw_Wall(15, ti);	//横(右)
			break;
		default://柱
			Draw_Wall(0, ti);	//縦(上)
			Draw_Wall(1, ti);	//横(左)
			Draw_Wall(2, ti);	//縦(下)
			Draw_Wall(3, ti);	//横(右)
			Draw_Wall(20, ti);	//天井
			break;
		}
		Set_Bright(255);
		for (auto& pl : human) { pl.Draw_Foot(ti, camerapos); }
		Set_Bright(255);
		for (auto& pl : human) { pl.Draw(ti); }
		Set_Bright(255);
		//DrawFormatString(ti.top[0].x, ti.top[0].y, GetColor(255, 255, 255), "%d\n%d,%d", ti.use, ti.hight, ti.bottom);
	}
	//y軸描画
	void Draw_Common(std::vector<Tiles>& T_X, pos2D limmin, pos2D cam, pos2D limmax) {
		for (auto& pl : human) { pl.Reset(); }
		for (auto& ti : T_X) {
			if (!(ti.top[0].y <= cam.y && ti.zero[3].y >= limmin.y)) { continue; }
			Draw_Tile(ti);
		}
		for (auto& pl : human) { pl.Reset(); }
		for (int y = (int)(T_X.size()) - 1; y >= 0; --y) {
			auto& ti = T_X[y];
			if (!(ti.top[3].y >= cam.y && ti.zero[0].y <= limmax.y)) { continue; }
			Draw_Tile(ti);
		}
	}
	//壁影描画
	void draw_wall_shadow(int UorL, const Tiles& ti) {//一辺
		switch (UorL % 4) {
		case 0:
		{
			switch (UorL) {
			case 0://縦(上)
				DrawModi_wrap(shadow_pos[0], shadow_pos[1], shadow_pos[6], shadow_pos[4], ti.wallhandle);
				break;
			case 8://上◣
				DrawModi_wrap(shadow_pos[0], shadow_pos[0], shadow_pos[6], shadow_pos[4], ti.wallhandle);
				break;
			case 4://上◢
				DrawModi_wrap(shadow_pos[1], shadow_pos[1], shadow_pos[6], shadow_pos[4], ti.wallhandle);
				break;
			case 16:
				if (shadow_pos[4].y < shadow_pos[2].y) {
					DrawModi_wrap(shadow_pos[2], shadow_pos[3], shadow_pos[6], shadow_pos[4], ti.floorhandle);
				}
				break;
			case 12:
				DrawModi_wrap(shadow_pos[2], shadow_pos[3], shadow_pos[6], shadow_pos[4], ti.wallhandle);
				break;
			}
		}
		break;
		case 1:
		{
			switch (UorL) {
			case 1://横(左)
				DrawModi_wrap(shadow_pos[2], shadow_pos[0], shadow_pos[4], shadow_pos[5], ti.wallhandle);
				break;
			case 5://左◢
				DrawModi_wrap(shadow_pos[2], shadow_pos[2], shadow_pos[4], shadow_pos[5], ti.wallhandle);
				break;
			case 9://左◥
				DrawModi_wrap(shadow_pos[0], shadow_pos[0], shadow_pos[4], shadow_pos[5], ti.wallhandle);
				break;
			case 13:
				DrawModi_wrap(shadow_pos[3], shadow_pos[1], shadow_pos[4], shadow_pos[5], ti.wallhandle);
				break;
			case 17:
				if (shadow_pos[4].x < shadow_pos[3].x) {
					DrawModi_wrap(shadow_pos[3], shadow_pos[1], shadow_pos[4], shadow_pos[5], ti.floorhandle);
				}
				break;
			}
		}
		break;
		case 2:
		{
			switch (UorL) {
			case 2://縦(下)
				DrawModi_wrap(shadow_pos[2], shadow_pos[3], shadow_pos[7], shadow_pos[5], ti.wallhandle);
				break;
			case 6://下◢
				DrawModi_wrap(shadow_pos[3], shadow_pos[3], shadow_pos[7], shadow_pos[5], ti.wallhandle);
				break;
			case 10://下◣
				DrawModi_wrap(shadow_pos[2], shadow_pos[2], shadow_pos[7], shadow_pos[5], ti.wallhandle);
				break;
			case 14:
				if (shadow_pos[5].y > shadow_pos[1].y) {
					DrawModi_wrap(shadow_pos[0], shadow_pos[1], shadow_pos[7], shadow_pos[5], ti.floorhandle);
				}
				break;
			case 18:
				DrawModi_wrap(shadow_pos[0], shadow_pos[1], shadow_pos[7], shadow_pos[5], ti.wallhandle);
				break;
			}
		}
		break;
		case 3:
		{
			switch (UorL) {
			case 3://横(右)
				DrawModi_wrap(shadow_pos[3], shadow_pos[1], shadow_pos[6], shadow_pos[7], ti.wallhandle);
				break;
			case 7://右◢
				DrawModi_wrap(shadow_pos[3], shadow_pos[3], shadow_pos[6], shadow_pos[7], ti.wallhandle);
				break;
			case 11://右◥
				DrawModi_wrap(shadow_pos[1], shadow_pos[1], shadow_pos[6], shadow_pos[7], ti.wallhandle);
				break;
			case 15:
				if (shadow_pos[6].x > shadow_pos[2].x) {
					DrawModi_wrap(shadow_pos[2], shadow_pos[0], shadow_pos[6], shadow_pos[7], ti.floorhandle);
				}
				break;
			case 19:
				DrawModi_wrap(shadow_pos[2], shadow_pos[0], shadow_pos[6], shadow_pos[7], ti.wallhandle);
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
					for (auto& ti : T_X) {
						if (!(ti.zero[0].y <= limmax.y && ti.zero[3].y >= limmin.y)) {
							continue;
						}
						if (ti.is_wall) {
							//柱の影描画
							tilesize_r = y_r(tilesize);
							const auto add_x = (int)(float(ti.hight - (ti.bottom + high)) * sin(light_yrad));
							const auto add_y = (int)(float(ti.hight - (ti.bottom + high)) * cos(light_yrad));
							const auto xmin = camerapos.x + tilesize_r * (ti.pos_tile.x + 0);
							const auto ymin = camerapos.y + tilesize_r * (ti.pos_tile.y + 0);
							const auto xmax = camerapos.x + tilesize_r * (ti.pos_tile.x + 1);
							const auto ymax = camerapos.y + tilesize_r * (ti.pos_tile.y + 1);
							shadow_pos[0] = GetPos(xmin + add_x, ymin + add_y, ti.bottom + high);
							shadow_pos[1] = GetPos(xmax + add_x, ymin + add_y, ti.bottom + high);
							shadow_pos[2] = GetPos(xmin + add_x, ymax + add_y, ti.bottom + high);
							shadow_pos[3] = GetPos(xmax + add_x, ymax + add_y, ti.bottom + high);

							shadow_pos[4] = GetPos(xmin, ymin, ti.bottom + high);//◤
							shadow_pos[5] = GetPos(xmin, ymax, ti.bottom + high);//◣
							shadow_pos[6] = GetPos(xmax, ymin, ti.bottom + high);//◥
							shadow_pos[7] = GetPos(xmax, ymax, ti.bottom + high);//◢
							switch (ti.use) {//三角柱
							case 0://上
								draw_wall_shadow(12, ti);		//縦(上)12
								draw_wall_shadow(5, ti);		//横(左)
								draw_wall_shadow(2, ti);		//縦(下)
								draw_wall_shadow(7, ti);		//横(右)
								break;
							case 1://左
								draw_wall_shadow(4, ti);		//縦(上)4
								draw_wall_shadow(13, ti);		//横(左)13
								draw_wall_shadow(6, ti);		//縦(下)
								draw_wall_shadow(3, ti);		//横(右)
								break;
							case 2://下
								draw_wall_shadow(0, ti);		//縦(上)
								draw_wall_shadow(9, ti);		//横(左)9
								draw_wall_shadow(18, ti);		//縦(下)18
								draw_wall_shadow(11, ti);		//横(右)
								break;
							case 3://右
								draw_wall_shadow(8, ti);		//縦(上)
								draw_wall_shadow(1, ti);		//横(左)
								draw_wall_shadow(10, ti);		//縦(下)
								draw_wall_shadow(19, ti);		//横(右)
								break;
							case 4://上
								draw_wall_shadow(16, ti);		//縦(上)//4
								break;
							case 5://左
								draw_wall_shadow(17, ti);		//横(左)
								break;
							case 6://下
								draw_wall_shadow(14, ti);		//縦(下)
								break;
							case 7://右
								draw_wall_shadow(15, ti);		//横(右)
								break;
							default://柱
								draw_wall_shadow(0, ti);		//縦(上)
								draw_wall_shadow(1, ti);		//横(左)
								draw_wall_shadow(2, ti);		//縦(下)
								draw_wall_shadow(3, ti);		//横(右)
								break;
							}
						}
						else {
							//*
							for (auto& pl : human) {
								pl.Draw_Shadow(ti, light_yrad);
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
		tilesize_r = y_r(tilesize);
		int radius = tilesize_r / 3;
		m_pos->x = std::clamp(m_pos->x, radius, tilesize_r * (int)(Tile.size()) - radius);
		m_pos->y = std::clamp(m_pos->y, radius, tilesize_r * (int)(Tile.back().size()) - radius);
		//抜け対策
		for (int i = 0; i < 2; i++) {
			for (auto& T_X : Tile) {
				for (auto& ti : T_X) {
					if (ti.is_wall) {
						const auto x0 = tilesize_r * ti.pos_tile.x - radius;
						const auto y0 = tilesize_r * ti.pos_tile.y - radius;
						const auto x1 = tilesize_r * ti.pos_tile.x + tilesize_r * 5 / 4;
						const auto y1 = tilesize_r * ti.pos_tile.y + tilesize_r * 5 / 4;
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
	//
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
				for (auto& m : TileEdit.Data) {
					const int xs = DrawPts->disp_y / 40 + (int)(m.pos_tile.x * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
					const int ys = DrawPts->disp_y / 40 + (int)(m.pos_tile.y * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
					const int xe = DrawPts->disp_y / 40 + (int)((m.pos_tile.x + 1) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
					const int ye = DrawPts->disp_y / 40 + (int)((m.pos_tile.y + 1) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
					const unsigned char mh = (unsigned char)(255 * (camhigh_r - abs(m.hight)) / camhigh_r);
					const unsigned char mb = (unsigned char)(255 * (camhigh_r - abs(m.bottom)) / camhigh_r);

					if (in2_(mouse_x, mouse_y, xs, ys, xe, ye)) {
						if (m.is_wall) {
							Grad_Box(xs, ys, xe, ye, mh, mh / 2, 0u, mb, mb / 2, 0u, m.dir);
						}
						else {
							Grad_Box(xs, ys, xe, ye, mh, mh / 2, mh / 2, mb, mb / 2, mb / 2, m.dir - 4);
						}
						Fonts.Get(y_r(40)).Get_handle().DrawStringFormat((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(40), GetColor(255, 255, 255), "(%03d,%03d)", m.pos_tile.x, m.pos_tile.y);

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
									change_tile(TileEdit.Data, m, x_size, y_size);
								}
							}
							save = true;
						}
						else {
							if (save) {
								TileEdit.Save();
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
									const int xs2 = DrawPts->disp_y / 40 + (int)((m.pos_tile.x +
										(
										(i == 0) ? 0 :
											(i == 1) ? 1 :
											(i == 2) ? 0 : -1
											)
										) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
									const int ys2 = DrawPts->disp_y / 40 + (int)((m.pos_tile.y +
										(
										(i == 0) ? 1 :
											(i == 1) ? 0 :
											(i == 2) ? -1 : 0
											)
										) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
									const int xe2 = DrawPts->disp_y / 40 + (int)((m.pos_tile.x + 1 +
										(
										(i == 0) ? 0 :
											(i == 1) ? 1 :
											(i == 2) ? 0 : -1
											)
										) * DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size));
									const int ye2 = DrawPts->disp_y / 40 + (int)((m.pos_tile.y + 1 +
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
				tilesize_r = y_r(tilesize);
				DrawCircle(
					DrawPts->disp_y / 40 + mapdata.plx * (int)(DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size)) / tilesize_r,
					DrawPts->disp_y / 40 + mapdata.ply * (int)(DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size)) / tilesize_r,
					y_r(DrawPts->disp_y / std::max(x_size, y_size) * 38 / 40),
					GetColor(0, 255, 0));
				for (auto& m : PlayerSpawnPoint) {
					DrawCircle(
						DrawPts->disp_y / 40 + m.pos_p.x * (int)(DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size)) / tilesize_r,
						DrawPts->disp_y / 40 + m.pos_p.y * (int)(DrawPts->disp_y * 38 / 40 / std::max(x_size, y_size)) / tilesize_r,
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
						static TCHAR* ansFile = &strFile[strlen(cdir) + 1];
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
						static TCHAR* ansFile = &strFile[strlen(cdir) + 1];
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
				up_down_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(480), ("設定する高さ : "s + std::to_string(hight_s)).c_str(), &upx, &dnx,
					[&]() { hight_s = std::min(hight_s + 8, cam_high); },
					[&]() { hight_s = std::max(hight_s - 8, -cam_high); });
				bottom_s = std::min(bottom_s, hight_s - 8);
				//底面
				up_down_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(580 + 15), ("設定する底面 : "s + std::to_string(bottom_s)).c_str(), &upy, &dny,
					[&]() {
					if (bottom_s < cam_high - 8) { hight_s = std::max(bottom_s + 8, hight_s); }
					else { bottom_s = cam_high; }
					bottom_s = std::min(bottom_s + 8, cam_high);
				},
					[&]() { bottom_s = std::max(bottom_s - 8, -cam_high); });
			}
			//アンドゥ
			button_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)), y_r(680), x_r(100), y_r(40), "戻る", TileEdit.CanUndo(), &undo, [&]() { TileEdit.Undo(); });
			//リドゥ
			button_set((int)(x_size * DrawPts->disp_y / std::max(x_size, y_size)) + x_r(150), y_r(680), x_r(100), y_r(40), "進む", TileEdit.CanRedo(), &redo, [&]() { TileEdit.Redo(); });
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
		TileEdit.Data.clear();
		//mapdata
		PlayerSpawnPoint.clear();
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
					std::fstream file;
					file.open(("data/Map/" + mapname + "/1.dat").c_str(), std::ios::binary | std::ios::in);
					do {
						TileEdit.Data.resize(TileEdit.Data.size() + 1);
						file.read((char*)&TileEdit.Data.back(), sizeof(TileEdit.Data.back()));
						x_size = std::max<size_t>(TileEdit.Data.back().pos_tile.x, x_size);
						y_size = std::max<size_t>(TileEdit.Data.back().pos_tile.y, y_size);
					} while (!file.eof());
					x_size++;
					y_size++;
					file.close();
					TileEdit.Data.pop_back();
				}
				//mapデータ2読み込み(プレイヤー初期位置、使用テクスチャ指定)
				{
					std::fstream file;
					file.open(("data/Map/" + mapname + "/2.dat").c_str(), std::ios::binary | std::ios::in);
					file.read((char*)&mapdata, sizeof(mapdata));
					file.close();
				}
				//mapデータ3読み込み(敵情報)
				{
					std::fstream file;
					file.open(("data/Map/" + mapname + "/3.dat").c_str(), std::ios::binary | std::ios::in);
					do {
						PlayerSpawnPoint.resize(PlayerSpawnPoint.size() + 1);
						file.read((char*)&PlayerSpawnPoint.back(), sizeof(PlayerSpawnPoint.back()));
					} while (!file.eof());
					file.close();
					PlayerSpawnPoint.pop_back();
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
							TileEdit.Data.resize(TileEdit.Data.size() + 1);
							TileEdit.Data.back() = { pos2D::Get(x,y), btm, mid, false,2, 2, 255 };
						}
					}
					//(マップチップ)
					for (int y = 0; y < y_size; y += 5) {
						for (int x = 0; x < x_size; x += 5) {
							const size_t s = size_t(x) + size_t(y) * x_size;
							/*
							TileEdit.Data[s + 2 + size_t(x_size)*1] = { pos2D::Get(x + 2, y + 1), btm, hig, true, 2, 2, -1 };
							TileEdit.Data[s + 1 + size_t(x_size)*2] = { pos2D::Get(x + 1, y + 2), btm, hig, true, 2, 2, -1 };
							TileEdit.Data[s + 2 + size_t(x_size)*2] = { pos2D::Get(x + 2, y + 2), btm, hig, true, 2, 2, -1 };
							TileEdit.Data[s + 3 + size_t(x_size)*2] = { pos2D::Get(x + 3, y + 2), btm, hig, true, 2, 2, -1 };
							TileEdit.Data[s + 2 + size_t(x_size)*3] = { pos2D::Get(x + 2, y + 3), btm, hig, true, 2, 2, -1 };
							//*/
							//*
							TileEdit.Data[s + 2 + x_size * 1] = { pos2D::Get(x + 2, y + 1), btm, hig, true, 2, 2, 0 };
							TileEdit.Data[s + 1 + x_size * 2] = { pos2D::Get(x + 1, y + 2), btm, hig, true, 2, 2, 1 };
							TileEdit.Data[s + 2 + x_size * 2] = { pos2D::Get(x + 2, y + 2), btm, hig, true, 2, 1, 255 };
							TileEdit.Data[s + 3 + x_size * 2] = { pos2D::Get(x + 3, y + 2), btm, hig, true, 2, 2, 3 };
							TileEdit.Data[s + 2 + x_size * 3] = { pos2D::Get(x + 2, y + 3), btm, hig, true, 2, 2, 2 };
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
					tilesize_r = y_r(tilesize);
					for (int i = 1; i < 8; i++) {
						PlayerSpawnPoint.resize(PlayerSpawnPoint.size() + 1);
						PlayerSpawnPoint.back().pos_p.x = tilesize_r * 5 * i + tilesize_r / 2;
						PlayerSpawnPoint.back().pos_p.y = tilesize_r * 5 * i + tilesize_r / 2;
					}
				}
			}
		}
		//エディター
		hight_s = 64;
		bottom_s = 0;
		undo = 2;
		redo = 2;
		save = false;
		smz = false;
		upx = 2;
		dnx = 2;
		upy = 2;
		dny = 2;
		TileEdit.List.clear();
		TileEdit.List.push_back(TileEdit.Data);
		TileEdit.ListItr = TileEdit.List.end();
		isend = false;
		while (ProcessMessage() == 0) {
			GetMousePoint(&mouse_x, &mouse_y);
			GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
			if (!Window2()) { break; }
			//画面の反映
			DrawPts->Screen_Flip();
		}
		if (isend) { return false; }
		TileEdit.List.clear();
		//mapデータ1書き込み(マップチップ)
		TileEdit.Write("data/Map/" + mapname + "/1.dat");
		//mapデータ2書き込み(プレイヤー初期位置、使用テクスチャ指定)
		{
			std::fstream file;
			file.open(("data/Map/" + mapname + "/2.dat").c_str(), std::ios::binary | std::ios::out);
			file.write((char*)&mapdata, sizeof(mapdata));
			file.close();
		}
		//mapデータ3書き込み(敵情報)
		{
			std::fstream file;
			file.open(("data/Map/" + mapname + "/3.dat").c_str(), std::ios::binary | std::ios::out);
			for (auto& m : PlayerSpawnPoint)
				file.write((char*)&m, sizeof(m));
			file.close();
			PlayerSpawnPoint.clear();
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
				TileData.resize(TileData.size() + 1);
				file.read((char*)&TileData.back(), sizeof(TileData.back()));
				map_x = std::max<size_t>(TileData.back().pos_tile.x, map_x);
				map_y = std::max<size_t>(TileData.back().pos_tile.y, map_y);
			} while (!file.eof());
			map_x++;
			map_y++;
			file.close();
			TileData.pop_back();

			Tile.resize(map_x);
			for (auto& T_X : Tile) {
				T_X.resize(map_y);
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
			//tilesize_r = y_r(tilesize);
			//GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, tilesize_r, tilesize_r * 2, &walls);
			//GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, tilesize_r, tilesize_r, &floors);
			GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, 16, 16 * 2, &walls);
			GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, 16, 16, &floors);
			player_id = human.size();
			human.resize(human.size() + 1);
			human.back().Init(*player_x, *player_y);
		}
		//mapデータ3読み込み(敵情報)
		{
			file.open(("data/Map/" + mapname + "/3.dat").c_str(), std::ios::binary | std::ios::in);
			do {
				Player_Info anse;
				file.read((char*)&anse, sizeof(anse));
				human.resize(human.size() + 1);
				human.back().Init(anse.pos_p.x / 2, anse.pos_p.y / 2);
			} while (!file.eof());
			file.close();
		}
	}
	//人の移動処理
	void Update_Human(pos2D* m_pos, bool* isstand) {
		for (auto& pl : human) {
			bool isPlayer = ((size_t)(&pl - &human.front()) == player_id);
			auto buf = pl.pos;
			bool oldstand = pl.isStand;
			if (!isPlayer) {
				//todo : cpu
				//pl.pos.x += GetRand(12) - 6;
				//pl.pos.y += GetRand(12) - 6;
			}
			else {
				//自機の移動
				pl.pos = (*m_pos) * -1.f;
				//伏せ時に前に動く
				if (pl.changing && !pl.isStand) {
					pl.pos -= pl.vec * ((10.f * 60.f / FPS) / pl.vecrange);
				}
				int x_m, y_m;
				GetMousePoint(&x_m, &y_m);
				pl.Set_Aim(x_m - DrawPts->disp_x / 2, y_m - DrawPts->disp_y / 2);
				if (!pl.standup) {
					pl.isStand = *isstand;
				}
				else {
					pl.standup = false;
					*isstand = pl.isStand;
				}
			}
			if (oldstand != pl.isStand) {
				pl.changing = true;
			}
			hit_wall(&pl.pos, buf);
			if (isPlayer) {
				*m_pos = pl.pos * -1.f;
			}
			buf -= pl.pos;
			pl.vec_buf = buf;
		}
	}
	//更新
	void FirstUpdate(int sx, int sy) {
		//カメラ座標設定
		camerapos.set(sx, sy);
		//mapの設置
		tilesize_r = y_r(tilesize);
		for (auto& at : TileData) {
			auto& z = Tile[at.pos_tile.x][at.pos_tile.y];
			z.pos_tile = at.pos_tile;
			z.bottom = at.bottom;
			z.hight = at.hight;
			const auto xmin = camerapos.x + tilesize_r * (z.pos_tile.x + 0);
			const auto ymin = camerapos.y + tilesize_r * (z.pos_tile.y + 0);
			const auto xmax = camerapos.x + tilesize_r * (z.pos_tile.x + 1);
			const auto ymax = camerapos.y + tilesize_r * (z.pos_tile.y + 1);
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
		for (auto& pl : human) { pl.Update(camerapos); }
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
		for (auto& T_X : Tile) {
			T_X.clear();
		}
		Tile.clear();
	}
};