#include "CDoor.h"
#include "CModel.h"
#include "CColliderMesh.h"
#include "CGameMenu.h"

CDoor::CDoor(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject(15,"eDoor")
	, mpModel(model)
{
	mpColliderSphere->Position(-10.0f, 0.0f, 0.0f);
	mpColliderMesh =
		new CColliderMesh(this, ELayer::eField, mpModel, true);
	Position(pos);
	Scale(scale);
	Rotate(rotation);

	// �d���I�����j���[���쐬
	mpJobMenu = new CGameMenu();
}

CDoor::~CDoor()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CDoor::Update()
{
	// �C���^���N�g����
	CInteractObject::Interact();

	// �d���I�����j���[���J���ĂȂ���΁A�C���^���N�g���ꂽ�Ƃ��J��
	if (!mpJobMenu->IsOpened())
	{
		if (mIsInteract)
		{
			mIsInteract = false;
			mpJobMenu->Open();
		}
	}
}

void CDoor::Render()
{
	mpModel->Render(Matrix());
}
