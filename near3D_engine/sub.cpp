#include "sub.hpp"

void MainClass::write_setting(void) {
	std::ofstream outputfile("data/setting.txt");
	outputfile << "YSync(1or0)=" + std::to_string(USE_YSync) + "\n";
	outputfile << "fps(30or60or120)=" + std::to_string(frate) + "\n";
	outputfile << "windowmode(1or0)=" + std::to_string(USE_windowmode) + "\n";
	outputfile << "se_vol(100~0)=" + std::to_string(se_vol * 100.f) + "\n"; //
	outputfile << "bgm_vol(100~0)=" + std::to_string(se_vol * 100.f) + "\n"; //
	outputfile.close();
}
MainClass::MainClass(void) {
	using namespace std::literals;
	//WIN32_FIND_DATA win32fdt;
	//HANDLE hFind;
	SetOutApplicationLogValidFlag(FALSE); /*log*/
	const auto mdata = FileRead_open("data/setting.txt", FALSE);
	{
		USE_YSync = bool(getparam_u(mdata));
		frate = (USE_YSync) ? 60.f : getparam_f(mdata);
		USE_windowmode = bool(getparam_u(mdata));
		se_vol = getparam_f(mdata) / 100.f;
		bgm_vol = getparam_f(mdata) / 100.f;
	}
	FileRead_close(mdata);

	//SetWindowStyleMode(4);			    /**/
	//SetWindowUserCloseEnableFlag(FALSE);		    /*alt+F4対処*/
	SetMainWindowText("near3D");		    /*name*/
	SetAeroDisableFlag(TRUE);			    /**/
	SetUseDirect3DVersion(DX_DIRECT3D_9);		    /*directX ver*/
	SetEnableXAudioFlag(FALSE);			    /**/
	Set3DSoundOneMetre(1.0f);			    /*3Dsound*/
	SetGraphMode(dispx, dispy, 32);			    /*解像度*/
	SetWaitVSyncFlag(USE_YSync);			    /*垂直同期*/
	ChangeWindowMode(USE_windowmode);		    /*窓表示*/
	DxLib_Init();					    /*init*/
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);  /*Effekseer*/
	SetAlwaysRunFlag(TRUE);				    /*background*/
	MV1SetLoadModelReMakeNormal(TRUE);		    /*法線*/
							    //SetSysCommandOffFlag(TRUE)//強制ポーズ対策()
}
MainClass::~MainClass(void) {
	DxLib_End();
}
void MainClass::Screen_Flip(void) {
	ScreenFlip();
}
void MainClass::Screen_Flip(LONGLONG waits) {
	ScreenFlip();
	if (!USE_YSync)
		while (GetNowHiPerformanceCount() - waits < 1000000.0f / frate) {}
}

Draw_lookdown::Draw_lookdown(){
	zcon.resize(siz);
	for (auto& z : zcon)
		z.resize(siz);
}

Draw_lookdown::~Draw_lookdown(){
	for (auto& z : zcon)
		z.clear();
	zcon.clear();
}

