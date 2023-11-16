#pragma once
#include "Tools.h"


// in header
//template <typename T> T ReadPtr(void* ptr) { return *static_cast<const T*>(ptr); }
//void* IntADDR2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); } // x86
//int VoidPtrToInt(void* _addr) { return reinterpret_cast<int>(_addr); }


//void* PD_IntPtr2VoidPtr(int _addr) { return reinterpret_cast<void*>(_addr); }
//int PD_VoidPtrToInt(void* _addr) { return reinterpret_cast<int>(_addr); }

//intptr_t PD_VoidPtrToInt(void* _addr) { return reinterpret_cast<intptr_t>(_addr); }
//void* PD_IntPtr2VoidPtr(intptr_t _addr) { return reinterpret_cast<void*>(_addr); }

uintptr_t T_VoidPtr2IntPtr(void* _addr) { return reinterpret_cast<uintptr_t>(_addr); }
void* T_IntPtr2VoidPtr(uintptr_t _addr) { return reinterpret_cast<void*>(_addr); }
char* T_constchar2char(const char* constString) { return const_cast<char*>(constString); }
char* T_string2char(std::string constString) { return const_cast<char*>(constString.c_str()); }
std::string T_Pointer2String(void* pointer) { std::stringstream ss; ss << pointer; return "0x" + ss.str(); }




void savepDataToArray(int* pArr, void* pData, size_t dataSize)
{
	/*if (dataSize != sizeof(int) * dataSize) { //??
		std::cerr << "Error: Data size mismatch." << std::endl;
		return;
	}*/
	unsigned char* pDataBytes = static_cast<unsigned char*>(pData);

	for (size_t i = 0; i < dataSize; ++i) {
		int asciiValue = static_cast<int>(pDataBytes[i]);
		pArr[i] = asciiValue;
	}
}
void saveArrayToFile(int* pArr, std::fstream& WfileStream, size_t dataSize)
{
	if (!WfileStream.is_open()) {
		std::cerr << "Error: File stream is not open for writing." << std::endl;
		return;
	}

	for (size_t i = 0; i < dataSize; ++i) {
		WfileStream << pArr[i] << std::endl;
	}
}
bool parseFileToGlobalIntArray(int* pArr, std::fstream& RfileStream, size_t dataSize)
{
	if (!RfileStream.is_open()) {
		std::cerr << "Error: File stream is not open for reading." << std::endl;
		return false;
	}

	for (size_t i = 0; i < dataSize; ++i) {
		int value;
		if (!(RfileStream >> value)) {
			std::cerr << "Error: Failed to read an integer from the file." << std::endl;
			return false;
		}

		pArr[i] = value;
	}

	return true;
}
bool parseGlobalIntArrayTopData(int* pArr, void* pData, size_t dataSize)
{
	/*if (dataSize != sizeof(int) * dataSize) { //??
		std::cerr << "Error: Data size mismatch." << std::endl;
		return false;
	}*/
	char* pDataBytes = static_cast<char*>(pData);

	for (size_t i = 0; i < dataSize; ++i) {
		pDataBytes[i] = static_cast<char>(pArr[i]);
	}

	return true;
}
std::string savepDataToBinaryString(void* pData, size_t dataSize)
{
	unsigned char* pDataBytes = static_cast<unsigned char*>(pData);
	std::string binaryString;

	for (size_t i = 0; i < dataSize; ++i) {
		binaryString += pDataBytes[i];
	}

	return binaryString;
}
void parseBinaryStringToData(std::string binaryString, void* pData, size_t dataSize)
{
	if (binaryString.size() != dataSize) {
		std::cerr << "Error: Data size mismatch." << std::endl;
		return;
	}

	const char* binaryData = binaryString.c_str();
	unsigned char* pDataBytes = static_cast<unsigned char*>(pData);

	for (size_t i = 0; i < dataSize; ++i) {
		pDataBytes[i] = binaryData[i];
	}
}





//DIRECTORY
bool DirectoryExists(std::string folderPath) // 1251 or utf
{
	if (is_valid_utf8(folderPath)) { folderPath = utf8_to_cp1251(folderPath); }
	struct stat info;
	if (stat(folderPath.c_str(), &info) != 0) { return false; }
	return (info.st_mode & S_IFDIR) != 0;
}


