#pragma once

class CUIDManager
{
public:
	// V‚µ‚¢ˆêˆÓ‚ÈID‚ğ¶¬‚µA•Ô‚·
	static unsigned int GenerateNewID();
private:
	static unsigned int mNextID;
};