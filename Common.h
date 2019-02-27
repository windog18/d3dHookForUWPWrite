#pragma once
#include <Windows.h>
#include <stdint.h>
#include <dxgi.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <d3d11on12.h>
#include "MinHook/include/MinHook.h"


#include "UWP.hpp"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include <clocale>
#include <locale>
#include <sstream> 
#include <string>
#include <vector>
#include "GlobalGathering.h"

inline std::string narrow(std::wstring const& text)
{
	std::locale const loc("");
	wchar_t const* from = text.c_str();
	std::size_t const len = text.size();
	std::vector<char> buffer(len + 1);
	std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
	return std::string(&buffer[0], &buffer[len]);
}
//=========================================================================================================================//

#include <fstream>
using namespace std;

const std::wstring DumpPath = fs::path(UWP::Current::Storage::GetTemporaryPath()) / L"DUMP" / L"log_hello_2019_syl.txt";
inline void Log(const char *fmt, ...)
{
	if (!fmt)	return;



	char		text[4096];
	va_list		ap;
	va_start(ap, fmt);
	vsprintf_s(text, fmt, ap);
	va_end(ap);
	OutputDebugStringA(text);
	return;
	//ofstream logfile(GetDirectoryFile((PCHAR)"log.txt"), ios::app);
	//LPCSTR str = const_cast<LPCSTR>(DumpPath.c_str());
	//OutputDebugStringA(narrow(DumpPath).c_str());

	ofstream logfile(narrow(DumpPath), ios::app);
	if (logfile.is_open() && text)	logfile << text << endl;
	logfile.close();
}

inline void Log(std::string inputStr) {
	Log(inputStr.c_str());
}

inline void Log_WithThreadID(std::string inputStr) {
	stringstream ss;
	ss << GetCurrentThreadId();
	inputStr = ss.str() + " " + inputStr;
	Log(inputStr.c_str());
}

#define  LOG_ONCE(...)\
static bool once = false;\
if(once == false){\
		once = true; \
		Log(__VA_ARGS__); \
}\

#define DECLARE_FUNCTIONPTR(DReturnType,DFunctionName,...) \
typedef DReturnType(__stdcall* DFunctionName)(__VA_ARGS__);\
DFunctionName o##DFunctionName = NULL; \
DReturnType __stdcall hk##DFunctionName(__VA_ARGS__) \



#define CREATE_HOOKPAIR(methodVirtualPtr,DFunctionName) \
MH_CreateHook(methodVirtualPtr, hk##DFunctionName, (LPVOID*)&o##DFunctionName)\


extern bool g_beginRecord;
extern bool g_record;

#define RecordStart \
	if( g_record ) \
	 {\

#define RecordEnd \
	}\


#define GetStreamFromThreadID() \
GlobalGathering::GetInstance()->GetOrCreateMemStream(GetCurrentThreadId()); \

#if 1
#define FUNC_DEFINE
#else
#define FUNC_DEFINE \
	char buf[256]; \
	sprintf_s(buf,"2020, call the %s \n",__FUNCTION__ ); \
	OutputDebugStringA(buf);
#endif


inline void ResetRecordState()
{
	ResourceTempData::Reset();
	GlobalGathering::GetInstance()->ResetRecordState();
}

inline void BeginRecord() {
	//OutputDebugStringA("Begin to RecordData");
	ResetRecordState();
	g_beginRecord = true;
}


inline void ToggleRecordState() {
	if (g_beginRecord) {
		g_record = true;
	}
	else {
		g_record = false;
	}
	g_beginRecord = false;
	
	if (g_record)
		OutputDebugStringA("Stop RecordData");
	else {
		OutputDebugStringA("Begin to RecordData");
	}
	ResetRecordState();
}

