#pragma once
#include <iocodes.h>

class KernelMemory
{
public:
	KernelMemory() : hFile(NULL)
	{
	}

	KernelMemory(const char* deviceName) : hFile(NULL)
	{
		hFile = CreateFileA(deviceName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	}

	~KernelMemory()
	{
		if (hFile && hFile != INVALID_HANDLE_VALUE)
			CloseHandle(hFile);
	}

	bool Open(const char* deviceName)
	{
		hFile = CreateFileA(deviceName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		return hFile && hFile != INVALID_HANDLE_VALUE;
	}

	bool IsOpen()
	{
		if (!hFile || hFile == INVALID_HANDLE_VALUE)
			return false;

		return true;
	}

	bool RequestGeneric(DWORD ioControlCode, void* request, size_t requestSize)
	{
		DWORD bytes;
		BOOL ret = DeviceIoControl(
			hFile,
			ioControlCode,
			request,
			requestSize,
			request,
			requestSize,
			&bytes,
			nullptr
		);

		return ret;
	}

	template <typename T>
	bool RequestGeneric(DWORD ioControlCode, T* request)
	{
		return RequestGeneric(ioControlCode, request, sizeof(T));
	}

	bool RequestProcessHandle(PIO_REQUEST_PROCID request)
	{
		return RequestGeneric(IO_CTL_GETPROCESSID, request);
	}

	ULONG_PTR RequestProcessId()
	{
		IO_REQUEST_PROCID request;
		if (RequestGeneric(IO_CTL_GETPROCESSID, &request))
		{
			return request.ProcessId;
		}
		return 0;
	}

	bool RequestModuleHandle(PIO_REQUEST_MODULE_HANDLE request)
	{
		RequestGeneric(IO_CTL_GETMODULEHANDLE, request);
	}

	ULONG_PTR RequestModuleHandle(const WCHAR* name)
	{
		IO_REQUEST_MODULE_HANDLE request;
		wcscpy_s(request.ModuleName, name);
		if (RequestGeneric(IO_CTL_GETMODULEHANDLE, &request))
		{
			return request.ImageBase;
		}
		return 0;
	}

	bool ReadVirtualMemory(ULONG_PTR procId, ULONG_PTR address, void* buff, size_t readSize)
	{
		IO_REQUEST_READ_MEMORY request;
		request.ProcessId = procId;
		request.AddressFrom = address;
		request.Buffer = buff;
		request.ReadSize = readSize;
		return RequestGeneric(IO_CTL_READMEMORY, &request);
	}

	template <typename T>
	bool ReadVirtualMemory(ULONG_PTR procId, ULONG_PTR address, T* value)
	{
		return ReadVirtualMemory(procId, address, value, sizeof(T));
	}

	bool WriteVirtualMemory(ULONG_PTR procId, ULONG_PTR address, void* buff, size_t writeSize)
	{
		IO_REQUEST_WRITE_MEMORY request;
		request.ProcessId = procId;
		request.AddressTo = address;
		request.Buffer = buff;
		request.WriteSize = writeSize;
		return RequestGeneric(IO_CTL_WRITEMEMORY, &request);
	}

	template <typename T>
	bool WriteVirtualMemory(ULONG_PTR procId, ULONG_PTR address, const T value)
	{
		return WriteVirtualMemory(procId, address, (void*)&value, sizeof(T));
	}

private:
	HANDLE hFile;
};
