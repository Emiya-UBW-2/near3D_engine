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






Draw::Draw(){
}

Draw::~Draw(){
}


struct pos2D {
	float x;
	float y;
};
inline pos2D getpos(float xpos, float ypos, int high, int camhigh, float xrad, float zrad) {
	pos2D p;

	p.x = float(dispx / 2) + ((float(camhigh) / cos(zrad)) *
		tanf(zrad + atan2f(xpos - float(dispx / 2), float(camhigh - high)))) *
		cos(xrad);
	p.y = float(dispy / 2) + (float(camhigh) / cos(xrad)) *
		tanf(xrad + atan2f(ypos - float(dispy / 2), float(camhigh - high))) *
		cos(zrad);

        return p;
}

inline int getxpos(float xpos, int high, int camhigh, float xrad, float zrad) {
	return int(float(dispx / 2) + 
		((float(camhigh) / cos(zrad)) * tanf(zrad + atan2f(xpos - float(dispx / 2), float(camhigh - high))))*cos(xrad)
		);
}

inline int getypos(float ypos, int high, int camhigh, float xrad, float zrad) {
	return int(float(dispy / 2) + (float(camhigh) / cos(xrad)) * tanf(xrad + atan2f(ypos - float(dispy / 2), float(camhigh - high))));
}

