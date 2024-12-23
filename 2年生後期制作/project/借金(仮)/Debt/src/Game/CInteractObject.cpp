#include "CInteractObject.h"
#include "CInput.h"
#include "CBillBoardUI.h"
#include "CObjectBase.h"
#include "CPlayerBase.h"
#include "CCamera.h"

#define INTERACT_IMAGE "UI\\interact.png"
#define IMAGE_SIZE CVector2(6.0f,2.0f)

//コンストラクタ
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eField, prio, 0, pause)
	, mInteractStr("調べる")
	, mIsInteract(false)
	, mIsInteractArea(false)
#if _DEBUG
	, mDebugName("InteractObj")
#endif
{
	// インタラクトの画像を表示
	mpBillBoardUI = new CBillBoardUI(INTERACT_IMAGE, ETag::eInteractUI, ETaskPauseType::eGame);
	mpBillBoardUI->SetSize(IMAGE_SIZE);
	mpBillBoardUI->SetShow(false);
}

CInteractObject::~CInteractObject()
{
	if (mpBillBoardUI != nullptr)
	{
		mpBillBoardUI->Kill();
	}
}

// 調べる内容のテキストを返す
std::string CInteractObject::GetInteractStr() const
{
	return mInteractStr;
}

// 調べられる状態かどうか
bool CInteractObject::CanInteract() const
{
	return true;
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

#if _DEBUG
// デバッグ表示用の名前を取得
std::string CInteractObject::GetDebugName() const
{
	return mDebugName;
}

// デバッグ表示用の名前を設定
void CInteractObject::SetDebugName(std::string name)
{
	mDebugName = name;
}
#endif