#include "CWand.h"
#include "CModel.h"

// コンストラクタ
CWand::CWand(CObjectBase* owner, ETag tag)
	: CWeapon(tag)
	, mpModel(nullptr)
{
	mpOwner = owner;

	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Wand");
}

// デストラクタ
CWand::~CWand()
{
}

// 更新
void CWand::Update()
{
}

// 描画
void CWand::Render()
{
	mpModel->Render(Matrix());
}
