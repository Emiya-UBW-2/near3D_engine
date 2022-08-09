#pragma once
#include "DXLib_ref/DXLib_ref.h"

using namespace DXLib_ref;

#include "Enum.hpp"

namespace Near3D {
	//2Dベクトル関連
	class Vector2D_I {
	public:
		int x = 0;
		int y = 0;
		//設定する
		void set(int _x, int _y) noexcept {
			this->x = _x;
			this->y = _y;
		}
		//取得する
		static Vector2D_I Get(int _x, int _y) noexcept {
			Vector2D_I p;
			p.set(_x, _y);
			return p;
		}
		static Vector2D_I Zero(void) noexcept { return Get(0, 0); }
		static Vector2D_I Right(void) noexcept { return Get(1, 0); }
		static Vector2D_I Front(void) noexcept { return Get(0, 1); }
		//比較
		const bool operator==(const Vector2D_I& _o) const noexcept { return (this->x == _o.x && this->y == _o.y); }
		const bool operator!=(const Vector2D_I& _o) const noexcept { return !(*this == _o); }
		//加算した値を返す
		const Vector2D_I operator+(const Vector2D_I& _o) const noexcept { return Get(this->x + _o.x, this->y + _o.y); }
		//加算する
		Vector2D_I& operator+=(const Vector2D_I& _o)noexcept {
			*this = *this + _o;
			return *this;
		}
		//減算した値を変えす
		const Vector2D_I operator-(const Vector2D_I& _o) const noexcept { return Get(this->x - _o.x, this->y - _o.y); }
		//減算する
		Vector2D_I& operator-=(const Vector2D_I& _o)noexcept {
			*this = *this - _o;
			return *this;
		}
		//乗算
		const Vector2D_I operator*(float _o) const noexcept { return Get((int)((float)(this->x) * _o), (int)((float)(this->y) * _o)); }
		Vector2D_I& operator*=(float _o)noexcept {
			*this = *this * _o;
			return *this;
		}
		//徐算
		const Vector2D_I operator/(float _o) const noexcept { return Get((int)((float)(this->x) / _o), (int)((float)(this->y) / _o)); }
		Vector2D_I& operator/=(float _o)noexcept {
			*this = *this / _o;
			return *this;
		}
		//距離の2乗を取得する
		const int hydist(void) const noexcept { return this->x * this->x + this->y * this->y; }
		//距離を取得する
		const int dist(void) const noexcept { return (int)std::sqrt(hydist()); }
		// 内積
		const int dot(const Vector2D_I& _o) const noexcept { return this->x * _o.x + this->y * _o.y; }
		// 外積
		const int cross(const Vector2D_I& _o) const noexcept { return this->x * _o.y - this->y * _o.x; }
	};
	//基幹描画
	static void DrawModi_wrap(const Vector2D_I& p1, const Vector2D_I& p2, const Vector2D_I& p3, const Vector2D_I& p4, GraphHandle* g_handle) noexcept { DrawModiGraph(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, g_handle->get(), TRUE); }
	static void DrawExtend_wrap(const Vector2D_I& p1, const Vector2D_I& p2, GraphHandle* g_handle) noexcept { g_handle->DrawExtendGraph(p1.x, p1.y, p2.x, p2.y, true); }
	static void DrawRota_wrap(const Vector2D_I& p1, float  ExRate, float  Angle, const GraphHandle& g_handle, bool TransFlag) noexcept { DrawRotaGraphFast(p1.x, p1.y, ExRate, Angle, g_handle.get(), TransFlag ? TRUE : FALSE); }
	static void Draw_Triangle(const Vector2D_I& p1, const Vector2D_I& p2, const Vector2D_I& p3, unsigned int _Color, bool _isfill) noexcept { DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, _Color, _isfill ? TRUE : FALSE); }
	//当たり判定
	enum class HIT_SELECT {
		ONLY_HIT,
		GET_HIT,
		GET_ZURI,
	};
	static bool Get_CollisionSegment2(const Vector2D_I& _pos, const Vector2D_I& _oldpos, const Vector2D_I& _pos2, const Vector2D_I& _oldpos2, HIT_SELECT Sel) noexcept {
		if (Sel != HIT_SELECT::ONLY_HIT) {
			return false;
		}
		const auto vec1 = _pos - _oldpos;
		const auto vec2 = _pos2 - _oldpos2;
		const auto cross_v1_v2 = vec1.cross(vec2);
		if (cross_v1_v2 != 0) {// 平行でない場合
			const auto vec0 = _oldpos2 - _oldpos;
			const auto cross_v0_v1 = vec0.cross(vec1);
			const auto cross_v0_v2 = vec0.cross(vec2);
			if (!(cross_v0_v2 < 0 || cross_v0_v2 > cross_v1_v2 || cross_v0_v1 < 0 || cross_v0_v1 > cross_v1_v2)) {// 交差X
				return true;
			}
		}
		return false;
	}
	static bool Get_CollisionSegment2(Vector2D_I* _pos, const Vector2D_I& _oldpos, const Vector2D_I& _pos2, const Vector2D_I& _oldpos2, HIT_SELECT Sel) noexcept {
		const auto vec1 = *_pos - _oldpos;
		const auto vec2 = _pos2 - _oldpos2;
		const auto cross_v1_v2 = vec1.cross(vec2);
		if (cross_v1_v2 != 0) {// 平行でない場合
			const auto vec0 = _oldpos2 - _oldpos;
			const auto cross_v0_v1 = vec0.cross(vec1);
			const auto cross_v0_v2 = vec0.cross(vec2);
			if (!(cross_v0_v2 < 0 || cross_v0_v2 > cross_v1_v2 || cross_v0_v1 < 0 || cross_v0_v1 > cross_v1_v2)) {// 交差X
				switch (Sel)
				{
				case HIT_SELECT::ONLY_HIT:
					break;
				case HIT_SELECT::GET_HIT:
					_pos->x = _oldpos.x + vec1.x * cross_v0_v2 / cross_v1_v2;
					_pos->y = _oldpos.y + vec1.y * cross_v0_v2 / cross_v1_v2;
					break;
				case HIT_SELECT::GET_ZURI:
					auto dist = vec2.hydist();
					auto cross = vec2.cross(vec1);
					_pos->x += vec2.y * cross / dist;
					_pos->y += -vec2.x * cross / dist;
					break;
				}
				return true;
			}
		}
		return false;
	}
	//Near3D用カメラ情報
	class Camera_Info {
	public:
		const int camhigh_base = 192 * 3 / 4;	//カメラの高さ
		Vector2D_I camerapos = { 0,0 };
		float camzoom = 1.f;
	public:

		//シングルトン化
#if true

	private:
		static inline  Camera_Info* m_Singleton = nullptr;
	public:
		static void Create(void) noexcept {
			if (m_Singleton == nullptr) {
				m_Singleton = new Camera_Info();
			}
		}
		static Camera_Info* Instance(void) noexcept {
			if (m_Singleton == nullptr) {
				m_Singleton = new Camera_Info();
			}
			return m_Singleton;
		}
	private:

#endif
		Camera_Info(void) noexcept {

		}
		~Camera_Info(void) noexcept {

		}
	};
	//座標変換
	static Vector2D_I ConvertPos(const Vector2D_I& _pos, int _hight) noexcept {
		auto caminfo = Camera_Info::Instance();
		auto cam_high = (int)((float)caminfo->camhigh_base / caminfo->camzoom);
		if (cam_high > _hight) {
			return Vector2D_I::Get(deskx / 2 + cam_high * _pos.x / (cam_high - _hight), desky / 2 + cam_high * _pos.y / (cam_high - _hight));
		}
		else {
			return Vector2D_I::Get(deskx / 2 + cam_high * _pos.x, deskx / 2 + cam_high * _pos.y);
		}
	}
	//カメラ座標込み
	static Vector2D_I ConvertPos_CalcCam(const Vector2D_I& _pos, int _hight) noexcept {
		auto caminfo = Camera_Info::Instance();
		return ConvertPos((_pos + caminfo->camerapos) * caminfo->camzoom, _hight);
	}
	//定数
	const int tilesize = 128;	//タイル一つ一つのサイズ
	//キャラボーン
	typedef std::pair<size_t, int> BoneSort;
	class Bonesdata {
	public:
		int parent = -1;
		Vector2D_I m_pos;
		int m_hight = 0;
		float xr = 0.f, yr = 0.f, zr = 0.f;
		float xrad = 0.f, yrad = 0.f, zrad = 0.f;
		float xdist = 0.f, ydist = 0.f, zdist = 0.f;
		bool edit = true;
	public:
		void SetDist(float m_xdist, float  m_ydist, float  m_zdist) noexcept {
			xdist = m_xdist * y_r(tilesize) / 32 * 5 / 6;
			ydist = m_ydist * y_r(tilesize) / 32 * 10;
			zdist = m_zdist;
		}
		void Leap_Rad(const Bonesdata& now, const Bonesdata& next, float per) noexcept {
			if (abs(now.xrad - next.xrad) < deg2rad(360)) {
				this->xrad = now.xrad + (next.xrad - now.xrad) * per;
			}
			else {
				float Now = ((now.xrad - next.xrad) >= deg2rad(360)) ? now.xrad - deg2rad(360) : now.xrad + deg2rad(360);
				this->xrad = Now + (next.xrad - Now) * per;
			}
			if (abs(now.yrad - next.yrad) < deg2rad(360)) {
				this->yrad = now.yrad + (next.yrad - now.yrad) * per;
			}
			else {
				float Now = ((now.yrad - next.yrad) >= deg2rad(360)) ? now.yrad - deg2rad(360) : now.yrad + deg2rad(360);
				this->yrad = Now + (next.yrad - Now) * per;
			}
			if (abs(now.zrad - next.zrad) < deg2rad(360)) {
				this->zrad = now.zrad + (next.zrad - now.zrad) * per;
			}
			else {
				float Now = ((now.zrad - next.zrad) >= deg2rad(360)) ? now.zrad - deg2rad(360) : now.zrad + deg2rad(360);
				this->zrad = Now + (next.zrad - Now) * per;
			}
		}
		void SetBoneData(std::string_view ttt, float rad) noexcept {
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
		const auto GetBoneData(std::string_view ttt) const noexcept {
			if (ttt.find("x") != std::string::npos || ttt.find("X") != std::string::npos) {
				return this->xrad;
			}
			else if (ttt.find("y") != std::string::npos || ttt.find("Y") != std::string::npos) {
				return this->yrad;
			}
			else if (ttt.find("z") != std::string::npos || ttt.find("Z") != std::string::npos) {
				return this->zrad;
			}
		}
		void Update_Parent(float y_rad_t, float yrad_aim, float yrad_aim_speed) noexcept {
			this->m_hight = 0;
			this->xr = 0;
			this->yr = y_rad_t + yrad_aim + yrad_aim_speed;
			this->zr = 0;
			this->edit = true;
		}
		bool Update_Child(const Bonesdata& parentB, float yrad_aim, float yrad_aim_speed) noexcept {
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
				this->m_pos = parentB.m_pos;
				this->m_pos.x += (int)(cos(this->yr) * x2 - sin(this->yr) * y1);
				this->m_pos.y += (int)(cos(this->yr) * y1 + sin(this->yr) * x2);
				this->m_hight = parentB.m_hight + (int)(cos(this->zr) * z1 - sin(this->zr) * this->xdist);
				this->edit = true;
				return true;
			}
			return false;
		}
	};
	//アニメ
	class Animesdata {
		int time{ 0 };
		std::vector<Bonesdata> m_bone;
	public:
		const auto& GetTime(void) const noexcept { return time; }
		const auto& GetBone(int _p) const noexcept { return m_bone[_p]; }
		void Set(int _bonesize, int _time) noexcept {
			m_bone.resize(_bonesize);
			time = _time;
		}
		void SetBoneData(Bone_Sel _sel, std::string_view _ttt, float _rad) noexcept { this->m_bone[(int)_sel].SetBoneData(_ttt, _rad); }
		const auto GetBoneData(Bone_Sel _sel, std::string_view _ttt) const noexcept { return this->m_bone[(int)_sel].GetBoneData(_ttt); }

		void Dispose(void) noexcept {
			m_bone.clear();
		}
	};
	class AnimeData {
	public:
		std::vector<std::vector<Animesdata>> anime;
	public:

		void LoadAnime(const std::string& FilePath) noexcept {
			this->anime.resize(this->anime.size() + 1);
			auto& Anim = this->anime.back();
			const auto mdata = FileRead_open(FilePath.c_str(), FALSE);
			do {
				int tmp;
				std::string ttt = getparams::getcmd(mdata, &tmp);
				if (ttt.find("frame") != std::string::npos) {
					Anim.resize(Anim.size() + 1);
					Anim.back().Set(33, tmp);
				}
				else if (ttt.find("left_hand_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::LEFTHAND, ttt, deg2rad(tmp)); }
				else if (ttt.find("left_arm2_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::LEFTARM2, ttt, deg2rad(tmp)); }
				else if (ttt.find("left_arm_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::LEFTARM1, ttt, deg2rad(tmp)); }
				else if (ttt.find("Body_Top_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::BODYTOP, ttt, deg2rad(tmp)); }
				else if (ttt.find("right_arm_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::RIGHTARM1, ttt, deg2rad(tmp)); }
				else if (ttt.find("right_arm2_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::RIGHTARM2, ttt, deg2rad(tmp)); }
				else if (ttt.find("right_hand_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::RIGHTHAND, ttt, deg2rad(tmp)); }
				else if (ttt.find("Body_Head_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::HEAD, ttt, deg2rad(tmp)); }
				else if (ttt.find("Body_Mid_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::BODYMIDDLE, ttt, deg2rad(tmp)); }
				else if (ttt.find("left_leg3_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::LEFTLEG3, ttt, deg2rad(tmp)); }
				else if (ttt.find("left_leg2_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::LEFTLEG2, ttt, deg2rad(tmp)); }
				else if (ttt.find("left_leg_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::LEFTLEG1, ttt, deg2rad(tmp)); }
				else if (ttt.find("Body_Bottom_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::BODYBOTTOM, ttt, deg2rad(tmp)); }
				else if (ttt.find("right_leg_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::RIGHTLEG1, ttt, deg2rad(tmp)); }
				else if (ttt.find("right_leg2_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::RIGHTLEG2, ttt, deg2rad(tmp)); }
				else if (ttt.find("right_leg3_") != std::string::npos) { Anim.back().SetBoneData(Bone_Sel::RIGHTLEG3, ttt, deg2rad(tmp)); }
				else if (ttt.find("end") != std::string::npos) { break; }
			} while (true);
			FileRead_close(mdata);
		}
		void SaveAnime(int _AnimSel, const std::string& FilePath) noexcept {
			using namespace std::literals;

			std::ofstream outputfile(FilePath);
			auto& Anim = this->anime[_AnimSel];
			for (auto& a : Anim) {
				outputfile << "frame="s + std::to_string(a.GetTime()) + "\n";

				for (int i = 0; i < (int)Bone_Sel::NUM; i++) {
					for (int t = 0; t < 2; t++) {
						auto xy = (t == 0) ? "x" : "y";
						int rad = (int)rad2deg(a.GetBoneData((Bone_Sel)i, xy));
						if (t == 0) {
							int yrad = (int)rad2deg(a.GetBoneData((Bone_Sel)i, "y"));
							if (rad == 0 && yrad == 0) { continue; }
						}
						else {
							int xrad = (int)rad2deg(a.GetBoneData((Bone_Sel)i, "x"));
							if (xrad == 0 && rad == 0) { continue; }
						}
						switch ((Bone_Sel)i) {
						case Bone_Sel::LEFTHAND:
							outputfile << "left_hand_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::LEFTHAND
							break;
						case Bone_Sel::LEFTARM2:
							outputfile << "left_arm2_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::LEFTARM2
							break;
						case Bone_Sel::LEFTARM1:
							outputfile << "left_arm_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::LEFTARM1
							break;
						case Bone_Sel::BODYTOP:
							outputfile << "Body_Top_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::BODYTOP
							break;
						case Bone_Sel::RIGHTARM1:
							outputfile << "right_arm_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::RIGHTARM1
							break;
						case Bone_Sel::RIGHTARM2:
							outputfile << "right_arm2_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::RIGHTARM2
							break;
						case Bone_Sel::RIGHTHAND:
							outputfile << "right_hand_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::RIGHTHAND
							break;
						case Bone_Sel::HEAD:
							outputfile << "Body_Head_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::HEAD
							break;
						case Bone_Sel::BODYMIDDLE:
							outputfile << "Body_Mid_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::BODYMIDDLE
							break;
						case Bone_Sel::LEFTLEG3:
							outputfile << "left_leg3_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::LEFTLEG3
							break;
						case Bone_Sel::LEFTLEG2:
							outputfile << "left_leg2_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::LEFTLEG2
							break;
						case Bone_Sel::LEFTLEG1:
							outputfile << "left_leg_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::LEFTLEG1
							break;
						case Bone_Sel::BODYBOTTOM:
							outputfile << "Body_Bottom_"s + xy + "=" + std::to_string(rad) + "\n";//	Bone_Sel::BODYBOTTOM
							break;
						case Bone_Sel::RIGHTLEG1:
							outputfile << "right_leg_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::RIGHTLEG1
							break;
						case Bone_Sel::RIGHTLEG2:
							outputfile << "right_leg2_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::RIGHTLEG2
							break;
						case Bone_Sel::RIGHTLEG3:
							outputfile << "right_leg3_"s + xy + "=" + std::to_string(rad) + "\n";//		Bone_Sel::RIGHTLEG3
							break;
						default:
							break;
						}
					}
				}
			}
			outputfile << "end=-1\n";
			outputfile.close();
		}
		void Dispose(void) noexcept {
			for (auto& AD : anime) {
				for (auto& ani : AD) {
					ani.Dispose();
				}
				AD.clear();
			}
			anime.clear();
		}
		//以下エディタオンリー
	private:
		Animesdata* nowAnimData{ nullptr }, *nextAnimData{ nullptr };
		int OldSel{ 0 }, NowSel{ 0 };
		int NowFrame{ 0 };
		int Time{ 1 };
		//int Time2{ 1 };
		bool IsEnd{ false };
		bool m_IsPlay{ true };
		bool m_FrameAdd{ false };
	public:
		const auto& GetNowAnim(void) const noexcept { return this->anime[this->NowSel]; }
		auto& SetNowAnim_NowFrame(void) noexcept { return this->anime[this->NowSel][this->NowFrame]; }
		const auto& GetNowFrame(void) const noexcept { return this->NowFrame; }
		void SetSel(Anim_Sel _nowsel) noexcept { NowSel = (int)_nowsel; }

		void ChangePlay(void) noexcept { m_IsPlay ^= 1; }
		void FrameAdvance(void) noexcept { this->m_FrameAdd = true; }
		const auto& isPlay(void) const noexcept { return this->m_IsPlay; }
		bool isEnd(void) noexcept {
			if (this->IsEnd) {
				this->IsEnd = false;
				return true;
			}
			return false;
		}
		void Update(std::vector<Bonesdata>* _bone_base) noexcept {
			if (this->OldSel != this->NowSel) {
				this->NowFrame = 0;
				this->Time = 0;
				this->IsEnd = false;
				this->nowAnimData = this->nextAnimData;
			}
			this->OldSel = this->NowSel;
			this->nextAnimData = &this->anime[this->NowSel][this->NowFrame];
			if (this->nowAnimData != nullptr && this->Time < this->nowAnimData->GetTime()) {
				if (this->m_IsPlay) {
					this->Time++;
				}
				else if (this->m_FrameAdd) {
					this->m_FrameAdd = false;
					this->Time++;
				}
				for (int b = 0; b < _bone_base->size(); b++) {
					(*_bone_base)[b].Leap_Rad(this->nowAnimData->GetBone(b), this->nextAnimData->GetBone(b), (float)this->Time / (float)this->nowAnimData->GetTime());
				}
			}
			else {
				++this->NowFrame %= this->anime[this->NowSel].size();
				if (this->NowFrame == (int)(this->anime[this->NowSel].size()) - 1) {
					this->IsEnd = true;
				}
				this->Time = 0;
				this->nowAnimData = this->nextAnimData;
			}
		}
	};
	class AnimeControl {
	public:
		const Animesdata* nowAnimData{ nullptr }, *nextAnimData{ nullptr };
	private:
		int OldSel{ 0 }, NowSel{ 0 };
		int NowFrame{ 0 };
		int Time{ 1 };
		//int Time2{ 1 };
		bool IsEnd{ false };
		bool m_IsPlay{ true };
		bool m_FrameAdd{ false };
	public:
		void SetSel(Anim_Sel _nowsel) noexcept { NowSel = (int)_nowsel; }
		int GetSel(void) const noexcept { return NowSel; }
		bool isEnd(void) noexcept {
			if (this->IsEnd) {
				this->IsEnd = false;
				return true;
			}
			return false;
		}
	public:
		void Update(std::vector<Bonesdata>* _bone_base, const std::vector<std::vector<Animesdata>>& _anime) noexcept {
			if (this->OldSel != this->NowSel) {
				this->NowFrame = 0;
				this->Time = 0;
				this->IsEnd = false;
				this->nowAnimData = this->nextAnimData;
			}
			this->OldSel = this->NowSel;
			this->nextAnimData = &_anime[this->NowSel][this->NowFrame];
			if (this->nowAnimData != nullptr && this->Time < this->nowAnimData->GetTime()) {
				if (this->m_IsPlay) {
					this->Time++;
				}
				else if (this->m_FrameAdd) {
					this->m_FrameAdd = false;
					this->Time++;
				}
				for (int b = 0; b < _bone_base->size(); b++) {
					(*_bone_base)[b].Leap_Rad(this->nowAnimData->GetBone(b), this->nextAnimData->GetBone(b), (float)this->Time / (float)this->nowAnimData->GetTime());
				}
			}
			else {
				++this->NowFrame %= _anime[this->NowSel].size();
				if (this->NowFrame == (int)(_anime[this->NowSel].size()) - 1) {
					this->IsEnd = true;
				}
				this->Time = 0;
				this->nowAnimData = this->nextAnimData;
			}
		}
	};
}