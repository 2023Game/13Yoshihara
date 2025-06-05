#pragma once
#include "CEnemyBase.h"
#include "CSpellCaster.h"

// 敵の基底クラスと
// 呪文詠唱クラスを継承した敵クラス
class CEnemy : public CEnemyBase, public CSpellCaster
{
public:
	// コンストラクタ
	CEnemy();
	// デストラクタ
	~CEnemy();

	// 更新
	void Update() override;

private:
	// コライダーを生成
	void CreateCol() override;

	// 経過時間
	float mElapsedTime;
};