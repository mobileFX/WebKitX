#pragma once

#include <algorithm>
#include <exception>
#include <cstdint>
#include <cstdio>
#include <stdarg.h>
#include <cstdlib>
#include <cstring>

#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <fstream>

#include <atlsafe.h>

#include <Shlobj.h>
#include <Strsafe.h>

#include <sys/stat.h>
#include <sys/types.h>

#pragma comment(lib, "comsuppw.lib")

/////////////////////////////////////////////////////////////////////////////
std::string rxrepace(std::string buff, std::string find, std::string replace)
{		
	std::regex rx(find, std::regex_constants::ECMAScript|std::regex_constants::icase); 
	

	buff = std::regex_replace(buff, rx, replace);
	return buff;
}
	
/////////////////////////////////////////////////////////////////////////////
std::string BSTR_to_UTF8(BSTR& value, int len=-1)
{
	int wlen = wcslen(value);
	//int blen = SysStringLen(value);
	UINT slen = len !=-1 ? len : wlen;
	if(slen==0) return std::string("");
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, value, (int)slen, NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, value, (int)slen, &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string html_for_edit(std::string html)
{
	// Disable Scripts
	html = Replace(html, "<script", "<!--8A1D54C8-1398-417E-BC7C-B8F5CD71F7D5<script");
	html = Replace(html, "</script>", "</script>8A1D54C8-1398-417E-BC7C-B8F5CD71F7D5-->");

	return html;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string html_for_preview(std::string html)
{
	// Enable Scripts
	html = Replace(html, "<!--8A1D54C8-1398-417E-BC7C-B8F5CD71F7D5<script", "<script");
	html = Replace(html, "</script>8A1D54C8-1398-417E-BC7C-B8F5CD71F7D5-->", "</script>");

	// Remove content editable
	html = rxrepace(html, "\\s*contenteditable\\s*=\\s*\"(?:true|false)\"\\s*", "");

	// Remove __uid attributes
	html = rxrepace(html, "\\s*__uid\\s*=\\s*\"__uid_\\d+\"\\s*", "");

	return html;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CefString LPCTSTR_to_CefString(LPCTSTR buff, bool DisableScriptBlocks)
{
	CComBSTR _buff(buff);
	std::string utf8 = BSTR_to_UTF8(_buff.m_str, _buff.Length());
	if(DisableScriptBlocks)	utf8 = html_for_edit(utf8);
	CefString _out(utf8);
	return _out;
}

/////////////////////////////////////////////////////////////////////////////
BSTR UTF8_to_BSTR(const char* utf8, int len=-1)
{	
	if(len==-1) len = strlen(utf8);
	if(len==0) return SysAllocString(L"");
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8, len, NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8, len, &wstrTo[0], size_needed);	
	return SysAllocString((BSTR)(wstrTo.data()));
}

/////////////////////////////////////////////////////////////////////////////
CComBSTR CefString_to_BSTR(CefString buff, bool CleanHTML)
{
	std::string _buff(buff);
	if(CleanHTML) _buff = html_for_preview(_buff);
	CComBSTR out(UTF8_to_BSTR(_buff.c_str()));
	return out;
}

/////////////////////////////////////////////////////////////////////////////
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


/*
/////////////////////////////////////////////////////////////////////////////
std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);
	std::string dblstr(len, '\0');
	len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen,	&dblstr[0], len, NULL, NULL);
	return dblstr;
}

/////////////////////////////////////////////////////////////////////////////
BSTR STR_to_BSTR(const std::string& str)
{
	int wslen = MultiByteToWideChar(CP_ACP, 0, str.data(), str.length(), NULL, 0);
	BSTR wsdata = SysAllocStringLen(NULL, wslen);
	MultiByteToWideChar(CP_ACP, 0, str.data(), str.length(), wsdata, wslen);
	return wsdata;
}

/////////////////////////////////////////////////////////////////////////////
std::string BSTR_to_STR(BSTR bstr)
{
	int wslen = SysStringLen(bstr);
	return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}

/////////////////////////////////////////////////////////////////////////////
std::string UTF8_to_STR(const char* utf8, int len=-1)
{
	BSTR b = UTF8_to_BSTR(utf8, len);
	std::string s = BSTR_to_STR(b);
	SysFreeString(b);
	return s;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
char* STR_TO_UTF8(std::string& u)
{
	BSTR b = STR_to_BSTR(u);
	std::string s = BSTR_to_UTF8(b);
	char* c = (char*)malloc(s.size()+1);
	memset(c, 0, s.size()+1);
	memcpy(c, s.c_str(), s.size());
	return c;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
inline BSTR str2bstr(std::string& s)
{	
	return UTF8_to_BSTR(s.c_str());
}

/////////////////////////////////////////////////////////////////////////////
std::string wchar_t2string(const wchar_t *wchar)
{
	std::string str = "";
	int index = 0;
	while(wchar[index] != 0)
	{
		str += (char)wchar[index];
		++index;
	}
	return str;
}

/////////////////////////////////////////////////////////////////////////////
wchar_t *string2wchar_t(const std::string &str)
{
	wchar_t wchar[260];
	int index = 0;
	while(index < (int) str.size())
	{
		wchar[index] = (wchar_t)str[index];
		++index;
	}
	wchar[index] = 0;
	return wchar;
}

/////////////////////////////////////////////////////////////////////////////
static int StringToInt(const char* s) 
{
	return atoi(s); 
}

/////////////////////////////////////////////////////////////////////////////
int AsciiToUtf16(const char* input_buffer, uint16_t* output_buffer) 
{
	int i;
	for (i = 0; input_buffer[i] != '\0'; ++i) 
		output_buffer[i] = static_cast<unsigned char>(input_buffer[i]);
	output_buffer[i] = 0;
	return i;
}

/////////////////////////////////////////////////////////////////////////////
int Utf16ToAscii(const uint16_t* input_buffer, int length, char* output_buffer, int output_len) 
{
	if (output_len >= 0) 
	{
		if (length > output_len - 1) 
			length = output_len - 1;
	}

	for (int i = 0; i < length; ++i)
		output_buffer[i] = static_cast<char>(input_buffer[i]);

	output_buffer[length] = '\0';	
	return length;
}
*/