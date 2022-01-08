#pragma once
#include "Near3DControl.hpp"
#include "Enum.hpp"

namespace Near3D {
	FontPool Fonts;
	FontPool Fonts2;
	SoundPool SE;
	SoundPool BGM;
	//
	int Bright_buf = -1;
	void Set_Bright(int p) {
		if (p != Bright_buf) {
			SetDrawBright(p, p, p);
			Bright_buf = p;
		}
	}
	//定数
	const int tilesize = 128;	//タイル一つ一つのサイズ
	const int EyeRad = 45;		//視認角度
	//カスタムランダム
	static const float GetRandf(float m_arg) noexcept { return -m_arg + (float)(GetRand((int)(m_arg * 2.f * 10000.f))) / 10000.f; }
	//フォント
	static const auto& GetFont(int size) noexcept { return Fonts.Get(size, DX_FONTTYPE_EDGE, "x14y24pxHeadUpDaisy", true).Get_handle(); }
	static const auto& GetFont2(int size) noexcept { return Fonts2.Get(size, DX_FONTTYPE_NORMAL).Get_handle(); }
	//
	class Near3DControl {
	private:
		std::shared_ptr<DXDraw> DrawPts{ nullptr };			//引き継ぐ
		LPCSTR m_font_path;
	private:
		class TileStat_Common {
		public:
			Vector2D_I m_postile;
			int m_bottom = 0, m_hight = 0;
			bool m_isWall = false;
		};
		class TileStatus : public TileStat_Common {
		public:
			int m_WallID = 0, m_FloorID = 0;
			unsigned char m_dir = 0;// rect = 255 else prism = 0~3,4~7
		public:
			void Set_Tile(bool _isWall, int _bottom, int _hight, int _wall_id, int _floor_id) noexcept {
				this->m_isWall = _isWall;
				this->m_bottom = _bottom;
				this->m_hight = (this->m_isWall) ? _hight : this->m_bottom;
				this->m_dir = 255;
				this->m_WallID = _wall_id;
				this->m_FloorID = _floor_id;
			}
			int Get_Tile(int _map_Xsize, int _xadd = 0, int _yadd = 0) const noexcept { return (this->m_postile.x + _xadd) + (this->m_postile.y + _yadd) * _map_Xsize; }
			void SetGround(int _hight, unsigned char _dir = 255) noexcept {
				if (!this->m_isWall) {
					this->m_hight = _hight;
					this->m_dir = (this->m_hight != this->m_bottom) ? _dir : 255;
				}
			}
		};
	private:
		/*--エディター限定--*/
		//エディター限定クラス
		class Edit {
		public:
			class Player_Info {
			public:
				Vector2D_I pos_p;
			};
			class maps {
			public:
				const int SP_Limit = 10;
				Vector2D_I SP[10];
				char wall_name[MAX_PATH];
				char floor_name[MAX_PATH];
				float m_light_yrad;
			};
		public:
			std::list<std::vector<TileStatus>> List;//
			std::list<std::vector<TileStatus>>::iterator ListItr;
			std::vector<TileStatus> Data;
			std::vector<Player_Info> PlayerSpawnPoint;
			std::vector<Vector2D_I> m_way_point;
			maps mapdata;

			std::vector<GraphHandle> m_walls;
			std::vector<GraphHandle> floors;

			void Dispose() {
				for (auto& w : this->floors) {
					w.Dispose();
				}
				this->floors.clear();
				for (auto& w : this->m_walls) {
					w.Dispose();
				}
				this->m_walls.clear();
			}
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

			void Load_Common() {
				//mapデータ2読み込み(使用テクスチャ指定)
				GraphHandle::LoadDiv(this->mapdata.wall_name, 32, 16, 2, 16, 16 * 2, &this->m_walls);
				GraphHandle::LoadDiv(this->mapdata.floor_name, 256, 16, 16, 16, 16, &this->floors);
			}

			void Read(const std::string& mapChipName, const std::string& mapTexName, const std::string& playerInfoName, const std::string& WayPointInfoName) {
				std::fstream file;
				//mapデータ1読み込み(マップチップ)
				file.open(mapChipName.c_str(), std::ios::binary | std::ios::in);
				do {
					this->Data.resize(this->Data.size() + 1);
					file.read((char*)&this->Data.back(), sizeof(this->Data.back()));
				} while (!file.eof());
				this->Data.pop_back();
				file.close();
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
				this->PlayerSpawnPoint.pop_back();
				file.close();
				//mapデータ4読み込み(ウェイポイント)
				this->m_way_point.clear();
				file.open(WayPointInfoName.c_str(), std::ios::binary | std::ios::in);
				do {
					this->m_way_point.resize(this->m_way_point.size() + 1);
					file.read((char*)&this->m_way_point.back(), sizeof(this->m_way_point.back()));
				} while (!file.eof());
				this->m_way_point.pop_back();
				file.close();
				Load_Common();
			}
			void Write(const std::string& mapChipName, const std::string& mapTexName, const std::string& playerInfoName, const std::string& WayPointInfoName) {
				std::fstream file;
				//mapデータ1書き込み(マップチップ)
				file.open(mapChipName.c_str(), std::ios::binary | std::ios::out);
				for (auto& data : this->Data) { file.write((char*)&data, sizeof(data)); }
				this->Data.clear();
				file.close();
				//mapデータ2書き込み(プレイヤー初期位置、使用テクスチャ指定)
				file.open(mapTexName.c_str(), std::ios::binary | std::ios::out);
				file.write((char*)&this->mapdata, sizeof(this->mapdata));
				file.close();
				//mapデータ3書き込み(敵情報)
				file.open(playerInfoName.c_str(), std::ios::binary | std::ios::out);
				for (auto& SP_t : this->PlayerSpawnPoint) { file.write((char*)&SP_t, sizeof(SP_t)); }
				this->PlayerSpawnPoint.clear();
				file.close();
				//mapデータ4書き込み(ウェイポイント)
				file.open(WayPointInfoName.c_str(), std::ios::binary | std::ios::out);
				for (auto& WP : this->m_way_point) { file.write((char*)&WP, sizeof(WP)); }
				this->m_way_point.clear();
				file.close();
			}

