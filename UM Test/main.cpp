#include <Windows.h>
#include <iostream>
#include <iocodes.h>

class KernelMemory
{
public:
	KernelMemory(const char* deviceName)
	{
		hFile = CreateFileA(deviceName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	}

	~KernelMemory()
	{
		if (hFile && hFile != INVALID_HANDLE_VALUE)
			CloseHandle(hFile);
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

	bool GetProcessHandle(PIO_REQUEST_PROCESS_HANDLE request)
	{
		return RequestGeneric(IO_CTL_GETPROCESSHANDLE, request);
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

void main()
{

#if 1
	
	KernelMemory KernelMemory = "\\\\.\\KernelMemory";
	if (!KernelMemory.IsOpen())
	{
		printf("Failed to open...\n");
		return;
	}

	IO_REQUEST_PROCESS_HANDLE process;
	if (!KernelMemory.GetProcessHandle(&process))
	{
		printf("Failed to get proc handle\n");
		return;
	}

	printf("ProcessId -> %p\nClientDLL -> %p\n", process.ProcessId, process.ClientDLL);


	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
			break;

		ULONG_PTR localpawn = 0;
		if (!KernelMemory.ReadVirtualMemory(process.ProcessId, process.ClientDLL + 0x1AF4B80, &localpawn))
		{
			printf("> Failed to read local pawn?\n");
			Sleep(250);
			continue;
		}

		printf("LocalPawn -> %p\n", localpawn);
		if (!localpawn)
			continue;

		float lastFlashTime;
		if (KernelMemory.ReadVirtualMemory(process.ProcessId, localpawn + 0x1668, &lastFlashTime))
		{
			printf("Last flash -> %f\n", lastFlashTime);
			if (lastFlashTime > 0)
			{
				// Remove flash effect by setting last flash time to 0
				KernelMemory.WriteVirtualMemory(process.ProcessId, localpawn + 0x1668, 0.f);
			}
		}

		Sleep(50);
	}
	Sleep(200);
#endif
}