#pragma once
#include "CEnemyBase.h"
#include "CResidentStatus.h"

/*
ゴミ拾いゲームの住人クラス
敵基底クラスと
住人のステータスクラスを継承
*/
class CResident : public CEnemyBase, public CResidentStatus
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">住人のモデル</param>
	/// <param name="patrolPoints">生成時の座標</param>
	/// <param name="patrolPoints">巡回ポイントのリスト</param>
	CResident(CModelX* model, CVector startPos,
		std::vector<CNavNode*> patrolPoints);
	// デストラクタ
	~CResident();

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render() override;
private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eMove,		// 移動
		eThrowBag,	// ゴミ袋を投げる


		Num
	};

	// 敵の状態
	enum class EState
	{
		eIdle,		// 待機
		ePatrol,	// 巡回
		eThrowBag,	// ゴミ袋を投げる
	};

	// 待機状態
	void UpdateIdle();
	// 巡回処理
	void UpdatePatrol();
	// ゴミ袋を投げる処理
	void UpdateThrowBag();

	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// 敵の状態
	int mStateStep;	// 状態内のステップ数
	float mElapsedTime;	// 経過時間

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
	// 状態の色を取得
	CColor GetStateColor(EState state) const;
#endif

	// 移動を中断したかどうか
	bool mIsMovePause;
};