bool DirectoryCreate(std::string folderPath) // 1251 input
{
	if (is_valid_utf8(folderPath)) { folderPath = utf8_to_cp1251(folderPath); }
	std::wstring folderPathw = cp1251_to_wstring(folderPath);
	if (CreateDirectoryW(folderPathw.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) { return true; }
	return false;
	//// Use the mkdir function to create the directory
	//if (mkdir(folderpath.c_str(), s_irwxu | s_irwxg | s_iroth | s_ixoth) != 0) {
	//	return false; // error creating directory
	//}
	//return true; // directory created successfully
}

bool DirectoryRemove(std::string folderPath) // 1251 input
{
	if (is_valid_utf8(folderPath)) { folderPath = utf8_to_cp1251(folderPath); }
	std::wstring folderPathw = cp1251_to_wstring(folderPath);

	if (RemoveDirectoryW(folderPathw.c_str())) { return true; }
	else
	{
		DWORD error = GetLastError();
		return (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND);
	}
}


std::vector<std::string> DirectoryGetAllFilesByExtention(std::string folderPath, std::string fileExtension)
{
	if (is_valid_utf8(folderPath)) { folderPath = utf8_to_cp1251(folderPath); }
	if (is_valid_utf8(fileExtension)) { folderPath = utf8_to_cp1251(fileExtension); }

	if (folderPath.back() != '\\' && folderPath.back() != '/') { folderPath += "\\"; }

	std::vector<std::string> matchingFiles;
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((folderPath + "*." + fileExtension).c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string filePath = folderPath + findFileData.cFileName;
			matchingFiles.push_back(filePath);
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	return matchingFiles;
}

std::vector<std::string> DirectoryGetAllOGGFiles(std::string folderPath)
{
	if (is_valid_utf8(folderPath)) { folderPath = utf8_to_cp1251(folderPath); }
	if (folderPath.back() != '\\' && folderPath.back() != '/') { folderPath += "\\"; }

	std::vector<std::string> oggFiles; // Создаем вектор для хранения путей к .ogg файлам
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((folderPath + "*.ogg").c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string filePath = folderPath + findFileData.cFileName;
			oggFiles.push_back(filePath);
			//std::cout << "ScanFile: " << filePath << "\n";
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	return oggFiles;
}

std::vector<std::string> DirectoryGetAllFiles(std::string folderPath)
{
	if (is_valid_utf8(folderPath)) { folderPath = utf8_to_cp1251(folderPath); }

	std::vector<std::string> files;
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((folderPath + "\\*").c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::string filePath = folderPath + "\\" + findFileData.cFileName;
				files.push_back(filePath);
			}
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}
	else {
		std::cerr << "Error accessing the directory." << std::endl;
	}

	return files;
}


//FILE
bool FileExists(std::string filePath)
{
	if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }
	std::ifstream file(filePath);
	return file.good();
}

void FileWrite(std::string filePath, std::string text)
{
	if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }

	// Open the file for writing. If it doesn't exist, create it.
	std::ofstream file(filePath, std::ios::out | std::ios::trunc);

	if (file.is_open()) {
		// Write the text to the file.
		file << text;

		// Close the file.
		file.close();
		//std::cout << "File written successfully." << std::endl;
	}
	//else {
	//	std::cerr << "Error opening or creating the file." << std::endl;
	//}
}

void FileClearMake(std::string filePath) // or make empty file
{
	if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }
	std::ofstream file(filePath, std::ios::out | std::ios::trunc);
	if (file.is_open()) { file.close(); }
}

bool FileRemove(std::string filePath)
{
	if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }

	if (std::remove(filePath.c_str()) == 0) {
		//std::cout << "File removed successfully." << std::endl;
		return true;
	}
	else {
		//std::cerr << "Error removing the file." << std::endl;
		return false;
	}
}

std::vector<std::string> FileReadAllLines(std::string filePath)
{
	if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }

	std::vector<std::string> lines;
	std::ifstream file(filePath);

	if (!file.is_open()) { return lines; }

	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}

	file.close();
	return lines;
}

