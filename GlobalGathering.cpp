#include "GlobalGathering.h"
#include "memstream.h"




std::map<std::pair<ID3D12Resource *,UINT>, void *> ResourceTempData<std::pair<ID3D12Resource *, UINT>, void *>::m_sTempMap;
std::mutex ResourceTempData<std::pair<ID3D12Resource *, UINT>, void *>::m_sMutex;

//IMPLEMENT_RESOURCE_DATA(std::pair<ID3D12Resource *, UINT>, void *,0)  :????????FIXED ME!
IMPLEMENT_RESOURCE_DATA(UINT64, ID3D12DescriptorHeap*, 0)
IMPLEMENT_RESOURCE_DATA(UINT64, ID3D12DescriptorHeap*, 1)
IMPLEMENT_RESOURCE_DATA(UINT64, ID3D12Resource*, 0)


GlobalGathering * GlobalGathering::m_sSingleton = nullptr;


GlobalGathering * GlobalGathering::GetInstance()
{
	if (m_sSingleton == nullptr) {
		m_sSingleton = new GlobalGathering();
	}
	return m_sSingleton;
}

GlobalGathering::GlobalGathering()
{
	m_sDeviceMap.clear();
	m_sCommandListMap.clear();
	m_sCommandQueueMap.clear();
	m_commandList = NULL;
	m_commandQueue = NULL;
	m_commandAllocator = NULL;

	m_isRecording = false;
	m_drawD3D11Device = NULL;
	m_drawD3D11DeviceContext = NULL;

}


GlobalGathering::~GlobalGathering()
{
	if (m_commandQueue != NULL) {
		m_commandList->Release();
		m_commandQueue->Release();
		m_commandAllocator->Release();

		m_drawD3D11Device->Release();
		m_drawD3D11DeviceContext->Release();
	}

	for (int i = 0; i < MEMMAP_COUNT; i++) {
		if (m_sRecordMemStreamMap[i].size() > 0) {
			for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap[i].begin(); it != m_sRecordMemStreamMap[i].end(); it++) {
				delete it->second;
			}
			m_sRecordMemStreamMap[i].clear();
		}
	}

}



bool GlobalGathering::GatherDevice(ID3D12Device * device)
{
	std::lock_guard<std::mutex> guard(m_sMutex);

	bool hasDevice = false;
	for (int i = 0; i < m_sDeviceMap.size(); i++) {
		if (m_sDeviceMap[i] == device) {
			hasDevice = true;
			break;
		}
	}

	if (hasDevice == false) {
		m_sDeviceMap.push_back(device);
		return true;
	}

	return false;
}

bool GlobalGathering::GatherCommandList(ID3D12CommandList * commandList)
{

	std::lock_guard<std::mutex> guard(m_sMutex);
	bool hasCommandList = false;
	for (int i = 0; i < m_sCommandListMap.size(); i++) {
		if (m_sCommandListMap[i] == commandList) {
			hasCommandList = true;
			break;
		}
	}

	if (hasCommandList == false) {
		m_sCommandListMap.push_back(commandList);
		return true;
	}

	return false;
}

bool GlobalGathering::GatherCommandQueue(ID3D12CommandQueue * commandQueue)
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	bool hasCommandQueue = false;
	for (int i = 0; i < m_sCommandListMap.size(); i++) {
		if (m_sCommandQueueMap[i] == commandQueue) {
			hasCommandQueue = true;
			break;
		}
	}

	if (hasCommandQueue == false) {
		m_sCommandQueueMap.push_back(commandQueue);
		return true;
	}

	return false;
}

MemStream * GlobalGathering::GetOrCreateMemStream(DWORD threadID)
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	if (m_sRecordMemStreamMap[m_curRecordMemMapIdx].find(threadID) == m_sRecordMemStreamMap[m_curRecordMemMapIdx].end()) {
		Log_Detail_0(Enum_other1,"alloc new memStream for newThread %d", threadID);
		MemStream *memStream = new MemStream();
		memStream->init();
		m_sRecordMemStreamMap[m_curRecordMemMapIdx].insert(std::make_pair(threadID, memStream));
		Log_Detail_0(Enum_other1, "total record memStreamSize: %d",m_sRecordMemStreamMap[m_curRecordMemMapIdx].size());

	}
	return m_sRecordMemStreamMap[m_curRecordMemMapIdx][threadID];
}

MemStream * GlobalGathering::GetOrCreateMemStreamForPtr(void * ptr)
{
	std::pair<void *, DWORD> pairVal = std::make_pair(ptr, GetCurrentThreadId());
	std::lock_guard<std::mutex> guard(m_sMutex);
	if (m_sCommandRecordMemStreamMap.find(pairVal) == m_sCommandRecordMemStreamMap.end()) {
		MemStream *memStream = new MemStream();
		memStream->init();
		m_sCommandRecordMemStreamMap.insert(std::make_pair(pairVal, memStream));
	}
	return m_sCommandRecordMemStreamMap[pairVal];
}

