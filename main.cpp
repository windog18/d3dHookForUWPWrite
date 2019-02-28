////////////////
// D3D12 HOOK //
////////////////

#include "main.h"
#include <sstream>
#include "D3D12ResourceHookInterface.h"
#include "D3D12CommandListHookInterface.h"
#include "D3D12DeviceHookInterface.h"
#include <d3d11.h>
#include "./FW1FontWrapper/FW1FontWrapper.h"
#include <thread>
bool g_beginRecord = false;
std::mutex g_mutex;
//=========================================================================================================================// D3D12 HOOKS 
// D3D12 HOOKS Example

bool InitOnce = true;
bool InitOnce2 = true;
bool InitOnce3 = true;
void HookWindowProc(HWND hWnd);
ID3D12Device *dDevice = NULL;
ID3D12GraphicsCommandList *gDCommandList = NULL;

typedef long(__stdcall* CreateDevice12) (IUnknown *pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, REFIID riid, void **ppDevice);
CreateDevice12 oCreateDevice12 = NULL;


typedef long(__stdcall* Present12) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present12 oPresent12 = NULL;

/////////////////////////commandlist interface


// typedef void(__stdcall *D3D12DrawInstanced)(ID3D12GraphicsCommandList *dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
// D3D12DrawInstanced oD3D12DrawInstanced = NULL;
// 
// typedef void(__stdcall *D3D12DrawIndexedInstanced)(ID3D12GraphicsCommandList *dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex);
// D3D12DrawIndexedInstanced oD3D12DrawIndexedInstanced = NULL;
// 
// typedef long(__stdcall* D3D12Dispatch)(ID3D12GraphicsCommandList *dCommandList, UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ);
// D3D12Dispatch oD3D12Dispatch = NULL;
// 
// typedef void(__stdcall* D3D12CopyBufferRegion)(ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pDstBuffer, UINT64 DstOffset, ID3D12Resource *pSrcBuffer, UINT64 SrcOffset, UINT64 NumBytes);
// D3D12CopyBufferRegion oD3D12CopyBufferRegion = NULL;
// 
// typedef void(__stdcall* D3D12CopyTextureRegion)(ID3D12GraphicsCommandList *dCommandList, 
// 	const D3D12_TEXTURE_COPY_LOCATION *pDst,
// 	UINT DstX, UINT DstY, UINT DstZ,
// 	const D3D12_TEXTURE_COPY_LOCATION *pSrc, const D3D12_BOX *pSrcBox);
// D3D12CopyTextureRegion oD3D12CopyTextureRegion = NULL;
// 
// typedef void(__stdcall* D3D12CopyResource)(ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pDstResource, ID3D12Resource *pSrcResource);
// D3D12CopyResource oD3D12CopyResource = NULL;

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
	
	//Log("[d3d12] D3D12ExecuteCommandLists");
	return oD3D12ExecuteCommandLists(dCommandQueue, NumCommandLists, ppCommandLists);
}


