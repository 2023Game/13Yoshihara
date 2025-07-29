#include "CLastRoom.h"
#include "CPortal.h"

// �|�[�^���̃I�t�Z�b�g���W
#define PORTAL_OFFSET_POS CVector(0.0f,15.0f,-60.0f)

// �R���X�g���N�^
CLastRoom::CLastRoom(const CVector& pos)
	: CRoomBase(0.0f)
{
	mpModel = CResourceManager::Get<CModel>("LastRoom");
	// ���W��ݒ�
	Position(pos);

	// �R���C�_�[�𐶐�
	CreateCol("LastRoom_Ground_Col", "LastRoom_Wall_Col", "");

	// �I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CLastRoom::~CLastRoom()
{
}

// �I�u�W�F�N�g�𐶐�
void CLastRoom::CreateFieldObjects()
{
	// �|�[�^���𐶐�
	mpPortal = new CPortal();
	// ���W��ݒ�
	mpPortal->Position(Position() + PORTAL_OFFSET_POS);
}