			void PreSet(int Map_Xsize_t, int Map_Ysize_t) {
				//mapデータ1書き込み(マップチップ)
				const int btm = 0;
				const int mid = 0;
				const int hig = 64;
				for (int y = 0; y < Map_Ysize_t; y++) {
					for (int x = 0; x < Map_Xsize_t; x++) {
						this->Data.resize(this->Data.size() + 1);
						this->Data.back().m_postile = Vector2D_I::Get(x, y);
						this->Data.back().Set_Tile(false, btm, mid, 2, 2);
					}
				}
				for (int y = 0; y < Map_Ysize_t; y += 5) {
					for (int x = 0; x < Map_Xsize_t; x += 5) {
						const size_t s = size_t(x + y * Map_Xsize_t);
						/*
						this->Data[s + 2 + Map_Xsize_t * 1] = { Vector2D_I::Get(x + 2, y + 1), btm, hig, true, 2, 2, 255 };
						this->Data[s + 1 + Map_Xsize_t * 2] = { Vector2D_I::Get(x + 1, y + 2), btm, hig, true, 2, 2, 255 };
						this->Data[s + 2 + Map_Xsize_t * 2] = { Vector2D_I::Get(x + 2, y + 2), btm, hig, true, 2, 2, 255 };
						this->Data[s + 3 + Map_Xsize_t * 2] = { Vector2D_I::Get(x + 3, y + 2), btm, hig, true, 2, 2, 255 };
						this->Data[s + 2 + Map_Xsize_t * 3] = { Vector2D_I::Get(x + 2, y + 3), btm, hig, true, 2, 2, 255 };
						//*/
						/*
						this->Data[s + 2 + Map_Xsize_t * 1] = { Vector2D_I::Get(x + 2, y + 1), btm, hig, true, 2, 2, 0 };
						this->Data[s + 1 + Map_Xsize_t * 2] = { Vector2D_I::Get(x + 1, y + 2), btm, hig, true, 2, 2, 1 };
						this->Data[s + 2 + Map_Xsize_t * 2] = { Vector2D_I::Get(x + 2, y + 2), btm, hig, true, 2, 1, 255 };
						this->Data[s + 3 + Map_Xsize_t * 2] = { Vector2D_I::Get(x + 3, y + 2), btm, hig, true, 2, 2, 3 };
						this->Data[s + 2 + Map_Xsize_t * 3] = { Vector2D_I::Get(x + 2, y + 3), btm, hig, true, 2, 2, 2 };
						//*/
					}
				}
				//mapデータ2書き込み(プレイヤー初期位置、使用テクスチャ指定)
				this->mapdata.SP[0].set(32, 32);
				this->mapdata.m_light_yrad = deg2rad(0);
				strcpy_s(this->mapdata.wall_name, "data/Chip/Wall/1.bmp");
				strcpy_s(this->mapdata.floor_name, "data/Chip/Floor/1.bmp");
				//mapデータ3書き込み(敵情報)
				this->PlayerSpawnPoint.clear();
				for (int i = 0; i < 7; i++) {
					this->PlayerSpawnPoint.resize(this->PlayerSpawnPoint.size() + 1);
					this->PlayerSpawnPoint.back().pos_p.x = y_r(tilesize) * 5 * (i + 1) + y_r(tilesize) / 2;
					this->PlayerSpawnPoint.back().pos_p.y = y_r(tilesize) * 5 * (i + 1) + y_r(tilesize) / 2;
				}
				//mapデータ4書き込み(ウェイポイント)
				this->m_way_point.clear();
				Load_Common();
			}
		};
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
				GetFont2(y_r(40)).DrawString_MID(xs + xsize / 2, ys, buf, on ? ((mouse_in) ? GetColor(255, 255, 0) : GetColor(255, 0, 0)) : GetColor(0, 0, 0));
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
				int x2 = xs + GetFont2(y_r(40)).GetDrawWidth(buf);
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
				GetFont2(y_r(40)).DrawString(xs, ys + y_r(15), buf, GetColor(255, 255, 0));
				DrawTriangle(x2 + xsize / 2, ys, x2, ys + ysize, x2 + xsize, ys + ysize, on ? ((mouse_in1) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
				DrawTriangle(x2, y2, x2 + xsize, y2, x2 + xsize / 2, y2 + ysize, on ? ((mouse_in2) ? GetColor(255, 0, 0) : GetColor(0, 255, 0)) : GetColor(128, 128, 128), TRUE);
			}
		};
	private:
		//エディター限定
		int m_mouse_x, m_mouse_y;
		bool m_save, m_wallorfloor, m_isread, m_smz, m_isend;
		int m_hight_s, m_bottom_s;
		int m_Map_Xsize, m_Map_Ysize;
		Edit m_TileEdit;
		DialogManager m_Dialog;
		bool m_TriggerWP = true;
		std::vector<bool> m_TriggerPP;
		std::vector<Button> m_Buttons;	//button
		int m_SelectWallTex = 0;
		int m_SelectFloorTex = 0;
	private:
		void Init_Editer() noexcept {
			m_Buttons.resize(12);
			//ダイアログ用
			m_Dialog.Init();
			m_TileEdit.Data.clear();
		}
		//エディター用関数
		void Init_Window1() noexcept {
			m_isread = false;
			m_Buttons[0].Init();
			m_Buttons[1].Init();
		}
		bool Window1() noexcept {
			DrawBox(x_r(960 - 320), y_r(540 - 180), x_r(960 + 320), y_r(540 + 180), GetColor(128, 128, 128), TRUE);
			GetFont2(y_r(40)).DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "プリセットを読み込みますか?", GetColor(255, 255, 0));
			bool end_f = true;
			m_Buttons[0].ButtonSet(m_mouse_x, m_mouse_y, x_r(960 + 320 - 340), y_r(540 + 180 - 140), x_r(300), y_r(40), "YES", true, [&]() { m_isread = true; end_f = false; });	//YES
			m_Buttons[1].ButtonSet(m_mouse_x, m_mouse_y, x_r(960 + 320 - 340), y_r(540 + 180 - 80), x_r(300), y_r(40), "NO", true, [&]() { end_f = false; });					//NO
			return end_f;
		}
		void Init_Window2() noexcept {
			m_wallorfloor = false;
			m_TileEdit.List.clear();
			m_TileEdit.List.push_back(m_TileEdit.Data);
			m_TileEdit.ListItr = m_TileEdit.List.end();
			m_hight_s = 64;
			m_bottom_s = 0;
			m_Buttons[2].Init();
			m_Buttons[3].Init();
			m_Buttons[4].Init();
			m_Buttons[5].Init();
			m_Buttons[6].Init();
			m_Buttons[7].Init();
			m_Buttons[8].Init();
			m_Buttons[9].Init();
			m_Buttons[10].Init();
			m_Buttons[11].Init();
			m_save = false;
			m_smz = false;
			m_isend = false;
			m_TriggerPP.resize(m_TileEdit.PlayerSpawnPoint.size());
		}
		bool Window2() noexcept {
			int tilesize_E2 = DrawPts->disp_y / std::max(m_Map_Xsize, m_Map_Ysize);
			int tilesize_E = tilesize_E2 * 38 / 40;
			//マップ描画
			{
				int xpos_E = DrawPts->disp_y / 40;
				int ypos_E = DrawPts->disp_y / 40;
				auto cam_high = (int)((float)m_caminfo.camhigh_base / m_caminfo.camzoom);
				for (auto& data : m_TileEdit.Data) {
					const int xs = xpos_E + (int)(data.m_postile.x * tilesize_E);
					const int ys = ypos_E + (int)(data.m_postile.y * tilesize_E);
					const int xe = xpos_E + (int)((data.m_postile.x + 1) * tilesize_E);
					const int ye = ypos_E + (int)((data.m_postile.y + 1) * tilesize_E);
					const unsigned char mh = (unsigned char)(255 * (cam_high - abs(data.m_hight)) / cam_high);
					const unsigned char mb = (unsigned char)(255 * (cam_high - abs(data.m_bottom)) / cam_high);

					if (in2_(m_mouse_x, m_mouse_y, xs, ys, xe, ye)) {
						if (data.m_isWall) {
							COLOR_U8 Color1 = GetColorU8(mh, mh / 2, 0u, 255u);
							COLOR_U8 Color2 = GetColorU8(mb, mb / 2, 0u, 255u);
							DrawGradationBox(xs, ys, xe, ye, Color1, Color2, data.m_dir);
						}
						else {
							COLOR_U8 Color1 = GetColorU8(mh, mh / 2, mh / 2, 255u);
							COLOR_U8 Color2 = GetColorU8(mb, mb / 2, mb / 2, 255u);
							DrawGradationBox(xs, ys, xe, ye, Color1, Color2, data.m_dir - 4);
						}
						GetFont2(y_r(40)).DrawStringFormat(m_Map_Xsize * tilesize_E2, y_r(40), GetColor(255, 255, 255), "(%03d,%03d)", data.m_postile.x, data.m_postile.y);

						if ((GetMouseInput() & MOUSE_INPUT_MIDDLE) != 0) {
							if (m_TriggerWP) {
								m_TriggerWP = false;
								m_TileEdit.m_way_point.emplace_back(Vector2D_I::Get(data.m_postile.x*y_r(tilesize) + y_r(tilesize) / 2, data.m_postile.y*y_r(tilesize) + y_r(tilesize) / 2));
							}
						}
						else {
							m_TriggerWP = true;
						}

						for (int i = 0; i < m_TileEdit.PlayerSpawnPoint.size(); i++) {
							if (CheckHitKey(KEY_INPUT_1 + i) != 0) {
								if (m_TriggerPP[i]) {
									m_TriggerPP[i] = false;
									m_TileEdit.PlayerSpawnPoint[i].pos_p = Vector2D_I::Get(data.m_postile.x*y_r(tilesize) + y_r(tilesize) / 2, data.m_postile.y*y_r(tilesize) + y_r(tilesize) / 2);
								}
							}
							else {
								m_TriggerPP[i] = true;
							}
						}

						if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
							if (m_wallorfloor) {
								data.Set_Tile(true, m_bottom_s, m_hight_s, m_SelectWallTex, m_SelectFloorTex);	//壁
							}
							else {
								data.Set_Tile(false, m_bottom_s, m_hight_s, m_SelectWallTex, m_SelectFloorTex);	//床
								//周りのタイルを変更
								if (m_smz) {
									if (!data.m_isWall) {
										if (data.m_postile.x > 0) {
											m_TileEdit.Data[data.Get_Tile(m_Map_Xsize, -1, 0)].SetGround(data.m_hight, 5);
										}
										if (data.m_postile.x < m_Map_Xsize - 1) {
											m_TileEdit.Data[data.Get_Tile(m_Map_Xsize, 1, 0)].SetGround(data.m_hight, 7);
										}

										if (data.m_postile.y > 0) {
											m_TileEdit.Data[data.Get_Tile(m_Map_Xsize, 0, -1)].SetGround(data.m_hight, 4);
										}
										if (data.m_postile.y < m_Map_Ysize - 1) {
											m_TileEdit.Data[data.Get_Tile(m_Map_Xsize, 0, 1)].SetGround(data.m_hight, 6);
										}
									}
								}
							}
							m_save = true;
						}
						else {
							if (m_save) {
								m_save = false;
								m_TileEdit.Save();
							}
						}
					}
					else {
						if (data.m_isWall) {
							COLOR_U8 Color1 = GetColorU8(mh, mh, 0u, 255u);
							COLOR_U8 Color2 = GetColorU8(mb, mb, 0u, 255u);
							DrawGradationBox(xs, ys, xe, ye, Color1, Color2, data.m_dir);

							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
							DrawExtend_wrap(Vector2D_I::Get(xs, ys), Vector2D_I::Get(xe, ye), &(m_TileEdit.floors[data.m_FloorID]));
							SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
						}
						else {
							{
								COLOR_U8 Color1 = GetColorU8(mh, mh, mh, 255u);
								COLOR_U8 Color2 = GetColorU8(mb, mb, mb, 255u);
								DrawGradationBox(xs, ys, xe, ye, Color1, Color2, data.m_dir - 4);

								SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
								DrawExtend_wrap(Vector2D_I::Get(xs, ys), Vector2D_I::Get(xe, ye), &(m_TileEdit.floors[data.m_FloorID]));
								SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
							}
							if (m_smz) {
								for (int i = 0; i < 4; i++) {
									int xsel = ((i == 0) ? 0 : (i == 1) ? 1 : (i == 2) ? 0 : -1);
									int ysel = ((i == 0) ? 1 : (i == 1) ? 0 : (i == 2) ? -1 : 0);
									const int xs2 = xpos_E + (data.m_postile.x + xsel) * tilesize_E;
									const int ys2 = ypos_E + (data.m_postile.y + ysel) * tilesize_E;
									const int xe2 = xpos_E + (data.m_postile.x + xsel + 1) * tilesize_E;
									const int ye2 = ypos_E + (data.m_postile.y + ysel + 1) * tilesize_E;
									if (in2_(m_mouse_x, m_mouse_y, xs2, ys2, xe2, ye2)) {
										COLOR_U8 Color1 = GetColorU8(mh, mh / 2, mh / 2, 255u);
										COLOR_U8 Color2 = GetColorU8(mb, mb / 2, mb / 2, 255u);
										DrawGradationBox(xs, ys, xe, ye, Color1, Color2, data.m_dir - 4);

										SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
										DrawExtend_wrap(Vector2D_I::Get(xs, ys), Vector2D_I::Get(xe, ye), &(m_TileEdit.floors[data.m_FloorID]));
										SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
										break;
									}
								}
							}
						}
					}

					if (data.m_isWall) {
						DrawBox(xs, ys, xe, ye, GetColor(0, 0, 0), FALSE);
					}
				}
				DrawCircle(xpos_E + m_TileEdit.mapdata.SP[0].x * tilesize_E / y_r(tilesize), ypos_E + m_TileEdit.mapdata.SP[0].y * tilesize_E / y_r(tilesize), tilesize_E, GetColor(0, 255, 0));
				for (auto& SP_t : m_TileEdit.PlayerSpawnPoint) {
					DrawCircle(xpos_E + SP_t.pos_p.x * tilesize_E / y_r(tilesize), ypos_E + SP_t.pos_p.y * tilesize_E / y_r(tilesize), tilesize_E2, GetColor(255, 0, 0));
				}
				for (auto& WP : m_TileEdit.m_way_point) {
					DrawCircle(xpos_E + WP.x * tilesize_E / y_r(tilesize), ypos_E + WP.y * tilesize_E / y_r(tilesize), tilesize_E2 / 2, GetColor(0, 255, 0));
				}
			}
			//壁か床か
			m_Buttons[6].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(80), x_r(400), y_r(40), "選択タイルを変更", true, [&]() { m_wallorfloor ^= 1; });
			GetFont2(y_r(40)).DrawString(m_Map_Xsize * tilesize_E2, y_r(80) + y_r(40), m_wallorfloor ? "壁を選択中" : "床を選択中", GetColor(255, 0, 0));
			//壁か床か
			m_Buttons[7].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(180), x_r(400), y_r(40), "地形編集", true, [&]() { m_smz ^= 1; });
			GetFont2(y_r(40)).DrawString(m_Map_Xsize * tilesize_E2, y_r(180) + y_r(40), m_smz ? "台形" : "矩形", GetColor(255, 0, 0));
			//床テクスチャ
			m_Buttons[8].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(280), x_r(400), y_r(40), "床テクスチャ選択", GetWindowModeFlag() == TRUE, [&]() {
				if (m_Dialog.Open()) {
					strcpy_s(m_TileEdit.mapdata.floor_name, m_Dialog.GetPath());
					for (auto& w : m_TileEdit.floors) {
						w.Dispose();
					}
					m_TileEdit.floors.clear();
					GraphHandle::LoadDiv(m_TileEdit.mapdata.floor_name, 256, 16, 16, 16, 16, &m_TileEdit.floors);
				}
			});
			GetFont2(y_r(30)).DrawString(m_Map_Xsize * tilesize_E2, y_r(280) + y_r(40), m_TileEdit.mapdata.floor_name, GetColor(255, 0, 0));
			{
				int xpos_E = DrawPts->disp_y / 40 + m_Map_Xsize * tilesize_E2 + x_r(400);
				int ypos_E = DrawPts->disp_y / 40 + x_r(100);// + x_r(280)
				int x_p = 0;
				int y_p = 0;
				int i = 0;
				for (auto& w : m_TileEdit.floors) {
					x_p = i % 16;
					y_p = i / 16;
					const int xs = xpos_E + (int)(x_p * tilesize_E * 2 / 3);
					const int ys = ypos_E + (int)(y_p * tilesize_E * 2 / 3);
					const int xe = xpos_E + (int)((x_p + 1) * tilesize_E * 2 / 3);
					const int ye = ypos_E + (int)((y_p + 1) * tilesize_E * 2 / 3);
					DrawExtend_wrap(Vector2D_I::Get(xs, ys), Vector2D_I::Get(xe, ye), &w);
					if (i == m_SelectFloorTex) {
						DrawBox(xs, ys, xe, ye, GetColor(255, 0, 0), FALSE);
					}
					if (in2_(m_mouse_x, m_mouse_y, xs, ys, xe, ye)) {
						if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
							m_SelectFloorTex = i;
						}
					}
					i++;
				}
				{
					const int xs = xpos_E + (int)(0 * tilesize_E * 2 / 3);
					const int ys = ypos_E + (int)(0 * tilesize_E * 2 / 3);
					const int xe = xpos_E + (int)(16 * tilesize_E * 2 / 3);
					const int ye = ypos_E + (int)(16 * tilesize_E * 2 / 3);
					DrawBox(xs, ys, xe, ye, GetColor(255, 0, 0), FALSE);
				}
			}
			//壁テクスチャ
			m_Buttons[9].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(380), x_r(400), y_r(40), "壁テクスチャ選択", GetWindowModeFlag() == TRUE, [&]() {
				if (m_Dialog.Open()) {
					strcpy_s(m_TileEdit.mapdata.wall_name, m_Dialog.GetPath());
					for (auto& w : m_TileEdit.m_walls) {
						w.Dispose();
					}
					m_TileEdit.m_walls.clear();
					GraphHandle::LoadDiv(m_TileEdit.mapdata.wall_name, 32, 16, 2, 16, 16 * 2, &m_TileEdit.m_walls);
				}
			});
			GetFont2(y_r(30)).DrawString(m_Map_Xsize * tilesize_E2, y_r(380) + y_r(40), m_TileEdit.mapdata.wall_name, GetColor(255, 0, 0));
			{
				int xpos_E = DrawPts->disp_y / 40 + m_Map_Xsize * tilesize_E2 + x_r(400);
				int ypos_E = DrawPts->disp_y / 40 + y_r(380);
				int x_p = 0;
				int y_p = 0;
				int i = 0;
				for (auto& w : m_TileEdit.m_walls) {
					x_p = i % 16;
					y_p = i / 16;
					const int xs = xpos_E + (int)(x_p * tilesize_E * 2 / 3);
					const int ys = ypos_E + (int)(y_p * tilesize_E * 2 / 3 * 2);
					const int xe = xpos_E + (int)((x_p + 1) * tilesize_E * 2 / 3);
					const int ye = ypos_E + (int)((y_p + 1) * tilesize_E * 2 / 3 * 2);
					DrawExtend_wrap(Vector2D_I::Get(xs, ys), Vector2D_I::Get(xe, ye), &w);
					if (i == m_SelectWallTex) {
						DrawBox(xs, ys, xe, ye, GetColor(255, 0, 0), FALSE);
					}
					if (in2_(m_mouse_x, m_mouse_y, xs, ys, xe, ye)) {
						if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
							m_SelectWallTex = i;
						}
					}
					i++;
				}
				{
					const int xs = xpos_E + (int)(0 * tilesize_E * 2 / 3);
					const int ys = ypos_E + (int)(0 * tilesize_E * 2 / 3 * 2);
					const int xe = xpos_E + (int)(16 * tilesize_E * 2 / 3);
					const int ye = ypos_E + (int)(2 * tilesize_E * 2 / 3 * 2);
					DrawBox(xs, ys, xe, ye, GetColor(255, 0, 0), FALSE);
				}
			}
			//設定する高さ
			{
				auto cam_high = (int)((float)m_caminfo.camhigh_base / m_caminfo.camzoom);
				//高
				m_Buttons[10].UpDownSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(480), "設定する高さ : " + std::to_string(m_hight_s), true,
					[&]() { m_hight_s = std::min(m_hight_s + 8, cam_high); },
					[&]() { m_hight_s = std::max(m_hight_s - 8, -cam_high); });
				m_bottom_s = std::min(m_bottom_s, m_hight_s - 8);
				//底面
				m_Buttons[11].UpDownSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(580 + 15), "設定する底面 : " + std::to_string(m_bottom_s), true,
					[&]() {
					if (m_bottom_s < cam_high - 8) { m_hight_s = std::max(m_bottom_s + 8, m_hight_s); }
					else { m_bottom_s = cam_high; }
					m_bottom_s = std::min(m_bottom_s + 8, cam_high);
				},
					[&]() { m_bottom_s = std::max(m_bottom_s - 8, -cam_high); });
			}
			m_Buttons[4].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(680), x_r(100), y_r(40), "戻る", m_TileEdit.CanUndo(), [&]() { m_TileEdit.Undo(); });				//アンドゥ
			m_Buttons[5].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2 + x_r(150), y_r(680), x_r(100), y_r(40), "進む", m_TileEdit.CanRedo(), [&]() { m_TileEdit.Redo(); });		//リドゥ
			bool end_f = true;
			m_Buttons[2].ButtonSet(m_mouse_x, m_mouse_y, x_r(1920 - 340), y_r(1080 - 160), x_r(300), y_r(40), "保存せず終了", true, [&]() { m_isend = true; end_f = false; });		//終了
			m_Buttons[3].ButtonSet(m_mouse_x, m_mouse_y, x_r(1920 - 340), y_r(1080 - 80), x_r(300), y_r(40), "保存して続行", true, [&]() { end_f = false; });						//終了
			if (!end_f) {
				m_TileEdit.Dispose();
			}
			return end_f;
		}
		void Init_Window3() noexcept {
			m_Map_Xsize = 40;
			m_Map_Ysize = 40;
			m_Buttons[10].Init();
			m_Buttons[11].Init();
			m_Buttons[3].Init();
		}
		bool Window3() noexcept {
			DrawBox(x_r(960 - 320), y_r(540 - 180), x_r(960 + 320), y_r(540 + 180), GetColor(128, 128, 128), TRUE);
			GetFont2(y_r(40)).DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "マップのサイズは?", GetColor(255, 255, 0));
			//高
			m_Buttons[10].UpDownSet(m_mouse_x, m_mouse_y, x_r(960 - 320 + 40), y_r(540 - 180 + 60 + 100), "X : " + std::to_string(m_Map_Xsize), true,
				[&]() { m_Map_Xsize++; },
				[&]() { if (m_Map_Xsize > 1) { m_Map_Xsize--; } });
			//底面
			m_Buttons[11].UpDownSet(m_mouse_x, m_mouse_y, x_r(960 - 320 + 40), y_r(540 - 180 + 60 + 100 + 115), "Y : " + std::to_string(m_Map_Ysize), true,
				[&]() { m_Map_Ysize++; },
				[&]() { if (m_Map_Ysize > 1) { m_Map_Ysize--; }});
			{
				int xsz = x_r(280);
				int ysz = y_r(120);
				int xm = x_r(1100);
				int ym = y_r(540);
				if (m_Map_Xsize * ysz / xsz >= m_Map_Ysize) {
					ysz = xsz * m_Map_Ysize / m_Map_Xsize;
				}
				else {
					xsz = ysz * m_Map_Xsize / m_Map_Ysize;
				}
				DrawBox(xm - xsz / 2, ym - ysz / 2, xm + xsz / 2, ym + ysz / 2, GetColor(255, 255, 0), FALSE);
			}
			//終了
			bool end_f = true;
			m_Buttons[3].ButtonSet(m_mouse_x, m_mouse_y, x_r(960 + 320 - 340), y_r(540 + 180 - 80), x_r(300), y_r(40), "OK", true, [&]() { end_f = false; });
			return end_f;
		}
	public:
		//エディター用関数
		bool Map_Editer(std::string _mapname) {
			Init_Editer();
			//map_data選択
			{
				Init_Window1();
				while (ProcessMessage() == 0) {
					GetMousePoint(&m_mouse_x, &m_mouse_y);
					GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
					if (!Window1()) { break; }
					DrawPts->Screen_Flip();
				}
				if (!m_isread) {
					//map読み込み
					m_TileEdit.Read("data/Map/" + _mapname + "/1.dat", "data/Map/" + _mapname + "/2.dat", "data/Map/" + _mapname + "/3.dat", "data/Map/" + _mapname + "/4.dat");
					m_Map_Xsize = 0;
					m_Map_Ysize = 0;
					for (auto& d : m_TileEdit.Data) {
						m_Map_Xsize = std::max(d.m_postile.x, m_Map_Xsize);
						m_Map_Ysize = std::max(d.m_postile.y, m_Map_Ysize);
					}
					m_Map_Xsize++;
					m_Map_Ysize++;
				}
				else {
					//mapデータプリセット
					Init_Window3();
					while (ProcessMessage() == 0) {
						GetMousePoint(&m_mouse_x, &m_mouse_y);
						GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
						if (!Window3()) { break; }
						DrawPts->Screen_Flip();
					}
					m_TileEdit.PreSet(m_Map_Xsize, m_Map_Ysize);
				}
			}
			//エディター
			{
				Init_Window2();
				while (ProcessMessage() == 0) {
					GetMousePoint(&m_mouse_x, &m_mouse_y);
					GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
					if (!Window2()) { break; }
					DrawPts->Screen_Flip();
				}
				if (m_isend) { return false; }
				m_TileEdit.List.clear();
			}
			m_TileEdit.Write("data/Map/" + _mapname + "/1.dat", "data/Map/" + _mapname + "/2.dat", "data/Map/" + _mapname + "/3.dat", "data/Map/" + _mapname + "/4.dat");			//mapデータ書き込み
			return true;
		}
		/*--ゲーム内使用--*/
	private:
		//Near3D用カメラ情報
		class Camera_Info {
		public:
			const int camhigh_base = 192;	//カメラの高さ
			Vector2D_I camerapos = { 0,0 };
			float camzoom = 1.f;
		};
		class Tiles : public TileStat_Common {
		public:
			unsigned char m_dir = 0;// rect = 255 else prism = 0~3,4~7
		public:
			std::array<Vector2D_I, 4> m_top;
			std::array<Vector2D_I, 4> m_floor;
			std::array<Vector2D_I, 4> m_zero;
			GraphHandle* wallhandle;
			GraphHandle* floorhandle;
			const bool GetXIn(int _x) const noexcept { return _x >= m_top[0].x && _x <= m_top[3].x; }
			const bool GetYIn(int _y) const noexcept { return _y >= m_top[0].y && _y <= m_top[3].y; }
		};
	private:
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
		static Vector2D_I ConvertPos_CalcCam(const Vector2D_I& _pos, int _hight, const Camera_Info& _caminfo) noexcept {
			return ConvertPos((_pos + _caminfo.camerapos) * _caminfo.camzoom, _hight, _caminfo);
		}
		//Near3D用サウンド
		static void PlaySound_Near3D(ENUM_SOUND _SoundID, const Vector2D_I& _pos, const Camera_Info& _caminfo, int _Vol = 255) noexcept {
			auto DispPos = ConvertPos_CalcCam(_pos, 0, _caminfo);
			auto Distance = (int)sqrt((DispPos - Vector2D_I::Get(deskx / 2, desky / 2)).hydist());
			SE.Get((int)_SoundID).Play(0, DX_PLAYTYPE_BACK, TRUE, std::clamp(_Vol - (_Vol / 2) * Distance / (deskx / 2), 0, 255), std::clamp(255 * (DispPos.x - (deskx / 2)) / (deskx / 2), -255, 255));
		}
		//球と壁の判定(接触箇所取得)
		static bool GetHitWall_GetHit(const std::vector<std::vector<Tiles>>& _tile, Vector2D_I* _pos, const Vector2D_I& _oldpos, int _radius) noexcept {
			//抜け対策
			bool isHit = false;
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
						if (GetCollisionSegment2_GetHit(_pos, _oldpos, s1, s0)) {
							isHit = true;
						}
						if (GetCollisionSegment2_GetHit(_pos, _oldpos, s0, s2)) {
							isHit = true;
						}
						if (GetCollisionSegment2_GetHit(_pos, _oldpos, s2, s3)) {
							isHit = true;
						}
						if (GetCollisionSegment2_GetHit(_pos, _oldpos, s3, s1)) {
							isHit = true;
						}
					}
				}
			}
			return isHit;
		}
		//球と壁の判定(ずり)
		static bool GetHitWall(const std::vector<std::vector<Tiles>>& _tile, Vector2D_I* _pos, const Vector2D_I& _oldpos, int _radius) noexcept {
			_pos->x = std::clamp(_pos->x, _radius, y_r(tilesize) * (int)(_tile.size()) - _radius);
			_pos->y = std::clamp(_pos->y, _radius, y_r(tilesize) * (int)(_tile.back().size()) - _radius);
			//抜け対策
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
							if (GetCollisionSegment2(_pos, _oldpos, s1, s0)) {
								isHit = true;
								break;
							}
							if (GetCollisionSegment2(_pos, _oldpos, s0, s2)) {
								isHit = true;
								break;
							}
							if (GetCollisionSegment2(_pos, _oldpos, s2, s3)) {
								isHit = true;
								break;
							}
							if (GetCollisionSegment2(_pos, _oldpos, s3, s1)) {
								isHit = true;
								break;
							}
						}
					}
				}
			}
			return isHit;
		}
		//球と壁の判定(当たりだけ)
		static bool GetHitWall(const std::vector<std::vector<Tiles>>& _tile, const Vector2D_I& _pos, const Vector2D_I& _oldpos, int _radius) noexcept {
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
						if (GetCollisionSegment2(_pos, _oldpos, s1, s0)) {
							return true;
						}
						if (GetCollisionSegment2(_pos, _oldpos, s0, s2)) {
							return true;
						}
						if (GetCollisionSegment2(_pos, _oldpos, s2, s3)) {
							return true;
						}
						if (GetCollisionSegment2(_pos, _oldpos, s3, s1)) {
							return true;
						}
					}
				}
			}
			return false;
		}
		//タイル
		const Tiles& GetTile(const Vector2D_I& _pos) const noexcept { return m_Tile[_pos.x / y_r(tilesize)][_pos.y / y_r(tilesize)]; }
	private:
		//ゲーム内使用クラス
		class shadow_handle {
			bool m_isupdate = true;
			GraphHandle m_Handle;
		public:
			void Init(int _x, int _y) {
				this->m_Handle = GraphHandle::Make(_x, _y, true);
				this->m_isupdate = true;
			}
			auto& GetHandle() {
				this->m_isupdate = true;
				return this->m_Handle;
			}
			void SetDraw(std::function<void()> _doing) {
				if (this->m_isupdate) {
					this->m_isupdate = false;
					this->m_Handle.SetDraw_Screen();
					{
						_doing();
					}
				}
			}
		};
		class Object_Common {
		protected:
			int m_Base_Hight = 0;
			int m_hight = 0;
			float m_Yrad = 0.f;
			float m_speed = 0.f;
		public:
			Vector2D_I pos;
		protected:
			bool GetCanDraw(const Tiles& _Ti, const Vector2D_I& ConvertedPos) { return _Ti.GetXIn(ConvertedPos.x) && _Ti.GetYIn(ConvertedPos.y); }
			bool GetCanDraw_CheckHight(const Tiles& _Ti, const Vector2D_I& ConvertedPos) { return GetCanDraw(_Ti, ConvertedPos) && (_Ti.m_hight <= this->m_Base_Hight); }
		public:
			float Getyrad() { return m_Yrad; }
			int GetHight() { return m_hight; }
			void posSet(Object_Common* have_t) {
				this->pos = have_t->pos;
			}
			void UpdateSpeed(float SpeedSub) {
				float spd = this->m_speed * 60.f / FPS;
				this->pos += Vector2D_I::Get((int)(sin(this->m_Yrad)*spd), (int)(-cos(this->m_Yrad)*spd));
				if (this->m_speed > 0) {
					this->m_speed -= SpeedSub / FPS;
				}
				else {
					this->m_speed = 0.f;
				}
			}
		public:
			void SetHight(const Tiles& _Ti) {
				this->m_Base_Hight = _Ti.m_hight;
			}
			void Init_Common(Object_Common* have_t) {
				this->pos = have_t->pos;
				this->m_Base_Hight = have_t->m_Base_Hight;
				this->m_Yrad = have_t->m_Yrad;
				this->m_hight = have_t->m_hight;
			}
			void Draw_Common(const Tiles& _Ti, const Camera_Info& _caminfo, float _Yrad, const GraphHandle& graph_t, const Vector2D_I& pos_add) {
				auto P = ConvertPos_CalcCam(pos_add + this->pos, this->m_Base_Hight, _caminfo);
				if (GetCanDraw_CheckHight(_Ti, P)) {
					auto zh = this->m_Base_Hight + this->m_hight;
					auto ConvPos = ConvertPos_CalcCam(pos_add + this->pos, zh, _caminfo);
					auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
					Set_Bright(255 - 255 * std::clamp(zh, 0, cam_high) / cam_high);
					DrawRota_wrap(ConvPos, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom, _Yrad, graph_t, TRUE);
				}
			}
			void Draw_Shadow_Common(const Tiles& _Ti, float _light_yrad, float m_ShadowRange, const Camera_Info& _caminfo, float _Yrad, const GraphHandle& graph_t, const Vector2D_I& pos_add) {
				auto P = ConvertPos_CalcCam(pos_add + this->pos, this->m_Base_Hight, _caminfo);
				if (GetCanDraw(_Ti, P)) {
					auto zh = this->m_Base_Hight + this->m_hight;
					auto zh2 = float(zh - _Ti.m_hight) * _caminfo.camzoom*m_ShadowRange;
					auto P2 = ConvertPos_CalcCam(pos_add + this->pos + Vector2D_I::Get((int)(zh2 * sin(_light_yrad)), (int)(zh2 * cos(_light_yrad))), _Ti.m_hight, _caminfo);
					auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
					DrawRota_wrap(P2, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom, _Yrad, graph_t, TRUE);
				}
			}
			void Draw_Light_Common(const Tiles& _Ti, const Camera_Info& _caminfo, float size) {
				auto P = ConvertPos_CalcCam(this->pos, this->m_Base_Hight, _caminfo);
				if (GetCanDraw(_Ti, P)) {
					auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
					DrawCircle(P.x, P.y, (int)(float(this->m_Base_Hight + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom*size), GetColor(255, 255, 255), TRUE);
				}
			}
		};
		class Object_Vanish : public Object_Common {
		protected:
			float Time = 0.f;
		public:
			void UpdateTime() {
				this->Time = std::max(this->Time - 1.f / FPS, 0.f);
			}
			bool iaActive() { return this->Time > 0.f; }
			bool isEnd() { return this->Time <= 0.f; }
		};
		class Gun_Object;
		class Human_Object : public Object_Common {
			typedef std::pair<size_t, int> pairs;
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
				void SetDist(float m_xdist, float  m_ydist, float  m_zdist) {
					xdist = m_xdist * y_r(tilesize) / 32 * 5 / 6;
					ydist = m_ydist * y_r(tilesize) / 32 * 10;
					zdist = m_zdist;
				}
				void Leap_Rad(const Bonesdata& now, const Bonesdata& next, float per) {
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
				void Update_Parent(float y_rad_t, float yrad_aim, float yrad_aim_speed) {
					this->m_hight = 0;
					this->xr = 0;
					this->yr = y_rad_t + yrad_aim + yrad_aim_speed;
					this->zr = 0;
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
						this->m_hight = parentB.m_hight + (int)(cos(this->zr) * z1 - sin(this->zr) * this->xdist);
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
					void SetBoneData(Bone_Sel sel, std::string_view ttt, float rad) { this->bone[(int)sel].SetBoneData(ttt, rad); }
				};
			private:
				std::vector<std::vector<Animesdata>> anime;
			public:
				Animesdata* nowAnimData = nullptr, *nextAnimData = nullptr;
			private:
				int OldSel = 0, NowSel = 0;
				int NowFrame = 0;
				int Time = 1;
				int Time2 = 1;
				bool IsEnd = false;
			public:
				void SetSel(Anim_Sel nowsel_t) { NowSel = (int)nowsel_t; }
				int GetSel() { return NowSel; }
				bool isEnd() {
					if (this->IsEnd) {
						this->IsEnd = false;
						return true;
					}
					return false;
				}
			public:
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
						else if (ttt.find("left_hand_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::LEFTHAND, ttt, deg2rad(tmp)); }
						else if (ttt.find("left_arm2_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::LEFTARM2, ttt, deg2rad(tmp)); }
						else if (ttt.find("left_arm_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::LEFTARM1, ttt, deg2rad(tmp)); }
						else if (ttt.find("Body_Top_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::BODYTOP, ttt, deg2rad(tmp)); }
						else if (ttt.find("right_arm_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::RIGHTARM1, ttt, deg2rad(tmp)); }
						else if (ttt.find("right_arm2_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::RIGHTARM2, ttt, deg2rad(tmp)); }
						else if (ttt.find("right_hand_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::RIGHTHAND, ttt, deg2rad(tmp)); }
						else if (ttt.find("Body_Head_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::HEAD, ttt, deg2rad(tmp)); }
						else if (ttt.find("Body_Mid_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::BODYMIDDLE, ttt, deg2rad(tmp)); }
						else if (ttt.find("left_leg3_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::LEFTLEG3, ttt, deg2rad(tmp)); }
						else if (ttt.find("left_leg2_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::LEFTLEG2, ttt, deg2rad(tmp)); }
						else if (ttt.find("left_leg_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::LEFTLEG1, ttt, deg2rad(tmp)); }
						else if (ttt.find("Body_Bottom_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::BODYBOTTOM, ttt, deg2rad(tmp)); }
						else if (ttt.find("right_leg_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::RIGHTLEG1, ttt, deg2rad(tmp)); }
						else if (ttt.find("right_leg2_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::RIGHTLEG2, ttt, deg2rad(tmp)); }
						else if (ttt.find("right_leg3_") != std::string::npos) { this->anime.back().back().SetBoneData(Bone_Sel::RIGHTLEG3, ttt, deg2rad(tmp)); }
						else if (ttt.find("end") != std::string::npos) { break; }
					} while (true);
					FileRead_close(mdata);
				}
				void Update(std::vector<Bonesdata>* bone_base) {
					if (this->OldSel != this->NowSel) {
						this->NowFrame = 0;
						this->Time = 0;
						this->IsEnd = false;
						this->nowAnimData = this->nextAnimData;
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
						if (this->NowFrame == (int)(this->anime[this->NowSel].size()) - 1) {
							this->IsEnd = true;
						}
						this->Time = 0;
						this->nowAnimData = this->nextAnimData;
					}
				}
			};
			class FootprintControl {
				class foots : public Object_Vanish {
					Vector2D_I pos;
					Bone_Sel ID;
					float yr = 0.f;
					float yrad = 0.f;
					float Per = 1.f;
				public:
					void Set(const Vector2D_I& PlayerPos, std::vector<Bonesdata>& bone, Bone_Sel ID_t, float per) {
						this->ID = ID_t;
						this->pos = PlayerPos + bone[(int)this->ID].pos;
						this->yr = bone[(int)this->ID].yr;
						this->yrad = bone[(int)this->ID].yrad;
						this->Per = per;
						this->Time = 5.f;
					}
					void Draw(const Tiles& _Ti, const std::vector<GraphHandle>& _Graphs, const Camera_Info& _caminfo) {
						auto P = ConvertPos_CalcCam(this->pos, _Ti.m_hight, _caminfo);
						if (GetCanDraw(_Ti, P)) {
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp((int)(255.f* this->Per), 0, 255));
							auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
							DrawRota_wrap(P, float(_Ti.m_hight + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom, this->yrad + this->yr, _Graphs[(int)this->ID], TRUE);
						}
					}
				};
			private:
				Bone_Sel prev_foot = Bone_Sel::NUM;
				float CoolDown = 0;
				std::vector<foots> Footprint;
			public:
				void Update(const Vector2D_I& PlayerPos, std::vector<Bonesdata>& bone, Bone_Sel now_foot, bool is_Run, float m_Speed, const Camera_Info& _caminfo) {
					if (this->CoolDown >= 10.f / 60.f && this->prev_foot != now_foot) {
						this->Footprint.resize(this->Footprint.size() + 1);
						this->Footprint.back().Set(PlayerPos, bone, now_foot, 1.f - 1.f / std::max(m_Speed / 2.f, 1.f));
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
				void Draw(const Tiles& _Ti, const std::vector<GraphHandle>& _Graphs, const Camera_Info& _caminfo) {
					Set_Bright(0);
					for (auto& g : this->Footprint) {
						g.Draw(_Ti, _Graphs, _caminfo);
					}
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				}
			};
			class SWITCH {
			public:
				bool Press = false;
				bool Switch = false;
				bool Start = false;
				bool On = false;

				void First() {
					this->Start = true;
					this->On = true;
				}

				void End() {
					this->Start = false;
					this->On = false;
				}

				void Update() {
					if (this->On) {
						this->Start = false;
					}
				}
			};
			//AI用
			class AI {
				std::vector<int> wayp_pre{ 0 };
			public:
				void Init() noexcept {
					wayp_pre.resize(6);
				}
				void Spawn(int now)  noexcept {
					this->Fill(now);
				}
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
				const auto& GetFront() const noexcept { return wayp_pre.front(); }
				bool Compare(int _id) const noexcept {
					for (auto& ww : this->wayp_pre) { if (_id == ww) { return false; } }
					return true;
				}
			};
		private:
			std::vector<pairs> sort;
			std::vector<Bonesdata> bone;
			AnimeControl m_anime;
			FootprintControl m_Footprint;
		private:
			std::vector<GraphHandle> m_Graphs;
			std::vector<bool> draw_ok = { false };
			bool draw_end = false;
			float yrad_aim = 0;
			float yrad_aim_speed = 0;
			Vector2D_I spawnpos;
			Vector2D_I vec_real;//キャラスプライトが実際に向いている向き
			Vector2D_I vec_buf;//移動方向
			//ダメージ
			int DamageDown = false;
			SWITCH m_Damage;
			float MeleeDamageSpeed = 0;
			Human_Object* AttackHuman = nullptr;//当てた人
			//姿勢
			float changingtime = 0.f;
			float RollingSpeed = 0;
			SWITCH m_Rolling;		//ローリング
			SWITCH m_Shoot;			//射撃
			SWITCH m_Melee;			//近接
			float MeleeSpeed = 0;
			int  MeleeCnt = 0;
			SWITCH m_Reload;		//リロード
			float ZoomStopTime = 0;
			float ZoomBuf = 0.8f;
			bool isHitWall = false;//銃が壁の中にあるか
			bool changing = false;
			bool standup = false;
			bool isStand = true;		//立っているか
			bool isDown = false;		//ダウンしているか
			bool isRun = false;			//走っているか否か
			bool isReadyGun = false;	//銃を構えるか否か
			bool ishaveGStart = false;

			float isFoundEnemyTime = 0.f;
			int m_phase = 0;
			float transceiverTimer = 0.f;
		public:
			Gun_Object* haveGun = nullptr;
			AI cpu_do;

			bool transceiverStart = false;
			bool transceiverSwitch = false;
		private:
			const auto IsDamage() const noexcept { return AttackHuman != nullptr; }
		public:
			const auto isPhase() const noexcept { return m_phase; }
			const auto isAlart() const noexcept { return m_phase >= 3; }
			const auto isCaution() const noexcept { return m_phase == 2; }
			const auto isHaveGun() const noexcept { return haveGun != nullptr; }
			const auto isReloadStart() const noexcept { return m_Reload.Start; }
			const auto GetLookyrad() const noexcept { return m_Yrad + this->yrad_aim; }
			const auto Getvec_real(float _P) const noexcept { return this->vec_real * (_P / sqrtf((float)this->vec_real.hydist())); }
			const auto isMove() const noexcept { return this->vec_buf.x != 0 || this->vec_buf.y != 0; }//移動するか
			const auto& Get_vec_buf() const noexcept { return vec_buf; }
			const auto& GetRightHandInfo() const noexcept { return bone[(int)Bone_Sel::RIGHTHAND]; }
			const auto& GetBodyTopInfo() const noexcept { return bone[(int)Bone_Sel::BODYTOP]; }
			const auto& GetHeadInfo() const noexcept { return bone[(int)Bone_Sel::HEAD]; }
			const auto& GetBaseHight() const noexcept { return this->sort.front().second; }
			const auto& isDamageDown() const noexcept { return this->DamageDown; }
		public:
			auto GetCamZoom() {
				if (isMove()) {
					ZoomStopTime = 1.f;
					ZoomBuf = 0.8f + ((float)sqrt(this->vec_buf.hydist()) / this->GetSpeed()) *((this->isReadyGun) ? 0.15f : 0.3f);
				}
				else {
					if (ZoomStopTime > 0) {
						ZoomStopTime -= 1.f / FPS;
					}
					else {
						ZoomBuf = 0.8f;
					}
				}
				//printfDx("%05.2f \n", ZoomBuf);
				return ZoomBuf;
			}
			void SetStand(bool is_stand) {
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
			const Vector2D_I GetMeleePoint() {
				int t = -y_r(tilesize);
				Vector2D_I vec;//移動方向
				vec.set((int)(-sin(this->m_Yrad) * t), (int)(cos(this->m_Yrad) * t));
				return this->pos + vec;
			}
			void Damage(Human_Object*Attacker, const Camera_Info& _caminfo) {
				if (Attacker != nullptr) {
					if (!this->m_Damage.On) {
						this->m_Damage.First();
						this->AttackHuman = Attacker;
						this->DamageDown = (this->AttackHuman->MeleeCnt == 2);
						if (this->DamageDown) {
							this->MeleeDamageSpeed = -20.f;
							PlaySound_Near3D(ENUM_SOUND::DownHit, this->pos, _caminfo);
						}
						else {
							this->MeleeDamageSpeed = -5.f;
							PlaySound_Near3D(ENUM_SOUND::Hit, this->pos, _caminfo);
						}
					}
				}
			}
			bool MeleeHit(Human_Object*Attacker) {
				if (Attacker->m_Melee.Start) {
					if (!this->isDown) {
						Vector2D_I dist = this->pos - Attacker->GetMeleePoint();
						int t = y_r(tilesize);
						if (dist.hydist() <= t * t) {
							return true;
						}
					}
				}
				return false;
			}
			const float GetSpeed() {
				if (!this->isStand) {
					if (this->m_Reload.On) {
						m_speed = 0.f;
					}
					else {
						m_speed = (this->isRun) ? 6.f : 1.f;
					}
				}
				else {
					m_speed = (this->isRun) ? 6.f : 3.f;
				}
				return m_speed;
			}
			void SetKey(const bool Aim, const bool shoot, const bool reload, const bool rolling, const Camera_Info& _caminfo) {
				if (!this->m_Melee.On) {
					this->m_Reload.Press = (Aim && isHaveGun()) & reload;	//リロード
					bool OLD = this->isReadyGun;
					this->isReadyGun = (Aim && isHaveGun());					//エイム
					if (OLD != this->isReadyGun) {
						PlaySound_Near3D(ENUM_SOUND::Equip, this->pos, _caminfo);
					}
					OLD = this->m_Rolling.Press;
					this->m_Rolling.Press = rolling;		//ローリング
					if (this->m_Rolling.Press && this->m_Rolling.Press != OLD) {
						PlaySound_Near3D(ENUM_SOUND::rolling, this->pos, _caminfo, 255);
					}
				}
				this->m_Shoot.Press = shoot;			//射撃キー押し
			}
			void Reset() {
				std::fill<>(this->draw_ok.begin(), this->draw_ok.end(), false);
				this->draw_end = false;
			}
			void SetGun(Gun_Object* haveGun_t) {
				if (this->isHaveGun()) {
					this->haveGun->SetHuman(nullptr);
				}
				this->haveGun = haveGun_t;
				if (this->isHaveGun()) {
					this->ishaveGStart = true;
					this->haveGun->SetHuman(this);
				}
			}
			void Update_Human(std::vector<std::vector<Tiles>>& _tile, const Vector2D_I&  Vec, Vector2D_I Aim, bool is_Run, int lowestphase) {
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
					Aim == Vector2D_I::Get(0, 0)
					) {
					Limit = 0;
				}
				if (this->IsDamage()) {
					float vecrange = 100000;//intで保持しているためvecrange倍
					Aim = this->AttackHuman->pos*vecrange - this->pos*vecrange;
				}
				float rad = std::atan2f((float)Aim.x, (float)-Aim.y) - this->m_Yrad;
				rad = std::atan2f(sin(rad), cos(rad));
				this->yrad_aim_speed = this->yrad_aim;
				easing_set(&this->yrad_aim, std::clamp(rad, deg2rad(-Limit), deg2rad(Limit)), 0.9f);
				if (this->m_Melee.Start || this->m_Damage.Start) {
					this->m_Yrad = this->m_Yrad + rad;
				}
				this->yrad_aim_speed = (this->yrad_aim - this->yrad_aim_speed)*FPS*0.1f;
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
				GetHitWall(_tile, &this->pos, OLDpos, y_r(tilesize) * 19 / 40);
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
					isHit = GetHitWall(_tile, NOW, OLD, 10);
				}
				this->isHitWall = this->isReadyGun && isHit;
				this->m_phase = std::max(this->m_phase, lowestphase);
				if (this->m_phase > lowestphase && !this->transceiverStart) {
					this->isFoundEnemyTime -= 1.f / FPS;
					if (this->isFoundEnemyTime < 0.f) {
						this->m_phase--;
						this->isFoundEnemyTime = 1.f;
					}
				}
				if (this->transceiverStart) {
					this->transceiverTimer -= 1.f / FPS;
					if (this->transceiverTimer < 0.f) {
						this->transceiverStart = false;
					}
				}
			}
			void EnableFoundEnemy() {
				this->m_phase = 4;
				this->isFoundEnemyTime = 1.f;
			}
			void EnableTransceiver() {
				this->transceiverStart = true;
				this->transceiverTimer = 5.f;
			}

			void LookGun() {
				this->ishaveGStart = true;
			}
			void Draw_GunUp(int _x, int _y) {
				if (this->isHaveGun()) {
					if (this->ishaveGStart) {
						this->ishaveGStart = false;
						{
							this->haveGun->UpStart();
						}
					}
					this->haveGun->Draw_Up(_x, _y);
				}
			}
			void Draw_UI(std::vector<std::vector<Tiles>>& _tile, const Camera_Info& _caminfo) {
				auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
				auto zh = this->m_Base_Hight + (this->GetHeadInfo().m_hight - this->sort.front().second);
				auto ConvPos = ConvertPos_CalcCam(this->pos, zh, _caminfo);

				GetFont(y_r(25)).DrawStringFormat(ConvPos.x + y_r(12), ConvPos.y + y_r(12), GetColor(255, 0, 0), "Phase : %d", this->m_phase);

				auto Color = isAlart() ? GetColor(255, 0, 0) : (isCaution() ? GetColor(255, 255, 0) : GetColor(0, 0, 255));
				int Range_I = -(isCaution() ? y_r(tilesize * 10) : y_r(tilesize * 7));
				float Range = float(zh + cam_high) / cam_high * (float)(Range_I)* _caminfo.camzoom;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
				for (int rad = -180; rad <= 180; rad++) {
					float RAD0 = GetLookyrad() + deg2rad((float)rad * EyeRad / 180);
					float RAD05 = GetLookyrad() + deg2rad(((float)rad + 0.5f) * EyeRad / 180);
					float RAD1 = GetLookyrad() + deg2rad(((float)rad + 1.f) * EyeRad / 180);
					Vector2D_I V2 = Vector2D_I::Get((int)(-sin(RAD0)*Range), (int)(cos(RAD0)*Range));
					Vector2D_I V3 = Vector2D_I::Get((int)(-sin(RAD1)*Range), (int)(cos(RAD1)*Range));
					Vector2D_I Buf = this->pos + Vector2D_I::Get((int)(-sin(RAD05)*(float)(Range_I)), (int)(cos(RAD05)*(float)(Range_I)));
					if (GetHitWall_GetHit(_tile, &Buf, this->pos, 0)) {
						float Range_buf = (float)(zh + cam_high) / (float)cam_high * (float)(-sqrt((Buf - this->pos).hydist())) * _caminfo.camzoom;
						V2 = Vector2D_I::Get((int)(-sin(RAD0)*Range_buf), (int)(cos(RAD0)*Range_buf));
						V3 = Vector2D_I::Get((int)(-sin(RAD1)*Range_buf), (int)(cos(RAD1)*Range_buf));
					}
					Draw_Triangle(ConvPos, ConvPos + V2, ConvPos + V3, Color, true);
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
		public:
			//開始
			void Init(const Vector2D_I& p_s, int _sel) {
				GraphHandle::LoadDiv("data/Char/" + std::to_string(_sel) + ".bmp", 33, 11, 3, 32, 32, &this->m_Graphs);
				this->sort.resize(this->m_Graphs.size());
				this->draw_ok.resize(this->m_Graphs.size());
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
						n_t.back().SetDist(0.0f, 0.0f, -4.5f);
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
					file.open("data/Char/" + std::to_string(_sel) + ".dat", std::ios::binary | std::ios::in);
					do {
						this->bone.resize(this->bone.size() + 1);
						file.read((char*)&this->bone.back(), sizeof(this->bone.back()));
					} while (!file.eof());
					this->bone.pop_back();
					file.close();
				}
				for (int i = 0; i < (int)Anim_Sel::NUM; i++) {
					this->m_anime.LoadAnime("data/Char/Mot/" + std::to_string(i) + ".mot");
				}

				this->spawnpos = p_s;
				this->pos = this->spawnpos;
				this->m_Yrad = deg2rad(90);

				int vecrange = 100000;//intで保持しているためvecrange倍
				this->vec_real.set((int)(-sin(this->m_Yrad) * vecrange), (int)(cos(this->m_Yrad) * vecrange));
				this->cpu_do.Init();

				this->transceiverSwitch = false;
			}
			//更新
			void Update(const Camera_Info& _caminfo) {
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
				for (auto& g : this->bone) { g.edit = false; }
				this->m_anime.Update(&this->bone);
				//その他更新
				if (!this->IsDamage()) {
					//ローリング
					if ((this->isStand && this->m_Rolling.Press) && !this->m_Rolling.On) {
						this->RollingSpeed = 15.f;
						this->m_Rolling.First();
					}
					if (!this->m_Rolling.On && this->changing) {
						this->changingtime += 1.f / FPS;
						if (this->changingtime >= 0.2f) {
							this->changingtime = 0.f;
							this->changing = false;
						}
					}
				}
				this->m_Melee.Update();
				if (this->m_Melee.On) {
					if (this->m_anime.isEnd()) {
						if (this->MeleeCnt < 2) {
							this->MeleeCnt++;
							this->MeleeSpeed = 5.f;
							if (this->MeleeCnt == 2) {
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
				}
				if (this->m_Rolling.On) {
					if (this->m_anime.isEnd()) {
						this->m_Rolling.End();
					}
				}
				if (this->m_Reload.On) {
					if (this->m_Rolling.On) {
						this->m_Reload.End();//ローリングでリロキャン
						PlaySound_Near3D(ENUM_SOUND::MagDown0, this->pos, _caminfo);
					}
					if (!this->isReadyGun) {
						this->m_Reload.End();
						PlaySound_Near3D(ENUM_SOUND::MagDown0, this->pos, _caminfo);
					}
					else if (this->m_anime.isEnd()) {
						this->haveGun->ReloadEnd();
						this->m_Reload.End();
						PlaySound_Near3D(ENUM_SOUND::MagDown0, this->pos, _caminfo);
					}
				}
				this->m_Damage.Update();
				if (this->m_Damage.On) {
					if (!this->DamageDown) {//19
						if (this->m_anime.isEnd()) {
							this->AttackHuman = nullptr;
							this->m_Damage.End();
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
					vec.set((int)(-sin(this->m_Yrad) * vecrange), (int)(cos(this->m_Yrad) * vecrange));
					auto Range = (int)(sqrt(this->vec_real.hydist()) * vecrange);
					auto Cross = this->vec_real.cross(vec);
					auto Dot = this->vec_real.dot(vec);
					if (Cross > sin(deg2rad(10)) * Range) {
						this->m_Yrad -= deg2rad(5);
					}
					else if (Cross < sin(deg2rad(10)) * -Range) {
						this->m_Yrad += deg2rad(5);
					}
					//真後ろ振り向き
					if (Dot <= -0.5f * Range) {
						this->m_Yrad += deg2rad(10);
						if (this->m_Rolling.On) {
							this->m_Yrad += deg2rad(40);
						}
					}
				}
				//足跡座標指定
				bool next = false;
				do {
					next = false;
					for (auto& bo : this->bone) {
						if (!bo.edit) {
							if (bo.parent == -1) {
								bo.Update_Parent(this->m_Yrad, this->yrad_aim, this->yrad_aim_speed);
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
				for (int i = 0; i < this->sort.size(); i++) { this->sort[i] = { i, this->bone[i].m_hight }; }
				std::sort(this->sort.begin(), this->sort.end(), [](const pairs& x, const pairs& y) { return x.second < y.second; });
				//一番低い場所に跡を置く
				this->m_Footprint.Update(this->pos, this->bone, (Bone_Sel)(this->sort.front().first), this->isRun, this->GetSpeed(), _caminfo);
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
						if (this->haveGun->UpEnable()) {
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
							if (!this->m_Reload.On) {
								if (this->m_Reload.Press) {
									if (this->m_Reload.Switch) {
										this->m_Reload.Switch = false;
										this->m_Reload.First();
										PlaySound_Near3D(ENUM_SOUND::MagDown0, this->pos, _caminfo);
									}
									else {
										this->m_Reload.Start = false;
									}
								}
								else {
									this->m_Reload.Switch = true;
									this->m_Reload.Start = false;
								}
							}
							else {
								this->m_Reload.Switch = true;
								this->m_Reload.Start = false;
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
			void Draw(const Tiles& _Ti, const Camera_Info& _caminfo) {
				this->m_Footprint.Draw(_Ti, this->m_Graphs, _caminfo);
				if (!this->draw_end) {
					bool t = true;
					for (auto& g : this->sort) {
						auto& b = this->bone[g.first];
						auto pos_m = b.pos;
						auto hight_m = b.m_hight - this->sort.front().second;
						auto P = ConvertPos_CalcCam(pos_m + this->pos, this->m_Base_Hight, _caminfo);
						if (GetCanDraw_CheckHight(_Ti, P)) {
							this->draw_ok[g.first] = true;
							auto cam_high = (int)((float)_caminfo.camhigh_base / _caminfo.camzoom);
							auto zh = this->m_Base_Hight + hight_m;
							auto ConvPos = ConvertPos_CalcCam(pos_m + this->pos, zh, _caminfo);
							Set_Bright(255 - 255 * std::clamp(zh, 0, cam_high) / cam_high);
							DrawRota_wrap(ConvPos, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * _caminfo.camzoom, b.yrad + b.yr, this->m_Graphs[g.first], TRUE);
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
			void Draw_Shadow(const Tiles& _Ti, float _light_yrad, float _ShadowRange, const Camera_Info& _caminfo) {
				for (auto& g : this->sort) {
					auto& b = this->bone[g.first];
					this->m_hight = b.m_hight - this->sort.front().second;
					Draw_Shadow_Common(_Ti, _light_yrad, _ShadowRange, _caminfo, b.yrad + b.yr, this->m_Graphs[g.first], b.pos);
				}
			}
		};
		class Gun_Object : public Object_Common {
		private:
			class Ammo : public Object_Vanish {
			private:
				bool isHitWall = false;
				bool isHitHuman = false;
				Gun_Object* haveGun = nullptr;
				Object_Common Hit;
				float Time_ShotFlash = 100.f;
			public:
				bool isHit() { return this->isHitWall || this->isHitHuman; }
				//ヒットエフェクト
				void Set_Hit(bool isWallHit) {
					if (isWallHit) {
						this->isHitWall = true;		//無機質
					}
					else {
						this->isHitHuman = true;	//人体
					}
				}
				//人との当たり判定
				bool GetHitHuman(Human_Object& pl) { return &pl != haveGun->haveHuman && (pl.pos - this->pos).hydist() < ((y_r(tilesize) / 4)*(y_r(tilesize) / 4)); }
			public:
				void Init(Gun_Object* haveGun_t) {
					if (haveGun_t != nullptr) {
						haveGun = haveGun_t;
						this->Init_Common(haveGun);

						this->isHitWall = false;
						this->isHitHuman = false;
						this->Time_ShotFlash = 0.f;
						this->Hit.Init_Common(this);

						this->Time = 5.f;
						this->m_speed = 35.f;
					}
				}
				void Update(std::vector<std::vector<Tiles>>& _tile, const Camera_Info& _caminfo) {
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
					if (GetHitWall(_tile, this->pos, buf, (int)(this->m_speed / 2))) {
						this->Set_Hit(true);
					}
				}
				void Draw(const Tiles& _Ti, const Camera_Info& _caminfo) {
					//
					{
						if (!this->isHitWall && !this->isHitHuman) {
							Draw_Common(_Ti, _caminfo, this->m_Yrad, this->haveGun->m_Graphs[0], Vector2D_I::Get(0, 0));
						}
						else {
							if (this->isHitWall) {
								int Cnt = std::clamp(10 + (int)(this->Time_ShotFlash / 0.2f * 4.f), 10, 13);
								//Cnt = 9;
								Draw_Common(_Ti, _caminfo, this->m_Yrad + deg2rad(180), this->haveGun->m_Graphs[Cnt], Vector2D_I::Get(0, 0));
							}
							if (this->isHitHuman) {

							}
						}
					}
				}
				void Draw_Light(const Tiles& _Ti, const Camera_Info& _caminfo) {
					//閃光
					if (this->Time >= 5.f - 0.05f) {
						this->Hit.Draw_Light_Common(_Ti, _caminfo, 100.f);
					}
				}
			};
			class Cart : public Object_Vanish {
			private:
				Gun_Object* haveGun = nullptr;
				float Time_ShotFlash = 100.f;
				float hight_f = 0;
				float hight_add = 0;
				float y_rad_G = 0.f;
				bool isFirstDown = false;
			public:
				void Set_Hit() {
					this->Time = 0.f;
				}
			public:
				void Init(Gun_Object* haveGun_t) {
					if (haveGun_t != nullptr) {
						haveGun = haveGun_t;
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
				void Update(std::vector<std::vector<Tiles>>& _tile, const Camera_Info& _caminfo) {
					auto buf = this->pos;
					this->UpdateSpeed(0.3f);
					this->UpdateTime();
					{
						this->hight_f -= this->hight_add*60.f / FPS;
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
					if (GetHitWall(_tile, this->pos, buf, (int)(this->m_speed / 2))) {
						this->Set_Hit();
						if (this->isFirstDown) {
							this->isFirstDown = false;
							PlaySound_Near3D(ENUM_SOUND::Case0, this->pos, _caminfo);
						}
					}
				}
				void Draw(const Tiles& _Ti, const Camera_Info& _caminfo) {
					Draw_Common(_Ti, _caminfo, this->y_rad_G, this->haveGun->m_Graphs[9], Vector2D_I::Get(0, 0));
				}
				void Draw_Shadow(const Tiles& _Ti, float _light_yrad, float _ShadowRange, const Camera_Info& _caminfo) {
					Draw_Shadow_Common(_Ti, _light_yrad, _ShadowRange, _caminfo, this->y_rad_G, this->haveGun->m_Graphs[9], Vector2D_I::Get(0, 0));
				}
			};
			std::vector<GraphHandle> m_Graphs;
			//
			GraphHandle m_FirstUp;
			GraphHandle m_FirstUp_Empty;
			int m_Xsize, m_Ysize;
			float m_xrand_b, m_yrand_b, m_rrand_b;
			float m_xrand, m_yrand, m_rrand;
			float m_ymove;
			float m_moveDowntime;
			bool m_moveStart;
			bool m_moveUp;
			//
		public:
			Human_Object* haveHuman = nullptr;
		private:
			Vector2D_I Recoilpos;
			float Recoil = 0.f;
			float RecoilCnt = 0.f;
			float ShotTime = 0.f;
			float Time_ShotFlash = 100.f;
			Object_Common Flash;
			int AmmoCnt = 0;
			bool inChamber = true;
			GUN_SELECT Select = GUN_SELECT::SEMI;
			int Sel_C = 0;
			float hight_f = 0;
			float hight_add = 0;
		private:
			int AmmoCntMax = 10;
			std::string Name;
			std::vector<GUN_SELECT> CanUseSelect;
			std::string AmmoType;
			float NextShotTime;
			std::string Info;
		public:
			std::vector<Ammo> ammo;
			std::vector<Cart> cart;
			bool isDraw = true;
		public:
			const auto& GetMagGraph() { return this->m_Graphs[14]; }
			void SetHuman(Human_Object* haveHuman_t) { haveHuman = haveHuman_t; }
			void ReloadEnd() {
				AmmoCnt = AmmoCntMax;
				inChamber = true;
			}
			bool Shoot(const Camera_Info& _caminfo) {
				if (AmmoCnt >= 0 && inChamber) {
					ammo.resize(ammo.size() + 1);
					ammo.back().Init(this);
					cart.resize(cart.size() + 1);
					cart.back().Init(this);
					Recoil = 10.f;
					RecoilCnt = 10.f;
					ShotTime = this->NextShotTime;
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
			bool canshoot() { return this->Select != GUN_SELECT::SEMI && ShotTime == 0.f; }

			void ChangeSel() {
				++Sel_C %= this->CanUseSelect.size();
				this->Select = this->CanUseSelect[Sel_C];
			}

			bool UpEnable() { return this->m_moveStart; }
			void UpStart() {
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
			void Draw_Up(int _x, int _y) {
				if (this->m_moveStart) {
					if (this->inChamber) {
						this->m_FirstUp.DrawRotaGraph(_x / 2 + (int)this->m_xrand, _y / 2 + (int)this->m_yrand + (int)(this->m_ymove*_y), (float)_y / m_Ysize, deg2rad(-30)*this->m_ymove + deg2rad(this->m_rrand), true);

					}
					else {
						this->m_FirstUp_Empty.DrawRotaGraph(_x / 2 + (int)this->m_xrand, _y / 2 + (int)this->m_yrand + (int)(this->m_ymove*_y), (float)_y / m_Ysize, deg2rad(-30)*this->m_ymove + deg2rad(this->m_rrand), true);
					}

					int XP = _x * 32 / 1920;
					int YP = _y * 960 / 1080;

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(255 - (int)(255.f*this->m_ymove), 0, 255));
					GetFont(y_r(40)).DrawString(XP, YP, this->Name, GetColor(216, 216, 216), GetColor(64, 64, 64)); YP += y_r(40);
					if (this->inChamber) {
						float rad = deg2rad(90 - 90 * AmmoCnt / AmmoCntMax);
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

					easing_set(&this->m_xrand_b, GetRandf((float)(_x / 20)), 0.995f);
					easing_set(&this->m_yrand_b, (float)(_y / 20) + GetRandf((float)(_y / 20)), 0.995f);
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
			void Put(const Vector2D_I& _pos, int _hight) {
				this->pos = _pos;
				this->m_Base_Hight = _hight + 2;
			}

			void Draw_Drop(const Tiles& _Ti, const Camera_Info& _caminfo) {
				if (haveHuman == nullptr) {
					Draw_Common(_Ti, _caminfo, this->m_Yrad, this->m_Graphs[6], Vector2D_I::Get(0, 0));
				}
			}

		public:
			void Init() {
				GraphHandle::LoadDiv("data/Gun/1.bmp", 15, 5, 3, 96, 96, &this->m_Graphs);
				this->m_FirstUp = GraphHandle::Load("data/Gun/1_Up.bmp");
				this->m_FirstUp.GetSize(&m_Xsize, &m_Ysize);
				this->m_FirstUp_Empty = GraphHandle::Load("data/Gun/1_Up_Empty.bmp");
				//this->m_FirstUp_Empty.GetSize(&m_Xsize, &m_Ysize);
				this->m_moveStart = false;
				//--
				{
					const auto mdata = FileRead_open("data/Gun/1.gdt", FALSE);
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
				//--
				ReloadEnd();
			}

			void Update(const Camera_Info& _caminfo) {
				if (haveHuman != nullptr) {
					if (inChamber) {
						if (this->Recoil == 10.f) {
							PlaySound_Near3D(ENUM_SOUND::Slide0, this->pos, _caminfo, 64);
						}
						this->Recoil = std::max(this->Recoil - 1.5f * 60.f / FPS, 0.f);
					}
					this->RecoilCnt = std::max(this->RecoilCnt - 1.5f * 60.f / FPS, 0.f);
					this->ShotTime = std::max(this->ShotTime - 1.f / FPS, 0.f);
					this->Recoilpos = Vector2D_I::Get(y_r(sin(this->m_Yrad)*this->Recoil), y_r(-cos(this->m_Yrad)*this->Recoil))*-1.f;
					this->pos = haveHuman->pos + haveHuman->GetRightHandInfo().pos;
					this->m_hight = haveHuman->GetRightHandInfo().m_hight - haveHuman->GetBaseHight() + 1;
					this->m_Yrad = haveHuman->GetRightHandInfo().yrad + haveHuman->GetRightHandInfo().yr;
				}
				else {
					//this->pos = ;
					this->m_Yrad += deg2rad(90) / FPS;
					{
						auto buf = this->pos;
						this->UpdateSpeed(0.1f);
						this->hight_f -= this->hight_add*60.f / FPS;
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
			void DrawAmmos(const Tiles& _Ti, const Camera_Info& _caminfo) {
				for (auto& am : ammo) {
					am.Draw(_Ti, _caminfo);
				}
				for (auto& am : cart) {
					am.Draw(_Ti, _caminfo);
				}
			}
			void Draw(const Tiles& _Ti, const Camera_Info& _caminfo) {
				if (haveHuman != nullptr) {
					if (this->isDraw) {
						Draw_Common(_Ti, _caminfo, this->m_Yrad, this->m_Graphs[7], Vector2D_I::Get(0, 0));
						Draw_Common(_Ti, _caminfo, this->m_Yrad, this->m_Graphs[5], this->Recoilpos);
					}
					//マズルフラッシュ
					{
						if (this->Time_ShotFlash <= 0.2f) {
							int Cnt = std::clamp(1 + (int)(this->Time_ShotFlash / 0.2f * 4.f), 1, 4);
							this->Flash.Draw_Common(_Ti, _caminfo, this->m_Yrad, this->m_Graphs[Cnt], Vector2D_I::Get(0, 0));
						}
					}
				}
			}
			void Draw_Shadow(const Tiles& _Ti, float _light_yrad, float _ShadowRange, const Camera_Info& _caminfo) {
				if (this->isDraw) {
					Draw_Shadow_Common(_Ti, _light_yrad, _ShadowRange, _caminfo, this->m_Yrad, this->m_Graphs[7], Vector2D_I::Get(0, 0));
					Draw_Shadow_Common(_Ti, _light_yrad, _ShadowRange, _caminfo, this->m_Yrad, this->m_Graphs[5], this->Recoilpos);
				}
				for (auto& am : this->cart) { am.Draw_Shadow(_Ti, _light_yrad, _ShadowRange, _caminfo); }
			}
			void Draw_Light(const Tiles& _Ti, const Camera_Info& _caminfo) {
				for (auto& am : this->ammo) { am.Draw_Light(_Ti, _caminfo); }
			}
		};
		class Magazine_Object : public Object_Vanish {
		private:
			Gun_Object* haveGun = nullptr;
			float Time_ShotFlash = 100.f;
			float hight_f = 0;
			float hight_add = 0;
		public:
			void Set_Hit() { this->Time = 0.f; }
		public:
			void Init(Gun_Object* haveGun_t) {
				if (haveGun_t != nullptr) {
					haveGun = haveGun_t;
					this->Init_Common(haveGun);

					this->m_Yrad += deg2rad(GetRand(360));
					this->Time_ShotFlash = 0.f;
					this->hight_f = (float)this->m_hight;
					this->hight_add = 0.f;

					this->Time = 2.f;
					this->m_speed = 1.f;
				}
			}
			void Update(std::vector<std::vector<Tiles>>& _tile) {
				auto buf = this->pos;
				this->UpdateSpeed(0.1f);
				this->UpdateTime();
				this->hight_f -= this->hight_add*60.f / FPS;
				if (this->hight_f <= 0.f) {
					this->hight_add = 0.f;
					this->hight_f = 0.f;
				}
				this->hight_add += 9.8f / FPS;
				this->m_hight = (int)this->hight_f;
				if (GetHitWall(_tile, this->pos, buf, (int)(this->m_speed / 2))) {
					this->Set_Hit();
				}
			}
			void Draw(const Tiles& _Ti, const Camera_Info& _caminfo) {
				Draw_Common(_Ti, _caminfo, this->m_Yrad, this->haveGun->GetMagGraph(), Vector2D_I::Get(0, 0));
			}
			void Draw_Shadow(const Tiles& _Ti, float _light_yrad, float _ShadowRange, const Camera_Info& _caminfo) {
				Draw_Shadow_Common(_Ti, _light_yrad, _ShadowRange, _caminfo, this->m_Yrad, this->haveGun->GetMagGraph(), Vector2D_I::Get(0, 0));
			}
		};
		class Effect2D {
		private:
			class Effect_Object : public Object_Vanish {
				Vector2D_I m_ofSet;
			public:
				//ヒットエフェクト
				void Set_Hit(Object_Common* have_t, Object_Common* ps_ef = nullptr) {
					if (have_t != nullptr) {
						this->Init_Common(have_t);
						//
						this->m_Yrad -= deg2rad(90);
						m_ofSet.set((int)((float)(48 + 12) * -cos(this->m_Yrad)), (int)((float)(48 + 12) * -sin(this->m_Yrad)));
						//
						if (ps_ef != nullptr) {
							this->posSet(ps_ef);
						}
						this->Time = 0.4f;
					}
				}
			public:
				void Update() {
					this->UpdateTime();
				}
				void Draw(const Tiles& _Ti, const Camera_Info& _caminfo, const std::vector<GraphHandle>& _Graphs) {
					if (this->iaActive()) {
						int Cnt = std::clamp(0 + (int)((0.4f - this->Time) / 0.4f * 10.f), 0, 10);
						//Cnt = 9;
						Draw_Common(_Ti, _caminfo, this->m_Yrad, _Graphs[Cnt], m_ofSet);
					}
				}
			};
		private:
			std::vector<GraphHandle> m_Graphs;
			std::vector<Effect_Object> m_Effects;
		public:
			void Set_Hit(Object_Common* have_t, Object_Common* ps_ef = nullptr) {
				if (have_t != nullptr) {
					m_Effects.resize(m_Effects.size() + 1);
					m_Effects.back().Set_Hit(have_t, ps_ef);
				}
			}
		public:
			void Init(std::string_view FileName) {
				GraphHandle::LoadDiv(FileName, 25, 5, 5, 48, 48, &this->m_Graphs);
				m_Effects.clear();
			}
			void Update() {
				for (int i = 0; i < m_Effects.size(); i++) {
					auto& ef = m_Effects[i];
					ef.Update();
					if (ef.isEnd()) {
						ef = m_Effects.back();
						m_Effects.pop_back();
						i--;
					}
				}
			}
			void Draw(const Tiles& _Ti, const Camera_Info& _caminfo) {
				for (auto& ef : m_Effects) {
					ef.Draw(_Ti, _caminfo, m_Graphs);
				}
			}
		};
	private:
		//ゲーム内使用
		std::vector<TileStatus> m_TileData;
		std::vector<std::vector<Tiles>> m_Tile;
		std::vector<Vector2D_I> m_way_point;
		std::vector<Human_Object> m_human;
		size_t m_player_id = 0;
		std::vector<Gun_Object> m_gun;
		std::vector<Magazine_Object> m_mag;
		std::vector<Effect2D> m_effect;
		float m_light_yrad = 0.0f;
		float m_ShadowRange = 6.f;
		std::array<shadow_handle, 8> m_shadow_graph;
		std::array<Vector2D_I, 8> m_shadow_pos;
		std::vector<GraphHandle> m_walls;
		std::vector<GraphHandle> m_floors;
		GraphHandle m_res_floor;
		GraphHandle m_screen;
		Camera_Info m_caminfo;
		float m_Zoom_buf;
		float m_WorldPhaseDownTimer = 0.f;
		float m_WorldPhaseDownTimerMax = 10.f;
		int m_WorldPhase = 0;
		int m_WorldPhase_Lowest = 0;
		int m_maximamPhase = 0;
	private:
		//基幹描画
		auto& GetFloor_BlendShadow(const Vector2D_I& _p1, const Vector2D_I& _p2, int _hight, GraphHandle* _handle) {
			const int g = DerivationGraph(
				std::max(0, _p1.x), std::max(0, _p1.y), std::min(DrawPts->disp_x, _p2.x - _p1.x), std::min(DrawPts->disp_y, _p2.y - _p1.y),
				m_shadow_graph[std::clamp<size_t>(_hight / 8, 0, m_shadow_graph.size() - 1)].GetHandle().get());
			GraphBlendBlt(_handle->get(), g, m_res_floor.get(), 128, DX_GRAPH_BLEND_NORMAL);
			DeleteGraph(g);
			return m_res_floor;
		}
		//壁
		void Draw_Wall(int _UorL, const Tiles& _Ti) {
			if (_UorL < 20 && _Ti.m_hight != _Ti.m_bottom) {
				{
					float rad = abs(cos(atan2f(float(_Ti.m_hight - _Ti.m_bottom), (float)y_r(tilesize) / m_caminfo.camzoom)));
					Set_Bright((int)(rad * (0.75f + cos(m_light_yrad + deg2rad((4 - _UorL % 4) * 90)) * 0.25f) * 255.f));
				}
				switch (_UorL) {
				case 0://縦(上)
					if (_Ti.m_floor[0].y < _Ti.m_top[0].y)
						DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[1], _Ti.m_floor[1], _Ti.m_floor[0], _Ti.wallhandle);
					break;
				case 4://上◢
					if (_Ti.m_floor[1].y < _Ti.m_top[1].y)//ここだけ変
						DrawModi_wrap(_Ti.m_top[1], _Ti.m_top[1], _Ti.m_floor[1], _Ti.m_floor[0], _Ti.wallhandle);
					break;
				case 8://上◣
					if (_Ti.m_floor[0].y < _Ti.m_top[0].y)
						DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[0], _Ti.m_floor[1], _Ti.m_floor[0], _Ti.wallhandle);
					break;
				case 12://縦(上)
					if (_Ti.m_floor[0].y < _Ti.m_top[2].y)
						DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[3], _Ti.m_floor[1], _Ti.m_floor[0], _Ti.wallhandle);
					break;
				case 16://縦(上)
					if (_Ti.m_floor[0].y < _Ti.m_top[2].y) {
						DrawModi_wrap(_Ti.m_floor[0], _Ti.m_floor[1], _Ti.m_top[3], _Ti.m_top[2], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.floorhandle));
					}
					break;
				case 1://横(左)
					if (_Ti.m_floor[0].x < _Ti.m_top[0].x)
						DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[0], _Ti.m_floor[0], _Ti.m_floor[2], _Ti.wallhandle);
					break;
				case 5://左◢
					if (_Ti.m_floor[0].x < _Ti.m_top[0].x)
						DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[2], _Ti.m_floor[0], _Ti.m_floor[2], _Ti.wallhandle);
					break;
				case 9://左◥
					if (_Ti.m_floor[0].x < _Ti.m_top[0].x)
						DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[0], _Ti.m_floor[0], _Ti.m_floor[2], _Ti.wallhandle);
					break;
				case 13://横(左)
					if (_Ti.m_floor[0].x < _Ti.m_top[3].x)
						DrawModi_wrap(_Ti.m_top[3], _Ti.m_top[1], _Ti.m_floor[0], _Ti.m_floor[2], _Ti.wallhandle);
					break;
				case 17://横(左)
					if (_Ti.m_floor[0].x < _Ti.m_top[3].x) {
						DrawModi_wrap(_Ti.m_floor[0], _Ti.m_top[1], _Ti.m_top[3], _Ti.m_floor[2], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.floorhandle));
					}
					break;
				case 2://縦(下)
					if (_Ti.m_floor[3].y >= _Ti.m_top[3].y)
						DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[3], _Ti.m_floor[3], _Ti.m_floor[2], _Ti.wallhandle);
					break;
				case 6://下◢
					if (_Ti.m_floor[3].y >= _Ti.m_top[3].y)
						DrawModi_wrap(_Ti.m_top[3], _Ti.m_top[3], _Ti.m_floor[3], _Ti.m_floor[2], _Ti.wallhandle);
					break;
				case 10://下◣
					if (_Ti.m_floor[3].y >= _Ti.m_top[3].y)
						DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[2], _Ti.m_floor[3], _Ti.m_floor[2], _Ti.wallhandle);
					break;
				case 14://縦(下)
					if (_Ti.m_floor[2].y > _Ti.m_top[1].y) {
						DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[1], _Ti.m_floor[3], _Ti.m_floor[2], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.floorhandle));
					}
					break;
				case 18://縦(下)
					if (_Ti.m_floor[2].y > _Ti.m_top[1].y)
						DrawModi_wrap(_Ti.m_top[0], _Ti.m_top[1], _Ti.m_floor[3], _Ti.m_floor[2], _Ti.wallhandle);
					break;
				case 3://横(右)
					if (_Ti.m_floor[3].x >= _Ti.m_top[3].x)
						DrawModi_wrap(_Ti.m_top[3], _Ti.m_top[1], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.wallhandle);
					break;
				case 7://右◢
					if (_Ti.m_floor[3].x >= _Ti.m_top[3].x)
						DrawModi_wrap(_Ti.m_top[3], _Ti.m_top[3], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.wallhandle);
					break;
				case 11://右◥
					if (_Ti.m_floor[3].x >= _Ti.m_top[3].x)
						DrawModi_wrap(_Ti.m_top[1], _Ti.m_top[1], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.wallhandle);
					break;
				case 15://横(右)
					if (_Ti.m_floor[1].x > _Ti.m_top[2].x) {
						DrawModi_wrap(_Ti.m_top[0], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.m_top[2], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.floorhandle));
					}
					break;
				case 19://横(右)
					if (_Ti.m_floor[1].x > _Ti.m_top[2].x)
						DrawModi_wrap(_Ti.m_top[2], _Ti.m_top[0], _Ti.m_floor[1], _Ti.m_floor[3], _Ti.wallhandle);
					break;
				}
			}
			else {
				{
					auto cam_high = (int)((float)m_caminfo.camhigh_base / m_caminfo.camzoom);
					Set_Bright(255 - 255 * std::clamp(_Ti.m_hight, 0, cam_high) / cam_high);
				}
				if (!_Ti.m_isWall) {
					DrawExtend_wrap(_Ti.m_top[0], _Ti.m_top[3], &GetFloor_BlendShadow(_Ti.m_top[0], _Ti.m_top[3], _Ti.m_hight, _Ti.floorhandle));
				}
				else {
					DrawExtend_wrap(_Ti.m_top[0], _Ti.m_top[3], _Ti.floorhandle);
				}
			}
		}
		//柱描画
		void Draw_Tile(const Tiles& _Ti) {
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
			for (auto& gn : m_gun) { gn.DrawAmmos(_Ti, m_caminfo); }
			for (auto& mg : m_mag) { mg.Draw(_Ti, m_caminfo); }
			for (auto& gn : m_gun) { gn.Draw_Drop(_Ti, m_caminfo); }
			for (auto& pl : m_human) { pl.Draw(_Ti, m_caminfo); }
			for (auto& gn : m_gun) { gn.Draw(_Ti, m_caminfo); }
			for (auto& ef : m_effect) { ef.Draw(_Ti, m_caminfo); }
			Set_Bright(255);
		}
		//y軸描画
		void DrawCommon_Y(std::vector<Tiles>& T_X) {
			//画面位置取得
			const Vector2D_I limmin = ConvertPos(Vector2D_I::Get(-DrawPts->disp_x / 2, -DrawPts->disp_y / 2), 0, m_caminfo);
			const Vector2D_I cam = ConvertPos(Vector2D_I::Get(0, 0), 0, m_caminfo);
			const Vector2D_I limmax = ConvertPos(Vector2D_I::Get(DrawPts->disp_x / 2, DrawPts->disp_y / 2), 0, m_caminfo);

			for (auto& pl : m_human) { pl.Reset(); }
			for (auto& ti : T_X) {
				if (!(cam.y >= ti.m_top[0].y && ti.m_zero[3].y >= limmin.y)) { continue; }
				Draw_Tile(ti);
			}
			for (auto& pl : m_human) { pl.Reset(); }
			for (int y = (int)(T_X.size()) - 1; y >= 0; --y) {
				auto& ti = T_X[y];
				if (!(cam.y <= ti.m_top[3].y && ti.m_zero[0].y <= limmax.y)) { continue; }
				Draw_Tile(ti);
			}
		}
		void DrawCommon() {
			//画面位置取得
			const Vector2D_I limmin = ConvertPos(Vector2D_I::Get(-DrawPts->disp_x / 2, -DrawPts->disp_y / 2), 0, m_caminfo);
			const Vector2D_I cam = ConvertPos(Vector2D_I::Get(0, 0), 0, m_caminfo);
			const Vector2D_I limmax = ConvertPos(Vector2D_I::Get(DrawPts->disp_x / 2, DrawPts->disp_y / 2), 0, m_caminfo);

			for (int x = 0; x < (int)(m_Tile.size()); ++x) {
				if (!(cam.x >= m_Tile[x][0].m_top[0].x && m_Tile[x][0].m_zero[3].x >= limmin.x)) { continue; }
				DrawCommon_Y(m_Tile[x]);
			}
			for (int x = (int)(m_Tile.size()) - 1; x >= 0; --x) {
				if (!(cam.x <= m_Tile[x][0].m_top[3].x && m_Tile[x][0].m_zero[0].x <= limmax.x)) { continue; }
				DrawCommon_Y(m_Tile[x]);
			}
		}
		//壁影描画
		void draw_wall_shadow(int _UorL, const Tiles& _Ti) {//一辺
			switch (_UorL) {
			case 0://縦(上)
				DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[4], _Ti.wallhandle);
				break;
			case 8://上◣
				DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[0], m_shadow_pos[6], m_shadow_pos[4], _Ti.wallhandle);
				break;
			case 4://上◢
				DrawModi_wrap(m_shadow_pos[1], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[4], _Ti.wallhandle);
				break;
			case 16://縦(上)
				if (m_shadow_pos[4].y < m_shadow_pos[2].y) {
					DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[6], m_shadow_pos[4], _Ti.floorhandle);
				}
				break;
			case 12://縦(上)
				DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[6], m_shadow_pos[4], _Ti.wallhandle);
				break;
			case 1://横(左)
				DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[4], m_shadow_pos[5], _Ti.wallhandle);
				break;
			case 5://左◢
				DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[2], m_shadow_pos[4], m_shadow_pos[5], _Ti.wallhandle);
				break;
			case 9://左◥
				DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[0], m_shadow_pos[4], m_shadow_pos[5], _Ti.wallhandle);
				break;
			case 13://横(左)
				DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[4], m_shadow_pos[5], _Ti.wallhandle);
				break;
			case 17://横(左)
				if (m_shadow_pos[4].x < m_shadow_pos[3].x) {
					DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[4], m_shadow_pos[5], _Ti.floorhandle);
				}
				break;
			case 2://縦(下)
				DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[3], m_shadow_pos[7], m_shadow_pos[5], _Ti.wallhandle);
				break;
			case 6://下◢
				DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[3], m_shadow_pos[7], m_shadow_pos[5], _Ti.wallhandle);
				break;
			case 10://下◣
				DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[2], m_shadow_pos[7], m_shadow_pos[5], _Ti.wallhandle);
				break;
			case 14://縦(下)
				if (m_shadow_pos[5].y > m_shadow_pos[1].y) {
					DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[7], m_shadow_pos[5], _Ti.floorhandle);
				}
				break;
			case 18://縦(下)
				DrawModi_wrap(m_shadow_pos[0], m_shadow_pos[1], m_shadow_pos[7], m_shadow_pos[5], _Ti.wallhandle);
				break;
			case 3://横(右)
				DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[7], _Ti.wallhandle);
				break;
			case 7://右◢
				DrawModi_wrap(m_shadow_pos[3], m_shadow_pos[3], m_shadow_pos[6], m_shadow_pos[7], _Ti.wallhandle);
				break;
			case 11://右◥
				DrawModi_wrap(m_shadow_pos[1], m_shadow_pos[1], m_shadow_pos[6], m_shadow_pos[7], _Ti.wallhandle);
				break;
			case 15://横(右)
				if (m_shadow_pos[6].x > m_shadow_pos[2].x) {
					DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[6], m_shadow_pos[7], _Ti.floorhandle);
				}
				break;
			case 19://横(右)
				DrawModi_wrap(m_shadow_pos[2], m_shadow_pos[0], m_shadow_pos[6], m_shadow_pos[7], _Ti.wallhandle);
				break;
			}
		}
		//影を一部分描画
		void Update_Shadow(void) {
			const auto limmin_shadow = ConvertPos(Vector2D_I::Get(-DrawPts->disp_x * 3 / 4, -DrawPts->disp_y * 3 / 4), 0, m_caminfo);
			const auto limmax_shadow = ConvertPos(Vector2D_I::Get(DrawPts->disp_x * 3 / 4, DrawPts->disp_y * 3 / 4), 0, m_caminfo);
			int high = 0;
			//*
			for (auto& g : m_shadow_graph) {
				g.SetDraw([&] {
					//環境影
					{
						if (m_light_yrad >= deg2rad(-90) && m_light_yrad <= deg2rad(90)) {
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::clamp(255 - (int)(192.f*abs(cos(m_light_yrad))), 0, 255));
						}
						if (m_light_yrad <= deg2rad(-90) || m_light_yrad >= deg2rad(90)) {
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
						}
						DrawBox(0, 0, DrawPts->disp_x, DrawPts->disp_y, GetColor(0, 0, 0), TRUE);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
						if (m_light_yrad >= deg2rad(-90) && m_light_yrad <= deg2rad(90)) {
							Set_Bright(0);
							for (auto& T_X : m_Tile) {
								if (!(T_X[0].m_zero[0].x <= limmax_shadow.x && T_X[0].m_zero[3].x >= limmin_shadow.x)) { continue; }
								for (auto& ti : T_X) {
									if (!(ti.m_zero[0].y <= limmax_shadow.y && ti.m_zero[3].y >= limmin_shadow.y)) { continue; }
									if (ti.m_isWall) {
										//柱の影描画
										const auto zh2 = (float)(ti.m_hight - ti.m_bottom - high)*m_ShadowRange;
										auto shadow_add = Vector2D_I::Get((int)(zh2 * sin(m_light_yrad)), (int)(zh2 * cos(m_light_yrad)));
										const auto xmin = y_r(tilesize) * (ti.m_postile.x + 0);
										const auto ymin = y_r(tilesize) * (ti.m_postile.y + 0);
										const auto xmax = y_r(tilesize) * (ti.m_postile.x + 1);
										const auto ymax = y_r(tilesize) * (ti.m_postile.y + 1);

										m_shadow_pos[0] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin) + shadow_add, ti.m_bottom + high, m_caminfo);
										m_shadow_pos[1] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin) + shadow_add, ti.m_bottom + high, m_caminfo);
										m_shadow_pos[2] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax) + shadow_add, ti.m_bottom + high, m_caminfo);
										m_shadow_pos[3] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax) + shadow_add, ti.m_bottom + high, m_caminfo);
										m_shadow_pos[4] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin), ti.m_bottom + high, m_caminfo);//◤
										m_shadow_pos[5] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax), ti.m_bottom + high, m_caminfo);//◣
										m_shadow_pos[6] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin), ti.m_bottom + high, m_caminfo);//◥
										m_shadow_pos[7] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax), ti.m_bottom + high, m_caminfo);//◢
										switch (ti.m_dir) {//三角柱
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
											if (m_light_yrad >= deg2rad(0)) {
												draw_wall_shadow(2, ti);		//縦(下)
												draw_wall_shadow(3, ti);		//横(右)
											}
											else {
												draw_wall_shadow(1, ti);		//横(左)
												draw_wall_shadow(2, ti);		//縦(下)
											}
											/*
											draw_wall_shadow(0, ti);		//縦(上)
											draw_wall_shadow(1, ti);		//横(左)
											draw_wall_shadow(2, ti);		//縦(下)
											draw_wall_shadow(3, ti);		//横(右)
											*/
											break;
										}
									}
									else {
										for (auto& pl : m_human) { pl.Draw_Shadow(ti, m_light_yrad, m_ShadowRange, m_caminfo); }
										for (auto& mg : m_mag) { mg.Draw_Shadow(ti, m_light_yrad, m_ShadowRange, m_caminfo); }
										for (auto& gn : m_gun) { gn.Draw_Shadow(ti, m_light_yrad, m_ShadowRange, m_caminfo); }
									}
								}
							}
						}
					}
					//ライト
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
						for (auto& T_X : m_Tile) {
							if (T_X[0].m_zero[0].x <= limmax_shadow.x && T_X[0].m_zero[3].x >= limmin_shadow.x) {
								for (auto& ti : T_X) {
									if (!(ti.m_zero[0].y <= limmax_shadow.y && ti.m_zero[3].y >= limmin_shadow.y)) { continue; }
									for (auto& gn : m_gun) { gn.Draw_Light(ti, m_caminfo); }
								}
							}
						}
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					}
				});
				high += 8;
			}
			//*/
			Set_Bright(255);
		}
		//AIの次ウェイポイントを取得
		const auto GetNextWaypoint(Human_Object& pl) const noexcept {
			int now = -1;
			auto tmp = Vector2D_I::Get(0, 1000);
			for (auto& w : this->m_way_point) {
				auto id = (int)(&w - &this->m_way_point[0]);
				if (pl.cpu_do.Compare(id)) {
					if (tmp.hydist() >= (w - pl.pos).hydist()) {
						auto Vec = Vector2D_I::Get((int)(sin(pl.Getyrad())*100.f), (int)(-cos(pl.Getyrad())*100.f));
						if (
							Vec.dot(w - pl.pos) > 0 &&
							!GetHitWall(m_Tile, w, pl.pos, 0)
							) {
							tmp = (w - pl.pos);
							now = int(id);
						}
					}
				}
			}
			return now;
		}
		const auto CheckFoundEnemy(Human_Object& pl, const Vector2D_I& tgt, int _range) const noexcept {
			int Dst = (tgt - pl.pos).hydist();

			if (Dst <= _range * _range) {
				auto Vec = Vector2D_I::Get((int)(sin(pl.GetLookyrad())*100.f), (int)(-cos(pl.GetLookyrad())*100.f));
				auto Rad = (int)(Vec.dot(tgt - pl.pos) * 180.f / DX_PI_F / ((int)(sqrt(Dst)) * 100));
				if (
					Rad > EyeRad &&
					!GetHitWall(m_Tile, tgt, pl.pos, 0)
					) {
					return true;
				}
			}
			return false;
		}

		//目標ウェイポイントに対する向きの取得
		const auto GetWaypointtoVec(Human_Object& pl) const noexcept {
			if (this->m_way_point.size() < pl.cpu_do.GetFront()) {
				return Vector2D_I::Get(0, 0);
			}
			else {
				return  this->m_way_point[pl.cpu_do.GetFront()] - pl.pos;
			}
		}
		//ウェイポイント設定
		void SetNextWaypoint(Human_Object& pl) const noexcept {
			auto now = GetNextWaypoint(pl);
			if (now != -1) {
				pl.cpu_do.Push(now);
			}
		}
	public:
		const Vector2D_I PlayerPos() const noexcept { return m_human[m_player_id].pos; }
	public:
		//コンストラクタ
		Near3DControl(std::shared_ptr<DXDraw>& _DrawPts) noexcept {
			DrawPts = _DrawPts;
			for (auto& g : m_shadow_graph) { g.Init(DrawPts->disp_x, DrawPts->disp_y); }
			m_screen = GraphHandle::Make(DrawPts->disp_x, DrawPts->disp_y, false);
			m_res_floor = GraphHandle::Make(16, 16, true);
			//エフェクト読み込み
			m_effect.resize(m_effect.size() + 1);
			m_effect.back().Init("data/Effect/1.bmp");
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
		}
		//デストラクタ
		~Near3DControl(void) noexcept {
			Dispose();
			DrawPts.reset();
			if (!RemoveFontResourceEx(m_font_path, FR_PRIVATE, NULL)) {
				MessageBox(NULL, "remove failure", "", MB_OK);
			}
		}
		//map選択
		void Start(int _SpawnPoint, std::string _mapname) {
			using namespace std::literals;
			int map_x = 0, map_y = 0;
			{
				std::fstream file;
				//mapデータ1読み込み(マップチップ)
				m_TileData.clear();
				file.open(("data/Map/" + _mapname + "/1.dat").c_str(), std::ios::binary | std::ios::in);
				do {
					m_TileData.resize(m_TileData.size() + 1);
					file.read((char*)&m_TileData.back(), sizeof(m_TileData.back()));
				} while (!file.eof());
				file.close();
				m_TileData.pop_back();
				for (auto& d : m_TileData) {
					map_x = std::max(d.m_postile.x, map_x);
					map_y = std::max(d.m_postile.y, map_y);
				}
				map_x++;
				map_y++;

				m_Tile.resize(map_x);
				for (auto& T_X : m_Tile) {
					T_X.resize(map_y);
				}
				//mapデータ2読み込み(プレイヤー初期位置、使用テクスチャ指定)
				Edit::maps mapb;
				file.open(("data/Map/" + _mapname + "/2.dat").c_str(), std::ios::binary | std::ios::in);
				file.read((char*)&mapb, sizeof(mapb));
				file.close();
				m_light_yrad = mapb.m_light_yrad;
				m_light_yrad = deg2rad(45);
				GraphHandle::LoadDiv(mapb.wall_name, 32, 16, 2, 16, 16 * 2, &m_walls);
				GraphHandle::LoadDiv(mapb.floor_name, 256, 16, 16, 16, 16, &m_floors);
				m_player_id = m_human.size();
				m_human.resize(m_human.size() + 1);
				m_human.back().Init(mapb.SP[std::clamp(_SpawnPoint, 0, mapb.SP_Limit - 1)] * -1.f, 1);
				//mapデータ3読み込み(敵情報)
				file.open(("data/Map/" + _mapname + "/3.dat").c_str(), std::ios::binary | std::ios::in);
				do {
					Edit::Player_Info anse;
					file.read((char*)&anse, sizeof(anse));
					if (anse.pos_p != Vector2D_I::Get(0, 0)) {
						m_human.resize(m_human.size() + 1);
						m_human.back().Init(anse.pos_p, 2);
					}
				} while (!file.eof());
				file.close();
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
			//銃セット
			{
				m_gun.resize(m_gun.size() + 1);//0
				m_gun.back().Init();
				m_gun.resize(m_gun.size() + 1);//1
				m_gun.back().Init();
				m_gun.resize(m_gun.size() + 1);//2
				m_gun.back().Init();
				m_gun.resize(m_gun.size() + 1);//3
				m_gun.back().Init();
				m_gun.resize(m_gun.size() + 1);//4
				m_gun.back().Init();
				m_gun.resize(m_gun.size() + 1);//5
				m_gun.back().Init();
				m_gun.resize(m_gun.size() + 1);//6
				m_gun.back().Init();
				m_gun.resize(m_gun.size() + 1);//7
				m_gun.back().Init();

				Vector2D_I tmp;
				tmp.set(200, 200);
				m_gun[0].Put(tmp, GetTile(tmp).m_hight);
				//m_human[0].SetGun(&m_gun[0]);
				m_human[1].SetGun(&m_gun[1]);
				m_human[2].SetGun(&m_gun[2]);
				m_human[3].SetGun(&m_gun[3]);
				m_human[4].SetGun(&m_gun[4]);
				m_human[5].SetGun(&m_gun[5]);
				m_human[6].SetGun(&m_gun[6]);
				m_human[7].SetGun(&m_gun[7]);
			}
			//環境音開始
			SE.Get((int)ENUM_SOUND::Envi).Play(0, DX_PLAYTYPE_LOOP, TRUE, 96);
			//
			for (auto& pl : m_human) {
				if ((size_t)(&pl - &m_human.front()) != m_player_id) {
					SetNextWaypoint(pl);
				}
			}
		}
		//更新
		void Update(Vector2D_I& m_vec, int _PlayerInput,
			//bool is_stand, const bool is_run, const bool aim, const bool shoot, const bool reload, const bool rolling, const bool _Look, const bool _Get,
			const Vector2D_I& cam_pos) {

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
				{
					m_light_yrad += deg2rad(0.1f) / FPS;
					if (m_light_yrad > deg2rad(180)) { m_light_yrad = deg2rad(-180); }
					if (m_light_yrad >= deg2rad(-90) && m_light_yrad <= deg2rad(90)) {
						m_ShadowRange = std::clamp(1.f + 9.f*abs(sin(m_light_yrad)), 3.f, 10.f);
					}
					else {
						m_ShadowRange = 10.f;
					}
				}
				bool EnableTrans = true;
				for (auto& pl : m_human) {
					if (pl.transceiverStart) {
						EnableTrans = false;
					}
				}
				bool FoundEnemyAny = false;
				int maximamPhase = 0;
				for (auto& pl : m_human) {
					Vector2D_I Vec, Aim;
					bool is_Run;
					bool is_Get = false;
					bool FoundEnemy = false;
					//CPU
					if ((size_t)(&pl - &m_human.front()) != m_player_id) {
						//todo
						int X = 0, Y = 0;
						{
							//アラートからコーションに落ちた時に警戒無線する
							if (!pl.transceiverSwitch && (!pl.isCaution() && pl.isAlart())) {
								pl.transceiverSwitch = true;
							}

							if (!EnableTrans) {
								pl.transceiverSwitch = false;
							}

							if (pl.transceiverSwitch && pl.isCaution()) {
								pl.transceiverSwitch = false;
								pl.EnableTransceiver();
							}

							if (CheckFoundEnemy(pl, m_human[m_player_id].pos, pl.isCaution() ? y_r(tilesize * 10) : y_r(tilesize * 7))) {
								FoundEnemy = true;
								FoundEnemyAny = true;
								pl.EnableFoundEnemy();
							}
							//方向指定
							Vector2D_I WPvec;
							if (pl.isHaveGun() || pl.isAlart()) {//AIナビ
								is_Run = (pl.isAlart() && !FoundEnemy);
								WPvec = GetWaypointtoVec(pl);
								//到達時
								if (WPvec.hydist() <= y_r(tilesize)*y_r(tilesize)) {
									SetNextWaypoint(pl);
								}
							}
							else {//近場の銃器を拾う
								bool tmpb = false;
								Vector2D_I tmpv = Vector2D_I::Get(100000, 100000);
								for (auto& gn : m_gun) {
									if (gn.haveHuman == nullptr) {
										if (tmpv.hydist() <= (gn.pos - pl.pos).hydist()) {
											if (CheckFoundEnemy(pl, gn.pos, y_r(tilesize * 7))) {
												tmpb = true;
												tmpv = (gn.pos - pl.pos);
											}
										}
									}
								}
								if (tmpb) {
									is_Run = true;
									WPvec = tmpv;
								}
								else {
									is_Run = (pl.isAlart() && !FoundEnemy);
									WPvec = GetWaypointtoVec(pl);
								}
								//到達時
								if (WPvec.hydist() <= y_r(tilesize / 2)*y_r(tilesize / 2)) {
									is_Get = true;
								}
							}
							//スタック回避
							float PP = ((float)sqrt(pl.Get_vec_buf().hydist()) / pl.GetSpeed());
							if (PP <= 0.5f) {
								SetNextWaypoint(pl);
							}
							if (!pl.transceiverStart) {
								if (WPvec.x != 0) {
									X = (WPvec.x > 0) ? 1 : -1;
								}
								if (WPvec.y != 0) {
									Y = (WPvec.y > 0) ? 1 : -1;
								}
							}
						}
						//立ち伏せ
						pl.SetStand(false);
						//入力
						pl.SetKey(true, FoundEnemy ? (GetRand(10) == 0) : false, false, false, m_caminfo);
						//方向入力
						{
							Vec.set(X, Y);
						}
						//エイム先
						if (pl.isAlart()) {
							Aim = m_human[m_player_id].pos - pl.pos;
						}
						else {
							Aim = Vector2D_I::Get(0, 0);
						}
					}
					//自機の移動
					else {
						//立ち伏せ
						pl.SetStand(is_stand && !is_run);
						//入力
						pl.SetKey(aim, shoot, reload, rolling, m_caminfo);
						if (_Look) {
							pl.LookGun();
						}
						is_Get = _Get;
						//方向入力
						Vec = m_vec;
						is_Run = is_run;
						//エイム先
						int x_m, y_m;
						GetMousePoint(&x_m, &y_m);

						auto P = ConvertPos_CalcCam(pl.pos, pl.GetHight(), m_caminfo);
						Aim.set(x_m - P.x, y_m - P.y);
						//
						easing_set(&m_Zoom_buf, pl.GetCamZoom(), 0.95f);
						m_caminfo.camzoom = std::clamp(m_Zoom_buf, 0.6f, 2.0f);
						//m_caminfo.camzoom = 0.4f;
					}
					if (is_Get) {
						for (auto& gn : m_gun) {
							if (pl.haveGun != &gn && gn.haveHuman == nullptr) {
								if ((gn.pos - pl.pos).hydist() < (y_r(tilesize) * y_r(tilesize))) {
									if (pl.isHaveGun()) {
										pl.haveGun->Put(pl.pos, GetTile(pl.pos).m_hight);
									}
									pl.SetGun(&gn);
									break;
								}
							}
						}
					}

					bool OLD = pl.transceiverStart;

					pl.Update_Human(m_Tile, Vec, Aim, is_Run, m_WorldPhase_Lowest);	//移動

					if (!pl.transceiverStart && pl.transceiverStart != OLD) {
						m_WorldPhaseDownTimer = m_WorldPhaseDownTimerMax;
						m_WorldPhase = std::min(m_WorldPhase + 1, 4);
						m_WorldPhase_Lowest = 1;
					}

					//マガジン落下
					if (pl.isReloadStart()) {
						m_mag.resize(m_mag.size() + 1);
						m_mag.back().Init(pl.haveGun);
					}
					if (maximamPhase <= pl.isPhase()) {
						maximamPhase = pl.isPhase();
					}
				}
				if (maximamPhase <= m_WorldPhase) {
					maximamPhase = m_WorldPhase;
				}
				if (maximamPhase >= 3) {
					maximamPhase = 3;
				}

				if (m_maximamPhase != maximamPhase) {
					switch (maximamPhase) {
					case 1:
						BGM.Get((int)ENUM_BGM::Phase1).Play(0, DX_PLAYTYPE_LOOP, TRUE);
						BGM.Get((int)ENUM_BGM::Phase1).SetVol_Local((int)(128.f*1.f));
						break;
					case 2:
						BGM.Get((int)ENUM_BGM::Phase2).Play(0, DX_PLAYTYPE_LOOP, TRUE);
						BGM.Get((int)ENUM_BGM::Phase2).SetVol_Local((int)(128.f*1.f));
						break;
					case 3:
						BGM.Get((int)ENUM_BGM::Phase3).Play(0, DX_PLAYTYPE_LOOP, TRUE);
						BGM.Get((int)ENUM_BGM::Phase3).SetVol_Local((int)(128.f*1.f));
						break;
					default:
						break;
					}
					switch (m_maximamPhase) {
					case 1:
						BGM.Get((int)ENUM_BGM::Phase1).SetVol_Local((int)(128.f*0.f));
						break;
					case 2:
						BGM.Get((int)ENUM_BGM::Phase2).SetVol_Local((int)(128.f*0.f));
						break;
					case 3:
						BGM.Get((int)ENUM_BGM::Phase3).SetVol_Local((int)(128.f*0.f));
						break;
					default:
						break;
					}
					m_maximamPhase = maximamPhase;
				}
				printfDx("%d\n", maximamPhase);
				if (m_WorldPhase > m_WorldPhase_Lowest && !FoundEnemyAny) {
					m_WorldPhaseDownTimer -= 1.f / FPS;
					if (m_WorldPhaseDownTimer < 0.f) {
						m_WorldPhaseDownTimer = m_WorldPhaseDownTimerMax;
						m_WorldPhase = std::max(m_WorldPhase - 1, m_WorldPhase_Lowest);
					}
				}
			}
			//カメラ座標設定
			m_caminfo.camerapos = cam_pos;
			//mapの設置
			for (auto& at : m_TileData) {
				auto& ti = m_Tile[at.m_postile.x][at.m_postile.y];
				ti.m_postile = at.m_postile;
				ti.m_bottom = at.m_bottom;
				ti.m_hight = at.m_hight;
				ti.m_isWall = at.m_isWall;
				ti.m_dir = at.m_dir;
				const auto xmin = y_r(tilesize) * (ti.m_postile.x + 0);
				const auto ymin = y_r(tilesize) * (ti.m_postile.y + 0);
				const auto xmax = y_r(tilesize) * (ti.m_postile.x + 1);
				const auto ymax = y_r(tilesize) * (ti.m_postile.y + 1);
				ti.m_top[0] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin), ti.m_hight, m_caminfo);
				ti.m_top[1] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin), ti.m_hight, m_caminfo);
				ti.m_top[2] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax), ti.m_hight, m_caminfo);
				ti.m_top[3] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax), ti.m_hight, m_caminfo);
				ti.m_floor[0] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin), ti.m_bottom, m_caminfo);
				ti.m_floor[1] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin), ti.m_bottom, m_caminfo);
				ti.m_floor[2] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax), ti.m_bottom, m_caminfo);
				ti.m_floor[3] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax), ti.m_bottom, m_caminfo);
				ti.m_zero[0] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymin), 0, m_caminfo);
				ti.m_zero[1] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymin), 0, m_caminfo);
				ti.m_zero[2] = ConvertPos_CalcCam(Vector2D_I::Get(xmin, ymax), 0, m_caminfo);
				ti.m_zero[3] = ConvertPos_CalcCam(Vector2D_I::Get(xmax, ymax), 0, m_caminfo);
				ti.wallhandle = &m_walls[at.m_WallID];
				ti.floorhandle = &m_floors[at.m_FloorID];
			}
			//描画用意
			for (auto& pl : m_human) {
				pl.Update(m_caminfo);
				pl.SetHight(GetTile(pl.pos));
			}
			for (auto& pl : m_human) {
				for (auto& tgt : m_human) {
					if (&tgt != &pl) {
						if (tgt.MeleeHit(&pl)) {
							tgt.Damage(&pl, m_caminfo);
							if (tgt.isDamageDown()) {
								if (tgt.isHaveGun()) {
									tgt.haveGun->Put(tgt.pos, GetTile(tgt.pos).m_hight);
								}
								tgt.SetGun(nullptr);
							}
						}
					}
				}
			}
			for (auto& mg : m_mag) { mg.Update(m_Tile); }
			for (auto& gn : m_gun) {
				gn.Update(m_caminfo);
				gn.SetHight(GetTile(gn.pos));
				for (auto& am : gn.ammo) {
					am.Update(m_Tile, m_caminfo);
					for (auto& pl : m_human) {
						if (am.GetHitHuman(pl)) {
							if (!am.isHit()) {
								m_effect[0].Set_Hit(&am, &pl);
							}
							am.Set_Hit(false);
							pl.Damage(gn.haveHuman, m_caminfo);
							if (pl.isDamageDown()) {
								if (pl.isHaveGun()) {
									pl.haveGun->Put(pl.pos, GetTile(pl.pos).m_hight);
								}
								pl.SetGun(nullptr);
							}
							break;
						}
					}
				}
				for (auto& am : gn.cart) { am.Update(m_Tile, m_caminfo); }
			}
			for (auto& ef : m_effect) {
				ef.Update();
			}
			//影
			Update_Shadow();
			//一気に描画
			m_screen.SetDraw_Screen();
			{
				DrawCommon();
				//インジケーター
				for (auto& pl : m_human) {
					if ((size_t)(&pl - &m_human.front()) != m_player_id) {
						pl.Draw_UI(m_Tile, m_caminfo);
					}
				}
				{
					int xp = y_r(12), yp = y_r(212);
					int xs = y_r(200), ys = y_r(40);
					DrawBox(xp, yp, xp + xs, yp + ys, GetColor(64, 64, 64), TRUE);
					DrawBox(xp, yp, xp + xs * (int)m_WorldPhaseDownTimer / (int)m_WorldPhaseDownTimerMax, yp + ys, GetColor(192, 0, 0), TRUE);
					DrawBox(xp, yp, xp + xs, yp + ys, GetColor(128, 128, 128), FALSE);

					yp += y_r(50);
					GetFont(y_r(40)).DrawStringFormat(xp, yp, GetColor(255, 64, 64), "Phase : %d", m_WorldPhase);
					yp += y_r(40);
					GetFont(y_r(15)).DrawStringFormat(xp + y_r(24), yp, GetColor(255, 64, 64), "Minimam : %d", m_WorldPhase_Lowest);
				}

				m_human[0].Draw_GunUp(DrawPts->disp_x, DrawPts->disp_y);
			}
		}
		//出力
		void Output(void) noexcept {
			m_screen.DrawGraph(0, 0, true);
		}
		//mapの後始末
		void Dispose(void) noexcept {
			m_walls.clear();
			m_floors.clear();
			m_human.clear();
			for (auto& T_X : m_Tile) {
				T_X.clear();
			}
			m_Tile.clear();
		}
	};
};
