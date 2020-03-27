#pragma once
#include "DxLib.h"
#include <string_view>
#include <string>

using std::size_t;
using std::uint8_t;
using std::int8_t;
using std::uint16_t;
inline const int dispx = (GetSystemMetrics(SM_CXSCREEN) / 2); /*描画X*/
inline const int dispy = (GetSystemMetrics(SM_CYSCREEN) / 2); /*描画Y*/
constexpr float M_GR = -9.8f;				  /*重力加速度*/
//pos->map
#define x2_(p1) (dispx / 2 + dispy * int(p1) / int((mapparts->get_maxsize() - mapparts->get_minsize()).x()))
#define y2_(p1) (dispy / 2 - dispy * int(p1) / int((mapparts->get_maxsize() - mapparts->get_minsize()).z()))
//mouse->pos
#define _2x(p1) float((mapparts->get_maxsize() - mapparts->get_minsize()).x() * int(p1 - dispx / 2) / dispy)
#define _2y(p1) float((mapparts->get_maxsize() - mapparts->get_minsize()).z() * int(dispy / 2 - p1) / dispy)
//リサイズ
#define x_r(p1) (int(p1) * dispx / 1920)
#define y_r(p1) (int(p1) * dispy / 1080)
//マウス判定
#define inm(x1, y1, x2, y2) (mousex >= x1 && mousex <= x2 && mousey >= y1 && mousey <= y2)
//重力加速度
#define m_ac(p1) (M_GR / 2.0f / p1 / p1)
//その他
#define deg2rad(p1) ((p1)*DX_PI_F / 180)					   //角度からラジアンに
#define rad2deg(p1) ((p1)*180 / DX_PI_F)					   //ラジアンから角度に
#define differential(p1, p2, p3) p1 += ((p2)-p1) * (p3)				   //p2の値に近づいていく
#define fpsdiff(p1, p2, p3) p1 += ((p2)-p1) * (1.0f - pow(1.0f - p3, frate / fps)) //p2の値に近づいていく
#define cross2D(px, py, qx, qy) (qy * px - qx * py)				   //2D外積

#ifndef INCLUDED_useful_h_
#define INCLUDED_useful_h_
typedef std::pair<size_t, int> pair;
//テキスト読み出し
const std::string getcmd(int p1, int *p2);
const long int getparam_i(int p1);
const unsigned long int getparam_u(int p2);
const float getparam_f(int p1);
std::string getright(char* p1);
std::string getleft(char* p1);
//
#endif
