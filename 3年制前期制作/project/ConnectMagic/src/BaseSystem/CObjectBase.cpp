#include "CObjectBase.h"
#include "CCollider.h"
#include "Maths.h"
#include "CNavNode.h"

// コンストラクタ
CObjectBase::CObjectBase(ETag tag,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause,
	bool dontDelete, bool addTaskList)
	: CTask(prio, sortOrder, pause, dontDelete, addTaskList)
	, mTag(tag)
	, mIsEnableCol(true)
	, mDepth(0.0f)
	, mColor(CColor::white)
	, mpNavNode(nullptr)
	, mIsDamage(false)
{
}

// デストラクタ
CObjectBase::~CObjectBase()
{
}

// オブジェクト削除を伝える関数
void CObjectBase::DeleteObject(CObjectBase* obj)
{
}

// オブジェクトタグを取得
ETag CObjectBase::Tag() const
{
	return mTag;
}

// オブジェクトタグを設定
void CObjectBase::SetTag(ETag tag)
{
	mTag = tag;
}

// 衝突判定を行うか設定
void CObjectBase::SetEnableCol(bool isEnable)
{
	mIsEnableCol = isEnable;
}

// 衝突判定を行うかどうか
bool CObjectBase::IsEnableCol() const
{
	return mIsEnableCol && IsEnable();
}

// カラーを設定
void CObjectBase::SetColor(const CColor& color)
{
	mColor = color;
}

// カラーを取得
const CColor& CObjectBase::GetColor() const
{
	return mColor;
}

// アルファ値設定
void CObjectBase::SetAlpha(float alpha)
{
	mColor.A(Math::Clamp01(alpha));
}

// アルファ値取得
float CObjectBase::GetAlpha() const
{
	return mColor.A();
}

// カメラまでの距離を計算
void CObjectBase::CalcDepth()
{
	CCamera* camera = CCamera::CurrentCamera();
	if (camera == nullptr) return;

	CVector camPos = camera->Position();
	CVector pos = Position();

	mDepth = (camPos - pos).LengthSqr();
}

// カメラからの距離を取得
float CObjectBase::GetDepth() const
{
	return mDepth;
}

// 衝突処理
void CObjectBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// レイとオブジェクトの衝突判定
bool CObjectBase::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	return false;
}

// 攻撃中か
bool CObjectBase::IsAttacking() const
{
	return false;
}

// 攻撃開始
void CObjectBase::AttackStart()
{
	// 攻撃がヒットしたオブジェクトのリストを初期化
	mAttackHitObjects.clear();
}

// 攻撃終了
void CObjectBase::AttackEnd()
{
}

// 経路探索用のノードを取得
CNavNode* CObjectBase::GetNavNode() const
{
	return mpNavNode;
}

// 攻撃を受けているかどうか
bool CObjectBase::IsDamaging() const
{
	return mIsDamage;
}

// すべて削除
void CObjectBase::Delete()
{
	Kill();
}

// コライダーを作成
void CObjectBase::CreateCol()
{
}

// 攻撃がヒットしたオブジェクトを追加
void CObjectBase::AddAttackHitObj(CObjectBase* obj)
{
	mAttackHitObjects.push_back(obj);
}

// 既に攻撃がヒットしているオブジェクトかどうか
bool CObjectBase::IsAttackHitObj(CObjectBase* obj) const
{
	auto find = std::find
	(
		mAttackHitObjects.begin(),
		mAttackHitObjects.end(),
		obj
	);
	return find != mAttackHitObjects.end();
}