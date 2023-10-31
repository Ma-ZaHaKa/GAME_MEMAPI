#pragma once
#include "Pointers_Diktor.h"

struct Process
{
	void* StartPtr;
	void* EndProcAvailablePtr;
	void* EndProcPtr;
	void* EndPtr; // max addr
	std::string ProcName;
	int WorkingSetSize;
	int WorkingSetSizeMB;
};

//#define INRANGE(x,a,b)    (x >= a && x <= b)
//#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
//#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))


char* PD_constchar2char(const char* constString) { return const_cast<char*>(constString); }
char* PD_string2char(std::string constString) { return const_cast<char*>(constString.c_str()); }
std::string PD_Pointer2String(void* pointer) { std::stringstream ss; ss << pointer; return "0x" + ss.str(); }

//void* PD_IntPtr2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); }
//int PD_VoidPtrToInt(void* _addr) { return reinterpret_cast<int>(_addr); }
//void* PD_LLIntPtr2VoidPtr(long long int _addr) { return reinterpret_cast<void*>(_addr); }
//long long int PD_VoidPtrToLLInt(void* _addr) { return reinterpret_cast<long long int>(_addr); }

//intptr_t PD_VoidPtrToInt(void* _addr) { return reinterpret_cast<intptr_t>(_addr); }
//void* PD_IntPtr2VoidPtr(intptr_t _addr) { return reinterpret_cast<void*>(_addr); }

uintptr_t PD_VoidPtrToInt(void* _addr) { return reinterpret_cast<uintptr_t>(_addr); }
void* PD_IntPtr2VoidPtr(uintptr_t _addr) { return reinterpret_cast<void*>(_addr); }
std::string PD_StringToLower(std::string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), std::tolower);
	return strToConvert;
}
std::string PD_StringToUpper(std::string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), std::toupper); //::toupper
	return strToConvert;
}

template <typename T> T PD_ReadPtr(void* ptr) { try { return *static_cast<const T*>(ptr); } catch (const std::exception& e) { return T{}; } }
template <typename T> void PD_Write(void* ptr, const T& value) { try { *static_cast<T*>(ptr) = value; } catch (const std::exception& e) {} }



bool IsHexStart0x(std::string str) { return ((PD_StringToLower(str)).substr(0, 2) == "0x"); }
bool IsHex(std::string input)
{
	if (IsHexStart0x(input)) { return true; }
	//for (char c : input) { if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) { return true; } }
	for (char c : input) { if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) { return true; } }
	return false;
}


std::string HexNormalize(std::string input) // 4 stoi parse like 0007baff75
{
	bool hex0xStart = IsHexStart0x(input);
	if (hex0xStart) { input = input.substr(2); }
	return input;
	//return PD_StringToUpper(input);
	//std::regex hexPattern("[0-9a-fA-F]+");
	//return std::regex_search(input, hexPattern);
}


bool ValidTypes(std::string type) { return ((type == "char") || (type == "int") || (type == "float") || (type == "double") || (type == "dword") || (type == "llint") || (type == "uintptr_t")); }

//uintptr_t Parseuintptr_t(std::string str) // коряво работает
//{
//	uintptr_t result;
//	std::istringstream stream(str);
//	stream >> result;
//	return result;
//}



std::string ReadMemoryByStringType(void* pointer, std::string type)
{
	std::string _out = "";

	if (type == "bool") { _out = std::to_string(PD_ReadPtr<bool>(pointer)); }
	else if (type == "char") { _out = std::to_string(PD_ReadPtr<char>(pointer)); }

	else if (type == "int") { _out = std::to_string(PD_ReadPtr<int>(pointer)); }
	else if (type == "llint") { _out = std::to_string(PD_ReadPtr<long long int>(pointer)); }
	else if (type == "uintptr_t ") { _out = std::to_string(PD_ReadPtr<uintptr_t>(pointer)); }

	else if (type == "double") { _out = std::to_string(PD_ReadPtr<double>(pointer)); }
	else if (type == "long") { _out = std::to_string(PD_ReadPtr<long>(pointer)); }
	else if (type == "float") { _out = std::to_string(PD_ReadPtr<float>(pointer)); }

	else if (type == "WORD") { _out = std::to_string(PD_ReadPtr<WORD>(pointer)); }
	else if (type == "DWORD") { _out = std::to_string(PD_ReadPtr<DWORD>(pointer)); }

	return _out;
}

void WriteMemoryByStringType(void* pointer, std::string type, int value)
{
	if (type == "bool") { PD_Write<bool>(pointer, value); }
	else if (type == "char") { PD_Write<char>(pointer, value); }

	else if (type == "int") { PD_Write<int>(pointer, value); }
	else if (type == "llint") { PD_Write<long long int>(pointer, value); }
	else if (type == "uintptr_t") { PD_Write<uintptr_t>(pointer, value); }

	else if (type == "double") { PD_Write<double>(pointer, value); }
	else if (type == "long") { PD_Write<long>(pointer, value); }
	else if (type == "float") { PD_Write<float>(pointer, value); }

	else if (type == "WORD") { PD_Write<WORD>(pointer, value); }
	else if (type == "DWORD") { PD_Write<DWORD>(pointer, value); }
}