long __stdcall hkPresent12(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	//Log_WithThreadID("[d3d12]present be called");
	if (InitOnce)
	{
		
		InitOnce = false;

		//get device
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void **)&dDevice)))
		{
			pSwapChain->GetDevice(__uuidof(dDevice), (void**)&dDevice);
// 			ID3D12CommandQueue* commandQueue;
// 			ID3D12CommandAllocator* commandAllocator;
// 			ID3D12GraphicsCommandList* commandList;


// 			D3D12_COMMAND_QUEUE_DESC queueDesc;
// 			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
// 			queueDesc.Priority = 0;
// 			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
// 			queueDesc.NodeMask = 0;
// 			dDevice->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**)&commandQueue);
// 			dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator);
// 			dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList);
// 
// 			UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
// 			//d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
// 			
// 			////createD3D11Device;
// 			ID3D11Device *d3d11Device;
// 			ID3D11DeviceContext *d3d11DeviceContext;
// 			D3D11On12CreateDevice(dDevice, d3d11DeviceFlags, nullptr,  0, reinterpret_cast<IUnknown**>(&commandQueue), 1, 0, &d3d11Device, &d3d11DeviceContext, nullptr);
// 
// 			GlobalGathering::GetInstance()->m_drawD3D11Device = d3d11Device;
// 			GlobalGathering::GetInstance()->m_drawD3D11DeviceContext = d3d11DeviceContext;
// 
// 
// 			static ID3D12Resource *d3d12Resource;
// 			static ID3D12DescriptorHeap *rtvHeap;
// 			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
// 			rtvHeapDesc.NumDescriptors = 1;
// 			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
// 			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
// 			dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
// 			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());
// 			pSwapChain->GetBuffer(0, IID_PPV_ARGS(&d3d12Resource));
// 			dDevice->CreateRenderTargetView(d3d12Resource, NULL, rtvHandle);
// 			d3d12Resource->Release();
// 			rtvHeap->Release();

			DXGI_SWAP_CHAIN_DESC sd;
			
			if (pSwapChain->GetDesc(&sd) == S_OK) {
				stringstream ss;
				ss << sd.BufferCount;
				Log(ss.str().c_str());
				ss << sd.Flags;
				Log(ss.str().c_str());
				ss << sd.Windowed;
				Log(ss.str().c_str());
			}
			HookWindowProc(sd.OutputWindow);
		}
		


	}
	
	bool recordState = GlobalGathering::GetInstance()->IsRecording();
	bool beginRecordState = GetBeginRecordState();
	static int count = 0;
	if (beginRecordState || recordState) {
// 		if (recordState) {
// 			GlobalGathering::GetInstance()->WriteAllBufferToResult();
// 			ResetRecordState();
// 		}
// 		ToggleRecordState();
		if (!beginRecordState && recordState) {
			GlobalGathering::GetInstance()->WriteAllBufferToResult();
			OutputDebugStringA("start write files");
			ResetRecordState();
		}
		GlobalGathering::GetInstance()->SetRecording(beginRecordState);
// 		//OutputDebugStringA("dsfsdfsfsfsdfs");
// 		if (count == 2) {
// 			ToggleRecordState();
// 			count = 0;
// 		}
		//count = count + 1;
	}

	
	


	//Log_WithThreadID("[d3d12] hkPresent12");
	/*
	if (GlobalGathering::GetInstance()->m_drawD3D11Device != NULL) {
		//Log("Begin to draw debug string");
		static IFW1Factory *pFW1Factory = nullptr;
		static HRESULT hResult;
		if (!pFW1Factory)
			hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);

		static IFW1FontWrapper *pFontWrapper = nullptr;
		if (!pFontWrapper) {
			hResult = pFW1Factory->CreateFontWrapper(GlobalGathering::GetInstance()->m_drawD3D11Device, L"Arial", &pFontWrapper);
			if (pFontWrapper != NULL) {
				Log("font wrapped create sccuessed!");
			}
		}
	
			
		static auto x = 1280.0f;

		//
		// Very crude marquee mechanism for demonstration
		// 
		if (x <= -1280.0f)x = 1280.0f;

		pFontWrapper->DrawString(
			GlobalGathering::GetInstance()->m_drawD3D11DeviceContext,
			L"Injected via Indicium-Supra by Nefarius",// String
			50.0f,// Font size
			x--,// X position
			30.0f,// Y position
			0xff0099ff,// Text color, 0xAaBbGgRr
			FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
	}
	*/
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

//========================================================================================================================= device hook interface//