std::string FileReadAllText(std::string filePath)
{
	if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }

	std::ifstream file(filePath);
	std::string content;

	if (!file.is_open()) {
		//std::cerr << "Error opening the file." << std::endl;
		return content; // Return an empty string in case of an error.
	}

	std::string line;
	while (std::getline(file, line)) {
		content += line + '\n'; // Concatenate the line and a newline character.
	}

	file.close();
	return content;
}
void FileWriteAllLines(std::string filePath, std::vector<std::string> lines)
{
	if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }
	if (!FileExists(filePath)) { FileClearMake(filePath); }
	std::ofstream file(filePath);
	if (!file.is_open()) { return; }
	for (const std::string& line : lines) { file << line << '\n'; }

	file.close();
}


/*bool MkINI(std::string pfile)
{
	if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }

	std::ofstream outfile(pfile);
	if (outfile.is_open()) {

		outfile << ";shance 6 star" << std::endl;
		outfile << "40" << std::endl;
		outfile << std::endl;

		outfile.close();
		return true;
	}
	return false;
}*/











//----STRINGS-----------------------------------------------------------------
std::vector<std::string> Split(std::string s, std::string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

std::string ToUpper(std::string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), std::toupper); //::toupper
	return strToConvert;
}
std::string ToLower(std::string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), std::tolower);
	return strToConvert;
}