void* GetCurrProcHandle() { return GetModuleHandle(NULL); }
HANDLE _GetCurrentProcess() { return GetCurrentProcess(); }

std::string GetCurrProcName()
{
	char processName[MAX_PATH];
	GetModuleFileName(nullptr, processName, MAX_PATH);
	return std::string(processName);
}

void* GetCurrProcNamePtr()
{ //void* result = GetCurrProcName(); char* processName = static_cast<char*>(result);
	char processName[MAX_PATH];
	GetModuleFileName(nullptr, processName, MAX_PATH);
	return static_cast<void*>(processName);
}

char* GetCurrProcNameCPtr()
{
	char* processName = new char[MAX_PATH];
	if (GetModuleFileName(nullptr, processName, MAX_PATH) == 0) { delete[] processName; return nullptr; }
	return processName;
}

int GetCurrProcWorkingSetSize()
{
	HANDLE hProcess = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) { return pmc.WorkingSetSize; }
	return -1;
}


int WorkingSetSize2MB(int WorkingSetSize) { return WorkingSetSize / (1024 * 1024); } // BYTES => MEGABYTES

void* CalculateLastAddress(void* baseAddress, int WorkingSetSize) { return PD_IntPtr2VoidPtr((PD_VoidPtrToInt(baseAddress)) + WorkingSetSize); }

void* GetLastHeapAddress()
{
	HANDLE heap = GetProcessHeap();
	if (heap == NULL)
	{
		return NULL;
	}

	PROCESS_HEAP_ENTRY phe = { 0 };
	while (HeapWalk(heap, &phe)) {}

	return phe.lpData;
}

void* GetCurrProcEndPtr()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return sysInfo.lpMaximumApplicationAddress;
}


bool GetMemoryBounds(void* moduleHandle, void** startAddress, void** endAddress, char* processName)
{
	MODULEINFO moduleInfo;
	if (GetModuleInformation(GetCurrentProcess(), (HMODULE)moduleHandle, &moduleInfo, sizeof(moduleInfo)) &&
		GetModuleFileNameEx(GetCurrentProcess(), (HMODULE)moduleHandle, processName, MAX_PATH)) {
		*startAddress = moduleInfo.lpBaseOfDll;
		*endAddress = (char*)moduleInfo.lpBaseOfDll + moduleInfo.SizeOfImage;
		return true;
	}
	return false;
}

bool GetMemoryBounds(void* moduleHandle, void** startAddress, void** endAddress)
{
	MODULEINFO moduleInfo;
	if (GetModuleInformation(GetCurrentProcess(), (HMODULE)moduleHandle, &moduleInfo, sizeof(moduleInfo))) {
		*startAddress = moduleInfo.lpBaseOfDll;
		*endAddress = (char*)moduleInfo.lpBaseOfDll + moduleInfo.SizeOfImage;
		return true;
	}
	return false;
}

MODULEINFO GetModuleInfo(void* pointer)
{
	//DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	DWORD rangeStart = PD_VoidPtrToInt(pointer);
	MODULEINFO miModInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	return miModInfo;
}

Process GetProcInfo()
{
	MODULEINFO moduleInfo;
	Process prc;

	void* startAddress;
	void* endAddress;

	if (GetMemoryBounds(GetCurrProcHandle(), &startAddress, &endAddress))
	{
		prc.StartPtr = startAddress;
		prc.EndProcAvailablePtr = Transpose(endAddress, -1);
		prc.EndProcPtr = endAddress;
		prc.ProcName = GetCurrProcName();
		prc.WorkingSetSize = GetCurrProcWorkingSetSize();
		prc.WorkingSetSizeMB = WorkingSetSize2MB(prc.WorkingSetSize);
		//prc.EndPtr = CalculateLastAddress(GetCurrProcHandle(), prc.WorkingSetSize); // не правильно
		//prc.EndPtr = GetLastHeapAddress();
		prc.EndPtr = GetCurrProcEndPtr();
	}

	return prc;
}

void DumpProcessStruct(Process& process)
{
	std::cout << "\n" << "DumpProcessStruct(): ДАМП" << "\n\n";
	std::cout << "Имя (путь) процесса: " << process.ProcName << "\n";
	std::cout << "Начальный адрес процесса: 0x" << process.StartPtr << "\n";
	std::cout << "Конечный доступный адрес процесса: 0x" << process.EndProcAvailablePtr << "\n";
	std::cout << "Конечный адрес процесса: 0x" << process.EndProcPtr << "\n";
	std::cout << "Конечный адрес: 0x" << process.EndPtr << "\n";
	std::cout << "Размер в памяти процесса: " << process.WorkingSetSizeMB << " MB" << "\n\n";
}

