#include "CSFrameData.h"
#include "CSMemory.h"
#include "offsets.h"

void CSFrameData::CaptureFrame()
{
	Memory.ReadMemory<::ViewMatrix>(Memory.ClientDll + cs2_dumper::offsets::client_dll::dwViewMatrix, &ViewMatrix);

}