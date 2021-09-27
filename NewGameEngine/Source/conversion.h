#pragma once
#include <string>
#include <fstream>
#include <WinNls.h>
std::wstring s2ws(const std::string& s)
{
	std::wstring temp(s.length(), ' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}

std::string ws2s(const std::wstring& s)
{
	std::string temp(s.length(), ' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}
