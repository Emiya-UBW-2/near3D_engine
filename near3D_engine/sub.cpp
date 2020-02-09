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

void Draw::draw_wall(int UorL, float sx, float sy, int px, int py, int size, float high, int graphhandle) {
	if (high == 0.f)
		UorL = -1;
	switch (UorL) {
	case 0:
		if ((dispy / 2.f - (sy + size * py)) <= 0) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px + size), int(sy + size * py),//基準
				int(sx + size * px), int(sy + size * py),//基準
				graphhandle, TRUE);
		}
		break;
	case 1:
		if ((dispx / 2.f - (sx + size * px)) <= 0) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px), int(sy + size * py),//基準
				int(sx + size * px), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;
	case 2:
		if ((dispy / 2.f - (sy + size * py + size)) >= 0) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int(sx + size * px + size), int(sy + size * py + size),//基準
				int(sx + size * px), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;
	case 3:
		if ((dispx / 2.f - (sx + size * px + size)) >= 0) {
			DrawModiGraph(
				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px + size), int(sy + size * py),//基準
				int(sx + size * px + size), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;
	case 4:
		DrawBox(int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
			int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),
			int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
			int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),
			GetColor(128 - int(128.f*high), 128 - int(128.f*high), 128 - int(128.f*high))
			, TRUE
		);
		break;
	case 5://上◢
		if ((dispy / 2.f - (sy + size * py)) <= 0) {
			DrawModiGraph(
				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px + size), int(sy + size * py),//基準
				int(sx + size * px), int(sy + size * py),//基準
				graphhandle, TRUE);
		}
		break;
	case 6://下◢
		if ((dispy / 2.f - (sy + size * py + size)) >= 0) {
			DrawModiGraph(
				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int(sx + size * px + size), int(sy + size * py + size),//基準
				int(sx + size * px), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;
	case 7://上◣
		if ((dispy / 2.f - (sy + size * py)) <= 0) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px + size), int(sy + size * py),//基準
				int(sx + size * px), int(sy + size * py),//基準
				graphhandle, TRUE);
		}
		break;
	case 8://下◣
		if ((dispy / 2.f - (sy + size * py + size)) >= 0) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int(sx + size * px + size), int(sy + size * py + size),//基準
				int(sx + size * px), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;
	case 9://左◥
		if ((dispx / 2.f - (sx + size * px)) <= 0) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px), int(sy + size * py),//基準
				int(sx + size * px), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;
	case 10://右◥
		if ((dispx / 2.f - (sx + size * px + size)) >= 0) {
			DrawModiGraph(
				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px + size), int(sy + size * py),//基準
				int(sx + size * px + size), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;


	case 11://左◢
		if ((dispx / 2.f - (sx + size * px)) <= 0) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int(sx + size * px), int(sy + size * py),//基準
				int(sx + size * px), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;
	case 12://右◢
		if ((dispx / 2.f - (sx + size * px + size)) >= 0) {
			DrawModiGraph(
				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int(sx + size * px + size), int(sy + size * py),//基準
				int(sx + size * px + size), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;

	case 13:
		if ((sy + size * py) < ((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high)) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int(sx + size * px + size), int(sy + size * py),//基準
				int(sx + size * px), int(sy + size * py),//基準
				graphhandle, TRUE);
		}
		break;
	case 14:
		if ((sx + size * px) < ((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high)) {
			DrawModiGraph(
				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px), int(sy + size * py),//基準
				int(sx + size * px), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;
	case 15:
		if (((sy + size * py) - (dispy / 2.f - (sy + size * py))*high) < (sy + size * py + size)) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int((sx + size * px + size) - (dispx / 2.f - (sx + size * px + size))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px + size), int(sy + size * py + size),//基準
				int(sx + size * px), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;
	case 16:
		if (((sx + size * px) - (dispx / 2.f - (sx + size * px))*high) < (sx + size * px + size)) {
			DrawModiGraph(
				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py + size) - (dispy / 2.f - (sy + size * py + size))*high),

				int((sx + size * px) - (dispx / 2.f - (sx + size * px))*high),
				int((sy + size * py) - (dispy / 2.f - (sy + size * py))*high),

				int(sx + size * px + size), int(sy + size * py),//基準
				int(sx + size * px + size), int(sy + size * py + size),//基準
				graphhandle, TRUE);
		}
		break;


	default://床
		DrawBox(int((sx + size * px)),
			int((sy + size * py)),
			int((sx + size * px + size)),
			int((sy + size * py + size)),
			GetColor(0, 255, 0)
			, TRUE
		);
		break;
	}
}

void Draw::drw_rect(float sx, float sy, int px, int py, int size, float high, int graphhandle){
	draw_wall(0, sx, sy, px, py, size, high, graphhandle);	//縦(上)
	draw_wall(1, sx, sy, px, py, size, high, graphhandle);	//横(左)
	draw_wall(2, sx, sy, px, py, size, high, graphhandle);	//縦(下)
	draw_wall(3, sx, sy, px, py, size, high, graphhandle);	//横(右)
	draw_wall(4, sx, sy, px, py, size, high);		//天井
}

void Draw::drw_prism(int UorL, float sx, float sy, int px, int py, int size, float high, int graphhandle)
{
	UorL = std::clamp(UorL, 0, 3);
	switch (UorL){
	case 0://上
		draw_wall(13, sx, sy, px, py, size, high, graphhandle);	//縦(上)
		draw_wall(11, sx, sy, px, py, size, high, graphhandle);	//横(左)
		draw_wall(2, sx, sy, px, py, size, high, graphhandle);	//縦(下)
		draw_wall(12, sx, sy, px, py, size, high, graphhandle);	//横(右)
		break;
	case 1://左
		draw_wall(5, sx, sy, px, py, size, high, graphhandle);	//縦(上)
		draw_wall(14, sx, sy, px, py, size, high, graphhandle);	//横(左)
		draw_wall(6, sx, sy, px, py, size, high, graphhandle);	//縦(下)
		draw_wall(3, sx, sy, px, py, size, high, graphhandle);	//横(右)
		break;
	case 2://下
		draw_wall(0, sx, sy, px, py, size, high, graphhandle);	//縦(上)
		draw_wall(9, sx, sy, px, py, size, high, graphhandle);	//横(左)
		draw_wall(15, sx, sy, px, py, size, high, graphhandle);	//縦(下)
		draw_wall(10, sx, sy, px, py, size, high, graphhandle);	//横(右)
		break;
	case 3://右
		draw_wall(7, sx, sy, px, py, size, high, graphhandle);	//縦(上)
		draw_wall(1, sx, sy, px, py, size, high, graphhandle);	//横(左)
		draw_wall(8, sx, sy, px, py, size, high, graphhandle);	//縦(下)
		draw_wall(16, sx, sy, px, py, size, high, graphhandle);	//横(右)
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
