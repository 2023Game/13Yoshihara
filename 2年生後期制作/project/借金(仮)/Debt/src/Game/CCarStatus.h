#pragma once
#include "CTrashStatusBase.h"

class CCarStatus : public CTrashStatusBase
{
public:
	CCarStatus();
	~CCarStatus();
protected:
	float mPopTime;		// oŒ»‚Ü‚Å‚ÌŠÔ
	float mDeleteTime;	// Á–Å‚Ü‚Å‚ÌŠÔ
};