#pragma once
#include "Common.h"
////base bias  44 + 19 + 9 = 72
////total function: 60
/*
[72]  QueryInterface             0
[73]  AddRef                     1
[74]  Release                    2
[75]  GetPrivateData             3
[76]  SetPrivateData             4
[77]  SetPrivateDataInterface    5
[78]  SetName                    6
[79]  GetDevice                  7
[80]  GetType                    8
[81]  Close                      9
[82]  Reset                     10
[83]  ClearState                11
[84]  DrawInstanced             12
[85]  DrawIndexedInstanced      13
[86]  Dispatch                  14
[87]  CopyBufferRegion          15
[88]  CopyTextureRegion         16
[89]  CopyResource              17
[90]  CopyTiles                 18
[91]  ResolveSubresource        19
[92]  IASetPrimitiveTopology    20
[93]  RSSetViewports            21
[94]  RSSetScissorRects         22
[95]  OMSetBlendFactor          23
[96]  OMSetStencilRef           24
[97]  SetPipelineState          25
[98]  ResourceBarrier           26
[99]  ExecuteBundle             27
[100] SetDescriptorHeaps        28
[101] SetComputeRootSignature   29
[102] SetGraphicsRootSignature  30
[103] SetComputeRootDescriptorTable      31
[104] SetGraphicsRootDescriptorTable     32
[105] SetComputeRoot32BitConstant        33
[106] SetGraphicsRoot32BitConstant       34
[107] SetComputeRoot32BitConstants       35
[108] SetGraphicsRoot32BitConstants      36
[109] SetComputeRootConstantBufferView   37
[110] SetGraphicsRootConstantBufferView  38
[111] SetComputeRootShaderResourceView   39
[112] SetGraphicsRootShaderResourceView  40
[113] SetComputeRootUnorderedAccessView  41 
[114] SetGraphicsRootUnorderedAccessView 42
[115] IASetIndexBuffer                   43
[116] IASetVertexBuffers                 44
[117] SOSetTargets                       45
[118] OMSetRenderTargets                 46
[119] ClearDepthStencilView              47
[120] ClearRenderTargetView              48
[121] ClearUnorderedAccessViewUint       49
[122] ClearUnorderedAccessViewFloat      50
[123] DiscardResource                    51
[124] BeginQuery                         52
[125] EndQuery                           53
[126] ResolveQueryData                   54
[127] SetPredication                     55
[128] SetMarker                          56
[129] BeginEvent                         57
[130] EndEvent                           58
[131] ExecuteIndirect                    59
*/

DECLARE_FUNCTIONPTR(long, D3D12CommandListQueryInterface, ID3D12GraphicsCommandList *dCommandList, REFIID riid, void **ppvObject)
{
	FUNC_DEFINE
	return oD3D12CommandListQueryInterface(dCommandList, riid, ppvObject);
}

DECLARE_FUNCTIONPTR(long, D3D12CommandListGetPrivateData, ID3D12GraphicsCommandList *dCommandList, REFGUID guid, UINT *pDataSize, void *pData) //72 + 3
{
	auto res = oD3D12CommandListGetPrivateData(dCommandList, guid, pDataSize, pData);
	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_GetPrivateData);
	RecordEnd

	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CommandListSetPrivateData, ID3D12GraphicsCommandList *dCommandList, REFGUID guid, UINT DataSize, const void *pData) // 72 + 4
{
	auto res = oD3D12CommandListSetPrivateData(dCommandList, guid, DataSize, pData);
	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetPrivateData);
	RecordEnd

	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CommandListSetPrivateDataInterface, ID3D12GraphicsCommandList *dCommandList, REFGUID guid, const IUnknown *pData)   // 72 + 5
{
	auto res = oD3D12CommandListSetPrivateDataInterface(dCommandList, guid, pData);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetPrivateDataInterface);
	RecordEnd
	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CommandListSetName, ID3D12GraphicsCommandList *dCommandList, LPCWSTR name)   // 72 + 6
{
	auto res = oD3D12CommandListSetName(dCommandList, name);
	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetName);
	RecordEnd
	return res;
}


