#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <list>
#include <string>
#include <process.h>

void debugPrint(const char* fmt, ...);
std::string ws2s(const std::wstring& wstr);
