#include "CDoor.h"
#include "CColliderMesh.h"
#include "CStageSelectMenu.h"

CDoor::CDoor(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject(15,"eDoor")
	, mpModel(model)
{
	mpCollider = new CColliderMesh
	{
		this,ELayer::eInteractObj,
		model,true
	};

	Position(pos);
	Scale(scale);
	Rotate(rotation);

	// ステージ選択メニューを作成
	mpStageSelectMenu = new CStageSelectMenu();
}

CDoor::~CDoor()
{

}

void CDoor::Interact()
{
}

void CDoor::Update()
{
	// インタラクト判定
	CInteractObject::Interact();

	// ステージ選択メニューを開いてなければ、インタラクトされたとき開く
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
