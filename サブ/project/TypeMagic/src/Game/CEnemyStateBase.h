#pragma once

class CEnemy;

// 敵の状態の基底クラス
class CEnemyStateBase
{
public:
	// デストラクタ
	virtual ~CEnemyStateBase() = default;
	// 開始時の処理
	virtual void Start(CEnemy* enemy);
	// 更新中の処理
	virtual void Update(CEnemy* enemy) = 0;
	// 終了時の処理
	virtual void End(CEnemy* enemy) = 0;
	// 名前を取得
	virtual std::string GetName() const = 0;
	// 呪文詠唱の処理
	void Cast(CEnemy* enemy, bool atkSpell);
};