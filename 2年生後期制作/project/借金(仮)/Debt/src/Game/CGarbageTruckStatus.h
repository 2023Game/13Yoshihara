#pragma once
#include "CCarStatus.h"

class CGarbageTruckStatus : public CCarStatus
{
public:
	CGarbageTruckStatus();
	~CGarbageTruckStatus();

	// “P‘Ş‚Ü‚Å‚ÌŠÔ‚ğæ“¾
	float GetBackTime();
	// “P‘Ş‚Ü‚Å‚ÌŠÔ‚ğİ’è
	void SetBackTime(float backTime);
	// “P‘Ş‚Ü‚Å‚ÌŠÔ‚ğƒJƒEƒ“ƒg
	void CountBackTime();

	// ‰ñû”ÍˆÍ‚Ì”¼Œa‚ğæ“¾‚·‚é
	float GetCollectRadius();

	// ‰ñûˆõ‚Ìl”‚ğæ“¾‚·‚é
	int GetCollectors();
	// ‰ñûˆõ‚Ìl”‚ğİ’è‚·‚é
	void SetCollectors(int collectors);
private:
	float mBackTime;		// “P‘Ş‚Ü‚Å‚ÌŠÔ
	float mCollectRadius;	// ‰ñû”ÍˆÍ‚Ì”¼Œa
	int mCollectors;		// ‰ñûˆõ‚Ìl”
};