bool CheckPointerBounds(void* pointer)
{
	if (!pointer) { return false; }

	MEMORY_BASIC_INFORMATION memInfo;
	if (VirtualQuery(pointer, &memInfo, sizeof(memInfo)) == sizeof(memInfo))
	{
		if (memInfo.State == MEM_COMMIT && (memInfo.Type == MEM_PRIVATE || memInfo.Type == MEM_MAPPED)) { return true; }
	}
	return false;
}

bool CheckPointerlpMaximumApplicationAddress(void* pointer)
{
	if (!pointer) { return false; }

	Process prc = GetProcInfo();
	//DumpProcessStruct(prc);
	void* startAddress = prc.StartPtr;
	//void* endAddress = prc.EndProcAvailablePtr;
	void* endAddress = prc.EndPtr;
	uintptr_t ptrValue = reinterpret_cast<uintptr_t>(pointer);

	//std::cout << "startAddress: " << reinterpret_cast<uintptr_t>(startAddress) << "\n";
	//std::cout << "pointer: 0x" << pointer << "\n";
	//std::cout << "ptrValue: " << ptrValue << "\n";
	//std::cout << "endAddress: " << reinterpret_cast<uintptr_t>(endAddress) << "\n";

	if (ptrValue >= reinterpret_cast<uintptr_t>(startAddress) && ptrValue <= reinterpret_cast<uintptr_t>(endAddress)) { return true; }
	return false;
}

bool CheckPointer(void* pointer) { return CheckPointerBounds(pointer); }


std::string CalcPointerByOffsets(std::string module_name, uintptr_t  base_offset, std::vector<uintptr_t> offsets, bool check_chain)
{
	void* LibPtr = GetModuleHandleA(module_name.c_str());
	//std::cout << "\nLibPtr: " << LibPtr << "\n";
	//std::cout << "LibPtr: 0x" << LibPtr << "\n";
	if (!LibPtr) { return "~ERROR! Lib not exists!"; }

	void* StartPtr = Transpose(LibPtr, base_offset);
	//std::cout << "StartPtr: " << StartPtr << "\n";


	void* TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<uintptr_t>(StartPtr)); // void*
	//std::cout << "TmpPtr[0]: " << TmpPtr << "\n";
	//std::cout << "PD_ReadPtr TmpPtr[0]: " << PD_ReadPtr<long long int>(StartPtr) << "\n";

	if (check_chain && (!CheckPointer(TmpPtr))) { return "~ERROR! Value of Start pointer chain is Zero"; }

	for (int i = 0; i < offsets.size(); i++)
	{
		TmpPtr = Transpose(TmpPtr, offsets[i]);
		if ((i + 1 >= offsets.size())) { break; } // the next reread will return the desired value (now TmpPtr indicate to value)
		TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<uintptr_t >(TmpPtr)); // void*
		if (check_chain && (!CheckPointer(TmpPtr))) { return "~ERROR! Value of TmpPtr chain is Zero"; } // оффсетов много значение 0 раньше, ошибка
		//std::cout << "TmpPtr: " << PointerToString(TmpPtr) << "\n";
		//std::cout << "offset: " << offsets[i] << "\n";
	}

	return PD_Pointer2String(TmpPtr);
}


bool CheckChainPointersByOffsets(std::string module_name, uintptr_t  base_offset, std::vector<uintptr_t> offsets)
{
	void* LibPtr = GetModuleHandleA(module_name.c_str());
	//std::cout << "\nLibPtr: " << PointerToString(LibPtr) << "\n";
	if (!LibPtr || (!CheckPointer(LibPtr))) { return false; }

	void* StartPtr = Transpose(LibPtr, base_offset);
	//std::cout << "StartPtr: " << PointerToString(StartPtr) << "\n";

	void* TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<uintptr_t>(StartPtr)); // void*
	if (!TmpPtr || (!CheckPointer(TmpPtr))) { return false; }
	//std::cout << "TmpPtr[0]: " << PointerToString(TmpPtr) << "\n";

	for (int i = 0; i < offsets.size(); i++)
	{
		TmpPtr = Transpose(TmpPtr, offsets[i]);
		if ((i + 1 >= offsets.size())) { break; } // the next reread will return the desired value (now TmpPtr indicate to value)
		TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<uintptr_t >(TmpPtr)); // void*
		if (!TmpPtr || (!CheckPointer(TmpPtr))) { return false; } // оффсетов много значение 0 раньше, ошибка
		//std::cout << "TmpPtr: " << PointerToString(TmpPtr) << "\n";
		//std::cout << "offset: " << offsets[i] << "\n";
	}

	return true;
}



