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
	zcon.resize(40 * 40);
}

Draw_lookdown::~Draw_lookdown(){
	zcon.clear();
}

void Draw_lookdown::draw_wall(int UorL, int sx, int sy, int px, int py, int size, int hight, int graphhandle) {
	if (hight == 0)
		UorL = -1;

	const auto a1_1 = getpos(sx + size * px		, sy + size * py	, hight, camhigh);
	const auto a2_1 = getpos(sx + size * px + size	, sy + size * py	, hight, camhigh);
	const auto a3_1 = getpos(sx + size * px		, sy + size * py + size	, hight, camhigh);
	const auto a4_1 = getpos(sx + size * px + size	, sy + size * py + size	, hight, camhigh);

	const auto a1_0 = getpos(sx + size * px		, sy + size * py	, 0, camhigh);
	const auto a2_0 = getpos(sx + size * px + size	, sy + size * py	, 0, camhigh);
	const auto a3_0 = getpos(sx + size * px		, sy + size * py + size	, 0, camhigh);
	const auto a4_0 = getpos(sx + size * px + size	, sy + size * py + size	, 0, camhigh);

	switch (UorL) {
	case 0:
		if (a1_0.y < a1_1.y) {
			DrawModiGraph(
				a1_1.x, a1_1.y,
				a2_1.x, a2_1.y,
				a2_0.x, a2_0.y,//基準
				a1_0.x, a1_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 1:
		if (a3_1.x >= a3_0.x) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a1_1.x, a1_1.y,
				a1_0.x, a1_0.y,//基準
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 2:
		if (a3_0.y > a3_1.y) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a4_1.x, a4_1.y,
				a4_0.x, a4_0.y,//基準
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 3:
		if (a4_0.x >= a4_1.x) {
			DrawModiGraph(
				a4_1.x, a4_1.y,
				a2_1.x, a2_1.y,
				a2_0.x, a2_0.y,//基準
				a4_0.x, a4_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 4:
		DrawTriangle(
			a1_1.x, a1_1.y,
			a2_1.x, a2_1.y,
			a4_1.x, a4_1.y,
			GetColor(128 - int(128.f*hight / camhigh), 128 - int(128.f*hight / camhigh), 128 - int(128.f*hight / camhigh))
			//GetColor(255,255,255)
			, TRUE
		);
		DrawTriangle(
			a1_1.x, a1_1.y,
			a3_1.x, a3_1.y,
			a4_1.x, a4_1.y,
			GetColor(128 - int(128.f*hight / camhigh), 128 - int(128.f*hight / camhigh), 128 - int(128.f*hight / camhigh))
			//GetColor(255,255,255)
			, TRUE
		);
		break;
	case 5://上◢
		if (a2_0.y < a2_1.y) {
			DrawModiGraph(
				a2_1.x, a2_1.y,
				a2_1.x, a2_1.y,
				a2_0.x, a2_0.y,
				a1_0.x, a1_0.y,
				graphhandle, TRUE);
		}
		break;
	case 6://下◢
		if (a4_0.y > a4_1.y) {
			DrawModiGraph(
				a4_1.x, a4_1.y,
				a4_1.x, a4_1.y,
				a4_0.x, a4_0.y,//基準
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 7://上◣
		if (a1_0.y < a1_1.y) {
			DrawModiGraph(
				a1_1.x, a1_1.y,
				a1_1.x, a1_1.y,
				a2_0.x, a2_0.y,
				a1_0.x, a1_0.y,
				graphhandle, TRUE);
		}
		break;
	case 8://下◣
		if (a3_0.y > a3_1.y) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a3_1.x, a3_1.y,
				a4_0.x, a4_0.y,//基準
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 9://左◥
		if (a3_1.x >= a1_1.x) {
			DrawModiGraph(
				a1_1.x, a1_1.y,
				a1_1.x, a1_1.y,
				a1_0.x, a1_0.y,
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 10://右◥
		if (a2_1.x >= a4_1.x) {
			DrawModiGraph(
				a2_1.x, a2_1.y,
				a2_1.x, a2_1.y,
				a2_0.x, a2_0.y,
				a4_0.x, a4_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 11://左◢
		if (a3_1.x >= a1_1.x) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a3_1.x, a3_1.y,
				a1_0.x, a1_0.y,
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 12://右◢
		if (a2_1.x >= a4_1.x) {
			DrawModiGraph(
				a4_1.x, a4_1.y,
				a4_1.x, a4_1.y,
				a2_0.x, a2_0.y,
				a4_0.x, a4_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 13:
		if (a1_0.y < a4_1.y) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a4_1.x, a4_1.y,
				a2_0.x, a2_0.y,
				a1_0.x, a1_0.y,
				graphhandle, TRUE);
		}
		break;
	case 14:
		if (a1_0.x < a4_1.x) {
			DrawModiGraph(
				a4_1.x, a4_1.y,
				a2_1.x, a2_1.y,
				a1_0.x, a1_0.y,
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 15:
		if (a3_0.y > a2_1.y) {
			DrawModiGraph(
				a1_1.x, a1_1.y,
				a2_1.x, a2_1.y,
				a4_0.x, a4_0.y,//基準
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 16:
		if (a2_0.x > a3_1.x) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a1_1.x, a1_1.y,
				a2_0.x, a2_0.y,
				a4_0.x, a4_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	default://床
		DrawTriangle(
			a1_0.x, a1_0.y,
			a2_0.x, a2_0.y,
			a4_0.x, a4_0.y,
			GetColor(0, 255, 0)
			, TRUE
		);
		DrawTriangle(
			a1_0.x, a1_0.y,
			a3_0.x, a3_0.y,
			a4_0.x, a4_0.y,
			GetColor(0, 255, 0)
			, TRUE
		);

		break;
	}
}

void Draw_lookdown::drw_rect(int sx, int sy, int px, int py, int size, int hight, int graphhandle){
	draw_wall(1, sx, sy, px, py, size, hight, graphhandle);	//横(左)
	draw_wall(3, sx, sy, px, py, size, hight, graphhandle);	//横(右)
	draw_wall(0, sx, sy, px, py, size, hight, graphhandle);	//縦(上)
	draw_wall(2, sx, sy, px, py, size, hight, graphhandle);	//縦(下)
	draw_wall(4, sx, sy, px, py, size, hight);		//天井
}

void Draw_lookdown::drw_prism(int UorL, int sx, int sy, int px, int py, int size, int hight, int graphhandle){
	UorL = std::clamp(UorL, 0, 3);
	switch (UorL) {
	case 0://上
		draw_wall(13, sx, sy, px, py, size, hight, graphhandle);	//縦(上)
		draw_wall(11, sx, sy, px, py, size, hight, graphhandle);	//横(左)
		draw_wall(2, sx, sy, px, py, size, hight, graphhandle);	//縦(下)
		draw_wall(12, sx, sy, px, py, size, hight, graphhandle);	//横(右)
		break;
	case 1://左
		draw_wall(5, sx, sy, px, py, size, hight, graphhandle);	//縦(上)
		draw_wall(14, sx, sy, px, py, size, hight, graphhandle);	//横(左)
		draw_wall(6, sx, sy, px, py, size, hight, graphhandle);	//縦(下)
		draw_wall(3, sx, sy, px, py, size, hight, graphhandle);	//横(右)
		break;
	case 2://下
		draw_wall(0, sx, sy, px, py, size, hight, graphhandle);	//縦(上)
		draw_wall(9, sx, sy, px, py, size, hight, graphhandle);	//横(左)
		draw_wall(15, sx, sy, px, py, size, hight, graphhandle);	//縦(下)
		draw_wall(10, sx, sy, px, py, size, hight, graphhandle);	//横(右)
		break;
	case 3://右
		draw_wall(7, sx, sy, px, py, size, hight, graphhandle);	//縦(上)
		draw_wall(1, sx, sy, px, py, size, hight, graphhandle);	//横(左)
		draw_wall(8, sx, sy, px, py, size, hight, graphhandle);	//縦(下)
		draw_wall(16, sx, sy, px, py, size, hight, graphhandle);	//横(右)
		break;
	}
}

void Draw_lookdown::put_drw(void){
	const auto cam = getpos(dispx / 2, dispy / 2, 0, camhigh);
	const auto siz = 40;
	//DRAW
	for (int y = 1; y < siz; ++y) {
		//*
		for (int x = siz - 1; x >= 0; --x) {
			auto& z = zcon[x + y * siz];
			if (z.dist2.x >= cam.x && z.dist1.y <= cam.y) {
				if (z.use == -1)
					drw_rect(z.sx, z.sy, z.px, z.py, z.size, z.hight, z.graphhandle);
				else
					drw_prism(z.use, z.sx, z.sy, z.px, z.py, z.size, z.hight, z.graphhandle);
			}
		}
		//*/
		//*
		for (int x = 0; x < siz; ++x) {
			auto& z = zcon[x + y * siz];
			if (z.dist1.x <= cam.x && z.dist1.y <= cam.y) {
				if (z.use == -1)
					drw_rect(z.sx, z.sy, z.px, z.py, z.size, z.hight, z.graphhandle);
				else
					drw_prism(z.use, z.sx, z.sy, z.px, z.py, z.size, z.hight, z.graphhandle);
			}
		}
		//*/
	}
	for (int y = siz - 1; y >= 0; --y) {
		//*
		for (int x = siz - 1; x >= 0; --x) {
			auto& z = zcon[x + y* siz];
			if (z.dist2.x >= cam.x && z.dist2.y >= cam.y) {
				if (z.use == -1)
					drw_rect(z.sx, z.sy, z.px, z.py, z.size, z.hight, z.graphhandle);
				else
					drw_prism(z.use, z.sx, z.sy, z.px, z.py, z.size, z.hight, z.graphhandle);
			}
		}
		//*/
		//*
		for (int x = 0; x < siz; ++x) {
			auto& z = zcon[x + y * siz];
			if (z.dist1.x <= cam.x && z.dist2.y >= cam.y) {
				if (z.use == -1)
					drw_rect(z.sx, z.sy, z.px, z.py, z.size, z.hight, z.graphhandle);
				else
					drw_prism(z.use, z.sx, z.sy, z.px, z.py, z.size, z.hight, z.graphhandle);
			}
		}
		//*/
	}
}

void Draw_lookdown::set_drw_rect(int sx, int sy, int px, int py, int size, int hight, int graphhandle){
	const auto siz = 40;
	auto& z = zcon[px + py * siz];
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
	const auto siz = 40;
	auto& z = zcon[px + py * siz];
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
