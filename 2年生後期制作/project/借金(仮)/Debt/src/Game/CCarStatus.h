#pragma once
#include "CTrashStatusBase.h"

class CCarStatus : public CTrashStatusBase
{
public:
	CCarStatus();
	~CCarStatus();

	// oŒ»‚Ü‚Å‚ÌŠÔ‚ğæ“¾‚·‚é
	float GetPopTime();
	// oŒ»‚Ü‚Å‚ÌŠÔ‚ğİ’è‚·‚é
	void SetPopTime(float popTime);

	// Á–Å‚Ü‚Å‚ÌŠÔ‚ğæ“¾‚·‚é
	float GetDeleteTime();
	// Á–Å‚Ü‚Å‚ÌŠÔ‚ğİ’è‚·‚é
	void SetDeleteTime(float deleteTime);
private:
	float mPopTime;		// oŒ»‚Ü‚Å‚ÌŠÔ
	float mDeleteTime;	// Á–Å‚Ü‚Å‚ÌŠÔ
};