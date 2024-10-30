#include "CComputer.h"
#include "CModel.h"
#include "CColliderMesh.h"
#include "CGameMenuBase.h"
#include "CInput.h"

#define MENU_ITEM1 "UI/menu_item.png"
#define MENU_SELECT "UI/menu_item_select.png"

CComputer::CComputer(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject(15, "eComputer")
	, mpModel(model)
{
	mMenuItemPathList.push_back(MENU_ITEM1);
	mMenuItemPathList.push_back(MENU_ITEM1);
	mMenuItemPathList.push_back(MENU_ITEM1);

	mMenuSelectPath = MENU_SELECT;

	mpColliderSphere->Position(-10.0f, 0.0f, 0.0f);
	mpColliderMesh = 
		new CColliderMesh(this, ELayer::eField, mpModel, true);
	Position(pos);
	Scale(scale);
	Rotate(rotation);

	// 購入メニューを作成
	mpBuyMenu = new CGameMenuBase(mMenuItemPathList, mMenuSelectPath);
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
