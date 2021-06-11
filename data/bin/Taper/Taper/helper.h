#pragma once
#include "pch.h"

namespace helper {

void ErrorMsg(const char* err);
std::wstring A2W(std::string str);
std::string W2A(std::wstring wstr);

bool GetSubStr(const char* str, const char* first, const char* end, std::string& ret);

bool HideModule(HMODULE hModule);

}