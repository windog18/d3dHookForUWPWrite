#include "GlobalGathering.h"
#include "memstream.h"

std::map<DWORD, void *> ResourceTempData::m_sTempMap;
std::mutex ResourceTempData::m_sMutex;




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

	if (m_sRecordMemStreamMap.size() > 0) {
		for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap.begin(); it != m_sRecordMemStreamMap.end(); it++) {
			delete it->second;
		}
		m_sRecordMemStreamMap.clear();
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
	if (m_sRecordMemStreamMap.find(threadID) == m_sRecordMemStreamMap.end()) {
		MemStream *memStream = new MemStream();
		memStream->init();
		m_sRecordMemStreamMap.insert(std::make_pair(threadID, memStream));
	}
	return m_sRecordMemStreamMap[threadID];
}

MemStream * GlobalGathering::GetOrCreateMemStreamForPtr(void * ptr)
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	if (m_sCommandRecordMemStreamMap.find(ptr) == m_sCommandRecordMemStreamMap.end()) {
		MemStream *memStream = new MemStream();
		memStream->init();
		m_sCommandRecordMemStreamMap.insert(std::make_pair(ptr, memStream));
	}
	return m_sCommandRecordMemStreamMap[ptr];
}

void GlobalGathering::WriteAllBufferToResult()
{
	fs::path basePath = fs::path(UWP::Current::Storage::GetTemporaryPath()) / L"DUMP";
	for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap.begin(); it != m_sRecordMemStreamMap.end(); it++)
	{
		wstringstream wss;
		wss << "RecordData_";
		wss << it->first;
		std::error_code ErrorCode;
		fs::path writePath = basePath / wss.str() / L"recordData.bin";
		fs::path writeNamePath = basePath / wss.str() / L"recordData_name.txt";
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


	for (std::map<void *, MemStream *>::iterator it = m_sCommandRecordMemStreamMap.begin(); it != m_sCommandRecordMemStreamMap.end(); it++)
	{
		wstringstream wss;
		wss << "RecordData_";
		wss << it->first;
		std::error_code ErrorCode;
		fs::path writePath = basePath / wss.str() / L"recordDataCommandOnly.bin";
		fs::path writeNamePath = basePath / wss.str() / L"recordDataCommandOnly_name.txt";
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
	if (m_sRecordMemStreamMap.size() > 0) {
		for (std::map<DWORD, MemStream *>::iterator it = m_sRecordMemStreamMap.begin(); it != m_sRecordMemStreamMap.end(); it++) {
			delete it->second;
		}
		m_sRecordMemStreamMap.clear();
	}


	if (m_sCommandRecordMemStreamMap.size() > 0) {
		for (std::map<void *, MemStream *>::iterator it = m_sCommandRecordMemStreamMap.begin(); it != m_sCommandRecordMemStreamMap.end(); it++) {
			delete it->second;
		}
		m_sCommandRecordMemStreamMap.clear();
	}
}



ID3D12Device * GlobalGathering::GetDevice(int idx)
{
	if(idx >= m_sDeviceMap.size())
		return nullptr;
	return m_sDeviceMap[idx];
}


