#include "CInteractObject.h"
#include "CInput.h"

//コンストラクタ
CInteractObject::CInteractObject(float radius, std::string interactName)
	: mIsInteract(false)
	, mIsInteractArea(false)
{
	mpColliderSphere = new CColliderSphere(this, ELayer::eInteract, radius, true);
	mInteractName = interactName;
}

CInteractObject::~CInteractObject()
{
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
	if (other->Layer() == ELayer::eInteract)
	{
		// プレイヤーと衝突しているときはインタラクトエリア内
		mIsInteractArea = true;
	}
}
