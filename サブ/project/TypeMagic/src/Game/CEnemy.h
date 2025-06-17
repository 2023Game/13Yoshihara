#pragma once
#include "CEnemyBase.h"
#include "CCastSpellStr.h"

// 敵の基底クラスと
// 呪文詠唱クラスを継承した敵クラス
class CEnemy : public CEnemyBase, public CCastSpellStr
{
public:
	// コンストラクタ
	CEnemy();
	// デストラクタ
	~CEnemy();

	// 更新
	void Update() override;

	// メイン属性を設定
	void SetMainElemental(ESpellElementalType elemental);

private:
	// コライダーを生成
	void CreateCol() override;

	// 敵の状態
	enum class EState
	{
		eIdle,	// 待機
		eCast,	// 詠唱中
		eChase,	// 追いかける
		eDodge,	// 回避中
		eRun,	// 逃げる
	};

	// 待機状態
	void UpdateIdle();
	// 移動処理
	void UpdateMove();
	// 詠唱中
	void UpdateCast();
	// 追いかける
	void UpdateChase();
	// 回避中
	void UpdateDodge();
	// 逃げる
	void UpdateRun();

	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// 敵の状態
	int mStateStep;	// 状態内のステップ管理用
	// 経過時間
	float mElapsedTime;

	// メイン属性
	ESpellElementalType mMainElement;
};