#include "CConnectObject.h"
#include "CConnectTarget.h"

// コンストラクタ
CConnectObject::CConnectObject(float weight, ETaskPriority prio,
	int sortOrder,ETaskPauseType pause)
	: CObjectBase(ETag::eConnectObject, prio, 0, pause)
	, mWeight(weight)
{
}

// デストラクタ
CConnectObject::~CConnectObject()
{
}

// オブジェクト削除を伝える関数
void CConnectObject::DeleteObject(CObjectBase* obj)
{
}

// 更新
void CConnectObject::Update()
{
}

// 衝突処理
void CConnectObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// 引っ張られた時の処理
void CConnectObject::Pull()
{
}

// 接続ターゲットの作成
void CConnectObject::CreateTarget(CVector pos)
{
	// ターゲット生成
	CConnectTarget* target = new CConnectTarget();
	// 位置設定
	target->Position(pos);
	// リストに追加
	mTargets.push_back(target);
}
