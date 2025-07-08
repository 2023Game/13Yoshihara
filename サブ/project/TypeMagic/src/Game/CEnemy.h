#pragma once
#include "CEnemyBase.h"
#include "CEnemyStatus.h"
#include "CCastSpellStr.h"
#include "CEnemyContext.h"

class CEnemyStateBase;

// 敵の基底クラスとステータスクラス
// を継承した敵クラス
class CEnemy : public CEnemyBase, public CEnemyStatus, public CCastSpellStr
{
	friend CEnemyStateBase;
public:
	// コンストラクタ
	CEnemy(ESpellElementalType elemental = ESpellElementalType::eNeutral);
	// デストラクタ
	~CEnemy();

	// 更新
	void Update() override;

	// メイン属性の文字列を設定
	void SetMainElementalStr(ESpellElementalType elemental);
	// メイン属性の文字列を取得
	std::string GetMainElementalStr() const;

	// 詠唱する形の文字列を設定
	void SetCastShapeStr(ESpellShapeType shape);
	// 詠唱する形の文字列を取得
	std::string GetCastShapeStr() const;

	// 状態内のステップを設定
	void SetStateStep(int step);
	// 状態内のステップを取得
	int GetStateStep() const;
	// 経過時間を設定
	void SetElapsedTime(float time);
	// 経過時間に加算
	void AddElapsedTime(float addTime);
	// 経過時間を取得
	float GetElapsedTime() const;

	// 呪文が飛んできているかを設定
	void SetSpellComing(bool enable, ESpellShapeType shape = ESpellShapeType::eError, float score = 0.0f, CVector moveDir = CVector::zero, float dist = 0.0f);
	// 飛んできている呪文の移動方向を取得
	CVector GetComingSpellMoveDir() const;

	// 最適な行動に変更する
	void ChangeBestState();

	// 状況を取得
	CEnemyContext::EnemyContext GetContext();

private:
	// コライダーを生成
	void CreateCol() override;

	// 死亡処理
	void Death() override;

	// 状態切り替え
	void ChangeState(CEnemyStateBase* state);
	CEnemyStateBase* mState;	// 敵の状態
	// 状態内のステップ管理用
	int mStateStep;
	// 経過時間
	float mElapsedTime;

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(CEnemyStateBase* state) const;
#endif

	// メイン属性の文字列
	std::string mMainElementalStr;
	// 詠唱する形の文字列
	std::string mCastShapeStr;

	// 飛んでくる呪文探知用
	CCollider* mpSpellSearch;
	// 呪文が飛んできているか
	bool mIsSpellComing;
	// 飛んできている呪文の形
	ESpellShapeType mComingSpellShape;
	// 呪文の回避優先度スコア
	float mPriorityScore;
	// 呪文の移動方向
	CVector mSpellMoveDir;
	// 飛んできている呪文との距離
	float mSpellDist;

	// MPゲージ
	CGaugeUI3D* mpMpGauge;
};