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
#include "Logger.h"
#include "GlobalGathering.h"


//=========================================================================================================================//

#include <fstream>
using namespace std;




#define DECLARE_FUNCTIONPTR(DReturnType,DFunctionName,...) \
typedef DReturnType(__stdcall* DFunctionName)(__VA_ARGS__);\
DFunctionName o##DFunctionName = NULL; \
DReturnType __stdcall hk##DFunctionName(__VA_ARGS__) \



#define CREATE_HOOKPAIR(methodVirtualPtr,DFunctionName) \
MH_CreateHook(methodVirtualPtr, hk##DFunctionName, (LPVOID*)&o##DFunctionName)\


extern bool g_beginRecord;
extern std::mutex g_mutex;

#define RecordStart \
	if( GlobalGathering::GetInstance()->IsRecording() ) \
	 {\

#define RecordEnd \
	}\


#define GetStreamFromThreadID() \
GlobalGathering::GetInstance()->GetOrCreateMemStream(GetCurrentThreadId()); \

#define GetStreamFromPtr(ptr) \
GlobalGathering::GetInstance()->GetOrCreateMemStreamForPtr(ptr); \

#if 1
#define FUNC_DEFINE
#else
#define FUNC_DEFINE \
	if(g_beginRecord){ \
	char buf[1024] = {}; \
	sprintf_s(buf,"call the %s \n",__FUNCTION__ ); \
	static bool once = false; \
	if(once == false) { \
			OutputDebugStringA("call func"); \
			once = true; \
		}\
	} 
#endif


inline void ResetRecordState()
{
	ResourceTempData::Reset();
	GlobalGathering::GetInstance()->ResetRecordState();
}

inline void BeginRecord() {
	Log_Detail_0(Enum_other1, "Begin to RecordData");
	ResetRecordState();
	g_beginRecord = true;
}


inline void EndRecord() {
	Log_Detail_0(Enum_other1, "Stop RecordData");
	//ResetRecordState();
	g_beginRecord = false;
}

inline void ToggleBeginRecordState() {
	std::lock_guard<std::mutex> tLock(g_mutex);
	if (!g_beginRecord) {
		BeginRecord();
	}
	else {
		EndRecord();
	}
}

inline bool GetBeginRecordState() {
	bool state = false;
	{
		std::lock_guard<std::mutex> tLock(g_mutex);
		state = g_beginRecord;
	}
	return state;
}


inline void ToggleRecordState() {
	if (g_beginRecord) {
		GlobalGathering::GetInstance()->SetRecording(true);
	}
	else {
		GlobalGathering::GetInstance()->SetRecording(false);
	}
	g_beginRecord = false;
	
	if (GlobalGathering::GetInstance()->IsRecording())
		Log_Detail_0(Enum_other1, "Begin RecordData");
	else {
		Log_Detail_0(Enum_other1,"Stop to RecordData");
	}
	ResetRecordState();
}