DECLARE_FUNCTIONPTR(long, D3D12CommandListGetDevice, ID3D12GraphicsCommandList *dCommandList, REFIID riid, void **ppvDevice) // 72 + 7
{
	auto res = oD3D12CommandListGetDevice(dCommandList, riid, ppvDevice);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_GetDevice);
	RecordEnd

	return res;
}

DECLARE_FUNCTIONPTR(D3D12_COMMAND_LIST_TYPE, D3D12CommandListGetType, ID3D12GraphicsCommandList *dCommandList) // 72 + 8
{
	auto res = oD3D12CommandListGetType(dCommandList);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_GetType);
	RecordEnd

	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CommandListClose, ID3D12GraphicsCommandList *dCommandList) // 72 + 9
{
	auto res = oD3D12CommandListClose(dCommandList);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_Close);
	RecordEnd

	return res;
}

DECLARE_FUNCTIONPTR(long, D3D12CommandListReset, ID3D12GraphicsCommandList *dCommandList) // 72 + 10
{
	auto res = oD3D12CommandListReset(dCommandList);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_Reset);
	RecordEnd

	return res;
}

DECLARE_FUNCTIONPTR(void, D3D12CommandListClearState, ID3D12GraphicsCommandList *dCommandList, ID3D12PipelineState *pPipelineState) // 72 + 11
{
	oD3D12CommandListClearState(dCommandList, pPipelineState);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_ClearState);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12DrawInstanced, ID3D12GraphicsCommandList *dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) // 72 + 12
{
	FUNC_DEFINE
// 	stringstream ss;
// 	ss << GetCurrentThreadId();
// 	Log(ss.str() + " [d3d12] D3D12DrawInstanced");
 	oD3D12DrawInstanced(dCommandList, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_DrawInstanced);
	streamInstance->write(dCommandList);
	streamInstance->write(VertexCountPerInstance);
	streamInstance->write(InstanceCount);
	streamInstance->write(StartVertexLocation);
	streamInstance->write(StartInstanceLocation);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12DrawIndexedInstanced, ID3D12GraphicsCommandList *dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex) // 72 + 13
{
// 	ss << GetCurrentThreadId();
// 	Log(ss.str() + " [d3d12] D3D12DrawIndexedInstanced");
	oD3D12DrawIndexedInstanced(dCommandList, IndexCount, InstanceCount, StartIndex, BaseVertex);
	RecordStart

	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandList_DrawIndexedInstanced);
	streamInstance->write(dCommandList);
	streamInstance->write(IndexCount);
	streamInstance->write(InstanceCount);
	streamInstance->write(StartIndex);
	streamInstance->write(BaseVertex);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12Dispatch, ID3D12GraphicsCommandList *dCommandList, UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ)  // 72 + 14
{
	oD3D12Dispatch(dCommandList, ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_Dispatch);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CopyBufferRegion, ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pDstBuffer, UINT64 DstOffset, ID3D12Resource *pSrcBuffer, UINT64 SrcOffset, UINT64 NumBytes) // 72 + 15
{
	FUNC_DEFINE
	oD3D12CopyBufferRegion(dCommandList, pDstBuffer, DstOffset, pSrcBuffer, SrcOffset, NumBytes);
	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_CopyBufferRegion);
	streamInstance->write(dCommandList);
	streamInstance->write(pDstBuffer);
	streamInstance->write(DstOffset);
	streamInstance->write(pSrcBuffer);
	streamInstance->write(SrcOffset);
	streamInstance->write(NumBytes);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12CopyTextureRegion, ID3D12GraphicsCommandList *dCommandList, const D3D12_TEXTURE_COPY_LOCATION *pDst, UINT DstX, UINT DstY, UINT DstZ, const D3D12_TEXTURE_COPY_LOCATION *pSrc, const D3D12_BOX *pSrcBox) // 72 + 16
{
	FUNC_DEFINE
	oD3D12CopyTextureRegion(dCommandList, pDst, DstX, DstY, DstZ, pSrc, pSrcBox);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_CopyTextureRegion);
	streamInstance->write(dCommandList);
	streamInstance->writePointerValue(pDst);
	streamInstance->write(DstX);
	streamInstance->write(DstY);
	streamInstance->write(DstZ);
	streamInstance->writePointerValue(pSrc);
	streamInstance->writePointerValue(pSrcBox);
	//streamInstance->write()
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12CopyResource, ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pDstResource, ID3D12Resource *pSrcResource) // 72 + 17
{
	oD3D12CopyResource(dCommandList, pDstResource, pSrcResource);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_CopyResource);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CopyTiles, ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pTiledResource, const D3D12_TILED_RESOURCE_COORDINATE *pTileRegionStartCoordinate,
					const D3D12_TILE_REGION_SIZE *pTileRegionSize, ID3D12Resource *pBuffer, UINT64 BufferStartOffsetInBytes, D3D12_TILE_COPY_FLAGS Flags) // 72 + 18
{
	oD3D12CopyTiles(dCommandList, pTiledResource, pTileRegionStartCoordinate, pTileRegionSize, pBuffer, BufferStartOffsetInBytes, Flags);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_CopyTiles);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12ResolveSubresource, ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pDstResource, UINT DstSubresource, ID3D12Resource *pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format)  // 72 + 19
{
	oD3D12ResolveSubresource(dCommandList, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_ResolveSubresource);
	RecordEnd


	return;
}

