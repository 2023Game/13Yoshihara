#include "CSpellBase.h"
#include "CModel.h"

// コンストラクタ
CSpellBase::CSpellBase(ESpellElementalType elemental, ESpellShapeType shape,
	CObjectBase* owner, CObjectBase* target, float deleteTime)
	: CObjectBase(ETag::eSpell, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, CSpellStatus(elemental, shape)
	, mpModel(nullptr)
	, mpOwner(owner)
	, mpTarget(target)
	, mMoveDir(CVector::zero)
	, mDeleteTime(deleteTime)
{
	// 待機状態へ
	ChangeState(EState::eIdle);
}

// デストラクタ
CSpellBase::~CSpellBase()
{
	SAFE_DELETE(mpSpellCol);
}

// 更新
void CSpellBase::Update()
{
	switch (mState)
	{
	case CSpellBase::EState::eIdle:			UpdateIdle();		break;
	case CSpellBase::EState::eShooting:		UpdateShooting();	break;
	}

	// 移動
	Position(Position() + mMoveSpeed);
}

// 描画
void CSpellBase::Render()
{
	mpModel->Render(Matrix());
}

// 衝突処理
void CSpellBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpSpellCol)
	{
		// 相手が地形なら
		if (other->Layer() == ELayer::eGround ||
			other->Layer() == ELayer::eWall ||
			other->Layer() == ELayer::eObject)
		{
			// 削除
			Kill();
		}
		// 相手がプレイヤーなら
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 削除
			Kill();
		}
		// 相手が敵なら
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 削除
			Kill();
		}
	}
}

// 呪文のターゲットを設定
void CSpellBase::SetTarget(CObjectBase* target)
{
	mpTarget = target;
}

// 待機中の更新
void CSpellBase::UpdateIdle()
{
	if (mpSpellCol != nullptr)
	{
		// コライダーは無効
		mpSpellCol->SetEnable(false);
	}
	// 持ち主についていく
	SetParent(mpOwner);
}

// 発射中の更新
void CSpellBase::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	// 消滅するまでの時間が経過したら削除
	if (mElapsedTime > mDeleteTime)
	{
		Kill();
		return;
	}
}

// 状態を切り替え
void CSpellBase::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;

	// 発射するときにコライダーがあるなら
	if (mState == EState::eShooting &&
		mpSpellCol != nullptr)
	{
		// 有効
		mpSpellCol->SetEnable(true);
		// ついていかない
		SetParent(nullptr);
	}
}
