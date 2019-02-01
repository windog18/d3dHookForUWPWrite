////////////////
// D3D12 HOOK //
////////////////

#include "main.h"
#include <sstream>



#define DECLARE_FUNCTIONPTR(DReturnType,DFunctionName,...) \
typedef DReturnType(__stdcall* DFunctionName)(__VA_ARGS__);\
DFunctionName o##DFunctionName = NULL; \
DReturnType __stdcall hk##DFunctionName(__VA_ARGS__) \


//=========================================================================================================================// D3D12 HOOKS 
// D3D12 HOOKS Example

bool InitOnce = true;
bool InitOnce2 = true;
bool InitOnce3 = true;

ID3D12Device *dDevice = NULL;
ID3D12GraphicsCommandList *gDCommandList = NULL;

typedef long(__stdcall* CreateDevice12) (IUnknown *pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, REFIID riid, void **ppDevice);
CreateDevice12 oCreateDevice12 = NULL;


typedef long(__stdcall* Present12) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present12 oPresent12 = NULL;

/////////////////////////commandlist interface


typedef void(__stdcall *D3D12DrawInstanced)(ID3D12GraphicsCommandList *dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
D3D12DrawInstanced oD3D12DrawInstanced = NULL;

typedef void(__stdcall *D3D12DrawIndexedInstanced)(ID3D12GraphicsCommandList *dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex);
D3D12DrawIndexedInstanced oD3D12DrawIndexedInstanced = NULL;

typedef long(__stdcall* D3D12Dispatch)(ID3D12GraphicsCommandList *dCommandList, UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ);
D3D12Dispatch oD3D12Dispatch = NULL;

typedef void(__stdcall* D3D12CopyBufferRegion)(ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pDstBuffer, UINT64 DstOffset, ID3D12Resource *pSrcBuffer, UINT64 SrcOffset, UINT64 NumBytes);
D3D12CopyBufferRegion oD3D12CopyBufferRegion = NULL;

typedef void(__stdcall* D3D12CopyTextureRegion)(ID3D12GraphicsCommandList *dCommandList, 
	const D3D12_TEXTURE_COPY_LOCATION *pDst,
	UINT DstX, UINT DstY, UINT DstZ,
	const D3D12_TEXTURE_COPY_LOCATION *pSrc);
D3D12CopyTextureRegion oD3D12CopyTextureRegion = NULL;

typedef void(__stdcall* D3D12CopyResource)(ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pDstResource, ID3D12Resource *pSrcResource);
D3D12CopyResource oD3D12CopyResource = NULL;

// typedef void(__stdcall* D3D12CopyTiles)(ID3D12GraphicsCommandList *dCommandList, 
// 	                                    ID3D12Resource *pTiledResource, 
// 	                                    const D3D12_TILED_RESOURCE_COORDINATE *pTileRegionStartCoordinate, 
// 	                                    const D3D12_TILE_REGION_SIZE *pTileRegionSize,
// 	                                    ID3D12Resource *pBuffer, 
// 	                                    UINT64 BufferStartOffsetInBytes, 
// 	                                    D3D12_TILE_COPY_FLAGS Flags);
// D3D12CopyTiles oD3D12CopyTiles = NULL;

//=========================================================================================================================//


//////////////////////////////////////////////////////////commandQueue interface
typedef long(__stdcall* D3D12QueryInterface)(ID3D12CommandQueue * dCommandQueue, REFIID riid, void **ppvObject);
D3D12QueryInterface oD3D12QueryInterface = NULL;


DECLARE_FUNCTIONPTR(void, D3D12ExecuteCommandLists, ID3D12CommandQueue * dCommandQueue, UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists)
{
	Log("[d3d12] D3D12ExecuteCommandLists");
	return oD3D12ExecuteCommandLists(dCommandQueue, NumCommandLists, ppCommandLists);
}



//////////////////////////////////////////////////////////device interface
DECLARE_FUNCTIONPTR(long, D3D12DeviceQueryInterface, ID3D12Device* dDevice, REFIID riid, void **ppvObject) //0
{
	Log("[d3d12] create D3D12DeviceQueryInterface");
	return oD3D12DeviceQueryInterface(dDevice, riid, ppvObject);
}

DECLARE_FUNCTIONPTR(long, D3D12DeviceCreateCommandQueue, ID3D12Device *dDevice, const D3D12_COMMAND_QUEUE_DESC *pDesc, REFIID riid, void **ppCommandQueue) //8
{
	Log("[d3d12] create D3D12DeviceCreateCommandQueue");
	return oD3D12DeviceCreateCommandQueue(dDevice, pDesc, riid, ppCommandQueue);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommandAllocator, ID3D12Device *dDevice, D3D12_COMMAND_LIST_TYPE type, REFIID riid, void **ppCommandAllocator) //9
{
	Log("[d3d12] create D3D12CreateCommandAllocator");
	return oD3D12CreateCommandAllocator(dDevice, type, riid, ppCommandAllocator);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommittedResource, ID3D12Device * dDevice,
const D3D12_HEAP_PROPERTIES *pHeapProperties,
D3D12_HEAP_FLAGS HeapFlags,
const D3D12_RESOURCE_DESC *pDesc,
D3D12_RESOURCE_STATES InitialResourceState,
const D3D12_CLEAR_VALUE *pOptimizedClearValue,
REFIID riidResource,
void **ppvResource)
{
	Log("[d3d12] create D3D12CreateCommittedResource");
	return oD3D12CreateCommittedResource(dDevice, pHeapProperties, HeapFlags, pDesc, InitialResourceState, pOptimizedClearValue, riidResource, ppvResource);
}

long __stdcall hkPresent12(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (InitOnce)
	{
		
		InitOnce = false;

		//get device
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void **)&dDevice)))
		{
			pSwapChain->GetDevice(__uuidof(dDevice), (void**)&dDevice);
		}
	}
	//Log("[d3d12]Present called");
	
	return oPresent12(pSwapChain, SyncInterval, Flags);
}