//EXAMPLE 
//void* ptr = PD_IntPtr2VoidPtr(0x123456);
//unsigned int _version = PD_ReadPtr<unsigned int>(ptr); // 55 00 00 00 (U)(85)
//if (_version != 85)
//{
//	pTicketNonConnectPointer = Transpose(ghsteamclient, 0x6CDB00);
//	pTicketNonConnectMemBlock = IntPtr2VoidPtr(ReadPtr<int>(pTicketNonConnectPointer));
//	pTicketNonConnect = VoidPattern(pTicketNonConnectMemBlock, 100000, "55 00 00 00 ? ? ? ? 76 65 72 00"); //  U id ver
//	bullet_params = memory::pattern("client.dll", "55 8B EC 56 8B 75 08 68 ? ? ? ? 56 E8 ? ? ? ? 83 C4 08 84 C0");
//	if (pTicketNonConnect == nullptr) {} // не найдено
//}

inline void* VoidPattern(void* ptrStart, int block_size, std::string pattern) // release in header
{
	uintptr_t rangeStart = PD_VoidPtrToInt(ptrStart);
	const char* pattern_4_cut = pattern.c_str();
	uintptr_t firstMatch = 0;
	uintptr_t rangeEnd = rangeStart + block_size;
	uintptr_t start_cut_pattern_ptr = 0; // если сорвёться паттерн вернуться от начала поиска +1(делает for)

	for (uintptr_t MemPtr = rangeStart; MemPtr < rangeEnd; MemPtr++)
	{
		if (!*pattern_4_cut) { return PD_IntPtr2VoidPtr(firstMatch); }
		if (*(PBYTE)pattern_4_cut == '\?' || *(BYTE*)MemPtr == getByte(pattern_4_cut))
		{
			if (!firstMatch) { firstMatch = MemPtr; start_cut_pattern_ptr = MemPtr; }
			if (!pattern_4_cut[2]) { return PD_IntPtr2VoidPtr(firstMatch); } // паттерн закончился
			//PWORD первых 2 символа из паттерна, PBYTE первый символ
			if (*(PWORD)pattern_4_cut == '\?\?' || *(PBYTE)pattern_4_cut != '\?') { pattern_4_cut += 3; }
			else { pattern_4_cut += 2; } //one ?
		}
		else
		{
			pattern_4_cut = pattern.c_str();
			if (firstMatch) { MemPtr = start_cut_pattern_ptr; } // start+1 делает for в MemPtr++ для того чтобы не было бесконечного срыва
			firstMatch = 0;
		}
	}
	return nullptr;
}

inline void* VoidPattern(std::string moduleName, std::string pattern) // release in header
{
	const char* pattern_4_cut = pattern.c_str();
	uintptr_t firstMatch = 0;
	uintptr_t rangeStart = reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName.c_str()));
	MODULEINFO miModInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;
	uintptr_t start_cut_pattern_ptr = 0; // если сорвёться паттерн вернуться от начала поиска +1(делает for)

	for (uintptr_t MemPtr = rangeStart; MemPtr < rangeEnd; MemPtr++)
	{
		if (!*pattern_4_cut) { return PD_IntPtr2VoidPtr(firstMatch); }
		if (*(PBYTE)pattern_4_cut == '\?' || *(BYTE*)MemPtr == getByte(pattern_4_cut))
		{
			if (!firstMatch) { firstMatch = MemPtr; start_cut_pattern_ptr = MemPtr; }
			if (!pattern_4_cut[2]) { return PD_IntPtr2VoidPtr(firstMatch); } // паттерн закончился
			//PWORD первых 2 символа из паттерна, PBYTE первый символ
			if (*(PWORD)pattern_4_cut == '\?\?' || *(PBYTE)pattern_4_cut != '\?') { pattern_4_cut += 3; }
			else { pattern_4_cut += 2; } //one ?
		}
		else
		{
			pattern_4_cut = pattern.c_str();
			if (firstMatch) { MemPtr = start_cut_pattern_ptr; } // start+1 делает for в MemPtr++ для того чтобы не было бесконечного срыва
			firstMatch = 0;
		}
	}
	return nullptr;
}

