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

	// ステージ選択メニューを作成
	mpStageSelectMenu = new CStageSelectMenu();
}

CDoor::~CDoor()
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
