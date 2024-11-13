#include "CInteractObject.h"
#include "CInput.h"
#include "CBillBoardImage.h"
#include "CObjectBase.h"
#include "CPlayer.h"

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
	mpBillBoardImage = new CBillBoardImage(INTERACT_IMAGE, ETag::eInteractUI, ETaskPauseType::eGame);
	mpBillBoardImage->SetSize(IMAGE_SIZE);
	mpBillBoardImage->SetShow(false);
}

CInteractObject::~CInteractObject()
{
	if (mpColliderSphere != nullptr)
	{
		delete mpColliderSphere;
		mpColliderSphere = nullptr;
	}
	if (mpBillBoardImage != nullptr)
	{
		mpBillBoardImage->Kill();
	}
}

// インタラクト判定
void CInteractObject::Interact()
{
	CVector direction = CPlayer::Instance()->Position() - mpBillBoardImage->Position();
	direction.Normalize();
	direction = direction * 10.0f;
	// プレイヤー側にビルボードを表示
	mpBillBoardImage->
		Position(CVector(Position().X() + direction.X(),
			CPlayer::Instance()->Position().Y() + 10.0f,
			Position().Z() + direction.Z()));

	// インタラクトエリア内でFキーを押すとmIsInteractをtrue
	if (mIsInteractArea == true)
	{
		mpBillBoardImage->SetShow(true);
		if(CInput::PushKey('F'))
		{
			mIsInteract = true;
		}
	}
	else
	{
		mpBillBoardImage->SetShow(false);
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
