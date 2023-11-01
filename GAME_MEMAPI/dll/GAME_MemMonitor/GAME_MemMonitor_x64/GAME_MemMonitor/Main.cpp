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


//void* IntPtr2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); }
//int VoidPtrToInt(void* _addr) { return reinterpret_cast<int>(_addr); }
//
//void* LLIntPtr2VoidPtr(long long int _addr) { return reinterpret_cast<void*>(_addr); }
//long long int VoidPtrToLLInt(void* _addr) { return reinterpret_cast<long long int>(_addr); }

//intptr_t VoidPtrToInt(void* _addr) { return reinterpret_cast<intptr_t>(_addr); }
//void* IntPtr2VoidPtr(intptr_t _addr) { return reinterpret_cast<void*>(_addr); }

uintptr_t VoidPtrToInt(void* _addr) { return reinterpret_cast<uintptr_t>(_addr); }
void* IntPtr2VoidPtr(uintptr_t _addr) { return reinterpret_cast<void*>(_addr); }
//std::string Pointer2String(void* pointer) { std::stringstream ss; ss << pointer; return "0x" + ss.str(); } //Tools.h

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




		//  { "mode":"get_pointer", "module" : "fmodstudio.dll", "base_offset" : "0x158B50", "offsets" : ["0x5C0", "0x8", "0x60", "0x60", "0x18", "0x208"] }
		//  { "mode":"get_pointer", "module" : "gta3.exe", "base_offset" : "0x5412F0", "offsets" : ["0x2C0"] }
		if ((mode == "get_pointer") && json.contains("module") && json.contains("base_offset") && json.contains("offsets"))
		{
			std::string module_name = json.at("module");

			std::string _base_offset = json.at("base_offset"); // 0x123ABC
			uintptr_t base_offset = 0;
			try
			{  // hex to int STOI
				if (IsHex(_base_offset)) { _base_offset = HexNormalize(_base_offset); base_offset = std::stoi(_base_offset, 0, 16); } // 16
				else { base_offset = std::stoll(_base_offset); } // 10
				//if (IsHexStr(_base_offset)) { _base_offset = _base_offset.substr(2); } // 16
				//base_offset = Parseuintptr_t(_base_offset);
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "~ERROR! INVALID OFFSET"); return; }

			std::vector<uintptr_t> offsets;
			for (const auto& offset_str : json["offsets"])
			{
				if (offset_str.is_string())
				{
					std::string _offset = offset_str.get<std::string>();
					try
					{ // hex to int STOI !!! NOT uintptr_t PARSE
						uintptr_t offset = 0;
						if (IsHex(_offset)) { _offset = HexNormalize(_offset); offset = std::stoi(_offset, 0, 16); } // 16
						else { offset = std::stoi(_offset); } // 10
						//if (IsHexStr(_offset)) { _offset = _offset.substr(2); } // 16
						//offset = Parseuintptr_t(_offset);
						offsets.push_back(offset);
					}
					catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "~ERROR! INVALID OFFSET"); return; }
				}
			}

			SendWebsocketSTR(port, CalcPointerByOffsets(module_name, base_offset, offsets, true));


			//--CHECKSUM
			/*std::cout << "mode: " << mode << "\n";
			std::cout << "module: " << module_name << "\n";
			std::cout << "base_offset: " << base_offset << "\n";
			for (const auto& offset : offsets) { std::cout << "offset: " << offset << "\n"; }*/
		}


		//  { "mode":"find_pattern", "pointer" : "0x7F", "pattern" : "55 8B EC 56 8B 75 08 68 ? ? ? ? 56 E8 ? ? ? ? 83 84 C0", "block_size" : "0x2C0" }
		else if ((mode == "find_pattern") && json.contains("pointer") && json.contains("pattern") && json.contains("block_size"))
		{
			std::string _pointer = json.at("pointer");
			std::string pattern = json.at("pattern");
			std::string _block_size = json.at("block_size");


			uintptr_t _intpointer = 0;
			void* pointer = NULL;

			try // pointer
			{ // hex to int STOI
				if (IsHex(_pointer)) { _pointer = HexNormalize(_pointer); _intpointer = std::stoll(_pointer, 0, 16); } // 16
				else { _intpointer = std::stoll(_pointer); } // 10
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "~ERROR! INVALID pointer (try_catch)"); return; }
			pointer = IntPtr2VoidPtr(_intpointer);
			if (!pointer) { SendWebsocketSTR(port, "~ERROR! INVALID pointer (nullptr)"); return; }


			//uintptr_t block_size = 0;
			int block_size = 0;
			try
			{  // hex to int STOI
				if (IsHex(_block_size)) { _block_size = HexNormalize(_block_size); block_size = std::stoi(_block_size, 0, 16); } // 16
				else { block_size = std::stoll(_block_size); } // 10
				//if (IsHexStr(_base_offset)) { _base_offset = _base_offset.substr(2); } // 16
				//base_offset = Parseuintptr_t(_base_offset);
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "~ERROR! INVALID block_size"); return; }

			//void* res_pointer = NULL;
			void* res_pointer = InlineSearchPointerByPattern(pointer, block_size, pattern);
			//if (res_pointer == nullptr) {}
			if (!res_pointer) { SendWebsocketSTR(port, "~ERROR! POINTER NOT FOUND"); }
			else { SendWebsocketSTR(port, Pointer2String(res_pointer)); }
		}



		// { "mode":"get_value", "pointer":"0x13C41058", "type":"int" }
		else if ((mode == "get_value") && json.contains("pointer") && json.contains("type"))
		{ // value only int. type is interpret
			std::string _pointer = json.at("pointer");
			std::string type = json.at("type");

			if (!ValidTypes(type)) { SendWebsocketSTR(port, "~ERROR! INVALID TYPE"); return; }

			uintptr_t _intpointer = 0;
			void* pointer = NULL;

			try // pointer
			{ // hex to int STOI
				if (IsHex(_pointer)) { _pointer = HexNormalize(_pointer); _intpointer = std::stoll(_pointer, 0, 16); } // 16
				else { _intpointer = std::stoll(_pointer); } // 10
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "~ERROR! INVALID POINTER"); return; }



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

			if (!ValidTypes(type)) { SendWebsocketSTR(port, "~ERROR! INVALID TYPE"); return; }

			int value = 0;
			uintptr_t _intpointer = 0;
			void* pointer = NULL;

			try // pointer
			{ // hex to int STOI
				if (IsHex(_pointer)) { _pointer = HexNormalize(_pointer); _intpointer = std::stoll(_pointer, 0, 16); } // 16
				else { _intpointer = std::stoll(_pointer); } // 10
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "~ERROR! INVALID POINTER"); return; }

			try // value
			{ // hex to int STOI
				if (IsHex(_value)) { _value = HexNormalize(_value); value = std::stoi(_value, 0, 16); } // 16
				else { value = std::stoi(_value); } // 10
			}
			catch (const std::invalid_argument& e) { SendWebsocketSTR(port, "~ERROR! INVALID VALUE"); return; }


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

		// { "mode":"hello" }
		else if ((mode == "hello")) { SendWebsocketSTR(port, "gma_hello"); }

		else { SendWebsocketSTR(port, "~ERROR! MODE NOT FOUND OR WRONG ARGS!"); }
	}
	else { SendWebsocketSTR(port, "~ERROR! INVALID JSON"); }
}