void GlobalGathering::SetFrameTagForAll(CommandEnum Tag)
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	if (IsRecording()) {
		for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap[m_curRecordMemMapIdx].begin(); it != m_sRecordMemStreamMap[m_curRecordMemMapIdx].end(); it++) {
			it->second->write(Tag);
		}
	}
}

void GlobalGathering::SwitchMemMapIdx(int idx)
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	m_curRecordMemMapIdx = idx;
}

void GlobalGathering::WriteAllBufferToResult()
{
	std::lock_guard<std::mutex> guard(m_sMutex);

	size_t count = m_sRecordMemStreamMap[0].size();
	Log_Detail_0(Enum_other1, "total dump files count: %d", count);
	fs::path basePath = fs::path(UWP::Current::Storage::GetTemporaryPath()) / L"DUMP";
	for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap[0].begin(); it != m_sRecordMemStreamMap[0].end(); it++)
	{
		wstringstream wss;
		wss << "RecordData_";
		wss << it->first;
		std::error_code ErrorCode;

		fs::path writePath = basePath / wss.str() / L"recordData.bin";
		fs::path writeNamePath = basePath / wss.str() / L"recordData_name.txt";

		//OutputDebugStringW(writePath.c_str());
		if (fs::create_directories(writePath.parent_path(), ErrorCode) == false && ErrorCode) {
			Log("could not create path at: " + narrow(writePath.c_str()));
		}
		else {
			std::string charStrs = narrow(writePath.c_str());
			it->second->writetoFile(charStrs.c_str());

			std::string nameCharStrs = narrow(writeNamePath.c_str());
			it->second->writeCommandNameToFile(nameCharStrs.c_str());
		}
	}


	count = m_sRecordMemStreamMap[1].size();
	Log_Detail_0(Enum_other1, "total dump last frame files count: %d", count);
	basePath = fs::path(UWP::Current::Storage::GetTemporaryPath()) / L"DUMP";
	for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap[1].begin(); it != m_sRecordMemStreamMap[1].end(); it++)
	{
		wstringstream wss;
		wss << "LastFrame_RecordData_";
		wss << it->first;
		std::error_code ErrorCode;

		fs::path writePath = basePath / wss.str() / L"recordData.bin";
		fs::path writeNamePath = basePath / wss.str() / L"recordData_name.txt";

		//OutputDebugStringW(writePath.c_str());
		if (fs::create_directories(writePath.parent_path(), ErrorCode) == false && ErrorCode) {
			Log("could not create path at: " + narrow(writePath.c_str()));
		}
		else {
			std::string charStrs = narrow(writePath.c_str());
			it->second->writetoFile(charStrs.c_str());

			std::string nameCharStrs = narrow(writeNamePath.c_str());
			it->second->writeCommandNameToFile(nameCharStrs.c_str());
		}
	}




	for (std::map<std::pair<void *, DWORD>, MemStream *>::iterator it = m_sCommandRecordMemStreamMap.begin(); it != m_sCommandRecordMemStreamMap.end(); it++)
	{
		wstringstream wss;
		wss << "RecordData_";
		wss << it->first.second;


		std::error_code ErrorCode;

		wstringstream nameStream;
		nameStream << it->first.first;
		nameStream << "_recordDataCommandOnly.bin";
		fs::path writePath = basePath / wss.str() / nameStream.str();

		nameStream.str(L"");
		nameStream.clear();
		nameStream << it->first.first;
		nameStream << "_recordDataCommandOnly_name.txt";
		fs::path writeNamePath = basePath / wss.str() / nameStream.str();
		//OutputDebugStringW(writePath.c_str());
		//OutputDebugStringW(writeNamePath.c_str());
		if (fs::create_directories(writePath.parent_path(), ErrorCode) == false && ErrorCode) {
			Log("could not create path at: " + narrow(writePath.c_str()));
		}
		else {
			std::string charStrs = narrow(writePath.c_str());
			it->second->writetoFile(charStrs.c_str());

			std::string nameCharStrs = narrow(writeNamePath.c_str());
			it->second->writeCommandNameToFile(nameCharStrs.c_str());
		}
	}



}

void GlobalGathering::ResetRecordState()
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	m_curRecordMemMapIdx = 0;
	for(int i = 0; i < MEMMAP_COUNT; i++){
		if (m_sRecordMemStreamMap[i].size() > 0)
			for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap[i].begin(); it != m_sRecordMemStreamMap[i].end(); it++) {
				delete it->second;
			}
		m_sRecordMemStreamMap[i].clear();
	}



	if (m_sCommandRecordMemStreamMap.size() > 0) {
		for (std::map<std::pair<void *, DWORD>, MemStream *>::iterator it = m_sCommandRecordMemStreamMap.begin(); it != m_sCommandRecordMemStreamMap.end(); it++) {
			delete it->second;
		}
		m_sCommandRecordMemStreamMap.clear();
	}
}


