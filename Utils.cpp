#include "StdAfx.h"
#include "Utils.h"
#include <regex>

/////////////////////////////////////////////////////////////////////////////
void debugPrint(const char* fmt, ...)
{
	char* buff = (char*) malloc(2048);
	va_list vl;	
	va_start(vl, fmt);
	vsprintf(buff, fmt, vl);
	va_end(vl);
	OutputDebugStringA(buff);
	free(buff);
}

/////////////////////////////////////////////////////////////////////////////
std::string ws2s(const std::wstring& wstr)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

/////////////////////////////////////////////////////////////////////////////
std::string rxreplace(std::string buff, std::string findRx, std::string replaceRx)
{
	std::regex rx(findRx); 
	std::string out = std::regex_replace(buff, rx, replaceRx);
	return out;
}