#include "CComputer.h"
#include "CGameMenu.h"

CComputer::CComputer(CModel* model, const CVector& pos, const CVector& scale)
	: mpModel(model)
{
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);
	Position(pos);
	Scale(scale);

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
	//mIsInteractがtrueなら
	if (GetInteract() == true)
	{
		//インタラクト処理をする
		Interact();
	}
}

void CComputer::Render()
{
	mpModel->Render(Matrix());
}

void CComputer::Interact()
{
	//TODO:仕事選択メニューを呼び出す
	if (!mpJobMenu->IsOpened())
	{
		mpJobMenu->Open();
	}
}
