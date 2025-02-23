#pragma once
#include "CPlayerBase.h"
#include "CDeliveryPlayerStatus.h"
#include "RoadType.h"

class CDeliveryHpUI2D;
class CSound;

/*
配達ゲームのプレイヤークラス
*/
class CDeliveryPlayer : public CPlayerBase , public CDeliveryPlayerStatus
{
public:
	// コンストラクタ
	CDeliveryPlayer();
	// デストラクタ
	~CDeliveryPlayer();

	// オブジェクト削除処理
	void DeleteObject(CObjectBase* obj) override;

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
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);// override;

	// 描画
	void Render();

	// 配達した数を1増やす
	void IncreaseDeliveryNum();
	// 配達した数を取得する
	int GetDeliveryNum() const;
	// 壊したトラックの数を1増やす
	void IncreaseDestroyEnemyNum();
	// 壊したトラックの数を取得する
	int GetDestroyEnemyNum() const;
	// 発射した数を1増やす
	void IncreaseShotNum();
	// 発射した数を取得する
	int GetShotNum() const;
	// 当たった数を1増やす
	void IncreaseHitNum();
	// 当たった数を取得する
	int GetHitNum() const;

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
	EState mState;	// プレイヤーの状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用
	float mInvincibleTime;		// 無敵時間計測用
	float mHitBlinkTime;		// 点滅間隔計測用
	float mLeftShootTime;		// 左に撃てる間隔計測用
	float mRightShootTime;		// 右に撃てる間隔計測用
	float mBackShootTime;		// 後に撃てる間隔計測用

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
	// アクションのキー入力
	void ActionInput();
	// ダメージの点滅と無敵時間の処理
	void HitBlink();

	// 3dモデル
	CModel* mpModel;
	// HpUI
	CDeliveryHpUI2D* mpHpUI;

	// 車線変更の目的地
	CVector mTargetPos;
	// 目的地の車道の種類
	ERoadType mTargetRoadType;
	// 左移動か
	bool mIsLeftMove;

	// 現在の車道
	ERoadType mRoadType;
	
	/// <summary>
	/// 車線変更先の座標を求める
	/// </summary>
	/// <param name="isLeft">左か</param>
	/// <returns></returns>
	CVector GetTargetPos(bool isLeft);

	// 配達した数
	int mDeliveryNum;
	// 敵を破壊した数
	int mDestroyEnemyNum;
	// 発射した数
	int mShotNum;
	// 当たった数
	int mHitNum;

	// 車同士の衝突音
	CSound* mpClashSE;
};