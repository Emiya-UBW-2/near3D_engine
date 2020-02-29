#pragma once
#include "DxLib.h"
#include <string_view>
#include <string>

using std::size_t;
using std::uint8_t;
using std::int8_t;
using std::uint16_t;
inline const int dispx = (GetSystemMetrics(SM_CXSCREEN)/4); /*描画X*/
inline const int dispy = (GetSystemMetrics(SM_CYSCREEN)/4); /*描画Y*/

//リサイズ
#define x_r(p1) (int(p1) * dispx / 1920)
#define y_r(p1) (int(p1) * dispy / 1080)
//マウス判定
#define inm(x1, y1, x2, y2) (mousex >= x1 && mousex <= x2 && mousey >= y1 && mousey <= y2)
//その他
#define deg2rad(p1) ((p1)*DX_PI_F / 180)					   //角度からラジアンに
#define rad2deg(p1) ((p1)*180 / DX_PI_F)					   //ラジアンから角度に

#ifndef INCLUDED_useful_h_
#define INCLUDED_useful_h_

typedef std::pair<size_t, float> pair;

const long int getparam_i(int p1);
const unsigned long int getparam_u(int p2);
const float getparam_f(int p1);

std::string getright(char* p1);

size_t count_impl(std::basic_string_view<TCHAR> pattern);
size_t count_team(std::string stage);
size_t count_enemy(std::string stage);

