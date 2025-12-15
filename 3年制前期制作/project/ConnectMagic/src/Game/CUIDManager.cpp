#include "CUIDManager.h"

unsigned int CUIDManager::mNextID = 0;

unsigned int CUIDManager::GenerateNewID()
{
	return ++mNextID;
}
