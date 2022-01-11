#pragma once
#include "Near3DControl.hpp"

namespace Near3D {
	FontPool Fonts2;
	static const auto& GetFont2(int size) noexcept { return Fonts2.Get(size, DX_FONTTYPE_NORMAL).Get_handle(); }

	class Near3DEditer {
	private:
		std::shared_ptr<DXDraw> DrawPts{ nullptr };			//�����p��
	private:
		/*--�G�f�B�^�[����--*/
		//�G�f�B�^�[����N���X
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
			//Data��ۑ�
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
				//map�f�[�^2�ǂݍ���(�g�p�e�N�X�`���w��)
				GraphHandle::LoadDiv(this->mapdata.wall_name, 32, 16, 2, 16, 16 * 2, &this->m_walls);
				GraphHandle::LoadDiv(this->mapdata.floor_name, 256, 16, 16, 16, 16, &this->floors);
			}

			void Read(const std::string& mapChipName, const std::string& mapTexName, const std::string& playerInfoName, const std::string& WayPointInfoName) {
				std::fstream file;
				//map�f�[�^1�ǂݍ���(�}�b�v�`�b�v)
				file.open(mapChipName.c_str(), std::ios::binary | std::ios::in);
				do {
					this->Data.resize(this->Data.size() + 1);
					file.read((char*)&this->Data.back(), sizeof(this->Data.back()));
				} while (!file.eof());
				this->Data.pop_back();
				file.close();
				//map�f�[�^2�ǂݍ���(�v���C���[�����ʒu�A�g�p�e�N�X�`���w��)
				file.open(mapTexName.c_str(), std::ios::binary | std::ios::in);
				file.read((char*)&this->mapdata, sizeof(this->mapdata));
				file.close();
				//map�f�[�^3�ǂݍ���(�G���)
				this->PlayerSpawnPoint.clear();
				file.open(playerInfoName.c_str(), std::ios::binary | std::ios::in);
				do {
					this->PlayerSpawnPoint.resize(this->PlayerSpawnPoint.size() + 1);
					file.read((char*)&this->PlayerSpawnPoint.back(), sizeof(this->PlayerSpawnPoint.back()));
				} while (!file.eof());
				this->PlayerSpawnPoint.pop_back();
				file.close();
				//map�f�[�^4�ǂݍ���(�E�F�C�|�C���g)
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
				//map�f�[�^1��������(�}�b�v�`�b�v)
				file.open(mapChipName.c_str(), std::ios::binary | std::ios::out);
				for (auto& data : this->Data) { file.write((char*)&data, sizeof(data)); }
				this->Data.clear();
				file.close();
				//map�f�[�^2��������(�v���C���[�����ʒu�A�g�p�e�N�X�`���w��)
				file.open(mapTexName.c_str(), std::ios::binary | std::ios::out);
				file.write((char*)&this->mapdata, sizeof(this->mapdata));
				file.close();
				//map�f�[�^3��������(�G���)
				file.open(playerInfoName.c_str(), std::ios::binary | std::ios::out);
				for (auto& SP_t : this->PlayerSpawnPoint) { file.write((char*)&SP_t, sizeof(SP_t)); }
				this->PlayerSpawnPoint.clear();
				file.close();
				//map�f�[�^4��������(�E�F�C�|�C���g)
				file.open(WayPointInfoName.c_str(), std::ios::binary | std::ios::out);
				for (auto& WP : this->m_way_point) { file.write((char*)&WP, sizeof(WP)); }
				this->m_way_point.clear();
				file.close();
			}

