#pragma once
#include "CEnemyBase.h"
#include "CEnemyStatus.h"
#include "CCastSpellStr.h"

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

	// メイン属性を設定
	void SetMainElemental(ESpellElementalType elemental);
	// メイン属性を取得
	ESpellElementalType GetMainElemental() const;

	// 詠唱する形を取得
	ESpellShapeType GetCastShape() const;

	// 状態内のステップを設定
	void SetStateStep(int step);
	// 状態内のステップを取得
	int GetStateStep() const;
	// 経過時間を設定
	void SetElapsedTime(float time);
	// 経過時間を取得
	float GetElapsedTime() const;

private:
	// コライダーを生成
	void CreateCol() override;

	// 最適な行動に変更する
	void ChangeBestState();

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

	// メイン属性
	ESpellElementalType mMainElemental;
	// 詠唱する形
	ESpellShapeType mCastShape;
};