//=========================================================================================================================//
typedef LRESULT(__stdcall* WINPROC) (_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
WINPROC OriginalDefWindowProc = nullptr;
WINPROC OriginalWindowProc = nullptr;


LRESULT WINAPI DetourDefWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
// 	static std::once_flag flag;
// 	std::call_once(flag, []() {Logger::get("DetourDefWindowProc").information("++ DetourDefWindowProc called"); });
// 
 	ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

	return OriginalDefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI DetourWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
// 	static std::once_flag flag;
// 	std::call_once(flag, []() {Logger::get("DetourWindowProc").information("++ DetourWindowProc called"); });
// 
 	ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

	return OriginalWindowProc(hWnd, Msg, wParam, lParam);
}


 LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	 //Log_WithThreadID(__FUNCTION__);
// 	 if (msg != 132 && msg != 32 && msg != 675) {
// 		 stringstream ss;
// 		 ss << msg;
// 		 std::string outputString = "procHandler be called " + ss.str();
// 		 Log(outputString.c_str());
// 	 }

	 if (msg == 77) { //F1
		 ToggleBeginRecordState();
	 }

	 switch (msg)
	 {
	 case WM_KEYDOWN:
	 case WM_SYSKEYDOWN:
	 case WM_LBUTTONDOWN:
	 case WM_RBUTTONDOWN:
	 case WM_MBUTTONDOWN:
	 case WM_LBUTTONDBLCLK:
	 case WM_RBUTTONDBLCLK:
	 case WM_MBUTTONDBLCLK:
		 Log("windows key pressed! -- 2019-SYL");
		 break;
	 default:
		 break;
	 }
	 
	 return 0;
// 	 if (ImGui::GetCurrentContext() == NULL)
// 		 return 0;
// 
// 	ImGuiIO& io = ImGui::GetIO();
// 	switch (msg)
// 	{
// 	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
// 	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
// 	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
// 	{
// 		int button = 0;
// 		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) button = 0;
// 		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) button = 1;
// 		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) button = 2;
// 		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
// 			::SetCapture(hwnd);
// 		io.MouseDown[button] = true;
// 		return 0;
// 	}
// 	case WM_LBUTTONUP:
// 	case WM_RBUTTONUP:
// 	case WM_MBUTTONUP:
// 	{
// 		int button = 0;
// 		if (msg == WM_LBUTTONUP) button = 0;
// 		if (msg == WM_RBUTTONUP) button = 1;
// 		if (msg == WM_MBUTTONUP) button = 2;
// 		io.MouseDown[button] = false;
// 		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == hwnd)
// 			::ReleaseCapture();
// 		return 0;
// 	}
// 	case WM_MOUSEWHEEL:
// 		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
// 		return 0;
// 	case WM_MOUSEHWHEEL:
// 		io.MouseWheelH += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
// 		return 0;
// 	case WM_MOUSEMOVE:
// 		io.MousePos.x = (signed short)(lParam);
// 		io.MousePos.y = (signed short)(lParam >> 16);
// 		return 0;
// 	case WM_KEYDOWN:
// 	case WM_SYSKEYDOWN:
// 		if (wParam < 256)
// 			io.KeysDown[wParam] = 1;
// 		return 0;
// 	case WM_KEYUP:
// 	case WM_SYSKEYUP:
// 		if (wParam < 256)
// 			io.KeysDown[wParam] = 0;
// 		return 0;
// 	case WM_CHAR:
// 		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
// 		if (wParam > 0 && wParam < 0x10000)
// 			io.AddInputCharacter((unsigned short)wParam);
// 		return 0;
// 	case WM_SETCURSOR:
// 		if (LOWORD(lParam) == HTCLIENT && ImGui_ImplWin32_UpdateMouseCursor())
// 			return 1;
// 		return 0;
// 	}
// 	return 0;
}

