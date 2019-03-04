#pragma once
#include <dxgi.h>
#include <d3d12.h>
#include <d3d11.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <mutex>
#include <sstream>
#include "Logger.h"
#include "xdefine.h"

class MemStream;
#define MEMMAP_COUNT 2
template<typename Key, typename Value ,int N=0>
class ResourceTempData {
public:
	
 	static void SetTempMapData(Key threadID,Value ptr) {
		std::lock_guard<std::mutex> lock(m_sMutex);
		m_sTempMap[threadID] = ptr;
	}


	static Value GetTempMapData(Key threadID) {
		Value ptr = NULL;
		{
			std::lock_guard<std::mutex> lock(m_sMutex);
			if (m_sTempMap.find(threadID) != m_sTempMap.end())
				ptr = m_sTempMap[threadID];
		}
		return ptr;
	}

	static void Reset() {
		std::lock_guard<std::mutex> lock(m_sMutex);
		m_sTempMap.clear();
	}
private:
	static std::map<Key, Value>  m_sTempMap;

	static std::mutex m_sMutex;
};


#define IMPLEMENT_RESOURCE_DATA(Key, Value, N0) \
std::map<Key, Value> ResourceTempData<Key, Value,N0>::m_sTempMap; \
std::mutex ResourceTempData<Key, Value,N0>::m_sMutex;



void ResetRecordState();


class GlobalGathering
{
public:
	static GlobalGathering * GetInstance();
	GlobalGathering();
	~GlobalGathering();


	bool GatherDevice(ID3D12Device *device);
	bool GatherCommandList(ID3D12CommandList *commandList);
	bool GatherCommandQueue(ID3D12CommandQueue *commandQueue);
	
	
	MemStream *GetOrCreateMemStream(DWORD threadID);
	MemStream *GetOrCreateMemStreamForPtr(void *ptr);

	void SetFrameTagForAll(CommandEnum Tag);

	void SetupForLastFrameRecord();
	void SwitchMemMapIdx(int idx);

	inline bool IsRecording() {
		bool recordState = false;
		{
			std::lock_guard<std::mutex> lock(m_recordMutex);
			recordState = m_isRecording;
		}
		return recordState;
	}

	inline void SetRecording(bool recordState) {
		{
			std::lock_guard<std::mutex> lock(m_recordMutex);
			if (m_isRecording != recordState) {
				std::stringstream ss;
				ss << "recording state changed: ";
				ss << recordState;
				Log_Detail_0(Enum_other1, ss.str().c_str());
				//OutputDebugStringA("start recording");
			}
			m_isRecording = recordState;

		}
	}


	void WriteAllBufferToResult();
	void ResetRecordState();


	ID3D12CommandList *m_commandList;
	ID3D12CommandAllocator *m_commandAllocator;
	ID3D12CommandQueue *m_commandQueue;
	ID3D11Device *m_drawD3D11Device;
	ID3D11DeviceContext *m_drawD3D11DeviceContext;
private:
	static GlobalGathering *m_sSingleton;
	std::vector<ID3D12Device *> m_sDeviceMap;
	std::vector<ID3D12CommandList *> m_sCommandListMap;
	std::vector<ID3D12CommandQueue *> m_sCommandQueueMap;


	std::map<DWORD, MemStream *> m_sRecordMemStreamMap[MEMMAP_COUNT];

	std::map<std::pair<void *,DWORD >, MemStream *> m_sCommandRecordMemStreamMap;

	std::mutex m_sMutex;


	int m_curRecordMemMapIdx;
	bool m_isRecording;
	std::mutex m_recordMutex;
};

