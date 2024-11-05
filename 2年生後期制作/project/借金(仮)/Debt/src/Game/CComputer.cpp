#include "CComputer.h"
#include "CModel.h"
#include "CColliderMesh.h"
#include "CBuyMenu.h"
#include "CInput.h"

CComputer::CComputer(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject(15, "eComputer")
	, mpModel(model)
{
	mpColliderSphere->Position(-10.0f, 0.0f, 0.0f);
	mpColliderMesh = 
		new CColliderMesh(this, ELayer::eField, mpModel, true);
	Position(pos);
	Scale(scale);
	Rotate(rotation);

	// 購入メニューを作成
	mpBuyMenu = new CBuyMenu();
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

	// 購入メニューが開いてなければ、インタラクトされたとき開く
	if (!mpBuyMenu->IsOpened())
	{
		if (mIsInteract)
		{
			mIsInteract = false;
			mpBuyMenu->Open();
		}
	}
}

void CComputer::Render()
{
	mpModel->Render(Matrix());
}
