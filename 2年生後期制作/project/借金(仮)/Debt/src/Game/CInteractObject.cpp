#include "CInteractObject.h"
#include "CInput.h"
#include "CBillBoardImage.h"

#define INTERACT_IMAGE "res\\UI\\menu_item.png"

//コンストラクタ
CInteractObject::CInteractObject(float radius, std::string interactName)
	: mIsInteract(false)
	, mIsInteractArea(false)
{
	// プレイヤーとだけ衝突判定をする
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eInteract,
		radius,
		true
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });

	// TODO:インタラクトの画像表示
	// インタラクトの画像を表示
	mpBillBoardImage=new CBillBoardImage
		(INTERACT_IMAGE,ETag::)
}

CInteractObject::~CInteractObject()
{
	if (mpColliderSphere != nullptr)
	{
		delete mpColliderSphere;
		mpColliderSphere = nullptr;
	}
}

// インタラクト判定
void CInteractObject::Interact()
{
	// インタラクトエリア内でFキーを押すとmIsInteractをtrue
	if (mIsInteractArea == true)
	{
		if(CInput::PushKey('F'))
		{
			mIsInteract = true;
		}
	}

	// プレイヤーと衝突してないときはmIsInteractAreaをfalse
	mIsInteractArea = false;
}

// 衝突処理
void CInteractObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	//プレイヤーと衝突したとき
	if (other->Layer() == ELayer::ePlayer)
	{
		// プレイヤーと衝突しているときはインタラクトエリア内
		mIsInteractArea = true;
	}
}
