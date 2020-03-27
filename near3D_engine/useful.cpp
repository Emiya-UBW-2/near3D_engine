#include "useful.hpp"

std::string getright(char* p1) {
	std::string tempname = p1;
	return tempname.substr(tempname.find('=') + 1);
}
std::string getleft(char* p1) {
	std::string tempname = p1;
	return tempname.substr(0, tempname.find('='));
}
const std::string getcmd(int p1, int *p2) {
	char mstr[64]; /*tank*/
	FileRead_gets(mstr, 64, p1);
	*p2 = std::stoi(getright(mstr));
	return getleft(mstr);
}
const long int getparam_i(int p1) {
	char mstr[64]; /*tank*/
	FileRead_gets(mstr, 64, p1);
	return std::stol(getright(mstr));
}
const unsigned long int getparam_u(int p2) {
	char mstr[64]; /*tank*/
	FileRead_gets(mstr, 64, p2);
	return std::stoul(getright(mstr));
}
const float getparam_f(int p1) {
	char mstr[64]; /*tank*/
	FileRead_gets(mstr, 64, p1);
	return std::stof(getright(mstr));
}