			void PreSet(int Map_Xsize_t, int Map_Ysize_t) {
				//map�f�[�^1��������(�}�b�v�`�b�v)
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
				//map�f�[�^2��������(�v���C���[�����ʒu�A�g�p�e�N�X�`���w��)
				this->mapdata.SP[0].set(32, 32);
				this->mapdata.m_light_yrad = deg2rad(0);
				strcpy_s(this->mapdata.wall_name, "data/Chip/Wall/1.bmp");
				strcpy_s(this->mapdata.floor_name, "data/Chip/Floor/1.bmp");
				//map�f�[�^3��������(�G���)
				this->PlayerSpawnPoint.clear();
				for (int i = 0; i < 7; i++) {
					this->PlayerSpawnPoint.resize(this->PlayerSpawnPoint.size() + 1);
					this->PlayerSpawnPoint.back().pos_p.x = y_r(tilesize) * 5 * (i + 1) + y_r(tilesize) / 2;
					this->PlayerSpawnPoint.back().pos_p.y = y_r(tilesize) * 5 * (i + 1) + y_r(tilesize) / 2;
				}
				//map�f�[�^4��������(�E�F�C�|�C���g)
				this->m_way_point.clear();
				Load_Common();
			}
		};
	private:
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
		//�G�f�B�^�[����
		Camera_Info m_caminfo;
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
			//�_�C�A���O�p
			m_Dialog.Init();
			m_TileEdit.Data.clear();
		}
		//�G�f�B�^�[�p�֐�
		void Init_Window1() noexcept {
			m_isread = false;
			m_Buttons[0].Init();
			m_Buttons[1].Init();
		}
		bool Window1() noexcept {
			DrawBox(x_r(960 - 320), y_r(540 - 180), x_r(960 + 320), y_r(540 + 180), GetColor(128, 128, 128), TRUE);
			GetFont2(y_r(40)).DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "�v���Z�b�g��ǂݍ��݂܂���?", GetColor(255, 255, 0));
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
			//�}�b�v�`��
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
								data.Set_Tile(true, m_bottom_s, m_hight_s, m_SelectWallTex, m_SelectFloorTex);	//��
							}
							else {
								data.Set_Tile(false, m_bottom_s, m_hight_s, m_SelectWallTex, m_SelectFloorTex);	//��
								//����̃^�C����ύX
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
			//�ǂ�����
			m_Buttons[6].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(80), x_r(400), y_r(40), "�I���^�C����ύX", true, [&]() { m_wallorfloor ^= 1; });
			GetFont2(y_r(40)).DrawString(m_Map_Xsize * tilesize_E2, y_r(80) + y_r(40), m_wallorfloor ? "�ǂ�I��" : "����I��", GetColor(255, 0, 0));
			//�ǂ�����
			m_Buttons[7].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(180), x_r(400), y_r(40), "�n�`�ҏW", true, [&]() { m_smz ^= 1; });
			GetFont2(y_r(40)).DrawString(m_Map_Xsize * tilesize_E2, y_r(180) + y_r(40), m_smz ? "��`" : "��`", GetColor(255, 0, 0));
			//���e�N�X�`��
			m_Buttons[8].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(280), x_r(400), y_r(40), "���e�N�X�`���I��", GetWindowModeFlag() == TRUE, [&]() {
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
			//�ǃe�N�X�`��
			m_Buttons[9].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(380), x_r(400), y_r(40), "�ǃe�N�X�`���I��", GetWindowModeFlag() == TRUE, [&]() {
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
			//�ݒ肷�鍂��
			{
				auto cam_high = (int)((float)m_caminfo.camhigh_base / m_caminfo.camzoom);
				//��
				m_Buttons[10].UpDownSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(480), "�ݒ肷�鍂�� : " + std::to_string(m_hight_s), true,
					[&]() { m_hight_s = std::min(m_hight_s + 8, cam_high); },
					[&]() { m_hight_s = std::max(m_hight_s - 8, -cam_high); });
				m_bottom_s = std::min(m_bottom_s, m_hight_s - 8);
				//���
				m_Buttons[11].UpDownSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(580 + 15), "�ݒ肷���� : " + std::to_string(m_bottom_s), true,
					[&]() {
					if (m_bottom_s < cam_high - 8) { m_hight_s = std::max(m_bottom_s + 8, m_hight_s); }
					else { m_bottom_s = cam_high; }
					m_bottom_s = std::min(m_bottom_s + 8, cam_high);
				},
					[&]() { m_bottom_s = std::max(m_bottom_s - 8, -cam_high); });
			}
			m_Buttons[4].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2, y_r(680), x_r(100), y_r(40), "�߂�", m_TileEdit.CanUndo(), [&]() { m_TileEdit.Undo(); });				//�A���h�D
			m_Buttons[5].ButtonSet(m_mouse_x, m_mouse_y, m_Map_Xsize * tilesize_E2 + x_r(150), y_r(680), x_r(100), y_r(40), "�i��", m_TileEdit.CanRedo(), [&]() { m_TileEdit.Redo(); });		//���h�D
			bool end_f = true;
			m_Buttons[2].ButtonSet(m_mouse_x, m_mouse_y, x_r(1920 - 340), y_r(1080 - 160), x_r(300), y_r(40), "�ۑ������I��", true, [&]() { m_isend = true; end_f = false; });		//�I��
			m_Buttons[3].ButtonSet(m_mouse_x, m_mouse_y, x_r(1920 - 340), y_r(1080 - 80), x_r(300), y_r(40), "�ۑ����đ��s", true, [&]() { end_f = false; });						//�I��
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
			GetFont2(y_r(40)).DrawString(x_r(960 - 320 + 40), y_r(540 - 180 + 60), "�}�b�v�̃T�C�Y��?", GetColor(255, 255, 0));
			//��
			m_Buttons[10].UpDownSet(m_mouse_x, m_mouse_y, x_r(960 - 320 + 40), y_r(540 - 180 + 60 + 100), "X : " + std::to_string(m_Map_Xsize), true,
				[&]() { m_Map_Xsize++; },
				[&]() { if (m_Map_Xsize > 1) { m_Map_Xsize--; } });
			//���
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
			//�I��
			bool end_f = true;
			m_Buttons[3].ButtonSet(m_mouse_x, m_mouse_y, x_r(960 + 320 - 340), y_r(540 + 180 - 80), x_r(300), y_r(40), "OK", true, [&]() { end_f = false; });
			return end_f;
		}
	public:
		Near3DEditer(std::shared_ptr<DXDraw>& _DrawPts) noexcept {
			DrawPts = _DrawPts;
		}
		//�G�f�B�^�[�p�֐�
		bool Map_Editer(std::string _mapname) {
			Init_Editer();
			//map_data�I��
			{
				Init_Window1();
				while (ProcessMessage() == 0) {
					GetMousePoint(&m_mouse_x, &m_mouse_y);
					GraphHandle::SetDraw_Screen((int)DX_SCREEN_BACK);
					if (!Window1()) { break; }
					DrawPts->Screen_Flip();
				}
				if (!m_isread) {
					//map�ǂݍ���
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
					//map�f�[�^�v���Z�b�g
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
			//�G�f�B�^�[
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
			m_TileEdit.Write("data/Map/" + _mapname + "/1.dat", "data/Map/" + _mapname + "/2.dat", "data/Map/" + _mapname + "/3.dat", "data/Map/" + _mapname + "/4.dat");			//map�f�[�^��������
			return true;
		}
	};
}