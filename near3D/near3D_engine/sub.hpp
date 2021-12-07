#pragma once
#include "DXLib_ref/DXLib_ref.h"

class DialogManager {
	OPENFILENAME ofn;
	TCHAR strFile[MAX_PATH], cdir[MAX_PATH];
public:
	void Init() {
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
	bool Open() { return GetOpenFileName(&ofn); }
	const TCHAR* GetPath() {
		std::string str = strFile;
		if (str.find(cdir) != std::string::npos) {
			return &strFile[strlen(cdir) + 1];
		}
		else {
			return strFile;
		}
	}
};

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

const int tilesize = 128;	//タイル一つ一つのサイズ

class Near3DControl {
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
		const pos2D operator*(float o) const noexcept {
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
	class Camera_Info {
	public:
		const int camhigh_base = 192;	//カメラの高さ
		pos2D camerapos = { 0,0 };
		float camzoom = 1.f;
	};
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
	class shadow_handle {
	public:
		bool isupdate = true;
		GraphHandle handle;
	};

	class Humans;
	class Guns;
	class Common {
	protected:
	public:
		pos2D pos;
		int base_Hight;
		float y_rad = 0.f;
	public:
		void SetHight(const Tiles& ti) {
			this->base_Hight = ti.hight;
		}
	};

	class Humans : public Common {
		typedef std::pair<size_t, int> pairs;
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
				xdist = m_xdist * y_r(tilesize) / 32 * 5 / 6;
				ydist = m_ydist * y_r(tilesize) / 32 * 10;
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
			void Update_Parent(float y_rad, float yrad_aim, float yrad_aim_speed) {
				this->hight = 0;
				this->xr = this->xrad;
				this->yr = this->yrad + y_rad + yrad_aim + yrad_aim_speed;
				this->zr = this->zrad;
				this->edit = true;
			}
			bool Update_Child(const Bonesdata& parentB, float yrad_aim, float yrad_aim_speed) {
				if (parentB.edit) {
					const float zd = (float)this->zdist * y_r(tilesize) / 32;
					const float zd2 = (float)y_r(this->zdist);
					//角度
					this->xr = parentB.xrad + parentB.xr;
					this->yr = parentB.yrad + parentB.yr;
					this->zr = parentB.zrad + parentB.zr;
					if (this->parent == 15) {
						this->yr -= yrad_aim_speed;
					}
					if (this->parent == 16) {
						this->yr -= yrad_aim;
					}
					//位置指定
					float y1 = cos(this->xr) * this->ydist + sin(this->xr) * zd;
					float z1 = cos(this->xr) * zd2 - sin(this->xr) * this->ydist;
					float x2 = cos(this->zr) * this->xdist + sin(this->zr) * z1;
					this->pos = parentB.pos;
					this->pos.x += (int)(cos(this->yr) * x2 - sin(this->yr) * y1);
					this->pos.y += (int)(cos(this->yr) * y1 + sin(this->yr) * x2);
					this->hight = parentB.hight + (int)(cos(this->zr) * z1 - sin(this->zr) * this->xdist);
					this->edit = true;
					return true;
				}
				return false;
			}
		};
		class AnimeControl {
			class Animesdata {
			public:
				int time = 0;
				std::vector<Bonesdata> bone;

				void Set(int bonesize, int time_t) {
					bone.resize(bonesize);
					time = time_t;
				}
				void SetBoneData(int sel, std::string_view ttt, float rad) { this->bone[sel].SetBoneData(ttt, rad); }
			};
		private:
			std::vector<std::vector<Animesdata>> anime;
			Animesdata* nowAnimData = nullptr;
			Animesdata* nextAnimData = nullptr;
			int OldSel = 0;
			int NowFrame = 0;
			int Time = 1;
		public:
			int NowSel = 0;
			void LoadAnime(const std::string& FilePath) {
				this->anime.resize(this->anime.size() + 1);
				const auto mdata = FileRead_open(FilePath.c_str(), FALSE);
				do {
					int tmp;
					std::string ttt = getparams::getcmd(mdata, &tmp);
					if (ttt.find("frame") != std::string::npos) {
						this->anime.back().resize(this->anime.back().size() + 1);
						this->anime.back().back().Set(33, tmp);
					}
					else if (ttt.find("left_hand_") != std::string::npos) { this->anime.back().back().SetBoneData(0, ttt, deg2rad(tmp)); }
					else if (ttt.find("left_arm2_") != std::string::npos) { this->anime.back().back().SetBoneData(3, ttt, deg2rad(tmp)); }
					else if (ttt.find("left_arm_") != std::string::npos) { this->anime.back().back().SetBoneData(4, ttt, deg2rad(tmp)); }
					else if (ttt.find("Body_Top_") != std::string::npos) { this->anime.back().back().SetBoneData(5, ttt, deg2rad(tmp)); }
					else if (ttt.find("right_arm_") != std::string::npos) { this->anime.back().back().SetBoneData(6, ttt, deg2rad(tmp)); }
					else if (ttt.find("right_arm2_") != std::string::npos) { this->anime.back().back().SetBoneData(7, ttt, deg2rad(tmp)); }
					else if (ttt.find("right_hand_") != std::string::npos) { this->anime.back().back().SetBoneData(10, ttt, deg2rad(tmp)); }
					else if (ttt.find("Body_Head_") != std::string::npos) { this->anime.back().back().SetBoneData(15, ttt, deg2rad(tmp)); }
					else if (ttt.find("Body_Mid_") != std::string::npos) { this->anime.back().back().SetBoneData(16, ttt, deg2rad(tmp)); }
					else if (ttt.find("left_leg3_") != std::string::npos) { this->anime.back().back().SetBoneData(22, ttt, deg2rad(tmp)); }
					else if (ttt.find("left_leg2_") != std::string::npos) { this->anime.back().back().SetBoneData(24, ttt, deg2rad(tmp)); }
					else if (ttt.find("left_leg_") != std::string::npos) { this->anime.back().back().SetBoneData(26, ttt, deg2rad(tmp)); }
					else if (ttt.find("Body_Bottom_") != std::string::npos) { this->anime.back().back().SetBoneData(27, ttt, deg2rad(tmp)); }
					else if (ttt.find("right_leg_") != std::string::npos) { this->anime.back().back().SetBoneData(28, ttt, deg2rad(tmp)); }
					else if (ttt.find("right_leg2_") != std::string::npos) { this->anime.back().back().SetBoneData(30, ttt, deg2rad(tmp)); }
					else if (ttt.find("right_leg3_") != std::string::npos) { this->anime.back().back().SetBoneData(32, ttt, deg2rad(tmp)); }
					else if (ttt.find("end") != std::string::npos) { break; }
				} while (true);
				FileRead_close(mdata);
			}
			void Update(std::vector<Bonesdata>* bone_base) {
				if (this->OldSel != this->NowSel) {
					this->NowFrame = 0;
					this->Time = 0;
				}
				this->OldSel = this->NowSel;
				this->nextAnimData = &this->anime[this->NowSel][this->NowFrame];
				if (this->nowAnimData != nullptr && this->Time < this->nowAnimData->time) {
					this->Time++;
					for (int b = 0; b < bone_base->size(); b++) {
						(*bone_base)[b].Leap_Rad(this->nowAnimData->bone[b], this->nextAnimData->bone[b], (float)this->Time / (float)this->nowAnimData->time);
					}
				}
				else {
					++this->NowFrame %= this->anime[this->NowSel].size();
					this->Time = 0;
					this->nowAnimData = this->nextAnimData;
				}
			}
		};
		class FootprintControl {
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
			size_t prev_foot = SIZE_MAX;
			float foottime = 0;
			std::vector<foots> Footprint;
		public:
			void Update(std::vector<Bonesdata>& bone, size_t first, const pos2D& SavePos) {
				if (this->foottime >= 1.f / 3.f && this->prev_foot != first) {
					this->Footprint.resize(this->Footprint.size() + 1);
					this->Footprint.back().Set(bone[first], SavePos, first);
					this->prev_foot = first;
					this->foottime = 0.f;
				}
				this->foottime += 1.f / FPS;
				bool tt;
				while (true) {
					tt = true;
					for (int i = 0; i < this->Footprint.size(); i++) {
						this->Footprint[i].Time -= 1.f / FPS;
						if (this->Footprint[i].Time < 0.f) {
							this->Footprint[i] = this->Footprint.back();
							this->Footprint.pop_back();
							tt = false;
							break;
						}
					}
					if (tt) { break; }
				}
			}
			void Draw_Footprint(const Tiles& ti, const std::vector<GraphHandle>& Graphs, const Camera_Info& caminfo_t) {
				for (auto& g : this->Footprint) {
					auto q = GetPos((caminfo_t.camerapos + g.pos) * caminfo_t.camzoom, ti.hight, caminfo_t);
					if (ti.Xin(q.x) && ti.Yin(q.y)) {
						Set_Bright(255);
						auto cam_high = (int)((float)caminfo_t.camhigh_base / caminfo_t.camzoom);
						DrawRotaGraphFast(q.x, q.y, float(ti.hight + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * caminfo_t.camzoom, g.yrad + g.yr, Graphs[g.ID].get(), TRUE);
					}
				}
			}
		};
	private:
		std::vector<GraphHandle> Graphs;
		std::vector<Bonesdata> bone;
		std::vector<pairs> sort;
		std::vector<bool> draw_ok = { false };
		bool draw_end = false;
		float changingtime = 0.f;
		float yrad_aim = 0;
		float yrad_aim_speed = 0;
		pos2D spawnpos;
		pos2D vec_real;//キャラスプライトが実際に向いている向き
		AnimeControl m_anime;
		FootprintControl m_Footprint;
		Guns* haveGun = nullptr;
		bool canShootGun = false;//銃を構えるか否か
		bool ShootSwitch = false;
	public:
		pos2D vec;//移動方向
		pos2D vec_buf;//移動方向
		int vecrange = 100000;

		bool standup = false;
		bool isStand = true;
		bool changing = false;

		bool isReadyGun = false;//銃を構えるか否か
		bool ShootPress = false;
	public:
		void Reset() {
			std::fill<>(this->draw_ok.begin(), this->draw_ok.end(), false);
			this->draw_end = false;
		}
		const Bonesdata& GetLeftHandInfo() const noexcept { return bone[10]; }
		void SetGun(Guns* haveGun_t) {
			if (haveGun_t != nullptr) {
				haveGun_t->SetHuman(this);
			}
			else if (haveGun != nullptr) {
				haveGun->SetHuman(nullptr);
			}
			haveGun = haveGun_t;
		}
		void Set_Aim(int x, int y) {
			int Limit = this->isStand ? 90 : 45;
			float rad = std::atan2f((float)x, (float)-y) - this->y_rad;
			this->yrad_aim_speed = this->yrad_aim;
			easing_set(&this->yrad_aim, std::clamp(std::atan2f(sin(rad), cos(rad)), deg2rad(-Limit), deg2rad(Limit)), 0.9f);
			this->yrad_aim_speed = (this->yrad_aim - this->yrad_aim_speed)*FPS*0.1f;
		}
	public:
		//開始
		void Init(const pos2D& p_s) {
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
				for (auto& b : n_t) {
					file.write((char*)&b, sizeof(b));
				}
				file.close();
			}
			//*/
			//モーションテキスト(直に打ち込めるように)
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
			for (int i = 0; i < 10; i++) {
				this->m_anime.LoadAnime("data/Char/Mot/" + std::to_string(i) + ".mot");
			}
			this->spawnpos = p_s;
			this->pos = this->spawnpos;
			this->vec_real.set(0, 1);
		}
		//更新
		void Update(const Camera_Info& caminfo_t) {
			for (auto& g : this->bone) { g.edit = false; }
			//アニメーション選択
			if (!this->changing) {
				if (this->isStand) {
					{
						//立ち
						if (!this->isReadyGun) {
							this->m_anime.NowSel = 1;
						}
						else {
							this->m_anime.NowSel = 6;
						}
					}
					if (this->vec_buf.x != 0 || this->vec_buf.y != 0) {
						if (this->vec_buf.hydist() < (3.f * 1.5f) * (3.f * 1.5f)) {
							//歩き
							if (!this->isReadyGun) {
								this->m_anime.NowSel = 0;
							}
							else {
								this->m_anime.NowSel = 7;
							}
						}
						else {
							this->m_anime.NowSel = 2;//走り
						}
					}
				}
				else {
					{
						//伏せ待機
						if (!this->isReadyGun) {
							this->m_anime.NowSel = 4;
						}
						else {
							this->m_anime.NowSel = 9;
						}
					}
					if (this->vec_buf.x != 0 || this->vec_buf.y != 0) {
						if (this->vec_buf.hydist() < (3.f * 1.5f) * (3.f * 1.5f)) {
							//伏せ進み
							if (!this->isReadyGun) {
								this->m_anime.NowSel = 3;
							}
							else {
								this->m_anime.NowSel = 8;
							}
						}
						else {
							//立ち上がる
							this->isStand = true;
							this->standup = true;
						}
					}
				}
			}
			else {
				//しゃがみ
				this->m_anime.NowSel = 5;
				this->changingtime += 1.f / FPS;
				if (this->changingtime >= 0.1f) {
					this->changingtime = 0.f;
					this->changing = false;
				}
			}
			//アニメーション更新
			this->m_anime.Update(&this->bone);
			//
			if (this->vec_buf.x != 0 || this->vec_buf.y != 0) {
				this->vec_real = this->vec_buf;
			}
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
			//足跡座標指定
			bool next = false;
			do {
				next = false;
				for (auto& bo : this->bone) {
					if (!bo.edit) {
						if (bo.parent == -1) {
							bo.Update_Parent(this->y_rad, this->yrad_aim, this->yrad_aim_speed);
						}
						else {
							if (!bo.Update_Child(this->bone[bo.parent], this->yrad_aim, this->yrad_aim_speed)) {
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
			this->m_Footprint.Update(this->bone, this->sort.front().first, this->pos);
			//銃の更新
			{
				//銃表示更新
				if (this->haveGun != nullptr) {
					if (!this->isReadyGun) {
						this->haveGun->isDraw = false;
						this->canShootGun = false;
					}
					else {
						this->haveGun->isDraw = true;
						this->canShootGun = true;
						if (this->m_anime.NowSel == 2) {
							this->haveGun->isDraw = false;
							this->canShootGun = false;
						}
						if (this->m_anime.NowSel == 8) {
							this->canShootGun = false;
						}
					}
					//発砲更新
					if (this->canShootGun) {
						if (this->ShootPress) {
							if (this->ShootSwitch) {
								this->ShootSwitch = false;
								this->haveGun->Shoot();
							}
						}
						else {
							this->ShootSwitch = true;
						}
					}
				}
				else {
					this->canShootGun = false;
				}
			}
		}
		//描画
		void Draw(const Tiles& ti, const Camera_Info& caminfo_t) {
			this->m_Footprint.Draw_Footprint(ti, this->Graphs, caminfo_t);
			if (!this->draw_end) {
				bool t = true;
				for (auto& g : this->sort) {
					auto& b = this->bone[g.first];
					auto zh = this->base_Hight + b.hight - this->sort.front().second;
					auto Pos = (b.pos + this->pos + caminfo_t.camerapos) * caminfo_t.camzoom;
					auto q = GetPos(Pos, zh, caminfo_t);
					this->draw_ok[g.first] = this->draw_ok[g.first] || ((ti.Xin(q.x) && ti.Yin(q.y)) && (ti.hight <= this->base_Hight));
					if (this->draw_ok[g.first]) {
						auto cam_high = (int)((float)caminfo_t.camhigh_base / caminfo_t.camzoom);
						{
							int c = 255 - 255 * std::clamp(zh, 0, cam_high) / cam_high;
							Set_Bright(c);
						}
						DrawRotaGraphFast(q.x, q.y, float((zh)+cam_high) / cam_high * (float)y_r(tilesize) / 64.f * caminfo_t.camzoom, b.yrad + b.yr, this->Graphs[g.first].get(), TRUE);
					}
					else {
						t = false;
					}
				}
				if (t) {
					this->draw_end = true;
				}
			}
		}
		//影描画
		void Draw_Shadow(const Tiles& ti, float light_yrad, float ShadowRange, const Camera_Info& caminfo_t) {
			for (auto& g : this->sort) {
				auto& b = this->bone[g.first];
				auto zh = ti.hight + b.hight - this->sort.front().second;
				auto zh2 = float(zh - ti.hight) * caminfo_t.camzoom*ShadowRange;
				auto Pos = (b.pos + this->pos + caminfo_t.camerapos) * caminfo_t.camzoom;
				auto q = GetPos(Pos, zh, caminfo_t);
				if (ti.Xin(q.x) && ti.Yin(q.y)) {
					auto p = GetPos(Pos + pos2D::Get((int)(zh2 * sin(light_yrad)), (int)(zh2 * cos(light_yrad))), ti.hight, caminfo_t);
					auto cam_high = (int)((float)caminfo_t.camhigh_base / caminfo_t.camzoom);
					DrawRotaGraphFast(p.x, p.y, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * caminfo_t.camzoom, b.yrad + b.yr, this->Graphs[g.first].get(), TRUE);
				}
			}
		}
	};
	class Guns : public Common {
	private:
		class Ammo : public Common {
		private:
			float Time = 0.f;
			int hight = 0;
			pos2D pos_hand;
			float Speed = 0.f;
			Guns* haveGun = nullptr;
		public:
			bool isEnd() { return this->Time <= 0.f; }
			void Update_Ammo() {
				float spd = this->Speed * 60.f / FPS;
				this->pos += pos2D::Get(sin(this->y_rad)*spd, -cos(this->y_rad)*spd);
			}
			void Set_Hit(bool isWallHit) {
				this->Time = 0.f;
				//ヒットエフェクト
				{
					if (isWallHit) {
						//無機質
					}
					else {
						//人体
					}
				}
			}
		public:
			void Init(Guns* haveGun_t) {
				if (haveGun_t != nullptr) {
					haveGun = haveGun_t;
					this->pos = haveGun->pos;
					this->pos_hand = haveGun->pos_hand;
					this->base_Hight = haveGun->base_Hight;
					this->hight = haveGun->hight - 1;
					this->y_rad = haveGun->y_rad;
					this->Time = 5.f;
					this->Speed = 50.f;
				}
			}
			void Update() {
				this->Time -= 1.f / FPS;
			}
			void Draw(const Tiles& ti, const Camera_Info& caminfo_t) {
				auto zh = this->base_Hight + this->hight;
				auto Pos = (this->pos + this->pos_hand + caminfo_t.camerapos) * caminfo_t.camzoom;
				auto q = GetPos(Pos, zh, caminfo_t);
				if ((ti.Xin(q.x) && ti.Yin(q.y)) && (ti.hight <= this->base_Hight)) {
					auto cam_high = (int)((float)caminfo_t.camhigh_base / caminfo_t.camzoom);
					{
						int c = 255 - 255 * std::clamp(zh, 0, cam_high) / cam_high;
						Set_Bright(c);
					}
					//
					DrawRotaGraphFast(q.x, q.y, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * caminfo_t.camzoom, this->y_rad, this->haveGun->Graphs[0].get(), TRUE);
				}
			}
		};
	private:
		int hight = 0;
		std::vector<GraphHandle> Graphs;
		pos2D pos_hand;
		pos2D Recoilpos;
		Humans* haveHuman = nullptr;
		float Recoil = 0.f;
	public:
		std::vector<Ammo> ammo;
		bool isDraw = true;
	public:
		void SetHuman(Humans* haveHuman_t) { haveHuman = haveHuman_t; }
		void Shoot() {
			ammo.resize(ammo.size() + 1);
			ammo.back().Init(this);
			Recoil = 10.f;
		}
	public:
		void Init(const pos2D& p_s) {
			GraphHandle::LoadDiv("data/Gun/1.bmp", 9, 5, 3, 96, 96, &this->Graphs);
		}
		void Update() {
			if (haveHuman != nullptr) {
				this->Recoil = std::max(this->Recoil - 1.5f * 60.f / FPS, 0.f);
				this->Recoilpos = pos2D::Get(y_r(sin(this->y_rad)*this->Recoil), y_r(-cos(this->y_rad)*this->Recoil))*-1.f;
				this->pos = haveHuman->pos;
				this->pos_hand = haveHuman->GetLeftHandInfo().pos;
				this->hight = haveHuman->GetLeftHandInfo().hight + 1;
				this->y_rad = haveHuman->GetLeftHandInfo().yrad + haveHuman->GetLeftHandInfo().yr;
			}
			for (int i = 0; i < ammo.size(); i++) {
				auto& am = ammo[i];
				if (am.isEnd()) {
					am = ammo.back();
					ammo.pop_back();
					i--;
				}
			}
		}
		void Draw(const Tiles& ti, const Camera_Info& caminfo_t) {
			for (auto& am : ammo) {
				am.Draw(ti, caminfo_t);
			}
			if (this->isDraw) {
				auto zh = this->base_Hight + this->hight;
				auto q = GetPos((this->pos + this->pos_hand + caminfo_t.camerapos) * caminfo_t.camzoom, zh, caminfo_t);
				if ((ti.Xin(q.x) && ti.Yin(q.y)) && (ti.hight <= this->base_Hight)) {
					auto cam_high = (int)((float)caminfo_t.camhigh_base / caminfo_t.camzoom);
					{
						int c = 255 - 255 * std::clamp(zh, 0, cam_high) / cam_high;
						Set_Bright(c);
					}
					//
					DrawRotaGraphFast(q.x, q.y, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * caminfo_t.camzoom, this->y_rad, this->Graphs[7].get(), TRUE);
					auto q2 = GetPos((this->pos + this->Recoilpos + this->pos_hand + caminfo_t.camerapos) * caminfo_t.camzoom, zh, caminfo_t);
					DrawRotaGraphFast(q2.x, q2.y, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * caminfo_t.camzoom, this->y_rad, this->Graphs[5].get(), TRUE);
				}
			}
		}
	};
private:
	class Edit {
	public:
		class Player_Info {
		public:
			pos2D pos_p;
		};
		class maps {
		public:
			const int SP_Limit = 10;
			pos2D SP[10];
			char wall_name[MAX_PATH];
			char floor_name[MAX_PATH];
			float light_yrad;
		};
	public:
		std::list<std::vector<TileStatus>> List;//
		std::list<std::vector<TileStatus>>::iterator ListItr;
		std::vector<TileStatus> Data;
		std::vector<Player_Info> PlayerSpawnPoint;
		maps mapdata;
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
		void Read(const std::string& mapChipName, const std::string& mapTexName, const std::string& playerInfoName) {
			std::fstream file;
			//mapデータ1読み込み(マップチップ)
			file.open(mapChipName.c_str(), std::ios::binary | std::ios::in);
			do {
				this->Data.resize(this->Data.size() + 1);
				file.read((char*)&this->Data.back(), sizeof(this->Data.back()));
			} while (!file.eof());
			file.close();
			this->Data.pop_back();
			//mapデータ2読み込み(プレイヤー初期位置、使用テクスチャ指定)
			file.open(mapTexName.c_str(), std::ios::binary | std::ios::in);
			file.read((char*)&this->mapdata, sizeof(this->mapdata));
			file.close();
			//mapデータ3読み込み(敵情報)
			this->PlayerSpawnPoint.clear();
			file.open(playerInfoName.c_str(), std::ios::binary | std::ios::in);
			do {
				this->PlayerSpawnPoint.resize(this->PlayerSpawnPoint.size() + 1);
				file.read((char*)&this->PlayerSpawnPoint.back(), sizeof(this->PlayerSpawnPoint.back()));
			} while (!file.eof());
			file.close();
			this->PlayerSpawnPoint.pop_back();
		}
		void Write(const std::string& mapChipName, const std::string& mapTexName, const std::string& playerInfoName) {
			std::fstream file;
			//mapデータ1書き込み(マップチップ)
			file.open(mapChipName.c_str(), std::ios::binary | std::ios::out);
			for (auto& data : this->Data) {
				file.write((char*)&data, sizeof(data));
			}
			file.close();
			this->Data.clear();
			//mapデータ2書き込み(プレイヤー初期位置、使用テクスチャ指定)
			file.open(mapTexName.c_str(), std::ios::binary | std::ios::out);
			file.write((char*)&this->mapdata, sizeof(this->mapdata));
			file.close();
			//mapデータ3書き込み(敵情報)
			file.open(playerInfoName.c_str(), std::ios::binary | std::ios::out);
			for (auto& SP_t : this->PlayerSpawnPoint) {
				file.write((char*)&SP_t, sizeof(SP_t));
			}
			file.close();
			this->PlayerSpawnPoint.clear();
		}

		void PreSet(int Map_Xsize_t, int Map_Ysize_t) {
			//mapデータ1書き込み(マップチップ)
			const int btm = 0;
			const int mid = 0;
			const int hig = 64;
			for (int y = 0; y < Map_Ysize_t; y++) {
				for (int x = 0; x < Map_Xsize_t; x++) {
					this->Data.resize(this->Data.size() + 1);
					this->Data.back() = { pos2D::Get(x,y), btm, mid, false,2, 2, 255 };
				}
			}
			for (int y = 0; y < Map_Ysize_t; y += 5) {
				for (int x = 0; x < Map_Xsize_t; x += 5) {
					const size_t s = size_t(x + y * Map_Xsize_t);
					/*
					this->Data[s + 2 + Map_Xsize_t * 1] = { pos2D::Get(x + 2, y + 1), btm, hig, true, 2, 2, 255 };
					this->Data[s + 1 + Map_Xsize_t * 2] = { pos2D::Get(x + 1, y + 2), btm, hig, true, 2, 2, 255 };
					this->Data[s + 2 + Map_Xsize_t * 2] = { pos2D::Get(x + 2, y + 2), btm, hig, true, 2, 2, 255 };
					this->Data[s + 3 + Map_Xsize_t * 2] = { pos2D::Get(x + 3, y + 2), btm, hig, true, 2, 2, 255 };
					this->Data[s + 2 + Map_Xsize_t * 3] = { pos2D::Get(x + 2, y + 3), btm, hig, true, 2, 2, 255 };
					//*/
					/*
					this->Data[s + 2 + Map_Xsize_t * 1] = { pos2D::Get(x + 2, y + 1), btm, hig, true, 2, 2, 0 };
					this->Data[s + 1 + Map_Xsize_t * 2] = { pos2D::Get(x + 1, y + 2), btm, hig, true, 2, 2, 1 };
					this->Data[s + 2 + Map_Xsize_t * 2] = { pos2D::Get(x + 2, y + 2), btm, hig, true, 2, 1, 255 };
					this->Data[s + 3 + Map_Xsize_t * 2] = { pos2D::Get(x + 3, y + 2), btm, hig, true, 2, 2, 3 };
					this->Data[s + 2 + Map_Xsize_t * 3] = { pos2D::Get(x + 2, y + 3), btm, hig, true, 2, 2, 2 };
					//*/
				}
			}
			//mapデータ2書き込み(プレイヤー初期位置、使用テクスチャ指定)
			this->mapdata.SP[0].set(32, 32);
			this->mapdata.light_yrad = deg2rad(0);
			strcpy_s(this->mapdata.wall_name, "data/Chip/Wall/1.bmp");
			strcpy_s(this->mapdata.floor_name, "data/Chip/Floor/1.bmp");
			//mapデータ3書き込み(敵情報)
			this->PlayerSpawnPoint.clear();
			for (int i = 1; i < 8; i++) {
				this->PlayerSpawnPoint.resize(this->PlayerSpawnPoint.size() + 1);
				this->PlayerSpawnPoint.back().pos_p.x = y_r(tilesize) * 5 * i + y_r(tilesize) / 2;
				this->PlayerSpawnPoint.back().pos_p.y = y_r(tilesize) * 5 * i + y_r(tilesize) / 2;
			}
		}
	};
private:
	std::vector<TileStatus> TileData;
	std::vector<std::vector<Tiles>> Tile;
	std::vector<Humans> human;
	std::vector<Guns> gun;
	size_t player_id = 0;
	std::array<pos2D, 8> shadow_pos;
	std::shared_ptr<DXDraw> DrawPts{ nullptr };			//引き継ぐ
	int mouse_x, mouse_y;

	float light_yrad = 0.0f;
	float ShadowRange = 6.f;

	std::array<shadow_handle, 8> shadow_graph;
	std::vector<GraphHandle> walls;
	std::vector<GraphHandle> floors;
	GraphHandle res_floor;
	GraphHandle screen;
	Camera_Info caminfo;
private:
	bool save, wallorfloor, isread, smz, isend;
	int hight_s, bottom_s;
	int Map_Xsize, Map_Ysize;
	Edit TileEdit;
	DialogManager Dialog;
	//座標変換
	static pos2D GetPos(const pos2D& pos, int high, const Camera_Info& caminfo_t) {
		auto cam_high = (int)((float)caminfo_t.camhigh_base / caminfo_t.camzoom);
		if (cam_high > high) {
			return pos2D::Get(deskx / 2 + cam_high * pos.x / (cam_high - high), desky / 2 + cam_high * pos.y / (cam_high - high));
		}
		else {
			return pos2D::Get(deskx / 2 + cam_high * pos.x, deskx / 2 + cam_high * pos.y);
		}
	}
	static pos2D GetPos(int xp, int yp, int high, const Camera_Info& caminfo_t) {
		return GetPos(pos2D::Get(xp, yp), high, caminfo_t);
	}
	//基幹描画
	void DrawModi_wrap(const pos2D& p1, const pos2D& p2, const pos2D& p3, const pos2D& p4, GraphHandle* g_handle) const noexcept { DrawModiGraph(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, g_handle->get(), TRUE); }
	void DrawExtend_wrap(const pos2D& p1, const pos2D& p2, GraphHandle* g_handle) const noexcept { g_handle->DrawExtendGraph(p1.x, p1.y, p2.x, p2.y, true); }
private:
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
				float rad = abs(cos(atan2f(float(ti.hight - ti.bottom), (float)y_r(tilesize) / caminfo.camzoom)));
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
				auto cam_high = (int)((float)caminfo.camhigh_base / caminfo.camzoom);
				int c = 255 - 255 * std::clamp(ti.hight, 0, cam_high) / cam_high;
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
		for (auto& pl : human) { pl.Draw(ti, caminfo); }
		for (auto& gn : gun) { gn.Draw(ti, caminfo); }
		Set_Bright(255);
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
		const auto limmin = GetPos(-DrawPts->disp_x * 3 / 4, -DrawPts->disp_y * 3 / 4, 0, caminfo);
		const auto limmax = GetPos(DrawPts->disp_x * 3 / 4, DrawPts->disp_y * 3 / 4, 0, caminfo);
		int high = 0;
		for (auto& g : shadow_graph) {
			if (g.isupdate) {
				g.isupdate = false;
				g.handle.SetDraw_Screen();
				{
					//環境影
					if (light_yrad >= deg2rad(-90) && light_yrad <= deg2rad(90)) {
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - std::clamp((int)(255.f*abs(cos(light_yrad))), 0, 255));
					}
					if (light_yrad <= deg2rad(-90) || light_yrad >= deg2rad(90)) {
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					}
					DrawBox(0, 0, DrawPts->disp_x, DrawPts->disp_y, GetColor(0, 0, 0), TRUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					if (light_yrad >= deg2rad(-90) && light_yrad <= deg2rad(90)) {
						Set_Bright(0);
						for (auto& T_X : Tile) {
							if (T_X[0].zero[0].x <= limmax.x && T_X[0].zero[3].x >= limmin.x) {
								for (auto& ti : T_X) {
									if (ti.zero[0].y <= limmax.y && ti.zero[3].y >= limmin.y) {
										if (ti.is_wall) {
											//柱の影描画
											const auto zh2 = (float)(ti.hight - ti.bottom - high)*caminfo.camzoom*ShadowRange;
											auto add_p = pos2D::Get((int)(zh2 * sin(light_yrad)), (int)(zh2 * cos(light_yrad)));
											pos2D min, max;
											min.set((int)((float)(caminfo.camerapos.x + y_r(tilesize) * (ti.pos_tile.x + 0)) * caminfo.camzoom), (int)((float)(caminfo.camerapos.y + y_r(tilesize) * (ti.pos_tile.y + 0)) * caminfo.camzoom));
											max.set((int)((float)(caminfo.camerapos.x + y_r(tilesize) * (ti.pos_tile.x + 1)) * caminfo.camzoom), (int)((float)(caminfo.camerapos.y + y_r(tilesize) * (ti.pos_tile.y + 1)) * caminfo.camzoom));
											shadow_pos[0] = GetPos(pos2D::Get(min.x, min.y) + add_p, ti.bottom + high, caminfo);
											shadow_pos[1] = GetPos(pos2D::Get(max.x, min.y) + add_p, ti.bottom + high, caminfo);
											shadow_pos[2] = GetPos(pos2D::Get(min.x, max.y) + add_p, ti.bottom + high, caminfo);
											shadow_pos[3] = GetPos(pos2D::Get(max.x, max.y) + add_p, ti.bottom + high, caminfo);
											shadow_pos[4] = GetPos(pos2D::Get(min.x, min.y), ti.bottom + high, caminfo);//◤
											shadow_pos[5] = GetPos(pos2D::Get(min.x, max.y), ti.bottom + high, caminfo);//◣
											shadow_pos[6] = GetPos(pos2D::Get(max.x, min.y), ti.bottom + high, caminfo);//◥
											shadow_pos[7] = GetPos(pos2D::Get(max.x, max.y), ti.bottom + high, caminfo);//◢
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
											for (auto& pl : human) { pl.Draw_Shadow(ti, light_yrad, ShadowRange, caminfo); }
										}
									}
								}
							}
						}
					}
				}
			}
			high += 8;
		}
		Set_Bright(255);
	}
	//線分衝突
	static bool ColSeg2(pos2D* m_pos, pos2D& pos1, pos2D& p1, pos2D& pos2) {
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
	//人と壁の判定
	bool hit_wall(pos2D* m_pos, pos2D& old, bool ischeck) {
		int radius = y_r(tilesize) / 3;
		m_pos->x = std::clamp(m_pos->x, radius, y_r(tilesize) * (int)(Tile.size()) - radius);
		m_pos->y = std::clamp(m_pos->y, radius, y_r(tilesize) * (int)(Tile.back().size()) - radius);
		//抜け対策
		for (int i = 0; i < 2; i++) {
			for (auto& T_X : Tile) {
				for (auto& ti : T_X) {
					if (ti.is_wall) {
						const auto x0 = y_r(tilesize) * ti.pos_tile.x - radius;
						const auto y0 = y_r(tilesize) * ti.pos_tile.y - radius;
						const auto x1 = y_r(tilesize) * ti.pos_tile.x + y_r(tilesize) * 5 / 4;
						const auto y1 = y_r(tilesize) * ti.pos_tile.y + y_r(tilesize) * 5 / 4;
						pos2D s0 = { x0 ,y0 };
						pos2D s1 = { x0 ,y1 };
						pos2D s2 = { x1 ,y0 };
						pos2D s3 = { x1 ,y1 };
						if (ColSeg2(m_pos, old, s1, s0)) {
							if (ischeck) { return true; }
							break;
						}
						if (ColSeg2(m_pos, old, s0, s2)) {
							if (ischeck) { return true; }
							break;
						}
						if (ColSeg2(m_pos, old, s2, s3)) {
							if (ischeck) { return true; }
							break;
						}
						if (ColSeg2(m_pos, old, s3, s1)) {
							if (ischeck) { return true; }
							break;
						}
					}
				}
			}
		}
		return false;
	}
	//button
	class Button {
		size_t count = 0;
		size_t count2 = 0;
	public:
		void Init() {
			count = 2;
			count2 = 2;
		}
		void ButtonSet(int mouse_x, int mouse_y, int xs, int ys, int xsize, int ysize, std::string_view buf, bool on, std::function<void()> doing1) {
			bool mouse_in = in2_(mouse_x, mouse_y, xs, ys, xs + xsize, ys + ysize);
			if (on) {
				if (mouse_in) { count = std::min<size_t>(count + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0); }
				else { count = 2; }
			}
			if (count == 1) { doing1(); }
			DrawBox(xs, ys, xs + xsize, ys + ysize, on ? ((mouse_in) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
			Fonts.Get(y_r(40)).Get_handle().DrawString_MID(xs + xsize / 2, ys, buf, on ? ((mouse_in) ? GetColor(255, 255, 0) : GetColor(255, 0, 0)) : GetColor(0, 0, 0));
		}
		bool Switch() {
			if (count == 1) {
				return true;
			}
			return false;
		}

		void UpDownSet(int mouse_x, int mouse_y, int xs, int ys, std::string_view buf, bool on, std::function<void()> doing1, std::function<void()> doing2) {
			int xsize = x_r(40);
			int ysize = y_r(30);
			int x2 = xs + Fonts.Get(y_r(40)).Get_handle().GetDrawWidth(buf);
			int y2 = ys + y_r(40);
			bool mouse_in1 = in2_(mouse_x, mouse_y, x2, ys, x2 + xsize, ys + ysize);
			bool mouse_in2 = in2_(mouse_x, mouse_y, x2, y2, x2 + xsize, y2 + ysize);
			if (on) {
				if (mouse_in1) { count = std::min<size_t>(count + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0); }
				else { count = 2; }
				if (mouse_in2) { count2 = std::min<size_t>(count2 + 1, ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0); }
				else { count2 = 2; }
			}
			if (count == 1) { doing1(); }
			if (count2 == 1) { doing2(); }
			Fonts.Get(y_r(40)).Get_handle().DrawString(xs, ys + y_r(15), buf, GetColor(255, 255, 0));
			DrawTriangle(x2 + xsize / 2, ys, x2, ys + ysize, x2 + xsize, ys + ysize, on ? ((mouse_in1) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
			DrawTriangle(x2, y2, x2 + xsize, y2, x2 + xsize / 2, y2 + ysize, on ? ((mouse_in2) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
		}
	};
	Button button_1;
	Button button_2;
	Button button_3;
	Button button_4;
	Button button_5;
	Button button_6;
	Button button_7;
	Button button_8;
	Button button_9;
	Button button_0;

	Button updown_1;
	Button updown_2;
	//
	bool Window1() {
		DrawBox(x_r(960 - 320), y_r(540 - 180), x_r(960 + 320), y_r(540 + 180), GetColor(128, 128, 128), TRUE);
		Fonts.Get(y_r(40)).Get_handle().DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "プリセットを読み込みますか?", GetColor(255, 255, 0));
		bool end_f = true;
		button_1.ButtonSet(mouse_x, mouse_y, x_r(960 + 320 - 340), y_r(540 + 180 - 140), x_r(300), y_r(40), "YES", true, [&]() { isread = true; end_f = false; });	//YES
		button_2.ButtonSet(mouse_x, mouse_y, x_r(960 + 320 - 340), y_r(540 + 180 - 80), x_r(300), y_r(40), "NO", true, [&]() { end_f = false; });					//NO
		return end_f;
	}
	bool Window2() {
		int xpos_E = DrawPts->disp_y / 40;
		int ypos_E = DrawPts->disp_y / 40;
		int tilesize_E2 = DrawPts->disp_y / std::max(Map_Xsize, Map_Ysize);
		int tilesize_E = tilesize_E2 * 38 / 40;
		//マップ描画
		{
			auto cam_high = (int)((float)caminfo.camhigh_base / caminfo.camzoom);
			for (auto& data : TileEdit.Data) {
				const int xs = xpos_E + (int)(data.pos_tile.x * tilesize_E);
				const int ys = ypos_E + (int)(data.pos_tile.y * tilesize_E);
				const int xe = xpos_E + (int)((data.pos_tile.x + 1) * tilesize_E);
				const int ye = ypos_E + (int)((data.pos_tile.y + 1) * tilesize_E);
				const unsigned char mh = (unsigned char)(255 * (cam_high - abs(data.hight)) / cam_high);
				const unsigned char mb = (unsigned char)(255 * (cam_high - abs(data.bottom)) / cam_high);

				if (in2_(mouse_x, mouse_y, xs, ys, xe, ye)) {
					if (data.is_wall) {
						Grad_Box(xs, ys, xe, ye, mh, mh / 2, 0u, mb, mb / 2, 0u, data.dir);
					}
					else {
						Grad_Box(xs, ys, xe, ye, mh, mh / 2, mh / 2, mb, mb / 2, mb / 2, data.dir - 4);
					}
					Fonts.Get(y_r(40)).Get_handle().DrawStringFormat(Map_Xsize * tilesize_E2, y_r(40), GetColor(255, 255, 255), "(%03d,%03d)", data.pos_tile.x, data.pos_tile.y);

					if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
						if (wallorfloor) {
							//壁
							data.hight = hight_s;
							data.bottom = bottom_s;
							data.dir = 255;
							data.is_wall = true;
						}
						else {
							//床
							data.hight = bottom_s;
							data.bottom = bottom_s;
							data.dir = 255;
							data.is_wall = false;
							//周りのタイルを変更
							if (smz) {
								if (!data.is_wall) {
									const size_t s = size_t(data.pos_tile.x + data.pos_tile.y * Map_Xsize);
									if (data.pos_tile.x >= 1) {
										auto& t = TileEdit.Data[s - 1];
										if (!t.is_wall) {
											t.hight = data.hight;
											t.dir = (t.hight != t.bottom) ? 5 : 255;
										}
									}
									if (data.pos_tile.x <= Map_Xsize - 1 - 1) {
										auto& t = TileEdit.Data[s + 1];
										if (!t.is_wall) {
											t.hight = data.hight;
											t.dir = (t.hight != t.bottom) ? 7 : 255;
										}
									}

									if (data.pos_tile.y >= 1) {
										auto& t = TileEdit.Data[s - Map_Xsize];
										if (!t.is_wall) {
											t.hight = data.hight;
											t.dir = (t.hight != t.bottom) ? 4 : 255;
										}
									}
									if (data.pos_tile.y <= Map_Ysize - 1 - 1) {
										auto& t = TileEdit.Data[s + Map_Xsize];
										//下
										if (!t.is_wall) {
											t.hight = data.hight;
											t.dir = (t.hight != t.bottom) ? 6 : 255;
										}
									}
								}
							}
						}
						save = true;
					}
					else {
						if (save) {
							save = false;
							TileEdit.Save();
						}
					}
				}
				else {
					if (data.is_wall) {
						Grad_Box(xs, ys, xe, ye, mh, mh, 0, mb, mb, 0, data.dir);
					}
					else {
						Grad_Box(xs, ys, xe, ye, mh, mh, mh, mb, mb, mb, data.dir - 4);
						if (smz) {
							for (int i = 0; i < 4; i++) {
								int xsel = ((i == 0) ? 0 : (i == 1) ? 1 : (i == 2) ? 0 : -1);
								int ysel = ((i == 0) ? 1 : (i == 1) ? 0 : (i == 2) ? -1 : 0);
								const int xs2 = xpos_E + (data.pos_tile.x + xsel) * tilesize_E;
								const int ys2 = ypos_E + (data.pos_tile.y + ysel) * tilesize_E;
								const int xe2 = xpos_E + (data.pos_tile.x + xsel + 1) * tilesize_E;
								const int ye2 = ypos_E + (data.pos_tile.y + ysel + 1) * tilesize_E;
								if (in2_(mouse_x, mouse_y, xs2, ys2, xe2, ye2)) {
									Grad_Box(xs, ys, xe, ye, mh, mh / 2, mh / 2, mb, mb / 2, mb / 2, data.dir - 4);
									break;
								}
							}
						}
					}
				}

				if (data.is_wall) {
					DrawBox(xs, ys, xe, ye, GetColor(0, 0, 0), FALSE);
				}
			}
			DrawCircle(xpos_E + TileEdit.mapdata.SP[0].x * tilesize_E / y_r(tilesize), ypos_E + TileEdit.mapdata.SP[0].y * tilesize_E / y_r(tilesize), tilesize_E, GetColor(0, 255, 0));
			for (auto& SP_t : TileEdit.PlayerSpawnPoint) {
				DrawCircle(xpos_E + SP_t.pos_p.x * tilesize_E / y_r(tilesize), ypos_E + SP_t.pos_p.y * tilesize_E / y_r(tilesize), tilesize_E2, GetColor(255, 0, 0));
			}
		}
		//壁か床か
		button_7.ButtonSet(mouse_x, mouse_y, Map_Xsize * tilesize_E2, y_r(80), x_r(400), y_r(40), "選択タイルを変更", true, [&]() { wallorfloor ^= 1; });
		Fonts.Get(y_r(40)).Get_handle().DrawString(Map_Xsize * tilesize_E2, y_r(80) + y_r(40), wallorfloor ? "壁を選択中" : "床を選択中", GetColor(255, 0, 0));
		//壁か床か
		button_8.ButtonSet(mouse_x, mouse_y, Map_Xsize * tilesize_E2, y_r(180), x_r(400), y_r(40), "地形編集", true, [&]() { smz ^= 1; });
		Fonts.Get(y_r(40)).Get_handle().DrawString(Map_Xsize * tilesize_E2, y_r(180) + y_r(40), smz ? "台形" : "矩形", GetColor(255, 0, 0));
		//床テクスチャ
		button_9.ButtonSet(mouse_x, mouse_y, Map_Xsize * tilesize_E2, y_r(280), x_r(400), y_r(40), "床テクスチャ選択", GetWindowModeFlag() == TRUE, [&]() {
			if (Dialog.Open()) {
				strcpy_s(TileEdit.mapdata.floor_name, Dialog.GetPath());
			}
		});
		Fonts.Get(y_r(30)).Get_handle().DrawString(Map_Xsize * tilesize_E2, y_r(280) + y_r(40), TileEdit.mapdata.floor_name, GetColor(255, 0, 0));
		//壁テクスチャ
		button_0.ButtonSet(mouse_x, mouse_y, Map_Xsize * tilesize_E2, y_r(380), x_r(400), y_r(40), "壁テクスチャ選択", GetWindowModeFlag() == TRUE, [&]() {
			if (Dialog.Open()) {
				strcpy_s(TileEdit.mapdata.wall_name, Dialog.GetPath());
			}
		});
		Fonts.Get(y_r(30)).Get_handle().DrawString(Map_Xsize * tilesize_E2, y_r(380) + y_r(40), TileEdit.mapdata.wall_name, GetColor(255, 0, 0));
		//設定する高さ
		{
			auto cam_high = (int)((float)caminfo.camhigh_base / caminfo.camzoom);
			//高
			updown_1.UpDownSet(mouse_x, mouse_y, Map_Xsize * tilesize_E2, y_r(480), "設定する高さ : " + std::to_string(hight_s), true,
				[&]() { hight_s = std::min(hight_s + 8, cam_high); },
				[&]() { hight_s = std::max(hight_s - 8, -cam_high); });
			bottom_s = std::min(bottom_s, hight_s - 8);
			//底面
			updown_2.UpDownSet(mouse_x, mouse_y, Map_Xsize * tilesize_E2, y_r(580 + 15), "設定する底面 : " + std::to_string(bottom_s), true,
				[&]() {
				if (bottom_s < cam_high - 8) { hight_s = std::max(bottom_s + 8, hight_s); }
				else { bottom_s = cam_high; }
				bottom_s = std::min(bottom_s + 8, cam_high);
			},
				[&]() { bottom_s = std::max(bottom_s - 8, -cam_high); });
		}
		button_5.ButtonSet(mouse_x, mouse_y, Map_Xsize * tilesize_E2, y_r(680), x_r(100), y_r(40), "戻る", TileEdit.CanUndo(), [&]() { TileEdit.Undo(); });				//アンドゥ
		button_6.ButtonSet(mouse_x, mouse_y, Map_Xsize * tilesize_E2 + x_r(150), y_r(680), x_r(100), y_r(40), "進む", TileEdit.CanRedo(), [&]() { TileEdit.Redo(); });		//リドゥ
		bool end_f = true;
		button_3.ButtonSet(mouse_x, mouse_y, x_r(1920 - 340), y_r(1080 - 160), x_r(300), y_r(40), "保存せず終了", true, [&]() { isend = true; end_f = false; });		//終了
		button_4.ButtonSet(mouse_x, mouse_y, x_r(1920 - 340), y_r(1080 - 80), x_r(300), y_r(40), "保存して続行", true, [&]() { end_f = false; });						//終了
		return end_f;
	}
	bool Window3() {
		DrawBox(x_r(960 - 320), y_r(540 - 180), x_r(960 + 320), y_r(540 + 180), GetColor(128, 128, 128), TRUE);
		Fonts.Get(y_r(40)).Get_handle().DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "マップのサイズは?", GetColor(255, 255, 0));
		//高
		updown_1.UpDownSet(mouse_x, mouse_y, x_r(960 - 320 + 40), y_r(540 - 180 + 60 + 100), "X : " + std::to_string(Map_Xsize), true,
			[&]() { Map_Xsize++; },
			[&]() { if (Map_Xsize > 1) { Map_Xsize--; } });
		//底面
		updown_2.UpDownSet(mouse_x, mouse_y, x_r(960 - 320 + 40), y_r(540 - 180 + 60 + 100 + 115), "Y : " + std::to_string(Map_Ysize), true,
			[&]() { Map_Ysize++; },
			[&]() { if (Map_Ysize > 1) { Map_Ysize--; }});
		{
			int xsz = x_r(280);
			int ysz = y_r(120);
			int xm = x_r(1100);
			int ym = y_r(540);
			if (Map_Xsize * ysz / xsz >= Map_Ysize) {
				ysz = xsz * Map_Ysize / Map_Xsize;
			}
			else {
				xsz = ysz * Map_Xsize / Map_Ysize;
			}
			DrawBox(xm - xsz / 2, ym - ysz / 2, xm + xsz / 2, ym + ysz / 2, GetColor(255, 255, 0), FALSE);
		}
		//終了
		bool end_f = true;
		button_4.ButtonSet(mouse_x, mouse_y, x_r(960 + 320 - 340), y_r(540 + 180 - 80), x_r(300), y_r(40), "OK", true, [&]() { end_f = false; });
		return end_f;
	}

public:
	const Tiles& GetTile(const pos2D& p_s) const noexcept { return Tile[p_s.x / y_r(tilesize)][p_s.y / y_r(tilesize)]; }
public:
	//コンストラクタ
	Near3DControl(std::shared_ptr<DXDraw>& DrawPts_t) {
		DrawPts = DrawPts_t;
		for (auto& g : shadow_graph) {
			g.handle = GraphHandle::Make(DrawPts->disp_x, DrawPts->disp_y, true);
			g.isupdate = true;
		}
		screen = GraphHandle::Make(DrawPts->disp_x, DrawPts->disp_y, false);
		res_floor = GraphHandle::Make(16, 16, true);
	}
	//デストラクタ
	~Near3DControl(void) {
		Dispose();
		DrawPts.reset();
	}
	//mapエディター
	bool Map_Editer(std::string mapname) {
		wallorfloor = false;
		//ダイアログ用
		Dialog.Init();
		TileEdit.Data.clear();
		//map_data
		{
			isread = false;
			button_1.Init();
			button_2.Init();
			while (ProcessMessage() == 0) {
				GetMousePoint(&mouse_x, &mouse_y);
				GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
				if (!Window1()) { break; }
				DrawPts->Screen_Flip();
			}
			if (!isread) {
				//map読み込み
				TileEdit.Read("data/Map/" + mapname + "/1.dat", "data/Map/" + mapname + "/2.dat", "data/Map/" + mapname + "/3.dat");
				Map_Xsize = 0;
				Map_Ysize = 0;
				for (auto& d : TileEdit.Data) {
					Map_Xsize = std::max(d.pos_tile.x, Map_Xsize);
					Map_Ysize = std::max(d.pos_tile.y, Map_Ysize);
				}
				Map_Xsize++;
				Map_Ysize++;
			}
			else {
				//mapデータプリセット
				Map_Xsize = 40;
				Map_Ysize = 40;
				updown_1.Init();
				updown_2.Init();
				button_4.Init();
				while (ProcessMessage() == 0) {
					GetMousePoint(&mouse_x, &mouse_y);
					GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
					if (!Window3()) { break; }
					DrawPts->Screen_Flip();
				}
				TileEdit.PreSet(Map_Xsize, Map_Ysize);
			}
		}
		//エディター
		{
			TileEdit.List.clear();
			TileEdit.List.push_back(TileEdit.Data);
			TileEdit.ListItr = TileEdit.List.end();
			hight_s = 64;
			bottom_s = 0;
			button_3.Init();
			button_4.Init();
			button_5.Init();
			button_6.Init();
			button_7.Init();
			button_8.Init();
			button_9.Init();
			button_0.Init();
			updown_1.Init();
			updown_2.Init();
			save = false;
			smz = false;
			isend = false;
			while (ProcessMessage() == 0) {
				GetMousePoint(&mouse_x, &mouse_y);
				GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
				if (!Window2()) { break; }
				DrawPts->Screen_Flip();
			}
			if (isend) { return false; }
			TileEdit.List.clear();
		}
		TileEdit.Write("data/Map/" + mapname + "/1.dat", "data/Map/" + mapname + "/2.dat", "data/Map/" + mapname + "/3.dat");			//mapデータ書き込み
		return true;
	}
	//map選択
	void Start(int SpawnPoint, std::string mapname) {
		using namespace std::literals;
		int map_x = 0, map_y = 0;
		{
			std::fstream file;
			//mapデータ1読み込み(マップチップ)
			TileData.clear();
			file.open(("data/Map/" + mapname + "/1.dat").c_str(), std::ios::binary | std::ios::in);
			do {
				TileData.resize(TileData.size() + 1);
				file.read((char*)&TileData.back(), sizeof(TileData.back()));
			} while (!file.eof());
			file.close();
			TileData.pop_back();
			//mapデータ2読み込み(プレイヤー初期位置、使用テクスチャ指定)
			Edit::maps mapb;
			file.open(("data/Map/" + mapname + "/2.dat").c_str(), std::ios::binary | std::ios::in);
			file.read((char*)&mapb, sizeof(mapb));
			file.close();
			light_yrad = mapb.light_yrad;
			light_yrad = deg2rad(45);
			GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, 16, 16 * 2, &walls);
			GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, 16, 16, &floors);
			player_id = human.size();
			human.resize(human.size() + 1);
			human.back().Init(mapb.SP[std::clamp(SpawnPoint, 0, mapb.SP_Limit - 1)] * -1.f);
			//mapデータ3読み込み(敵情報)
			file.open(("data/Map/" + mapname + "/3.dat").c_str(), std::ios::binary | std::ios::in);
			do {
				Edit::Player_Info anse;
				file.read((char*)&anse, sizeof(anse));
				human.resize(human.size() + 1);
				human.back().Init(anse.pos_p);
			} while (!file.eof());
			file.close();

			gun.resize(gun.size() + 1);
			gun.back().Init(pos2D::Get(0, 0));
		}
		{
			for (auto& d : TileData) {
				map_x = std::max(d.pos_tile.x, map_x);
				map_y = std::max(d.pos_tile.y, map_y);
			}
			map_x++;
			map_y++;

			Tile.resize(map_x);
			for (auto& T_X : Tile) {
				T_X.resize(map_y);
			}
		}
		//銃セット
		{
			human[0].SetGun(&gun[0]);
		}
	}
	//人の移動処理
	const pos2D PlayerPos() { return human[0].pos; }

	void Update_Human(pos2D* m_vec, bool* isstand, const bool Aim, const bool shoot) {
		caminfo.camzoom += GetMouseWheelRotVolF() / 10.f;
		caminfo.camzoom = std::clamp(caminfo.camzoom, 0.6f, 1.0f);

		light_yrad += deg2rad(0.1f) / FPS;
		if (light_yrad > deg2rad(180)) { light_yrad = deg2rad(-180); }

		if (light_yrad >= deg2rad(-90) && light_yrad <= deg2rad(90)) {
			ShadowRange = 1.f + 9.f*abs(sin(light_yrad));
		}
		else {
			ShadowRange = 10.f;
		}
		ShadowRange = std::clamp(ShadowRange, 3.f, 10.f);

		clsDx();
		printfDx("%f\n", caminfo.camzoom);

		for (auto& pl : human) {
			bool isPlayer = ((size_t)(&pl - &human.front()) == player_id);
			auto buf = pl.pos;
			bool oldstand = pl.isStand;
			pos2D OLDPos = pl.pos;

			if (!isPlayer) {
				//todo : cpu
				//pl.pos.x += GetRand(12) - 6;
				//pl.pos.y += GetRand(12) - 6;
			}
			else {
				//自機の移動
				pl.pos += *m_vec;
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
				{
					pl.isReadyGun = Aim;	//エイム
					pl.ShootPress = shoot;	//射撃キー押し
				}
			}
			if (oldstand != pl.isStand) {
				pl.changing = true;
			}
			hit_wall(&pl.pos, buf, false);
			if (isPlayer) {
				*m_vec = (pl.pos - OLDPos)*-1.f;
			}
			buf -= pl.pos;
			pl.vec_buf = buf;
		}
	}
	//更新
	void Update(const pos2D& cam_pos) {
		//カメラ座標設定
		caminfo.camerapos = cam_pos;
		//mapの設置
		for (auto& at : TileData) {
			auto& ti = Tile[at.pos_tile.x][at.pos_tile.y];
			ti.pos_tile = at.pos_tile;
			ti.bottom = at.bottom;
			ti.hight = at.hight;
			const auto xmin = (float)(caminfo.camerapos.x + y_r(tilesize) * (ti.pos_tile.x + 0)) * caminfo.camzoom;
			const auto ymin = (float)(caminfo.camerapos.y + y_r(tilesize) * (ti.pos_tile.y + 0)) * caminfo.camzoom;
			const auto xmax = (float)(caminfo.camerapos.x + y_r(tilesize) * (ti.pos_tile.x + 1)) * caminfo.camzoom;
			const auto ymax = (float)(caminfo.camerapos.y + y_r(tilesize) * (ti.pos_tile.y + 1)) * caminfo.camzoom;
			ti.top[0] = GetPos(pos2D::Get((int)(xmin), (int)(ymin)), ti.hight, caminfo);
			ti.top[1] = GetPos(pos2D::Get((int)(xmax), (int)(ymin)), ti.hight, caminfo);
			ti.top[2] = GetPos(pos2D::Get((int)(xmin), (int)(ymax)), ti.hight, caminfo);
			ti.top[3] = GetPos(pos2D::Get((int)(xmax), (int)(ymax)), ti.hight, caminfo);
			ti.floor[0] = GetPos(pos2D::Get((int)(xmin), (int)(ymin)), ti.bottom, caminfo);
			ti.floor[1] = GetPos(pos2D::Get((int)(xmax), (int)(ymin)), ti.bottom, caminfo);
			ti.floor[2] = GetPos(pos2D::Get((int)(xmin), (int)(ymax)), ti.bottom, caminfo);
			ti.floor[3] = GetPos(pos2D::Get((int)(xmax), (int)(ymax)), ti.bottom, caminfo);
			ti.zero[0] = GetPos(pos2D::Get((int)(xmin), (int)(ymin)), 0, caminfo);
			ti.zero[1] = GetPos(pos2D::Get((int)(xmax), (int)(ymin)), 0, caminfo);
			ti.zero[2] = GetPos(pos2D::Get((int)(xmin), (int)(ymax)), 0, caminfo);
			ti.zero[3] = GetPos(pos2D::Get((int)(xmax), (int)(ymax)), 0, caminfo);
			ti.wallhandle = &walls[at.wall_id];
			ti.floorhandle = &floors[at.floor_id];
			ti.use = at.dir;
			ti.is_wall = at.is_wall;
		}
		//人の描画用意
		for (auto& pl : human) { pl.Update(caminfo); }
		for (auto& pl : human) { pl.SetHight(GetTile(pl.pos)); }
		for (auto& gn : gun) { gn.Update(); }
		for (auto& gn : gun) {
			for (auto& am : gn.ammo) {
				auto buf = am.pos;
				am.Update_Ammo();
				if (hit_wall(&am.pos, buf, true)) {
					am.Set_Hit(true);
				}
				am.Update();
			}
			//
		}
		for (auto& gn : gun) { gn.SetHight(GetTile(gn.pos)); }
		//影
		Update_Shadow();
		//一気に描画
		const auto limmin = GetPos(-DrawPts->disp_x / 2, -DrawPts->disp_y / 2, 0, caminfo);
		const auto cam = GetPos(0, 0, 0, caminfo);
		const auto limmax = GetPos(DrawPts->disp_x / 2, DrawPts->disp_y / 2, 0, caminfo);
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
	//出力
	void Output(void) {
		screen.DrawGraph(0, 0, true);
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