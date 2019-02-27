#pragma once
#include "Common.h"

#include "memstream.h"
////base bias 0
////total function: 44
/*
[0]   QueryInterface
[1]   AddRef
[2]   Release
[3]   GetPrivateData
[4]   SetPrivateData
[5]   SetPrivateDataInterface
[6]   SetName
[7]   GetNodeCount
[8]   CreateCommandQueue
[9]   CreateCommandAllocator
[10]  CreateGraphicsPipelineState
[11]  CreateComputePipelineState
[12]  CreateCommandList
[13]  CheckFeatureSupport
[14]  CreateDescriptorHeap
[15]  GetDescriptorHandleIncrementSize
[16]  CreateRootSignature
[17]  CreateConstantBufferView
[18]  CreateShaderResourceView
[19]  CreateUnorderedAccessView
[20]  CreateRenderTargetView
[21]  CreateDepthStencilView
[22]  CreateSampler
[23]  CopyDescriptors
[24]  CopyDescriptorsSimple
[25]  GetResourceAllocationInfo
[26]  GetCustomHeapProperties
[27]  CreateCommittedResource
[28]  CreateHeap
[29]  CreatePlacedResource
[30]  CreateReservedResource
[31]  CreateSharedHandle
[32]  OpenSharedHandle
[33]  OpenSharedHandleByName
[34]  MakeResident
[35]  Evict
[36]  CreateFence
[37]  GetDeviceRemovedReason
[38]  GetCopyableFootprints
[39]  CreateQueryHeap
[40]  SetStablePowerState
[41]  CreateCommandSignature
[42]  GetResourceTiling
[43]  GetAdapterLuid
*/

//////////////////////////////////////////////////////////device interface
DECLARE_FUNCTIONPTR(long, D3D12DeviceQueryInterface, ID3D12Device* dDevice, REFIID riid, void **ppvObject) //0
{
	Log("[d3d12] create D3D12DeviceQueryInterface");
	return oD3D12DeviceQueryInterface(dDevice, riid, ppvObject);
}

DECLARE_FUNCTIONPTR(UINT, D3D12DeviceGetNodeCount, ID3D12Device *dDevice)  //7
{
	return oD3D12DeviceGetNodeCount(dDevice);
}

DECLARE_FUNCTIONPTR(long, D3D12DeviceCreateCommandQueue, ID3D12Device *dDevice, const D3D12_COMMAND_QUEUE_DESC *pDesc, REFIID riid, void **ppCommandQueue) //8
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	Log("[d3d12] create D3D12DeviceCreateCommandQueue");
	return oD3D12DeviceCreateCommandQueue(dDevice, pDesc, riid, ppCommandQueue);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommandAllocator, ID3D12Device *dDevice, D3D12_COMMAND_LIST_TYPE type, REFIID riid, void **ppCommandAllocator) //9
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	Log("[d3d12] create D3D12CreateCommandAllocator");
	return oD3D12CreateCommandAllocator(dDevice, type, riid, ppCommandAllocator);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateGraphicsPipelineState, ID3D12Device *dDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc, REFIID riid, void **ppPipelineState) //10
{
	if (GlobalGathering::GetInstance()->GatherDevice(dDevice) == true) {
		const void *address = static_cast<const void*>(dDevice);
		std::stringstream ss;
		ss << address;
		std::string outputResult = "[d3d12] D3D12CreateGraphicsPipelineState: ";
		outputResult = outputResult + std::string(ss.str());
		Log(outputResult.c_str());
	}
	HRESULT result = oD3D12CreateGraphicsPipelineState(dDevice, pDesc, riid, ppPipelineState);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateGraphicsPipelineState);
	streaminstance->write(dDevice);
	WriteStream(*pDesc, streaminstance);
	streaminstance->write(riid);
	streaminstance->write(*ppPipelineState);
	RecordEnd
	return result;
}

