#include "CFire.h"
#include "CFlamethrower.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CColliderMesh.h"

// �d�ʁi�������Ȃ��j
#define WEIGHT 1.0f

// �R���C�_�[���a
#define RADIUS 5.0f

// �R���X�g���N�^
CFire::CFire(CVector fireOffsetPos,
	float fireScale, float fireSpeed, CVector targetOffsetPos)
	: CConnectObject(WEIGHT,ETaskPriority::eBillboard)
	, mIsFire(false)
{
	// �ڑ��^�[�Q�b�g�̃^�O�����ɐݒ�
	SetConnectObjTag(EConnectObjTag::eFire);

	// �d�͖���
	mIsGravity = false;

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector::zero,
		CVector::up
	);
	// �ړ����x��ݒ�
	mpFlamethrower->SetFlameMoveSpeed(fireSpeed);
	// �X�P�[����ݒ�
	mpFlamethrower->SetFlameScale(fireScale);
	// �X�P�[���l�͕ω����Ă����Ȃ�
	mpFlamethrower->SetFlameScaleAnimTime(0.0f);
	// �I�t�Z�b�g���W��ݒ�
	mpFlamethrower->SetThrowOffsetPos(fireOffsetPos);

	// �ڑ��^�[�Q�b�g�𐶐�
	CreateTarget(Position() + fireOffsetPos + targetOffsetPos);
}

// �f�X�g���N�^
CFire::~CFire()
{
}

// �q�������Ƃ��̏���
void CFire::Connect(CVector wandPointPos, CVector targetPointPos)
{
	//// ���̐ڑ��I�u�W�F�N�g�Ȃ�
	//if (other->GetConnectObjTag() == EConnectObjTag::eFire)
	//{
	//	// ���̃N���X���擾
	//	CFire* fire = dynamic_cast<CFire*>(other);
	//	
	//	// ����̉������Ă���Ȃ�
	//	if (fire->GetFire())
	//	{
	//		if (!mIsFire)
	//		{
	//			// �����̉���t����
	//			SetFire(true);
	//		}
	//	}
	//}
}

// �������Ă��邩��ݒ�
void CFire::SetFire(bool isEnable)
{
	// �������Ă��邩��ݒ�
	mIsFire = isEnable;
	// �������Ă���Ȃ�
	if (mIsFire)
	{
		// ���̐����J�n
		mpFlamethrower->Start();
		// �R�����Ƃ��̏���
		Burning();
	}
	// �������Ă��Ȃ��Ȃ�
	else
	{
		// ���̐�����~
		mpFlamethrower->Stop();
	}
}

// �������Ă��邩���擾
bool CFire::GetFire() const
{
	return mIsFire;
}

void CFire::CreateCol()
{
	mpCol = new CColliderSphere(
		this, ELayer::eObject,
		RADIUS, true
	);
	// �ڑ��I�u�W�F�N�g�̒T�m�p�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::eConnectSearch });
}

// �R�������̏���
void CFire::Burning()
{
}