inline void* VoidPatternDBGVER(void* ptrStart, int block_size, std::string pattern)
{
	uintptr_t rangeStart = PD_VoidPtrToInt(ptrStart);
	const char* pat = pattern.c_str();
	uintptr_t firstMatch = 0;
	//MODULEINFO miModInfo;
	//GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	uintptr_t rangeEnd = rangeStart + block_size;
	uintptr_t start_cut_pattern_ptr = 0;
	////----DEBUG
	//std::cout << "ptrStart: 0x" << ptrStart << "\n";
	//std::cout << "rangeStart: 0x" << rangeStart << "\n";
	//std::cout << "pattern: " << pattern << "\n";
	//std::cout << "block_size: " << block_size << "\n";
	////---------
	//std::cout << "\nSRCHBT: ";

	for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat) { return PD_IntPtr2VoidPtr(firstMatch); }

		//char c = (*(BYTE*)pCur);
		////int hexValue = static_cast<unsigned int>(static_cast<unsigned char>(c));
		//int hexValue = static_cast<int>((c));
		////std::cout << std::setw(2) << std::setfill('0') << std::hex << hexValue << " ";
		//std::cout << hexValue << " ";
		////return nullptr;

		auto a1 = *(PBYTE)pat;

		auto b1 = *(BYTE*)pCur;
		auto b2 = getByte(pat);

		if (a1 == '\?' || b1 == b2)
		{
			if (!firstMatch) { firstMatch = pCur; start_cut_pattern_ptr = pCur; }
			if (!pat[2]) { return PD_IntPtr2VoidPtr(firstMatch); }

			auto c1 = *(PWORD)pat;
			char lowByte = (char)(c1 & 0xFF);       // Младший байт (нижние 8 бит)
			char highByte = (char)((c1 >> 8) & 0xFF); // Старший байт (верхние 8 бит)

			auto c2 = *(PBYTE)pat;

			if (c1 == '\?\?' || c2 != '\?') { pat += 3; }
			else { pat += 2; } //one ?
		}
		else
		{
			pat = pattern.c_str();
			if (firstMatch) { pCur = start_cut_pattern_ptr; } // start+1 делает for pCur++
			firstMatch = 0;
		}
	}
	return nullptr;
}



//-----------НИЖЕ--ВСЁ---ДЛЯ---X86------------------------------
//inline void* VoidPatternx86(void* ptrStart, int block_size, std::string pattern) // x86 only
//{
//	DWORD rangeStart = PD_VoidPtrToInt(ptrStart);
//	const char* pat = pattern.c_str();
//	DWORD firstMatch = 0;
//	//MODULEINFO miModInfo;
//	//GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
//	DWORD rangeEnd = rangeStart + block_size;
//	DWORD start_cut_pattern_ptr = 0; // если сорвёться паттерн вернуться от начала поиска +1(делает for)
//
//	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
//	{
//		if (!*pat)
//			return PD_IntPtr2VoidPtr(firstMatch);
//
//		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
//		{
//			if (!firstMatch) { firstMatch = pCur; start_cut_pattern_ptr = pCur; }
//
//			if (!pat[2])
//				return PD_IntPtr2VoidPtr(firstMatch);
//
//			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
//				pat += 3;
//
//			else
//				pat += 2; //one ?
//		}
//		else
//		{
//			pat = pattern.c_str();
//			if (firstMatch) { pCur = start_cut_pattern_ptr; } // start+1 делает for в pCur++ для того чтобы не было бесконечного срыва
//			firstMatch = 0;
//		}
//	}
//	return nullptr;
//}
//inline DWORD pattern(DWORD rangeStart, DWORD SZ, std::string pattern)
//{
//	const char* pat = pattern.c_str();
//	DWORD firstMatch = 0;
//	//MODULEINFO miModInfo;
//	//GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
//	DWORD rangeEnd = rangeStart + SZ;
//	DWORD start_cut_pattern_ptr = 0; // если сорвёться паттерн вернуться от начала поиска +1(делает for)
//
//	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
//	{
//		if (!*pat)
//			return firstMatch;
//
//		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
//		{
//			if (!firstMatch) { firstMatch = pCur; start_cut_pattern_ptr = pCur; }
//
//
//			if (!pat[2])
//				return firstMatch;
//
//			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
//				pat += 3;
//
//			else
//				pat += 2; //one ?
//		}
//		else
//		{
//			pat = pattern.c_str();
//			if (firstMatch) { pCur = start_cut_pattern_ptr; } // start+1 делает for в pCur++ для того чтобы не было бесконечного срыва
//			firstMatch = 0;
//		}
//	}
//	return NULL;
//}
//
//
//inline DWORD pattern(std::string moduleName, std::string pattern)
//{
//	const char* pat = pattern.c_str();
//	DWORD firstMatch = 0;
//	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
//	MODULEINFO miModInfo;
//	GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
//	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
//	DWORD start_cut_pattern_ptr = 0; // если сорвёться паттерн вернуться от начала поиска +1(делает for)
//
//	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
//	{
//		if (!*pat)
//			return firstMatch;
//
//		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
//		{
//			if (!firstMatch) { firstMatch = pCur; start_cut_pattern_ptr = pCur; }
//
//
//			if (!pat[2])
//				return firstMatch;
//
//			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
//				pat += 3;
//
//			else
//				pat += 2; //one ?
//		}
//		else
//		{
//			pat = pattern.c_str();
//			if (firstMatch) { pCur = start_cut_pattern_ptr; } // start+1 делает for в pCur++ для того чтобы не было бесконечного срыва
//			firstMatch = 0;
//		}
//	}
//	return NULL;
//}
//----------------------------------------------------------------------------------









