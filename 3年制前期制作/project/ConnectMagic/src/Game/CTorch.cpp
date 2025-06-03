#include "CTorch.h"
#include "CColliderCapsule.h"

// �����̉��̃I�t�Z�b�g���W
#define TORCH_FIRE_OFFSET_POS CVector(0.0f,9.0f,0.0f)

// �R���C�_�[�̔��a
#define RADIUS 5.0f

// �����̏c�̒���
#define TORCH_HEIGHT 7.0f

// �R���X�g���N�^
CTorch::CTorch()
	: CFire(TORCH_FIRE_OFFSET_POS)
{
	mpModel = CResourceManager::Get<CModel>("Torch");

	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CTorch::~CTorch()
{
}

// �R���C�_�[�𐶐�
void CTorch::CreateCol()
{
	mpCol = new CColliderCapsule(
		this, ELayer::eObject,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, TORCH_HEIGHT, 0.0f),
		RADIUS, true
	);
	// �I�u�W�F�N�g�ƃv���C���[�Ɛڑ��I�u�W�F�N�g�̒T�m�p�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::eObject,ELayer::ePlayer,
		ELayer::eConnectSearch });
}
