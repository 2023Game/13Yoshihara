#pragma once
#include "CCharaBase.h"
#include "CGarbageTruckStatus.h"
#include "CVehicleBase.h"
#include "CGarbageTruckStatus.h"

class CModel;

// ゴミ収集車のクラス
// 車のクラスを継承
class CGarbageTruck : public CVehicleBase , public CGarbageTruckStatus
{
public:
	// コンストラクタ
	CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation, 
		ERoadType road, std::vector<CNavNode*> patrolPoints);
	// デストラクタ
	~CGarbageTruck();

	// 更新
	void Update();
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// 描画
	void Render() override;

	// 変数をリセット
	void Reset() override;

	// ターゲットのポインタを取得
	CObjectBase* GetTarget() const;
private:
	// ターゲットのポインタ
	CObjectBase* mpTarget;
	// プレイヤーと敵の探知用
	CCollider* mpSearchCol;

	// 撤退の移動中かどうか
	bool mIsWithdraw;

	// 移動処理
	void UpdateMove();
	// 停止処理
	void UpdateStop();
	// 壊れた処理
	void UpdateBroken();
	// 車線変更処理
	void UpdateChangeRoad();
	// 回収処理
	void UpdateCollect();

	// 車の状態
	enum class EState
	{
		eMove,		// 移動
		eStop,		// 停止
		eBroken,	// 壊れる
		eChangeRoad,// 車線変更
		eCollect,	// 回収
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