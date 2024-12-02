#pragma once
#include "CTrashStatusBase.h"

class CGarbageTruckStatus : public CTrashStatusBase
{
public:
	CGarbageTruckStatus();
	~CGarbageTruckStatus();
protected:
	float mPopTime;			// oŒ»‚Ü‚Å‚ÌŠÔ
	float mDeleteTime;		// Á–Å‚Ü‚Å‚ÌŠÔ
	float mBackTime;		// “P‘Ş‚Ü‚Å‚ÌŠÔ
	float mCollectRadius;	// ‰ñû”ÍˆÍ‚Ì”¼Œa
	int mCollectors;		// ‰ñûˆõ‚Ìl”
};