
#include "memstream.h"



const int MaxDatasize = 10 * 1024 * 1024;

const int MaxCallSequenceSize = 1024 * 1024 * 1024;


// void SaveRecordToFile(const char* name)
// {
// 	MemStream::getstream()->writetoFile(name);
// }
// 
// void ResetStream()
// {
// 	MemStream::getstream()->reset();
// }



void MemStream::writetoFile(const char* pname)
{
	FILE *f;
	fopen_s(&f, pname, "ab");

	fwrite(&streamcount, sizeof(INT64), 1, f);
	fwrite(memhandle, streamcount, 1, f);

	fclose(f);

}

void MemStream::writeCommandNameToFile(const char * pname)
{

	ofstream fout(pname,ios::app);
	fout << nameListCache.str();
}

void MemStream::readfromfile(const char* pname)
{
	reset();

	FILE *f;
	fopen_s(&f, pname, "rb");

	INT64 memcount = 0;
	fread(&memcount, sizeof(INT64), 1, f);
	fread(memhandle, memcount, 1, f);

	fclose(f);
}

void MemStream::reset()
{
	streamcount = 0;
	streamhandle = memhandle;

	nameListCache.clear();
}

MemStream::MemStream()
{
	memhandle = NULL;

}

MemStream::~MemStream()
{
	if (memhandle != NULL) {
		delete []memhandle;
		memhandle = NULL;
	}

}

void MemStream::init()
{

	memhandle = new unsigned char[MaxDatasize];
	streamcount = 0;
	streamhandle = memhandle;

	nameListCache.clear();
}




void MemStream::write(const void* pdata, size_t datasize)
{
	memcpy((void*)streamhandle, pdata, datasize);

	streamhandle = (unsigned char*)streamhandle + datasize;

	streamcount += datasize;
}


void MemStream::write(CommandEnum enu)
{

	memcpy(streamhandle, &enu, sizeof(CommandEnum));
	streamcount += sizeof(CommandEnum);
	streamhandle = (unsigned char*)streamhandle + sizeof(CommandEnum);

	std::string commandName = enum_to_string(enu);
	commandName = commandName + "\n";
	
	nameListCache << commandName;
}





void MemStream::read(const void* pdata, size_t datasize)
{

	memcpy((void*)pdata,(void*)streamhandle, datasize);

	streamhandle = (unsigned char*)streamhandle + datasize;

	streamcount += datasize;
}

void MemStream::write(const D3D12_INPUT_LAYOUT_DESC& desc)
{

	write(desc.NumElements);

	for (int i = 0; i < desc.NumElements; i++)
	{
		size_t lent = strlen(desc.pInputElementDescs[i].SemanticName);

		write(lent);
		write((void*)desc.pInputElementDescs[i].SemanticName, lent * sizeof(char));

		write(desc.pInputElementDescs[i].SemanticIndex);
		write(desc.pInputElementDescs[i].Format);
		write(desc.pInputElementDescs[i].InputSlot);
		write(desc.pInputElementDescs[i].AlignedByteOffset);
		write(desc.pInputElementDescs[i].InputSlotClass);
		write(desc.pInputElementDescs[i].InstanceDataStepRate);

	}
}

void MemStream::write(const D3D12_STREAM_OUTPUT_DESC& desc)
{
	write(desc.NumEntries);

	if (desc.NumEntries == 0)
	{

	}
	else
	{
		//assert(0);
	}

	write(desc.NumStrides);

	if (desc.NumStrides == 0)
	{

	}
	else
	{
		//assert(0);
	}
	write(desc.RasterizedStream);
}

void MemStream::read( D3D12_STREAM_OUTPUT_DESC& desc)
{
	read(desc.NumEntries);

	if (desc.NumEntries == 0)
	{
		desc.pSODeclaration = NULL;
	}
	else
	{
		//assert(0);
	}

	read(desc.NumStrides);

	if (desc.NumStrides == 0)
	{
		desc.pBufferStrides = NULL;
	}
	else
	{
		//assert(0);
	}
	read(desc.RasterizedStream);
}