DECLARE_FUNCTIONPTR(void, D3D12IASetPrimitiveTopology, ID3D12GraphicsCommandList *dCommandList, D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology)  // 72 + 20
{
	FUNC_DEFINE
	oD3D12IASetPrimitiveTopology(dCommandList, PrimitiveTopology);
	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(dCommandList);
	streamInstance->write(PrimitiveTopology);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12RSSetViewports, ID3D12GraphicsCommandList *dCommandList, UINT NumViewports, const D3D12_VIEWPORT *pViewports) // // 72 + 21
{
	oD3D12RSSetViewports(dCommandList, NumViewports, pViewports);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_RSSetViewports);
	RecordEnd


	return;
}

DECLARE_FUNCTIONPTR(void, D3D12RSSetScissorRects, ID3D12GraphicsCommandList *dCommandList, UINT NumRects, const D3D12_RECT *pRects) // 72 + 22
{
	oD3D12RSSetScissorRects(dCommandList, NumRects, pRects);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_RSSetScissorRects);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12OMSetBlendFactor, ID3D12GraphicsCommandList *dCommandList, const FLOAT BlendFactor[4]) // 72 + 23
{
	 oD3D12OMSetBlendFactor(dCommandList, BlendFactor);

	 RecordStart
	 MemStream *streamInstance = GetStreamFromThreadID();
	 streamInstance->write(CommandEnum::CommandList_OMSetBlendFactor);
	 RecordEnd

	 return;
}