std::string Trim(std::string str)
{
	// Find the first non-whitespace character from the beginning.
	size_t start = str.find_first_not_of(" \t\n\r\f\v");

	if (start == std::string::npos) {
		// If the string consists only of whitespace, return an empty string.
		return "";
	}

	// Find the last non-whitespace character from the end.
	size_t end = str.find_last_not_of(" \t\n\r\f\v");

	// Calculate the length of the trimmed substring.
	size_t length = end - start + 1;

	// Extract and return the trimmed substring.
	return str.substr(start, length);
}
std::string Replace(std::string input, std::string target, std::string replacement)
{
	std::string result = input;
	size_t startPos = 0;

	while ((startPos = result.find(target, startPos)) != std::string::npos)
	{
		result.replace(startPos, target.length(), replacement);
		startPos += replacement.length();
	}

	return result;
}
std::string Repeat(char character, int count)
{
	std::string result;
	for (int i = 0; i < count; i++) { result += character; }
	return result;
}
std::string cp1251_to_utf8(std::string str)
{
	std::string res;
	int result_u, result_c;
	result_u = MultiByteToWideChar(1251, 0, str.c_str(), -1, 0, 0);
	if (!result_u) { return 0; }
	wchar_t* ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(1251, 0, str.c_str(), -1, ures, result_u)) {
		delete[] ures;
		return 0;
	}
	result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0);
	if (!result_c) {
		delete[] ures;
		return 0;
	}
	char* cres = new char[result_c];
	if (!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0)) {
		delete[] cres;
		return 0;
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}
std::string cp1251_to_utf8(const char* str)
{
	std::string res;
	int result_u, result_c;
	result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
	if (!result_u) { return ""; }
	wchar_t* ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(1251, 0, str, -1, ures, result_u)) {
		delete[] ures;
		return 0;
	}
	result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0);
	if (!result_c) {
		delete[] ures;
		return 0;
	}
	char* cres = new char[result_c];
	if (!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0)) {
		delete[] cres;
		return 0;
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}
std::string utf8_to_cp1251(std::string str)
{
	int result_u, result_c;
	result_u = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	if (!result_u) { return ""; }
	wchar_t* ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, ures, result_u)) {
		delete[] ures;
		return "";
	}
	result_c = WideCharToMultiByte(1251, 0, ures, -1, nullptr, 0, 0, 0);
	if (!result_c) {
		delete[] ures;
		return "";
	}
	char* cres = new char[result_c];
	if (!WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0)) {
		delete[] ures;
		delete[] cres;
		return "";
	}
	delete[] ures;
	std::string res(cres);
	delete[] cres;
	return res;
}
std::string utf8_to_cp1251(const char* str)
{
	int result_u, result_c;
	result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
	if (!result_u) { return ""; }
	wchar_t* ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u)) {
		delete[] ures;
		return "";
	}
	result_c = WideCharToMultiByte(1251, 0, ures, -1, nullptr, 0, 0, 0);
	if (!result_c) {
		delete[] ures;
		return "";
	}
	char* cres = new char[result_c];
	if (!WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0)) {
		delete[] ures;
		delete[] cres;
		return "";
	}
	delete[] ures;
	std::string res(cres);
	delete[] cres;
	return res;
}
bool is_valid_utf8(const char* string)
{
	if (!string) { return true; }
	const unsigned char* bytes = (const unsigned char*)string;
	unsigned int cp;
	int num;
	while (*bytes != 0x00) {
		if ((*bytes & 0x80) == 0x00) {
			// U+0000 to U+007F 
			cp = (*bytes & 0x7F);
			num = 1;
		}
		else if ((*bytes & 0xE0) == 0xC0) {
			// U+0080 to U+07FF 
			cp = (*bytes & 0x1F);
			num = 2;
		}
		else if ((*bytes & 0xF0) == 0xE0) {
			// U+0800 to U+FFFF 
			cp = (*bytes & 0x0F);
			num = 3;
		}
		else if ((*bytes & 0xF8) == 0xF0) {
			// U+10000 to U+10FFFF 
			cp = (*bytes & 0x07);
			num = 4;
		}
		else { return false; }
		bytes += 1;
		for (int i = 1; i < num; ++i) {
			if ((*bytes & 0xC0) != 0x80) { return false; }
			cp = (cp << 6) | (*bytes & 0x3F);
			bytes += 1;
		}
		if ((cp > 0x10FFFF) ||
			((cp <= 0x007F) && (num != 1)) ||
			((cp >= 0xD800) && (cp <= 0xDFFF)) ||
			((cp >= 0x0080) && (cp <= 0x07FF) && (num != 2)) ||
			((cp >= 0x0800) && (cp <= 0xFFFF) && (num != 3)) ||
			((cp >= 0x10000) && (cp <= 0x1FFFFF) && (num != 4))) {
			return false;
		}
	}
	return true;
}
bool is_valid_utf8(std::string sstring)
{
	const char* string = sstring.c_str();
	if (!string) { return true; }
	const unsigned char* bytes = (const unsigned char*)string;
	unsigned int cp;
	int num;
	while (*bytes != 0x00) {
		if ((*bytes & 0x80) == 0x00) {
			// U+0000 to U+007F 
			cp = (*bytes & 0x7F);
			num = 1;
		}
		else if ((*bytes & 0xE0) == 0xC0) {
			// U+0080 to U+07FF 
			cp = (*bytes & 0x1F);
			num = 2;
		}
		else if ((*bytes & 0xF0) == 0xE0) {
			// U+0800 to U+FFFF 
			cp = (*bytes & 0x0F);
			num = 3;
		}
		else if ((*bytes & 0xF8) == 0xF0) {
			// U+10000 to U+10FFFF 
			cp = (*bytes & 0x07);
			num = 4;
		}
		else { return false; }
		bytes += 1;
		for (int i = 1; i < num; ++i) {
			if ((*bytes & 0xC0) != 0x80) { return false; }
			cp = (cp << 6) | (*bytes & 0x3F);
			bytes += 1;
		}
		if ((cp > 0x10FFFF) ||
			((cp <= 0x007F) && (num != 1)) ||
			((cp >= 0xD800) && (cp <= 0xDFFF)) ||
			((cp >= 0x0080) && (cp <= 0x07FF) && (num != 2)) ||
			((cp >= 0x0800) && (cp <= 0xFFFF) && (num != 3)) ||
			((cp >= 0x10000) && (cp <= 0x1FFFFF) && (num != 4))) {
			return false;
		}
	}
	return true;
}
std::wstring cp1251_to_wstring(std::string str)
{
	std::wstring convertedString;
	int requiredSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, 0, 0);
	if (requiredSize > 0) {
		std::vector<wchar_t> buffer(requiredSize);
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &buffer[0], requiredSize);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}
	return convertedString;
}
std::string wstring_to_cp1251(std::wstring wstr)
{
	int requiredSize = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (requiredSize > 0) {
		std::vector<char> buffer(requiredSize);
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &buffer[0], requiredSize, nullptr, nullptr);
		return std::string(buffer.data());
	}
	return std::string(); // Возвращаем пустую строку в случае ошибки
}
// Function to encode a string to Base64
std::string base64_encode(std::string input)
{
	std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string encoded;
	int i = 0;
	int j = 0;
	uint8_t char_array_3[3];
	uint8_t char_array_4[4];

	for (char c : input)
	{
		char_array_3[i++] = c;
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			for (i = 0; i < 4; i++) { encoded += base64_chars[char_array_4[i]]; }
			i = 0;
		}
	}

	if (i > 0)
	{
		for (int k = i; k < 3; k++) { char_array_3[k] = '\0'; }
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		for (int k = 0; k < i + 1; k++) { encoded += base64_chars[char_array_4[k]]; }
		while (i++ < 3) { encoded += '='; }
	}

	return encoded;
}

