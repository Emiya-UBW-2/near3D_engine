#pragma once
#include "DXLib_ref/DXLib_ref.h"

namespace Near3D {

	//2Dベクトル関連
	class Vector2D_I {
	public:
		int x = 0;
		int y = 0;
		//設定する
		void set(int _x, int _y)noexcept {
			this->x = _x;
			this->y = _y;
		}
		//取得する
		static Vector2D_I Get(int _x, int _y) {
			Vector2D_I p;
			p.set(_x, _y);
			return p;
		}
		//比較
		const bool operator==(const Vector2D_I& _o) const noexcept { return (this->x == _o.x && this->y == _o.y); }
		const bool operator!=(const Vector2D_I& _o) const noexcept { return !(this->x == _o.x); }
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
		//
		const Vector2D_I operator*(float _o) const noexcept { return Get((int)((float)(this->x) * _o), (int)((float)(this->y) * _o)); }
		//距離の2乗を取得する
		const int hydist(void) const noexcept { return this->x * this->x + this->y * this->y; }
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

	enum class HIT_SELECT {
		ONLY_HIT,
		GET_HIT,
		GET_ZURI,
	};
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
		const int camhigh_base = 192;	//カメラの高さ
		Vector2D_I camerapos = { 0,0 };
		float camzoom = 1.f;
	};

	//座標変換
	static Vector2D_I ConvertPos(const Vector2D_I& _pos, int _hight, const Camera_Info& _caminfo) noexcept {
		auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
		if (cam_high > _hight) {
			return Vector2D_I::Get(deskx / 2 + cam_high * _pos.x / (cam_high - _hight), desky / 2 + cam_high * _pos.y / (cam_high - _hight));
		}
		else {
			return Vector2D_I::Get(deskx / 2 + cam_high * _pos.x, deskx / 2 + cam_high * _pos.y);
		}
	}
	//カメラ座標込み
	static Vector2D_I ConvertPos_CalcCam(const Vector2D_I& _pos, int _hight, const Camera_Info& _caminfo) noexcept { return ConvertPos((_pos + _caminfo.camerapos) * _caminfo.camzoom, _hight, _caminfo); }

	//定数
	const int tilesize = 128;	//タイル一つ一つのサイズ
	const int EyeRad = 45;		//視認角度


	typedef std::pair<size_t, int> BoneSort;
	class Bonesdata {
	public:
		int parent = -1;
		Vector2D_I pos;
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
				this->pos = parentB.pos;
				this->pos.x += (int)(cos(this->yr) * x2 - sin(this->yr) * y1);
				this->pos.y += (int)(cos(this->yr) * y1 + sin(this->yr) * x2);
				this->m_hight = parentB.m_hight + (int)(cos(this->zr) * z1 - sin(this->zr) * this->xdist);
				this->edit = true;
				return true;
			}
			return false;
		}
	};
}