void WriteStream(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, MemStream* pstream)
{
	pstream->write(desc.pRootSignature);

	pstream->write(desc.BlendState);

	size_t psosize = desc.CachedPSO.CachedBlobSizeInBytes;
	
	pstream->write(psosize);
	
	pstream->write(desc.CachedPSO.pCachedBlob, psosize);

	pstream->write(desc.DepthStencilState);

	size_t codesize = desc.DS.BytecodeLength;
	
	pstream->write(codesize);

	pstream->write(desc.DS.pShaderBytecode, codesize);

	pstream->write(desc.DSVFormat);

	pstream->write(desc.Flags);

	codesize = desc.GS.BytecodeLength;
	pstream->write(codesize);

	pstream->write(desc.GS.pShaderBytecode, codesize);
	codesize = desc.HS.BytecodeLength;

	codesize = desc.HS.BytecodeLength;
	pstream->write(codesize);
	pstream->write(desc.HS.pShaderBytecode, codesize);

	pstream->write(desc.IBStripCutValue);

	pstream->write(desc.InputLayout);

	pstream->write(desc.NodeMask);

	pstream->write(desc.NumRenderTargets);

	pstream->write(desc.PrimitiveTopologyType);

	codesize = desc.PS.BytecodeLength;

	pstream->write(codesize);
	
	pstream->write(desc.PS.pShaderBytecode, codesize);

	pstream->write(desc.RasterizerState);

	pstream->write( desc.RTVFormats, 8 * sizeof(DXGI_FORMAT) );

	pstream->write(desc.SampleDesc);

	pstream->write(desc.SampleMask);

	pstream->write(desc.StreamOutput);

	codesize = desc.VS.BytecodeLength;
	
	pstream->write(codesize);

	pstream->write(desc.VS.pShaderBytecode, codesize);

	
}

void MemStream::read(D3D12_INPUT_LAYOUT_DESC& desc)
{
	read(desc.NumElements);

	desc.pInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[desc.NumElements];

	for (int i = 0; i < desc.NumElements; i++)
	{
		size_t lent;

		read(lent);
		char name[512];
		read((void*)(name), lent * sizeof(char));
		name[lent] = 0;
		//desc.pInputElementDescs[i].SemanticName = name;

		read(desc.pInputElementDescs[i].SemanticIndex);
		read(desc.pInputElementDescs[i].Format);
		read(desc.pInputElementDescs[i].InputSlot);
		read(desc.pInputElementDescs[i].AlignedByteOffset);
		read(desc.pInputElementDescs[i].InputSlotClass);
		read(desc.pInputElementDescs[i].InstanceDataStepRate);
	}
}




void ReadStream(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, MemStream* pstream)
{
	INT64 pointer;
	pstream->read(pointer );

	
	desc.pRootSignature = (ID3D12RootSignature *)pointer;
	
	pstream->read(desc.BlendState);

	size_t psosize;
	pstream->read(psosize);
	desc.CachedPSO.CachedBlobSizeInBytes = psosize;
	if (psosize == 0)
	{
		desc.CachedPSO.pCachedBlob = NULL;
	}
	else
	{
		pstream->read(desc.CachedPSO.pCachedBlob, psosize);
	}

	pstream->read(desc.DepthStencilState);

	size_t codesize;

	pstream->read(codesize);
	desc.DS.BytecodeLength = codesize;
	if (codesize == 0)
	{
		desc.DS.pShaderBytecode = NULL;
	}
	else
	{
		pstream->read(desc.DS.pShaderBytecode, codesize);
	}

	pstream->read(desc.DSVFormat);

	pstream->read(desc.Flags);

	pstream->read(codesize);
	desc.GS.BytecodeLength = codesize;
	if (codesize == 0)
	{
		desc.GS.pShaderBytecode = NULL;
	}
	else
	{
		pstream->read(desc.GS.pShaderBytecode, codesize);
	}

	
	pstream->read(codesize);
	desc.HS.BytecodeLength = codesize;
	if (codesize == 0)
	{
		desc.HS.pShaderBytecode = NULL;
	}
	else
	{
		pstream->read(desc.HS.pShaderBytecode, codesize);
	}
	

	pstream->read(desc.IBStripCutValue);


	pstream->read(desc.InputLayout);

	

	pstream->read(desc.NodeMask);

	pstream->read(desc.NumRenderTargets);

	pstream->read(desc.PrimitiveTopologyType);

	
	pstream->read(codesize);

	unsigned char* pcode = new unsigned char[codesize];

	pstream->read(pcode, codesize);

	desc.PS.pShaderBytecode = pcode;
	desc.PS.BytecodeLength = codesize;
	
	pstream->read(desc.RasterizerState);

	pstream->read(desc.RTVFormats, 8 * sizeof(DXGI_FORMAT));

	pstream->read(desc.SampleDesc);

	pstream->read(desc.SampleMask);

	pstream->read(desc.StreamOutput);

	
	pstream->read(codesize);
	pcode = new unsigned char[codesize];

	pstream->read(pcode, codesize);

	desc.VS.pShaderBytecode = pcode;
	desc.VS.BytecodeLength = codesize;

	

}

