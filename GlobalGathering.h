#pragma once
#include <dxgi.h>
#include <d3d12.h>
#include <d3d11.h>
#include <vector>
#include <map>
#include <mutex>

class MemStream;

class ResourceTempData {
public:
	
 	static void SetTempMapData(DWORD threadID,void *ptr) {
		std::lock_guard<std::mutex> lock(m_sMutex);
		m_sTempMap[threadID] = ptr;
	}


	static void* GetTempMapData(DWORD threadID) {
		void *ptr = NULL;
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
	static std::map<DWORD, void *>  m_sTempMap;

	static std::mutex m_sMutex;
};


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
			m_isRecording = recordState;
		}
	}


	void WriteAllBufferToResult();
	void ResetRecordState();


	ID3D12Device *GetDevice(int idx);



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


	std::map<DWORD, MemStream *> m_sRecordMemStreamMap;
	std::map<std::pair<void *,DWORD >, MemStream *> m_sCommandRecordMemStreamMap;

	std::mutex m_sMutex;


	bool m_isRecording;
	std::mutex m_recordMutex;
};

