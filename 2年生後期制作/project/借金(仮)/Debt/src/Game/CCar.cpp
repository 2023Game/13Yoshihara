#include "CCar.h"
#include "CModel.h"

#define MOVE_SPEED 

CCar::CCar(CModel* model, const CVector& pos, const CVector& rotation)
	: CCharaBase(ETag::eCar, ETaskPriority::eDefault)
	, mpModel(model)
	, mState(EState::eMove)
{
	Position(pos);
	Rotation(rotation);

	// �ŏ��͕`��A�X�V���Ȃ�
	SetEnable(false);
	SetShow(false);
}

CCar::~CCar()
{
}

void CCar::Update()
{
	switch (mState)
	{
	// �ړ�
	case EState::eMove:

		Move(GetStatusBase().mMoveSpeed);
		
		break;

	// ��ꂽ
	case EState::eBroken:


		break;
	}
}

void CCar::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

void CCar::Render()
{
	mpModel->Render(Matrix());
}

// ���ʕ����ֈړ�����
void CCar::Move(float moveSpeed)
{
	Position(Position() + VectorZ() * 2.0f);
}
