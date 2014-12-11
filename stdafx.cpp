// stdafx.cpp : source file that includes just the standard includes
//  WebKitX.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

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
