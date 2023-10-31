#pragma once
// inline ONLY IN HEADER
#include "Windows.h"
#include "XMemory.h"
#include "psapi.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <iomanip>


#define INRANGE(x,a,b)    (x >= a && x <= b)
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

//char* constchar2char(const char* constString) { return const_cast<char*>(constString); }
//char* string2char(std::string constString) { return const_cast<char*>(constString.c_str()); }
//std::string Pointer2String(void* pointer) { std::stringstream ss; ss << pointer; return "0x" + ss.str(); }
//template <typename T> T ReadPtr(void* ptr) { return *static_cast<const T*>(ptr); }
//void* IntADDR2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); }

uintptr_t PD_VoidPtrToInt(void* _addr);
void* PD_IntPtr2VoidPtr(uintptr_t _addr);


std::string ReadMemoryByStringType(void* pointer, std::string type);
void WriteMemoryByStringType(void* pointer, std::string type, int value);


bool IsHexStart0x(std::string str);
bool IsHex(std::string input);
std::string HexNormalize(std::string input);


bool ValidTypes(std::string type);

bool CheckPointer(void* pointer);
//uintptr_t Parseuintptr_t(std::string str);
std::string CalcPointerByOffsets(std::string module_name, uintptr_t  base_offset, std::vector<uintptr_t> offsets, bool check_chain);
bool CheckChainPointersByOffsets(std::string module_name, uintptr_t  base_offset, std::vector<uintptr_t> offsets);


//std::string CalcPointerByOffsetsx64(std::string module_name, long long int base_offset, std::vector<long long int> offsets, bool check_chain);
//bool CheckChainPointersByOffsetsx64(std::string module_name, long long int base_offset, std::vector<long long int> offsets);
//std::string CalcPointerByOffsetsx64(std::string module_name, int base_offset, std::vector<int> offsets, bool check_chain);
//bool CheckChainPointersByOffsetsx64(std::string module_name, int base_offset, std::vector<int> offsets);
//std::string CalcPointerByOffsetsx86(std::string module_name, int base_offset, std::vector<int> offsets, bool check_chain);
//bool CheckChainPointersByOffsetsx86(std::string module_name, int base_offset, std::vector<int> offsets);

inline void* InlineSearchPointerByPattern(void* ptrStart, int block_size, std::string pattern)
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