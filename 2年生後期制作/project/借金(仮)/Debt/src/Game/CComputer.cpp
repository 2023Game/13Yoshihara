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

	//仕事選択メニューを作成
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
	// インタラクト判定
	CInteractObject::Interact();

	// 仕事選択メニューが開いてなければ、インタラクトされたとき開く
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
