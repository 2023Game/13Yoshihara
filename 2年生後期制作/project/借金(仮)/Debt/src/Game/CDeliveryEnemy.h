#pragma once
#include "CEnemyBase.h"
#include "CDeliveryEnemyStatus.h"
#include "RoadType.h"

/*
配達ゲームの敵クラス
敵基底クラスを継承
*/
class CDeliveryEnemy : public CEnemyBase, public CDeliveryEnemyStatus
{
public:
	// コンストラクタ
	CDeliveryEnemy();
	// デストラクタ
	~CDeliveryEnemy();

	/// <summary>
	/// ダメージを受ける
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	/// <param name="causer">攻撃してきた相手</param>
	/// <param name="causer">自分の射撃による減少か</param>
	void TakeDamage(int damage, CObjectBase* causer, bool isShot = false);

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
	void Render();

	// 有効無効を切り替える
	void SetOnOff(bool setOnOff);
	// 現在の車道を設定
	void SetRoadType(ERoadType roadType);
	// 現在の車道を取得
	ERoadType GetRoadType() const;

private:
	// 状態
	enum class EState
	{
		eMove,			// 移動
		eChangeRoad,	// 車線変更
		eDeath,			// 死亡
	};
	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// 敵の状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用
	float mInvincibleTime;		// 無敵時間計測用
	float mHitFlashTime;		// 点滅間隔計測用

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
#endif
	// 移動の更新処理
	void UpdateMove();
	// 車線変更の更新処理
	void UpdateChangeRoad();
	// 死亡の更新処理
	void UpdateDeath();
	// 死亡
	void Death() override;

	// コライダ―を生成
	void CreateCol();
	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	// ダメージの点滅と無敵時間の処理
	void HitFlash();

	// 3dモデル
	CModel* mpModel;

	// 車線変更の目的地
	CVector mTargetPos;
	// 目的地の車道の種類
	ERoadType mTargetRoadType;

	// 現在の車道
	ERoadType mRoadType;
};