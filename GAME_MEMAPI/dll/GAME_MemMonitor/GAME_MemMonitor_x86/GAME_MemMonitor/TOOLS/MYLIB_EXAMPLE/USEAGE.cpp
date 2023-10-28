//#pragma once
#include "Tools\Tools.h"
#include "Windows.h"
#include "plugin_III.h"
#include "CCheat.h"

using namespace std;


typedef std::vector<std::string>(*WGetMessagesFunction)(bool);
WGetMessagesFunction WGetMessages;

typedef bool (*WSendMessageFunction)(std::string);
WSendMessageFunction WSendMessage;

typedef bool (*WSendMessageAsJsonFunction)(std::vector<std::string>, std::vector<std::string>);
WSendMessageAsJsonFunction WSendMessageAsJson;

typedef bool (*WSendMessageAsDataJsonFunction)(std::string);
WSendMessageAsDataJsonFunction WSendMessageAsDataJson;

typedef bool (*WGetInitStatusFunction)();
WGetInitStatusFunction WGetInitStatus;

typedef bool (*WStartuWSFunction)(int);
WStartuWSFunction WStartuWS;

DWORD CALLBACK MessageEntry(LPVOID);
static void Cheat(std::string _msg);



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		HMODULE ghDiktorUWS = LoadLibraryA("WebSocketDiktor.dll"); // default init port 9002
		while (true) { if (!GetProcAddress(ghDiktorUWS, "WGetMessages")) { Sleep(50); } else { break; } } // load lib dynamic delay

		WGetMessages = reinterpret_cast<WGetMessagesFunction>(GetProcAddress(ghDiktorUWS, "WGetMessages"));
		WSendMessage = reinterpret_cast<WSendMessageFunction>(GetProcAddress(ghDiktorUWS, "WSendMessage"));
		WSendMessageAsJson = reinterpret_cast<WSendMessageAsJsonFunction>(GetProcAddress(ghDiktorUWS, "WSendMessageAsJson"));
		WSendMessageAsDataJson = reinterpret_cast<WSendMessageAsDataJsonFunction>(GetProcAddress(ghDiktorUWS, "WSendMessageAsDataJson"));
		WGetInitStatus = reinterpret_cast<WGetInitStatusFunction>(GetProcAddress(ghDiktorUWS, "WGetInitStatus"));
		WStartuWS = reinterpret_cast<WStartuWSFunction>(GetProcAddress(ghDiktorUWS, "WStartuWS"));
		Sleep(50);

		CreateThread(NULL, 0, MessageEntry, NULL, 0, NULL);

		//int port = 9002;
		//Websocket::Setup(port, OnMessage);
	}
	return TRUE;
}

DWORD CALLBACK MessageEntry(LPVOID)
{
	int port = 9002;
	WStartuWS(port); // return true if init flag = false
	while (true) { if (!WGetInitStatus()) { Sleep(50); } else { break; } } // wait init srv

	while (true)
	{
		std::vector<std::string> messages = WGetMessages(true); // true = clear stack messages
		if (!messages.empty()) {
			for (const std::string& message : messages) { Cheat(message); }
		}
		Sleep(500);
	}
	return TRUE;
}



static void Cheat(std::string _msg)
{
	try {

		if (_msg.length() != 0)
		{
			std::string text = StringToUpper(_msg);
			if (text == "GUNSGUNSGUNS") { WeaponCheat(); }                  // Все оружие
			else if (text == "IFIWEREARICHMAN") { MoneyCheat(); }                   // Получить деньги
			else if (text == "TORTOISE") { ArmourCheat(); }                  // Полная броня
			else if (text == "GESUNDHEIT") { HealthCheat(); }                  // Полное здоровье
			else if (text == "MOREPOLICEPLEASE") { WantedLevelUpCheat(); }           // Повысить уровень розыска
			else if (text == "NOPOLICEPLEASE") { WantedLevelDownCheat(); }         // Убрать уровень розыска
			else if (text == "GIVEUSATANK") { TankCheat(); }                    // Получить танк
			else if (text == "BANGBANGBANG") { BlowUpCarsCheat(); }              // Взорвать все машины
			else if (text == "ITSALLGOINGMAAAD") { MayhemCheat(); }                  // Безумные пешеходы
			else if (text == "NOBODYLIKESME") { EverybodyAttacksPlayerCheat(); }  // Пешеходы атакуют игрока
			else if (text == "WEAPONSFORALL") { WeaponsForAllCheat(); }           // Пешеходы дерутся + оружие
			else if (text == "TIMEFLIESWHENYOU") { FastWeatherCheat(); }             // Ускорить время погоды
			else if (text == "SKINCANCERFORME") { SunnyWeatherCheat(); }            // Ясная погода
			else if (text == "ILIKESSCOTLAND") { CloudyWeatherCheat(); }           // Пасмурная погода
			else if (text == "ILOVESCOTLAND") { RainyWeatherCheat(); }            // Дождливая погода
			else if (text == "PEASOUP") { FoggyWeatherCheat(); }            // Туманная погода
			else if (text == "MADWEATHER") { FastTimeCheat(); }                // Ускорить игровые часы
			else if (text == "BOOOOORING") { SlowTimeCheat(); }                // Замедление
			else if (text == "ILIKEDRESSINGUP") { ChangePlayerCheat(); }            // Сменить модель игрока на случайную
			else if (text == "ANICESETOFWHEELS") { OnlyRenderWheelsCheat(); }        // Невидимые машины, видны только колёса
			else if (text == "CORNERSLIKEMAD") { StrongGripCheat(); }              // Улучшенное управление машинами
			else if (text == "CHITTYCHITTYBB") { ChittyChittyBangBangCheat(); }    // Машины летают
			else if (text == "NASTYLIMBSCHEAT") { NastyLimbsCheat(); }              // Кровавый режим
			//else if (text == "TEST") { }
		}
		else // wrong length
		{
			WSendMessage("INVALID COMMAND!");
		}
	}
	catch (exception e) {}
}


































