#include "CComputer.h"
#include "CColliderSphere.h"
#include "CBuyMenu.h"
#include "CInput.h"

#define UI_OFFSET_Y 2.5f

CComputer::CComputer(CModel* model, const CVector& pos,
	const CVector& scale, const CVector& rotation)
	: CInteractObject()
	, mpModel(model)
{
	// UIのオフセット位置を設定
	mUIOffsetPos = CVector(0.0f, UI_OFFSET_Y, 0.0f);

	mpCollider = new CColliderSphere
	{
		this,ELayer::eInteractObj,
		4.0f
	};
	// コライダの位置調整
	mpCollider->Position(1.0f, 1.0f, 0.0f);
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer,ELayer::eInteractSearch });

	Position(pos);
	Scale(scale);
	Rotate(rotation);

	mInteractStr = "パソコンを使用";

	// 購入メニューを作成
	mpBuyMenu = new CBuyMenu();
}

CComputer::~CComputer()
{
	SAFE_DELETE(mpCollider);
}

// 調べる
void CComputer::Interact()
{
	// 購入メニューが開いていなければ
	if (!mpBuyMenu->IsOpened())
	{
		// 開く
		mpBuyMenu->Open();
	}
}

void CComputer::Render()
{
	mpModel->Render(Matrix());
}
