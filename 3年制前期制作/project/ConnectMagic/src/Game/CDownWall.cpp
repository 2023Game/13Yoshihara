#include "CDownWall.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"

// �ړ���
#define MOVE_DIST 50.0f
// �ő�ړ���
#define MAX_MOVE_DIST 125.0f

// �R���X�g���N�^
CDownWall::CDownWall(CVector defaultPos)
	: CSwitchObject(ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mDefaultPosY(defaultPos.Y())
{
	mpModel = CResourceManager::Get<CModel>("DownWall");

	// �R���C�_�[�𐶐�
	CreateCol();

	Position(defaultPos);
}

// �f�X�g���N�^
CDownWall::~CDownWall()
{
}

// �R���C�_�[�𐶐�
void CDownWall::CreateCol()
{
	mpCol = new CColliderMesh(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("DownWall"),
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
void CDownWall::UpdateOff()
{
	// �����l���Y���W�����������
	if (Position().Y() < mDefaultPosY)
	{
		// ��Ɉړ�
		CVector pos = Position();
		pos.Y(pos.Y() + MOVE_DIST * Times::DeltaTime());
		Position(pos);
	}
	// �����l���Y���W���傫�����
	else if(Position().Y() > mDefaultPosY)
	{
		// Y���W�������l�ɐݒ�
		CVector pos = Position();
		pos.Y(mDefaultPosY);
		Position(pos);
	}
}

// ��p���Ă��鎞�̏���
void CDownWall::UpdateOn()
{
	// �����l-�ő�ړ��ʂ��Y���W���傫�����
	if (Position().Y() > mDefaultPosY - MAX_MOVE_DIST)
	{
		// ���Ɉړ�
		CVector pos = Position();
		pos.Y(pos.Y() - MOVE_DIST * Times::DeltaTime());
		Position(pos);
	}
	// �����l-�ő�ʂ��Y���W�����������
	else if (Position().Y() < mDefaultPosY - MAX_MOVE_DIST)
	{
		// Y���W�������l-�ő�ʂɐݒ�
		CVector pos = Position();
		pos.Y(mDefaultPosY - MAX_MOVE_DIST);
		Position(pos);
	}
}
