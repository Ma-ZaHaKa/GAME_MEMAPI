#pragma once
#include "Windows.h" // всегда самым первым
#include "Internet.h"
#include "API.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <TlHelp32.h>

#include "XMemory.h"
#include "ENGINE_CMD\memory.h"
#include "Tools.h"

//#include "DIKTOR_UWS\DIKTOR_UWS.h" // ломает pipe
#include "ENGINE_CMD\memory.h"
#include "ENGINE_CMD\interfaces.h"


//FARPROC(WINAPI* pfnGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
//!!  https://developer.valvesoftware.com/wiki/SteamID "STEAM_X:Y:Z, Y is part of the ID number for the account. Y is either 0 or 1. ADD CHANGER Y

void LoadDlls();
HANDLE InitConsole();
void LeaveConsole(HANDLE hConsole = nullptr);

DWORD getProcessIdByName(const char* szProcessName);
void FreezeProcess(DWORD processId);
void ThawProcess(DWORD processId);

//EXPORT EXAMPLE
/*extern "C" __declspec(dllexport) int MyFunction(int param1, int param2) {
	return param1 + param2;
}
extern "C" int MyFunction(int param1, int param2);*/

/*const char* CreateInterfaceA1 = 0;
int* CreateInterfaceA2 = 0;
void* CreateInterfaceRet = nullptr;*/




//==========================================================
//~~~~~~~~~~~~HANDLERS DIKTOR
HANDLE ghThreadFixingA[5]; // 
HANDLE ghThreadGameInitHandler = nullptr; // monitor init
HANDLE ghThreadUWS = nullptr; // websocket
HANDLE ghThreadEntry = nullptr;
//~~~~~~~~~~~~MODULES ADDR
HMODULE ghsteamclient = nullptr;
HMODULE ghEngine = nullptr;
//HMODULE ghDiktorUWS = nullptr;
ivengineclient* engine;




//static_cast  reinterpret_cast
// copy from Tools.h linker error template
template <typename T> T ReadPtr(void* ptr) { return *static_cast<const T*>(ptr); }
void* IntPtr2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); }
int VoidPtr2IntPtr(void* _addr) { return reinterpret_cast<int>(_addr); }



void pTicketDIKTOR_RSZ(void* _IN_pbTicket, int sz_in, unsigned char* pbTicket, int sz_out) // version U(55) SZ 330
{
	unsigned char* IN_pbTicket = (unsigned char*)_IN_pbTicket;
	//pbTicket[0]-[7] (8) старшая часть
	//memcpy(&pbTicket[104], szSHAHash, 32);
	// pbTicket+16 (0x10) id+01 00 10 01 сдублировать в начало пакета

	//HIGH PART TICKET 0-8
	memcpy(pbTicket, &IN_pbTicket[16], 8);
	/*memcpy(pbTicket, &IN_pbTicket[16], 4); // destination, source, sz
	pbTicket[4] = 0x01; // 1
	pbTicket[5] = 0x00; // 0
	pbTicket[6] = 0x10; // 2
	pbTicket[7] = 0x01; // 1*/

	//8-264
	memcpy(&pbTicket[8], IN_pbTicket, sz_in); // destination, source, sz // 256 part

	/*static auto pzsSentimentMsg = "I hate birzyk and his emulators";
	memcpy((char*)&pbTicket[136], pzsSentimentMsg, strlen(pzsSentimentMsg) + 1);*/


	static auto pzsSentimentMsg = "Spoofer by Ma-ZaHaKa";
	//memcpy((char*)&pbTicket[144], pzsSentimentMsg, strlen(pzsSentimentMsg) + 1); // сразу занимаем свободное место
	memcpy((char*)&pbTicket[145], pzsSentimentMsg, strlen(pzsSentimentMsg) + 1); // кастом

	int i = 0; //  144 - 176
	//for (i = 144; i < 176; i++) { pbTicket[i] = 0; } // 2 пустые строки в блоке 8 - 264 не задействованы


	//!! 176 - 261inc https://4exchange.ru/job/index.php
	//176->261 => UNK_TICKET SHIT PHP SRV SIZE=86  ticket field 96 - 86 = 10 (zero byte)

	// блок до 264
	//for (i = 176; i < 261; i++) { pbTicket[i] = 0; } // <== KEY ENCODED FROM SRV
	//pbTicket[262] = 0;
	//pbTicket[263] = 0;
	for (i = 264; i < 272; i++) { pbTicket[i] = 0; } // добить серверный-ключ-строку нулями

	for (i = 272; i < 304; i++) { pbTicket[i] = 0; } // 2 пустые строки

	//------------- 2 строки конца билета (статика)
	pbTicket[304] = 32; //0x20
	pbTicket[305] = 0;
	pbTicket[306] = 0;
	pbTicket[307] = 0;
	pbTicket[308] = 3; // 0x03
	pbTicket[309] = 0;
	pbTicket[310] = 0;
	pbTicket[311] = 0;
	pbTicket[312] = 14; // 0x0e
	pbTicket[313] = 0;
	pbTicket[314] = 0;
	pbTicket[315] = 0;
	pbTicket[316] = 66; // 0x42
	pbTicket[317] = 90; // 0x5a
	pbTicket[318] = 104; // 0x68
	pbTicket[319] = 49; // 0x31
	pbTicket[320] = 23; // 0x17
	pbTicket[321] = 114; // 0x72
	pbTicket[322] = 69; // 0x45
	pbTicket[323] = 56; // 0x38
	pbTicket[324] = 80; // 0x50
	pbTicket[325] = 144; // 0x90
	pbTicket[326] = 0;
	pbTicket[327] = 0;
	pbTicket[328] = 0;
	pbTicket[329] = 0;

	//Req("data", "write", savepDataToBinaryString(pbTicket, sz_out), false);
	//std::cout << "DMP: " << savepDataToBinaryString(pbTicket, sz_out) << "\n";
	//Req("data", "write", savepDataToBinaryString(pbTicket, sz_out), false);
	//memcpy(IN_pbTicket, pbTicket, sz_out); // TEMP DUMP
}