/*void test()
{
	MessageBox(NULL, "123", "", MB_ICONINFORMATION);
}*/

//void OnTimer(HWND hwnd, UINT msg, UINT idTimer, DWORD dwTime)
//{
//	if (GetAsyncKeyState(0x47) & 0x8000) //G
//	{
//		//CCheat::WantedCheat();
//		char test[] = "LXGIWYL";
//		CMessages::AddMessageJumpQ(test, 500, NULL, false);
//		CMessages::AddBigMessageQ(test, 500, false);
//
//		/*CPlayerPed *player = FindPlayerPed();
//		CPlayerInfo *playerInfo = player->GetPlayerInfoForThisPlayerPed();
//
//		if (player)
//		{
//		CPlayerInfo *playerInfo = player->GetPlayerInfoForThisPlayerPed();
//
//		if (player->m_fHealth <= 25.0f)
//		player->m_fHealth = 0.0f;
//		else
//		player->m_fHealth = 25.0f;
//
//		player->m_fArmour = 0.0f;
//		playerInfo->m_nMoney = max(-100, playerInfo->m_nMoney - 250000);
//
//		CVehicle *vehicle = FindPlayerVehicle(-1, false);
//		if (vehicle) vehicle->m_fHealth -= 250.0f;
//		CCheat::WantedLevelUpCheat();
//		}*/
//
//
//
//		/*DWORD Many = *(DWORD*)0xB7CE50;
//		if (Many<99999999)
//		{
//		*(DWORD*)0xB7CE50 = 99999999;
//		}*/
//		/*DWORD Many = *(DWORD*)0xBAA420;
//		if (count == 0) {
//		*(DWORD*)0xBAA420 = 51;
//		count++;
//		}
//		else if (count == 1) {
//		*(DWORD*)0xBAA420 = 181;
//		count++;
//		}
//		else if (count == 2) {
//		*(DWORD*)0xBAA420 = 551;
//		count++;
//		}
//		else if (count == 3) {
//		*(DWORD*)0xBAA420 = 1201;
//		count++;
//		}
//		else if (count == 4) {
//		*(DWORD*)0xBAA420 = 2401;
//		count++;
//		}
//		else if (count == 5) {
//		*(DWORD*)0xBAA420 = 4601;
//		count++;
//		}
//		else if (count == 6) {
//		*(DWORD*)0xBAA420 = 0;
//		count = 0;
//		}
//		*/
//		/*char szText[] = "Hello world.",
//		szTitle[] = "Information";
//		MessageBox(NULL, szText, szTitle, MB_ICONINFORMATION);*/ //50 180 550 1200 2400 4600
//
//
//	}
//}


//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
//{
//
//	switch (fdwReason)
//	{
//	case DLL_PROCESS_ATTACH:
//	{
//		//_beginthreadex(NULL, 0, &driver, NULL, 0, NULL);
//		Websocket::Setup();
//		break;
//	}
//	case DLL_PROCESS_DETACH:
//	{
//		break;
//	}
//	case DLL_THREAD_ATTACH:
//	{
//		break;
//	}
//	case DLL_THREAD_DETACH:
//	{
//		break;
//	}
//	}
//
//	return TRUE;
//}
