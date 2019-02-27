#pragma once


enum CommandEnum
{
	Device_CreateCommandQueue = 0,
	Device_CreateDescriptorHeap,
	Device_CreateCommandAllocator,
	Device_CreateRootSignature,
	Device_CreateGraphicsPipelineState,
	Device_CreateCommandList,
	Device_CreateRenderTargetView,
	Device_CreateCommittedResource,
	Device_CreateSampler,
	Device_CreateShaderResourceView,
	Device_CreateDepthStencilView,
	Device_CreateFence,
	Device_CreateConstantBufferView,
	CommandList_CopyBufferRegion,
	CommandList_ResourceBarrier,
	CommandList_CopyTextureRegion,
	CommandList_Close, 
	CommandList_SetGraphicsRootSignature,
	CommandList_SetDescriptorHeaps,
	CommandList_IASetPrimitiveTopology,
	CommandList_IASetIndexBuffer,
	CommandList_IASetVertexBuffers,
	CommandList_SetGraphicsRootDescriptorTable,
	CommandList_BeginEvent,
	CommandList_SetPipelineState,
	CommandList_SetGraphicsRoot32BitConstant,
	CommandList_DrawInstanced,
	CommandList_DrawIndexedInstanced,
	CommandList_EndEvent,
	CommandList_Reset,
	CommandList_RSSetViewports,
	CommandList_RSSetScissorRects,
	CommandList_OMSetRenderTargets,
	CommandList_ClearRenderTargetView,
	CommandList_ClearDepthStencilView,
	CommandList_ExecuteBundle,
	CommandQueue_EndEvent,
	CommandQueue_Signal,
	CommandQueue_BeginEvent,
	Resource_GetDesc,
	Resource_Map,
	Resource_Unmap,
	Resource_GetGPUVirtualAddress,
	end_frame,

	forcedw = 0xffffffff
};


inline const char* enum_to_string(CommandEnum t) {
	switch (t) {
	case Device_CreateCommandQueue:
		return "Device_CreateCommandQueue";
	case Device_CreateDescriptorHeap:
		return "Device_CreateDescriptorHeap";
	case Device_CreateCommandAllocator:
		return "Device_CreateCommandAllocator";
	case Device_CreateRootSignature:
		return "Device_CreateRootSignature";
	case Device_CreateGraphicsPipelineState:
		return "Device_CreateGraphicsPipelineState";
	case Device_CreateCommandList:
		return "Device_CreateCommandList";
	case Device_CreateRenderTargetView:
		return "Device_CreateRenderTargetView";
	case Device_CreateCommittedResource:
		return "Device_CreateCommittedResource";
	case Device_CreateSampler:
		return "Device_CreateSampler";
	case Device_CreateShaderResourceView:
		return "Device_CreateShaderResourceView";
	case Device_CreateFence:
		return "Device_CreateFence";
	case CommandList_CopyBufferRegion:
		return "CommandList_CopyBufferRegion";
	case CommandList_ResourceBarrier:
		return "CommandList_ResourceBarrier";
	case CommandList_CopyTextureRegion:
		return "CommandList_CopyTextureRegion";
	case CommandList_Close:
		return "CommandList_Close";
	case CommandList_SetGraphicsRootSignature:
		return "CommandList_SetGraphicsRootSignature";
	case CommandList_SetDescriptorHeaps:
		return "CommandList_SetDescriptorHeaps";
	case CommandList_IASetPrimitiveTopology:
		return "CommandList_IASetPrimitiveTopology";
	case CommandList_IASetIndexBuffer:
		return "CommandList_IASetIndexBuffer";
	case CommandList_IASetVertexBuffers:
		return "CommandList_IASetVertexBuffers";
	case CommandList_SetGraphicsRootDescriptorTable:
		return "CommandList_SetGraphicsRootDescriptorTable";
	case CommandList_BeginEvent:
		return "CommandList_BeginEvent";
	case CommandList_SetPipelineState:
		return "CommandList_SetPipelineState";
	case CommandList_SetGraphicsRoot32BitConstant:
		return "CommandList_SetGraphicsRoot32BitConstant";
	case CommandList_DrawInstanced:
		return "CommandList_DrawInstanced";
	case CommandList_DrawIndexedInstanced:
		return "CommandList_DrawIndexedInstanced";
	case CommandList_EndEvent:
		return "CommandList_EndEvent";
	case CommandList_Reset:
		return "CommandList_Reset";
	case CommandList_RSSetViewports:
		return "CommandList_RSSetViewports";
	case CommandList_RSSetScissorRects:
		return "CommandList_RSSetScissorRects";
	case CommandList_OMSetRenderTargets:
		return "CommandList_OMSetRenderTargets";
	case CommandList_ClearRenderTargetView:
		return "CommandList_ClearRenderTargetView";
	case CommandList_ClearDepthStencilView:
		return "CommandList_ClearDepthStencilView";
	case CommandList_ExecuteBundle:
		return "CommandList_ExecuteBundle";
	case CommandQueue_EndEvent:
		return "CommandQueue_EndEvent";
	case CommandQueue_Signal:
		return "CommandQueue_Signal";
	case CommandQueue_BeginEvent:
		return "CommandQueue_BeginEvent";
	case Resource_GetDesc:
		return "Resource_GetDesc";
	case Resource_Map:
		return "Resource_Map";
	case Resource_Unmap:
		return "Resource_Unmap";
	case Resource_GetGPUVirtualAddress:
		return "Resource_GetGPUVirtualAddress";
	case end_frame:
		return "end_frame";
	default:
		return "INVALID ENUM";
	}
}