// Function to decode a Base64 encoded string
std::string base64_decode(std::string encoded)
{
	std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string decoded;
	int i = 0;
	int j = 0;
	uint8_t char_array_4[4];
	uint8_t char_array_3[3];

	for (char c : encoded)
	{
		if (c == '=') { break; }
		if (base64_chars.find(c) != std::string::npos) { char_array_4[i++] = c; }
		if (i == 4)
		{
			for (i = 0; i < 4; i++) { char_array_4[i] = base64_chars.find(char_array_4[i]); }
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

			for (i = 0; i < 3; i++) { decoded += char_array_3[i]; }
			i = 0;
		}
	}
	if (i > 0)
	{
		for (int k = i; k < 4; k++) { char_array_4[k] = 0; }
		for (i = 0; i < 4; i++) { char_array_4[i] = base64_chars.find(char_array_4[i]); }
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		for (int k = 0; k < i - 1; k++) { decoded += char_array_3[k]; }
	}
	return decoded;
}

// Function to URL encode a string
std::string url_encode(std::string input)
{
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : input)
	{
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') { escaped << c; }
		else if (c == ' ') { escaped << '+'; }
		else { escaped << '%' << std::setw(2) << int((unsigned char)c); }
	}

	return escaped.str();
}

// Function to URL decode a string
std::string url_decode(std::string input)
{
	std::ostringstream decoded;

	for (size_t i = 0; i < input.length(); ++i)
	{
		if (input[i] == '%')
		{
			if (i + 2 < input.length())
			{
				std::istringstream hex(input.substr(i + 1, 2));
				int value;
				hex >> std::hex >> value;
				decoded << static_cast<char>(value);
				i += 2;
			}
		}
		else if (input[i] == '+') { decoded << ' '; }
		else { decoded << input[i]; }
	}

	return decoded.str();
}
//--------------------------------------------------------------------------------------------







HANDLE InitConsole() // with proto
{
	AllocConsole();

	//SetConsoleOutputCP(866);
	setlocale(LC_ALL, "Russian");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);


	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

	return hConsole;
}

//void LeaveConsole(HANDLE hConsole = nullptr)
void LeaveConsole(HANDLE hConsole) // with proto
{
	if (hConsole != nullptr) { SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); } // Reset to default color
	FreeConsole();
}



float _max(float a, float b) { return (((a) > (b)) ? (a) : (b)); }
float _min(float a, float b) { return (((a) < (b)) ? (a) : (b)); }


// std::mt19937
int _RandVUKL(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(min, max - 1); // Используем max - 1, чтобы исключить верхнее значение
	return distribution(gen);
}
int _RandVKL(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(gen);
}
bool Shance(int shance, int maxshance)
{
	int randomValue = _RandVKL(0, maxshance);
	return randomValue <= shance;
}
/*int _RandVUKL(int min, int max) {
	// Инициализируем генератор случайных чисел с использованием текущего времени
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Генерируем случайное число в диапазоне [min, max - 1]
	return min + std::rand() % (max - min);
}

int _RandVKL(int min, int max) {
	// Инициализируем генератор случайных чисел с использованием текущего времени
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Генерируем случайное число в диапазоне [min, max]
	return min + std::rand() % (max - min + 1);
}*/


//std::vector<std::string> parseFile(std::string& filePath) // rows to strings
//{
//	std::vector<std::string> lines;
//	std::ifstream file(filePath);
//
//	if (!file.is_open()) {
//		std::ofstream createFile(filePath); //mkfile
//		createFile.close();
//		return lines;
//	}
//
//	std::string line;
//	while (std::getline(file, line)) {
//		if (!line.empty()) {
//			lines.push_back(line);
//		}
//	}
//
//	file.close();
//	return lines;
//}




bool DKSleep(float deltaTime, float wait_time, float& sleepBuffer)
{
	sleepBuffer += deltaTime; // Накапливаем время
	// Если накопленное время больше или равно времени ожидания
	if (sleepBuffer >= wait_time) {
		sleepBuffer = 0.0f; // Сбрасываем накопленное время
		return true; // Время ожидания истекло, возвращаем true
	}
	return false; // Время ожидания не истекло, возвращаем false
}

