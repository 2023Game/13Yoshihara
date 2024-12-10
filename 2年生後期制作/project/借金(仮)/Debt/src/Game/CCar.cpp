#include "CCar.h"
#include "CColliderCapsule.h"
#include "CModel.h"

#define CAR_HEIGHT 5.0f
#define CAR_WIDTH 5.0f
#define CAPSULE_RADIUS 5.0f

CCar::CCar(CModel* model, const CVector& pos, const CVector& rotation)
	: CCharaBase(ETag::eCar, ETaskPriority::eDefault)
	, mpModel(model)
	, mState(EState::eMove)
{
	Position(pos);
	Rotation(rotation);

	// �v���C���[�A�G�A�X�|�[���]�[���Ƃ����Փ˔��������
	mpColliderCapsule = new CColliderCapsule
	{
		this,ELayer::eVehicle,
		CVector(CAR_HEIGHT - CAPSULE_RADIUS * 10,CAR_HEIGHT,0.0f),
		CVector(-CAR_HEIGHT + CAPSULE_RADIUS * 10,CAR_HEIGHT,0.0f),
		CAPSULE_RADIUS
	};
	mpColliderCapsule->SetCollisionLayers({ ELayer::ePlayer, ELayer::eEnemy, ELayer::eSpawnZone });

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
	case EState::eMove:	UpdateMove();	break;
	// ��ꂽ
	case EState::eBroken:	UpdateBroken();	break;


		break;
	}

	CDebugPrint::Print("CarState:%s\n", GetStateStr(mState).c_str());
}

// �Փˏ���
void CCar::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderCapsule)
	{

	}
}

void CCar::Render()
{
	mpModel->Render(Matrix());
}

// �ړ�����
void CCar::UpdateMove()
{
	// ���ʂֈړ�
	Position(Position() + VectorZ() * mCarStatus.GetBaseMoveSpeed());
}

// ��ꂽ����
// �ړ����~���ď��Ŏ��Ԃ��o������\��������
void CCar::UpdateBroken()
{
	// ���ł���܂�
	mCarStatus.CountDeleteTime();
	
	if (mCarStatus.IsElapsedDeleteTime())
	{
		SetEnable(false);
		SetShow(false);
	}
}

// �X�e�[�^�X�؂�ւ�
void CCar::ChangeState(EState state)
{
	// �����Ȃ珈�����Ȃ�
	if (state == mState) return;

	mState = state;
}

// ��Ԃ̕�������擾
std::string CCar::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:		return "�ړ���";
	case EState::eStop:		return "��~��";
	case EState::eBroken:	return "���Ă���";
	case EState::eCollect:	return "�����";
	}
	return "";
}
