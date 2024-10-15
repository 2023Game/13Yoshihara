#include "CComputer.h"
#include "CGameMenu.h"

CComputer::CComputer(CModel* model, const CVector& pos, const CVector& scale)
	: mpModel(model)
{
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);
	Position(pos);
	Scale(scale);

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
	//mIsInteract��true�Ȃ�
	if (GetInteract() == true)
	{
		//�C���^���N�g����������
		Interact();
	}
}

void CComputer::Render()
{
	mpModel->Render(Matrix());
}

void CComputer::Interact()
{
	//TODO:�d���I�����j���[���Ăяo��
	if (!mpJobMenu->IsOpened())
	{
		mpJobMenu->Open();
	}
}