DECLARE_FUNCTIONPTR(void, D3D12OMSetStencilRef, ID3D12GraphicsCommandList *dCommandList, UINT StencilRef) //72 + 24
{
	oD3D12OMSetStencilRef(dCommandList, StencilRef);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_OMSetStencilRef);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetPipelineState, ID3D12GraphicsCommandList *dCommandList, ID3D12PipelineState *pPipelineState) //72 + 25
{
	FUNC_DEFINE
	oD3D12SetPipelineState(dCommandList, pPipelineState);
	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandList_SetPipelineState);
	streamInstance->write(dCommandList);
	streamInstance->write(pPipelineState);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12ResourceBarrier, ID3D12GraphicsCommandList *dCommandList, UINT NumBarriers, const D3D12_RESOURCE_BARRIER *pBarriers)  //72 + 26
{
	oD3D12ResourceBarrier(dCommandList, NumBarriers, pBarriers);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_ResourceBarrier);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12ExecuteBundle, ID3D12GraphicsCommandList *dCommandList, ID3D12GraphicsCommandList *pCommandList)  //72 + 27
{
	oD3D12ExecuteBundle(dCommandList, pCommandList);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_ExecuteBundle);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetDescriptorHeaps, ID3D12GraphicsCommandList *dCommandList, UINT NumDescriptorHeaps, ID3D12DescriptorHeap *const *ppDescriptorHeaps)  //72 + 28
{
	FUNC_DEFINE
	oD3D12SetDescriptorHeaps(dCommandList, NumDescriptorHeaps, ppDescriptorHeaps);
	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetDescriptorHeaps);
	streamInstance->write(dCommandList);
	streamInstance->write(NumDescriptorHeaps);
	streamInstance->write(ppDescriptorHeaps);
	RecordEnd
	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetComputeRootSignature, ID3D12GraphicsCommandList *dCommandList, ID3D12RootSignature *pRootSignature)  //72 + 29
{
	oD3D12SetComputeRootSignature(dCommandList, pRootSignature);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetComputeRootSignature);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetGraphicsRootSignature, ID3D12GraphicsCommandList *dCommandList, ID3D12RootSignature *pRootSignature) //72 + 30
{
	FUNC_DEFINE
	//Log("[d3d12] D3D12SetGraphicsRootSignature");
	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(CommandList_SetGraphicsRootSignature);
	streaminstance->write(dCommandList);
	streaminstance->write(pRootSignature);
	RecordEnd
    oD3D12SetGraphicsRootSignature(dCommandList, pRootSignature);


}

DECLARE_FUNCTIONPTR(void, D3D12SetComputeRootDescriptorTable, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) //72 + 31
{
	oD3D12SetComputeRootDescriptorTable(dCommandList, RootParameterIndex, BaseDescriptor);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetComputeRootDescriptorTable);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetGraphicsRootDescriptorTable, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) //72 + 32
{
	FUNC_DEFINE
	oD3D12SetGraphicsRootDescriptorTable(dCommandList, RootParameterIndex, BaseDescriptor);
	RecordStart
	MemStream* instance = GetStreamFromThreadID();
	instance->write(CommandList_SetGraphicsRootDescriptorTable);
	instance->write(dCommandList);
	instance->write(RootParameterIndex);
	instance->write(BaseDescriptor);
	RecordEnd

}