void OnInitCMD() { engine->clientcmd_unrestricted("echo 2023"); }
void SendSMS(std::string _msg) { engine->clientcmd_unrestricted(("say " + _msg).c_str()); Sleep(1000); } // spam sm

void* FindTicketPtr(void* point, int maxoffset = 100000) // recode by pattern
{
	int ver_num = 85;
	int ver_word = 7500406; // ver00
	for (int i = 38000; i < maxoffset; i++)
	{
		void* pTicketNonConnect = Transpose(point, i); // offset 0 to U 55!!
		int _ver_num = ReadPtr<unsigned int>(pTicketNonConnect); // 55 00 00 00 (U)(85)
		int _ver_word = ReadPtr<unsigned int>(Transpose(pTicketNonConnect, 8)); // ver00 76 65 72 00

		if ((_ver_num == ver_num) && (_ver_word == ver_word)) { return pTicketNonConnect; }
	}
	return nullptr;
}

void GameInitHandler()
{
	bool cmdmsg = false;
	bool initclose = false;
	bool findptr = false;
	bool dumpticket = false;
	while (true)
	{
		// сообщение в консоль после инита
		if (IsInitEnd && (!cmdmsg)) { OnInitCMD(); cmdmsg = true; }
		if (checkid) { initclose = true; }
		if (IsInitEnd && (!initclose)) {
			for (int i = 0; i < ThreadsFixes; i++) { if (ghThreadFixingA[i]) { TerminateThread(ghThreadFixingA[i], 0); CloseHandle(ghThreadFixingA[i]); } }
			initclose = true;
		}
		// найти указатель на pTicket
		if (IsInitEnd && (!findptr))
		{

			pTicketNonConnectPointer = Transpose(ghsteamclient, 0x6CDB00); // base addres subprocess cs:s !!!! oldbit)
			void* pTicketNonConnectMemBlock = IntPtr2VoidPtr(ReadPtr<int>(pTicketNonConnectPointer));
			pTicketNonConnect = memory::VoidPattern(pTicketNonConnectMemBlock, 5000, "55 00 00 00 ? ? ? ? 76 65 72 00"); //  U id ver
			if (pTicketNonConnect == nullptr)
			{
				std::cout << "INFO: pTicketNonConnect pointer NULL" << "\n";
				Sleep(1000);
				continue;
			}

			//pTicketNonConnectPointer = Transpose(ghsteamclient, 0x6CDB00); // base addres subprocess cs:s !!!! oldbit)
			/*pTicketNonConnect = FindTicketPtr(pTicketNonConnectMemBlock);
			if (pTicketNonConnect == nullptr) { std::cout << "INFO: OffsetFinder NotFound" << "\n"; continue; }*/
			//pTicketNonConnectMemBlock = IntPtr2VoidPtr(VoidPtr2IntPtr(pTicketNonConnectMemBlock) & 0xFFFF0000); // отброс младших бит
			///pTicketNonConnect = Transpose(pTicketNonConnectMemBlock, 0x418); // offset 0 to U 55!!



			/*if (ProcStartCSS) { pTicketNonConnectPointer = Transpose(ghsteamclient, 0x69A2FC); } // POINTER NON CONNECT (U(55)) low part ticket
			else { pTicketNonConnectPointer = Transpose(ghsteamclient, 0x6CDB00); }*/

			/*pTicketNonConnectPointer = Transpose(ghsteamclient, 0x6CDB00); // base addres subprocess cs:s !!!! oldbit)
			void* pTicketNonConnectMemBlock = IntPtr2VoidPtr(ReadPtr<int>(pTicketNonConnectPointer));
			pTicketNonConnectMemBlock = IntPtr2VoidPtr(VoidPtr2IntPtr(pTicketNonConnectMemBlock) & 0xFFFF0000); // отброс младших бит
			pTicketNonConnect = Transpose(pTicketNonConnectMemBlock, 0xAD08); // offset 0 to U 55!!*/

			/*void* pTicketNonConnectMemBlock = IntPtr2VoidPtr(ReadPtr<int>(pTicketNonConnectPointer));
			if (ProcStartCSS) { pTicketNonConnect = Transpose(pTicketNonConnectMemBlock, 0xA900 - 0xB0); }// size 256? плавающий оффсет
			else { pTicketNonConnect = Transpose(pTicketNonConnectMemBlock, 0x520); } // size 256?*/

			//-----оффсеты по билету 256 от U55
			int version = ReadPtr<unsigned int>(pTicketNonConnect); // 55 00 00 00 (U)(85)
			outIdDIV2 = ReadPtr<unsigned int>(Transpose(pTicketNonConnect, 0x4)); // generated ID /2 middle 60.99
			outId = ReadPtr<unsigned int>(Transpose(pTicketNonConnect, 0x10)); // generated ID // start pbTicket hoG 120id

			// фикс бага дампа билета раньше того чем приходит ответ от сервера и копируеться в 168 256билета
			int pTicketFirstByte = (int)(ReadPtr<unsigned char>(Transpose(pTicketNonConnect, 168))); // result checker ОФФСЕТЫ ДЛЯ НЕДОБИЛЕТА SZ 256!!
			//int pTicketMidByte = (int)(ReadPtr<unsigned char>(Transpose(pTicketNonConnect, 200))); // result checker
			int pTicketLastByte = (int)(ReadPtr<unsigned char>(Transpose(pTicketNonConnect, 253))); // result checker

			if ((pTicketFirstByte == 0) || (pTicketLastByte == 0))
			{
				std::cout << "INFO: pTicketSrv Bytes zero!!!!!!!!!!!!!!" << "\n";
				std::cout << "ProcStartCSS: " << ProcStartCSS << "\n";
				std::cout << "pTicketNonConnect: 0x" << pTicketNonConnect << "\n";
				std::cout << "pTicketNonConnectMemBlock: 0x" << pTicketNonConnectMemBlock << "\n";
				//Sleep(1000);
				continue;
			} // билет не гото

			pTicketNonConInit = true;
			findptr = true;
			int ver = 85;
			if (version != ver) { Sleep(2 * 1000); } // чисто для читаемости в консоли
			std::cout << "BYTESARRAY: " << serialBuffer[0] << serialBuffer[1] << serialBuffer[2] << serialBuffer[3] << "\n";
			std::cout << "version: " << version << "\n";
			std::cout << "pTicketNonConnectPointer: 0x" << pTicketNonConnectPointer << "\n";
			std::cout << "pTicketNonConnect: 0x" << pTicketNonConnect << "\n";
			std::cout << "outIdDIV2PTR: 0x" << Transpose(pTicketNonConnect, 0x4) << "\n";
			std::cout << "outIdDIV2: " << outIdDIV2 << "\n";
			std::cout << "outIdPTR: 0x" << Transpose(pTicketNonConnect, 0x10) << "\n";
			std::cout << "outId: " << outId << "\n";

			if (version != ver) { Mbox("Прога запущена не в том режиме! Неправильные офсеты", "TGen by Diktor"); ExitProcess(EXIT_SUCCESS); return; } // запуск не в том режиме

		}
		// дамп билета на сервер
		if (IsInitEnd /*&& isStartUWS*/ && findptr && (!dumpticket)) { OnInitDumpTicket(); dumpticket = true; }

		if (cmdmsg && initclose && findptr && dumpticket) { break; }
		Sleep(50);
	}

	TerminateThread(ghThreadGameInitHandler, 0);
	CloseHandle(ghThreadGameInitHandler);
}






