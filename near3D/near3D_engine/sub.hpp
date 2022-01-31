#pragma once
#include "Near3DControl.hpp"
#include "Enum.hpp"
#include "Near3DEditer.hpp"

#include <filesystem>

namespace Near3D {
	SoundPool SE;
	SoundPool BGM;
	//描画メイド変更(同じ値の場合スルーするように)
	int Bright_buf = -1;
	void Set_Bright(int _p) noexcept {
		if (_p != Bright_buf) {
			SetDrawBright(_p, _p, _p);
			Bright_buf = _p;
		}
	}
	//マップ情報
	const auto GetMapInfo2(std::string _mapname) noexcept {
		std::fstream file;
		Near3DEditer::Edit::maps mapb;
		file.open(("data/Map/" + _mapname + "/2.dat").c_str(), std::ios::binary | std::ios::in);
		file.read((char*)&mapb, sizeof(mapb));
		file.close();
		return mapb;
	}
	const auto GetMapInfo3(std::string _mapname) noexcept {
		std::fstream file;
		std::vector<Near3DEditer::Edit::Player_Info> info3;
		Near3DEditer::Edit::Player_Info anse;
		file.open(("data/Map/" + _mapname + "/3.dat").c_str(), std::ios::binary | std::ios::in);
		do {
			info3.resize(info3.size() + 1);
			file.read((char*)&anse, sizeof(anse));
			info3.back() = anse;
		} while (!file.eof());
		file.close();
		return info3;
	}
	//カスタムランダム
	static const float GetRandf(float _arg) noexcept { return -_arg + (float)(GetRand((int)(_arg * 2.f * 10000.f))) / 10000.f; }
	//フォント
	FontPool Fonts;
	static const auto& GetFont(int _size) noexcept { return Fonts.Get(_size, DX_FONTTYPE_EDGE, "x14y24pxHeadUpDaisy", true).Get_handle(); }
	//
	class Near3DControl {
		class Tiles {
		public:
			Vector2D_I m_postile;
			int m_bottom{ 0 }, m_hight{ 0 };
			bool m_isWall{ false };
			unsigned char m_dir{ 0 };// rect = 255 else prism = 0~3,4~7
			std::array<Vector2D_I, 4> m_top;
			std::array<Vector2D_I, 4> m_floor;
			std::array<Vector2D_I, 4> m_zero;
			GraphHandle* m_wallhandle{ nullptr };
			GraphHandle* m_floorhandle{ nullptr };
		public:
			const auto GetXIn(int _x) const noexcept { return _x >= m_top[0].x && _x <= m_top[3].x; }
			const auto GetYIn(int _y) const noexcept { return _y >= m_top[0].y && _y <= m_top[3].y; }
		};
		class Object_Common {
		protected:
			int m_Base_Hight{ 0 };
			int m_hight{ 0 };
			float m_Yrad{ 0.f };
			float m_speed{ 0.f };
		public:
			Vector2D_I pos;
		protected:
			const auto GetCanDraw(const Tiles& _Ti, const Vector2D_I& _ConvertedPos) const noexcept { return _Ti.GetXIn(_ConvertedPos.x) && _Ti.GetYIn(_ConvertedPos.y); }
			const auto GetCanDraw_CheckHight(const Tiles& _Ti, const Vector2D_I& _ConvertedPos) const noexcept { return GetCanDraw(_Ti, _ConvertedPos) && (_Ti.m_hight <= this->m_Base_Hight); }
		public:
			const auto& Getyrad(void) const noexcept { return m_Yrad; }
			const auto& GetHight(void) const noexcept { return m_hight; }
			void SetPos(Object_Common* _have) noexcept { this->pos = _have->pos; }
			void SetHight(const Tiles& _Ti) noexcept { this->m_Base_Hight = _Ti.m_hight; }
			void UpdateSpeed(float _SpeedSub) noexcept {
				float spd = this->m_speed * 60.f / FPS;
				this->pos += Vector2D_I::Get((int)(sin(this->m_Yrad) * spd), (int)(-cos(this->m_Yrad) * spd));
				this->m_speed = std::max(this->m_speed - _SpeedSub / FPS, 0.f);
			}
		public:
			void Init_Common(Object_Common* _have) noexcept {
				this->pos = _have->pos;
				this->m_Base_Hight = _have->m_Base_Hight;
				this->m_Yrad = _have->m_Yrad;
				this->m_hight = _have->m_hight;
			}
			const auto Draw_Common(const Tiles& _Ti, const Vector2D_I& _pos_add, const Camera_Info& _caminfo, int _hight, float _Yrad, const GraphHandle& _graph) const noexcept {
				auto P = ConvertPos_CalcCam(this->pos + _pos_add, this->m_Base_Hight, _caminfo);
				if (GetCanDraw_CheckHight(_Ti, P)) {
					auto zh = this->m_Base_Hight + _hight;
					auto ConvPos = ConvertPos_CalcCam(this->pos + _pos_add, zh, _caminfo);
					auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
					Set_Bright(255 - 255 * std::clamp(zh, 0, cam_high) / cam_high);
					DrawRota_wrap(ConvPos, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom, _Yrad, _graph, TRUE);
					return true;
				}
				return false;
			}
			const auto Draw_Common(const Tiles& _Ti, const Vector2D_I& _pos_add, const Camera_Info& _caminfo, float _Yrad, const GraphHandle& _graph) const noexcept {
				return Draw_Common(_Ti, _pos_add, _caminfo, this->m_hight, _Yrad, _graph);
			}

			void Draw_Shadow_Point_Common(const Tiles& _Ti, const Vector2D_I& _lightpos, const Vector2D_I& _pos, int _high, float _ShadowRange, const Camera_Info& _caminfo, float _Yrad, const GraphHandle& _graph) const noexcept {
				auto P = ConvertPos_CalcCam(_pos, this->m_Base_Hight, _caminfo);
				if (GetCanDraw(_Ti, P)) {
					auto zh = this->m_Base_Hight + this->m_hight;
					auto p2 = GetPointShadowPos(_lightpos, _pos, _ShadowRange, _high, _Ti.m_bottom, zh, _caminfo);
					auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
					DrawRota_wrap(p2, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom, _Yrad, _graph, TRUE);
				}
			}
			void Draw_Shadow_Common(const Tiles& _Ti, float _light_yrad, const Vector2D_I& _pos_add, float _ShadowRange, const Camera_Info& _caminfo, float _Yrad, const GraphHandle& _graph) const noexcept {
				auto P = ConvertPos_CalcCam(this->pos + _pos_add, this->m_Base_Hight, _caminfo);
				if (GetCanDraw(_Ti, P)) {
					auto zh = this->m_Base_Hight + this->m_hight;
					auto zh2 = float(zh - _Ti.m_hight) * _caminfo.camzoom * _ShadowRange;
					auto p2 = ConvertPos_CalcCam(this->pos + _pos_add + Vector2D_I::Get((int)(zh2 * sin(_light_yrad)), (int)(zh2 * cos(_light_yrad))), _Ti.m_hight, _caminfo);
					auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
					DrawRota_wrap(p2, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom, _Yrad, _graph, TRUE);
				}
			}
			void Draw_Light_Common(const Tiles& _Ti, const Camera_Info& _caminfo, float _size) const noexcept {
				auto P = ConvertPos_CalcCam(this->pos, this->m_Base_Hight, _caminfo);
				if (GetCanDraw(_Ti, P)) {
					auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
					DrawCircle(P.x, P.y, (int)(float(this->m_Base_Hight + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom * _size), GetColor(255, 255, 255), TRUE);
				}
			}
		};
		class Object_Vanish : public Object_Common {
		protected:
			float Time{ 0.f };
		public:
			void UpdateTime(void) noexcept {
				this->Time = std::max(this->Time - 1.f / FPS, 0.f);
			}
			const auto iaActive(void) const noexcept { return this->Time > 0.f; }
			const auto isEnd(void) const noexcept { return this->Time <= 0.f; }
		};
		class Gun_Object;
		class Human_Graph {
		public:
			std::vector<GraphHandle> m_HGraphs;
			std::vector<Bonesdata> m_bonedata;
		public:
			void Load(int _sel) noexcept {
				GraphHandle::LoadDiv("data/Char/" + std::to_string(_sel) + ".bmp", 33, 11, 3, 32, 32, &this->m_HGraphs);
				//キャラバイナリ
				std::fstream file;
				file.open("data/Char/" + std::to_string(_sel) + ".dat", std::ios::binary | std::ios::in);
				do {
					this->m_bonedata.resize(this->m_bonedata.size() + 1);
					file.read((char*)&this->m_bonedata.back(), sizeof(this->m_bonedata.back()));
				} while (!file.eof());
				this->m_bonedata.pop_back();
				file.close();
			}
			void Dispose(void) noexcept {
				this->m_HGraphs.clear();
			}
		};
		class Human_Object : public Object_Common {
			class FootprintControl {
				class foots : public Object_Vanish {
					Bone_Sel ID{ Bone_Sel::NUM };
					float yr{ 0.f };
					float yrad{ 0.f };
					float Per{ 1.f };
				public:
					Vector2D_I pos;
				public:
					void Set(const Vector2D_I& _PlayerPos, std::vector<Bonesdata>& _bone, Bone_Sel _ID, float _per) noexcept {
						this->ID = _ID;
						this->pos = _PlayerPos + _bone[(int)this->ID].pos;
						this->yr = _bone[(int)this->ID].yr;
						this->yrad = _bone[(int)this->ID].yrad;
						this->Per = _per;
						this->Time = 5.f;
					}
					void Draw(const Tiles& _Ti, const std::vector<GraphHandle>& _Graphs, const Camera_Info& _caminfo) const noexcept {
						auto P = ConvertPos_CalcCam(this->pos, _Ti.m_hight, _caminfo);
						if (GetCanDraw(_Ti, P)) {
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp((int)(255.f * this->Per), 0, 255));
							auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
							DrawRota_wrap(P, float(_Ti.m_hight + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom, this->yrad + this->yr, _Graphs[(int)this->ID], TRUE);
						}
					}
				};
			private:
				Bone_Sel prev_foot{ Bone_Sel::NUM };
				float CoolDown{ 0 };
				std::vector<foots> Footprint;
			public:
				void NextStage(const Vector2D_I& _move) noexcept {
					for (auto& g : this->Footprint) {
						g.pos += _move;
					}
				}
				void Update(const Vector2D_I& PlayerPos, std::vector<Bonesdata>& _bone, Bone_Sel now_foot, bool is_Run, float m_Speed, const Camera_Info& _caminfo) noexcept {
					if (this->CoolDown >= 10.f / 60.f && this->prev_foot != now_foot) {
						this->Footprint.resize(this->Footprint.size() + 1);
						this->Footprint.back().Set(PlayerPos, _bone, now_foot, 1.f - 1.f / std::max(m_Speed / 2.f, 1.f));
						this->prev_foot = now_foot;
						this->CoolDown = 0.f;
						if (now_foot == Bone_Sel::LEFTLEG3 || now_foot == Bone_Sel::RIGHTLEG3) {
							PlaySound_Near3D((is_Run) ? ENUM_SOUND::RUN : ENUM_SOUND::WALK, PlayerPos, _caminfo, 128);
						}
					}
					this->CoolDown += 1.f / FPS;
					bool tt;
					while (true) {
						tt = true;
						for (int i = 0; i < this->Footprint.size(); i++) {
							this->Footprint[i].UpdateTime();
							if (this->Footprint[i].isEnd()) {
								this->Footprint[i] = this->Footprint.back();
								this->Footprint.pop_back();
								tt = false;
								break;
							}
						}
						if (tt) { break; }
					}
				}
				void Draw(const Tiles& _Ti, const std::vector<GraphHandle>& _Graphs, const Camera_Info& _caminfo) const noexcept {
					Set_Bright(0);
					for (auto& g : this->Footprint) { g.Draw(_Ti, _Graphs, _caminfo); }
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				}
				void Dispose(void) noexcept {
					this->Footprint.clear();
				}
			};
			class SWITCH {
			public:
				bool Press{ false };
				bool Switch{ false };
				bool Start{ false };
				bool On{ false };