DECLARE_FUNCTIONPTR(void, D3D12SetComputeRoot32BitConstant, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues) //72 + 33
{
	oD3D12SetComputeRoot32BitConstant(dCommandList, RootParameterIndex, SrcData, DestOffsetIn32BitValues);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetComputeRoot32BitConstant);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetGraphicsRoot32BitConstant, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)//72 + 34
{
	FUNC_DEFINE
	oD3D12SetGraphicsRoot32BitConstant(dCommandList, RootParameterIndex, SrcData, DestOffsetIn32BitValues);

	RecordStart
	MemStream* instance = GetStreamFromThreadID();
	instance->write(CommandList_SetGraphicsRoot32BitConstant);
	instance->write(dCommandList);
	instance->write(RootParameterIndex);
	instance->write(SrcData);
	instance->write(DestOffsetIn32BitValues);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12SetComputeRoot32BitConstants, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, 
                   UINT Num32BitValuesToSet, const void *pSrcData, UINT DestOffsetIn32BitValues) //72 + 35
{
	oD3D12SetComputeRoot32BitConstants(dCommandList, RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetComputeRoot32BitConstants);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetGraphicsRoot32BitConstants, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, UINT Num32BitValuesToSet, 
					const void *pSrcData, UINT DestOffsetIn32BitValues) //72 + 36
{
	oD3D12SetGraphicsRoot32BitConstants(dCommandList, RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetGraphicsRoot32BitConstants);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetComputeRootConstantBufferView, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) //72 + 37
{
	oD3D12SetComputeRootConstantBufferView(dCommandList, RootParameterIndex, BufferLocation);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetComputeRootConstantBufferView);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetGraphicsRootConstantBufferView, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) //72 + 38
{
	oD3D12SetGraphicsRootConstantBufferView(dCommandList, RootParameterIndex, BufferLocation);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetGraphicsRootConstantBufferView);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetComputeRootShaderResourceView, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) //72 + 39
{
	oD3D12SetComputeRootShaderResourceView(dCommandList, RootParameterIndex, BufferLocation);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetComputeRootShaderResourceView);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetGraphicsRootShaderResourceView, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) //72 + 40
{
	oD3D12SetGraphicsRootShaderResourceView(dCommandList, RootParameterIndex, BufferLocation);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetGraphicsRootShaderResourceView);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetComputeRootUnorderedAccessView, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) //72 + 41
{
	oD3D12SetComputeRootUnorderedAccessView(dCommandList, RootParameterIndex, BufferLocation);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetComputeRootUnorderedAccessView);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12SetGraphicsRootUnorderedAccessView, ID3D12GraphicsCommandList *dCommandList, UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) //72 + 42
{
	oD3D12SetGraphicsRootUnorderedAccessView(dCommandList, RootParameterIndex, BufferLocation);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetGraphicsRootUnorderedAccessView);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12IASetIndexBuffer, ID3D12GraphicsCommandList *dCommandList, const D3D12_INDEX_BUFFER_VIEW *pView) //72 + 43
{
	FUNC_DEFINE
	LOG_ONCE("[d3d12] D3D12IASetIndexBuffer");
	oD3D12IASetIndexBuffer(dCommandList, pView);
	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(CommandList_IASetIndexBuffer);
	streaminstance->write(dCommandList);
	streaminstance->writePointerValue(pView);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12IASetVertexBuffers, ID3D12GraphicsCommandList *dCommandList, UINT StartSlot, UINT NumViews, const D3D12_VERTEX_BUFFER_VIEW *pViews) //72 + 44
{
	FUNC_DEFINE
	LOG_ONCE("[d3d12] D3D12IASetVertexBuffers");
	oD3D12IASetVertexBuffers(dCommandList, StartSlot, NumViews, pViews);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(CommandList_IASetVertexBuffers);
	streaminstance->write(dCommandList);
	streaminstance->write(StartSlot);
	streaminstance->write(NumViews);
	streaminstance->writePointerValue(pViews);
	RecordEnd
}

DECLARE_FUNCTIONPTR(void, D3D12SOSetTargets, ID3D12GraphicsCommandList *dCommandList, UINT StartSlot, UINT NumViews, const D3D12_STREAM_OUTPUT_BUFFER_VIEW *pViews) //72 + 45
{
	oD3D12SOSetTargets(dCommandList, StartSlot, NumViews, pViews);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SOSetTargets);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12OMSetRenderTargets, ID3D12GraphicsCommandList *dCommandList, UINT NumRenderTargetDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE *pRenderTargetDescriptors,
BOOL RTsSingleHandleToDescriptorRange, const D3D12_CPU_DESCRIPTOR_HANDLE *pDepthStencilDescriptor)  //72 + 46
{
	oD3D12OMSetRenderTargets(dCommandList, NumRenderTargetDescriptors, pRenderTargetDescriptors, RTsSingleHandleToDescriptorRange, pDepthStencilDescriptor);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_OMSetRenderTargets);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12ClearDepthStencilView, ID3D12GraphicsCommandList *dCommandList, D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView, D3D12_CLEAR_FLAGS ClearFlags, 
FLOAT Depth, UINT8 Stencil, UINT NumRects, const D3D12_RECT *pRects)  //72 + 47
{
	oD3D12ClearDepthStencilView(dCommandList, DepthStencilView, ClearFlags, Depth, Stencil, NumRects, pRects);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_ClearDepthStencilView);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12ClearRenderTargetView, ID3D12GraphicsCommandList *dCommandList, D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView, 
					const FLOAT ColorRGBA[4], UINT NumRects, const D3D12_RECT *pRects)  //72 + 48
{
	oD3D12ClearRenderTargetView(dCommandList, RenderTargetView, ColorRGBA, NumRects, pRects);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_ClearRenderTargetView);
	RecordEnd


	return;
}

