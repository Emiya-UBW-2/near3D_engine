#include "useful.hpp"
size_t count_impl(std::basic_string_view<TCHAR> pattern) {
	WIN32_FIND_DATA win32fdt;
	size_t cnt = 0;
	const auto hFind = FindFirstFile(pattern.data(), &win32fdt);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (win32fdt.cFileName[0] != '.')
				++cnt;
		} while (FindNextFile(hFind, &win32fdt));
	}
	FindClose(hFind);
	return cnt;
}
size_t count_team(std::string stage) { return count_impl("stage/" + stage + "/team/*.txt"); }
size_t count_enemy(std::string stage) { return count_impl("stage/" + stage + "/enemy/*.txt"); }


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

std::string getright(char* p1) {
	std::string tempname = p1;
	return tempname.substr(tempname.find('=') + 1);
}
