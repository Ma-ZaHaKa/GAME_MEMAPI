//#pragma once
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

#include <nlohmann/json.hpp> //!!

using namespace std;



//-------------------BY---DIKTOR--------------------------//
using OnMessageEvent = std::function<void(int, std::string)>; // void OnMessage(int port, std::string message) { std::cout << "Port: " << port << " MESSAGE: " << message << "\n"; }
// 9 FUNCS
typedef bool (*SetupFunction)(int, bool); // bool Setup(int port, bool OnConnectHello)
SetupFunction Setup;

typedef bool (*SetupHandleFunction)(int, bool, OnMessageEvent); // bool SetupHandle(int port, bool OnConnectHello, OnMessageEvent onMessageCallback)
SetupHandleFunction SetupHandle;

typedef bool (*StopFunction)(int); // bool Stop(int port)
StopFunction Stop;

typedef bool (*GetInitStatusFunction)(int); // bool GetInitStatus(int port)
GetInitStatusFunction GetInitStatus;

typedef std::vector<std::string>(*GetMessagesFunction)(int); // std::vector<std::string> GetMessages(int port)
GetMessagesFunction GetMessages;

typedef std::vector<std::string>(*GetMessagesCLSFunction)(int); // std::vector<std::string> GetMessagesCLS(int port) // clear all stack msg by port
GetMessagesCLSFunction GetMessagesCLS;

typedef bool (*SendWebsocketSTRFunction)(int, std::string); // bool SendWebsocketSTR(int port, std::string text)
SendWebsocketSTRFunction SendWebsocketSTR;

typedef bool (*SendWebsocketJSONFunction)(int, nlohmann::json); // bool SendWebsocketJSON(int port, nlohmann::json json)
SendWebsocketJSONFunction SendWebsocketJSON;

typedef bool (*SendWebsocketSTRAsJSONFunction)(int, std::string); // bool SendWebsocketSTRAsJSON(int port, std::string text) // строка в json["data"]
SendWebsocketSTRAsJSONFunction SendWebsocketSTRAsJSON;
//----------------------------------------------------------------------------------------------------------//





DWORD CALLBACK MessageEntry(LPVOID);


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL); // ускорение загрузки библиотеки

		//InitConsole();

		const char* wslib = "WebSocketDiktor.dll";

		HMODULE ghDiktorUWS = GetModuleHandleA(wslib);
		if (!ghDiktorUWS) { ghDiktorUWS = LoadLibraryA(wslib); }
		while (true) { if (!GetProcAddress(ghDiktorUWS, "Setup")) { Sleep(50); } else { break; } } // load lib dynamic delay

		//std::cout << wslib << ": 0x" << ghDiktorUWS << "\n";

		Setup = reinterpret_cast<SetupFunction>(GetProcAddress(ghDiktorUWS, "Setup"));
		SetupHandle = reinterpret_cast<SetupHandleFunction>(GetProcAddress(ghDiktorUWS, "SetupHandle"));
		Stop = reinterpret_cast<StopFunction>(GetProcAddress(ghDiktorUWS, "Stop"));
		GetInitStatus = reinterpret_cast<GetInitStatusFunction>(GetProcAddress(ghDiktorUWS, "GetInitStatus"));
		GetMessages = reinterpret_cast<GetMessagesFunction>(GetProcAddress(ghDiktorUWS, "GetMessages"));
		GetMessagesCLS = reinterpret_cast<GetMessagesCLSFunction>(GetProcAddress(ghDiktorUWS, "GetMessagesCLS"));
		SendWebsocketSTR = reinterpret_cast<SendWebsocketSTRFunction>(GetProcAddress(ghDiktorUWS, "SendWebsocketSTR"));
		SendWebsocketJSON = reinterpret_cast<SendWebsocketJSONFunction>(GetProcAddress(ghDiktorUWS, "SendWebsocketJSON"));
		SendWebsocketSTRAsJSON = reinterpret_cast<SendWebsocketSTRAsJSONFunction>(GetProcAddress(ghDiktorUWS, "SendWebsocketSTRAsJSON"));
		Sleep(50);

		CreateThread(NULL, 0, MessageEntry, NULL, 0, NULL);
		//FreeLibrary(ghDiktorUWS);
	}
	return TRUE;
}

int main() { DllMain(NULL, DLL_PROCESS_ATTACH, NULL); std::string a = ""; std::cin >> a; return 0; }

void OnMessage(int port, std::string message) { std::cout << "Port: " << port << " MESSAGE: " << message << "\n"; }

DWORD CALLBACK MessageEntry(LPVOID)
{
	int Port = 9005;
	bool HelloOnStart = false;

	//Setup(Port, HelloOnStart);
	SetupHandle(Port, HelloOnStart, OnMessage);
	//SetupHandle(9006, HelloOnStart, OnMessage);


	//while (true) { if (!GetInitStatus(Port)) { Sleep(50); } else { break; } } // не обязательно

	//--CUSTOM METHOD
	/*while (true)
	{
		std::vector<std::string> messages = GetMessagesCLS(Port);
		if (!messages.empty())
		{
			for (const std::string& message : messages) { std::cout << "MESSAGE: " << message << "\n"; }
		}
		Sleep(500);
	}*/


	std::string a = "";
	std::cin >> a;
	Stop(Port);

	std::cout << "Port: " << Port << " Stoped!" << "\n";
	return TRUE;
}
