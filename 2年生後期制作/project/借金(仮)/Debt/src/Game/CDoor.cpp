#include "CDoor.h"
#include "CModel.h"
#include "CColliderMesh.h"
#include "CStageMenu.h"

#define MENU_ITEM1 "UI/menu_item.png"
#define MENU_SELECT "UI/menu_item_select.png"

CDoor::CDoor(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject(15,"eDoor")
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

	// ステージ選択メニューを作成
	mpStageMenu = new CStageMenu(mMenuItemPathList, mMenuSelectPath);
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
	// インタラクト判定
	CInteractObject::Interact();

	// ステージ選択メニューを開いてなければ、インタラクトされたとき開く
	if (!mpStageMenu->IsOpened())
	{
		if (mIsInteract)
		{
			mIsInteract = false;
			mpStageMenu->Open();
		}
	}
}

void CDoor::Render()
{
	mpModel->Render(Matrix());
}