DECLARE_FUNCTIONPTR(void, D3D12ClearUnorderedAccessViewUint, ID3D12GraphicsCommandList *dCommandList, D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
					D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle, ID3D12Resource *pResource, const UINT Values[4], UINT NumRects, const D3D12_RECT *pRects) //72 + 49
{
	oD3D12ClearUnorderedAccessViewUint(dCommandList, ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource, Values, NumRects, pRects);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_ClearUnorderedAccessViewUint);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12ClearUnorderedAccessViewFloat, ID3D12GraphicsCommandList *dCommandList, D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle, ID3D12Resource *pResource, const FLOAT Values[4], UINT NumRects, const D3D12_RECT *pRects) //72 + 50
{
	oD3D12ClearUnorderedAccessViewFloat(dCommandList, ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource, Values, NumRects, pRects);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_ClearUnorderedAccessViewFloat);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12DiscardResource, ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pResource, const D3D12_DISCARD_REGION *pRegion) //72 + 51
{
	oD3D12DiscardResource(dCommandList, pResource, pRegion);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_DiscardResource);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CommandListBeginQuery, ID3D12GraphicsCommandList *dCommandList, ID3D12QueryHeap *pQueryHeap, D3D12_QUERY_TYPE Type, UINT Index)  //72 + 52
{
	oD3D12CommandListBeginQuery(dCommandList, pQueryHeap, Type, Index);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_BeginQuery);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CommandListEndQuery, ID3D12GraphicsCommandList *dCommandList, ID3D12QueryHeap *pQueryHeap, D3D12_QUERY_TYPE Type, UINT Index) //72 + 53
{
	oD3D12CommandListEndQuery(dCommandList, pQueryHeap, Type, Index);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_EndQuery);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CommandListResolveQueryData, ID3D12GraphicsCommandList *dCommandList, ID3D12QueryHeap *pQueryHeap, D3D12_QUERY_TYPE Type, UINT StartIndex, 
UINT NumQueries, ID3D12Resource *pDestinationBuffer, UINT64 AlignedDestinationBufferOffset)  //72 + 54
{
	 oD3D12CommandListResolveQueryData(dCommandList, pQueryHeap, Type, StartIndex, NumQueries, pDestinationBuffer, AlignedDestinationBufferOffset);

	 RecordStart
	 MemStream *streamInstance = GetStreamFromThreadID();
	 streamInstance->write(CommandEnum::CommandList_ResolveQueryData);
	 RecordEnd

	 return;
}

DECLARE_FUNCTIONPTR(void, D3D12CommandListSetPredication, ID3D12GraphicsCommandList *dCommandList, ID3D12Resource *pBuffer, UINT64 AlignedBufferOffset, D3D12_PREDICATION_OP Operation)  //72 + 55
{
	oD3D12CommandListSetPredication(dCommandList, pBuffer, AlignedBufferOffset, Operation);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetPredication);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CommandListSetMarker, ID3D12GraphicsCommandList *dCommandList, UINT Metadata, const void *pData, UINT Size)   //72 + 56
{
	oD3D12CommandListSetMarker(dCommandList, Metadata, pData, Size);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_SetMarker);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CommandListBeginEvent, ID3D12GraphicsCommandList *dCommandList, UINT Metadata, const void *pData, UINT Size)   //72 + 57
{
	oD3D12CommandListBeginEvent(dCommandList, Metadata, pData, Size);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_BeginEvent);
	RecordEnd


	return;
}

DECLARE_FUNCTIONPTR(void, D3D12CommandListEndEvent, ID3D12GraphicsCommandList *dCommandList)   //72 + 58
{
	oD3D12CommandListEndEvent(dCommandList);

	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_EndEvent);
	RecordEnd

	return;
}

