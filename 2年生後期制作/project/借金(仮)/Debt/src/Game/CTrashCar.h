#pragma once
#include "CTrashVehicleBase.h"
#include "CVehicleStatus.h"

class CModel;

// 車のクラス
class CTrashCar : public CTrashVehicleBase, public CVehicleStatus
{
public:
	// コンストラクタ
	CTrashCar(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints);
	// デストラクタ
	~CTrashCar();

	/// <summary>
	/// ダメージを受ける
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	/// <param name="causer">攻撃してきた相手</param>
	void TakeDamage(int damage, CObjectBase* causer) override;
	/// <summary>
	/// クリティカルダメージを受ける
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	/// <param name="causer">攻撃してきた相手</param>
	void TakeCritical(int damage, CObjectBase* causer) override;

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
	// 死亡(壊れた)
	void Death() override;

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