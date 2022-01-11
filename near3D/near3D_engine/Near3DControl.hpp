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

	//定数
	const int tilesize = 128;	//タイル一つ一つのサイズ
	const int EyeRad = 45;		//視認角度
}