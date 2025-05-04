#include "CInteractObject.h"
#include "CInput.h"
#include "CInteractUI3D.h"
#include "CObjectBase.h"
#include "CCamera.h"
#include "CPlayerBase.h"

//コンストラクタ
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eInteractObject, prio, 0, pause)
	, mInteractStr("調べる")
#if _DEBUG
	, mDebugName("InteractObj")
#endif
{

	// インタラクトの画像を表示
	mpInteractUI3D = new CInteractUI3D(this);
	mpInteractUI3D->SetOwner(this);
	// 最初は非表示
	mpInteractUI3D->SetEnable(false);
	mpInteractUI3D->SetShow(false);
}

CInteractObject::~CInteractObject()
{
	SAFE_DELETE(mpInteractUI3D);

	// インタラクトUIが存在したら、一緒に削除
	if (mpInteractUI3D != nullptr)
	{
		mpInteractUI3D->SetOwner(nullptr);
		mpInteractUI3D->Kill();
	}
}

// オブジェクト削除を伝える関数
void CInteractObject::DeleteObject(CObjectBase* obj)
{
	// 削除されたのがインタラクトUIであれば、
	// インタラクトUIのポインタを空にする
	if (obj == mpInteractUI3D)
	{
		mpInteractUI3D = nullptr;
	}
}

// 更新
void CInteractObject::Update()
{
	// インタラクトUIを更新
	mpInteractUI3D->Position(Position() + mUIOffsetPos);

	// プレイヤーの一番近くにいる調べるオブジェクトなら
	if (this == CPlayerBase::Instance()->GetNearInteractObject())
	{
		// UIを表示
		mpInteractUI3D->SetShow(true);
	}
	else
	{
		// UIを非表示
		mpInteractUI3D->SetShow(false);
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

// 衝突処理
void CInteractObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
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