//std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
//float SleepBuffer = 0.0f;
//
//
//Events::drawHudEvent += []
//{
//	//--calc fps
//	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
//	std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - previousTime);
//	previousTime = currentTime;
//
//	if (DKSleep(deltaTime.count(), delayDuration, SleepBuffer)) { canSetWantedLevel = true; } // true когда таймер выйдет
//
//}




void Mbox(const char* msg, const char* title)
{
	MessageBoxA(HWND_DESKTOP, msg, title, MB_SYSTEMMODAL | MB_ICONWARNING);
}
//void MboxSTD(const std::string& msg, const std::string& title = "title")
void MboxSTD(std::string msg, std::string title)
{
	MessageBoxA(HWND_DESKTOP, msg.c_str(), title.c_str(), MB_SYSTEMMODAL | MB_ICONWARNING);
}
void EXIT_F()
{
	ExitProcess(EXIT_FAILURE);
}
void EXIT_S()
{
	ExitProcess(EXIT_SUCCESS);
}
void RaiseError(const char* fmt, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buf, fmt, args);
	va_end(args);

	MessageBoxA(HWND_DESKTOP, buf, "Fatal Error", MB_SYSTEMMODAL | MB_ICONWARNING);
	ExitProcess(EXIT_FAILURE);
}


//#include "TOOLS\Tools.h"
std::string ccfg = "VW_CFG.ini";
float ddelayDuration = 2.0f;
float hhealthdiff = 10.0f;

bool _MkCFG(std::string& config_path)
{
	std::ofstream outfile(config_path);
	if (outfile.is_open()) {

		outfile << ";delayDuration задержка после розыска (с)" << std::endl;
		outfile << "2.0f" << std::endl;

		outfile << ";healthdiff разница здоровья до и после удара" << std::endl;
		outfile << "10.0f" << std::endl;
		outfile << std::endl;

		outfile.close();
		return true;
	}
	return false;
}

void _InitCFG(std::string& config_path)
{
	//if (!std::ifstream(config_path))
	if (!FileExists(config_path))
	{ // mk ini
		//std::ofstream outfile(config_path);
		if (!_MkCFG(config_path)) { Mbox("InitCFG couldnt create ini file!", "ERROR"); return; }
	}
	//if (!FileExists(config_path)) { Mbox("InitCFG couldnt create ini file!", "ERROR"); return; }

	//std::ifstream infile(config_path);
	//std::locale loc("C");
	//infile.imbue(loc); // для парсинга 1.2f

	//if (infile.is_open())
	//{
	//	std::string firstLine = ""; // soundline

	//	std::string line;
	//	int i = 0;
	//	while (getline(infile, line)) // FUCKING getline(infile >> std::ws, tmp) coudnt parse
	//	{
	//		++i;
	//		int s5 = std::stoi(line);
	//		int SpinStartSpeed = std::stof(line);
	//	}
	//	infile.close();
	//}

	std::vector<std::string> cfg = FileReadAllLines(config_path);

	std::string _delayDuration = cfg[1];
	std::string _healthdiff = cfg[3];

	// Устанавливаем временную локаль с точкой в качестве разделителя
	std::locale prevLoc = std::locale::global(std::locale("C"));

	ddelayDuration = std::stof(_delayDuration);
	hhealthdiff = std::stof(_healthdiff);

	// Восстанавливаем предыдущую локаль
	std::locale::global(prevLoc);
}

//Events::initGameEvent += [] {
//	if (initRwEventFIX) { return; } // adapter to initRwEvent
//	else { initRwEventFIX = true; }
//	InitCFG(ccfg);
//};



std::vector<std::string> ListProcessModules()
{
	std::vector<std::string> modules;
	DWORD processId = GetCurrentProcessId();
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (hProcess == NULL) {
		std::cerr << "Failed to open process" << std::endl;
		return modules;
	}

	HMODULE hModules[1024];
	DWORD cbNeeded;

	if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
		for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
			TCHAR szModuleName[MAX_PATH];

			if (GetModuleFileNameEx(hProcess, hModules[i], szModuleName, sizeof(szModuleName) / sizeof(TCHAR))) {
				modules.push_back(szModuleName);
			}
		}
	}

	CloseHandle(hProcess);
	return modules;
}

std::string CWGetString()
{
	std::string input;
	std::getline(std::cin, input);
	return input;
}