void HookWindowProc(HWND hWnd)
{
	MH_STATUS ret;
	if ((ret = MH_CreateHook(&DefWindowProcW,&DetourDefWindowProc, reinterpret_cast<LPVOID*>(&OriginalDefWindowProc))) != MH_OK)
	{
		Log("Couldn't create hook for DefWindowProcW: ");
	}

	if (ret == MH_OK && MH_EnableHook(&DefWindowProcW) != MH_OK)
	{
		Log("Couldn't enable DefWindowProcW hook");
	}

	if ((ret = MH_CreateHook(
		&DefWindowProcA,
		&DetourDefWindowProc,
		reinterpret_cast<LPVOID*>(&OriginalDefWindowProc))
		) != MH_OK)
	{
		Log("Couldn't create hook for DefWindowProcA");
		return;
	}

	if (ret == MH_OK && MH_EnableHook(&DefWindowProcA) != MH_OK)
	{
		Log("Couldn't enable DefWindowProcW hook");
	}

	auto lptrWndProc = reinterpret_cast<WINPROC>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
	if (hWnd == NULL) {
		Log("could not GetWindowLongPtr ");
	}

	if (MH_CreateHook(lptrWndProc, &DetourWindowProc, reinterpret_cast<LPVOID*>(&OriginalWindowProc)) != MH_OK)
	{
		Log("Couldn't create hook for GWLP_WNDPROC");
		return;
	}

	if (MH_EnableHook(lptrWndProc) != MH_OK)
	{
		Log("Couldn't enable GWLP_WNDPROC hook");
	}

}

HHOOK g_hHook = NULL;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	Log("hooked keyboard!--------syl-2019");
	//MessageBox(NULL, IsNumber(wParam), _T("Message"), 0);
	return CallNextHookEx(0, nCode, wParam, lParam);
	
}
HINSTANCE g_instanceID = NULL;

int dx12Thread()
{

	if (dx12::init(dx12::RenderType::D3D12) == dx12::Status::Success)
	{

		MH_Initialize();

 		//CreateHookD3D12CommandListInterface(dx12::getMethodsTable());
 //		CreateHookD3D12ResourceInterface(dx12::getMethodsTable());
// 		
 		CreateHookD3D12DeviceInterface(dx12::getMethodsTable());

		//MH_CreateHook((LPVOID)dx12::getMethodsTable()[0], hkD3D12DeviceQueryInterface, (LPVOID*)&oD3D12DeviceQueryInterface);
		//MH_CreateHook((LPVOID)dx12::getMethodsTable()[8], hkD3D12DeviceCreateCommandQueue, (LPVOID*)&oD3D12DeviceCreateCommandQueue);
		//MH_CreateHook((LPVOID)dx12::getMethodsTable()[9], hkD3D12CreateCommandAllocator, (LPVOID*)&oD3D12CreateCommandAllocator);
		//MH_CreateHook((LPVOID)dx12::getMethodsTable()[27], hkD3D12CreateCommittedResource, (LPVOID*)&oD3D12CreateCommittedResource);

		MH_CreateHook((LPVOID)dx12::getMethodsTable()[140], hkPresent12, (LPVOID*)&oPresent12);

		MH_CreateHook((LPVOID)dx12::getMethodsTable()[44], hkD3D12QueryInterface, (LPVOID*)&oD3D12QueryInterface);
		MH_CreateHook((LPVOID)dx12::getMethodsTable()[44+10], hkD3D12ExecuteCommandLists, (LPVOID*)&oD3D12ExecuteCommandLists);



		//MH_EnableHook((LPVOID)dx12::getMethodsTable()[0]);
		//MH_EnableHook((LPVOID)dx12::getMethodsTable()[8]);
		//MH_EnableHook((LPVOID)dx12::getMethodsTable()[9]);
		//MH_EnableHook((LPVOID)dx12::getMethodsTable()[27]);


		MH_EnableHook((LPVOID)dx12::getMethodsTable()[140]);

		MH_EnableHook((LPVOID)dx12::getMethodsTable()[44]);
		MH_EnableHook((LPVOID)dx12::getMethodsTable()[44 + 10]);

	}

	return 0;
}


//=========================================================================================================================//

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID)
{
	char dlldir[320];
	DisableThreadLibraryCalls(hInstance);

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		//SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, hInstance, 0);
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
	g_instanceID = hInstance;
	return TRUE;
}