DECLARE_FUNCTIONPTR(void, D3D12ExecuteIndirect, ID3D12GraphicsCommandList *dCommandList, ID3D12CommandSignature *pCommandSignature, UINT MaxCommandCount, 
ID3D12Resource *pArgumentBuffer, UINT64 ArgumentBufferOffset, ID3D12Resource *pCountBuffer, UINT64 CountBufferOffset)  //72 + 59
{
// 	stringstream ss;
// 	ss << GetCurrentThreadId();
// 
// 	Log(ss.str() + " [d3d12] D3D12ExecuteIndirect");
	oD3D12ExecuteIndirect(dCommandList, pCommandSignature, MaxCommandCount, pArgumentBuffer, ArgumentBufferOffset, pCountBuffer, CountBufferOffset);


	RecordStart
	MemStream *streamInstance = GetStreamFromThreadID();
	streamInstance->write(CommandEnum::CommandList_ExecuteIndirect);
	RecordEnd

	return;
}


void CreateHookD3D12CommandListInterface(uint64_t* methodVirtualTable)
{

	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 +  8], D3D12CommandListGetType);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 +  9], D3D12CommandListClose);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 10], D3D12CommandListReset);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 11], D3D12CommandListClearState);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 12], D3D12DrawInstanced);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 13], D3D12DrawIndexedInstanced);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 14], D3D12Dispatch);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 15], D3D12CopyBufferRegion);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 16], D3D12CopyTextureRegion);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 17], D3D12CopyResource);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 18], D3D12CopyTiles);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 19], D3D12ResolveSubresource);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 20], D3D12IASetPrimitiveTopology);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 21], D3D12RSSetViewports);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 22], D3D12RSSetScissorRects);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 23], D3D12OMSetBlendFactor);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 24], D3D12OMSetStencilRef);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 25], D3D12SetPipelineState);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 26], D3D12ResourceBarrier);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 27], D3D12ExecuteBundle);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 28], D3D12SetDescriptorHeaps);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 29], D3D12SetComputeRootSignature);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 30], D3D12SetGraphicsRootSignature);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 31], D3D12SetComputeRootDescriptorTable);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 32], D3D12SetGraphicsRootDescriptorTable);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 33], D3D12SetComputeRoot32BitConstant);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 34], D3D12SetGraphicsRoot32BitConstant);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 35], D3D12SetComputeRoot32BitConstants);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 36], D3D12SetGraphicsRoot32BitConstants);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 37], D3D12SetComputeRootConstantBufferView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 38], D3D12SetGraphicsRootConstantBufferView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 39], D3D12SetComputeRootShaderResourceView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 40], D3D12SetGraphicsRootShaderResourceView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 41], D3D12SetComputeRootUnorderedAccessView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 42], D3D12SetGraphicsRootUnorderedAccessView);

	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 43], D3D12IASetIndexBuffer);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 44], D3D12IASetVertexBuffers);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 45], D3D12SOSetTargets);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 46], D3D12OMSetRenderTargets);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 47], D3D12ClearDepthStencilView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 48], D3D12ClearRenderTargetView);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 49], D3D12ClearUnorderedAccessViewUint);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 50], D3D12ClearUnorderedAccessViewFloat);

	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[72 + 59], D3D12ExecuteIndirect);


	MH_EnableHook((LPVOID)methodVirtualTable[72 + 8]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 9]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 10]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 11]);
 	MH_EnableHook((LPVOID)methodVirtualTable[72 + 12]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 13]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 14]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 15]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 16]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 17]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 18]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 19]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 20]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 21]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 22]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 23]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 24]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 25]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 26]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 27]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 28]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 29]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 30]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 31]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 32]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 33]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 34]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 35]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 36]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 37]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 38]); 
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 39]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 40]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 41]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 42]);

	MH_EnableHook((LPVOID)methodVirtualTable[72 + 43]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 44]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 45]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 46]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 47]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 48]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 49]);
	MH_EnableHook((LPVOID)methodVirtualTable[72 + 50]);

	MH_EnableHook((LPVOID)methodVirtualTable[72 + 59]);
}
