#pragma once
#include "Common.h"
#include "stdio.h"
#include "xdefine.h"





//void SaveRecordToFile(const char* name);
//void ResetStream();

class MemStream
{

private:
	//static MemStream* instance;
	
	unsigned char* memhandle;
	unsigned char* streamhandle;

	INT64 streamcount;
	

	stringstream nameListCache;

public:

	MemStream();
	~MemStream();
	void init();
	//static MemStream* getstream();
	
	void writetoFile(const char* pname);

	void writeCommandNameToFile(const char *pname);

	void readfromfile(const char* pname);

	void reset();

	template <typename T>
	inline void write(T &el);

	template <typename T>
	inline void write(T *el);

	template <typename T>
	inline void writePointerValue(T *el);

	void write(CommandEnum enu);

	void write(const void* pdata, size_t datasize);

	void write(const D3D12_INPUT_LAYOUT_DESC& desc);

	void write(const D3D12_STREAM_OUTPUT_DESC& desc);


	void read(D3D12_STREAM_OUTPUT_DESC& desc);

	template <typename T>
	inline void read(T &el);

	template <typename T>
	inline void read(T *el);

	template <typename T>
	inline void readPointerValue(T *el);

	inline void read(CommandEnum& enu);

	void read(D3D12_INPUT_LAYOUT_DESC& desc);

	void read(const void* pdata, size_t datasize);

	
};


template <typename T>
inline void MemStream::write(T &el)
{
	size_t datasize = sizeof(T);
	memcpy(streamhandle, &el, datasize);
	streamcount += datasize;
	streamhandle  += datasize;
}

template <typename T>
inline void MemStream::write(T* el)
{
	size_t datasize = sizeof(T*);
	memcpy(streamhandle, &el, datasize);
	streamcount += datasize;
	streamhandle = streamhandle + datasize;
}

template <typename T>
inline void MemStream::writePointerValue(T *el)
{
	bool isnullpointer;
	if (el == nullptr)
	{
		isnullpointer = true;
		write(isnullpointer);
	}
	else
	{
		isnullpointer = false;
		write(isnullpointer);
		write(*el);
	}
}


template <typename T>
inline void MemStream::read(T &el)
{
	size_t datasize = sizeof(T);
	memcpy((void*)&el,streamhandle , datasize);
	streamcount += datasize;
	streamhandle += datasize;
}

template <typename T>
inline void MemStream::read(T *el)
{
	size_t datasize = sizeof(T*);
	memcpy(&el, streamhandle , datasize);
	streamcount += datasize;
	streamhandle = streamhandle + datasize;
}

template <typename T>
inline void MemStream::readPointerValue(T *el)
{
	
	bool isnullpointer;
	read(isnullpointer);
	if (isnullpointer)
	{
		
	}
	else
	{
		read(*el);
	}
}


inline void MemStream::read(CommandEnum& enu)
{
	memcpy(&enu,streamhandle, sizeof(CommandEnum));
	streamcount += sizeof(CommandEnum);
	streamhandle = (unsigned char*)streamhandle + sizeof(CommandEnum);

}

void WriteStream(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, MemStream* pstream);


void ReadStream(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, MemStream* pstream);