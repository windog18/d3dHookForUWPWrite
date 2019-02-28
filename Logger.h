#pragma once
#include <Windows.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>
#include "UWP.hpp"
using namespace std;
namespace fs = std::experimental::filesystem;

enum LogType{
	LogType_Detail_0 = 1,
	LogType_Detail_1 = 2,
	LogType_Detail_2 = 4,
	LogType_All = 7,
};

enum LogFunEnumType {
	Enum_Device = 1,
	Enum_CommandList = 2,
	Enum_Resource = 4,
	Enum_other1 = 8,
	Enum_other2 = 16,
	Enum_All = 31,
};


inline std::string FunEunString(LogFunEnumType type) {
	switch (type)
	{
	case Enum_Device:
		return "Enum_Device";
		break;
	case Enum_CommandList:
		return "Enum_CommandList";
		break;
	case Enum_Resource:
		return "Enum_Resource";
		break;
	case Enum_other1:
		return "Enum_other1";
		break;
	case Enum_other2:
		return "Enum_other2";
		break;
	case Enum_All:
		return "Enum_All";
		break;
	default:
		break;
	}

	return "null";
}



inline std::string narrow(std::wstring const& text)
{
	std::locale const loc("");
	wchar_t const* from = text.c_str();
	std::size_t const len = text.size();
	std::vector<char> buffer(len + 1);
	std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
	return std::string(&buffer[0], &buffer[len]);
}
const std::wstring DumpPath = fs::path(UWP::Current::Storage::GetTemporaryPath()) / L"DUMP" / L"log_hello_2019_syl.txt";

class Logger {
public:
	static void SetLogLevel(LogType type,LogFunEnumType enumType) {
		m_sLogType = type;
		m_sEnumType = enumType;
	}


	static void Log(LogFunEnumType enumType, LogType type,const char *fmt,...) {
		if ((m_sLogType & type) && ( m_sEnumType & enumType) ){
			if (!fmt)	return;
			char		text[4096];
			va_list		ap;
			va_start(ap, fmt);
			vsprintf_s(text, fmt, ap);
			va_end(ap);

			stringstream ss;
			ss << "[Detail Level ";
			ss << log2((int)type);
			ss << "_";

			ss << FunEunString(enumType);
			ss << "]";
			string result = m_sLogPrefix + ss.str() + ": " + text;
			OutputDebugStringA(result.c_str());
		}
	}

	static void Log(LogFunEnumType enumType, LogType type, std::string output) {
		Log(enumType, type, output.c_str());
	}
private: 
	static std::string m_sLogPrefix;
	static LogType m_sLogType;
	static LogFunEnumType m_sEnumType;
};





// inline void Log(const char *fmt, ...)
// {
// 	if (!fmt)	return;
// 
// 
// 
// 	char		text[4096];
// 	va_list		ap;
// 	va_start(ap, fmt);
// 	vsprintf_s(text, fmt, ap);
// 	va_end(ap);
// 	string prefix = "[2020] ";
// 	prefix = prefix + text;
// 	OutputDebugStringA(prefix.c_str());
// 	return;
// 	//ofstream logfile(GetDirectoryFile((PCHAR)"log.txt"), ios::app);
// 	//LPCSTR str = const_cast<LPCSTR>(DumpPath.c_str());
// 	//OutputDebugStringA(narrow(DumpPath).c_str());
// 
// 	ofstream logfile(narrow(DumpPath), ios::app);
// 	if (logfile.is_open() && prefix.c_str())	logfile << prefix.c_str() << endl;
// 	logfile.close();
// }

#define LOG_DEVICE(logType, fmt, ...) \
	Logger::Log(Enum_Device, logType, fmt,__VA_ARGS__);

#define Log_COMMANDLIST(logType, fmt, ...) \
	Logger::Log(Enum_CommandList, LogType, fmt,__VA_ARGS__);

#define Log_Resource(logType, fmt,...) \
	Logger::Log(Enum_Resource, LogType, fmt, __VA_ARGS__);


#define Log_Detail_0(EnumType, fmt, ...) \
	Logger::Log(EnumType, LogType_Detail_0, fmt, __VA_ARGS__);

#define  Log_Detail_1(EnumType, fmt, ...) \
	Logger::Log(EnumType, LogType_Detail_1 , fmt, __VA_ARGS__);

#define Log_Detail_2(EnumType, fmt, ...) \
	Logger::Log(EnumType, LogType_Detail_2, fmt, __VA_ARGS__);

#define Log_Full(...) \
	Logger::Log(...)

#define Log_Simple(fmt,...) \
	Logger::Log(Enum_All, LogType_Detail_0, fmt,__VA_ARGS__);


inline void Log(std::string res) {
	Logger::Log(Enum_All, LogType_Detail_0, res.c_str());
}

#define LOG_ONCE(...)\
static bool once = false;\
if(once == false){\
		once = true; \
		Logger::Log(Enum_All, LogType_Detail_0,__VA_ARGS__); \
}

// inline void Log(std::string inputStr) {
// 	Log(inputStr.c_str());
// }

// inline void Log_WithThreadID(std::string inputStr) {
// 	stringstream ss;
// 	ss << GetCurrentThreadId();
// 	inputStr = ss.str() + " " + inputStr;
// 	Log(inputStr.c_str());
// }


