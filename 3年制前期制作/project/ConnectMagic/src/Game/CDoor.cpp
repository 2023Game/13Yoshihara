#include "CDoor.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"

// �J�����Ƃ��̉�]
#define OPEN_ROT 90.0f
// ��]���x
#define ROTATE_SPEED 2.0f

// 臒l
#define THRESHOLD 1.0f

// �R���X�g���N�^
CDoor::CDoor(const CVector& rot)
	: CSwitchObject()
	, mDefaultRotation(rot)
{
	mpModel = CResourceManager::Get<CModel>("Door");

	// �R���C�_�[�𐶐�
	CreateCol();

	// ��]��ݒ�
	Rotation(rot);
}

// �f�X�g���N�^
CDoor::~CDoor()
{
}

// �R���C�_�[�𐶐�
void CDoor::CreateCol()
{
	mpCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Door_Col"),
		true
	);
	// �v���C���[�A�G�A�I�u�W�F�N�g�ƏՓ˔���
	mpCol->SetCollisionLayers({ ELayer::ePlayer,
		ELayer::eEnemy,ELayer::eObject });

	// �ڑ����̊Ǘ��N���X�̏Փ˔��肷��R���C�_�[�ɒǉ�
	CConnectPointManager::Instance()->AddCollider(mpCol);

	// �J�����̏Փ˔��肷��R���C�_�[�ɒǉ�
	CCamera::CurrentCamera()->AddCollider(mpCol);
}

// ��p���Ă��Ȃ����̏���
void CDoor::UpdateOff()
{
	// ������Ԃ���]���傫�����
	if (EulerAngles().Y() > mDefaultRotation.Y() &&
		EulerAngles().Y() <= mDefaultRotation.Y() + OPEN_ROT + THRESHOLD)
	{
		RotateAxis(CVector::up, -ROTATE_SPEED);
	}
	// �������Ȃ������Ԃ�ݒ�
	else
	{
		// ��]�p�x���擾
		CVector rot = EulerAngles();
		// �����l��ݒ�
		rot.Y(mDefaultRotation.Y());
		Rotation(rot);
	}
}

// ��p���Ă��鎞�̏���
void CDoor::UpdateOn()
{
	// �J������Ԃ���]�����������
	if (EulerAngles().Y() < mDefaultRotation.Y() + OPEN_ROT - THRESHOLD)
	{
		RotateAxis(CVector::up, ROTATE_SPEED);
	}
	// �傫���Ȃ�J������Ԃ�ݒ�
	else
	{
		// ��]�p�x���擾
		CVector rot = EulerAngles();
		// �����l�{�J�����Ƃ��̉�]��ݒ�
		rot.Y(mDefaultRotation.Y() + OPEN_ROT);
		Rotation(rot);
	}
}
