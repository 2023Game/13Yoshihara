#include "CComputer.h"
#include "CGameMenu.h"
#include "CInput.h"

CComputer::CComputer(CModel* model, const CVector& pos, const CVector& scale, const CVector& rotation)
	: CInteractObject(15, "eComputer")
	, mpModel(model)
{
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);
	Position(pos);
	Scale(scale);
	Rotate(rotation);

	//�d���I�����j���[���쐬
	mpJobMenu = new CGameMenu();
}

CComputer::~CComputer()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CComputer::Update()
{
	// �C���^���N�g����
	CInteractObject::Interact();

	// �d���I�����j���[���J���ĂȂ���΁A�C���^���N�g���ꂽ�Ƃ��J��
	if (!mpJobMenu->IsOpened())
	{
		if (mIsInteract == true)
		{
			mIsInteract = false;
			mpJobMenu->Open();
		}
	}
}

void CComputer::Render()
{
	mpModel->Render(Matrix());
}