DECLARE_FUNCTIONPTR(long, D3D12CreateComputePipelineState, ID3D12Device *dDevice, const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc, REFIID riid, void **ppPipelineState) //11
{
	Log("[d3d12] D3D12CreateComputePipelineState");
	return oD3D12CreateComputePipelineState(dDevice, pDesc, riid, ppPipelineState);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommandList, ID3D12Device *dDevice, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator *pCommandAllocator,
ID3D12PipelineState *pInitialState, REFIID riid, void **ppCommandList) //12
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	Log("[d3d12] D3D12CreateCommandList");
	return oD3D12CreateCommandList(dDevice, nodeMask, type, pCommandAllocator, pInitialState, riid, ppCommandList);
}

DECLARE_FUNCTIONPTR(long, D3D12CheckFeatureSupport, ID3D12Device *dDevice, D3D12_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize) //13
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	//Log("[d3d12] D3D12CheckFeatureSupport");
	return oD3D12CheckFeatureSupport(dDevice, Feature, pFeatureSupportData, FeatureSupportDataSize);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateDescriptorHeap, ID3D12Device *dDevice, const D3D12_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc, REFIID riid, void **ppvHeap) //14
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	LOG_ONCE("[d3d12] D3D12CreateDescriptorHeap");
	return oD3D12CreateDescriptorHeap(dDevice, pDescriptorHeapDesc, riid, ppvHeap);
}

DECLARE_FUNCTIONPTR(UINT, D3D12GetDescriptorHandleIncrementSize, ID3D12Device *dDevice, D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType) //15
{
	//Log("[d3d12] D3D12GetDescriptorHandleIncrementSize");
	return oD3D12GetDescriptorHandleIncrementSize(dDevice, DescriptorHeapType);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateRootSignature, ID3D12Device *dDevice, UINT nodeMask, const void *pBlobWithRootSignature, SIZE_T blobLengthInBytes, 
REFIID riid, void **ppvRootSignature)                        //16
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	LOG_ONCE("[d3d12] D3D12CreateRootSignature");

	HRESULT ert = oD3D12CreateRootSignature(dDevice, nodeMask, pBlobWithRootSignature, blobLengthInBytes, riid, ppvRootSignature);
	RecordStart
		MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateRootSignature);
		streaminstance->write(dDevice);
		streaminstance->write(nodeMask);
		streaminstance->write(blobLengthInBytes);
		streaminstance->write(pBlobWithRootSignature, blobLengthInBytes);
		streaminstance->write(riid);
		streaminstance->write(*ppvRootSignature);
	RecordEnd
	return ert;
}

DECLARE_FUNCTIONPTR(void, D3D12CreateConstantBufferView, ID3D12Device *dDevice, const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //17
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	//Log("[d3d12] D3D12CreateConstantBufferView");
	return oD3D12CreateConstantBufferView(dDevice, pDesc, DestDescriptor);
}

DECLARE_FUNCTIONPTR(void, D3D12CreateShaderResourceView, ID3D12Device *dDevice, ID3D12Resource *pResource, const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //18
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	//Log("[d3d12] D3D12CreateShaderResourceView");
	oD3D12CreateShaderResourceView(dDevice, pResource, pDesc, DestDescriptor);


	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateShaderResourceView);
	streaminstance->write(dDevice);
	streaminstance->write(pResource);
	streaminstance->writePointerValue(pDesc);
	streaminstance->write(DestDescriptor);

	//ID3D12DescriptorHeap* heap = XD3D12DescriptorHeap::m_handlemap1[DestDescriptor.ptr];
	//streaminstance->write(heap);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12CreateUnorderedAccessView, ID3D12Device *dDevice, ID3D12Resource *pResource, ID3D12Resource *pCounterResource,
const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //19
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	//Log("[d3d12] D3D12CreateUnorderedAccessView");
	return oD3D12CreateUnorderedAccessView(dDevice, pResource, pCounterResource, pDesc, DestDescriptor);
}

DECLARE_FUNCTIONPTR(void, D3D12CreateRenderTargetView, ID3D12Device *dDevice, ID3D12Resource *pResource,
const D3D12_RENDER_TARGET_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //20
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	return oD3D12CreateRenderTargetView(dDevice, pResource, pDesc, DestDescriptor);
}