////std::string CalcPointerByOffsetsx64(std::string module_name, long long int base_offset, std::vector<long long int> offsets, bool check_chain)
////std::string CalcPointerByOffsetsx64(std::string module_name, int base_offset, std::vector<int> offsets, bool check_chain)
//std::string CalcPointerByOffsets(std::string module_name, int base_offset, std::vector<int> offsets, bool check_chain)
//{
//	void* LibPtr = GetModuleHandleA(module_name.c_str());
//	//std::cout << "\nLibPtr: " << LibPtr << "\n";
//	//std::cout << "LibPtr: 0x" << LibPtr << "\n";
//	if (!LibPtr) { return "~ERROR! Lib not exists!"; }
//
//	void* StartPtr = Transpose(LibPtr, base_offset);
//	//std::cout << "StartPtr: " << StartPtr << "\n";
//
//
//	void* TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<long long int>(StartPtr)); // void*
//	//std::cout << "TmpPtr[0]: " << TmpPtr << "\n";
//	//std::cout << "PD_ReadPtr TmpPtr[0]: " << PD_ReadPtr<long long int>(StartPtr) << "\n";
//
//	if (check_chain && (!CheckPointer(TmpPtr))) { return "~ERROR! Value of Start pointer chain is Zero"; }
//
//	for (int i = 0; i < offsets.size(); i++)
//	{
//		TmpPtr = Transpose(TmpPtr, offsets[i]);
//		if ((i + 1 >= offsets.size())) { break; } // the next reread will return the desired value (now TmpPtr indicate to value)
//		TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<long long int>(TmpPtr)); // void*
//		if (check_chain && (!CheckPointer(TmpPtr))) { return "~ERROR! Value of TmpPtr chain is Zero"; } // оффсетов много значение 0 раньше, ошибка
//		//std::cout << "TmpPtr: " << PointerToString(TmpPtr) << "\n";
//		//std::cout << "offset: " << offsets[i] << "\n";
//	}
//
//	return PD_Pointer2String(TmpPtr);
//}
//
////bool CheckChainPointersByOffsetsx64(std::string module_name, long long int base_offset, std::vector<long long int> offsets)
////bool CheckChainPointersByOffsetsx64(std::string module_name, int base_offset, std::vector<int> offsets)
//bool CheckChainPointersByOffsets(std::string module_name, int base_offset, std::vector<int> offsets)
//{
//	void* LibPtr = GetModuleHandleA(module_name.c_str());
//	//std::cout << "\nLibPtr: " << PointerToString(LibPtr) << "\n";
//	if (!LibPtr || (!CheckPointer(LibPtr))) { return false; }
//
//	void* StartPtr = Transpose(LibPtr, base_offset);
//	//std::cout << "StartPtr: " << PointerToString(StartPtr) << "\n";
//
//	void* TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<long long int>(StartPtr)); // void*
//	if (!TmpPtr || (!CheckPointer(TmpPtr))) { return false; }
//	//std::cout << "TmpPtr[0]: " << PointerToString(TmpPtr) << "\n";
//
//	for (int i = 0; i < offsets.size(); i++)
//	{
//		TmpPtr = Transpose(TmpPtr, offsets[i]);
//		if ((i + 1 >= offsets.size())) { break; } // the next reread will return the desired value (now TmpPtr indicate to value)
//		TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<long long int>(TmpPtr)); // void*
//		if (!TmpPtr || (!CheckPointer(TmpPtr))) { return false; } // оффсетов много значение 0 раньше, ошибка
//		//std::cout << "TmpPtr: " << PointerToString(TmpPtr) << "\n";
//		//std::cout << "offset: " << offsets[i] << "\n";
//	}
//
//	return true;
//}