inline int cos_i(int deg, int range) {
	return int(float(range)*cos(deg2rad(deg)));
}
inline int sin_i(int deg, int range) {
	/*
	int ans = 1;
	if (deg <= 0) { ans = -1; }
	deg -= deg/ 360 * 360;
	if (deg >= 180) { ans = -1; }
	deg %= 180;
	if (deg >= 90) { deg = 180 - deg; }

	if (deg == 0) ans *= 0;
	if (deg == 1) ans *= range * 175 / 10000;
	if (deg == 2) ans *= range * 349 / 10000;
	if (deg == 3) ans *= range * 523 / 10000;
	if (deg == 4) ans *= range * 698 / 10000;
	if (deg == 5) ans *= range * 872 / 10000;
	if (deg == 6) ans *= range * 1045 / 10000;
	if (deg == 7) ans *= range * 1219 / 10000;
	if (deg == 8) ans *= range * 1392 / 10000;
	if (deg == 9) ans *= range * 1564 / 10000;
	if (deg == 10) ans *= range * 1736 / 10000;
	if (deg == 11) ans *= range * 1908 / 10000;
	if (deg == 12) ans *= range * 2079 / 10000;
	if (deg == 13) ans *= range * 2250 / 10000;
	if (deg == 14) ans *= range * 2419 / 10000;
	if (deg == 15) ans *= range * 2588 / 10000;
	if (deg == 16) ans *= range * 2756 / 10000;
	if (deg == 17) ans *= range * 2924 / 10000;
	if (deg == 18) ans *= range * 3090 / 10000;
	if (deg == 19) ans *= range * 3256 / 10000;
	if (deg == 20) ans *= range * 3420 / 10000;
	if (deg == 21) ans *= range * 3584 / 10000;
	if (deg == 22) ans *= range * 3746 / 10000;
	if (deg == 23) ans *= range * 3907 / 10000;
	if (deg == 24) ans *= range * 4067 / 10000;
	if (deg == 25) ans *= range * 4226 / 10000;
	if (deg == 26) ans *= range * 4384 / 10000;
	if (deg == 27) ans *= range * 4540 / 10000;
	if (deg == 28) ans *= range * 4695 / 10000;
	if (deg == 29) ans *= range * 4848 / 10000;
	if (deg == 30) ans *= range * 5000 / 10000;
	if (deg == 31) ans *= range * 5150 / 10000;
	if (deg == 32) ans *= range * 5299 / 10000;
	if (deg == 33) ans *= range * 5446 / 10000;
	if (deg == 34) ans *= range * 5592 / 10000;
	if (deg == 35) ans *= range * 5736 / 10000;
	if (deg == 36) ans *= range * 5878 / 10000;
	if (deg == 37) ans *= range * 6018 / 10000;
	if (deg == 38) ans *= range * 6157 / 10000;
	if (deg == 39) ans *= range * 6293 / 10000;
	if (deg == 40) ans *= range * 6428 / 10000;
	if (deg == 41) ans *= range * 6561 / 10000;
	if (deg == 42) ans *= range * 6691 / 10000;
	if (deg == 43) ans *= range * 6820 / 10000;
	if (deg == 44) ans *= range * 6947 / 10000;
	if (deg == 45) ans *= range * 7071 / 10000;
	if (deg == 46) ans *= range * 7193 / 10000;
	if (deg == 47) ans *= range * 7314 / 10000;
	if (deg == 48) ans *= range * 7431 / 10000;
	if (deg == 49) ans *= range * 7547 / 10000;
	if (deg == 50) ans *= range * 7660 / 10000;
	if (deg == 51) ans *= range * 7771 / 10000;
	if (deg == 52) ans *= range * 7880 / 10000;
	if (deg == 53) ans *= range * 7986 / 10000;
	if (deg == 54) ans *= range * 8090 / 10000;
	if (deg == 55) ans *= range * 8192 / 10000;
	if (deg == 56) ans *= range * 8290 / 10000;
	if (deg == 57) ans *= range * 8387 / 10000;
	if (deg == 58) ans *= range * 8480 / 10000;
	if (deg == 59) ans *= range * 8572 / 10000;
	if (deg == 60) ans *= range * 8660 / 10000;
	if (deg == 61) ans *= range * 8746 / 10000;
	if (deg == 62) ans *= range * 8829 / 10000;
	if (deg == 63) ans *= range * 8910 / 10000;
	if (deg == 64) ans *= range * 8988 / 10000;
	if (deg == 65) ans *= range * 9063 / 10000;
	if (deg == 66) ans *= range * 9135 / 10000;
	if (deg == 67) ans *= range * 9205 / 10000;
	if (deg == 68) ans *= range * 9272 / 10000;
	if (deg == 69) ans *= range * 9336 / 10000;
	if (deg == 70) ans *= range * 9397 / 10000;
	if (deg == 71) ans *= range * 9455 / 10000;
	if (deg == 72) ans *= range * 9511 / 10000;
	if (deg == 73) ans *= range * 9563 / 10000;
	if (deg == 74) ans *= range * 9613 / 10000;
	if (deg == 75) ans *= range * 9659 / 10000;
	if (deg == 76) ans *= range * 9703 / 10000;
	if (deg == 77) ans *= range * 9744 / 10000;
	if (deg == 78) ans *= range * 9781 / 10000;
	if (deg == 79) ans *= range * 9816 / 10000;
	if (deg == 80) ans *= range * 9848 / 10000;
	if (deg == 81) ans *= range * 9877 / 10000;
	if (deg == 82) ans *= range * 9903 / 10000;
	if (deg == 83) ans *= range * 9925 / 10000;
	if (deg == 84) ans *= range * 9945 / 10000;
	if (deg == 85) ans *= range * 9962 / 10000;
	if (deg == 86) ans *= range * 9976 / 10000;
	if (deg == 87) ans *= range * 9986 / 10000;
	if (deg == 88) ans *= range * 9994 / 10000;
	if (deg == 89) ans *= range * 9998 / 10000;
	if (deg == 90) ans *= range;

	return ans;

	//*/
	return int(float(range)*sin(deg2rad(deg)));
}

//高速距離算出
inline int CalcDist(int x, int y) {
	x = abs(x);
	y = abs(y);
	int max = x, min = y;
	if (max < min) {
		max = y;
		min = x;
	}
	int leng = max * 1007 / 1024 + min * 441 / 1024;
	if (max < 16 * min)
		leng -= max * 5 / 128;
	return leng;
}
/*
//従来型
inline int CalcDist(int x, int y) {
	return int(hypotf(float(x), float(y)));
}
*/
#endif
