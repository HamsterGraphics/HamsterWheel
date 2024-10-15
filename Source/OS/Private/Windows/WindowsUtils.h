#pragma once

#include <Config.h>
#include <Containers/Vector.h>

#include <windows.h>

static hg::Vector<WCHAR> CreateWideStringFromUTF8(const char* pValue)
{
	int32 count = ::MultiByteToWideChar(CP_UTF8, 0, pValue, -1, NULL, 0);
	hg::Vector<WCHAR> target(count, 0);
	::MultiByteToWideChar(CP_UTF8, 0, pValue, -1, target.data(), count);
	return target;
}

static hg::Vector<char> CreateUTF8StringFromWide(const wchar_t* pValue)
{
	int32_t count = ::WideCharToMultiByte(CP_UTF8, 0, pValue, -1, NULL, 0, NULL, NULL);
	hg::Vector<char> target(count, 0);
	::WideCharToMultiByte(CP_UTF8, 0, pValue, -1, target.data(), count, NULL, NULL);
	return target;
}