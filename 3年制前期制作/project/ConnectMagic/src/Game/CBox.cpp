#include "CBox.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CConnectPointManager.h"

// �ڑ��^�[�Q�b�g�̍��W
#define TARGET_POS_1 CVector( 0.0f,10.0f, 0.0f)
#define TARGET_POS_2 CVector( 0.0f, 0.0f, 0.0f)
#define TARGET_POS_3 CVector( 6.0f, 5.0f, 0.0f)
#define TARGET_POS_4 CVector(-6.0f, 5.0f, 0.0f)
#define TARGET_POS_5 CVector( 0.0f, 5.0f, 6.0f)
#define TARGET_POS_6 CVector( 0.0f, 5.0f,-6.0f)

// ���̏d��
#define WEIGHT 0.1f

// �R���C�_�[�̔��a
#define RADIUS 2.4f

// �X�P�[���̔{��
#define SCALE 2.0f

// ���ɓ������ĕԂ��Ă���܂ł̎���
#define RETURN_TIME 0.5f

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

	std::vector<CVector> targetPosList = { TARGET_POS_1,TARGET_POS_2,TARGET_POS_3,TARGET_POS_4,TARGET_POS_5,TARGET_POS_6 };
	for (int i = 0; i < targetPosList.size(); i++)
	{
		CVector pos = targetPosList[i] * scaleRatio;
		// 1�ڂłȂ����
		if (i != 0)
		{
			// ����Y���W�ɐݒ�
			pos.Y(targetPosList[i].Y());
		}
		// �ڑ��^�[�Q�b�g�𐶐�
		CreateTarget(pos);
	}

	Position(defaultPos);
}

// �f�X�g���N�^
CBox::~CBox()
{
	SAFE_DELETE(mpCharaCol);
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
		if (other->Layer() == ELayer::eWater)
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

	CConnectObject::Update();
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
	// �t�B�[���h�A�I�u�W�F�N�g�A�R�l�N�g�I�u�W�F�N�g�̒T�m�p�A�X�C�b�`�A���ƏՓ˔���
	mpCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall,
		ELayer::eObject, ELayer::eConnectSearch, ELayer::eSwitch,
		ELayer::eWater});

	// �L�����ƏՓ˔��������R���C�_�[
	mpCharaCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Box_Col")
	);
	// �v���C���[�A�G�A�I�u�W�F�N�g�ƏՓ˔���
	mpCharaCol->SetCollisionLayers({ ELayer::ePlayer, ELayer::eEnemy, ELayer::eObject });

	// �ڑ����̊Ǘ��N���X�̏Փ˔��肷��R���C�_�[�ɒǉ�
	CConnectPointManager::Instance()->AddCollider(mpCharaCol);

	// �J�����̏Փ˔��肷��R���C�_�[�ɒǉ�
	CCamera::CurrentCamera()->AddCollider(mpCharaCol);
}
