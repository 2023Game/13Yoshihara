#pragma once
#include "CCharaBase.h"
#include "CCarStatus.h"

class CColliderCapsule;
class CModel;

// 車のクラス
class CCar : public CCharaBase
{
public:
	CCar(CModel* model, const CVector& pos, const CVector& rotation);
	~CCar();

	void Update();
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	void Render();

protected:
	// 移動処理
	void UpdateMove();
	// 壊れた処理
	void UpdateBroken();

	CCarStatus mCarStatus;	// 車のステータス
	CModel* mpModel;
	CColliderCapsule* mpColliderCapsule;

	enum class EState
	{
		eMove,		// 移動
		eStop,		// 停止
		eBroken,	// 壊れる
		eCollect,	// 回収
	};
	// ステートを変更
	void ChangeState(EState state);
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
	EState mState;
};