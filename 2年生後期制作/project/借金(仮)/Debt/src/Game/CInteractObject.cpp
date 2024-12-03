#include "CInteractObject.h"
#include "CInput.h"
#include "CBillBoardUI.h"
#include "CObjectBase.h"
#include "CPlayerBase.h"
#include "CCamera.h"

#define INTERACT_IMAGE "UI\\interact.png"
#define IMAGE_SIZE CVector2(6.0f,2.0f)

//コンストラクタ
CInteractObject::CInteractObject(float radius, std::string interactName)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mIsInteract(false)
	, mIsInteractArea(false)
{
	// プレイヤーとだけ衝突判定をする
	mpColliderSphere = new CColliderSphere(this, ELayer::eInteract, radius, true);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });

	// インタラクトの画像を表示
	mpBillBoardUI = new CBillBoardUI(INTERACT_IMAGE, ETag::eInteractUI, ETaskPauseType::eGame);
	mpBillBoardUI->SetSize(IMAGE_SIZE);
	mpBillBoardUI->SetShow(false);
}

CInteractObject::~CInteractObject()
{
	if (mpColliderSphere != nullptr)
	{
		delete mpColliderSphere;
		mpColliderSphere = nullptr;
	}
	if (mpBillBoardUI != nullptr)
	{
		mpBillBoardUI->Kill();
	}
}

// インタラクト判定
void CInteractObject::Interact()
{
	// プレイヤーの高さにビルボードを表示
	mpBillBoardUI->
		Position(Position().X(),
			CPlayerBase::Instance()->Position().Y() + 10.0f,
			Position().Z());

	// インタラクトエリア内でFキーを押すとmIsInteractをtrue
	if (mIsInteractArea == true)
	{
		mpBillBoardUI->SetShow(true);
		if(CInput::PushKey('F'))
		{
			mIsInteract = true;
		}
	}
	else
	{
		mpBillBoardUI->SetShow(false);
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
		mpPlayer = other->Owner();
		// プレイヤーと衝突しているときはインタラクトエリア内
		mIsInteractArea = true;
	}
}