void Draw::draw_wall(int UorL, float sx, float sy, int px, int py, int size, int hight, int graphhandle) {
	float xrad = deg2rad(0);
	float zrad = deg2rad(0);
	int camhigh = 64;
	if (hight == 0)
		UorL = -1;

	const auto a1_1 = getpos(sx + size * px, sy + size * py, hight, camhigh, xrad, zrad);
	const auto a2_1 = getpos(sx + size * px + size, sy + size * py, hight, camhigh, xrad, zrad);
	const auto a3_1 = getpos(sx + size * px, sy + size * py + size, hight, camhigh, xrad, zrad);
	const auto a4_1 = getpos(sx + size * px + size, sy + size * py + size, hight, camhigh, xrad, zrad);

	const auto a1_0 = getpos(sx + size * px, sy + size * py, 0, camhigh, xrad, zrad);
	const auto a2_0 = getpos(sx + size * px + size, sy + size * py, 0, camhigh, xrad, zrad);
	const auto a3_0 = getpos(sx + size * px, sy + size * py + size, 0, camhigh, xrad, zrad);
	const auto a4_0 = getpos(sx + size * px + size, sy + size * py + size, 0, camhigh, xrad, zrad);

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
		if (a3_0.x < a3_1.x) {
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
		if (a4_0.x > a4_1.x) {
			DrawModiGraph(
				a4_1.x, a4_1.y,
				a2_1.x, a2_1.y,
				a2_0.x, a2_0.y,//基準
				a4_0.x, a4_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 4:
		DrawBox(
			a1_1.x, a1_1.y,
			a4_1.x, a4_1.y,
			GetColor(128 - int(128.f*hight/camhigh), 128 - int(128.f*hight / camhigh), 128 - int(128.f*hight / camhigh))
			//GetColor(255,255,255)
			, TRUE
		);
		break;
	case 5://上◢
		if ((dispy / 2.f - (sy + size * py)) <= 0) {
			DrawModiGraph(
				a2_1.x, a2_1.y,
				a2_1.x, a2_1.y,
				a2_0.x, a2_0.y,
				a1_0.x, a1_0.y,
				graphhandle, TRUE);
		}
		break;
	case 6://下◢
		if ((dispy / 2.f - getypos(sy + size * py + size, 0, camhigh, xrad, zrad)) >= 0) {
			DrawModiGraph(
				a4_1.x, a4_1.y,
				a4_1.x, a4_1.y,

				a4_0.x, a4_0.y,//基準
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 7://上◣
		if ((dispy / 2.f - (sy + size * py)) <= 0) {
			DrawModiGraph(
				a1_1.x, a1_1.y,
				a1_1.x, a1_1.y,


				a2_0.x, a2_0.y,
				a1_0.x, a1_0.y,
				graphhandle, TRUE);
		}
		break;
	case 8://下◣
		if ((dispy / 2.f - getypos(sy + size * py + size, 0, camhigh, xrad, zrad)) >= 0) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a3_1.x, a3_1.y,

				a4_0.x, a4_0.y,//基準
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 9://左◥
		if ((dispx / 2.f - getxpos(sx + size * px, 0, camhigh, xrad, zrad)) <= 0) {
			DrawModiGraph(
				a1_1.x, a1_1.y,
				a1_1.x, a1_1.y,

				a1_0.x, a1_0.y,
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 10://右◥
		if ((dispx / 2.f - getxpos(sx + size * px + size, 0, camhigh, xrad, zrad)) >= 0) {
			DrawModiGraph(
				a2_1.x, a2_1.y,
				a2_1.x, a2_1.y,

				a2_0.x, a2_0.y,
				a4_0.x, a4_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 11://左◢
		if ((dispx / 2.f - getxpos(sx + size * px, 0, camhigh, xrad, zrad)) <= 0) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a3_1.x, a3_1.y,

				a1_0.x, a1_0.y,
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 12://右◢
		if ((dispx / 2.f - getxpos(sx + size * px + size, 0, camhigh, xrad, zrad)) >= 0) {
			DrawModiGraph(
				a4_1.x, a4_1.y,
				a4_1.x, a4_1.y,

				a2_0.x, a2_0.y,
				a4_0.x, a4_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 13:
		if (getypos(sy + size * py, 0, camhigh, xrad, zrad) < getypos(sy + size * py + size, hight, camhigh, xrad, zrad)) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a4_1.x, a4_1.y,

				a2_0.x, a2_0.y,
				a1_0.x, a1_0.y,
				graphhandle, TRUE);
		}
		break;
	case 14:
		if (getxpos(sx + size * px, 0, camhigh, xrad, zrad) < getxpos(sx + size * px + size, hight, camhigh, xrad, zrad)) {
			DrawModiGraph(
				a4_1.x, a4_1.y,
				a2_1.x, a2_1.y,

				a1_0.x, a1_0.y,
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 15:
		if (getypos(sy + size * py, hight, camhigh, xrad, zrad) < getypos(sy + size * py+size, 0, camhigh, xrad, zrad)) {
			DrawModiGraph(
				a1_1.x, a1_1.y,

				a2_1.x, a2_1.y,

				a4_0.x, a4_0.y,//基準
				a3_0.x, a3_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	case 16:
		if (getxpos(sx + size * px, hight, camhigh, xrad, zrad) < getxpos(sx + size * px+size, 0, camhigh, xrad, zrad)) {
			DrawModiGraph(
				a3_1.x, a3_1.y,
				a1_1.x, a1_1.y,


				a2_0.x, a2_0.y,
				a4_0.x, a4_0.y,//基準
				graphhandle, TRUE);
		}
		break;
	default://床
		DrawBox(
			a1_0.x, a1_0.y,
			a4_0.x, a4_0.y,
			GetColor(0, 255, 0)
			, TRUE
		);
		break;
	}
}

void Draw::drw_rect(float sx, float sy, int px, int py, int size, int hight, int graphhandle){
	draw_wall(0, sx, sy, px, py, size, hight, graphhandle);	//縦(上)
	draw_wall(1, sx, sy, px, py, size, hight, graphhandle);	//横(左)
	draw_wall(2, sx, sy, px, py, size, hight, graphhandle);	//縦(下)
	draw_wall(3, sx, sy, px, py, size, hight, graphhandle);	//横(右)
	draw_wall(4, sx, sy, px, py, size, hight);		//天井
}

void Draw::drw_prism(int UorL, float sx, float sy, int px, int py, int size, int hight, int graphhandle){
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