void Draw_lookdown::draw_wall(int UorL, con cont){
	if (cont.hight == 0)
		UorL = -1;

	const auto oo_1 = getpos(cont.sx + cont.size * cont.px, cont.sy + cont.size * cont.py, cont.hight, camhigh);
	const auto lo_1 = getpos(cont.sx + cont.size * cont.px + cont.size, cont.sy + cont.size * cont.py, cont.hight, camhigh);
	const auto ol_1 = getpos(cont.sx + cont.size * cont.px, cont.sy + cont.size * cont.py + cont.size, cont.hight, camhigh);
	const auto ll_1 = getpos(cont.sx + cont.size * cont.px + cont.size, cont.sy + cont.size * cont.py + cont.size, cont.hight, camhigh);

	const auto oo_0 = getpos(cont.sx + cont.size * cont.px, cont.sy + cont.size * cont.py, 0, camhigh);
	const auto lo_0 = getpos(cont.sx + cont.size * cont.px + cont.size, cont.sy + cont.size * cont.py, 0, camhigh);
	const auto ol_0 = getpos(cont.sx + cont.size * cont.px, cont.sy + cont.size * cont.py + cont.size, 0, camhigh);
	const auto ll_0 = getpos(cont.sx + cont.size * cont.px + cont.size, cont.sy + cont.size * cont.py + cont.size, 0, camhigh);

	switch (UorL) {
	case 0:
		if (oo_0.y < oo_1.y) {
			DrawModiGraph(
				oo_1.x, oo_1.y,
				lo_1.x, lo_1.y,
				lo_0.x, lo_0.y,//基準
				oo_0.x, oo_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 1:
		if (ol_1.x >= ol_0.x) {
			DrawModiGraph(
				ol_1.x, ol_1.y,
				oo_1.x, oo_1.y,
				oo_0.x, oo_0.y,//基準
				ol_0.x, ol_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 2:
		if (ol_0.y > ol_1.y) {
			DrawModiGraph(
				ol_1.x, ol_1.y,
				ll_1.x, ll_1.y,
				ll_0.x, ll_0.y,//基準
				ol_0.x, ol_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 3:
		if (ll_0.x >= ll_1.x) {
			DrawModiGraph(
				ll_1.x, ll_1.y,
				lo_1.x, lo_1.y,
				lo_0.x, lo_0.y,//基準
				ll_0.x, ll_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 4:
		DrawTriangle(
			oo_1.x, oo_1.y,
			lo_1.x, lo_1.y,
			ll_1.x, ll_1.y,
			GetColor(128 - int(128.f*cont.hight / camhigh), 128 - int(128.f*cont.hight / camhigh), 128 - int(128.f*cont.hight / camhigh))
			//GetColor(255,255,255)
			, TRUE
		);
		DrawTriangle(
			oo_1.x, oo_1.y,
			ol_1.x, ol_1.y,
			ll_1.x, ll_1.y,
			GetColor(128 - int(128.f*cont.hight / camhigh), 128 - int(128.f*cont.hight / camhigh), 128 - int(128.f*cont.hight / camhigh))
			//GetColor(255,255,255)
			, TRUE
		);
		break;
	case 5://上◢
		if (lo_0.y < lo_1.y) {
			DrawModiGraph(
				lo_1.x, lo_1.y,
				lo_1.x, lo_1.y,
				lo_0.x, lo_0.y,
				oo_0.x, oo_0.y,
				cont.graphhandle, TRUE);
		}
		break;
	case 6://下◢
		if (ll_0.y > ll_1.y) {
			DrawModiGraph(
				ll_1.x, ll_1.y,
				ll_1.x, ll_1.y,
				ll_0.x, ll_0.y,//基準
				ol_0.x, ol_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 7://上◣
		if (oo_0.y < oo_1.y) {
			DrawModiGraph(
				oo_1.x, oo_1.y,
				oo_1.x, oo_1.y,
				lo_0.x, lo_0.y,
				oo_0.x, oo_0.y,
				cont.graphhandle, TRUE);
		}
		break;
	case 8://下◣
		if (ol_0.y > ol_1.y) {
			DrawModiGraph(
				ol_1.x, ol_1.y,
				ol_1.x, ol_1.y,
				ll_0.x, ll_0.y,//基準
				ol_0.x, ol_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 9://左◥
		if (ol_1.x >= oo_1.x) {
			DrawModiGraph(
				oo_1.x, oo_1.y,
				oo_1.x, oo_1.y,
				oo_0.x, oo_0.y,
				ol_0.x, ol_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 10://右◥
		if (lo_1.x >= ll_1.x) {
			DrawModiGraph(
				lo_1.x, lo_1.y,
				lo_1.x, lo_1.y,
				lo_0.x, lo_0.y,
				ll_0.x, ll_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 11://左◢
		if (ol_1.x >= oo_1.x) {
			DrawModiGraph(
				ol_1.x, ol_1.y,
				ol_1.x, ol_1.y,
				oo_0.x, oo_0.y,
				ol_0.x, ol_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 12://右◢
		if (lo_1.x >= ll_1.x) {
			DrawModiGraph(
				ll_1.x, ll_1.y,
				ll_1.x, ll_1.y,
				lo_0.x, lo_0.y,
				ll_0.x, ll_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 13:
		if (oo_0.y < ll_1.y) {
			DrawModiGraph(
				ol_1.x, ol_1.y,
				ll_1.x, ll_1.y,
				lo_0.x, lo_0.y,
				oo_0.x, oo_0.y,
				cont.graphhandle, TRUE);
		}
		break;
	case 14:
		if (oo_0.x < ll_1.x) {
			DrawModiGraph(
				ll_1.x, ll_1.y,
				lo_1.x, lo_1.y,
				oo_0.x, oo_0.y,
				ol_0.x, ol_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 15:
		if (ol_0.y > lo_1.y) {
			DrawModiGraph(
				oo_1.x, oo_1.y,
				lo_1.x, lo_1.y,
				ll_0.x, ll_0.y,//基準
				ol_0.x, ol_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	case 16:
		if (lo_0.x > ol_1.x) {
			DrawModiGraph(
				ol_1.x, ol_1.y,
				oo_1.x, oo_1.y,
				lo_0.x, lo_0.y,
				ll_0.x, ll_0.y,//基準
				cont.graphhandle, TRUE);
		}
		break;
	default://床
		DrawTriangle(
			oo_0.x, oo_0.y,
			lo_0.x, lo_0.y,
			ll_0.x, ll_0.y,
			GetColor(0, 255, 0)
			, TRUE
		);
		DrawTriangle(
			oo_0.x, oo_0.y,
			ol_0.x, ol_0.y,
			ll_0.x, ll_0.y,
			GetColor(0, 255, 0)
			, TRUE
		);

		break;
	}
}

void Draw_lookdown::drw_rect(con cont){
	draw_wall(1, cont);	//横(左)
	draw_wall(3, cont);	//横(右)
	draw_wall(0, cont);	//縦(上)
	draw_wall(2, cont);	//縦(下)
	draw_wall(4, cont);		//天井
}

void Draw_lookdown::drw_prism(con cont){
	switch (cont.use) {
	case 0://上
		draw_wall(13, cont);	//縦(上)
		draw_wall(11, cont);	//横(左)
		draw_wall(2, cont);	//縦(下)
		draw_wall(12, cont);	//横(右)
		break;
	case 1://左
		draw_wall(5, cont);	//縦(上)
		draw_wall(14, cont);	//横(左)
		draw_wall(6, cont);	//縦(下)
		draw_wall(3, cont);	//横(右)
		break;
	case 2://下
		draw_wall(0, cont);	//縦(上)
		draw_wall(9, cont);	//横(左)
		draw_wall(15, cont);	//縦(下)
		draw_wall(10, cont);	//横(右)
		break;
	case 3://右
		draw_wall(7, cont);	//縦(上)
		draw_wall(1, cont);	//横(左)
		draw_wall(8, cont);	//縦(下)
		draw_wall(16, cont);	//横(右)
		break;
	}

}

void Draw_lookdown::put_drw(void){
	const auto cam = getpos(dispx / 2, dispy/2 , 0, camhigh);
	//DRAW
	for (int y = 1; y < siz; ++y) {
		for (int x = siz - 1; x >= 0; --x) {
			auto& z = zcon[x][y];
			if (z.dist2.x >= cam.x && z.dist1.y <= cam.y) {
				if (z.use == -1)
					drw_rect(z);
				else
					drw_prism(z);
			}
		}
		for (int x = 0; x < siz; ++x) {
			auto& z = zcon[x][y];
			if (z.dist1.x <= cam.x && z.dist1.y <= cam.y) {
				if (z.use == -1)
					drw_rect(z);
				else
					drw_prism(z);
			}
		}
	}
	for (int y = siz - 1; y >= 0; --y) {
		for (int x = siz - 1; x >= 0; --x) {
			auto& z = zcon[x][y];
			if (z.dist2.x >= cam.x && z.dist2.y >= cam.y) {
				if (z.use == -1)
					drw_rect(z);
				else
					drw_prism(z);
			}
		}
		for (int x = 0; x < siz; ++x) {
			auto& z = zcon[x][y];
			if (z.dist1.x <= cam.x && z.dist2.y >= cam.y) {
				if (z.use == -1)
					drw_rect(z);
				else
					drw_prism(z);
			}
		}
	}
}

void Draw_lookdown::set_drw_rect(int sx, int sy, int px, int py, int size, int hight, int graphhandle){
	auto& z = zcon[px][py];
	z.dist1 = getpos(sx + size * px, sy + size * py, hight, camhigh);
	z.dist2 = getpos(sx + size * px + size, sy + size * py + size, hight, camhigh);
	z.use = -1;
	z.sx = sx;
	z.sy = sy;
	z.px = px;//box
	z.py = py;//box
	z.size = size;
	z.hight = hight;
	z.graphhandle = graphhandle;
}

void Draw_lookdown::set_drw_prism(int UorL, int sx, int sy, int px, int py, int size, int hight, int graphhandle){
	auto& z = zcon[px][py];
	z.dist1 = getpos(sx + size * px, sy + size * py, hight, camhigh);
	z.dist2 = getpos(sx + size * px + size, sy + size * py + size, hight, camhigh);
	z.use = std::clamp(UorL, 0, 3);
	z.sx = sx;
	z.sy = sy;
	z.px = px;//box
	z.py = py;//box
	z.size = size;
	z.hight = hight;
	z.graphhandle = graphhandle;
}

void UIS::put_way(void) {
	waypoint = GetNowHiPerformanceCount();
	seldeb = 0;
}
void UIS::end_way(void) {
	if (seldeb < 6)
		waydeb[seldeb++] = (float)(GetNowHiPerformanceCount() - waypoint) / 1000.0f;
}
void UIS::debug(float fps, float time) {
	deb[0][0] = time;
	for (size_t j = std::size(deb) - 1; j >= 1; --j) {
		deb[j][0] = deb[j - 1][0];
	}
	for (size_t i = 0; i < std::size(waydeb); ++i) {
		if (seldeb - 1 <= i) {
			waydeb[i] = waydeb[seldeb - 1];
		}
		deb[0][i + 1] = waydeb[i];
		for (size_t j = std::size(deb) - 1; j >= 1; --j) {
			deb[j][i + 1] = deb[j - 1][i + 1];
		}
	}
	const auto c_ffff00 = GetColor(255, 255, 0);
	for (int j = 0; j < 60 - 1; ++j) {
		for (int i = 0; i < 6; ++i) {
			DrawLine(100 + j * 5, 100 + (int)(200.f - deb[j][i + 1] * 10.f), 100 + (j + 1) * 5, 100 + (int)(200.f - deb[j + 1][i + 1] * 10.f), GetColor(50, 50, 128 + 127 * i / 6));
		}
		DrawLine(100 + j * 5, 100 + (int)(200.f - deb[j][0] * 10.f), 100 + (j + 1) * 5, 100 + (int)(200.f - deb[j + 1][0] * 10.f), c_ffff00);
	}
	const auto c_ffffff = GetColor(255, 255, 255);
	DrawLine(100, 100 + 200 - 166, 100 + 60 * 5, 100 + 200 - 166, GetColor(0, 255, 0));
	DrawBox(100, 100 + 0, 100 + 60 * 5, 100 + 200, GetColor(255, 0, 0), FALSE);
	DrawFormatString(100, 100 + 0, c_ffffff, "%05.2ffps ( %.2fms)(total %.2fms)", fps, time, 1000.0f / fps);

	DrawFormatString(100, 100 + 18, c_ffffff, "%d(%.2fms)", 0, waydeb[0]);
	for (size_t j = 1; j < std::size(waydeb); ++j) {
		DrawFormatString(100, int(100 + 18 + j * 18), c_ffffff, "%d(%.2fms)", j, waydeb[j] - waydeb[j - 1u]);
	}
}

Draw_fps::Draw_fps() {
	campos = { 0, 0, 500 };
	camvec = { 0, 0, 0 };
	fov = deg2rad(90);
}

Draw_fps::~Draw_fps() {
}

void Draw_fps::set_cam(pos3D cams, pos3D vecs,int fovs){
	campos = cams;
	camvec = vecs;
	fov = deg2rad(fovs);
}

void Draw_fps::draw_dot(int sx, int sy, int sz){
	pos3D t = {sx,sy,sz};
	const auto d = getpos(t);
	if(d.z>0)
		DrawPixel(d.x, d.y, GetColor(255, 255, 255));
}

void Draw_fps::draw_line(int sx, int sy, int sz, int ex, int ey, int ez){
	pos3D s = { sx,sy,sz };
	pos3D e = { ex,ey,ez };

	pos3D t = s;
	bool on = false;
	for (int i = 0; i < 20; ++i) {
		const auto d1 = getpos(t);
		int col = std::clamp(255 - 255 * getdist(t, campos)/ 5000,0,255);
		t = { sx + (ex - sx)*i / 20,sy + (ey - sy)*i / 20,sz + (ez - sz)*i / 20 };
		const auto d2 = getpos(t);
		if (d2.z < 0) {
			if (on) {
				pos3D pt = { sx + (ex - sx)*(i - 1) / 20,sy + (ey - sy)*(i - 1) / 20,sz + (ez - sz)*(i - 1) / 20 };
				pos3D st = pt;
				for (int j = 0; j <= 10; ++j) {
					const auto d1 = getpos(pt);
					int col = std::clamp(255 - 255 * getdist(pt, campos) / 5000, 0, 255);
					pt = { st.x + (t.x - st.x)*j / 10,st.y + (t.y - st.y)*j / 10,st.z + (t.z - st.z)*j / 10 };
					const auto d2 = getpos(pt);
					DrawLine(d1.x, d1.y, d2.x, d2.y, GetColor(col*int(abs(s.x - e.x)) / getdist(s, e), col*int(abs(s.y - e.y)) / getdist(s, e), col*int(abs(s.z - e.z)) / getdist(s, e)));
					if (d2.z < 0) {
						break;
					}
				}
				break;
			}
			else
			{
				pos3D tt = t;
				for (; i < 20; ++i) {
					tt = { sx + (ex - sx)*i / 20,sy + (ey - sy)*i / 20,sz + (ez - sz)*i / 20 };
					const auto d2 = getpos(tt);
					if (d2.z < 0) {
						continue;
					}
					t = { sx + (ex - sx)*i / 20,sy + (ey - sy)*i / 20,sz + (ez - sz)*i / 20 };
					break;
				}
				pos3D pt = t;
				pos3D st = { sx + (ex - sx)*(i - 1) / 20,sy + (ey - sy)*(i - 1) / 20,sz + (ez - sz)*(i - 1) / 20 };
				for (int j = 10; j >= 0; --j) {
					const auto d1 = getpos(pt);
					int col = std::clamp(255 - 255 * getdist(pt, campos) / 5000, 0, 255);
					pt = { st.x + (t.x - st.x)*j / 10,st.y + (t.y - st.y)*j / 10,st.z + (t.z - st.z)*j / 10 };
					const auto d2 = getpos(pt);
					DrawLine(d1.x, d1.y, d2.x, d2.y, GetColor(col*int(abs(s.x - e.x)) / getdist(s, e), col*int(abs(s.y - e.y)) / getdist(s, e), col*int(abs(s.z - e.z)) / getdist(s, e)));
					if (d2.z < 0) {
						break;
					}
				}
				continue;
			}
		}
		on = true;
		DrawLine(d1.x, d1.y, d2.x, d2.y, GetColor(col*int(abs(s.x - e.x)) / getdist(s, e), col*int(abs(s.y - e.y)) / getdist(s, e), col*int(abs(s.z - e.z)) / getdist(s, e)));
	}
}

void Draw_fps::draw_boad(int sx, int sy, int sz, int graphhandle){
}

void Draw_fps::drw_rect(int sx, int sy, int sz, int graphhandle){
}

void Draw_fps::set_drw_rect(int sx, int sy, int sz, int graphhandle){
}

void Draw_fps::put_drw(void){
}
