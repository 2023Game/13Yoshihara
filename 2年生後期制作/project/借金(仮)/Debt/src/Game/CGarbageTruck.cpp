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
	// �ړ�
	case EState::eMove:	CCar::UpdateMove();	break;
	// ��~
	case EState::eStop: UpdateStop();	break;
	// ��ꂽ
	case EState::eBroken: CCar::UpdateBroken();	break;
	// ���
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

// ��~
void CGarbageTruck::UpdateStop()
{
}

// ���
void CGarbageTruck::UpdateCollect()
{
}
