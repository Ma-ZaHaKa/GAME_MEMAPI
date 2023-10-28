//#pragma once
#include "Tools\Tools.h"
#include "Tools\XMemory.h"
#include "Tools\Pointers_Diktor.h"
#include "Windows.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>


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


void* IntPtr2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); }
int VoidPtrToInt(void* _addr) { return reinterpret_cast<int>(_addr); }



//------------------------------------PROTO----
DWORD CALLBACK MessageEntry(LPVOID);
void MessageHandler(int port, std::string message);



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

DWORD CALLBACK MessageEntry(LPVOID)
{
	int Port = 9005;
	bool HelloOnStart = false;

	//Setup(Port, HelloOnStart);
	SetupHandle(Port, HelloOnStart, MessageHandler);
	return TRUE;
}

bool IsHexStr(std::string str) { return (ToLower((str)).substr(0, 2) == "0x"); }


//std::vector<void*> pointers;
void MessageHandler(int port, std::string message)
{
	nlohmann::json json;

	//try { json = nlohmann::json::parse(ToLower(message)); }
	try { json = nlohmann::json::parse(message); }
	catch (const std::exception& e) { return; }

	if (json.contains("mode"))
	{
		std::string mode = json.at("mode");
		//int type = json.at("type");


		//  { "mode":"get_pointer", "module" : "gta3.exe", "base_offset" : "0x5412F0", "offsets" : ["0x2C0"] }
		if ((mode == "get_pointer") && json.contains("module") && json.contains("base_offset") && json.contains("offsets"))
		{
			std::string module_name = json.at("module");

			std::string _base_offset = json.at("base_offset"); // 0x123ABC
			int base_offset = 0;
			try
			{  // hex to int STOI
				if (IsHexStr(_base_offset)) { _base_offset = _base_offset.substr(2); base_offset = std::stoi(_base_offset, 0, 16); } // 16
				else { base_offset = std::stoi(_base_offset); } // 10
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "INVALID OFFSET"); return; }

			std::vector<int> offsets;
			for (const auto& offset_str : json["offsets"])
			{
				if (offset_str.is_string())
				{
					std::string _offset = offset_str.get<std::string>();
					try
					{ // hex to int STOI
						int offset = 0;
						if (IsHexStr(_offset)) { _offset = _offset.substr(2); offset = std::stoi(_offset, 0, 16); } // 16
						else { offset = std::stoi(_offset); } // 10
						offsets.push_back(offset);
					}
					catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "INVALID OFFSET"); return; }
				}
			}

			SendWebsocketSTR(port, CalcPointerByOffsets(module_name, base_offset, offsets, true));


			//--CHECKSUM
			/*std::cout << "mode: " << mode << "\n";
			std::cout << "module: " << module_name << "\n";
			std::cout << "base_offset: " << base_offset << "\n";
			for (const auto& offset : offsets) { std::cout << "offset: " << offset << "\n"; }*/
		}





		// { "mode":"get_value", "pointer":"0x13C41058", "type":"int" }
		else if ((mode == "get_value") && json.contains("pointer") && json.contains("type"))
		{ // value only int. type is interpret
			std::string _pointer = json.at("pointer");
			std::string type = json.at("type");

			if ((type != "char") && (type != "int") && (type != "float") && (type != "double") && (type != "dword")) { SendWebsocketSTR(port, "INVALID TYPE"); }

			int _intpointer = 0;
			void* pointer = NULL;

			try // pointer
			{ // hex to int STOI
				if (IsHexStr(_pointer)) { _pointer = _pointer.substr(2); _intpointer = std::stoi(_pointer, 0, 16); } // 16
				else { _intpointer = std::stoi(_pointer); } // 10
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "INVALID POINTER"); return; }



			pointer = IntPtr2VoidPtr(_intpointer);
			SendWebsocketSTR(port, ReadMemoryByStringType(pointer, type));
		}



		// 1120403456 => float 100
		// { "mode":"set_value", "pointer":"0x13C41058", "type":"int", "value":"0" }
		else if ((mode == "set_value") && json.contains("pointer") && json.contains("type") && json.contains("value"))
		{ // value only int. type is interpret
			std::string _pointer = json.at("pointer");
			std::string type = json.at("type");
			std::string _value = json.at("value");

			if ((type != "char") && (type != "int") && (type != "float") && (type != "double") && (type != "dword")) { SendWebsocketSTR(port, "INVALID TYPE"); }

			int value = 0;
			int _intpointer = 0;
			void* pointer = NULL;

			try // pointer
			{ // hex to int STOI
				if (IsHexStr(_pointer)) { _pointer = _pointer.substr(2); _intpointer = std::stoi(_pointer, 0, 16); } // 16
				else { _intpointer = std::stoi(_pointer); } // 10
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "INVALID POINTER"); return; }

			try // value
			{ // hex to int STOI
				if (IsHexStr(_value)) { _value = _value.substr(2); value = std::stoi(_value, 0, 16); } // 16
				else { value = std::stoi(_value); } // 10
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "INVALID VALUE"); return; }


			pointer = IntPtr2VoidPtr(_intpointer);
			WriteMemoryByStringType(pointer, type, value);
			SendWebsocketSTR(port, "SET OK!");
		}



		// { "mode":"get_module", "module":"WebSocketDiktor.dll" }
		else if ((mode == "get_module") && json.contains("module"))
		{
			std::string module = json.at("module");

			HMODULE hmodule = GetModuleHandleA(module.c_str());
			std::string _out = hmodule ? Pointer2String((void*)hmodule) : "~ERROR! Module not found!";

			SendWebsocketSTR(port, _out);
		}

		// { "mode":"get_modules" }
		else if ((mode == "get_modules"))
		{
			std::string _out = "";
			std::vector<std::string> modules = ListProcessModules();
			for (int i = 0; i < modules.size(); i++) { _out += modules[i]; if (!((i + 1) >= modules.size())) { _out += "~"; } }

			if (_out == "") { _out = "~ERROR! Modules not found"; }
			SendWebsocketSTR(port, _out);
		}


	}
	else { SendWebsocketSTR(port, "INVALID JSON"); }
}

