#pragma once
#include "Near3DControl.hpp"

namespace Near3D {
	FontPool Fonts2;
	static const auto& GetFont2(int size) noexcept { return Fonts2.Get(size, DX_FONTTYPE_NORMAL).Get_handle(); }

	class Near3DEditer {
	private:
		/*--エディター限定--*/
		//エディター限定クラス
	public:
		class TileStatus {
		public:
			Vector2D_I m_postile;
			int m_bottom = 0, m_hight = 0;
			bool m_isWall = false;
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
				char wall_name[MAX_PATH] = "";
				char floor_name[MAX_PATH] = "";
				float m_DirectionalLight_Rad{ 0.f };
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

			void Dispose(void) noexcept {
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
			void Save(void) noexcept {
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
			bool CanUndo(void) noexcept { return this->List.size() >= 2 && this->ListItr != this->List.begin(); }
			void Undo(void) noexcept {
				this->ListItr--;
				this->Data = *this->ListItr;
			}
			//Redo
			bool CanRedo(void) noexcept { return this->List.size() >= 2 && std::next(this->ListItr, 1) != this->List.end(); }
			void Redo(void) noexcept {
				this->ListItr++;
				this->Data = *this->ListItr;
			}

			void Load_Common(void) noexcept {
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
						//const size_t s = size_t(x + y * Map_Xsize_t);
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
				this->mapdata.m_DirectionalLight_Rad = deg2rad(0);
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

			class colors {
			public:
				int r{ 0 }, g{ 0 }, b{ 0 }, a{ 255 };

				bool operator==(const colors& tgt) const noexcept {
					return (this->r == tgt.r) && (this->g == tgt.g) && (this->b == tgt.b);
				}
				void Set(int _r, int _g, int _b) {
					this->r = _r;
					this->g = _g;
					this->b = _b;
				}
			};
			class chips {
			public:
				colors m_color;
			};

			std::vector<chips> m_chip;

			void ReadSet(int Map_X_t, int Map_Y_t) {
				int Map_Xsize_t = 40;
				int Map_Ysize_t = 40;
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
				//
				{
					m_chip.clear();
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(1, 1, 1);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(18, 97, 41);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(24, 129, 55);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(75, 157, 32);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(238, 218, 166);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(239, 228, 176);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(159, 159, 159);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(127, 127, 127);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(22, 22, 22);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(136, 0, 21);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(255, 255, 255);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(100, 119, 109);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(237, 28, 36);
					m_chip.resize(m_chip.size() + 1);
					m_chip.back().m_color.Set(148, 148, 148);

					colors color_buf;
					int mapbasehandle = LoadSoftImage("data/mapdata/mapdot.bmp");
					int mapWallhandle = LoadSoftImage("data/mapdata/mapwall.bmp");
					int mapHighthandle = LoadSoftImage("data/mapdata/maphight.bmp");

					for (int y = 0; y < Map_Ysize_t; y++) {
						for (int x = 0; x < Map_Xsize_t; x++) {
							//
							GetPixelSoftImage(mapbasehandle, Map_X_t*Map_Xsize_t + x, Map_Y_t*Map_Ysize_t + y, &color_buf.r, &color_buf.g, &color_buf.b, &color_buf.a);
							int sel = 0;
							for (auto& c : m_chip) {
								if (c.m_color == color_buf) {
									sel = (int)(&c - &m_chip.front());
								}
							}
							//
							GetPixelSoftImage(mapWallhandle, Map_X_t*Map_Xsize_t + x, Map_Y_t*Map_Ysize_t + y, &color_buf.r, &color_buf.g, &color_buf.b, &color_buf.a);
							bool is_Wall = (color_buf.r < 128);
							//
							GetPixelSoftImage(mapHighthandle, Map_X_t*Map_Xsize_t + x, Map_Y_t*Map_Ysize_t + y, &color_buf.r, &color_buf.g, &color_buf.b, &color_buf.a);
							int btm_t = (color_buf.r == 0) ? 0 : std::clamp(16 * (256 - color_buf.r) / 256, 0, 64);
							//
							const size_t s = size_t(x + y * Map_Ysize_t);
							this->Data[s].Set_Tile(is_Wall, btm_t, hig, 0, sel);

							//left
							{
								int x_t = std::clamp(x - 1, 0, Map_Xsize_t), y_t = std::clamp(y, 0, Map_Ysize_t);
								GetPixelSoftImage(mapHighthandle, Map_X_t*Map_Xsize_t + x_t, Map_Y_t*Map_Ysize_t + y_t, &color_buf.r, &color_buf.g, &color_buf.b, &color_buf.a);
								int btm_t2 = (color_buf.r == 0) ? 0 : std::clamp(16 * (256 - color_buf.r) / 256, 0, 64);
								if (btm_t2 < btm_t) {
									this->Data[s].m_bottom = btm_t2;
									this->Data[s].SetGround(btm_t, 5);
								}
							}
							//right
							{
								int x_t = std::clamp(x + 1, 0, Map_Xsize_t), y_t = std::clamp(y, 0, Map_Ysize_t);
								GetPixelSoftImage(mapHighthandle, Map_X_t*Map_Xsize_t + x_t, Map_Y_t*Map_Ysize_t + y_t, &color_buf.r, &color_buf.g, &color_buf.b, &color_buf.a);
								int btm_t2 = (color_buf.r == 0) ? 0 : std::clamp(16 * (256 - color_buf.r) / 256, 0, 64);
								if (btm_t2 < btm_t) {
									this->Data[s].m_bottom = btm_t2;
									this->Data[s].SetGround(btm_t, 7);
								}
							}
							//up
							{
								int x_t = std::clamp(x, 0, Map_Xsize_t), y_t = std::clamp(y - 1, 0, Map_Ysize_t);
								GetPixelSoftImage(mapHighthandle, Map_X_t*Map_Xsize_t + x_t, Map_Y_t*Map_Ysize_t + y_t, &color_buf.r, &color_buf.g, &color_buf.b, &color_buf.a);
								int btm_t2 = (color_buf.r == 0) ? 0 : std::clamp(16 * (256 - color_buf.r) / 256, 0, 64);
								if (btm_t2 < btm_t) {
									this->Data[s].m_bottom = btm_t2;
									this->Data[s].SetGround(btm_t, 4);
								}
							}
							//down
							{
								int x_t = std::clamp(x, 0, Map_Xsize_t), y_t = std::clamp(y+1, 0, Map_Ysize_t);
								GetPixelSoftImage(mapHighthandle, Map_X_t*Map_Xsize_t + x_t, Map_Y_t*Map_Ysize_t + y_t, &color_buf.r, &color_buf.g, &color_buf.b, &color_buf.a);
								int btm_t2 = (color_buf.r == 0) ? 0 : std::clamp(16 * (256 - color_buf.r) / 256, 0, 64);
								if (btm_t2 < btm_t) {
									this->Data[s].m_bottom = btm_t2;
									this->Data[s].SetGround(btm_t, 6);
								}
							}

							//X4X
							//5X7
							//X6X
						}
					}

					DeleteSoftImage(mapbasehandle);
					DeleteSoftImage(mapWallhandle);
					DeleteSoftImage(mapHighthandle);
				}
				//mapデータ2書き込み(プレイヤー初期位置、使用テクスチャ指定)
				this->mapdata.SP[0].set(32, 32);
				this->mapdata.m_DirectionalLight_Rad = deg2rad(0);
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
	private:
		class Window {
			int selanim_m_x{ 0 }, selanim_m_y{ 0 };
			int x_pos = x_r(40), y_pos = y_r(40);
			int x_size = x_r(100), y_size = y_r(100);
			size_t count = 2;
			bool press = false;
			bool pressstart = false;
			std::function<void()> delgate = nullptr;
			bool m_isActive = false;
		public:
			void Init(int _x, int _y) {
				x_pos = _x;
				y_pos = _y;
				count = 2;
				press = false;
				delgate = nullptr;
			}

			const auto& GetXpos(void) const noexcept { return x_pos; }
			const auto& GetYpos(void) const noexcept { return y_pos; }
			const auto& GetPressStart(void) const noexcept { return pressstart; }
			const auto& IsActive(void) const noexcept { return m_isActive; }
			void SetActive(bool on) {
				m_isActive = on;
			}

			void BackGround(int xsize, int ysize, std::function<void()> doing) {
				x_size = xsize;
				y_size = ysize;
				delgate = doing;
			}

			void Draw(int mouse_x, int mouse_y) {
				if (delgate != nullptr) {
					/*
					if (!m_isActive) {
						mouse_x = -1;
						mouse_y = -1;
					}
					*/
					int tabsize = y_r(18);//タブサイズ
					int edge = 4;//縁
					int yp2 = y_pos + edge + tabsize + edge;
					//背景
					DrawBox_Shadow(x_pos, y_pos, x_pos + x_size, yp2 + y_size, GetColor(96, 96, 96));
					//タブ
					{
						if (press && pressstart) {
							pressstart = false;
						}

						auto mouse_in = in2_(mouse_x, mouse_y, x_pos + edge, y_pos + edge, x_pos + x_size - edge, y_pos + edge + tabsize);
						if (mouse_in) {
							count = std::min<size_t>(count + 1, ((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0);
						}
						else {
							count = 2;
						}
						if (count == 1) {
							press = true;
							pressstart = true;
							selanim_m_x = mouse_x - x_pos;
							selanim_m_y = mouse_y - y_pos;
						}
						if (!((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0)) {
							press = false;
							pressstart = false;
						}
						if (press) {
							x_pos = mouse_x - selanim_m_x;
							y_pos = mouse_y - selanim_m_y;
						}
						DrawBox_Shadow(x_pos + edge, y_pos + edge, x_pos + x_size - edge, y_pos + edge + tabsize, ((mouse_in) ? GetColor(174, 174, 174) : GetColor(216, 216, 216)));
					}
					//前
					delgate();
				}
			}
		};
		class Button {
			size_t count = 0;
			size_t count2 = 0;

			bool press = false;
			float m_Per{ 0.f };
		public:
			void Init(void) noexcept {
				count = 2;
				count2 = 2;
				press = false;
				m_Per = 0.f;
			}
			void ButtonSet(int mouse_x, int mouse_y, int xs, int ys, int xsize, int ysize, std::string_view buf, bool on, std::function<void()> doing1) {
				bool mouse_in = in2_(mouse_x, mouse_y, xs, ys, xs + xsize, ys + ysize);
				if (on) {
					if (mouse_in) { count = std::min<size_t>(count + 1, ((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0); }
					else { count = 2; }
				}
				if (count == 1) { doing1(); }
				DrawBox_Shadow(xs, ys, xs + xsize, ys + ysize, on ? ((mouse_in) ? GetColor(174, 174, 174) : GetColor(216, 216, 216)) : GetColor(96, 96, 96));
				GetFont2(ysize).DrawString_MID(xs + xsize / 2, ys, buf, on ? ((mouse_in) ? GetColor(0, 0, 0) : GetColor(48, 48, 48)) : GetColor(0, 0, 0));
			}
			bool Switch(void) noexcept { return (count == 1); }

			void UpDownSet(int mouse_x, int mouse_y, int xs, int ys, std::string_view buf, bool on, std::function<void()> doing1, std::function<void()> doing2) {
				int xsize = x_r(40);
				int ysize = y_r(30);
				int x2 = xs + GetFont2(y_r(40)).GetDrawWidth(buf);
				int y2 = ys + y_r(40);
				bool mouse_in1 = in2_(mouse_x, mouse_y, x2, ys, x2 + xsize, ys + ysize);
				bool mouse_in2 = in2_(mouse_x, mouse_y, x2, y2, x2 + xsize, y2 + ysize);
				if (on) {
					if (mouse_in1) { count = std::min<size_t>(count + 1, ((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0); }
					else { count = 2; }
					if (mouse_in2) { count2 = std::min<size_t>(count2 + 1, ((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0); }
					else { count2 = 2; }
				}
				if (count == 1) { doing1(); }
				if (count2 == 1) { doing2(); }
				GetFont2(y_r(40)).DrawString(xs, ys + y_r(15), buf, GetColor(48, 48, 48));
				DrawTriangle(x2 + xsize / 2 + 3, ys + 3, x2 + 3, ys + ysize + 3, x2 + xsize + 3, ys + ysize + 3, GetColor(0, 0, 0), TRUE);
				DrawTriangle(x2 + 3, y2 + 3, x2 + xsize + 3, y2 + 3, x2 + xsize / 2 + 3, y2 + ysize + 3, GetColor(0, 0, 0), TRUE);
				DrawTriangle(x2 + xsize / 2, ys, x2, ys + ysize, x2 + xsize, ys + ysize, on ? ((mouse_in1) ? GetColor(174, 174, 174) : GetColor(216, 216, 216)) : GetColor(128, 128, 128), TRUE);
				DrawTriangle(x2, y2, x2 + xsize, y2, x2 + xsize / 2, y2 + ysize, on ? ((mouse_in2) ? GetColor(174, 174, 174) : GetColor(216, 216, 216)) : GetColor(128, 128, 128), TRUE);
			}

			void SetSliderPer(float _per) noexcept { m_Per = std::clamp(_per, -1.f, 1.f);; }
			const auto GetSliderPer(void) const noexcept { return m_Per; }
			const auto GetSliderPress(void) const noexcept { return press; }

			void SliderSet(int mouse_x, int mouse_y, int xs, int ys, int xsize, int ysize, bool on) {
				bool mouse_in = in2_(mouse_x, mouse_y, xs, ys, xs + xsize, ys + ysize);
				if (on) {
					if (mouse_in) {
						count = std::min<size_t>(count + 1, ((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0) ? 2 : 0);
					}
					else {
						count = 2;
					}
					if (count == 1) {
						press = true;
					}
					if (!((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0)) {
						press = false;
					}
					if (press) {
						m_Per = std::clamp((float)(mouse_x - (xs + xsize / 2)) / (float)(xsize / 2), -1.f, 1.f);
					}
				}
				DrawBox_Shadow(xs, ys, xs + xsize, ys + ysize, GetColor(108, 108, 108));
				DrawBox_Shadow(
					xs + xsize / 2 + (int)((float)(xsize / 2)*m_Per) - 3, ys + 3,
					xs + xsize / 2 + (int)((float)(xsize / 2)*m_Per) + 3, ys + ysize - 3,
					on ? ((mouse_in) ? GetColor(174, 174, 174) : GetColor(216, 216, 216)) : GetColor(128, 128, 128));
			}
		};
	private:
		int m_mouse_x{ 0 }, m_mouse_y{ 0 };
		bool m_save{ false }, m_wallorfloor{ false }, m_isread{ false }, m_smz{ false }, m_isend{ false };
		int m_hight_s{ 0 }, m_bottom_s{ 0 };
		int m_Map_Xsize{ 0 }, m_Map_Ysize{ 0 };
		Edit m_TileEdit;
		DialogManager m_Dialog;
		bool m_TriggerWP = true;
		std::vector<bool> m_TriggerPP;
		std::vector<Window> m_Windows;	//Window
		std::vector<size_t> m_winSel;
		std::vector<Button> m_Buttons;	//button
		int m_SelectWallTex = 0;
		int m_SelectFloorTex = 0;
	private:
		//ミニウィンドウ
		void DrawWindow(void) noexcept {
			for (auto& w : m_Windows) {
				if (w.GetPressStart()) {
					size_t ID = &w - &m_Windows.front();
					for (auto& ws : m_winSel) {
						if (ws == ID) {
							//ずらし
							int SS = ((int)(&ws - &m_winSel.front()));
							for (int i = SS; i < m_winSel.size() - 1; i++) {
								m_winSel[i] = m_winSel[i + 1];
							}
							m_winSel.back() = ID;
							for (auto& w2 : m_Windows) {
								w2.SetActive(false);
							}
							w.SetActive(true);
							break;
						}
					}
				}
			}
			for (auto& ws : m_winSel) {
				m_Windows[ws].Draw(m_mouse_x, m_mouse_y);
			}
		}
	private:
		//エディター用関数
		void Init_Window1(void) noexcept {
			m_isread = false;
			m_Buttons[0].Init();
			m_Buttons[1].Init();

			m_Windows[0].Init(x_r(960 - 320), y_r(540 - 180));
			m_Windows[1].Init(0, 0);
			end_f_win1 = true;
		}
		bool end_f_win1 = true;
		bool Window1(void) noexcept {
			DrawBox(0, 0, DXDraw::Instance()->disp_x, DXDraw::Instance()->disp_y, GetColor(128, 128, 128), TRUE);

			int xsize = x_r(640), ysize = x_r(360);//ウィンドウサイズ
//背景
			m_Windows[0].BackGround(xsize, ysize, [&]() {
				//bool isActive = m_Windows[0].IsActive();
				int xp = m_Windows[0].GetXpos(), yp = m_Windows[0].GetYpos();//座標
				//int xsize = x_r(640), ysize = x_r(360);//ウィンドウサイズ
				int edge = 4;//縁
				int tabsize = y_r(18);//タブサイズ
				int yp2 = yp + edge + tabsize + edge;
				GetFont2(y_r(40)).DrawString(xp + x_r(40), yp2 + y_r(60), "プリセットを読み込みますか?", GetColor(255, 255, 0));
				m_Buttons[0].ButtonSet(m_mouse_x, m_mouse_y, xp + x_r(300), yp2 + y_r(250), x_r(300), y_r(40), "YES", true, [&]() { m_isread = true; end_f_win1 = false; });	//YES
				m_Buttons[1].ButtonSet(m_mouse_x, m_mouse_y, xp + x_r(300), yp2 + y_r(300), x_r(300), y_r(40), "NO", true, [&]() { end_f_win1 = false; });						//NO
			});

			DrawWindow();
			return end_f_win1;
		}
		void Init_Window2(void) noexcept {
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
		bool Window2(void) noexcept {
			auto caminfo = Camera_Info::Instance();
			DrawBox(0, 0, DXDraw::Instance()->disp_x, DXDraw::Instance()->disp_y, GetColor(128, 128, 128), TRUE);

			int tilesize_E2 = DXDraw::Instance()->disp_y / std::max(m_Map_Xsize, m_Map_Ysize);
			int tilesize_E = tilesize_E2 * 38 / 40;
			//マップ描画
			{
				int xpos_E = DXDraw::Instance()->disp_y / 40;
				int ypos_E = DXDraw::Instance()->disp_y / 40;
				auto cam_high = (int)((float)caminfo->camhigh_base / caminfo->camzoom);
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
							DrawGradationBox_2D(xs, ys, xe, ye, Color1, Color2, data.m_dir);
						}
						else {
							COLOR_U8 Color1 = GetColorU8(mh, mh / 2, mh / 2, 255u);
							COLOR_U8 Color2 = GetColorU8(mb, mb / 2, mb / 2, 255u);
							DrawGradationBox_2D(xs, ys, xe, ye, Color1, Color2, data.m_dir - 4);
						}
						GetFont2(y_r(40)).DrawStringFormat(m_Map_Xsize * tilesize_E2, y_r(40), GetColor(255, 255, 255), GetColor(0, 0, 0), "(%03d,%03d)", data.m_postile.x, data.m_postile.y);

						if ((GetMouseInputWithCheck() & MOUSE_INPUT_MIDDLE) != 0) {
							if (m_TriggerWP) {
								m_TriggerWP = false;
								m_TileEdit.m_way_point.emplace_back(Vector2D_I::Get(data.m_postile.x*y_r(tilesize) + y_r(tilesize) / 2, data.m_postile.y*y_r(tilesize) + y_r(tilesize) / 2));
							}
						}
						else {
							m_TriggerWP = true;
						}

						if (CheckHitKeyWithCheck(KEY_INPUT_0) != 0) {
							m_TileEdit.mapdata.SP[0] = Vector2D_I::Get(data.m_postile.x*y_r(tilesize) + y_r(tilesize) / 2, data.m_postile.y*y_r(tilesize) + y_r(tilesize) / 2);
						}
						for (int i = 0; i < m_TileEdit.PlayerSpawnPoint.size(); i++) {
							if (CheckHitKeyWithCheck(KEY_INPUT_1 + i) != 0) {
								if (m_TriggerPP[i]) {
									m_TriggerPP[i] = false;
									m_TileEdit.PlayerSpawnPoint[i].pos_p = Vector2D_I::Get(data.m_postile.x*y_r(tilesize) + y_r(tilesize) / 2, data.m_postile.y*y_r(tilesize) + y_r(tilesize) / 2);
								}
							}
							else {
								m_TriggerPP[i] = true;
							}
						}

						if ((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0) {
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
							DrawGradationBox_2D(xs, ys, xe, ye, Color1, Color2, data.m_dir);

							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
							DrawExtend_wrap(Vector2D_I::Get(xs, ys), Vector2D_I::Get(xe, ye), &(m_TileEdit.floors[data.m_FloorID]));
							SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
						}
						else {
							{
								COLOR_U8 Color1 = GetColorU8(mh, mh, mh, 255u);
								COLOR_U8 Color2 = GetColorU8(mb, mb, mb, 255u);
								DrawGradationBox_2D(xs, ys, xe, ye, Color1, Color2, data.m_dir - 4);

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
										DrawGradationBox_2D(xs, ys, xe, ye, Color1, Color2, data.m_dir - 4);

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
				int xpos_E = DXDraw::Instance()->disp_y / 40 + m_Map_Xsize * tilesize_E2 + x_r(400);
				int ypos_E = DXDraw::Instance()->disp_y / 40 + x_r(100);// + x_r(280)
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
						if ((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0) {
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
				int xpos_E = DXDraw::Instance()->disp_y / 40 + m_Map_Xsize * tilesize_E2 + x_r(400);
				int ypos_E = DXDraw::Instance()->disp_y / 40 + y_r(380);
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
						if ((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0) {
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
				auto cam_high = (int)((float)caminfo->camhigh_base / caminfo->camzoom);
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
		void Init_Window3(void) noexcept {
			m_Map_Xsize = 40;
			m_Map_Ysize = 40;
			m_Buttons[10].Init();
			m_Buttons[11].Init();
			m_Buttons[3].Init();
		}
		bool Window3(void) noexcept {
			DrawBox(0, 0, DXDraw::Instance()->disp_x, DXDraw::Instance()->disp_y, GetColor(128, 128, 128), TRUE);

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

		int map_sel_x = 0;
		int map_sel_y = 0;
		const int map_sel_xsize = 40;
		const int map_sel_ysize = 40;
		void Init_Window5(void) noexcept {
			map_sel_x = 0;
			map_sel_y = 0;
			m_Buttons[10].Init();
			m_Buttons[11].Init();
			m_Buttons[3].Init();
		}
		bool Window5(void) noexcept {
			DrawBox(0, 0, DXDraw::Instance()->disp_x, DXDraw::Instance()->disp_y, GetColor(128 - 32, 128 - 32, 128 - 32), TRUE);

			DrawBox(x_r(960 - 320), y_r(540 - 240), x_r(960 + 320), y_r(540 + 240), GetColor(128, 128, 128), TRUE);
			GetFont2(y_r(40)).DrawString(x_r(960 - 320 + 40), y_r(540 - 240 + 60), "編集するマップは?", GetColor(255, 255, 0));
			//高
			m_Buttons[10].UpDownSet(m_mouse_x, m_mouse_y, x_r(960 - 320 + 40), y_r(540 - 240 + 60 + 100), "X : " + std::to_string(map_sel_x), true,
				[&]() { if (map_sel_x < map_sel_xsize - 1) { map_sel_x++; }},
				[&]() { if (map_sel_x > 0) { map_sel_x--; }});
			//底面
			m_Buttons[11].UpDownSet(m_mouse_x, m_mouse_y, x_r(960 - 320 + 40), y_r(540 - 240 + 60 + 100 + 115), "Y : " + std::to_string(map_sel_y), true,
				[&]() { if (map_sel_y < map_sel_ysize - 1) { map_sel_y++; }},
				[&]() { if (map_sel_y > 0) { map_sel_y--; }});
			{
				int xsz = x_r(240);
				int ysz = y_r(240);
				int xm = x_r(960 + 140);
				int ym = y_r(540);
				for (int x = 0; x < map_sel_xsize; x++) {
					for (int y = 0; y < map_sel_ysize; y++) {
						int p_x = xm - xsz / 2 + x * xsz / map_sel_xsize;
						int p_y = ym - ysz / 2 + y * ysz / map_sel_ysize;
						if (map_sel_x == x && map_sel_y == y) {
							DrawBox(p_x, p_y, p_x + xsz / map_sel_xsize, p_y + ysz / map_sel_ysize, GetColor(255, 0, 0), TRUE);
						}
						DrawBox(p_x, p_y, p_x + xsz / map_sel_xsize, p_y + ysz / map_sel_ysize, GetColor(128, 128, 0), FALSE);
					}
				}
			}
			//終了
			bool end_f = true;
			m_Buttons[3].ButtonSet(m_mouse_x, m_mouse_y, x_r(960 + 320 - 340), y_r(540 + 240 - 80), x_r(300), y_r(40), "OK", true, [&]() { end_f = false; });
			return end_f;
		}
	public:
		Near3DEditer(void) noexcept {
			this->m_Windows.resize(2);
			this->m_winSel.resize(2);
			for (auto& w : m_winSel) {
				w = (m_winSel.size() - 1) - (&w - &m_winSel.front());
			}

			this->m_Buttons.resize(13 + 17 * 2);
			//ダイアログ用
			this->m_Dialog.Init();
			this->m_TileEdit.Data.clear();
		}
		~Near3DEditer(void) noexcept {
			this->m_Windows.clear();
			this->m_Buttons.clear();
			this->m_TileEdit.Data.clear();
			this->sort.clear();
			this->bone.clear();
			this->m_Graphs.clear();
		}
		//エディター用関数
		bool Map_Editer_Select(std::string* _mapname) {
			//mapデータプリセット
			Init_Window5();
			while (ProcessMessage() == 0) {
				GetMousePoint(&m_mouse_x, &m_mouse_y);
				GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
				if (!Window5()) { break; }
				DXDraw::Instance()->Screen_Flip();
			}
			*_mapname = "map" + (std::string)((map_sel_x < 10) ? "0" : "") + std::to_string(map_sel_x) + "_" + (std::string)((map_sel_y < 10) ? "0" : "") + std::to_string(map_sel_y);
			return true;
		}

		bool Map_Editer(std::string _mapname) {
			//map_data選択
#if false
			{
				Init_Window1();
				while (ProcessMessage() == 0) {
					GetMousePoint(&m_mouse_x, &m_mouse_y);
					GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
					if (!Window1()) { break; }
					DXDraw::Instance()->Screen_Flip();
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
						DXDraw::Instance()->Screen_Flip();
					}
					m_TileEdit.PreSet(m_Map_Xsize, m_Map_Ysize);
				}
			}
#else
			m_Map_Xsize = 40;
			m_Map_Ysize = 40;
			auto P = _mapname.substr(_mapname.find('p') + 1);
			int x_t = std::stoi(P.substr(0, P.find('_')));
			int y_t = std::stoi(P.substr(P.find('_') + 1));
			m_TileEdit.ReadSet(x_t, y_t);
#endif
			//エディター
			{
				Init_Window2();
				while (ProcessMessage() == 0) {
					GetMousePoint(&m_mouse_x, &m_mouse_y);
					GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
					if (!Window2()) { break; }
					DXDraw::Instance()->Screen_Flip();
				}
				if (m_isend) { return false; }
				m_TileEdit.List.clear();
			}
			m_TileEdit.Write("data/Map/" + _mapname + "/1.dat", "data/Map/" + _mapname + "/2.dat", "data/Map/" + _mapname + "/3.dat", "data/Map/" + _mapname + "/4.dat");			//mapデータ書き込み
			return true;
		}
		bool Map_builder(void) noexcept {
			for (map_sel_x = 0; map_sel_x < 40; map_sel_x++) {
				for (map_sel_y = 0; map_sel_y < 40; map_sel_y++) {
					std::string mapname = "map" + (std::string)((map_sel_x < 10) ? "0" : "") + std::to_string(map_sel_x) + "_" + (std::string)((map_sel_y < 10) ? "0" : "") + std::to_string(map_sel_y);
					auto P = mapname.substr(mapname.find('p') + 1);
					int x_t = std::stoi(P.substr(0, P.find('_')));
					int y_t = std::stoi(P.substr(P.find('_') + 1));
					m_TileEdit.ReadSet(x_t, y_t);
					m_TileEdit.Write("data/Map/" + mapname + "/1.dat", "data/Map/" + mapname + "/2.dat", "data/Map/" + mapname + "/3.dat", "data/Map/" + mapname + "/4.dat");			//mapデータ書き込み
				}
			}
		}
	private:
		std::vector<BoneSort> sort;
		std::vector<Bonesdata> bone;
		std::vector<GraphHandle> m_Graphs;
		AnimeData m_anime;
		bool m_isLoop = true;

		float xrad = 0.f, yrad = deg2rad(0);
		int X_X = 0, Y_Y = 0;
		int SELECT_ANIM = 0;
		int SELANIMW = 0;

		int SEL_BONE = 0;
	private:
		static void DrawBox_Shadow(int _xp, int _yp, int _xp2, int _yp2, unsigned int _Color = GetColor(216, 216, 216)) {
			DrawBox(_xp + 2, _yp + 2, _xp2 + 2, _yp2 + 2, GetColor(0, 0, 0), TRUE);
			DrawBox(_xp, _yp, _xp2, _yp2, _Color, TRUE);
		}
		void mouse_move(float* x_m, float* y_m, const float fov_per = 1.f) {
			int x_t, y_t;
			GetMousePoint(&x_t, &y_t);//~0.01
			if ((GetMouseInputWithCheck() & MOUSE_INPUT_RIGHT) != 0) {
				*x_m += float(std::clamp(x_t - X_X, -120, 120)) * fov_per / GetFPS();
				*y_m -= float(std::clamp(y_t - Y_Y, -120, 120)) * fov_per / GetFPS();
			}
			X_X = x_t;
			Y_Y = y_t;
			//SetMouseDispFlag(FALSE);
		}

		void LoadCharaBone(int _sel) {
			//キャラバイナリ書き込み
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
			n_t.back().parent = 28;
			n_t.back().SetDist(0.0f, 0.0f, -2.0f);
			//n_t.back().parent = 10;
			//n_t.back().SetDist(0.0f, 0.0f, 0.0f);

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
			file.open("data/Char/" + std::to_string(_sel) + ".dat", std::ios::binary | std::ios::out);
			for (auto& b : n_t) {
				file.write((char*)&b, sizeof(b));
			}
			file.close();
		}
	private:
		void Init_Window4(int _sel) noexcept {
			GraphHandle::LoadDiv("data/Char/" + std::to_string(_sel) + ".bmp", 33, 11, 3, 32, 32, &this->m_Graphs);
			this->sort.resize(this->m_Graphs.size());
			//キャラバイナリ書き込み
			LoadCharaBone(_sel);
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
				this->sort.resize(this->bone.size());
			}
			//アニメ
			for (int i = 0; i < (int)Anim_Sel::NUM; i++) {
				this->m_anime.LoadAnime("data/Char/Mot/" + std::to_string(i) + ".mot");
			}

			m_Windows[0].Init(x_r(40), y_r(40));
			m_Windows[1].Init(x_r(440), y_r(40));
			for (int i = 0; i < (int)this->m_Buttons.size(); i++) {
				m_Buttons[i].Init();
			}
		}
		bool Window4(void) noexcept {
			auto caminfo = Camera_Info::Instance();

			this->m_anime.SetSel((Anim_Sel)SELECT_ANIM);
			//アニメーション更新
			for (auto& g : this->bone) { g.edit = false; }
			this->m_anime.Update(&this->bone);
			//足跡座標指定
			bool next = false;
			do {
				next = false;
				for (auto& bo : this->bone) {
					if (!bo.edit) {
						if (bo.parent == -1) {
							bo.Update_Parent(0/*this->m_Yrad*/, 0/*this->yrad_aim*/, 0/*this->yrad_aim_speed*/);
						}
						else {
							if (!bo.Update_Child(this->bone[bo.parent], 0/*this->yrad_aim*/, 0/*this->yrad_aim_speed*/)) {
								next = true;
							}
						}
					}
				}
			} while (next);
			//高さでソート
			for (int i = 0; i < this->sort.size(); i++) { this->sort[i] = { i, this->bone[i].m_hight }; }
			std::sort(this->sort.begin(), this->sort.end(), [](const BoneSort& x, const BoneSort& y) { return x.second < y.second; });

			caminfo->camzoom = 2.f;
			//描画
			mouse_move(&yrad, &xrad, 1.f);
			xrad = std::clamp(xrad, deg2rad(-89), deg2rad(0));

			VECTOR_ref campos = ((MATRIX_ref::RotX(xrad) * MATRIX_ref::RotY(yrad)).zvec() * 200.f);
			VECTOR_ref camvec = VECTOR_ref::vget(0, 0, 0);
			VECTOR_ref camup = VECTOR_ref::vget(0, 1, 0);
			GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK, campos, camvec, camup, deg2rad(45), 1.f, 1000.f);
			DrawBox(0, 0, DXDraw::Instance()->disp_x, DXDraw::Instance()->disp_y, GetColor(128, 128, 128), TRUE);
			{
				//
				for (int y = -10; y <= 10; y++) {
					DrawLine3D(VECTOR_ref::vget(-100.f, 0, 10.f*y).get(), VECTOR_ref::vget(100.f, 0, 10.f*y).get(), GetColor(255, 255, 255));
				}
				for (int x = -10; x <= 10; x++) {
					DrawLine3D(VECTOR_ref::vget(10.f*x, 0, -100.f).get(), VECTOR_ref::vget(10.f*x, 0, 100.f).get(), GetColor(255, 255, 255));
				}
				DrawLine3D(VECTOR_ref::vget(0, 0, 0).get(), VECTOR_ref::vget(0, 0, -100).get(), GetColor(0, 0, 255));
				DrawLine3D(VECTOR_ref::vget(0, 0, 0).get(), VECTOR_ref::vget(0, 100, 0).get(), GetColor(0, 255, 0));
				DrawLine3D(VECTOR_ref::vget(0, 0, 0).get(), VECTOR_ref::vget(-100, 0, 0).get(), GetColor(255, 0, 0));
				for (auto& g : this->sort) {
					auto& b = this->bone[g.first];
					auto pos_m = b.m_pos;
					auto hight_m = b.m_hight - this->sort.front().second;
					auto P = ConvertPos_CalcCam(pos_m, 0);
					{
						auto zh = hight_m;

						VECTOR_ref m_pos;
						m_pos.Set((float)(-pos_m.x * 32 / y_r(tilesize)), (float)(zh), (float)(pos_m.y * 32 / y_r(tilesize)));

						DrawBillboard3D(m_pos.get(), 0.5f, 0.5f, 15.f, -(b.yrad + b.yr) + yrad, this->m_Graphs[g.first].get(), TRUE);

						{
							switch ((Bone_Sel)g.first) {
							case Bone_Sel::LEFTHAND:
							case Bone_Sel::LEFTARM2:
							case Bone_Sel::LEFTARM1:
							case Bone_Sel::BODYTOP:
							case Bone_Sel::RIGHTARM1:
							case Bone_Sel::RIGHTARM2:
							case Bone_Sel::RIGHTHAND:
							case Bone_Sel::HEAD:
							case Bone_Sel::BODYMIDDLE:
							case Bone_Sel::LEFTLEG3:
							case Bone_Sel::LEFTLEG2:
							case Bone_Sel::LEFTLEG1:
							case Bone_Sel::BODYBOTTOM:
							case Bone_Sel::RIGHTLEG1:
							case Bone_Sel::RIGHTLEG2:
							case Bone_Sel::RIGHTLEG3:
								DrawSphere3D(m_pos.get(), 1, 8, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);
								break;
							default:
								break;
							}
						}
					}
				}

				//2D
				caminfo->camerapos.set(y_r(tilesize) * 2, 0);
				for (auto& g : this->sort) {
					auto& b = this->bone[g.first];
					auto pos_m = b.m_pos;
					auto hight_m = b.m_hight - this->sort.front().second;
					auto P = ConvertPos_CalcCam(pos_m, 0);
					{
						auto cam_high = (int)((float)caminfo->camhigh_base / caminfo->camzoom);
						auto zh = hight_m;
						auto ConvPos = ConvertPos_CalcCam(pos_m, zh);
						DrawRota_wrap(ConvPos, float(zh + cam_high) / cam_high * (float)y_r(tilesize) / 64.f * caminfo->camzoom, b.yrad + b.yr, this->m_Graphs[g.first], TRUE);
					}
				}
			}
			//アニメ編集
			{
				int xsize = x_r(600), ysize = x_r(300);//ウィンドウサイズ
				m_Windows[1].BackGround(xsize, ysize, [&] {
					bool isActive = m_Windows[1].IsActive();
					int xp = m_Windows[1].GetXpos(), yp = m_Windows[1].GetYpos();//座標
					int xsize = x_r(600), ysize = x_r(300);//ウィンドウサイズ
					int ber_size = x_r(18);//スクロールバーサイズ
					int edge = 4;//縁
					int tabsize = y_r(18);//タブサイズ
					int yp2 = yp + edge + tabsize + edge;
					//スクロールバー
					int MAX = 8;//表示限度
					int mmax = 16;//最大数
					{
						int max_lim = ysize - edge - edge;//スクロールサイズ
						DrawBox_Shadow(
							xp + xsize - edge - ber_size, yp2 + std::max(0, max_lim*SEL_BONE / mmax) + edge,
							xp + xsize - edge, yp2 + std::min(max_lim, max_lim*(SEL_BONE + MAX) / mmax) + edge);
						if (in2_(m_mouse_x, m_mouse_y, xp, yp2, xp + xsize, yp2 + ysize)) {
							SEL_BONE = std::clamp(SEL_BONE - GetMouseWheelRotVol(), 0, mmax - MAX);
						}
					}
					//ボタンs
					int ys_fit = y_r(5);//ボタン同士の間隔
					int xsize_button = (xsize - edge - edge - edge - ber_size - y_r(20 * 3)) / 3;
					int ysize_button = (ysize - edge - ys_fit * (MAX - 1)) / MAX;//ボタンの幅計算
					{
						int z = 0, i2 = 0;
						for (auto& Frame : this->m_anime.GetNowAnim()) {
							if (z == this->m_anime.GetNowFrame()) {
								GetFont2(y_r(18)).DrawStringFormat(xp + edge, yp + edge, GetColor(0, 0, 0), GetColor(0, 0, 0), "FrameTime = %d", Frame.GetTime());
								for (int i = 0; i < (int)Bone_Sel::NUM; i++) {
									int xpos = xp + edge;
									int CanSel = SEL_BONE <= i2 && i2 < MAX + SEL_BONE;
									if (CanSel) {
										int ypos = yp2 + (ysize_button + ys_fit) * (i2 - SEL_BONE);
										switch ((Bone_Sel)i) {
										case Bone_Sel::LEFTHAND:
											GetFont2(ysize_button).DrawString(xpos, ypos, "LEFTHAND", GetColor(0, 0, 0));
											break;
										case Bone_Sel::LEFTARM2:
											GetFont2(ysize_button).DrawString(xpos, ypos, "LEFTARM2", GetColor(0, 0, 0));
											break;
										case Bone_Sel::LEFTARM1:
											GetFont2(ysize_button).DrawString(xpos, ypos, "LEFTARM1", GetColor(0, 0, 0));
											break;
										case Bone_Sel::BODYTOP:
											GetFont2(ysize_button).DrawString(xpos, ypos, "BODYTOP", GetColor(0, 0, 0));
											break;
										case Bone_Sel::RIGHTARM1:
											GetFont2(ysize_button).DrawString(xpos, ypos, "RIGHTARM1", GetColor(0, 0, 0));
											break;
										case Bone_Sel::RIGHTARM2:
											GetFont2(ysize_button).DrawString(xpos, ypos, "RIGHTARM2", GetColor(0, 0, 0));
											break;
										case Bone_Sel::RIGHTHAND:
											GetFont2(ysize_button).DrawString(xpos, ypos, "RIGHTHAND", GetColor(0, 0, 0));
											break;
										case Bone_Sel::HEAD:
											GetFont2(ysize_button).DrawString(xpos, ypos, "HEAD", GetColor(0, 0, 0));
											break;
										case Bone_Sel::BODYMIDDLE:
											GetFont2(ysize_button).DrawString(xpos, ypos, "BODYMIDDLE", GetColor(0, 0, 0));
											break;
										case Bone_Sel::LEFTLEG3:
											GetFont2(ysize_button).DrawString(xpos, ypos, "LEFTLEG3", GetColor(0, 0, 0));
											break;
										case Bone_Sel::LEFTLEG2:
											GetFont2(ysize_button).DrawString(xpos, ypos, "LEFTLEG2", GetColor(0, 0, 0));
											break;
										case Bone_Sel::LEFTLEG1:
											GetFont2(ysize_button).DrawString(xpos, ypos, "LEFTLEG1", GetColor(0, 0, 0));
											break;
										case Bone_Sel::BODYBOTTOM:
											GetFont2(ysize_button).DrawString(xpos, ypos, "BODYBOTTOM", GetColor(0, 0, 0));
											break;
										case Bone_Sel::RIGHTLEG1:
											GetFont2(ysize_button).DrawString(xpos, ypos, "RIGHTLEG1", GetColor(0, 0, 0));
											break;
										case Bone_Sel::RIGHTLEG2:
											GetFont2(ysize_button).DrawString(xpos, ypos, "RIGHTLEG2", GetColor(0, 0, 0));
											break;
										case Bone_Sel::RIGHTLEG3:
											GetFont2(ysize_button).DrawString(xpos, ypos, "RIGHTLEG3", GetColor(0, 0, 0));
											break;
										default:
											break;
										}
									}
									switch ((Bone_Sel)i) {
									case Bone_Sel::LEFTHAND:
									case Bone_Sel::LEFTARM2:
									case Bone_Sel::LEFTARM1:
									case Bone_Sel::BODYTOP:
									case Bone_Sel::RIGHTARM1:
									case Bone_Sel::RIGHTARM2:
									case Bone_Sel::RIGHTHAND:
									case Bone_Sel::HEAD:
									case Bone_Sel::BODYMIDDLE:
									case Bone_Sel::LEFTLEG3:
									case Bone_Sel::LEFTLEG2:
									case Bone_Sel::LEFTLEG1:
									case Bone_Sel::BODYBOTTOM:
									case Bone_Sel::RIGHTLEG1:
									case Bone_Sel::RIGHTLEG2:
									case Bone_Sel::RIGHTLEG3:
									{
										if (CanSel) {
											int ypos = yp2 + (ysize_button + ys_fit) * (i2 - SEL_BONE);
											xpos = xpos + xsize_button + y_r(20);
											auto& BT = m_Buttons[14 + i2 * 2];
											BT.SliderSet(m_mouse_x, m_mouse_y, xpos, ypos, xsize_button, ysize_button, isActive);
											GetFont2(ysize_button).DrawStringFormat(xpos, ypos, GetColor(0, 0, 0), GetColor(0, 0, 0), "%4d°", (int)rad2deg(Frame.GetBone(i).xrad));

											if (BT.GetSliderPress()) {
												this->m_anime.SetNowAnim_NowFrame().SetBoneData((Bone_Sel)i, "x", deg2rad(180.f*BT.GetSliderPer()));
											}
											else {
												BT.SetSliderPer(this->m_anime.SetNowAnim_NowFrame().GetBoneData((Bone_Sel)i, "x") / DX_PI_F);
											}

											xpos = xpos + xsize_button + y_r(20);
											auto& BT2 = m_Buttons[14 + i2 * 2 + 1];
											BT2.SliderSet(m_mouse_x, m_mouse_y, xpos, ypos, xsize_button, ysize_button, isActive);
											GetFont2(ysize_button).DrawStringFormat(xpos, ypos, GetColor(0, 0, 0), GetColor(0, 0, 0), "%4d°", (int)rad2deg(Frame.GetBone(i).yrad));

											if (BT2.GetSliderPress()) {
												this->m_anime.SetNowAnim_NowFrame().SetBoneData((Bone_Sel)i, "y", deg2rad(180.f*BT2.GetSliderPer()));
											}
											else {
												BT2.SetSliderPer(this->m_anime.SetNowAnim_NowFrame().GetBoneData((Bone_Sel)i, "y") / DX_PI_F);
											}
										}
										i2++;
									}
									break;
									default:
										break;
									}
								}
							}
							z++;
						}
					}
				}
				);
			}
			//アニメ選択
			{
				int xsize = x_r(300), ysize = x_r(300);//ウィンドウサイズ
				//背景
				m_Windows[0].BackGround(xsize, ysize, [&]() {
					bool isActive = m_Windows[0].IsActive();
					int xp = m_Windows[0].GetXpos(), yp = m_Windows[0].GetYpos();//座標
					int xsize = x_r(300), ysize = x_r(300);//ウィンドウサイズ
					int ber_size = x_r(18);//スクロールバーサイズ
					int edge = 4;//縁
					int tabsize = y_r(18);//タブサイズ
					int yp2 = yp + edge + tabsize + edge;
					//スクロールバー
					int MAX = 12 - 3;//表示限度
					int mmax = (int)Anim_Sel::NUM;//最大数
					{
						int max_lim = ysize - edge - edge;//スクロールサイズ
						DrawBox_Shadow(
							xp + xsize - edge - ber_size, yp2 + std::max(0, max_lim*SELANIMW / mmax) + edge,
							xp + xsize - edge, yp2 + std::min(max_lim, max_lim*(SELANIMW + MAX) / mmax) + edge);
						if (in2_(m_mouse_x, m_mouse_y, xp, yp2, xp + xsize, yp2 + ysize)) {
							SELANIMW = std::clamp(SELANIMW - GetMouseWheelRotVol(), 0, mmax - MAX);
						}
					}
					//ボタンs
					int ys_fit = y_r(5);//ボタン同士の間隔
					int xsize_button = xsize - edge - edge - edge - ber_size;
					int ysize_button = (ysize - edge - ys_fit * (MAX - 1)) / MAX;//ボタンの幅計算
					for (int i = 0; i < std::min(mmax, MAX); i++) {
						int xpos = xp + edge, ypos = yp2 + (ysize_button + ys_fit) * i;
						m_Buttons[1 + i].ButtonSet(m_mouse_x, m_mouse_y, xpos, ypos, xsize_button, ysize_button, "ANIME : " + std::to_string(i + SELANIMW), isActive, [&]() { SELECT_ANIM = i + SELANIMW; });
						if (SELECT_ANIM == i + SELANIMW) {
							DrawBox(xpos, ypos, xpos + xsize_button, ypos + ysize_button, GetColor(255, 0, 0), FALSE);
							DrawBox(xpos - 1, ypos - 1, xpos + xsize_button + 1, ypos + ysize_button + 1, GetColor(255, 0, 0), FALSE);
						}
					}
				});
			}
			//ループ設定
			m_Buttons[10].ButtonSet(m_mouse_x, m_mouse_y, x_r(10), y_r(1080 - 50 - 50 * 3), x_r(240), y_r(40), m_isLoop ? "ループする" : "ループしない", true, [&]() { m_isLoop ^= 1; });
			if (!m_isLoop && this->m_anime.isPlay() && this->m_anime.isEnd()) {
				this->m_anime.ChangePlay();
			}
			//アニメ再生
			m_Buttons[11].ButtonSet(m_mouse_x, m_mouse_y, x_r(10), y_r(1080 - 50 - 50 * 2), x_r(240), y_r(40), this->m_anime.isPlay() ? "再生中" : "一時停止", true, [&]() { this->m_anime.ChangePlay(); });
			//アニメ再生
			m_Buttons[12].ButtonSet(m_mouse_x, m_mouse_y, x_r(10), y_r(1080 - 50 - 50 * 1), x_r(240), y_r(40), "コマ送り", true, [&]() { this->m_anime.FrameAdvance(); });
			//セーブ
			m_Buttons[13].ButtonSet(m_mouse_x, m_mouse_y, x_r(10), y_r(1080 - 50), x_r(240), y_r(40), "アニメのセーブ", true, [&]() {
				this->m_anime.SaveAnime(SELECT_ANIM, "data/Char/Mot/" + std::to_string(SELECT_ANIM) + ".mot");
			});
			DrawWindow();
			//終了
			bool end_f = true;
			m_Buttons[0].ButtonSet(m_mouse_x, m_mouse_y, x_r(1920 - 340), y_r(1080 - 80), x_r(300), y_r(40), "OK", true, [&]() { end_f = false; });
			return end_f;
		}
	public:
		//
		bool Chara_Editer(int _sel) {
			Init_Window4(_sel);
			while (ProcessMessage() == 0) {
				GetMousePoint(&m_mouse_x, &m_mouse_y);
				GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
				if (!Window4()) { break; }
				DXDraw::Instance()->Screen_Flip();
			}

			this->m_anime.Dispose();

			return true;
		}
	};
}