DECLARE_FUNCTIONPTR(void, D3D12CreateDepthStencilView, ID3D12Device *dDevice, ID3D12Resource *pResource, 
const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //21
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	return oD3D12CreateDepthStencilView(dDevice, pResource, pDesc, DestDescriptor);
	
}

DECLARE_FUNCTIONPTR(void, D3D12CreateSampler, ID3D12Device *dDevice, const D3D12_SAMPLER_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) //22
{
	oD3D12CreateSampler(dDevice, pDesc, DestDescriptor);

	RecordStart
	MemStream* instancestream = GetStreamFromThreadID();
	instancestream->write(Device_CreateSampler);
	instancestream->write(dDevice);
	instancestream->write(*pDesc);

	//ID3D12DescriptorHeap* heap = XD3D12DescriptorHeap::m_handlemap1[DestDescriptor.ptr];
	instancestream->write(DestDescriptor);
	RecordEnd
}


DECLARE_FUNCTIONPTR(void, D3D12CopyDescriptors, ID3D12Device *dDevice, UINT NumDestDescriptorRanges, const D3D12_CPU_DESCRIPTOR_HANDLE *pDestDescriptorRangeStarts,
const UINT *pDestDescriptorRangeSizes, UINT NumSrcDescriptorRanges, const D3D12_CPU_DESCRIPTOR_HANDLE *pSrcDescriptorRangeStarts,
const UINT *pSrcDescriptorRangeSizes, D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType) //23
{
	return oD3D12CopyDescriptors(dDevice, NumDestDescriptorRanges, pDestDescriptorRangeStarts, pDestDescriptorRangeSizes,
		NumSrcDescriptorRanges, pSrcDescriptorRangeStarts, pSrcDescriptorRangeSizes, DescriptorHeapsType);
}

DECLARE_FUNCTIONPTR(void, D3D12CopyDescriptorsSimple, ID3D12Device *dDevice, UINT NumDescriptors, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart, D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType) //24
{
	return oD3D12CopyDescriptorsSimple(dDevice, NumDescriptors, DestDescriptorRangeStart, SrcDescriptorRangeStart, DescriptorHeapsType);
}

DECLARE_FUNCTIONPTR(D3D12_RESOURCE_ALLOCATION_INFO, D3D12GetResourceAllocationInfo, ID3D12Device *dDevice, UINT visibleMask,
UINT numResourceDescs, const D3D12_RESOURCE_DESC *pResourceDescs) //25
{
	return oD3D12GetResourceAllocationInfo(dDevice, visibleMask, numResourceDescs, pResourceDescs);
}

DECLARE_FUNCTIONPTR(D3D12_HEAP_PROPERTIES, D3D12GetCustomHeapProperties, ID3D12Device *dDevice, UINT nodeMask, D3D12_HEAP_TYPE heapType) //26
{
	return oD3D12GetCustomHeapProperties(dDevice, nodeMask, heapType);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommittedResource, ID3D12Device *dDevice,
const D3D12_HEAP_PROPERTIES *pHeapProperties,
D3D12_HEAP_FLAGS HeapFlags,
const D3D12_RESOURCE_DESC *pDesc,
D3D12_RESOURCE_STATES InitialResourceState,
const D3D12_CLEAR_VALUE *pOptimizedClearValue,
REFIID riidResource,
void **ppvResource) //27
{
	GlobalGathering::GetInstance()->GatherDevice(dDevice);
	//Log("[d3d12] create D3D12CreateCommittedResource");
	ID3D12Resource* pres;
	HRESULT result = oD3D12CreateCommittedResource(dDevice, pHeapProperties, HeapFlags, pDesc, InitialResourceState, pOptimizedClearValue, riidResource, (void**)&pres);
	*ppvResource = pres;

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Device_CreateCommittedResource);

	streaminstance->write(dDevice);
	streaminstance->write(*pHeapProperties);
	streaminstance->write(HeapFlags);
	streaminstance->writePointerValue(pDesc);
	streaminstance->write(InitialResourceState);
	streaminstance->writePointerValue(pOptimizedClearValue);
	streaminstance->write(riidResource);
	streaminstance->write(pres);
	RecordEnd
	return result;
}

