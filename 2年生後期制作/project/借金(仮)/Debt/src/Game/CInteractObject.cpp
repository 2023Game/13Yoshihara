#include "CInteractObject.h"
#include "CPlayer.h"

//コンストラクタ
CInteractObject::CInteractObject()
{
	mpColliderSphere = new CColliderSphere(this, ELayer::eInteract, 5, true);
}

CInteractObject::~CInteractObject()
{
}

//衝突処理
void CInteractObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	//プレイヤーと衝突したとき
	if (other->Layer() == ELayer::ePlayer)
	{
		//mIsInteractの値をプレイヤーのインタラクトの値と同じにする
		SetInteract(CPlayer::Instance()->GetInteract());
	}
}

//mIsInteractの値を設定する
void CInteractObject::SetInteract(bool interact)
{
	mIsInteract = interact;
}

//mIsInteractの値を返す
bool CInteractObject::GetInteract()
{
	return mIsInteract;
}
