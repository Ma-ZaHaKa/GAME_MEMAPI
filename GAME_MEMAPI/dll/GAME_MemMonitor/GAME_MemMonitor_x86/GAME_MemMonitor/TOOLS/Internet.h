#pragma once
#define HTTP_UTILS_H

#include <Windows.h>
#include <wininet.h>
#include <iostream>
#include <string>

#pragma comment(lib, "wininet.lib")

#define URL_INVALID 0
#define URL_HTTP    1
#define URL_HTTPS   2

int IsURL(LPCSTR lpszURL);
//BOOL ParseURL(PCHAR pszURL, PCHAR pszHost, PCHAR pszObject, bool* bIsHTTPS = nullptr);
//std::string HTTPGetRequest(LPCSTR lpszURL, INT* lpiReturnCode = nullptr);
BOOL ParseURL(const char* pszURL, char* pszHost, char* pszObject, bool* bIsHTTPS);
std::string HTTPGetRequest(std::string url, INT* lpiReturnCode = nullptr);
