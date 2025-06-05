#include "CWand.h"
#include "CModel.h"

// �R���X�g���N�^
CWand::CWand(CObjectBase* owner, ETag tag)
	: CWeapon(tag)
	, mpModel(nullptr)
{
	mpOwner = owner;

	// ���f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Wand");
}

// �f�X�g���N�^
CWand::~CWand()
{
}

// �X�V
void CWand::Update()
{
}

// �`��
void CWand::Render()
{
	mpModel->Render(Matrix());
}
