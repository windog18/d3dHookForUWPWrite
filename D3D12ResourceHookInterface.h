#pragma once
#include "Common.h"
#include "memstream.h"
////base bias 44 + 19 + 9 + 60 + 18 = 150
////total function: 15

////QueryInterface 0
////AddRef         1
////Release        2
////GetPrivateData 3
////SetPrivateData 4
////SetPrivateDataInterface 5
////SetName                 6
////GetDevice               7
////Map                     8
////Unmap                   9
////GetDesc                10
////GetGPUVirtualAddress   11
////WriteToSubresource     12
////ReadFromSubresource    13
////GetHeapProperties      14
////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////D3DRESOUCRES hook interface//




DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceQueryInterface, ID3D12Resource * dResource, REFIID riid, void **ppvObject) //0
{
	//Log("[D3D12]D3D12Resouces QueryInterface ");
	return oD3D12ResourceQueryInterface(dResource, riid, ppvObject);
}



DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceSetPrivateData, ID3D12Resource *dResource, REFGUID guid, UINT DataSize, const void *pData) //4
{
	return oD3D12ResourceSetPrivateData(dResource, guid, DataSize, pData);
}

DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceSetPrivateDataInterface, ID3D12Resource *dResource, REFGUID guid, const IUnknown *pData) //5
{
	return oD3D12ResourceSetPrivateDataInterface(dResource, guid, pData);
}

DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceSetName, ID3D12Resource *dResource, LPCWSTR Name) //6
{
	return oD3D12ResourceSetName(dResource, Name);
}

DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceGetDevice, ID3D12Resource *dResource, REFIID riid, void **ppvDevice) //7 
{
	return oD3D12ResourceGetDevice(dResource, riid, ppvDevice);
}

DECLARE_FUNCTIONPTR(HRESULT, D3D12ResourceMap, ID3D12Resource *dResource, UINT subresource, const D3D12_RANGE *pReadRange, void **ppData) //8
{
	//Log("[D3D12]D3D12Resources Map Resources");
	HRESULT result = oD3D12ResourceMap(dResource, subresource, pReadRange, ppData);
	ResourceTempData::SetTempMapData(GetCurrentThreadId(), *ppData);
	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();
	streaminstance->write(Resource_Map);
	streaminstance->write(dResource);
	streaminstance->write(subresource);
	streaminstance->writePointerValue(pReadRange);
	RecordEnd
	return result;
}

DECLARE_FUNCTIONPTR(void, D3D12ResourceUnmap, ID3D12Resource *dResource, UINT subresource, const D3D12_RANGE *pWrittenRange) //9
{
	//Log("[D3D12]D3D12Resources Unmap Resources");
	oD3D12ResourceUnmap(dResource, subresource, pWrittenRange);

	RecordStart
	MemStream* streaminstance = GetStreamFromThreadID();

	D3D12_RESOURCE_DESC desc = dResource->GetDesc();

	streaminstance->write(Resource_Unmap);
	streaminstance->write(dResource);
	streaminstance->write(subresource);
	streaminstance->writePointerValue(pWrittenRange);


	//write the real data
	streaminstance->write(desc.Dimension);
	if (desc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
	{
		size_t buffersize = desc.Width;
		streaminstance->write(buffersize);
		streaminstance->write(ResourceTempData::GetTempMapData(GetCurrentThreadId()), buffersize);
	}
	else if (desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)
	{
		size_t texsize = desc.Width;
		//size_t texsize = desc.Width*desc.Height * 4;
		//streaminstance->write(pTemp, texsize);
	}
	RecordEnd
}

DECLARE_FUNCTIONPTR(D3D12_RESOURCE_DESC, D3D12ResourceGetDesc, ID3D12Resource *dResource) //10
{
	//Log("[D3D12]D3D12Resources GetDesc");
	return oD3D12ResourceGetDesc(dResource);
}

DECLARE_FUNCTIONPTR(D3D12_GPU_VIRTUAL_ADDRESS,D3D12GetGPUVirtualAddress, ID3D12Resource * dResource) //11
{
	return oD3D12GetGPUVirtualAddress(dResource);
}

DECLARE_FUNCTIONPTR(long, D3D12WriteToSubresource, ID3D12Resource *dResource, UINT DstSubresource, const D3D12_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)  //12
{
	return oD3D12WriteToSubresource(dResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

DECLARE_FUNCTIONPTR(long, D3D12ReadFromSubresource, ID3D12Resource *dResource, void *pDstData, UINT DstRowPitch, UINT DstDepthPitch, UINT SrcSubresource, const D3D12_BOX *pSrcBox) //13
{
	return oD3D12ReadFromSubresource(dResource, pDstData, DstRowPitch, DstDepthPitch, SrcSubresource, pSrcBox);
}

DECLARE_FUNCTIONPTR(long, D3D12GetHeapProperties, ID3D12Resource *dResource, D3D12_HEAP_PROPERTIES *pHeapProperties, D3D12_HEAP_FLAGS *pHeapFlags) //14
{
	return oD3D12GetHeapProperties(dResource, pHeapProperties, pHeapFlags);
}

void CreateHookD3D12ResourceInterface(uint64_t* methodVirtualTable)
{
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 0], D3D12ResourceQueryInterface);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 4], D3D12ResourceSetPrivateData);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 5], D3D12ResourceSetPrivateDataInterface);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 6], D3D12ResourceSetName);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 7], D3D12ResourceGetDevice);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 8], D3D12ResourceMap);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 9], D3D12ResourceUnmap);
	//CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 10], D3D12ResourceGetDesc); ??? will crash 
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 11], D3D12GetGPUVirtualAddress);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 12], D3D12WriteToSubresource);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 13], D3D12ReadFromSubresource);
	CREATE_HOOKPAIR((LPVOID)methodVirtualTable[150 + 14], D3D12GetHeapProperties);


	MH_EnableHook((LPVOID)methodVirtualTable[150 +  0]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  4]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  5]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  6]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  7]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  8]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 +  9]);
	//MH_EnableHook((LPVOID)methodVirtualTable[150 + 10]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 + 11]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 + 12]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 + 13]);
	MH_EnableHook((LPVOID)methodVirtualTable[150 + 14]);

// 	MH_CreateHook((LPVOID)methodVirtualTable[150 + 0], hkD3D12ResourceQueryInterface, (LPVOID*)&oD3D12ResourceQueryInterface);
// 	MH_CreateHook((LPVOID)methodVirtualTable[150 + 8], hkD3D12ResourceUnmap, (LPVOID*)&oD3D12ResourceUnmap);
// 
// 	MH_EnableHook((LPVOID)methodVirtualTable[150 + 0]);
}