//std::string CalcPointerByOffsetsx86(std::string module_name, int base_offset, std::vector<int> offsets, bool check_chain)
//{
//	void* LibPtr = GetModuleHandleA(module_name.c_str());
//	//std::cout << "\nLibPtr: " << PointerToString(LibPtr) << "\n";
//	if (!LibPtr) { return "~ERROR! Lib not exists!"; }
//
//	void* StartPtr = Transpose(LibPtr, base_offset);
//	//std::cout << "StartPtr: " << PointerToString(StartPtr) << "\n";
//
//	void* TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<unsigned int>(StartPtr)); // void*
//	//std::cout << "TmpPtr[0]: " << PD_Pointer2String(TmpPtr) << "\n";
//	//std::cout << "PD_ReadPtr TmpPtr[0]: " << PD_ReadPtr<unsigned int>(StartPtr) << "\n";
//	if (check_chain && (!CheckPointer(TmpPtr))) { return "~ERROR! Value of Start pointer chain is Zero"; }
//
//	for (int i = 0; i < offsets.size(); i++)
//	{
//		TmpPtr = Transpose(TmpPtr, offsets[i]);
//		if ((i + 1 >= offsets.size())) { break; } // the next reread will return the desired value (now TmpPtr indicate to value)
//		TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<unsigned int>(TmpPtr)); // void*
//		if (check_chain && (!CheckPointer(TmpPtr))) { return "~ERROR! Value of TmpPtr chain is Zero"; } // оффсетов много значение 0 раньше, ошибка
//		//std::cout << "TmpPtr: " << PointerToString(TmpPtr) << "\n";
//		//std::cout << "offset: " << offsets[i] << "\n";
//	}
//
//	return PD_Pointer2String(TmpPtr);
//}
//
//bool CheckChainPointersByOffsetsx86(std::string module_name, int base_offset, std::vector<int> offsets)
//{
//	void* LibPtr = GetModuleHandleA(module_name.c_str());
//	//std::cout << "\nLibPtr: " << PointerToString(LibPtr) << "\n";
//	if (!LibPtr || (!CheckPointer(LibPtr))) { return false; }
//
//	void* StartPtr = Transpose(LibPtr, base_offset);
//	//std::cout << "StartPtr: " << PointerToString(StartPtr) << "\n";
//
//	void* TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<unsigned int>(StartPtr)); // void*
//	if (!TmpPtr || (!CheckPointer(TmpPtr))) { return false; }
//	//std::cout << "TmpPtr[0]: " << PointerToString(TmpPtr) << "\n";
//
//	for (int i = 0; i < offsets.size(); i++)
//	{
//		TmpPtr = Transpose(TmpPtr, offsets[i]);
//		if ((i + 1 >= offsets.size())) { break; } // the next reread will return the desired value (now TmpPtr indicate to value)
//		TmpPtr = PD_IntPtr2VoidPtr(PD_ReadPtr<unsigned int>(TmpPtr)); // void*
//		if (!TmpPtr || (!CheckPointer(TmpPtr))) { return false; } // оффсетов много значение 0 раньше, ошибка
//		//std::cout << "TmpPtr: " << PointerToString(TmpPtr) << "\n";
//		//std::cout << "offset: " << offsets[i] << "\n";
//	}
//
//	return true;
//}











//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
//{
//	if (fdwReason == DLL_PROCESS_ATTACH)
//	{
//		DisableThreadLibraryCalls(hinstDLL);
//
//		//InitConsole();
//
//		const char* wslib = "WebSocketDiktor.dll";
//
//		HMODULE ghDiktorUWS = GetModuleHandleA(wslib);
//		if (!ghDiktorUWS) { ghDiktorUWS = LoadLibraryA(wslib); }
//		while (true) { if (!GetProcAddress(ghDiktorUWS, "Setup")) { Sleep(50); } else { break; } } // load lib dynamic delay
//
//		//std::cout << wslib << ": 0x" << ghDiktorUWS << "\n";
//
//		Setup = reinterpret_cast<SetupFunction>(GetProcAddress(ghDiktorUWS, "Setup"));
//		SetupHandle = reinterpret_cast<SetupHandleFunction>(GetProcAddress(ghDiktorUWS, "SetupHandle"));
//		Stop = reinterpret_cast<StopFunction>(GetProcAddress(ghDiktorUWS, "Stop"));
//		GetInitStatus = reinterpret_cast<GetInitStatusFunction>(GetProcAddress(ghDiktorUWS, "GetInitStatus"));
//		GetMessages = reinterpret_cast<GetMessagesFunction>(GetProcAddress(ghDiktorUWS, "GetMessages"));
//		GetMessagesCLS = reinterpret_cast<GetMessagesCLSFunction>(GetProcAddress(ghDiktorUWS, "GetMessagesCLS"));
//		SendWebsocketSTR = reinterpret_cast<SendWebsocketSTRFunction>(GetProcAddress(ghDiktorUWS, "SendWebsocketSTR"));
//		SendWebsocketJSON = reinterpret_cast<SendWebsocketJSONFunction>(GetProcAddress(ghDiktorUWS, "SendWebsocketJSON"));
//		SendWebsocketSTRAsJSON = reinterpret_cast<SendWebsocketSTRAsJSONFunction>(GetProcAddress(ghDiktorUWS, "SendWebsocketSTRAsJSON"));
//		Sleep(50);
//
//		CreateThread(NULL, 0, MessageEntry, NULL, 0, NULL);
//		//FreeLibrary(ghDiktorUWS);
//	}
//	return TRUE;
//}









