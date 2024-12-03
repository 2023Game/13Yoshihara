#include "CDoor.h"
#include "CColliderMesh.h"
#include "CStageSelectMenu.h"

CDoor::CDoor(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject(15,"eDoor")
	, mpModel(model)
{
	mpColliderSphere->Position(-10.0f, 0.0f, 0.0f);

	Position(pos);
	Scale(scale);
	Rotate(rotation);

	// �X�e�[�W�I�����j���[���쐬
	mpStageSelectMenu = new CStageSelectMenu();
}

CDoor::~CDoor()
{

}

void CDoor::Update()
{
	// �C���^���N�g����
	CInteractObject::Interact();

	// �X�e�[�W�I�����j���[���J���ĂȂ���΁A�C���^���N�g���ꂽ�Ƃ��J��
	if (!mpStageSelectMenu->IsOpened())
	{
		if (mIsInteract)
		{
			mIsInteract = false;
			mpStageSelectMenu->Open();
		}
	}
}

void CDoor::Render()
{
	mpModel->Render(Matrix());
}
