#pragma once
#include "CCharaBase.h"
#include "CGarbageTruckStatus.h"
#include "CVehicleBase.h"
#include "CGarbageTruckStatus.h"

class CModel;
class CCollector;

// ゴミ収集車のクラス
// 車のクラスを継承
// ゴミ収集車のステータスを継承
class CGarbageTruck : public CVehicleBase , public CGarbageTruckStatus
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">座標</param>
	/// <param name="rotation">回転</param>
	/// <param name="road">道の種類</param>
	/// <param name="patrolPoints">巡回するポイントのリスト</param>
	/// <param name="punisher">trueならば、お仕置き用</param>
	CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints, bool punisher);
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

	// 回収をできるZの範囲内にいるかどうか
	bool CanCollectPosZ();

private:
	// 回収員のリスト
	std::vector<CCollector*> mpCollectors;
	// プレイヤーと敵の探知用
	CCollider* mpSearchCol;

	// 撤退の移動中かどうか
	bool mIsReturn;

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