//void* _GetCurrProcStartPtr() { return GetModuleHandle(NULL); }
//void* GetCurrProcEndPtr()
//{
//	MODULEINFO moduleInfo;
//	if (GetModuleInformation(GetCurrentProcess(), (HMODULE)GetModuleHandle(NULL), &moduleInfo, sizeof(moduleInfo))) {
//		*startAddress = moduleInfo.lpBaseOfDll;
//		*endAddress = (char*)moduleInfo.lpBaseOfDll + moduleInfo.SizeOfImage;
//		return true;
//	}
//}
//
//bool CheckPointer(void* pointer)
//{
//	if (!pointer) { return false; }
//
//	MEMORY_BASIC_INFORMATION memInfo;
//	if (VirtualQuery(pointer, &memInfo, sizeof(memInfo)) == sizeof(memInfo))
//	{
//		if (memInfo.State == MEM_COMMIT && (memInfo.Type == MEM_PRIVATE || memInfo.Type == MEM_MAPPED)) { return true; }
//	}
//	return false;
//}




//----WORK
//bool GetMemoryBounds(void* moduleHandle, void** startAddress, void** endAddress) {
//	MODULEINFO moduleInfo;
//	if (GetModuleInformation(GetCurrentProcess(), (HMODULE)moduleHandle, &moduleInfo, sizeof(moduleInfo))) {
//		*startAddress = moduleInfo.lpBaseOfDll;
//		*endAddress = (char*)moduleInfo.lpBaseOfDll + moduleInfo.SizeOfImage;
//		return true;
//	}
//	return false;
//}

//---EXAMPLE
//void* startAddress;
//void* endAddress;
//
//void* moduleHandle = GetModuleHandle(NULL);
//if (GetMemoryBounds(moduleHandle, &startAddress, &endAddress)) {
//	std::cout << "Начальный адрес: " << startAddress << std::endl;
//	std::cout << "Последний доступный адрес: " << endAddress << std::endl;
//}
//else {
//	std::cerr << "Не удалось получить информацию о модуле." << std::endl;
//}









//bool CheckPointer(void* pointer)
//{
//	if (!pointer) { return false; }
//
//	// Получаем имя текущего процесса
//	//char processName[MAX_PATH];
//	//GetModuleFileName(nullptr, processName, MAX_PATH);
//	//std::cout << "Имя процесса: " << processName << std::endl;
//
//	// Получаем дескриптор текущего процесса
//	HANDLE hProcess = GetCurrentProcess();
//
//	// Получаем информацию о процессе, включая адрес начала
//	PROCESS_MEMORY_COUNTERS pmc;
//	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
//	{
//		// Получаем адрес начала процесса
//		void* startAddress = GetModuleHandle(NULL);
//		void* endAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(startAddress) + pmc.QuotaPeakPagedPoolUsage);
//
//		// Получаем адрес конца процесса
//		SYSTEM_INFO sysInfo;
//		GetSystemInfo(&sysInfo);
//		void* endAddressTotal = sysInfo.lpMaximumApplicationAddress;
//
//		std::cout << "Размер в памяти процесса: " << pmc.WorkingSetSize / (1024 * 1024) << " MB" << "\n";
//		std::cout << "Начальный адрес процесса: 0x" << startAddress << "\n";
//		std::cout << "Конечный адрес процесса: 0x" << endAddress << "\n";
//		std::cout << "Конечный возможный адрес процесса: 0x" << endAddressTotal << "\n";
//
//		uintptr_t ptrValue = reinterpret_cast<uintptr_t>(pointer);
//
//		if (ptrValue >= reinterpret_cast<uintptr_t>(startAddress) && ptrValue <= reinterpret_cast<uintptr_t>(endAddress)) {
//			return true;
//		}
//	}
//
//	return false;
//}

//bool CheckPointer(void* pointer)
//{
//	if (!pointer) { return false; }
//
//	// Получаем дескриптор текущего процесса
//	HANDLE hProcess = GetCurrentProcess();
//
//	// Получаем информацию о процессе, включая адреса начала и конца
//	PROCESS_MEMORY_COUNTERS_EX pmc;
//	if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
//	{
//		// Получаем адрес начала и конца процесса
//		void* startAddress = pmc.WorkingSetSize;
//		void* endAddress = (BYTE*)startAddress + pmc.WorkingSetSize;
//
//		std::cout << "Размер в памяти процесса: " << pmc.WorkingSetSize / (1024 * 1024) << " MB" << "\n";
//		std::cout << "PROCESS_MEMORY_COUNTERS_EX startAddress: 0x" << startAddress << "\n";
//		std::cout << "PROCESS_MEMORY_COUNTERS_EX endAddress: 0x" << endAddress << "\n";
//
//		uintptr_t ptrValue = reinterpret_cast<uintptr_t>(pointer);
//
//		if (ptrValue >= reinterpret_cast<uintptr_t>(startAddress) && ptrValue <= reinterpret_cast<uintptr_t>(endAddress)) {
//			return true;
//		}
//		return false;
//	}
//
//	return false;
//}