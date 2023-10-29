#pragma once
#include "Windows.h"
#include "XMemory.h"
#include "psapi.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>


//char* constchar2char(const char* constString) { return const_cast<char*>(constString); }
//char* string2char(std::string constString) { return const_cast<char*>(constString.c_str()); }
//std::string Pointer2String(void* pointer) { std::stringstream ss; ss << pointer; return "0x" + ss.str(); }
//template <typename T> T ReadPtr(void* ptr) { return *static_cast<const T*>(ptr); }
//void* IntADDR2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); }

std::string ReadMemoryByStringType(void* pointer, std::string type);
void WriteMemoryByStringType(void* pointer, std::string type, int value);


bool IsHexStr(std::string str);
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