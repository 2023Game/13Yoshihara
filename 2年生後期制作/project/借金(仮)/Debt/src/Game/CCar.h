#pragma once
#include "CVehicleBase.h"
#include "CVehicleStatus.h"

class CModel;

// 車のクラス
class CCar : public CVehicleBase, public CVehicleStatus
{
public:
	// コンストラクタ
	CCar(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints);
	// デストラクタ
	~CCar();

	// 更新
	void Update();
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// 描画
	void Render() override;

	// 変数をリセット
	void Reset() override;
private:
	// 移動処理
	void UpdateMove();
	// 停止処理
	void UpdateStop();
	// 壊れた処理
	void UpdateBroken();
	// 車線変更処理
	void UpdateChangeRoad();

	// 車の状態
	enum class EState
	{
		eMove,		// 移動
		eStop,		// 停止
		eBroken,	// 壊れる
		eChangeRoad,// 車線変更
	};
	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// 車両の状態
	int mStateStep;	// 状態内のステップ数
	float mElapsedTime;	// 経過時間

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
#endif
};