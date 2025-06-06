#pragma once
#include "CObjectBase.h"
#include "CSpellStatus.h"
#include "SpellDefine.h"

class CSpellCaster;
class CModel;

// 呪文の基底クラス
// オブジェクトベースと呪文のステータスを継承
class CSpellBase : public CObjectBase, public CSpellStatus
{
	friend CSpellCaster;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="elemental">属性</param>
	/// <param name="shape">形</param>
	/// <param name="owner">持ち主</param>
	/// <param name="target">目標</param>
	/// <param name="deleteTime">消滅するまでの時間</param>
	CSpellBase(ESpellElementalType elemental, ESpellShapeType shape,
		CObjectBase* owner, CObjectBase* target, float deleteTime);
	// デストラクタ
	~CSpellBase();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 呪文のターゲットを設定
	void SetTarget(CObjectBase* target);

protected:
	// 待機中の更新
	virtual void UpdateIdle();
	// 発射中の更新
	virtual void UpdateShooting();

	// 消滅するまでの時間
	float mDeleteTime;

	// 状態
	enum class EState
	{
		eIdle,			// 待機中
		eShooting,		// 発射中
	};
	// 状態を切り替え
	void ChangeState(EState state);
	EState mState;
	int mStateStep;	// 状態内のステップ
	float mElapsedTime;	// 経過時間

	// 移動速度
	CVector mMoveSpeed;

	// 魔法のモデル
	CModel* mpModel;

	// コライダー
	CCollider* mpSpellCol;

	// 持ち主
	CObjectBase* mpOwner;

	// 呪文のターゲット
	CObjectBase* mpTarget;

	// 移動方向
	CVector mMoveDir;
};