DECLARE_FUNCTIONPTR(long, D3D12CreateHeap, ID3D12Device *dDevice, const D3D12_HEAP_DESC *pDesc, REFIID riid, void **ppvHeap) //28
{
	return oD3D12CreateHeap(dDevice, pDesc, riid, ppvHeap);
}

DECLARE_FUNCTIONPTR(long, D3D12CreatePlacedResource, ID3D12Device *dDevice, ID3D12Heap *pHeap, UINT64 HeapOffset, const D3D12_RESOURCE_DESC *pDesc,
D3D12_RESOURCE_STATES InitialState, const D3D12_CLEAR_VALUE *pOptimizedClearValue, REFIID riid, void **ppvResource) //29
{
	return oD3D12CreatePlacedResource(dDevice, pHeap, HeapOffset, pDesc, InitialState, pOptimizedClearValue, riid, ppvResource);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateReservedResource, ID3D12Device *dDevice, const D3D12_RESOURCE_DESC *pDesc, D3D12_RESOURCE_STATES InitialState,
const D3D12_CLEAR_VALUE *pOptimizedClearValue, REFIID riid, void **ppvResource) //30
{
	return oD3D12CreateReservedResource(dDevice, pDesc, InitialState, pOptimizedClearValue, riid, ppvResource);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateSharedHandle, ID3D12Device *dDevice, ID3D12DeviceChild *pObject, const SECURITY_ATTRIBUTES *pAttributes,
DWORD Access, LPCWSTR Name, HANDLE *pHandle) //31
{
	return oD3D12CreateSharedHandle(dDevice, pObject, pAttributes, Access, Name, pHandle);
}

DECLARE_FUNCTIONPTR(long, D3D12OpenSharedHandle, ID3D12Device *dDevice, HANDLE NTHandle, REFIID riid, void **ppvObj) //32
{
	return oD3D12OpenSharedHandle(dDevice, NTHandle, riid, ppvObj);
}

DECLARE_FUNCTIONPTR(long, D3D12OpenSharedHandleByName, ID3D12Device *dDevice, LPCWSTR Name, DWORD Access, HANDLE *pNTHandle) //33
{
	return oD3D12OpenSharedHandleByName(dDevice, Name, Access, pNTHandle);
}

DECLARE_FUNCTIONPTR(long, D3D12MakeResident, ID3D12Device *dDevice, UINT NumObjects, ID3D12Pageable *const *ppObjects) //34
{
	return oD3D12MakeResident(dDevice, NumObjects, ppObjects);
}

DECLARE_FUNCTIONPTR(long, D3D12Evict, ID3D12Device *dDevice, UINT NumObjects, ID3D12Pageable *const *ppObjects) //35
{
	return oD3D12Evict(dDevice, NumObjects, ppObjects);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateFence, ID3D12Device *dDevice, UINT64 InitialValue, D3D12_FENCE_FLAGS Flags, REFIID riid, void **ppFence) //36
{
	return oD3D12CreateFence(dDevice, InitialValue, Flags, riid, ppFence);
}

DECLARE_FUNCTIONPTR(long, D3D12GetDeviceRemovedReason, ID3D12Device *dDevice) //37
{
	return oD3D12GetDeviceRemovedReason(dDevice);
}

DECLARE_FUNCTIONPTR(void, D3D12GetCopyableFootprints, ID3D12Device *dDevice, const D3D12_RESOURCE_DESC *pResourceDesc, UINT FirstSubresource, 
UINT NumSubresources, UINT64 BaseOffset, D3D12_PLACED_SUBRESOURCE_FOOTPRINT *pLayouts, UINT *pNumRows, UINT64 *pRowSizeInBytes, UINT64 *pTotalBytes) //38
{
	return oD3D12GetCopyableFootprints(dDevice, pResourceDesc, FirstSubresource, NumSubresources, BaseOffset, pLayouts, pNumRows, pRowSizeInBytes, pTotalBytes);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateQueryHeap, ID3D12Device *dDevice, const D3D12_QUERY_HEAP_DESC *pDesc, REFIID riid, void **ppvHeap) //39
{
	return oD3D12CreateQueryHeap(dDevice, pDesc, riid, ppvHeap);
}

