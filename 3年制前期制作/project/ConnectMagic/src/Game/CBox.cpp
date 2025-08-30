#include "CBox.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CConnectPointManager.h"

// �ڑ��^�[�Q�b�g�̍��W
#define TARGET_POS_1 CVector( 0.0f,10.0f, 0.0f)

// ���̏d��
#define WEIGHT 0.1f

// �R���C�_�[�̔��a
#define RADIUS 2.4f

// �X�P�[���̔{��
#define SCALE 2.0f

// ���ɓ������ĕԂ��Ă���܂ł̎���
#define RETURN_TIME 0.5f

// �폜�����Y���W
#define DELETE_POS_Y -50.0f

// �R���X�g���N�^
CBox::CBox(CVector defaultPos, float scaleRatio)
	: CConnectObject(WEIGHT)
	, mDefaultPos(defaultPos)
	, mIsRespawn(false)
	, mElapsedTime(0.0f)
{
	mpModel = CResourceManager::Get<CModel>("Box");

	Scale(Scale() * SCALE * scaleRatio);

	// �R���C�_�[�𐶐�
	CreateCol();
	
	// �ڑ��^�[�Q�b�g�𐶐�
	CreateTarget(TARGET_POS_1);

	Position(defaultPos);
}

// �f�X�g���N�^
CBox::~CBox()
{
}

// �Փˏ���
void CBox::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CConnectObject::Collision(self, other, hit);

	if (mpCol == self)
	{
		// ���肪�n�ʂȂ�
		if (other->Layer() == ELayer::eGround)
		{
			// ��邱�Ƃ��o����I�u�W�F�N�g�Ȃ�
			if (other->Tag() == ETag::eRideableObject)
			{
				// ����Ă���I�u�W�F�N�g�ɐݒ肷��
				mpRideObject = other->Owner();
			}
		}
		// ���肪���Ȃ�
		if (other->Layer() == ELayer::eCrushed)
		{
			mIsRespawn = true;
		}
	}
}

// �X�V
void CBox::Update()
{
	// ���X�|�[������Ȃ�
	if (mIsRespawn)
	{
		// ���Ԃ��o��
		mElapsedTime += Times::DeltaTime();
		if (mElapsedTime >= RETURN_TIME)
		{
			// ���X�|�[������
			mIsRespawn = false;
			// �o�ߎ��ԃ��Z�b�g
			mElapsedTime = 0.0f;
			// �������W�ɖ߂�
			Position(mDefaultPos);
		}
	}

	// �폜�������W�ȉ��Ȃ�
	if (Position().Y() <= DELETE_POS_Y)
	{
		SetEnable(false);
	}

	CConnectObject::Update();
	// ��������t���Ă��Ȃ�
	SetIsAttach(false);
}

// �����X�C�b�`�ɒ���t���Ă��邩��ݒ�
void CBox::SetIsAttach(bool enable)
{
	mIsAttach = enable;
}

// �����X�C�b�`�ɒ���t���Ă��邩
bool CBox::GetIsAttach() const
{
	return mIsAttach;
}

// �R���C�_�[�𐶐�
void CBox::CreateCol()
{
	// �t�B�[���h��I�u�W�F�N�g�ƏՓ˔��������R���C�_�[
	mpCol = new CColliderSphere
	(
		this, ELayer::eObject,
		RADIUS
	);
	// ���W�𒲐�
	mpCol->Position(Position() + CVector(0.0f, RADIUS / 2, 0.0f));
	// �v���C���[�A�G�A�t�B�[���h�A�I�u�W�F�N�g�A�R�l�N�g�I�u�W�F�N�g�̒T�m�p�A�X�C�b�`�A���ƏՓ˔���
	mpCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eGround, ELayer::eWall,
		ELayer::eObject, ELayer::eConnectSearch, ELayer::eSwitch,
		ELayer::eCrushed});
}