//====DLL-S
void LoadDlls()
{
	ghsteamclient = LoadLibraryA("_stmcl.dll");
	HMODULE hEngine = NULL;
	while (!hEngine)
	{
		hEngine = GetModuleHandleA("engine.dll");
		Sleep(50);
	}
	ghEngine = hEngine;
}


void Hook_CSteam3Client_InitiateConnection()
{
	HMODULE hEngine = NULL;

	while (!hEngine)
	{
		hEngine = GetModuleHandleA("engine.dll");
		Sleep(50);
	}

	auto jnz1 = Transpose(hEngine, 0x9EAD4);
	auto jnz2 = Transpose(hEngine, 0x9EB81);
	auto call = Transpose(hEngine, 0x9EB77);

	if (!Check<unsigned char>(jnz1, 0x75))
	{
		Error("Could not find first jump opcode.");
		return;
	}

	if (!Check<unsigned char>(jnz2, 0x75))
	{
		Error("Could not find second jump opcode.");
		return;
	}

	if (!Check<unsigned char>(call, 0xE8))
	{
		Error("Could not find call opcode.");
		return;
	}

	{
		//
		// Ignore SteamClient() null value, needs for emulator crash fix.
		//

		Write<unsigned char>(jnz1, 0xEB);
		Write<unsigned char>(jnz2, 0xEB);
	}

	{
		//
		// Insert SteamID changer.
		//

		InsertCall(call, hkCSteam3Client_InitiateConnection);
	}

	RegisterSpooferCVars();
}



BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL); // ускорение загрузки библиотеки

		// Получение пути к текущей DLL
		char dllPath[MAX_PATH];
		GetModuleFileName(hinstDLL, dllPath, MAX_PATH);

		// Извлечение пути до директории
		char* lastBackslash = strrchr(dllPath, '\\');
		if (lastBackslash)
			*lastBackslash = '\0'; // Убираем имя файла из пути
		std::string* dllPathStr = new std::string(dllPath);

		Init();

		//CreateThread(NULL, 0, ThreadFixing, dllPathStr, 0, NULL);

		//ghThreadUWS = CreateThread(NULL, 0, ThreadUWS, dllPathStr, 0, NULL);
		ghThreadGameInitHandler = CreateThread(NULL, 0, ThreadGameInitHandler, dllPathStr, 0, NULL);
		ghThreadEntry = CreateThread(NULL, 0, ThreadEntry, dllPathStr, 0, NULL);

		/*DisableThreadLibraryCalls(hinstDLL);

		// Получение пути к текущей DLL
		char dllPath[MAX_PATH];
		GetModuleFileName(hinstDLL, dllPath, MAX_PATH);

		// Извлечение пути до директории
		char* lastBackslash = strrchr(dllPath, '\\');
		if (lastBackslash)
			*lastBackslash = '\0'; // Убираем имя файла из пути
		std::string* dllPathStr = new std::string(dllPath);

		CreateThread(NULL, 0, ThreadEntry, dllPathStr, 0, NULL);*/

		//CreateThread(NULL, 0, ThreadEntry, NULL, 0, NULL); // ----ORIG
		///CreateThread(NULL, 0, HandlerThread, dllPathStr, 0, NULL); // указатель dllPathStr на каталог с dll
		//CreateThread(NULL, 0, ConsoleThread, dllPathStr, 0, NULL); // указатель dllPathStr на каталог с dll
	}

	return TRUE;
}