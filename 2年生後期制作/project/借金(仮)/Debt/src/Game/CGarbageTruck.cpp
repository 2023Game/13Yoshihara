#include "CGarbageTruck.h"
#include "CModel.h"

CGarbageTruck::CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation)
	: CCar(model, pos, rotation)
{
}

CGarbageTruck::~CGarbageTruck()
{
}

void CGarbageTruck::Update()
{
	switch (mState)
	{
	// ˆÚ“®
	case EState::eMove:	CCar::UpdateMove();	break;
	// ’â~
	case EState::eStop: UpdateStop();	break;
	// ‰ó‚ê‚½
	case EState::eBroken: CCar::UpdateBroken();	break;
	// ‰ñû
	case EState::eCollect: UpdateCollect();	break;
	}

	CDebugPrint::Print("TruckState:%s\n", GetStateStr(mState).c_str());
}

void CGarbageTruck::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

void CGarbageTruck::Render()
{
	mpModel->Render(Matrix());
}

// ’â~
void CGarbageTruck::UpdateStop()
{
}

// ‰ñû
void CGarbageTruck::UpdateCollect()
{
}