				void First(void) noexcept {
					this->Start = true;
					this->On = true;
				}
				void End(void) noexcept {
					this->Start = false;
					this->On = false;
				}
				void Update(void) noexcept {
					if (this->On) {
						this->Start = false;
					}
				}
			};
			//AI用
			class AI {
				std::vector<int> wayp_pre{ 0 };
			public:
				void Init(void) noexcept {
					wayp_pre.resize(6);
				}
				void Dispose(void) noexcept {
					wayp_pre.clear();
				}
				void Spawn(int now) noexcept { this->Fill(now); }
				void Fill(int now) noexcept { for (auto& w : this->wayp_pre) { w = now; } }
				void Push(int now) noexcept {
					for (size_t i = (this->wayp_pre.size() - 1); i >= 1; i--) { this->wayp_pre[i] = this->wayp_pre[i - 1]; }
					this->wayp_pre[0] = now;
				}
				void PushBack(void) noexcept {
					auto ppp = this->wayp_pre[1];
					for (size_t i = (this->wayp_pre.size() - 1); i >= 1; i--) { this->wayp_pre[i] = this->wayp_pre[0]; }
					this->wayp_pre[0] = ppp;
				}
				const auto& GetFront(void) const noexcept { return wayp_pre.front(); }
				const auto Compare(int _id) const noexcept {
					for (auto& ww : this->wayp_pre) { if (_id == ww) { return false; } }
					return true;
				}
			};
		private:
			bool m_isPlayer{ false };
			std::vector<BoneSort> sort;
			std::vector<Bonesdata> m_bone;
			std::vector<bool> draw_ok = { false };
			AnimeControl m_anime;
			float m_Damageinter{ 0.f };
			float m_DamageTimerStart{ 0.f };
			FootprintControl m_Footprint;
			Human_Graph* m_HumanData{ nullptr };
			bool draw_end{ false };
			float m_Yrad_buf{ 0.f };
			float yrad_aim{ 0 };
			float yrad_aim_speed{ 0 };
			Vector2D_I spawnpos;
			Vector2D_I vec_real;					//キャラスプライトが実際に向いている向き
			Vector2D_I vec_buf;						//移動方向
			int DamageDown{ false };				//ダメージ
			float MeleeDamageSpeed{ 0 };			//ダメージ
			Human_Object* AttackHuman{ nullptr };	//当てた人
			float changingtime{ 0.f };				//姿勢
			float RollingSpeed{ 0 };
			SWITCH m_Damage;						//ダメージ
			SWITCH m_Rolling;						//ローリング
			SWITCH m_Shoot;							//射撃
			SWITCH m_Melee;							//近接
			SWITCH m_Reload;						//リロード
			float MeleeSpeed{ 0 };
			int  MeleeCnt{ 0 };
			float ZoomStopTime{ 0 };
			float ZoomBuf{ 0.8f };
			bool isHitWall{ false };				//銃が壁の中にあるか
			bool changing{ false };
			bool standup{ false };
			bool isStand{ true };					//立っているか
			bool isDown{ false };					//ダウンしているか
			bool isRun{ false };					//走っているか否か
			bool isReadyGun{ false };				//銃を構えるか否か
			bool ishaveGStart{ false };
			float isFoundEnemyTime{ 0.f };
			int m_phase{ 0 };
			float transceiverTimer{ 0.f };
		public:
			Gun_Object* haveGun{ nullptr };
			AI cpu_do;
			SWITCH m_Transceiver;					//無線
			float Eye_Rad = 60.f;
			float Eye_Range = 1.f;
			float Stop_Time = 0.f;
			float Walk_Time = 10.f;
			float Look_Rad = 0.f;
		private:
			const auto IsDamage(void) const noexcept { return this->AttackHuman != nullptr && (this->m_Damageinter == 0.f); }
			void Update_FootPrint(const Camera_Info& _caminfo) noexcept {
				//足跡座標指定
				bool next = false;
				do {
					next = false;
					for (auto& bo : this->m_bone) {
						if (!bo.edit) {
							if (bo.parent == -1) {
								bo.Update_Parent(this->m_Yrad, this->yrad_aim, this->yrad_aim_speed);
							}
							else {
								if (!bo.Update_Child(this->m_bone[bo.parent], this->yrad_aim, this->yrad_aim_speed)) {
									next = true;
								}
							}
						}
					}
				} while (next);
				//高さでソート
				for (int i = 0; i < this->sort.size(); i++) { this->sort[i] = { i, this->m_bone[i].m_hight }; }
				std::sort(this->sort.begin(), this->sort.end(), [](const BoneSort& x, const BoneSort& y) { return x.second < y.second; });
				//一番低い場所に跡を置く
				this->m_Footprint.Update(this->pos, this->m_bone, (Bone_Sel)(this->sort.front().first), this->isRun, this->GetSpeed(), _caminfo);
			}
		public:
			const auto isDownMeleeAttack(void) const noexcept { return this->MeleeCnt == 2; }
			const auto GetPhase(void) const noexcept { return this->m_phase; }
			const auto isAlart(void) const noexcept { return this->m_phase >= 3; }
			const auto isCaution(void) const noexcept { return this->m_phase == 2; }
			const auto isHaveGun(void) const noexcept { return this->haveGun != nullptr; }
			const auto isReloadStart(void) const noexcept { return this->m_Reload.Start; }
			const auto GetLookyrad(void) const noexcept { return m_Yrad + this->yrad_aim; }
			const auto GetLookyrad2(void) const noexcept { return m_Yrad + this->yrad_aim; }
			const auto Getvec_real(float _P) const noexcept { return this->vec_real * (_P / sqrt((float)this->vec_real.hydist())); }
			const auto isMove(void) const noexcept { return this->vec_buf.x != 0 || this->vec_buf.y != 0; }//移動するか
			const auto CanFindEnemy(void) const noexcept { return !this->m_Damage.On && this->m_DamageTimerStart == 0.f; }
			const auto& Get_vec_buf(void) const noexcept { return this->vec_buf; }
			const auto& GetRightHandInfo(void) const noexcept { return this->m_bone[(int)Bone_Sel::RIGHTHAND]; }
			const auto& GetHOLSTERInfo(void) const noexcept { return this->m_bone[(int)Bone_Sel::HOLSTER]; }
			const auto& GetBodyTopInfo(void) const noexcept { return this->m_bone[(int)Bone_Sel::BODYTOP]; }
			const auto& GetHeadInfo(void) const noexcept { return this->m_bone[(int)Bone_Sel::HEAD]; }
			const auto& GetBaseHight(void) const noexcept { return this->sort.front().second; }
			const auto& isDamageDown(void) const noexcept { return this->DamageDown; }
			const auto& IsPlayer(void) const noexcept { return this->m_isPlayer; }
		public:
			void SetPlayerSpawnPos(const Vector2D_I& _pos) noexcept {
				this->spawnpos = _pos;
				this->pos = this->spawnpos;
			}
			auto GetCamZoom(void) noexcept {
				if (this->isMove()) {
					this->ZoomStopTime = 1.f;
					this->ZoomBuf = 0.8f + ((float)sqrt(this->vec_buf.hydist()) / this->GetSpeed()) * ((this->isReadyGun) ? 0.15f : 0.3f);
				}
				else {
					if (this->ZoomStopTime > 0) {
						this->ZoomStopTime -= 1.f / FPS;
					}
					else {
						this->ZoomBuf = 0.8f;
					}
				}
				return this->ZoomBuf;
			}
			void SetStand(bool is_stand) noexcept {
				bool oldstand = this->isStand;
				if (!this->m_Reload.On) {
					if (is_stand) {
						this->isStand ^= 1;
					}
				}
				if (!this->isStand && this->isRun) {//しゃがみ解除
					this->isStand = true;
				}
				//移行アクション
				if (oldstand != this->isStand) { this->changing = true; }
			}
			const auto GetMeleePoint(void) const noexcept {
				int t = -y_r(tilesize);
				Vector2D_I vec;//移動方向
				vec.set((int)(-sin(this->m_Yrad) * t), (int)(cos(this->m_Yrad) * t));
				return this->pos + vec;
			}
			void Damage(Human_Object* Attacker, const Camera_Info& _caminfo, bool _isDownDamage, bool isMelee) noexcept {
				if (Attacker != nullptr && !this->m_Damage.On) {
					this->m_DamageTimerStart = 0.5f;
					this->m_Damage.First();
					this->AttackHuman = Attacker;
					this->DamageDown = _isDownDamage;
					if (this->DamageDown) {
						if (isMelee) {
							this->MeleeDamageSpeed = -20.f;
						}
						else {
							this->MeleeDamageSpeed = -7.f;
						}
						PlaySound_Near3D(ENUM_SOUND::DownHit, this->pos, _caminfo);
					}
					else {
						if (isMelee) {
							this->MeleeDamageSpeed = -5.f;
						}
						else {
							this->MeleeDamageSpeed = -7.f;
						}
						PlaySound_Near3D(ENUM_SOUND::Hit, this->pos, _caminfo);
					}
					CautionEnemy();
				}
			}
			const auto MeleeHit(Human_Object* Attacker) const noexcept {
				return Attacker->m_Melee.Start && !this->isDown && (this->pos - Attacker->GetMeleePoint()).dist() <= y_r(tilesize);
			}
			const float GetSpeed(void) noexcept {
				if (!this->isStand) {
					if (this->m_Reload.On) {
						this->m_speed = 0.f;
					}
					else {
						this->m_speed = (this->isRun) ? 6.f : 1.f;
					}
				}
				else {
					this->m_speed = (this->isRun) ? 6.f : 3.f;
				}
				return this->m_speed;
			}
			void SetKey(const bool Aim, const bool shoot, const bool reload, const bool rolling, const Camera_Info& _caminfo) noexcept {
				if (!this->m_Melee.On) {
					this->m_Reload.Press = (Aim && isHaveGun()) & reload;	//リロード
					bool OLD = this->isReadyGun;
					this->isReadyGun = (Aim && isHaveGun());					//エイム
					if (OLD != this->isReadyGun) {
						PlaySound_Near3D(ENUM_SOUND::Equip, this->pos, _caminfo);
					}
					this->m_Rolling.Press = rolling;		//ローリング
				}
				this->m_Shoot.Press = shoot;			//射撃キー押し
			}
			void Reset(void) noexcept {
				std::fill<>(this->draw_ok.begin(), this->draw_ok.end(), false);
				this->draw_end = false;
			}
			void SetGun(Gun_Object* _haveGun) noexcept {
				if (this->isHaveGun()) {
					this->haveGun->SetHuman(nullptr);
				}
				this->haveGun = _haveGun;
				if (this->isHaveGun()) {
					this->ishaveGStart = true;
					this->haveGun->SetHuman(this);
				}
			}
			void Update_Human(std::vector<std::vector<Tiles>>& _tile, const Vector2D_I& Vec, Vector2D_I Aim, bool is_Run, int lowestphase) noexcept {
				this->isRun = is_Run;
				//エイム指定
				int Limit = this->isStand ? 60 : 45;
				if (this->isReadyGun) {
					Limit = this->isStand ? 120 : 45;
				}
				if (this->m_anime.GetSel() == 2) {
					Limit = 20;
				}
				if (
					this->m_Melee.On ||
					this->IsDamage() ||
					this->isDown ||
					this->m_Rolling.On ||
					Aim == Vector2D_I::Zero()
					) {
					Limit = 0;
				}
				if (this->AttackHuman != nullptr) {
					float vecrange = 100000;//intで保持しているためvecrange倍
					Aim = this->AttackHuman->pos * vecrange - this->pos * vecrange;
				}
				float rad = std::atan2f((float)Aim.x, (float)-Aim.y) - this->m_Yrad;
				rad = std::atan2f(sin(rad), cos(rad));
				this->yrad_aim_speed = this->yrad_aim;
				easing_set(&this->yrad_aim, std::clamp(rad, deg2rad(-Limit), deg2rad(Limit)), 0.9f);
				if (this->m_Melee.Start || this->m_Damage.Start) {
					this->m_Yrad = this->m_Yrad + rad;
					this->m_Yrad_buf = this->m_Yrad;
				}
				this->yrad_aim_speed = (this->yrad_aim - this->yrad_aim_speed) * FPS * 0.1f;
				//移動
				auto OLDpos = this->pos;
				if (!(this->IsDamage())) {
					if (!this->m_Melee.On) {
						if (!this->m_Rolling.On) {
							this->pos += Vec * (this->GetSpeed() * 60.f / FPS);
						}
						//伏せ時に前に動く
						if (this->changing && !this->isStand) {
							this->pos += this->Getvec_real(-5.f * 60.f / FPS);
						}
						//ローリング
						if (this->m_Rolling.On) {
							this->pos += this->Getvec_real(-(this->GetSpeed() + this->RollingSpeed) * 60.f / FPS);
							easing_set(&this->RollingSpeed, 0.f, 0.95f);
						}
					}
					else {
						Vector2D_I vec;//移動方向
						int vecrange = 100000;//intで保持しているためvecrange倍
						vec.set((int)(-sin(this->m_Yrad) * vecrange), (int)(cos(this->m_Yrad) * vecrange));
						this->pos += vec * ((-this->MeleeSpeed * 60.f / FPS) / vecrange);
						easing_set(&this->MeleeSpeed, 0.f, 0.9f);
					}
				}
				else {
					Vector2D_I vec;//移動方向
					int vecrange = 100000;//intで保持しているためvecrange倍
					vec.set((int)(-sin(this->m_Yrad) * vecrange), (int)(cos(this->m_Yrad) * vecrange));
					this->pos += vec * ((-this->MeleeDamageSpeed * 60.f / FPS) / vecrange);
					easing_set(&this->MeleeDamageSpeed, 0.f, 0.95f);
					this->vec_real = vec;
				}
				//当たり判定
				Get_HitWall(_tile, &this->pos, OLDpos, y_r(tilesize) * 19 / 40, HIT_SELECT::GET_ZURI);
				OLDpos -= this->pos;
				this->vec_buf = OLDpos;
				if (!this->IsDamage()) {
					if (isMove()) {
						this->vec_real = this->vec_buf;
					}
				}
				//銃の接触
				bool isHit = false;
				if (this->isReadyGun) {
					Vector2D_I OLD = this->pos;
					Vector2D_I NOW = OLD - Vector2D_I::Get((int)(-sin(this->m_Yrad + this->yrad_aim) * 60.f), (int)(cos(this->m_Yrad + this->yrad_aim) * 60.f));
					isHit = Get_HitWall(_tile, &NOW, OLD, 10, HIT_SELECT::ONLY_HIT);
				}
				this->isHitWall = this->isReadyGun && isHit;
				this->m_phase = std::max(this->m_phase, lowestphase);
				if (this->m_phase > lowestphase && !this->m_Transceiver.Start) {
					this->isFoundEnemyTime -= 1.f / FPS;
					if (this->isFoundEnemyTime < 0.f) {
						this->m_phase--;
						this->isFoundEnemyTime = 5.f;
					}
				}
				if (this->m_Transceiver.Start) {
					this->transceiverTimer -= 1.f / FPS;
					if (this->transceiverTimer < 0.f) {
						this->m_Transceiver.Start = false;
					}
				}
			}
			void CautionEnemy(void) noexcept {
				this->m_phase = 2;
				this->isFoundEnemyTime = 5.f;
			}
			void FindEnemy(void) noexcept {
				this->m_phase = 4;
				this->isFoundEnemyTime = 5.f;
			}
			void EnableTransceiver(void) noexcept {
				this->m_Transceiver.Start = true;
				this->transceiverTimer = 5.f;
			}
			void LookGun(void) noexcept { this->ishaveGStart = true; }
			void Draw_GunUp(int _dispx, int _dispy) noexcept {
				if (this->isHaveGun()) {
					if (this->ishaveGStart) {
						this->ishaveGStart = false;
						this->haveGun->LookGunStart();
					}
					this->haveGun->Draw_Up(_dispx, _dispy);
				}
			}
			void Draw_UI(std::vector<std::vector<Tiles>>& _tile, const Camera_Info& _caminfo, int _x, int _y, int s_x, int s_y) noexcept {
				auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
				auto zh = this->m_Base_Hight + (this->GetHeadInfo().m_hight - this->sort.front().second);
				auto ConvPos = ConvertPos_CalcCam(this->pos + Vector2D_I::Get(s_x, s_y), zh, _caminfo);
				if (in2_(ConvPos.x, ConvPos.y, 0 - this->Eye_Range, 0 - this->Eye_Range, _x + this->Eye_Range, _y + this->Eye_Range)) {
					GetFont(y_r(25)).DrawStringFormat(ConvPos.x + y_r(12), ConvPos.y + y_r(12), GetColor(255, 0, 0), "Phase : %d", this->m_phase);

					//GetFont(y_r(25)).DrawStringFormat(ConvPos.x + y_r(12), ConvPos.y + y_r(12) + y_r(25), GetColor(255, 0, 0), "WayPoint : %d", this->cpu_do.GetFront());
					//GetFont(y_r(25)).DrawStringFormat(ConvPos.x + y_r(12), ConvPos.y + y_r(12) + y_r(25)*2, GetColor(255, 0, 0), "WalkTime : %.2f", this->Stop_Time);
					GetFont(y_r(25)).DrawStringFormat(ConvPos.x + y_r(12), ConvPos.y + y_r(12) + y_r(25)*2, GetColor(255, 0, 0), "WalkTime : %.2f", rad2deg(this->yrad_aim));
					
					if (CanFindEnemy()) {
						auto Color = isAlart() ? GetColor(255, 0, 0) : (isCaution() ? GetColor(255, 255, 0) : GetColor(0, 0, 255));
						float Range = float(zh + cam_high) / cam_high * (float)(-this->Eye_Range) * _caminfo.camzoom;
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
						int Div = 60;
						for (int rad = -Div; rad <= Div; rad++) {
							float RAD0 = GetLookyrad2() - deg2rad((float)rad * this->Eye_Rad / Div);
							float RAD05 = GetLookyrad2() - deg2rad(((float)rad + 0.5f) * this->Eye_Rad / Div);
							float RAD1 = GetLookyrad2() - deg2rad(((float)rad + 1.f) * this->Eye_Rad / Div);
							Vector2D_I V2 = Vector2D_I::Get((int)(-sin(RAD0) * Range), (int)(cos(RAD0) * Range));
							Vector2D_I V3 = Vector2D_I::Get((int)(-sin(RAD1) * Range), (int)(cos(RAD1) * Range));
							Vector2D_I Buf = this->pos + Vector2D_I::Get((int)(-sin(RAD05) * (float)(-this->Eye_Range)), (int)(cos(RAD05) * (float)(-this->Eye_Range)));
							if (Get_HitWall(_tile, &Buf, this->pos, 0, HIT_SELECT::GET_HIT)) {
								float Range_buf = (float)(zh + cam_high) / (float)cam_high * (float)(-sqrt((Buf - this->pos).hydist())) * _caminfo.camzoom;
								V2 = Vector2D_I::Get((int)(-sin(RAD0) * Range_buf), (int)(cos(RAD0) * Range_buf));
								V3 = Vector2D_I::Get((int)(-sin(RAD1) * Range_buf), (int)(cos(RAD1) * Range_buf));
							}
							Draw_Triangle(ConvPos, ConvPos + V2, ConvPos + V3, Color, true);
						}
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					}
				}
			}
			void NextStage(const Vector2D_I& _move) noexcept {
				this->m_Footprint.NextStage(_move);
			}
		public:
			void Dispose(void) noexcept {
				this->sort.clear();
				this->m_bone.clear();
				this->m_HumanData = nullptr;
				this->draw_ok.clear();
				this->cpu_do.Dispose();
				this->m_Footprint.Dispose();
			}
			//開始
			void Init(Human_Graph* _GraphPtr, bool _isPlayer = false) noexcept {
				this->m_isPlayer = _isPlayer;
				//使用テクスチャ
				this->m_HumanData = _GraphPtr;


				this->m_bone = this->m_HumanData->m_bonedata;
				this->sort.resize(this->m_bone.size());
				this->draw_ok.resize(this->m_bone.size());

				int vecrange = 100000;//intで保持しているためvecrange倍
				this->vec_real.set((int)(-sin(this->m_Yrad) * vecrange), (int)(cos(this->m_Yrad) * vecrange));
				this->cpu_do.Init();

				this->m_Transceiver.Switch = false;

				this->Walk_Time = 7.f + (float)GetRandf(5.f);
			}
			void Init(const Vector2D_I& p_s, Human_Graph* _GraphPtr, bool _isPlayer = false) noexcept {
				SetPlayerSpawnPos(p_s);
				this->m_Yrad = deg2rad(90);
				this->m_Yrad_buf = this->m_Yrad;
				Init(_GraphPtr, _isPlayer);
			}
			//更新
			void Update(const Camera_Info& _caminfo, const std::vector<std::vector<Animesdata>>& _anime) noexcept {
				//アニメーション選択
				if (!this->IsDamage()) {
					if (this->m_Rolling.On) {
						this->m_anime.SetSel(Anim_Sel::ROLLING);
					}
					else {
						if (!this->changing) {
							if (this->isStand) {
								if (isMove()) {
									if (!this->isRun) {
										if (!this->isReadyGun) {
											this->m_anime.SetSel(Anim_Sel::WALK);
										}
										else {
											if (this->m_Reload.On) {
												this->m_anime.SetSel(Anim_Sel::RELOADWALK);
											}
											else {
												if (this->isHitWall) {
													this->m_anime.SetSel(Anim_Sel::EQUIPWALK_HITWALL);
												}
												else {
													this->m_anime.SetSel(Anim_Sel::EQUIPWALK);
												}
											}
										}
									}
									else {
										this->m_anime.SetSel(Anim_Sel::RUN);
									}
								}
								else {
									if (!this->isReadyGun) {
										this->m_anime.SetSel(Anim_Sel::STAND);
									}
									else {
										if (this->m_Reload.On) {
											this->m_anime.SetSel(Anim_Sel::RELOADSTAND);
										}
										else {
											if (this->isHitWall) {
												this->m_anime.SetSel(Anim_Sel::EQUIPSTAND_HITWALL);
											}
											else {
												this->m_anime.SetSel(Anim_Sel::EQUIPSTAND);
											}
										}
									}
								}
								if (this->m_Melee.On) {
									this->m_anime.SetSel((Anim_Sel)(16 + this->MeleeCnt));
								}
							}
							else {
								if (isMove()) {
									if (!this->isReadyGun) {
										this->m_anime.SetSel(Anim_Sel::PRONEMOVE);
									}
									else {
										this->m_anime.SetSel(Anim_Sel::EQUIPPRONEMOVE);
									}
								}
								else {
									if (!this->isReadyGun) {
										this->m_anime.SetSel(Anim_Sel::PRONE);
									}
									else {
										if (this->m_Reload.On) {
											this->m_anime.SetSel(Anim_Sel::RELOADPRONE);
										}
										else {
											this->m_anime.SetSel(Anim_Sel::EQUIPPRONE);
										}
									}
								}
							}
						}
						else {
							this->m_anime.SetSel(Anim_Sel::SQUAT);
						}
					}
				}
				else {
					if (this->DamageDown) {
						if (this->isDown) {
							this->m_anime.SetSel(Anim_Sel::DOWN);
						}
						else {
							this->m_anime.SetSel(Anim_Sel::DOWNDAMAGE);
						}
					}
					else {
						this->m_anime.SetSel(Anim_Sel::NOMALDAMAGE);
					}
				}
				//アニメーション更新
				for (auto& g : this->m_bone) { g.edit = false; }
				this->m_anime.Update(&this->m_bone, _anime);
				//その他更新
				this->m_Damage.Update();
				this->m_Rolling.Update();
				this->m_Shoot.Update();
				this->m_Melee.Update();
				this->m_Reload.Update();
				//ローリング
				if (!this->IsDamage()) {
					if ((this->isStand && this->m_Rolling.Press) && !this->m_Rolling.On) {
						this->RollingSpeed = 15.f;
						this->m_Rolling.First();
						PlaySound_Near3D(ENUM_SOUND::rolling, this->pos, _caminfo, 255);
					}
					if (!this->m_Rolling.On && this->changing) {
						this->changingtime += 1.f / FPS;
						if (this->changingtime >= 0.2f) {
							this->changingtime = 0.f;
							this->changing = false;
						}
					}
				}
				if (this->m_Rolling.On && this->m_anime.isEnd()) {
					this->m_Rolling.End();
				}
				//メレー
				if (this->m_Melee.On && this->m_anime.isEnd()) {
					if (this->MeleeCnt < 2) {
						this->MeleeCnt++;
						this->MeleeSpeed = 5.f;
						if (this->isDownMeleeAttack()) {
							PlaySound_Near3D(ENUM_SOUND::Kick, this->pos, _caminfo);
						}
						else {
							PlaySound_Near3D(ENUM_SOUND::Punch, this->pos, _caminfo);
						}
						this->m_Melee.First();
					}
					else {
						this->MeleeCnt = 0;
						this->m_Melee.End();
					}
				}
				//リロード
				if (this->m_Reload.On) {
					bool reload_off = false;
					reload_off |= this->m_Rolling.On;//ローリングリロキャン
					reload_off |= !this->isReadyGun;//構えリロキャン
					if (this->isReadyGun && this->m_anime.isEnd()) {//リロード完了
						this->haveGun->ReloadEnd();
						reload_off = true;
					}
					if (reload_off) {
						this->m_Reload.End();
						PlaySound_Near3D(ENUM_SOUND::MagDown0, this->pos, _caminfo);
					}
				}
				//ダメージ
				this->m_DamageTimerStart = std::max(this->m_DamageTimerStart - 1.f / FPS, 0.f);
				this->m_Damageinter = std::max(this->m_Damageinter - 1.f / FPS, 0.f);
				if (this->m_Damage.On) {
					if (!this->DamageDown) {//19
						if (this->m_anime.isEnd()) {
							this->AttackHuman = nullptr;
							this->m_Damage.End();
							this->m_Damageinter = 0.1f;
						}
					}
					else {
						if (!this->isDown) {//20
							if (this->m_anime.isEnd()) {
								this->isDown = true;
							}
						}
						else {//21 起き上がり
							if (this->m_anime.isEnd()) {
								this->isDown = false;
								this->AttackHuman = nullptr;
								this->DamageDown = false;
								this->m_Damage.End();
							}
						}
					}
				}
				//移動方向に向く
				if (!(
					this->m_Melee.On ||
					this->IsDamage() ||
					this->isDown
					)) {
					Vector2D_I vec;//移動方向
					int vecrange = 100000;//intで保持しているためvecrange倍
					vec.set((int)(-sin(this->m_Yrad_buf) * vecrange), (int)(cos(this->m_Yrad_buf) * vecrange));
					auto Range = (int)(sqrt(this->vec_real.hydist()) * vecrange);
					auto Cross = this->vec_real.cross(vec);
					auto Dot = this->vec_real.dot(vec);
					{
						if (Cross > sin(deg2rad(10)) * Range) {
							this->m_Yrad_buf -= deg2rad(5);
						}
						else if (Cross < sin(deg2rad(10)) * -Range) {
							this->m_Yrad_buf += deg2rad(5);
						}
						//真後ろ振り向き
						if (Dot <= -0.5f * Range) {
							this->m_Yrad_buf += deg2rad(10);
							if (this->m_Rolling.On) {
								this->m_Yrad_buf += deg2rad(40);
							}
						}
						if (IsPlayer()) {
							this->m_Yrad = this->m_Yrad_buf;
						}
						else {
							easing_set(&this->m_Yrad, this->m_Yrad_buf, 0.9f);
						}
					}
				}
				//足跡の更新
				Update_FootPrint(_caminfo);
				//銃の更新
				{
					//銃表示更新
					if (this->isHaveGun()) {
						bool canShootGun = false;//銃を撃てるか否か
						if (this->isReadyGun) {
							this->haveGun->isDraw = true;
							canShootGun = true;
						}
						if (!this->isReadyGun ||
							this->m_anime.GetSel() == 2 ||
							this->m_Rolling.On) {
							this->haveGun->isDraw = false;
							canShootGun = false;
						}
						if (this->haveGun->CanLookGun()) {
							canShootGun = false;
						}
						if (this->isReadyGun) {
							if (this->m_anime.GetSel() == 8 ||
								this->isHitWall ||
								this->m_Reload.On) {
								this->m_Shoot.Press = false;
							}
						}
						else {
							if (!(
								this->m_anime.GetSel() == 0 ||
								this->m_anime.GetSel() == 1 ||
								this->m_anime.GetSel() == 2
								)
								) {
								this->m_Shoot.Press = false;
							}

							if (this->m_Shoot.Press) {
								if (!this->m_Melee.On) {
									this->m_Melee.First();
									this->MeleeSpeed = 5.f;
									PlaySound_Near3D(ENUM_SOUND::Punch, this->pos, _caminfo);
								}
							}
						}
						//発砲更新
						if (canShootGun) {
							if (this->m_Shoot.Press) {
								if (this->m_Shoot.Switch) {
									//射撃
									this->m_Shoot.Switch = false;
									//空の時はリロードを押す
									if (!this->haveGun->Shoot(_caminfo)) {
										this->m_Reload.Press = true;
									}
									else {
										PlaySound_Near3D(ENUM_SOUND::Fire0, this->pos, _caminfo);
									}
								}
								else {
									//次弾発射チェック
									this->m_Shoot.Switch = this->haveGun->canshoot();
								}
							}
							else {
								this->m_Shoot.Switch = true;
							}
							//リロード
							if (!this->m_Reload.On && this->m_Reload.Press) {
								if (this->m_Reload.Switch) {
									this->m_Reload.Switch = false;
									this->m_Reload.First();
									PlaySound_Near3D(ENUM_SOUND::MagDown0, this->pos, _caminfo);
								}
							}
							else {
								this->m_Reload.Switch = true;
							}
						}
					}
					else {
						if (!(
							this->m_anime.GetSel() == 0 ||
							this->m_anime.GetSel() == 1 ||
							this->m_anime.GetSel() == 2
							)
							) {
							this->m_Shoot.Press = false;
						}

						if (this->m_Shoot.Press) {
							if (!this->m_Melee.On) {
								this->m_Melee.First();
								this->MeleeSpeed = 5.f;
								PlaySound_Near3D(ENUM_SOUND::Punch, this->pos, _caminfo);
							}
						}
					}
				}
			}
			//描画
			void Draw(const Tiles& _Ti, const Camera_Info& _caminfo, int s_x, int s_y, Vector2D_I LIGHTPOS) noexcept {
				this->m_Footprint.Draw(_Ti, this->m_HumanData->m_HGraphs, _caminfo);
				if (!this->draw_end) {
					bool t = true;
					for (auto& g : this->sort) {
						auto& b = this->m_bone[g.first];
						if (LIGHTPOS == Vector2D_I::Zero()) {
							if (Draw_Common(_Ti, b.pos + Vector2D_I::Get(s_x, s_y), _caminfo, b.m_hight - this->sort.front().second, b.yrad + b.yr, this->m_HumanData->m_HGraphs[g.first])) {
								if (this->haveGun != nullptr && g.first == (size_t)Bone_Sel::HOLSTER) {
									this->haveGun->Draw_NotReady(_Ti, _caminfo, s_x, s_y);
								}
								this->draw_ok[g.first] = true;
							}
							else {
								t = false;
							}
						}
						else {
							Vector2D_I vec = LIGHTPOS - (this->pos + b.pos + Vector2D_I::Get(s_x, s_y));
							vec = vec * (10.f / vec.dist());
							int L_x = vec.x > 0 ? 2 : -2;
							int L_y = vec.y > 0 ? 2 : -2;

							SetIgnoreDrawGraphColor(TRUE);
							bool isdraw_ = Draw_Common(_Ti, b.pos + Vector2D_I::Get(s_x, s_y) + Vector2D_I::Get(L_x, L_y), _caminfo, b.m_hight - this->sort.front().second, b.yrad + b.yr, this->m_HumanData->m_HGraphs[g.first]);
							SetIgnoreDrawGraphColor(FALSE);
							if (isdraw_) {
								Draw_Common(_Ti, b.pos + Vector2D_I::Get(s_x, s_y), _caminfo, b.m_hight - this->sort.front().second, b.yrad + b.yr, this->m_HumanData->m_HGraphs[g.first]);
								if (this->haveGun != nullptr && g.first == (size_t)Bone_Sel::HOLSTER) {
									this->haveGun->Draw_NotReady(_Ti, _caminfo, s_x, s_y);
								}
								this->draw_ok[g.first] = true;
							}
							else {
								t = false;
							}
						}

					}
					if (t) {
						this->draw_end = true;
					}
				}
			}
			//影描画
			void Draw_Shadow_Point(const Tiles& _Ti, const Vector2D_I& _lightpos, int _high, float _ShadowRange, const Camera_Info& _caminfo, int s_x, int s_y) noexcept {
				for (auto& g : this->sort) {
					auto& b = this->m_bone[g.first];
					this->m_hight = b.m_hight - this->sort.front().second;
					Draw_Shadow_Point_Common(_Ti, _lightpos, b.pos + this->pos + Vector2D_I::Get(s_x, s_y), _high, _ShadowRange, _caminfo, b.yrad + b.yr, this->m_HumanData->m_HGraphs[g.first]);
				}
			}
			void Draw_Shadow(const Tiles& _Ti, float _light_yrad, float _ShadowRange, const Camera_Info& _caminfo, int s_x, int s_y) noexcept {
				for (auto& g : this->sort) {
					auto& b = this->m_bone[g.first];
					this->m_hight = b.m_hight - this->sort.front().second;
					Draw_Shadow_Common(_Ti, _light_yrad, b.pos + Vector2D_I::Get(s_x, s_y), _ShadowRange, _caminfo, b.yrad + b.yr, this->m_HumanData->m_HGraphs[g.first]);
				}
			}
		};
		class Gun_Graph {
			GraphHandle m_FirstUp;
			GraphHandle m_FirstUp_Empty;
			int m_Xsize{ 0 }, m_Ysize{ 0 };
		public:
			std::vector<GraphHandle> m_GGraphs;
			std::string Name;
			std::vector<GUN_SELECT> CanUseSelect;
			std::string AmmoType;
			float NextShotTime{ 0.f };
			std::string Info;
			int AmmoCntMax{ 10 };
		public:
			void Load(int _sel) noexcept {
				GraphHandle::LoadDiv("data/Gun/" + std::to_string(_sel) + "/Dot.bmp", 15, 5, 3, 96, 96, &this->m_GGraphs);
				this->m_FirstUp = GraphHandle::Load("data/Gun/" + std::to_string(_sel) + "/Up.bmp");
				this->m_FirstUp.GetSize(&m_Xsize, &m_Ysize);
				this->m_FirstUp_Empty = GraphHandle::Load("data/Gun/" + std::to_string(_sel) + "/Up_Empty.bmp");
				//data
				{
					const auto mdata = FileRead_open(("data/Gun/" + std::to_string(_sel) + "/Data.gdt").c_str(), FALSE);
					//name
					this->Name = getparams::_str(mdata);
					//type
					getparams::_str(mdata);
					//mode
					{
						auto SEL = getparams::_str(mdata);
						while (true) {
							if (SEL.find('/') == std::string::npos) {
								break;
							}
							auto buf = SEL.substr(0, SEL.find('/'));
							if (buf == "Semi") {
								this->CanUseSelect.emplace_back(GUN_SELECT::SEMI);	//mode
							}
							else if (buf == "Full") {
								this->CanUseSelect.emplace_back(GUN_SELECT::SEMI);
							}
							SEL = SEL.substr(SEL.find('/') + 1);
						}
					}
					//damage
					this->AmmoType = getparams::_str(mdata);
					//rpm
					this->NextShotTime = 60.f / getparams::_int(mdata);
					//round
					this->AmmoCntMax = getparams::_int(mdata);
					//motion
					getparams::_int(mdata);
					//info
					this->Info = getparams::_str(mdata) + "\n";
					this->Info += getparams::_str(mdata) + "\n";
					this->Info += getparams::_str(mdata) + "\n";
					FileRead_close(mdata);
				}
			}
			void DrawUp(bool inChamber, const int& posx, const int& posy, int _dispy, float rad, bool trns) noexcept {
				if (inChamber) {
					this->m_FirstUp.DrawRotaGraph(posx, posy, (float)_dispy / this->m_Ysize, rad, trns);

				}
				else {
					this->m_FirstUp_Empty.DrawRotaGraph(posx, posy, (float)_dispy / this->m_Ysize, rad, trns);
				}
			}
			void Dispose(void) noexcept {
				this->m_GGraphs.clear();
				this->Name = "";
				this->CanUseSelect.clear();
				this->AmmoType = "";
				this->NextShotTime = 0.f;
				this->AmmoCntMax = 0;
				this->Info = "";
			}
		};
		class Gun_Object : public Object_Common {
			class Ammo : public Object_Vanish {
			private:
				bool isHitWall{ false };
				bool isHitHuman{ false };
				Gun_Object* haveGun{ nullptr };
				Object_Common Hit;
				float Time_ShotFlash{ 100.f };
			public:
				const auto GetHitHuman(Human_Object& pl) const noexcept { return &pl != haveGun->haveHuman && (pl.pos - this->pos).hydist() < ((y_r(tilesize) / 4) * (y_r(tilesize) / 4)); }				//人との当たり判定
				const auto lightActive(void) const noexcept { return this->Time >= 5.f - 0.05f; }
				const auto isHit(void) const noexcept { return this->isHitWall || this->isHitHuman; }
				//ヒットエフェクト
				void Set_Hit(bool isWallHit) noexcept {
					if (isWallHit) {
						this->isHitWall = true;		//無機質
					}
					else {
						this->isHitHuman = true;	//人体
					}
				}
			public:
				void Init(Gun_Object* _haveGun) noexcept {
					if (_haveGun != nullptr) {
						haveGun = _haveGun;
						this->Init_Common(haveGun);

						this->isHitWall = false;
						this->isHitHuman = false;
						this->Time_ShotFlash = 0.f;
						this->Hit.Init_Common(this);

						this->Time = 5.f;
						this->m_speed = 35.f;
					}
				}
				void Update(std::vector<std::vector<Tiles>>& _tile, const Camera_Info& _caminfo) noexcept {
					auto buf = this->pos;
					if (!this->isHitWall && !this->isHitHuman) {
						this->UpdateSpeed(0.f);
					}
					this->UpdateTime();
					if (this->isHitWall) {
						if (this->Time_ShotFlash == 0.f) {
							PlaySound_Near3D(ENUM_SOUND::WallHit1, this->pos, _caminfo, 255);
						}
						this->Time_ShotFlash += 1.f / FPS;
						if (this->Time_ShotFlash > 0.2f) {
							this->Time = 0.f;
						}
					}
					if (this->isHitHuman) {
						this->Time = 0.f;
					}
					if (Get_HitWall(_tile, &this->pos, buf, (int)(this->m_speed / 2), HIT_SELECT::ONLY_HIT)) {
						this->Set_Hit(true);
					}
				}
				void Draw(const Tiles& _Ti, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
					if (!this->isHitWall && !this->isHitHuman) {
						Draw_Common(_Ti, Vector2D_I::Get(s_x, s_y), _caminfo, this->m_Yrad, this->haveGun->m_GunGraphPtr->m_GGraphs[0]);
					}
					else {
						if (this->isHitWall) {
							int Cnt = std::clamp(10 + (int)(this->Time_ShotFlash / 0.2f * 3.f), 10, 12);
							Draw_Common(_Ti, Vector2D_I::Get(s_x, s_y), _caminfo, this->m_Yrad + deg2rad(180), this->haveGun->m_GunGraphPtr->m_GGraphs[Cnt]);
						}
						//if (this->isHitHuman) {}
					}
				}
				void Draw_Light(const Tiles& _Ti, const Camera_Info& _caminfo) const noexcept {
					//閃光
					if (lightActive()) {
						this->Hit.Draw_Light_Common(_Ti, _caminfo, 100.f);
					}
				}
			};
			class Cart : public Object_Vanish {
			private:
				Gun_Object* haveGun{ nullptr };
				float Time_ShotFlash{ 100.f };
				float hight_f{ 0.f };
				float hight_add{ 0.f };
				float y_rad_G{ 0.f };
				bool isFirstDown{ false };
			public:
				void Set_Hit(void) noexcept {
					this->Time = 0.f;
				}
			public:
				void Init(Gun_Object* _haveGun) noexcept {
					if (_haveGun != nullptr) {
						haveGun = _haveGun;
						this->Init_Common(haveGun);

						this->m_Yrad += deg2rad(90 + GetRand(30));
						this->y_rad_G = this->m_Yrad;
						this->Time_ShotFlash = 0.f;
						this->hight_f = (float)this->m_hight;
						this->hight_add = 0.f;

						this->Time = 2.f;
						this->m_speed = 6.f;

						this->isFirstDown = true;
					}
				}
				void Update(std::vector<std::vector<Tiles>>& _tile, const Camera_Info& _caminfo) noexcept {
					auto buf = this->pos;
					this->UpdateSpeed(0.3f);
					this->UpdateTime();
					{
						this->hight_f -= this->hight_add * 60.f / FPS;
						if (this->hight_f <= 0.f) {
							this->hight_add = -0.98f;
							if (this->isFirstDown) {
								this->isFirstDown = false;
								PlaySound_Near3D(ENUM_SOUND::Case0, this->pos, _caminfo, 128);
							}
						}
						this->hight_add += 9.8f / FPS;
					}
					this->m_hight = (int)this->hight_f;
					this->m_Yrad += deg2rad(25) / FPS;
					this->y_rad_G += deg2rad(25 * 60) / FPS;
					if (Get_HitWall(_tile, &this->pos, buf, (int)(this->m_speed / 2), HIT_SELECT::ONLY_HIT)) {
						this->Set_Hit();
						if (this->isFirstDown) {
							this->isFirstDown = false;
							PlaySound_Near3D(ENUM_SOUND::Case0, this->pos, _caminfo);
						}
					}
				}
				void Draw(const Tiles& _Ti, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
					Draw_Common(_Ti, Vector2D_I::Get(s_x, s_y), _caminfo, this->y_rad_G, this->haveGun->m_GunGraphPtr->m_GGraphs[9]);
				}
				void Draw_Shadow_Point(const Tiles& _Ti, const Vector2D_I& _lightpos, int _high, float _ShadowRange, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
					Draw_Shadow_Point_Common(_Ti, _lightpos, this->pos + Vector2D_I::Get(s_x, s_y), _high, _ShadowRange, _caminfo, this->y_rad_G, this->haveGun->m_GunGraphPtr->m_GGraphs[9]);
				}
				void Draw_Shadow(const Tiles& _Ti, float _light_yrad, float _ShadowRange, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
					Draw_Shadow_Common(_Ti, _light_yrad, Vector2D_I::Get(s_x, s_y), _ShadowRange, _caminfo, this->y_rad_G, this->haveGun->m_GunGraphPtr->m_GGraphs[9]);
				}
			};
		private:
			Gun_Graph* m_GunGraphPtr{ nullptr };
			float m_xrand_b{ 0.f }, m_yrand_b{ 0.f }, m_rrand_b{ 0.f };
			float m_xrand{ 0.f }, m_yrand{ 0.f }, m_rrand{ 0.f };
			float m_ymove{ 0.f };
			float m_moveDowntime{ 0.f };
			bool m_moveStart{ false };
			bool m_moveUp{ false };
			Vector2D_I Recoilpos;
			float Recoil{ 0.f };
			float RecoilCnt{ 0.f };
			float ShotTime{ 0.f };
			float Time_ShotFlash{ 100.f };
			Object_Common Flash;
			int AmmoCnt{ 0 };
			bool inChamber{ true };
			GUN_SELECT Select{ GUN_SELECT::SEMI };
			int Sel_C{ 0 };
			float hight_f{ 0.f };
			float hight_add{ 0.f };
		public:
			Human_Object* haveHuman{ nullptr };
			std::vector<Ammo> ammo;
			std::vector<Cart> cart;
			bool isDraw{ true };
		public:
			const bool& CanLookGun(void) const noexcept { return this->m_moveStart; }
			const auto& GetMagGraph(void) const noexcept { return this->m_GunGraphPtr->m_GGraphs[14]; }
			const bool canshoot(void) const noexcept { return this->Select != GUN_SELECT::SEMI && ShotTime == 0.f; }
			const bool GetHaveHuman(void) const noexcept { return haveHuman != nullptr; }
			void SetHuman(Human_Object* _haveHuman) noexcept { haveHuman = _haveHuman; }
			void ReloadEnd(void) noexcept {
				AmmoCnt = this->m_GunGraphPtr->AmmoCntMax;
				inChamber = true;
			}
			bool Shoot(const Camera_Info& _caminfo) noexcept {
				if (AmmoCnt >= 0 && inChamber) {
					ammo.resize(ammo.size() + 1);
					ammo.back().Init(this);
					cart.resize(cart.size() + 1);
					cart.back().Init(this);
					Recoil = 10.f;
					RecoilCnt = 10.f;
					ShotTime = this->m_GunGraphPtr->NextShotTime;
					this->Time_ShotFlash = 0.f;
					this->Flash.Init_Common(this);
					if (AmmoCnt > 0) {
						AmmoCnt--;
					}
					else {
						inChamber = false;
					}
					PlaySound_Near3D(ENUM_SOUND::Trigger, this->pos, _caminfo, 64);
					return true;
				}
				return false;//空
			}
			void ChangeSel(void) noexcept {
				++this->Sel_C %= this->m_GunGraphPtr->CanUseSelect.size();
				this->Select = this->m_GunGraphPtr->CanUseSelect[this->Sel_C];
			}
			void LookGunStart(void) noexcept {
				if (!this->m_moveStart) {
					this->m_moveStart = true;
					this->m_moveUp = true;
					this->m_ymove = 1.f;
					this->m_xrand_b = 0.f;
					this->m_yrand_b = 0.f;
					this->m_rrand_b = 0.f;
					this->m_xrand = 0.f;
					this->m_yrand = 0.f;
					this->m_rrand = 0.f;
					this->m_moveDowntime = 0.f;
					SE.Get((int)ENUM_SOUND::Look).Play(0, DX_PLAYTYPE_BACK, TRUE);
				}
			}
			void Draw_Up(int _dispx, int _dispy) noexcept {
				if (this->m_moveStart) {
					this->m_GunGraphPtr->DrawUp(this->m_moveStart, _dispx / 2 + (int)this->m_xrand, _dispy / 2 + (int)this->m_yrand + (int)(this->m_ymove * _dispy), _dispy, deg2rad(-30) * this->m_ymove + deg2rad(this->m_rrand), true);

					int XP = _dispx * 32 / 1920;
					int YP = _dispy * 960 / 1080;

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(255 - (int)(255.f * this->m_ymove), 0, 255));
					GetFont(y_r(40)).DrawString(XP, YP, this->m_GunGraphPtr->Name, GetColor(216, 216, 216), GetColor(64, 64, 64)); YP += y_r(40);
					if (this->inChamber) {
						float rad = deg2rad(90 - 90 * AmmoCnt / this->m_GunGraphPtr->AmmoCntMax);
						int col_r = GetColor(std::clamp(int(255.f * sin(rad) * 2.f), 0, 255), std::clamp(int(255.f * cos(rad) * 2.f), 0, 255), 0);
						GetFont(y_r(25)).DrawStringFormat(XP + y_r(15), YP, col_r, "%2d%s", this->AmmoCnt, this->inChamber ? "+1" : ""); YP += y_r(25);
					}
					else {
						GetFont(y_r(25)).DrawString(XP + y_r(15), YP, "EMPTY", GetColor(216, 0, 0)); YP += y_r(25);
					}
					YP += y_r(5);
					switch (this->Select) {
					case GUN_SELECT::SEMI:
						GetFont(y_r(15)).DrawString(XP + y_r(35), YP, "semi", GetColor(174, 174, 174)); YP += y_r(15);
						break;
					case GUN_SELECT::FULL:
						GetFont(y_r(15)).DrawString(XP + y_r(35), YP, "full", GetColor(174, 174, 174)); YP += y_r(15);
						break;
					}

					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

					easing_set(&this->m_xrand_b, GetRandf((float)(_dispx / 20)), 0.995f);
					easing_set(&this->m_yrand_b, (float)(_dispy / 20) + GetRandf((float)(_dispy / 20)), 0.995f);
					easing_set(&this->m_rrand_b, GetRandf(30), 0.975f);
					easing_set(&this->m_xrand, this->m_xrand_b, 0.975f);
					easing_set(&this->m_yrand, this->m_yrand_b, 0.975f);
					easing_set(&this->m_rrand, this->m_rrand_b, 0.975f);

					if (this->m_ymove <= 0.01f) {
						this->m_moveUp = false;
					}
					if (this->m_moveUp) {
						easing_set(&this->m_ymove, 0.f, 0.925f);
					}
					else {
						if (this->m_moveDowntime > 1.f) {
							if (this->m_ymove >= 0.99f) {
								this->m_moveStart = false;
							}
							if (this->m_ymove <= 0.01f) {
								SE.Get((int)ENUM_SOUND::Look).Play(0, DX_PLAYTYPE_BACK, TRUE);
							}
							easing_set(&this->m_ymove, 1.f, 0.9f);
						}
						else {
							this->m_moveDowntime += 1.f / FPS;
						}
					}
				}
			}
			void Put(const Vector2D_I& _pos, int _hight) noexcept {
				this->pos = _pos;
				this->m_Base_Hight = _hight + 2;
			}
			void Draw_Drop(const Tiles& _Ti, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
				if (!GetHaveHuman()) {
					Draw_Common(_Ti, Vector2D_I::Get(s_x, s_y), _caminfo, this->m_Yrad, this->m_GunGraphPtr->m_GGraphs[6]);
				}
			}
			const auto lightActive(void) const noexcept {
				for (auto& am : this->ammo) { if (am.lightActive()) { return true; } }
				return false;
			}
		public:
			void Init(Gun_Graph* _GunGraphPtr) noexcept {
				m_GunGraphPtr = _GunGraphPtr;
				this->m_moveStart = false;
				ReloadEnd();
			}
			void Dispose(void) noexcept {
				this->m_GunGraphPtr = nullptr;
				this->ammo.clear();
				this->cart.clear();
			}
			void Update(const Camera_Info& _caminfo) noexcept {
				if (GetHaveHuman()) {
					if (inChamber) {
						if (this->Recoil == 10.f) {
							PlaySound_Near3D(ENUM_SOUND::Slide0, this->pos, _caminfo, 64);
						}
						this->Recoil = std::max(this->Recoil - 1.5f * 60.f / FPS, 0.f);
					}
					this->RecoilCnt = std::max(this->RecoilCnt - 1.5f * 60.f / FPS, 0.f);
					this->ShotTime = std::max(this->ShotTime - 1.f / FPS, 0.f);
					this->Recoilpos = Vector2D_I::Get(y_r(sin(this->m_Yrad) * this->Recoil), y_r(-cos(this->m_Yrad) * this->Recoil)) * -1.f;
					if (this->isDraw) {
						this->pos = haveHuman->pos + haveHuman->GetRightHandInfo().pos;
						this->m_hight = haveHuman->GetRightHandInfo().m_hight - haveHuman->GetBaseHight() + 1;
						this->m_Yrad = haveHuman->GetRightHandInfo().yrad + haveHuman->GetRightHandInfo().yr;
					}
					else {
						this->pos = haveHuman->pos + haveHuman->GetHOLSTERInfo().pos;
						this->m_hight = haveHuman->GetHOLSTERInfo().m_hight - haveHuman->GetBaseHight() + 1;
						this->m_Yrad = haveHuman->GetHOLSTERInfo().yrad + haveHuman->GetHOLSTERInfo().yr;
					}
				}
				else {
					//this->pos = ;
					this->m_Yrad += deg2rad(90) / FPS;
					{
						auto buf = this->pos;
						this->UpdateSpeed(0.1f);
						this->hight_f -= this->hight_add * 60.f / FPS;
						if (this->hight_f <= 0.f) {
							this->hight_add = 0.f;
							this->hight_f = 0.f;
						}
						this->hight_add += 9.8f / FPS;
						this->m_hight = (int)this->hight_f;
					}
				}
				for (int i = 0; i < ammo.size(); i++) {
					auto& am = ammo[i];
					if (am.isEnd()) {
						am = ammo.back();
						ammo.pop_back();
						i--;
					}
				}
				for (int i = 0; i < cart.size(); i++) {
					auto& am = cart[i];
					if (am.isEnd()) {
						am = cart.back();
						cart.pop_back();
						i--;
					}
				}
				this->Time_ShotFlash += 1.f / FPS;
			}
			void DrawAmmos(const Tiles& _Ti, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
				for (auto& am : ammo) {
					am.Draw(_Ti, _caminfo, s_x, s_y);
				}
				for (auto& am : cart) {
					am.Draw(_Ti, _caminfo, s_x, s_y);
				}
			}
			void Draw_NotReady(const Tiles& _Ti, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
				if (GetHaveHuman()) {
					if (!this->isDraw) {
						Draw_Common(_Ti, Vector2D_I::Get(s_x, s_y), _caminfo, this->m_Yrad, this->m_GunGraphPtr->m_GGraphs[13]);
					}
				}
			}
			void Draw(const Tiles& _Ti, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
				if (GetHaveHuman()) {
					if (this->isDraw) {
						Draw_Common(_Ti, Vector2D_I::Get(s_x, s_y), _caminfo, this->m_Yrad, this->m_GunGraphPtr->m_GGraphs[7]);
						Draw_Common(_Ti, Vector2D_I::Get(s_x, s_y) + this->Recoilpos, _caminfo, this->m_Yrad, this->m_GunGraphPtr->m_GGraphs[5]);
					}
					//マズルフラッシュ
					{
						if (this->Time_ShotFlash <= 0.2f) {
							int Cnt = std::clamp(1 + (int)(this->Time_ShotFlash / 0.2f * 4.f), 1, 4);
							this->Flash.Draw_Common(_Ti, Vector2D_I::Get(s_x, s_y), _caminfo, this->m_Yrad, this->m_GunGraphPtr->m_GGraphs[Cnt]);
						}
					}
				}
			}
			void Draw_Shadow_Point(const Tiles& _Ti, const Vector2D_I& _lightpos, int _high, float _ShadowRange, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
				if (this->isDraw) {
					Draw_Shadow_Point_Common(_Ti, _lightpos, this->pos + Vector2D_I::Get(s_x, s_y), _high, _ShadowRange, _caminfo, this->m_Yrad, this->m_GunGraphPtr->m_GGraphs[7]);
					Draw_Shadow_Point_Common(_Ti, _lightpos, this->pos + Vector2D_I::Get(s_x, s_y) + this->Recoilpos, _high, _ShadowRange, _caminfo, this->m_Yrad, this->m_GunGraphPtr->m_GGraphs[5]);
				}
				for (auto& am : this->cart) { am.Draw_Shadow_Point(_Ti, _lightpos, _high, _ShadowRange, _caminfo, s_x, s_y); }
			}
			void Draw_Shadow(const Tiles& _Ti, float _light_yrad, float _ShadowRange, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
				if (this->isDraw) {
					Draw_Shadow_Common(_Ti, _light_yrad, Vector2D_I::Get(s_x, s_y), _ShadowRange, _caminfo, this->m_Yrad, this->m_GunGraphPtr->m_GGraphs[7]);
					Draw_Shadow_Common(_Ti, _light_yrad, Vector2D_I::Get(s_x, s_y) + this->Recoilpos, _ShadowRange, _caminfo, this->m_Yrad, this->m_GunGraphPtr->m_GGraphs[5]);
				}
				for (auto& am : this->cart) { am.Draw_Shadow(_Ti, _light_yrad, _ShadowRange, _caminfo, s_x, s_y); }
			}
			void Draw_Light(const Tiles& _Ti, const Camera_Info& _caminfo) const noexcept {
				for (auto& am : this->ammo) { am.Draw_Light(_Ti, _caminfo); }
			}
		};
		class MapInfo {
			class Magazine_Object : public Object_Vanish {
			private:
				Gun_Object* haveGun{ nullptr };
				float Time_ShotFlash{ 100.f };
				float hight_f{ 0.f };
				float hight_add{ 0.f };
			public:
				void Set_Hit(void) noexcept { this->Time = 0.f; }
			public:
				void Init(Gun_Object* _haveGun) noexcept {
					if (_haveGun != nullptr) {
						haveGun = _haveGun;
						this->Init_Common(haveGun);

						this->m_Yrad += deg2rad(GetRand(360));
						this->Time_ShotFlash = 0.f;
						this->hight_f = (float)this->m_hight;
						this->hight_add = 0.f;

						this->Time = 2.f;
						this->m_speed = 1.f;
					}
				}
				void Update(std::vector<std::vector<Tiles>>& _tile) noexcept {
					auto buf = this->pos;
					this->UpdateSpeed(0.1f);
					this->UpdateTime();
					this->hight_f -= this->hight_add * 60.f / FPS;
					if (this->hight_f <= 0.f) {
						this->hight_add = 0.f;
						this->hight_f = 0.f;
					}
					this->hight_add += 9.8f / FPS;
					this->m_hight = (int)this->hight_f;
					if (Get_HitWall(_tile, &this->pos, buf, (int)(this->m_speed / 2), HIT_SELECT::ONLY_HIT)) {
						this->Set_Hit();
					}
				}
				void Draw(const Tiles& _Ti, const Camera_Info& _caminfo) const noexcept {
					Draw_Common(_Ti, Vector2D_I::Zero(), _caminfo, this->m_Yrad, this->haveGun->GetMagGraph());
				}
				void Draw_Shadow_Point(const Tiles& _Ti, const Vector2D_I& _lightpos, int _high, float _ShadowRange, const Camera_Info& _caminfo, int s_x, int s_y) const noexcept {
					Draw_Shadow_Point_Common(_Ti, _lightpos, this->pos + Vector2D_I::Get(s_x, s_y), _high, _ShadowRange, _caminfo, this->m_Yrad, this->haveGun->GetMagGraph());
				}
				void Draw_Shadow(const Tiles& _Ti, float _light_yrad, float _ShadowRange, const Camera_Info& _caminfo) const noexcept {
					Draw_Shadow_Common(_Ti, _light_yrad, Vector2D_I::Zero(), _ShadowRange, _caminfo, this->m_Yrad, this->haveGun->GetMagGraph());
				}
			};
			class Effect2D {
				class Effect_Object : public Object_Vanish {
					Vector2D_I m_ofSet;
				public:
					//ヒットエフェクト
					void Set_Hit(Object_Common* _have, Object_Common* _effectpos = nullptr) noexcept {
						if (_have != nullptr) {
							this->Init_Common(_have);
							this->m_Yrad -= deg2rad(90);
							m_ofSet.set((int)((float)(48 + 12) * -cos(this->m_Yrad)), (int)((float)(48 + 12) * -sin(this->m_Yrad)));
							if (_effectpos != nullptr) {
								this->SetPos(_effectpos);
							}
							this->Time = 0.4f;
						}
					}
				public:
					void Draw(const Tiles& _Ti, const Camera_Info& _caminfo, const std::vector<GraphHandle>& _Graphs) const noexcept {
						if (this->iaActive()) {
							int Cnt = std::clamp(0 + (int)((0.4f - this->Time) / 0.4f * 10.f), 0, 10);
							Draw_Common(_Ti, m_ofSet, _caminfo, this->m_Yrad, _Graphs[Cnt]);
						}
					}
				};
			private:
				std::vector<GraphHandle> m_EGraphs;
				std::vector<Effect_Object> m_Effects;
			public:
				void Set_Hit(Object_Common* _have, Object_Common* _effectpos = nullptr) noexcept {
					if (_have != nullptr) {
						m_Effects.resize(m_Effects.size() + 1);
						m_Effects.back().Set_Hit(_have, _effectpos);
					}
				}
			public:
				void Init(std::string_view FileName) noexcept {
					GraphHandle::LoadDiv(FileName, 25, 5, 5, 48, 48, &this->m_EGraphs);
					m_Effects.clear();
				}
				void Update(void) noexcept {
					for (int i = 0; i < m_Effects.size(); i++) {
						auto& ef = m_Effects[i];
						ef.UpdateTime();
						if (ef.isEnd()) {
							ef = m_Effects.back();
							m_Effects.pop_back();
							i--;
						}
					}
				}
				void Draw(const Tiles& _Ti, const Camera_Info& _caminfo) const noexcept {
					for (auto& ef : m_Effects) {
						ef.Draw(_Ti, _caminfo, m_EGraphs);
					}
				}
			};
			class WorldPhaseControl {
				float m_DownTimer{ 0.f };
				float m_DownTimerMax{ 10.f };
				int m_Phase{ 0 };
				int m_Phase_Lowest{ 0 };
				int m_maximamPhase{ 0 };
			public:
				const auto& GetPhase(void) const noexcept { return m_Phase; }
				const auto& Get_LowestPhase(void) const noexcept { return m_Phase_Lowest; }
				void Find(void) noexcept {
					this->m_DownTimer = this->m_DownTimerMax;
					this->m_Phase = std::min(this->m_Phase + 1, 4);
					this->m_Phase_Lowest = 1;
				}
				void Update(int* _maximamPhase, bool _FoundEnemyAny) noexcept {
					*_maximamPhase = std::max(*_maximamPhase, this->GetPhase());
					*_maximamPhase = std::min(*_maximamPhase, 3);

					if (this->m_maximamPhase != *_maximamPhase) {
						switch (*_maximamPhase) {
						case 1:
							BGM.Get((int)ENUM_BGM::Phase1).Play(0, DX_PLAYTYPE_LOOP, TRUE);
							BGM.Get((int)ENUM_BGM::Phase1).SetVol_Local((int)(128.f * 1.f));
							break;
						case 2:
							BGM.Get((int)ENUM_BGM::Phase2).Play(0, DX_PLAYTYPE_LOOP, TRUE);
							BGM.Get((int)ENUM_BGM::Phase2).SetVol_Local((int)(128.f * 1.f));
							break;
						case 3:
							BGM.Get((int)ENUM_BGM::Phase3).Play(0, DX_PLAYTYPE_LOOP, TRUE);
							BGM.Get((int)ENUM_BGM::Phase3).SetVol_Local((int)(128.f * 1.f));
							break;
						default:
							break;
						}
						switch (this->m_maximamPhase) {
						case 1:
							BGM.Get((int)ENUM_BGM::Phase1).SetVol_Local((int)(128.f * 0.f));
							break;
						case 2:
							BGM.Get((int)ENUM_BGM::Phase2).SetVol_Local((int)(128.f * 0.f));
							break;
						case 3:
							BGM.Get((int)ENUM_BGM::Phase3).SetVol_Local((int)(128.f * 0.f));
							break;
						default:
							break;
						}
						this->m_maximamPhase = *_maximamPhase;
					}
					if (this->m_Phase > this->m_Phase_Lowest && !_FoundEnemyAny) {
						this->m_DownTimer -= 1.f / FPS;
						if (this->m_DownTimer < 0.f) {
							this->m_DownTimer = this->m_DownTimerMax;
							this->m_Phase = std::max(this->m_Phase - 1, this->m_Phase_Lowest);
						}
					}
				}
				void Draw_UI(void) const noexcept {
					int xp = y_r(12), yp = y_r(212);
					int xs = y_r(200), ys = y_r(40);
					DrawBox(xp, yp, xp + xs, yp + ys, GetColor(64, 64, 64), TRUE);
					DrawBox(xp, yp, xp + xs * (int)this->m_DownTimer / (int)this->m_DownTimerMax, yp + ys, GetColor(192, 0, 0), TRUE);
					DrawBox(xp, yp, xp + xs, yp + ys, GetColor(128, 128, 128), FALSE);

					yp += y_r(50);
					GetFont(y_r(40)).DrawStringFormat(xp, yp, GetColor(255, 64, 64), "Phase : %d", this->m_Phase);
					yp += y_r(40);
					GetFont(y_r(15)).DrawStringFormat(xp + y_r(24), yp, GetColor(255, 64, 64), "Minimam : %d", this->m_Phase_Lowest);
				}
			};
			class HG_Buf {
			public:
				Vector2D_I m_ID;
				int m_OffSet = 0;
				int m_Size = 0;
			};
		private:
			float m_Zoom_buf{ 0.6f };
			const size_t m_player_id{ 0 };					//playerは0に固定
			std::vector<HG_Buf> m_Buffer;
			AnimeData m_HumanAnimeData;
			float m_DirectionalLight_Rad{ 0.f };
			float m_ShadowRange{ 6.f };
		public:
			std::vector<Human_Graph> m_humangraph;
			std::vector<Gun_Graph> m_gungraph;
			std::vector<Human_Object> m_human;
			std::vector<Gun_Object> m_gun;
			std::vector<Magazine_Object> m_mag;
			Camera_Info m_caminfo;
			WorldPhaseControl m_WorldPhase;
			std::vector<Effect2D> m_effect;
		public:
			const auto& GetHumanAnimeData(void) const noexcept { return this->m_HumanAnimeData.anime; }
			auto& SetDirectionalLight_Rad(void) noexcept { return this->m_DirectionalLight_Rad; }
			const auto& GetDirectionalLight_Rad(void) const noexcept { return this->m_DirectionalLight_Rad; }
			const auto& GetShadowRange(void) const noexcept { return this->m_ShadowRange; }
			auto& SetPlayer(void) noexcept { return this->m_human[this->m_player_id]; }
			const auto& GetPlayer(void) const noexcept { return this->m_human[this->m_player_id]; }
			void AddMag(Gun_Object* _haveGun) noexcept {
				this->m_mag.resize(this->m_mag.size() + 1);
				this->m_mag.back().Init(_haveGun);
			}
			void Set_Human_Gun_Buffer(std::string _mapname, int x, int y) noexcept {
				m_Buffer.resize(m_Buffer.size() + 1);
				m_Buffer.back().m_ID.set(x, y);
				m_Buffer.back().m_OffSet = (int)this->m_human.size();
				//mapデータ3読み込み(人、銃情報)
				auto info3 = GetMapInfo3(_mapname);
				m_Buffer.back().m_Size = 0;
				for (const auto& m : info3) {
					if (m.pos_p != Vector2D_I::Zero()) {
						this->m_human.resize(this->m_human.size() + 1);
						this->m_gun.resize(this->m_gun.size() + 1);
						m_Buffer.back().m_Size++;
					}
				}
			}
			const auto GetOffSet(const Vector2D_I& _STAGE_ID) const noexcept {
				for (int i = 0; i < m_Buffer.size(); i++) {
					if (m_Buffer[i].m_ID == _STAGE_ID) {
						return m_Buffer[i].m_OffSet;
					}
				}
				return -1;
			}
			const auto GetBufSize(const Vector2D_I& _STAGE_ID) const noexcept {
				for (int i = 0; i < m_Buffer.size(); i++) {
					if (m_Buffer[i].m_ID == _STAGE_ID) {
						return m_Buffer[i].m_Size;
					}
				}
				return -1;
			}
		public:
			void Init(void) noexcept {
				//エフェクト読み込み
				this->m_effect.resize(this->m_effect.size() + 1);
				this->m_effect.back().Init("data/Effect/1.bmp");
				//人間写真読み込み
				for (int i = 0; i < 2; i++) {
					this->m_humangraph.resize(this->m_humangraph.size() + 1);
					this->m_humangraph.back().Load(1 + i);
				}
				//
				for (int i = 0; i < 1; i++) {
					this->m_gungraph.resize(this->m_gungraph.size() + 1);
					this->m_gungraph.back().Load(1 + i);
				}
				//
				for (int i = 0; i < (int)Anim_Sel::NUM; i++) {
					this->m_HumanAnimeData.LoadAnime("data/Char/Mot/" + std::to_string(i) + ".mot");
				}
			}
			void Dispose(void) noexcept {
				this->m_gun.clear();
				this->m_humangraph.clear();
				this->m_gungraph.clear();
				this->m_HumanAnimeData.Dispose();
			}
			void Update_DirectionalLight(void) noexcept {
				this->m_DirectionalLight_Rad += deg2rad(0.1f) / FPS;
				if (this->m_DirectionalLight_Rad > deg2rad(180)) { this->m_DirectionalLight_Rad = deg2rad(-180); }
				if (this->m_DirectionalLight_Rad >= deg2rad(-90) && this->m_DirectionalLight_Rad <= deg2rad(90)) {
					this->m_ShadowRange = std::clamp(1.f + 9.f * abs(sin(this->m_DirectionalLight_Rad)), 3.f, 10.f);
				}
				else {
					this->m_ShadowRange = 10.f;
				}
			}
			void Update_Cam(const Vector2D_I& cam_pos) noexcept {
				easing_set(&this->m_Zoom_buf, SetPlayer().GetCamZoom(), 0.95f);
				this->m_caminfo.camerapos = cam_pos;
				this->m_caminfo.camzoom = std::clamp(this->m_Zoom_buf, 0.6f, 2.0f);

				//this->m_caminfo.camzoom = 0.2f;
			}
		};
		class MapDraw {
			class shadow_handle {
				bool m_isupdate{ true };
				GraphHandle m_Handle;
			public:
				const auto& GetHandle(void) noexcept {
					this->m_isupdate = true;
					return this->m_Handle;
				}
				void SetDraw(std::function<void()> _doing) noexcept {
					if (this->m_isupdate) {
						this->m_isupdate = false;
						this->m_Handle.SetDraw_Screen();
						{
							_doing();
						}
					}
				}
				void Init(int _x, int _y) noexcept {
					this->m_isupdate = true;
					this->m_Handle = GraphHandle::Make(_x, _y, true);
				}
			};
		private:
			std::vector<std::vector<Tiles>> m_Tile;
			int map_xsize = 0, map_ysize = 0;
			std::vector<Human_Object*> m_humanPtr;		//実際に演算する人間
			std::vector<Gun_Object*> m_gunPtr;
			std::shared_ptr<DXDraw> m_DrawPts{ nullptr };			//引き継ぐ
			MapInfo* m_MapInfoPtr{ nullptr };
			std::vector<Near3DEditer::TileStatus> m_TileData;
			std::string wall_name = "";
			std::string floor_name = "";
			std::vector<GraphHandle> m_walls;
			std::vector<GraphHandle> m_floors;
			std::vector<Vector2D_I> m_way_point;
			GraphHandle m_screen;
			std::array<shadow_handle, 8> m_shadow_graph;
			GraphHandle m_res_floor;
			int stage_x = 0, stage_y = 0;
		private:
			//タイル
			const auto& GetTile(const Vector2D_I& _pos) const noexcept { return m_Tile[std::clamp(_pos.x / y_r(tilesize), 0, map_xsize - 1)][std::clamp(_pos.y / y_r(tilesize), 0, map_ysize - 1)]; }
			//床面に影をブレンド
			auto& GetFloor_BlendShadow(const Vector2D_I& _p1, const Vector2D_I& _p2, int _hight, GraphHandle* _handle) noexcept {
				const int g = DerivationGraph(
					std::max(0, _p1.x), std::max(0, _p1.y), std::min(m_DrawPts->disp_x, _p2.x - _p1.x), std::min(m_DrawPts->disp_y, _p2.y - _p1.y),
					m_shadow_graph[std::clamp<size_t>(_hight / 8, 0, m_shadow_graph.size() - 1)].GetHandle().get());
				GraphBlendBlt(_handle->get(), g, m_res_floor.get(), 128, DX_GRAPH_BLEND_NORMAL);
				DeleteGraph(g);
				return m_res_floor;
			}
			//壁描画
			void Draw_Wall(int _UorL, const Tiles& _Ti) noexcept {
				if (_UorL < 20 && _Ti.m_hight != _Ti.m_bottom) {
					{
						float rad = abs(cos(atan2f(float(_Ti.m_hight - _Ti.m_bottom), (float)y_r(tilesize) / m_MapInfoPtr->m_caminfo.camzoom)));
						Set_Bright((int)(rad * (0.75f + cos(m_MapInfoPtr->GetDirectionalLight_Rad() + deg2rad((4 - _UorL % 4) * 90)) * 0.25f) * 255.f));
					}
					switch (_UorL) {
					case 0://縦(上)
						if (_Ti.m_floor[0].y < _Ti.m_top[0].y)
							DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[1], _Ti.m_floor[1], _Ti.m_floor[0], _Ti.m_wallhandle);
						break;
					case 4://上◢
						if (_Ti.m_floor[1].y < _Ti.m_top[1].y)//ここだけ変
							DrawModi_wrap(_Ti.m_top[1], _Ti.m_top[1], _Ti.m_floor[1], _Ti.m_floor[0], _Ti.m_wallhandle);
						break;
					case 8://上◣
						if (_Ti.m_floor[0].y < _Ti.m_top[0].y)
							DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[0], _Ti.m_floor[1], _Ti.m_floor[0], _Ti.m_wallhandle);
						break;
					case 12://縦(上)
						if (_Ti.m_floor[0].y < _Ti.m_top[2].y)
							DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[3], _Ti.m_floor[1], _Ti.m_floor[0], _Ti.m_wallhandle);
						break;
					case 16://縦(上)
						if (_Ti.m_floor[0].y < _Ti.m_top[2].y)
							DrawModi_wrap(_Ti.m_floor[0], _Ti.m_floor[1], _Ti.m_top[3], _Ti.m_top[2], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.m_floorhandle));
						break;
					case 1://横(左)
						if (_Ti.m_floor[0].x < _Ti.m_top[0].x)
							DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[0], _Ti.m_floor[0], _Ti.m_floor[2], _Ti.m_wallhandle);
						break;
					case 5://左◢
						if (_Ti.m_floor[0].x < _Ti.m_top[0].x)
							DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[2], _Ti.m_floor[0], _Ti.m_floor[2], _Ti.m_wallhandle);
						break;
					case 9://左◥
						if (_Ti.m_floor[0].x < _Ti.m_top[0].x)
							DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[0], _Ti.m_floor[0], _Ti.m_floor[2], _Ti.m_wallhandle);
						break;
					case 13://横(左)
						if (_Ti.m_floor[0].x < _Ti.m_top[3].x)
							DrawModi_wrap(_Ti.m_top[3], _Ti.m_top[1], _Ti.m_floor[0], _Ti.m_floor[2], _Ti.m_wallhandle);
						break;
					case 17://横(左)
						if (_Ti.m_floor[0].x < _Ti.m_top[3].x)
							DrawModi_wrap(_Ti.m_floor[0], _Ti.m_top[1], _Ti.m_top[3], _Ti.m_floor[2], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.m_floorhandle));
						break;
					case 2://縦(下)
						if (_Ti.m_floor[3].y >= _Ti.m_top[3].y)
							DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[3], _Ti.m_floor[3], _Ti.m_floor[2], _Ti.m_wallhandle);
						break;
					case 6://下◢
						if (_Ti.m_floor[3].y >= _Ti.m_top[3].y)
							DrawModi_wrap(_Ti.m_top[3], _Ti.m_top[3], _Ti.m_floor[3], _Ti.m_floor[2], _Ti.m_wallhandle);
						break;
					case 10://下◣
						if (_Ti.m_floor[3].y >= _Ti.m_top[3].y)
							DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[2], _Ti.m_floor[3], _Ti.m_floor[2], _Ti.m_wallhandle);
						break;
					case 14://縦(下)
						if (_Ti.m_floor[2].y > _Ti.m_top[1].y)
							DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[1], _Ti.m_floor[3], _Ti.m_floor[2], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.m_floorhandle));
						break;
					case 18://縦(下)
						if (_Ti.m_floor[2].y > _Ti.m_top[1].y)
							DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[1], _Ti.m_floor[3], _Ti.m_floor[2], _Ti.m_wallhandle);
						break;
					case 3://横(右)
						if (_Ti.m_floor[3].x >= _Ti.m_top[3].x)
							DrawModi_wrap(_Ti.m_top[3], _Ti.m_top[1], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.m_wallhandle);
						break;
					case 7://右◢
						if (_Ti.m_floor[3].x >= _Ti.m_top[3].x)
							DrawModi_wrap(_Ti.m_top[3], _Ti.m_top[3], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.m_wallhandle);
						break;
					case 11://右◥
						if (_Ti.m_floor[3].x >= _Ti.m_top[3].x)
							DrawModi_wrap(_Ti.m_top[1], _Ti.m_top[1], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.m_wallhandle);
						break;
					case 15://横(右)
						if (_Ti.m_floor[1].x > _Ti.m_top[2].x)
							DrawModi_wrap(_Ti.m_top[0], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.m_top[2], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.m_floorhandle));
						break;
					case 19://横(右)
						if (_Ti.m_floor[1].x > _Ti.m_top[2].x)
							DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[0], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.m_wallhandle);
						break;
					}
				}
				else {
					{
						auto cam_high = (int)((float)m_MapInfoPtr->m_caminfo.camhigh_base / m_MapInfoPtr->m_caminfo.camzoom);
						Set_Bright(255 - 255 * std::clamp(_Ti.m_hight, 0, cam_high) / cam_high);
					}
					if (!_Ti.m_isWall) {
						DrawExtend_wrap(_Ti.m_top[0], _Ti.m_top[3], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.m_floorhandle));
					}
					else {
						DrawExtend_wrap(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_floorhandle);
					}
				}
			}
			//柱描画
			void Draw_Tile(const Tiles& _Ti, int s_x, int s_y, Vector2D_I LIGHTPOS) noexcept {
				switch (_Ti.m_dir) {//三角柱
				case 0://上
					Draw_Wall(12, _Ti);	//縦(上)//12
					Draw_Wall(5, _Ti);	//横(左)
					Draw_Wall(2, _Ti);	//縦(下)
					Draw_Wall(7, _Ti);	//横(右)
					break;
				case 1://左
					Draw_Wall(4, _Ti);	//縦(上)//4
					Draw_Wall(13, _Ti);	//横(左)
					Draw_Wall(6, _Ti);	//縦(下)
					Draw_Wall(3, _Ti);	//横(右)
					break;
				case 2://下
					Draw_Wall(0, _Ti);	//縦(上)//12
					Draw_Wall(9, _Ti);	//横(左)
					Draw_Wall(18, _Ti);	//縦(下)
					Draw_Wall(11, _Ti);	//横(右)
					break;
				case 3://右
					Draw_Wall(8, _Ti);	//縦(上)//8
					Draw_Wall(1, _Ti);	//横(左)
					Draw_Wall(10, _Ti);	//縦(下)
					Draw_Wall(19, _Ti);	//横(右)
					break;
				case 4://上
					Draw_Wall(16, _Ti);	//縦(上)//12
					break;
				case 5://左
					Draw_Wall(17, _Ti);	//横(左)
					break;
				case 6://下
					Draw_Wall(14, _Ti);	//縦(下)
					break;
				case 7://右
					Draw_Wall(15, _Ti);	//横(右)
					break;
				default://柱
					Draw_Wall(0, _Ti);	//縦(上)
					Draw_Wall(1, _Ti);	//横(左)
					Draw_Wall(2, _Ti);	//縦(下)
					Draw_Wall(3, _Ti);	//横(右)
					Draw_Wall(20, _Ti);	//天井
					break;
				}
				for (auto& gn : this->m_gunPtr) { gn->DrawAmmos(_Ti, m_MapInfoPtr->m_caminfo, s_x, s_y); }
				for (auto& mg : this->m_MapInfoPtr->m_mag) { mg.Draw(_Ti, m_MapInfoPtr->m_caminfo); }
				for (auto& gn : this->m_gunPtr) { gn->Draw_Drop(_Ti, m_MapInfoPtr->m_caminfo, s_x, s_y); }
				for (auto& pl : this->m_humanPtr) { pl->Draw(_Ti, m_MapInfoPtr->m_caminfo, s_x, s_y, LIGHTPOS); }
				for (auto& gn : this->m_gunPtr) { gn->Draw(_Ti, m_MapInfoPtr->m_caminfo, s_x, s_y); }
				for (auto& ef : this->m_MapInfoPtr->m_effect) { ef.Draw(_Ti, m_MapInfoPtr->m_caminfo); }
				Set_Bright(255);
			}
			//y軸描画
			void DrawCommon_Y(std::vector<Tiles>& T_X, int s_x, int s_y, Vector2D_I LIGHTPOS) noexcept {
				//画面位置取得
				const Vector2D_I limmin = ConvertPos(Vector2D_I::Get(-m_DrawPts->disp_x / 2, -m_DrawPts->disp_y / 2), 0, m_MapInfoPtr->m_caminfo);
				const Vector2D_I cam = ConvertPos(Vector2D_I::Zero(), 0, m_MapInfoPtr->m_caminfo);
				const Vector2D_I limmax = ConvertPos(Vector2D_I::Get(m_DrawPts->disp_x / 2, m_DrawPts->disp_y / 2), 0, m_MapInfoPtr->m_caminfo);

				for (auto& pl : this->m_humanPtr) { pl->Reset(); }
				for (auto& ti : T_X) {
					if (!(cam.y >= ti.m_top[0].y && ti.m_zero[3].y >= limmin.y)) { continue; }
					Draw_Tile(ti, s_x, s_y, LIGHTPOS);
				}
				for (auto& pl : this->m_humanPtr) { pl->Reset(); }
				for (int y = (int)(T_X.size()) - 1; y >= 0; --y) {
					auto& ti = T_X[y];
					if (!(cam.y <= ti.m_top[3].y && ti.m_zero[0].y <= limmax.y)) { continue; }
					Draw_Tile(ti, s_x, s_y, LIGHTPOS);
				}
			}
			void DrawCommon(int s_x, int s_y, Vector2D_I LIGHTPOS) noexcept {
				m_screen.SetDraw_Screen();
				{
					//画面位置取得
					const Vector2D_I limmin = ConvertPos(Vector2D_I::Get(-m_DrawPts->disp_x / 2, -m_DrawPts->disp_y / 2), 0, m_MapInfoPtr->m_caminfo);
					const Vector2D_I cam = ConvertPos(Vector2D_I::Zero(), 0, m_MapInfoPtr->m_caminfo);
					const Vector2D_I limmax = ConvertPos(Vector2D_I::Get(m_DrawPts->disp_x / 2, m_DrawPts->disp_y / 2), 0, m_MapInfoPtr->m_caminfo);

					for (int x = 0; x < (int)(m_Tile.size()); ++x) {
						if (!(cam.x >= m_Tile[x][0].m_top[0].x && m_Tile[x][0].m_zero[3].x >= limmin.x)) { continue; }
						DrawCommon_Y(m_Tile[x], s_x, s_y, LIGHTPOS);
					}
					for (int x = (int)(m_Tile.size()) - 1; x >= 0; --x) {
						if (!(cam.x <= m_Tile[x][0].m_top[3].x && m_Tile[x][0].m_zero[0].x <= limmax.x)) { continue; }
						DrawCommon_Y(m_Tile[x], s_x, s_y, LIGHTPOS);
					}
				}
			}
			//全体的な暗さ
			const auto SetAmbientBrightness(float _rad) const noexcept {
				if (_rad >= deg2rad(-90) && _rad <= deg2rad(90)) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192 + std::clamp(255 - (int)(192.f * abs(cos(_rad))), 0, 255) * (255 - 192) / 255);
					DrawBox(0, 0, m_DrawPts->disp_x, m_DrawPts->disp_y, GetColor(0, 0, 0), TRUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					return true;
				}
				else {
					DrawBox(0, 0, m_DrawPts->disp_x, m_DrawPts->disp_y, GetColor(0, 0, 0), TRUE);
				}
				return false;
			}
			//柱の影描画　ポイントライト用
			void Draw_Tile_PointShadow(const Tiles& _Ti, const Vector2D_I& _lightpos, int _high, int s_x, int s_y) noexcept {
				if (_Ti.m_isWall) {
					const auto xmin = y_r(tilesize) * (_Ti.m_postile.x + 0 + map_xsize * stage_x);
					const auto ymin = y_r(tilesize) * (_Ti.m_postile.y + 0 + map_ysize * stage_y);
					const auto xmax = y_r(tilesize) * (_Ti.m_postile.x + 1 + map_xsize * stage_x);
					const auto ymax = y_r(tilesize) * (_Ti.m_postile.y + 1 + map_ysize * stage_y);
					std::array<Vector2D_I, 8> m_shadow_pos;
					float _shadowRange = 16;

					m_shadow_pos[0] = GetPointShadowPos(_lightpos, Vector2D_I::Get(xmin, ymin), _shadowRange, _high, _Ti.m_bottom, _Ti.m_hight - _Ti.m_bottom, m_MapInfoPtr->m_caminfo);
					m_shadow_pos[1] = GetPointShadowPos(_lightpos, Vector2D_I::Get(xmax, ymin), _shadowRange, _high, _Ti.m_bottom, _Ti.m_hight - _Ti.m_bottom, m_MapInfoPtr->m_caminfo);
					m_shadow_pos[2] = GetPointShadowPos(_lightpos, Vector2D_I::Get(xmin, ymax), _shadowRange, _high, _Ti.m_bottom, _Ti.m_hight - _Ti.m_bottom, m_MapInfoPtr->m_caminfo);
					m_shadow_pos[3] = GetPointShadowPos(_lightpos, Vector2D_I::Get(xmax, ymax), _shadowRange, _high, _Ti.m_bottom, _Ti.m_hight - _Ti.m_bottom, m_MapInfoPtr->m_caminfo);
					m_shadow_pos[4] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin), _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);//◤
					m_shadow_pos[5] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax), _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);//◣
					m_shadow_pos[6] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin), _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);//◥
					m_shadow_pos[7] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax), _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);//◢
					//要改善
					//auto Aim = (Vector2D_I::Get(xmin + (xmax - xmin) / 2, ymin + (ymax - ymin) / 2) - _lightpos);
					//float _directionalLight_Rad = std::atan2f((float)Aim.x, (float)-Aim.y);

					switch (_Ti.m_dir) {//三角柱
					case 0://上
						DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)
						DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[3], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[2], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);		//横(左)
						break;
					case 1://左
						DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[3], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)
						DrawModi_wrap(m_shadow_pos[1], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);		//横(左)13
						break;
					case 2://下
						DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)18
						DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						DrawModi_wrap(m_shadow_pos[1], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[0], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);		//横(左)9
						break;
					case 3://右
						DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[2], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)
						DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[0], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);//横(左)
						break;
					case 4://上
						if (m_shadow_pos[4].y < m_shadow_pos[2].y) {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_floorhandle);
						}
						break;
					case 5://左
						if (m_shadow_pos[4].x < m_shadow_pos[3].x) {
							DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_floorhandle);
						}
						break;
					case 6://下
						if (m_shadow_pos[5].y > m_shadow_pos[1].y) {
							DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_floorhandle);
						}
						break;
					case 7://右
						if (m_shadow_pos[6].x > m_shadow_pos[2].x) {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_floorhandle);
						}
						break;
					default://柱
						DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)
						DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);//横(左)
						break;
					}
				}
				else {
					float _shadowRange = 3;
					for (auto& pl : this->m_humanPtr) { pl->Draw_Shadow_Point(_Ti, _lightpos, _high, _shadowRange, m_MapInfoPtr->m_caminfo, s_x, s_y); }
					for (auto& mg : m_MapInfoPtr->m_mag) { mg.Draw_Shadow_Point(_Ti, _lightpos, _high, _shadowRange, m_MapInfoPtr->m_caminfo, s_x, s_y); }
					for (auto& gn : this->m_gunPtr) { gn->Draw_Shadow_Point(_Ti, _lightpos, _high, _shadowRange, m_MapInfoPtr->m_caminfo, s_x, s_y); }
				}
			}
			//柱の影描画　環境光用
			void Draw_Tile_Shadow(const Tiles& _Ti, float _directionalLight_Rad, float _shadowRange, int _high, int s_x, int s_y) noexcept {
				if (_Ti.m_isWall) {
					const auto zh2 = (float)(_Ti.m_hight - _Ti.m_bottom - _high) * _shadowRange;
					auto shadow_add = Vector2D_I::Get((int)(zh2 * sin(_directionalLight_Rad)), (int)(zh2 * cos(_directionalLight_Rad)));
					const auto xmin = y_r(tilesize) * (_Ti.m_postile.x + 0 + map_xsize * stage_x);
					const auto ymin = y_r(tilesize) * (_Ti.m_postile.y + 0 + map_ysize * stage_y);
					const auto xmax = y_r(tilesize) * (_Ti.m_postile.x + 1 + map_xsize * stage_x);
					const auto ymax = y_r(tilesize) * (_Ti.m_postile.y + 1 + map_ysize * stage_y);
					std::array<Vector2D_I, 8> m_shadow_pos;

					m_shadow_pos[0] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin) + shadow_add, _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);
					m_shadow_pos[1] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin) + shadow_add, _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);
					m_shadow_pos[2] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax) + shadow_add, _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);
					m_shadow_pos[3] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax) + shadow_add, _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);
					m_shadow_pos[4] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin), _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);//◤
					m_shadow_pos[5] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax), _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);//◣
					m_shadow_pos[6] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin), _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);//◥
					m_shadow_pos[7] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax), _Ti.m_bottom + _high, m_MapInfoPtr->m_caminfo);//◢

					bool isDrawDown = _directionalLight_Rad >= deg2rad(-90) && _directionalLight_Rad <= deg2rad(90);
					bool isDrawRight = _directionalLight_Rad >= 0;
					switch (_Ti.m_dir) {//三角柱
					case 0://上
						if (isDrawDown) {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)
						}
						else {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						}
						if (isDrawRight) {
							DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[3], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						}
						else {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[2], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);		//横(左)
						}
						break;
					case 1://左
						if (isDrawDown) {
							DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[3], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)
						}
						else {
							DrawModi_wrap(m_shadow_pos[1], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						}
						if (isDrawRight) {
							DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						}
						else {
							DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);		//横(左)13
						}
						break;
					case 2://下
						if (isDrawDown) {
							DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)18
						}
						else {
							DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						}
						if (isDrawRight) {
							DrawModi_wrap(m_shadow_pos[1], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						}
						else {
							DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[0], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);		//横(左)9
						}
						break;
					case 3://右
						if (isDrawDown) {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[2], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)
						}
						else {
							DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[0], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						}
						if (isDrawRight) {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						}
						else {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);//横(左)
						}
						break;
					case 4://上
						if (!isDrawDown) {
							if (m_shadow_pos[4].y < m_shadow_pos[2].y) {
								DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_floorhandle);
							}
						}
						break;
					case 5://左
						if (m_shadow_pos[4].x < m_shadow_pos[3].x) {
							DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_floorhandle);
						}
						break;
					case 6://下
						if (isDrawDown) {
							if (m_shadow_pos[5].y > m_shadow_pos[1].y) {
								DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_floorhandle);
							}
						}
						break;
					case 7://右
						if (isDrawRight) {
							if (m_shadow_pos[6].x > m_shadow_pos[2].x) {
								DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_floorhandle);
							}
						}
						break;
					default://柱
						if (isDrawDown) {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[7], m_shadow_pos[5], _Ti.m_wallhandle);		//縦(下)
						}
						else {
							DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[4], _Ti.m_wallhandle);
						}
						if (isDrawRight) {
							DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[7], _Ti.m_wallhandle);		//横(右)
						}
						else {
							DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[4], m_shadow_pos[5], _Ti.m_wallhandle);//横(左)
						}
						break;
					}
				}
				else {
					for (auto& pl : this->m_humanPtr) { pl->Draw_Shadow(_Ti, _directionalLight_Rad, _shadowRange, m_MapInfoPtr->m_caminfo, s_x, s_y); }
					for (auto& mg : m_MapInfoPtr->m_mag) { mg.Draw_Shadow(_Ti, _directionalLight_Rad, _shadowRange, m_MapInfoPtr->m_caminfo); }
					for (auto& gn : this->m_gunPtr) { gn->Draw_Shadow(_Ti, _directionalLight_Rad, _shadowRange, m_MapInfoPtr->m_caminfo, s_x, s_y); }
				}
			}
			//影を一部分描画
			void Update_Shadow(int s_x, int s_y) noexcept {
				const auto limmin_shadow = ConvertPos(Vector2D_I::Get(-m_DrawPts->disp_x * 3 / 4, -m_DrawPts->disp_y * 3 / 4), 0, m_MapInfoPtr->m_caminfo);
				const auto limmax_shadow = ConvertPos(Vector2D_I::Get(m_DrawPts->disp_x * 3 / 4, m_DrawPts->disp_y * 3 / 4), 0, m_MapInfoPtr->m_caminfo);
				int high = 0;
				for (auto& g : m_shadow_graph) {
					g.SetDraw([&] {
						//環境影
						if (SetAmbientBrightness(m_MapInfoPtr->GetDirectionalLight_Rad())) {
							Set_Bright(0);
							for (auto& T_X : m_Tile) {
								if (!(T_X[0].m_zero[0].x <= limmax_shadow.x && T_X[0].m_zero[3].x >= limmin_shadow.x)) { continue; }
								for (auto& ti : T_X) {
									if (!(ti.m_zero[0].y <= limmax_shadow.y && ti.m_zero[3].y >= limmin_shadow.y)) { continue; }
									Draw_Tile_Shadow(ti, m_MapInfoPtr->GetDirectionalLight_Rad(), m_MapInfoPtr->GetShadowRange(), high, s_x, s_y);
								}
							}
						}
						//ライト
						{
							Set_Bright(255);
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
							for (auto& T_X : m_Tile) {
								if (!(T_X[0].m_zero[0].x <= limmax_shadow.x && T_X[0].m_zero[3].x >= limmin_shadow.x)) { continue; }
								for (auto& ti : T_X) {
									if (!(ti.m_zero[0].y <= limmax_shadow.y && ti.m_zero[3].y >= limmin_shadow.y)) { continue; }
									for (auto& gn : this->m_gunPtr) { gn->Draw_Light(ti, m_MapInfoPtr->m_caminfo); }
								}
							}
							SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
						}
						//ポイントライト影
						{
							Set_Bright(0);
							for (auto& T_X : m_Tile) {
								if (!(T_X[0].m_zero[0].x <= limmax_shadow.x && T_X[0].m_zero[3].x >= limmin_shadow.x)) { continue; }
								for (auto& ti : T_X) {
									if (!(ti.m_zero[0].y <= limmax_shadow.y && ti.m_zero[3].y >= limmin_shadow.y)) { continue; }
									for (auto& gn : this->m_gunPtr) {
										if (gn->lightActive()) {
											Draw_Tile_PointShadow(ti, gn->pos, high, s_x, s_y);
										}
									}
								}
							}
						}
					});
					high += 8;
				}
				Set_Bright(255);
			}
			//目標ウェイポイントに対する向きの取得
			const auto GetWaypointtoVec(Human_Object& pl) const noexcept { return (this->m_way_point.size() <= pl.cpu_do.GetFront()) ? Vector2D_I::Zero() : (this->m_way_point[pl.cpu_do.GetFront()] - pl.pos); }
			//ウェイポイント設定
			void SetNextWaypoint(Human_Object& pl) noexcept {
				int now = -1;
				//AIの次ウェイポイントを取得
				{
					auto tmp = Vector2D_I::Get(0, 1000);
					for (auto& w : this->m_way_point) {
						auto id = (int)(&w - &this->m_way_point[0]);
						if (pl.cpu_do.Compare(id)) {
							if (tmp.hydist() >= (w - pl.pos).hydist()) {
								auto Vec = Vector2D_I::Get((int)(sin(pl.Getyrad()) * 100.f), (int)(-cos(pl.Getyrad()) * 100.f));
								if (
									Vec.dot(w - pl.pos) > 0 &&
									!Get_HitWall(m_Tile, &w, pl.pos, 0, HIT_SELECT::ONLY_HIT) && 
									pl.cpu_do.GetFront() != id
									) {
									tmp = (w - pl.pos);
									now = int(id);
								}
							}
						}
					}
				}
				if (now != -1) {
					pl.cpu_do.Push(now);
				}
			}
			//敵視認
			const auto CheckFoundEnemy(Human_Object& pl, Vector2D_I* tgt, int _range) const noexcept {
				int Dst = (*tgt - pl.pos).dist();
				if (Dst <= _range) {
					auto Vec = Vector2D_I::Get((int)(sin(pl.GetLookyrad()) * 100.f), (int)(-cos(pl.GetLookyrad()) * 100.f));
					auto Rad = (int)(Vec.dot(*tgt - pl.pos) * 180.f / DX_PI_F / (Dst * 100) * 3 / 2);
					if (
						Rad > (90 - pl.Eye_Rad) &&
						!Get_HitWall(m_Tile, tgt, pl.pos, 0, HIT_SELECT::ONLY_HIT)
						) {
						return true;
					}
				}
				return false;
			}
		public:
			const auto GetMapXSize() const noexcept { return y_r(tilesize) * this->map_xsize; }
			const auto GetMapYSize() const noexcept { return y_r(tilesize) * this->map_xsize; }
		public:
			//仮のコンストラクタ、デストラクタ
			void Init(std::shared_ptr<DXDraw>& _DrawPts, MapInfo* _MapInfoPtr) noexcept {
				m_DrawPts = _DrawPts;			//引き継ぐ
				m_MapInfoPtr = _MapInfoPtr;

				for (auto& g : m_shadow_graph) { g.Init(m_DrawPts->disp_x, m_DrawPts->disp_y); }
				m_screen = GraphHandle::Make(m_DrawPts->disp_x, m_DrawPts->disp_y, true);
				m_res_floor = GraphHandle::Make(16, 16, true);
			}
			void Dispose(void) noexcept {
				m_DrawPts.reset();
			}
			//開始時処理
			const auto CheckNextPoint(Vector2D_I& POS) noexcept {
				Vector2D_I Pos = m_MapInfoPtr->GetPlayer().pos;
				int siz = 1;
				int X_MinLim = siz;
				int X_MaxLim = y_r(tilesize) * (int)(m_Tile.size() - 0) - siz;
				int Y_MinLim = siz;
				int Y_MaxLim = y_r(tilesize) * (int)(m_Tile.back().size() - 0) - siz;

				int siz2 = 2;
				int X_MinLim2 = siz2;
				int X_MaxLim2 = y_r(tilesize) * (int)(m_Tile.size() - 0) - siz2;
				int Y_MinLim2 = siz2;
				int Y_MaxLim2 = y_r(tilesize) * (int)(m_Tile.back().size() - 0) - siz2;

				int ans = 0;
				POS = m_MapInfoPtr->GetPlayer().pos;

				if (Pos.x < X_MinLim) {
					POS.x = X_MaxLim2;
					ans = 0;
				}
				else if (Pos.x <= X_MaxLim) {
					ans = 3;
				}
				else {
					POS.x = X_MinLim2;
					ans = 6;
				}

				if (Pos.y < Y_MinLim) {
					POS.y = Y_MaxLim2;
					return 0 + ans;
				}
				else if (Pos.y <= Y_MaxLim) {
					return (ans == 3) ? -1 : 1 + ans;//中央だけ-1
				}
				else {
					POS.y = Y_MinLim2;
					return 2 + ans;
				}
			}
			void SetStagePos(int _x, int _y) noexcept {
				stage_x = _x;
				stage_y = _y;
			}
			void Start(std::string _mapname) noexcept {
				std::fstream file;
				//mapデータ1読み込み(マップチップ)
				file.open(("data/Map/" + _mapname + "/1.dat").c_str(), std::ios::binary | std::ios::in);
				do {
					m_TileData.resize(m_TileData.size() + 1);
					file.read((char*)&m_TileData.back(), sizeof(m_TileData.back()));
				} while (!file.eof());
				file.close();
				m_TileData.pop_back();
				map_xsize = 0;
				map_ysize = 0;
				for (auto& d : m_TileData) {
					map_xsize = std::max(d.m_postile.x, map_xsize);
					map_ysize = std::max(d.m_postile.y, map_ysize);
				}
				map_xsize++;
				map_ysize++;

				m_Tile.resize(map_xsize);
				for (auto& T_X : m_Tile) {
					T_X.resize(map_ysize);
				}
				//mapデータ2読み込み(使用テクスチャ指定)
				Near3DEditer::Edit::maps mapb = GetMapInfo2(_mapname);
				m_MapInfoPtr->SetDirectionalLight_Rad() = mapb.m_DirectionalLight_Rad;
				if (wall_name != mapb.wall_name) {
					m_walls.clear();
					GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, 16, 16 * 2, &m_walls);
				}
				if (floor_name != mapb.floor_name) {
					m_floors.clear();
					GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, 16, 16, &m_floors);
				}
				wall_name = mapb.wall_name;
				floor_name = mapb.floor_name;
				//タイルの用意
				for (auto& at : m_TileData) {
					auto& ti = m_Tile[at.m_postile.x][at.m_postile.y];
					ti.m_postile = at.m_postile;
					ti.m_bottom = at.m_bottom;
					ti.m_hight = at.m_hight;
					ti.m_isWall = at.m_isWall;
					ti.m_dir = at.m_dir;
					ti.m_wallhandle = &m_walls[at.m_WallID];
					ti.m_floorhandle = &m_floors[at.m_FloorID];
				}
				//mapデータ4読み込み(ウェイポイント)
				file.open(("data/Map/" + _mapname + "/4.dat").c_str(), std::ios::binary | std::ios::in);
				do {
					Vector2D_I anse;
					file.read((char*)&anse, sizeof(anse));
					this->m_way_point.resize(this->m_way_point.size() + 1);
					this->m_way_point.back() = anse;
				} while (!file.eof());
				this->m_way_point.pop_back();
				file.close();
			}
			void Ready(void) noexcept {
				SE.Get((int)ENUM_SOUND::Envi).Play(0, DX_PLAYTYPE_LOOP, TRUE, 96);				//環境音開始
			}
			void Continue_Enemy(const Vector2D_I& _STAGE_ID) noexcept {
				this->m_humanPtr.resize(this->m_humanPtr.size() + 1);
				this->m_humanPtr.back() = &m_MapInfoPtr->SetPlayer();
				if (m_MapInfoPtr->SetPlayer().isHaveGun()) {
					this->m_gunPtr.resize(this->m_gunPtr.size() + 1);
					this->m_gunPtr.back() = m_MapInfoPtr->SetPlayer().haveGun;
				}
				int Human_OfSet = m_MapInfoPtr->GetOffSet(_STAGE_ID);
				int Human_size = m_MapInfoPtr->GetBufSize(_STAGE_ID);

				for (int i = 0; i < Human_size; i++) {
					this->m_humanPtr.resize(this->m_humanPtr.size() + 1);
					this->m_humanPtr.back() = &m_MapInfoPtr->m_human[Human_OfSet];
					this->m_humanPtr.back()->Init(&this->m_MapInfoPtr->m_humangraph[1]);
					if (!m_MapInfoPtr->m_gun[Human_OfSet].GetHaveHuman()) {
						this->m_gunPtr.resize(this->m_gunPtr.size() + 1);
						this->m_gunPtr.back() = &m_MapInfoPtr->m_gun[Human_OfSet];
						this->m_gunPtr.back()->Init(&this->m_MapInfoPtr->m_gungraph[0]);
					}
					Human_OfSet++;
				}
				//人間の用意
				{
					int i = (m_MapInfoPtr->SetPlayer().isHaveGun()) ? 1 : 0;
					for (auto& pl : m_humanPtr) {
						if (!pl->IsPlayer()) {
							if (m_gunPtr.size() > i) {
								if (m_gunPtr[i]->haveHuman == nullptr) {
									pl->SetGun(m_gunPtr[i]);
								}
								i++;
							}
							SetNextWaypoint(*pl);
						}
					}
				}
			}
			void Start_Enemy(std::string _mapname, const Vector2D_I& _STAGE_ID) noexcept {
				std::fstream file;
				//mapデータ3読み込み(敵情報)
				{
					this->m_humanPtr.resize(this->m_humanPtr.size() + 1);
					this->m_humanPtr.back() = &m_MapInfoPtr->SetPlayer();
					if (m_MapInfoPtr->SetPlayer().isHaveGun()) {
						this->m_gunPtr.resize(this->m_gunPtr.size() + 1);
						this->m_gunPtr.back() = m_MapInfoPtr->SetPlayer().haveGun;
					}
					int Human_OfSet = m_MapInfoPtr->GetOffSet(_STAGE_ID);
					auto info3 = GetMapInfo3(_mapname);
					for (const auto& m : info3) {
						if (m.pos_p != Vector2D_I::Zero()) {
							//人間セット
							this->m_humanPtr.resize(this->m_humanPtr.size() + 1);
							this->m_humanPtr.back() = &m_MapInfoPtr->m_human[Human_OfSet];
							this->m_humanPtr.back()->Init(m.pos_p, &this->m_MapInfoPtr->m_humangraph[1]);
							if (!m_MapInfoPtr->m_gun[Human_OfSet].GetHaveHuman()) {
								this->m_gunPtr.resize(this->m_gunPtr.size() + 1);
								this->m_gunPtr.back() = &m_MapInfoPtr->m_gun[Human_OfSet];
								this->m_gunPtr.back()->Init(&this->m_MapInfoPtr->m_gungraph[0]);
							}
							Human_OfSet++;
						}
					}
				}
				//人間の用意
				{
					int i = (m_MapInfoPtr->SetPlayer().isHaveGun()) ? 1 : 0;
					for (auto& pl : m_humanPtr) {
						if (!pl->IsPlayer()) {
							if (m_gunPtr.size() > i) {
								if (m_gunPtr[i]->haveHuman == nullptr) {
									pl->SetGun(m_gunPtr[i]);
								}
								i++;
							}
							SetNextWaypoint(*pl);
						}
					}
				}
			}
			//更新
			void Update(Vector2D_I& m_vec, int _PlayerInput, const Vector2D_I& cam_pos, int _ismainSeg) noexcept {
				bool is_stand = ((_PlayerInput & (1 << 0)) != 0);
				bool is_run = ((_PlayerInput & (1 << 1)) != 0);
				bool aim = ((_PlayerInput & (1 << 2)) != 0);
				bool shoot = ((_PlayerInput & (1 << 3)) != 0);
				bool reload = ((_PlayerInput & (1 << 4)) != 0);
				bool rolling = ((_PlayerInput & (1 << 5)) != 0);
				bool _Look = ((_PlayerInput & (1 << 6)) != 0);
				bool _Get = ((_PlayerInput & (1 << 7)) != 0);
				//人の移動処理
				{
					bool DisableTrans = false;
					for (auto& pl : m_humanPtr) { DisableTrans |= pl->m_Transceiver.Start; }
					bool FoundEnemyAny = false;
					int maximamPhase = 0;
					for (auto& pl : m_humanPtr) {
						Vector2D_I Vec, Aim;
						bool is_Run;
						bool is_Get = false;
						bool FoundEnemy = false;
						//CPU
						if (!pl->IsPlayer()) {
							int X = 0, Y = 0;
							float stoptime = 9.f;
							{
								//アラートからコーションに落ちた時に警戒無線する
								if (!pl->m_Transceiver.Switch && (!pl->isCaution() && pl->isAlart())) {
									pl->m_Transceiver.Switch = true;
								}
								if (DisableTrans) {
									pl->m_Transceiver.Switch = false;
								}
								if (pl->m_Transceiver.Switch && pl->isCaution()) {
									pl->m_Transceiver.Switch = false;
									pl->EnableTransceiver();
								}

								if (pl->CanFindEnemy() && _ismainSeg) {
									easing_set(&pl->Eye_Range, (float)(pl->isCaution() ? y_r(tilesize * 7) : y_r(tilesize * 4)), 0.9f);
									float rad = (float)(pl->m_Transceiver.Start ? 20 : 60);
									if (pl->Stop_Time > pl->Walk_Time) {
										rad = 30.f;
									}
									easing_set(&pl->Eye_Rad, rad, 0.95f);
									if (CheckFoundEnemy(*pl, &(m_MapInfoPtr->SetPlayer().pos), (int)pl->Eye_Range)) {
										FoundEnemy = true;
										FoundEnemyAny = true;
										pl->FindEnemy();
									}
								}
								//方向指定
								Vector2D_I WPvec;
								if (pl->isHaveGun() || pl->isAlart()) {//AIナビ
									is_Run = (pl->isAlart() && !FoundEnemy);
									WPvec = GetWaypointtoVec(*pl);
									//到達時
									if (WPvec.hydist() <= y_r(tilesize) * y_r(tilesize)) {
										SetNextWaypoint(*pl);
									}
								}
								else {//近場の銃器を拾う
									bool tmpb = false;
									Vector2D_I tmpv = Vector2D_I::Get(100000, 100000);
									for (auto& gn : m_gunPtr) {
										if (!gn->GetHaveHuman()) {
											if (tmpv.hydist() <= (gn->pos - pl->pos).hydist()) {
												if (CheckFoundEnemy(*pl, &gn->pos, y_r(tilesize * 7))) {
													tmpb = true;
													tmpv = (gn->pos - pl->pos);
												}
											}
										}
									}
									if (tmpb) {
										is_Run = true;
										WPvec = tmpv;
									}
									else {
										is_Run = (pl->isAlart() && !FoundEnemy);
										WPvec = GetWaypointtoVec(*pl);
									}
									//到達時
									if (!pl->isDamageDown() && WPvec.dist() <= y_r(tilesize / 2)) {
										is_Get = true;
									}
								}
								//スタック回避
								float PP = ((float)pl->Get_vec_buf().dist() / pl->GetSpeed());
								if (PP <= 0.25f) {
									SetNextWaypoint(*pl);
								}

								float walk_time = pl->Walk_Time;
								if (pl->m_Transceiver.Start) {
									pl->Stop_Time = walk_time;
								}
								else {
									pl->Stop_Time += 1.f / FPS;
									if (pl->Stop_Time < walk_time) {
										for (auto& tgt : m_humanPtr) {
											if (tgt != pl && !tgt->IsPlayer() && (tgt->pos - pl->pos).dist() <= pl->Eye_Range * 2 / 3) {
												pl->Stop_Time = 0.f;
												break;
											}
										}
									}
								}
								if (pl->Stop_Time - walk_time > stoptime) {
									pl->Walk_Time = 7.f + (float)GetRandf(5.f);
									pl->Stop_Time = 0.f;
								}
								if (pl->Stop_Time < walk_time) {
									if (WPvec.x != 0) {
										X = (WPvec.x > 0) ? 1 : -1;
									}
									if (WPvec.y != 0) {
										Y = (WPvec.y > 0) ? 1 : -1;
									}
								}
							}
							pl->SetStand(false);													//立ち伏せ
							pl->SetKey(true, FoundEnemy ? (GetRand(10) == 0) : false, false, false, m_MapInfoPtr->m_caminfo);	//入力
							Vec.set(X, Y);														//方向入力
							//エイム先
							if (pl->isAlart()) {
								pl->Stop_Time = 0.f;
								Aim = (m_MapInfoPtr->GetPlayer().pos - pl->pos);
							}
							else {
								if (pl->Stop_Time > pl->Walk_Time) {
									float tim_rad = deg2rad(90)*atan(sin(deg2rad((pl->Stop_Time - pl->Walk_Time) / stoptime * 720.f)));
									easing_set(&pl->Look_Rad, tim_rad, 0.95f);
									float rad = pl->Getyrad() + pl->Look_Rad;
									float spd = 100.f;
									Aim = Vector2D_I::Get((int)(sin(rad) * spd), (int)(-cos(rad) * spd));
								}
								else {
									Aim = Vector2D_I::Zero();
								}
							}

							if (is_Get) {
								for (auto& gn : this->m_gunPtr) {
									if (pl->haveGun != gn && !gn->GetHaveHuman()) {
										if ((gn->pos - pl->pos).dist() < y_r(tilesize)) {
											if (pl->isHaveGun()) {
												pl->haveGun->Put(pl->pos, GetTile(pl->pos).m_hight);
											}
											pl->SetGun(gn);
											break;
										}
									}
								}
							}
							bool OLD = pl->m_Transceiver.Start;
							pl->Update_Human(m_Tile, Vec, Aim, is_Run, m_MapInfoPtr->m_WorldPhase.Get_LowestPhase());	//移動
							if (pl->isReloadStart()) { m_MapInfoPtr->AddMag(pl->haveGun); }								//マガジン落下
							maximamPhase = std::min(maximamPhase, pl->GetPhase());
							if (!pl->m_Transceiver.Start && pl->m_Transceiver.Start != OLD) { m_MapInfoPtr->m_WorldPhase.Find(); }
						}
						//自機の移動
						else {
							if (_ismainSeg) {
								pl->SetStand(is_stand && !is_run);									//立ち伏せ
								pl->SetKey(aim, shoot, reload, rolling, m_MapInfoPtr->m_caminfo);	//入力
								if (_Look) { pl->LookGun(); }
								is_Get = _Get;
								Vec = m_vec;														//方向入力
								is_Run = is_run;
								int x_m, y_m;
								GetMousePoint(&x_m, &y_m);
								auto P = ConvertPos_CalcCam(pl->pos, pl->GetHight(), m_MapInfoPtr->m_caminfo);
								Aim = Vector2D_I::Get(x_m, y_m) - P;								//エイム先

								if (is_Get) {
									for (auto& gn : this->m_gunPtr) {
										if (pl->haveGun != gn && !gn->GetHaveHuman()) {
											if ((gn->pos - pl->pos).dist() < y_r(tilesize)) {
												if (pl->isHaveGun()) {
													pl->haveGun->Put(pl->pos, GetTile(pl->pos).m_hight);
												}
												pl->SetGun(gn);
												break;
											}
										}
									}
								}
								pl->Update_Human(m_Tile, Vec, Aim, is_Run, m_MapInfoPtr->m_WorldPhase.Get_LowestPhase());	//移動
								if (pl->isReloadStart()) { m_MapInfoPtr->AddMag(pl->haveGun); }								//マガジン落下
							}
						}
					}
					if (_ismainSeg) {
						m_MapInfoPtr->m_WorldPhase.Update(&maximamPhase, FoundEnemyAny);	//警戒度指定
					}
				}
				if (_ismainSeg) {
					m_MapInfoPtr->Update_Cam(cam_pos);									//カメラ座標設定
					//その他アップデート
				}
				{
					//人間その他更新
					for (auto& pl : this->m_humanPtr) {
						if (!pl->IsPlayer()) {
							pl->Update(this->m_MapInfoPtr->m_caminfo, this->m_MapInfoPtr->GetHumanAnimeData());
							pl->SetHight(GetTile(pl->pos));
						}
						else {
							if (_ismainSeg) {
								pl->Update(this->m_MapInfoPtr->m_caminfo, this->m_MapInfoPtr->GetHumanAnimeData());
								pl->SetHight(GetTile(pl->pos));
							}
						}
					}
					//近接攻撃
					for (auto& pl : this->m_humanPtr) {
						for (auto& tgt : this->m_humanPtr) {
							if (&tgt != &pl) {
								if (tgt->MeleeHit(pl)) {
									tgt->Damage(pl, this->m_MapInfoPtr->m_caminfo, pl->isDownMeleeAttack(), true);
									if (tgt->isDamageDown()) {
										if (tgt->isHaveGun()) { tgt->haveGun->Put(tgt->pos, GetTile(tgt->pos).m_hight); }
										tgt->SetGun(nullptr);
									}
								}
							}
						}
					}
					{
						//マガジンのアップデート
						for (auto& mg : this->m_MapInfoPtr->m_mag) { mg.Update(m_Tile); }
						//銃アップデート
						for (auto& gn : this->m_gunPtr) {
							if (!_ismainSeg && gn->haveHuman != nullptr && gn->haveHuman->IsPlayer()) {
								continue;
							}

							gn->Update(this->m_MapInfoPtr->m_caminfo);
							gn->SetHight(GetTile(gn->pos));
							for (auto& am : gn->ammo) {
								am.Update(m_Tile, this->m_MapInfoPtr->m_caminfo);
								for (auto& pl : this->m_humanPtr) {
									if (am.GetHitHuman(*pl)) {
										if (!am.isHit()) { this->m_MapInfoPtr->m_effect[0].Set_Hit(&am, pl); }
										am.Set_Hit(false);
										pl->Damage(gn->haveHuman, this->m_MapInfoPtr->m_caminfo, false, false);
										if (pl->isDamageDown()) {
											if (pl->isHaveGun()) { pl->haveGun->Put(pl->pos, GetTile(pl->pos).m_hight); }
											pl->SetGun(nullptr);
										}
										break;
									}
								}
							}
							for (auto& am : gn->cart) { am.Update(m_Tile, this->m_MapInfoPtr->m_caminfo); }
						}
						//エフェクトのアップデート
						for (auto& ef : this->m_MapInfoPtr->m_effect) { ef.Update(); }
					}
				}
			}
			void Set_Draw(void) noexcept {
				//タイルの設置
				for (auto& at : m_TileData) {
					auto& ti = m_Tile[at.m_postile.x][at.m_postile.y];
					const auto xmin = y_r(tilesize) * (ti.m_postile.x + 0 + map_xsize * stage_x);
					const auto ymin = y_r(tilesize) * (ti.m_postile.y + 0 + map_ysize * stage_y);
					const auto xmax = y_r(tilesize) * (ti.m_postile.x + 1 + map_xsize * stage_x);
					const auto ymax = y_r(tilesize) * (ti.m_postile.y + 1 + map_ysize * stage_y);
					ti.m_top[0] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin), ti.m_hight, m_MapInfoPtr->m_caminfo);
					ti.m_top[1] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin), ti.m_hight, m_MapInfoPtr->m_caminfo);
					ti.m_top[2] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax), ti.m_hight, m_MapInfoPtr->m_caminfo);
					ti.m_top[3] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax), ti.m_hight, m_MapInfoPtr->m_caminfo);
					ti.m_floor[0] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin), ti.m_bottom, m_MapInfoPtr->m_caminfo);
					ti.m_floor[1] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin), ti.m_bottom, m_MapInfoPtr->m_caminfo);
					ti.m_floor[2] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax), ti.m_bottom, m_MapInfoPtr->m_caminfo);
					ti.m_floor[3] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax), ti.m_bottom, m_MapInfoPtr->m_caminfo);
					ti.m_zero[0] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin), 0, m_MapInfoPtr->m_caminfo);
					ti.m_zero[1] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin), 0, m_MapInfoPtr->m_caminfo);
					ti.m_zero[2] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax), 0, m_MapInfoPtr->m_caminfo);
					ti.m_zero[3] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax), 0, m_MapInfoPtr->m_caminfo);
				}
				Update_Shadow(y_r(tilesize) * map_xsize * stage_x, y_r(tilesize) * map_ysize * stage_y);		//影
				Vector2D_I LIGHTPOS;
				for (auto& gn : this->m_gunPtr) {
					if (gn->lightActive()) {
						LIGHTPOS = gn->pos;
						break;
					}
				}
				DrawCommon(y_r(tilesize) * map_xsize * stage_x, y_r(tilesize) * map_ysize * stage_y, LIGHTPOS);			//一気に描画
			}
			//出力
			void Output(void) noexcept {
				m_screen.DrawGraph(0, 0, true);
			}
			void Output_UI(void) noexcept {
				for (auto& pl : m_humanPtr) {
					if (!pl->IsPlayer()) { pl->Draw_UI(m_Tile, m_MapInfoPtr->m_caminfo, m_DrawPts->disp_x, m_DrawPts->disp_y, y_r(tilesize) * map_xsize * stage_x, y_r(tilesize) * map_ysize * stage_y); }
				}
			}
			//後始末
			void Dispose_Map(void) noexcept {
				//マップデータ削除
				this->m_way_point.clear();
				m_TileData.clear();
				for (auto& T_X : m_Tile) {
					T_X.clear();
				}
				m_Tile.clear();
			}
			void Dispose_Enemy(void) noexcept {
				//銃の保持を解除
				for (auto& pl : m_humanPtr) {
					if (!pl->IsPlayer()) {
						pl->SetGun(nullptr);
						pl->Dispose();
					}
				}
				for (auto& gn : this->m_gunPtr) {
					if (gn->haveHuman == nullptr) { gn->Dispose(); }
				}
				m_humanPtr.clear();				//人間ののポインタを解除
				m_gunPtr.clear();				//銃のポインタを解除
				m_MapInfoPtr->m_mag.clear();	//マガジンは初期化
			}
		};
		class STAGE_PREV {
		public:
			Vector2D_I STAGE = Vector2D_I::Get(-1, -1);
			Vector2D_I STAGEV = Vector2D_I::Get(-1, -1);
		};
	private:
		std::shared_ptr<DXDraw> m_DrawPts{ nullptr };			//引き継ぐ
		LPCSTR m_font_path;
		MapInfo m_MapInfo;
		std::array<STAGE_PREV, 3> m_PS;
		int NextCnt = 0;
		int next_stage = 4;
		std::array<std::array<MapDraw, 3>, 3> m_MapDraws;		//描画するステージ
		const int m_StageXSize = 40;
		const int m_StageYSize = 40;
		std::array<std::array<std::string, 40>, 40> m_MapName;	//名前保存バッファ
	private:
		//ポイントライト用
		static Vector2D_I GetPointShadowPos(const Vector2D_I& _lightpos, const Vector2D_I& _pos, float _shadowRange, int _high, int _basehight, int _hight, const Camera_Info& _caminfo) noexcept {
			auto Aim = (_pos - _lightpos);
			float _directionalLight_Rad = std::atan2f((float)Aim.x, (float)Aim.y);
			float zh2 = (float)(_hight - _high) * _shadowRange;
			auto shadow_add = Vector2D_I::Get((int)(zh2 * sin(_directionalLight_Rad)), (int)(zh2 * cos(_directionalLight_Rad)));
			return ConvertPos_CalcCam(_pos + shadow_add, _basehight + _high, _caminfo);
		}
		//Near3D用サウンド
		static void PlaySound_Near3D(ENUM_SOUND _SoundID, const Vector2D_I& _pos, const Camera_Info& _caminfo, int _Vol = 255) noexcept {
			auto DispPos = ConvertPos_CalcCam(_pos, 0, _caminfo);
			auto Distance = (DispPos - Vector2D_I::Get(deskx / 2, desky / 2)).dist();
			SE.Get((int)_SoundID).Play(0, DX_PLAYTYPE_BACK, TRUE, std::clamp(_Vol - (_Vol / 2) * Distance / (deskx / 2), 0, 255), std::clamp(255 * (DispPos.x - (deskx / 2)) / (deskx / 2), -255, 255));
		}
		//球と壁の判定
		static bool Get_HitWall(const std::vector<std::vector<Tiles>>& _tile, Vector2D_I* _pos, const Vector2D_I& _oldpos, int _radius, HIT_SELECT Sel) noexcept {
			if (Sel == HIT_SELECT::GET_ZURI) {
				//_pos->x = std::clamp(_pos->x, _radius, y_r(tilesize) * (int)(_tile.size()) - _radius);
				//_pos->y = std::clamp(_pos->y, _radius, y_r(tilesize) * (int)(_tile.back().size()) - _radius);
			}
			bool isHit = false;
			for (int i = 0; i < 2; i++) {
				for (auto& T_X : _tile) {
					for (auto& ti : T_X) {
						if (ti.m_isWall) {
							const auto x0 = y_r(tilesize) * (ti.m_postile.x + 0) - _radius;
							const auto y0 = y_r(tilesize) * (ti.m_postile.y + 0) - _radius;
							const auto x1 = y_r(tilesize) * (ti.m_postile.x + 1) + _radius;
							const auto y1 = y_r(tilesize) * (ti.m_postile.y + 1) + _radius;
							Vector2D_I s0 = { x0 ,y0 };
							Vector2D_I s1 = { x0 ,y1 };
							Vector2D_I s2 = { x1 ,y0 };
							Vector2D_I s3 = { x1 ,y1 };
							if (Get_CollisionSegment2(_pos, _oldpos, s1, s0, Sel)) {
								switch (Sel)
								{
								case HIT_SELECT::ONLY_HIT:
									return true;
									break;
								case HIT_SELECT::GET_HIT:
									isHit = true;
									break;
								case HIT_SELECT::GET_ZURI:
									isHit = true;
									break;
								}
							}
							if (Get_CollisionSegment2(_pos, _oldpos, s0, s2, Sel)) {
								switch (Sel)
								{
								case HIT_SELECT::ONLY_HIT:
									return true;
									break;
								case HIT_SELECT::GET_HIT:
									isHit = true;
									break;
								case HIT_SELECT::GET_ZURI:
									isHit = true;
									break;
								}
							}
							if (Get_CollisionSegment2(_pos, _oldpos, s2, s3, Sel)) {
								switch (Sel)
								{
								case HIT_SELECT::ONLY_HIT:
									return true;
									break;
								case HIT_SELECT::GET_HIT:
									isHit = true;
									break;
								case HIT_SELECT::GET_ZURI:
									isHit = true;
									break;
								}
							}
							if (Get_CollisionSegment2(_pos, _oldpos, s3, s1, Sel)) {
								switch (Sel)
								{
								case HIT_SELECT::ONLY_HIT:
									return true;
									break;
								case HIT_SELECT::GET_HIT:
									isHit = true;
									break;
								case HIT_SELECT::GET_ZURI:
									isHit = true;
									break;
								}
							}
						}
					}
				}
				if (Sel != HIT_SELECT::GET_ZURI) {
					return isHit;
				}
			}
			return isHit;
		}
		//
		auto GetNextStageV(void) noexcept {
			Vector2D_I S;
			switch (next_stage) {
			case 0:
				S += Vector2D_I::Get(-1, -1);
				break;
			case 1:
				S += Vector2D_I::Get(-1, 0);
				break;
			case 2:
				S += Vector2D_I::Get(-1, 1);
				break;
			case 3:
				S += Vector2D_I::Get(0, -1);
				break;
			case 4:
				S += Vector2D_I::Get(0, 0);
				break;
			case 5:
				S += Vector2D_I::Get(0, 1);
				break;
			case 6:
				S += Vector2D_I::Get(1, -1);
				break;
			case 7:
				S += Vector2D_I::Get(1, 0);
				break;
			case 8:
				S += Vector2D_I::Get(1, 1);
				break;
			default:
				break;
			}
			return S;
		}
	public:
		const auto PlayerPos(void) const noexcept { return m_MapInfo.GetPlayer().pos; }
	public:
		//コンストラクタ
		Near3DControl(std::shared_ptr<DXDraw>& _DrawPts) noexcept {
			m_DrawPts = _DrawPts;			//引き継ぐ
			m_font_path = "data/x14y24pxHeadUpDaisy.ttf"; // 読み込むフォントファイルのパス
			if (AddFontResourceEx(m_font_path, FR_PRIVATE, NULL) < 0) {
				MessageBox(NULL, "フォント読込失敗", "", MB_OK);
			}
			//音声読み込み
			{
				SE.Add((int)ENUM_SOUND::RUN, 3, "data/Audio/SE/run.wav");
				SE.Add((int)ENUM_SOUND::WALK, 3, "data/Audio/SE/walk.wav");
				SE.Add((int)ENUM_SOUND::Punch, 3, "data/Audio/SE/Punch.wav");
				SE.Add((int)ENUM_SOUND::Kick, 3, "data/Audio/SE/Kick.wav");
				SE.Add((int)ENUM_SOUND::Hit, 3, "data/Audio/SE/Hit.wav");
				SE.Add((int)ENUM_SOUND::DownHit, 3, "data/Audio/SE/DownHit.wav");
				SE.Add((int)ENUM_SOUND::Fire0, 3, "data/Audio/SE/Gun/fire_0.wav");
				SE.Add((int)ENUM_SOUND::Case0, 3, "data/Audio/SE/Gun/case_0.wav");
				SE.Add((int)ENUM_SOUND::MagDown0, 3, "data/Audio/SE/Gun/magdown_0.wav");
				SE.Add((int)ENUM_SOUND::MagSet0, 3, "data/Audio/SE/Gun/magset_0.wav");
				SE.Add((int)ENUM_SOUND::Slide0, 3, "data/Audio/SE/Gun/slide_0.wav");
				SE.Add((int)ENUM_SOUND::Trigger, 3, "data/Audio/SE/Gun/trigger.wav");
				SE.Add((int)ENUM_SOUND::Equip, 3, "data/Audio/SE/equip.wav");
				SE.Add((int)ENUM_SOUND::WallHit0, 3, "data/Audio/SE/Hit/hit0.wav");
				SE.Add((int)ENUM_SOUND::WallHit1, 3, "data/Audio/SE/Hit/hit1.wav");
				SE.Add((int)ENUM_SOUND::Look, 3, "data/Audio/SE/Look.wav");
				SE.Add((int)ENUM_SOUND::rolling, 3, "data/Audio/SE/rolling.wav");
				SE.Add((int)ENUM_SOUND::Envi, 1, "data/Audio/SE/envi.wav");
				//SE.SetVol(0.5f);
				BGM.Add((int)ENUM_BGM::Phase1, 1, "data/Audio/BGM/Phase1.wav");
				BGM.Add((int)ENUM_BGM::Phase2, 1, "data/Audio/BGM/Phase2.wav");
				BGM.Add((int)ENUM_BGM::Phase3, 1, "data/Audio/BGM/Phase3.wav");
			}
			//共通リソース読み込み
			m_MapInfo.Init();
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					m_MapDraws[x][y].Init(m_DrawPts, &m_MapInfo);
				}
			}
			/*
			{
				int x_q = 0, y_q = 0;
				for (auto& xy : m_MapName) {
					y_q = 0;
					for (auto& y : xy) {
						if (!(x_q == 0 && y_q == 0)) {
							//std::format()
							y = "map"
								+ (std::string)((x_q < 10) ? "0" : "") + std::to_string(x_q) + "_"
								+ (std::string)((y_q < 10) ? "0" : "") + std::to_string(y_q);
							std::filesystem::create_directory("data/Map/" + y);
							std::filesystem::copy("data/Map/map00_00/1.dat", "data/Map/" + y + "/1.dat");
							std::filesystem::copy("data/Map/map00_00/2.dat", "data/Map/" + y + "/2.dat");
							std::filesystem::copy("data/Map/map00_00/3.dat", "data/Map/" + y + "/3.dat");
							std::filesystem::copy("data/Map/map00_00/4.dat", "data/Map/" + y + "/4.dat");
						}
						y_q++;
					}
					x_q++;
				}
			}
			//*/
			{
				int x_q = 0, y_q = 0;
				for (auto& xy : m_MapName) {
					y_q = 0;
					for (auto& y : xy) {
						y = "map"
							+ (std::string)((x_q < 10) ? "0" : "") + std::to_string(x_q) + "_"
							+ (std::string)((y_q < 10) ? "0" : "") + std::to_string(y_q);
						y_q++;
					}
					x_q++;
				}
			}
			//
		}
		//デストラクタ
		~Near3DControl(void) noexcept {
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					m_MapDraws[x][y].Dispose();
				}
			}
			m_DrawPts.reset();
			Dispose();
			m_MapInfo.Dispose();
			if (!RemoveFontResourceEx(m_font_path, FR_PRIVATE, NULL)) {
				MessageBox(NULL, "remove failure", "", MB_OK);
			}
		}
		//読み込み
		void Start(int _SpawnPoint, Vector2D_I _STAGE) noexcept {
			m_PS[0].STAGEV = Vector2D_I::Get(1, 1);
			m_PS[0].STAGE.set(std::clamp(_STAGE.x, 0, m_StageXSize - 1), std::clamp(_STAGE.y, 0, m_StageYSize - 1));
			{
				std::fstream file;
				//mapデータ2読み込み(プレイヤー初期位置指定)
				Near3DEditer::Edit::maps mapb = GetMapInfo2(m_MapName[m_PS[0].STAGE.x][m_PS[0].STAGE.y]);
				this->m_MapInfo.m_human.resize(1);
				this->m_MapInfo.m_human.back().Init(mapb.SP[std::clamp(_SpawnPoint, 0, mapb.SP_Limit - 1)], &this->m_MapInfo.m_humangraph[0], true);
			}
			int X = 0, Y = 0;
			for (auto& xy : m_MapName) {
				X = 0;
				for (auto& yt : xy) {
					m_MapInfo.Set_Human_Gun_Buffer(yt, X, Y);
					X++;
				}
				Y++;
			}
			next_stage = 4;
			NextStage();
		}
		//開始時処理
		void Ready(void) noexcept {
			m_MapDraws[m_PS[0].STAGEV.x][m_PS[0].STAGEV.x].Ready();
		}
		//更新
		void Update(Vector2D_I& m_vec, int _PlayerInput, const Vector2D_I& cam_pos) noexcept {
			m_MapInfo.Update_DirectionalLight();
			m_MapDraws[m_PS[0].STAGEV.x][m_PS[0].STAGEV.x].Update(m_vec, _PlayerInput, cam_pos, true);
			if (NextCnt >= 2 && m_PS[1].STAGEV != m_PS[0].STAGEV) {
				m_MapDraws[m_PS[1].STAGEV.x][m_PS[1].STAGEV.y].Update(m_vec, 0, cam_pos, false);
			}
			if (NextCnt >= 3 && m_PS[2].STAGEV != m_PS[1].STAGEV && m_PS[2].STAGEV != m_PS[0].STAGEV) {
				m_MapDraws[m_PS[2].STAGEV.x][m_PS[2].STAGEV.y].Update(m_vec, 0, cam_pos, false);
			}
			//描画の更新
			auto CP = m_MapInfo.m_caminfo.camerapos * -1.f;
			int X_size = m_MapDraws[1][1].GetMapXSize();
			int Y_size = m_MapDraws[1][1].GetMapYSize();
			if (CP.x > X_size * 2 / 3) {
				if (CP.y > Y_size * 2 / 3) {
					m_MapDraws[2][2].Set_Draw();
					m_MapDraws[1][2].Set_Draw();
				}
				else if (CP.y > Y_size / 3) {
				}
				else {
					m_MapDraws[2][0].Set_Draw();
					m_MapDraws[1][0].Set_Draw();
				}
				m_MapDraws[2][1].Set_Draw();
			}
			else if (CP.x > X_size / 3) {
			}
			else {
				if (CP.y > Y_size * 2 / 3) {
					m_MapDraws[0][2].Set_Draw();
					m_MapDraws[1][2].Set_Draw();
				}
				else if (CP.y > Y_size / 3) {
				}
				else {
					m_MapDraws[0][0].Set_Draw();
					m_MapDraws[1][0].Set_Draw();
				}
				m_MapDraws[0][1].Set_Draw();
			}
			m_MapDraws[1][1].Set_Draw();
		}
		//出力
		void Output(void) noexcept {
			auto CP = m_MapInfo.m_caminfo.camerapos * -1.f;
			int X_size = m_MapDraws[1][1].GetMapXSize();
			int Y_size = m_MapDraws[1][1].GetMapYSize();
			if (CP.x > X_size * 2 / 3) {
				if (CP.y > Y_size * 2 / 3) {
					m_MapDraws[2][2].Output();
					m_MapDraws[1][2].Output();
				}
				else if (CP.y > Y_size / 3) {
				}
				else {
					m_MapDraws[2][0].Output();
					m_MapDraws[1][0].Output();
				}
				m_MapDraws[2][1].Output();
			}
			else if (CP.x > X_size / 3) {
			}
			else {
				if (CP.y > Y_size * 2 / 3) {
					m_MapDraws[0][2].Output();
					m_MapDraws[1][2].Output();
				}
				else if (CP.y > Y_size / 3) {
				}
				else {
					m_MapDraws[0][0].Output();
					m_MapDraws[1][0].Output();
				}
				m_MapDraws[0][1].Output();
			}
			m_MapDraws[1][1].Output();
		}
		void Output_UI(void) noexcept {
			auto CP = m_MapInfo.m_caminfo.camerapos * -1.f;
			int X_size = m_MapDraws[1][1].GetMapXSize();
			int Y_size = m_MapDraws[1][1].GetMapYSize();
			if (CP.x > X_size * 2 / 3) {
				if (CP.y > Y_size * 2 / 3) {
					m_MapDraws[2][2].Output_UI();
					m_MapDraws[1][2].Output_UI();
				}
				else if (CP.y > Y_size / 3) {
				}
				else {
					m_MapDraws[2][0].Output_UI();
					m_MapDraws[1][0].Output_UI();
				}
				m_MapDraws[2][1].Output_UI();
			}
			else if (CP.x > X_size / 3) {
			}
			else {
				if (CP.y > Y_size * 2 / 3) {
					m_MapDraws[0][2].Output_UI();
					m_MapDraws[1][2].Output_UI();
				}
				else if (CP.y > Y_size / 3) {
				}
				else {
					m_MapDraws[0][0].Output_UI();
					m_MapDraws[1][0].Output_UI();
				}
				m_MapDraws[0][1].Output_UI();
			}
			//インジケーター、UI
			m_MapDraws[1][1].Output_UI();
			m_MapInfo.m_WorldPhase.Draw_UI();
			m_MapInfo.SetPlayer().Draw_GunUp(m_DrawPts->disp_x, m_DrawPts->disp_y);
		}
		//次ステージ確認 todo::画面外に行かない処理ないし画面外に行かないような地形に
		const auto GetNextStage(void) noexcept {
			Vector2D_I POS;
			next_stage = this->m_MapDraws[m_PS[0].STAGEV.x][m_PS[0].STAGEV.x].CheckNextPoint(POS);
			if (next_stage > 0) {
				auto S = m_PS[0].STAGE + GetNextStageV();
				if (S.x >= 0 && S.x <= m_StageXSize - 1 && S.y >= 0 && S.y <= m_StageYSize - 1) {
					this->m_MapInfo.SetPlayer().SetPlayerSpawnPos(POS);
					return true;
				}
			}
			return false;
		}
		void NextStage(void) noexcept {
			auto Gone = GetNextStageV();
			Dispose();
			m_PS[2].STAGE = m_PS[1].STAGE;
			m_PS[2].STAGEV = m_PS[1].STAGEV + Gone * -1.f;
			m_PS[1].STAGE = m_PS[0].STAGE;
			m_PS[1].STAGEV = m_PS[0].STAGEV + Gone * -1.f;
			m_PS[0].STAGE.set(std::clamp(m_PS[0].STAGE.x + Gone.x, 0, m_StageXSize - 1), std::clamp(m_PS[0].STAGE.y + Gone.y, 0, m_StageYSize - 1));
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					int XP = m_PS[0].STAGE.x + x - 1;
					int YP = m_PS[0].STAGE.y + y - 1;
					if ((XP >= 0 && XP <= m_StageXSize - 1) && (YP >= 0 && YP <= m_StageYSize - 1)) {
						m_MapDraws[x][y].SetStagePos(x - 1, y - 1);//
						m_MapDraws[x][y].Start(m_MapName[XP][YP]);
					}
				}
			}
			NextCnt++;
			if (NextCnt >= 3) {
				m_MapDraws[m_PS[2].STAGEV.x][m_PS[2].STAGEV.y].Continue_Enemy(m_PS[2].STAGE);
			}
			if (NextCnt >= 2) {
				m_MapDraws[m_PS[1].STAGEV.x][m_PS[1].STAGEV.y].Continue_Enemy(m_PS[1].STAGE);
			}
			if (m_PS[2].STAGEV != m_PS[0].STAGEV) {
				m_MapDraws[m_PS[0].STAGEV.x][m_PS[0].STAGEV.x].Start_Enemy(m_MapName[m_PS[0].STAGE.x][m_PS[0].STAGE.y], m_PS[0].STAGE);
			}
			//足跡の移動
			m_MapInfo.SetPlayer().NextStage(Vector2D_I::Get(-Gone.x * m_MapDraws[m_PS[0].STAGEV.x][m_PS[0].STAGEV.x].GetMapXSize(), -Gone.y * m_MapDraws[m_PS[0].STAGEV.x][m_PS[0].STAGEV.x].GetMapYSize()));
		}
		//後始末
		void Dispose(void) noexcept {
			if (NextCnt >= 3) {
				m_MapDraws[m_PS[2].STAGEV.x][m_PS[2].STAGEV.y].Dispose_Enemy();
			}
			if (NextCnt >= 2) {
				m_MapDraws[m_PS[1].STAGEV.x][m_PS[1].STAGEV.y].Dispose_Enemy();
			}
			if (m_PS[2].STAGEV != m_PS[0].STAGEV) {
				m_MapDraws[m_PS[0].STAGEV.x][m_PS[0].STAGEV.x].Dispose_Enemy();
			}
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					m_MapDraws[x][y].Dispose_Map();//mapの後始末
				}
			}
		}
		//
	};
};