long __stdcall hkCreateDevice12(IUnknown *pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, REFIID riid, void **ppDevice)
{
	Log("[d3d12]Create Device Called");
	return oCreateDevice12(pAdapter, MinimumFeatureLevel, riid, ppDevice);
}

//=========================================================================================================================commandQueue hook interface//
long __stdcall hkD3D12QueryInterface(ID3D12CommandQueue * dCommandQueue, REFIID riid, void **ppvObject)
{
	//Log("[d3d12]QueryInterface");
	return oD3D12QueryInterface(dCommandQueue, riid, ppvObject);
}


//=========================================================================================================================commandlist hook interface//

void __stdcall hkD3D12DrawInstanced(ID3D12GraphicsCommandList *dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
{
	if (InitOnce2)
	{

		InitOnce2 = false;
		std::string outputStr;
		std::stringstream  ss;
		ss << VertexCountPerInstance;
		ss >> outputStr;
		//OutputDebugStringA(outputStr.c_str());
		Log(outputStr.c_str());
	}


	//no fog/smoke/glow test (elemental-demo-dx12)
// 	const float f[4] = { 0, 0, 0, 0 };
// 	dCommandList->OMSetBlendFactor(f);

	return oD3D12DrawInstanced(dCommandList, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

void __stdcall hkD3D12DrawIndexedInstanced(ID3D12GraphicsCommandList *dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex)
{
	if (InitOnce3)
	{
		InitOnce3 = false;
		Log("[d3d12]DrawIndexedInstanced called");
	}


	//no fog/smoke/glow test (elemental-demo-dx12)
// 	const float f[4] = { 0, 0, 0, 0 };
// 	dCommandList->OMSetBlendFactor(f);

	return oD3D12DrawIndexedInstanced(dCommandList, IndexCount, InstanceCount, StartIndex, BaseVertex);
}

long __stdcall hkD3D12Dispatch(ID3D12GraphicsCommandList *dCommandList, UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ)
{
	//Log("[d3d12]Dispatch ");
	return oD3D12Dispatch(dCommandList, ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
}

void __stdcall hkD3D12CopyBufferRegion(ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pDstBuffer, UINT64 DstOffset, ID3D12Resource *pSrcBuffer, UINT64 SrcOffset, UINT64 NumBytes)
{
	//Log("[d3d12]hkD3D12CopyBufferRegion ");
	oD3D12CopyBufferRegion(dCommandList, pDstBuffer, DstOffset, pSrcBuffer, SrcOffset, NumBytes);
}

void __stdcall hkD3D12CopyTextureRegion(ID3D12GraphicsCommandList *dCommandList, 
	                                    const D3D12_TEXTURE_COPY_LOCATION *pDst, 
	                                    UINT DstX, UINT DstY, UINT DstZ, 
	                                    const D3D12_TEXTURE_COPY_LOCATION *pSrc)
{
	//Log("[d3d12]hkD3D12CopyTextureRegion ");
	oD3D12CopyTextureRegion(dCommandList, pDst, DstX, DstY, DstZ, pSrc);
}

void __stdcall hkD3D12CopyResource(ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pDstResource, ID3D12Resource *pSrcResource)
{
	//Log("[d3d12]hkD3D12CopyResource ");
	oD3D12CopyResource(dCommandList, pDstResource, pSrcResource);
}

// void __stdcall hkD3D12CopyTiles(ID3D12GraphicsCommandList *dCommandList,
// 	ID3D12Resource *pTiledResource,
// 	const D3D12_TILED_RESOURCE_COORDINATE *pTileRegionStartCoordinate,
// 	const D3D12_TILE_REGION_SIZE *pTileRegionSize,
// 	ID3D12Resource *pBuffer,
// 	UINT64 BufferStartOffsetInBytes,
// 	D3D12_TILE_COPY_FLAGS Flags)
// {
// 	Log("[d3d12]hkD3D12CopyTiles ");
// 	oD3D12CopyTiles(dCommandList, pTiledResource, pTileRegionStartCoordinate, pTileRegionSize, pBuffer, BufferStartOffsetInBytes, Flags);
// }

//========================================================================================================================= device hook interface//




//=========================================================================================================================//

int dx12Thread()
{
	if (dx12::init(dx12::RenderType::D3D12) == dx12::Status::Success)
	{
		//OutputDebugStringA("Hello 2019_2019_attach success_1");
		MH_Initialize();
		//OutputDebugStringA("Hello 2019_2019_attach success_2");
		MH_CreateHook((LPVOID)dx12::getMethodsTable()[0], hkD3D12DeviceQueryInterface, (LPVOID*)&oD3D12DeviceQueryInterface);
		//MH_CreateHook((LPVOID)dx12::getMethodsTable()[8], hkD3D12DeviceCreateCommandQueue, (LPVOID*)&oD3D12DeviceCreateCommandQueue);
		//MH_CreateHook((LPVOID)dx12::getMethodsTable()[9], hkD3D12CreateCommandAllocator, (LPVOID*)&oD3D12CreateCommandAllocator);
		MH_CreateHook((LPVOID)dx12::getMethodsTable()[27], hkD3D12CreateCommittedResource, (LPVOID*)&oD3D12CreateCommittedResource);

		MH_CreateHook((LPVOID)dx12::getMethodsTable()[140], hkPresent12, (LPVOID*)&oPresent12);

		MH_CreateHook((LPVOID)dx12::getMethodsTable()[44], hkD3D12QueryInterface, (LPVOID*)&oD3D12QueryInterface);
		MH_CreateHook((LPVOID)dx12::getMethodsTable()[44+10], hkD3D12ExecuteCommandLists, (LPVOID*)&oD3D12ExecuteCommandLists);

		MH_CreateHook((LPVOID)dx12::getMethodsTable()[84], hkD3D12DrawInstanced, (LPVOID*)&oD3D12DrawInstanced);
		MH_CreateHook((LPVOID)dx12::getMethodsTable()[85], hkD3D12DrawIndexedInstanced, (LPVOID*)&oD3D12DrawIndexedInstanced);
		MH_CreateHook((LPVOID)dx12::getMethodsTable()[86], hkD3D12Dispatch, (LPVOID*)&oD3D12Dispatch);
		MH_CreateHook((LPVOID)dx12::getMethodsTable()[87], hkD3D12CopyBufferRegion, (LPVOID*)&oD3D12CopyBufferRegion);
		MH_CreateHook((LPVOID)dx12::getMethodsTable()[88], hkD3D12CopyTextureRegion, (LPVOID*)&oD3D12CopyTextureRegion);
		MH_CreateHook((LPVOID)dx12::getMethodsTable()[89], hkD3D12CopyResource, (LPVOID*)&oD3D12CopyResource);


		MH_EnableHook((LPVOID)dx12::getMethodsTable()[0]);
		//MH_EnableHook((LPVOID)dx12::getMethodsTable()[8]);
		//MH_EnableHook((LPVOID)dx12::getMethodsTable()[9]);
		MH_EnableHook((LPVOID)dx12::getMethodsTable()[27]);

		MH_EnableHook((LPVOID)dx12::getMethodsTable()[140]);

		MH_EnableHook((LPVOID)dx12::getMethodsTable()[44]);
		MH_EnableHook((LPVOID)dx12::getMethodsTable()[44 + 10]);

		MH_EnableHook((LPVOID)dx12::getMethodsTable()[84]);
		MH_EnableHook((LPVOID)dx12::getMethodsTable()[85]);
		MH_EnableHook((LPVOID)dx12::getMethodsTable()[86]);
		MH_EnableHook((LPVOID)dx12::getMethodsTable()[87]);
		MH_EnableHook((LPVOID)dx12::getMethodsTable()[88]);
		MH_EnableHook((LPVOID)dx12::getMethodsTable()[89]);
	}

	return 0;
}


//=========================================================================================================================//

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID)
{
	DisableThreadLibraryCalls(hInstance);

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		const fs::path WritePath = DumpPath;
		std::error_code ErrorCode;
		if (fs::create_directories(WritePath.parent_path(), ErrorCode) == false && ErrorCode)
		{
			const std::string error_prefix("error code_syl: ");
			const std::string ErrorMessage(ErrorCode.message());
			OutputDebugStringA((error_prefix + ErrorMessage).c_str());
		}
		DisableThreadLibraryCalls(hInstance);
		GetModuleFileNameA(hInstance, dlldir, 512);
		for (size_t i = strlen(dlldir); i > 0; i--) { if (dlldir[i] == '\\') { dlldir[i + 1] = 0; break; } }
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dx12Thread, NULL, 0, NULL);
		break;
	}

	return TRUE;
}
