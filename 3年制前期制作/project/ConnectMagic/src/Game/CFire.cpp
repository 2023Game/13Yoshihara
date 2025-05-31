#include "CFire.h"
#include "CFlamethrower.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CColliderMesh.h"

// �d�ʁi�������Ȃ��j
#define WEIGHT 1.0f

// �R���C�_�[�̔��a
#define RADIUS 5.0f

// �����̏c�̒���
#define TORCH_HEIGHT 7.0f

// �R���X�g���N�^
CFire::CFire(std::string modelName, CVector fireOffsetPos,
	float fireScale, float fireSpeed, CVector targetOffsetPos)
	: CConnectObject(WEIGHT,ETaskPriority::eBillboard)
	, mIsFire(false)
{
	// �ڑ��^�[�Q�b�g�̃^�O�����ɐݒ�
	SetConnectObjTag(EConnectObjTag::eFire);

	// ���f���̖��O���ݒ肳��Ă���Ȃ�
	if (modelName != "")
	{
		// ���f����ݒ�
		mpModel = CResourceManager::Get<CModel>(modelName.c_str());
	}
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

	// �R���C�_�[�𐶐�
	CreateCol(modelName);
}

// �f�X�g���N�^
CFire::~CFire()
{
}

// �q�������Ƃ��̏���
void CFire::Connect(CConnectObject* other)
{
	// ���̐ڑ��I�u�W�F�N�g�Ȃ�
	if (other->GetConnectObjTag() == EConnectObjTag::eFire)
	{
		// ���̃N���X���擾
		CFire* fire = dynamic_cast<CFire*>(other);
		
		// ����̉������Ă���Ȃ�
		if (fire->GetFire())
		{
			if (!mIsFire)
			{
				// �����̉���t����
				SetFire(true);
			}
		}
	}
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

// �R���C�_�[�𐶐�
void CFire::CreateCol(std::string modelName)
{
	// ��̏ꍇ
	if (modelName == "")
	{
		mpCol = new CColliderSphere
		(
			this, ELayer::eObject,
			RADIUS, true
		);
		// �ڑ��I�u�W�F�N�g�̒T�m�p�Ƃ����Փ˔���
		mpCol->SetCollisionLayers({ ELayer::eConnectSearch });
	}
	// �L�����v�t�@�C���[�̏ꍇ
	else if (modelName == "CampFire")
	{
		mpCol = new CColliderMesh
		(
			this, ELayer::eObject,
			CResourceManager::Get<CModel>("CampFire_Col"),
			true
		);
	}
	// ����ȊO�̏ꍇ
	else
	{
		mpCol = new CColliderCapsule
		(
			this, ELayer::eObject,
			CVector(0.0f, 0.0f, 0.0f) , CVector(0.0f, TORCH_HEIGHT, 0.0f),
			RADIUS, true
		);
		// �v���C���[�Ɛڑ��I�u�W�F�N�g�̒T�m�p�Ƃ����Փ˔���
		mpCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eConnectSearch });
	}
}