DECLARE_FUNCTIONPTR(long, D3D12SetStablePowerState, ID3D12Device *dDevice, BOOL Enable) //40
{
	return oD3D12SetStablePowerState(dDevice, Enable);
}

DECLARE_FUNCTIONPTR(long, D3D12CreateCommandSignature, ID3D12Device *dDevice, const D3D12_COMMAND_SIGNATURE_DESC *pDesc, ID3D12RootSignature *pRootSignature,
REFIID riid, void **ppvCommandSignature) //41
{
	return oD3D12CreateCommandSignature(dDevice, pDesc, pRootSignature, riid, ppvCommandSignature);
}

DECLARE_FUNCTIONPTR(void, D3D12GetResourceTiling, ID3D12Device *dDevice, ID3D12Resource *pTiledResource, UINT *pNumTilesForEntireResource, D3D12_PACKED_MIP_INFO *pPackedMipDesc,
D3D12_TILE_SHAPE *pStandardTileShapeForNonPackedMips, UINT *pNumSubresourceTilings, UINT FirstSubresourceTilingToGet, D3D12_SUBRESOURCE_TILING *pSubresourceTilingsForNonPackedMips) //42
{
	return oD3D12GetResourceTiling(dDevice, pTiledResource, pNumTilesForEntireResource, pPackedMipDesc, pStandardTileShapeForNonPackedMips,
		pNumSubresourceTilings, FirstSubresourceTilingToGet, pSubresourceTilingsForNonPackedMips);
}

DECLARE_FUNCTIONPTR(LUID, D3D12GetAdapterLuid, ID3D12Device *dDevice) //43
{
	return oD3D12GetAdapterLuid(dDevice);
}

void CreateHookD3D12DeviceInterface(uint64_t* methodVirtualTable)
{
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[0], D3D12DeviceQueryInterface);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[7], D3D12DeviceGetNodeCount);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[8], D3D12DeviceCreateCommandQueue);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[9], D3D12CreateCommandAllocator);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[10], D3D12CreateGraphicsPipelineState);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[11], D3D12CreateComputePipelineState);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[12], D3D12CreateCommandList);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[13], D3D12CheckFeatureSupport);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[14], D3D12CreateDescriptorHeap);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[15], D3D12GetDescriptorHandleIncrementSize);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[16], D3D12CreateRootSignature);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[17], D3D12CreateConstantBufferView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[18], D3D12CreateShaderResourceView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[19], D3D12CreateUnorderedAccessView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[20], D3D12CreateRenderTargetView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[21], D3D12CreateDepthStencilView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[22], D3D12CreateSampler);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[23], D3D12CopyDescriptors);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[27], D3D12CreateCommittedResource);

	MH_EnableHook((LPVOID)methodVirtualTable[0]);
	MH_EnableHook((LPVOID)methodVirtualTable[7]);
	MH_EnableHook((LPVOID)methodVirtualTable[8]);
	MH_EnableHook((LPVOID)methodVirtualTable[9]);
	MH_EnableHook((LPVOID)methodVirtualTable[10]);
	MH_EnableHook((LPVOID)methodVirtualTable[11]);
	MH_EnableHook((LPVOID)methodVirtualTable[12]);
	MH_EnableHook((LPVOID)methodVirtualTable[13]);
	MH_EnableHook((LPVOID)methodVirtualTable[14]);
	MH_EnableHook((LPVOID)methodVirtualTable[15]);
	MH_EnableHook((LPVOID)methodVirtualTable[16]);
	MH_EnableHook((LPVOID)methodVirtualTable[17]);
	MH_EnableHook((LPVOID)methodVirtualTable[18]);
	MH_EnableHook((LPVOID)methodVirtualTable[19]);
	MH_EnableHook((LPVOID)methodVirtualTable[20]);
	MH_EnableHook((LPVOID)methodVirtualTable[21]);
	MH_EnableHook((LPVOID)methodVirtualTable[22]);
	MH_EnableHook((LPVOID)methodVirtualTable[23]);
	MH_EnableHook((LPVOID)methodVirtualTable[27]);
}