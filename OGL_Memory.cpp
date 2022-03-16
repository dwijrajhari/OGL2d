#include<stdlib.h>

#include "OGL_Memory.h"
#include "OGL_Common.h"

void* MemManager::MemAllocate(long size)
{
	_ASSERT(size > 0);

	if (size > 1024)
	{
		m_MemCount += size;
		return malloc(size);
	}


	return nullptr;
}

void MemManager::MemFree()
{
}
